/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <jiaojinxing1987@gmail.com>
**
** Licensed under the Academic Free License version 2.1
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**--------------------------------------------------------------------------------------------------------
** File name:               task.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            任务
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/kvars.h"
#include "kern/func_config.h"

#include <string.h>
#include <stdlib.h>

#if CONFIG_VFS_EN > 0
#include "vfs/vfs.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#endif

extern int ipc_task_init(task_t *task);
extern int ipc_task_cleanup(task_t *task);

/*********************************************************************************************************
** Function name:           kthread_shell
** Descriptions:            内核线程外壳
** input parameters:        task                任务控制块
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void kthread_shell(task_t *task)
{
#if CONFIG_VFS_EN > 0
    int fd = open("/dev/serial0", O_RDONLY);                            /*  打开三个标准文件            */
    stdin  = fdopen(fd,  "r");

    open("/dev/serial0", O_WRONLY);
    stdout = fdopen(fd, "w");

    open("/dev/serial0", O_WRONLY);
    stderr = fdopen(fd, "w");

    putenv("PATH=/");                                                   /*  设置环境变量                */
    putenv("HOME=/");
#endif

    task->thread(task->arg);                                            /*  进入真正的内核线程函数      */

    _exit(0);                                                           /*  退出内核线程                */
}
/*********************************************************************************************************
** Function name:           __kthread_create
** Descriptions:            创建内核线程
** input parameters:        name                内核线程名字
**                          func                内核线程进入点函数
**                          arg                 内核线程参数
**                          stack_size          内核线程栈空间大小
**                          priority            内核线程优先级
**                          idle                是否为空闲进程
** output parameters:       NONE
** Returned value:          内核线程 TID
*********************************************************************************************************/
static int32_t __kthread_create(const char *name,
                                void (*func)(void *),
                                void *arg,
                                size_t stack_size,
                                uint8_t priority,
                                bool_t idle)
{
    int32_t  tid;
    task_t  *task;
    reg_t    reg;

    if (func == NULL) {
        return -1;
    }

    if (stack_size < 1 * KB) {                                          /*  栈空间最小为 1 KB           */
        stack_size = 1 * KB;
    }

    stack_size = MEM_ALIGN_SIZE(stack_size);                            /*  对齐栈空间大小              */

    reg = interrupt_disable();

    /*
     * 分配内核线程控制块
     */
    if (idle) {
        tid  = 0;
        task = tasks;
    } else {
        /*
         * 遍历所有的内核线程控制块
         */
        for (tid = PROCESS_NR, task = tasks + PROCESS_NR; tid < TASK_NR; tid++, task++) {
            if (task->status == TASK_UNALLOCATE) {                      /*  如果内核线程控制块未分配    */
                break;
            }
        }
    }

    if (tid == TASK_NR) {                                               /*  没有空闲的内核线程控制块    */
        interrupt_resume(reg);
        return -1;
    }

    /*
     * 初始化内核线程控制块
     */
    task->pid           = 0;                                            /*  PID                         */
    task->tid           = tid;                                          /*  TID                         */
    task->status        = TASK_RUNNING;                                 /*  就绪态                      */
    task->timeslice     = priority;                                     /*  时间片                      */
    task->delay         = 0;                                            /*  延时                        */
    task->priority      = priority;                                     /*  优先级                      */
    task->type          = TASK_TYPE_KTHREAD;                            /*  任务类型                    */
    task->resume_type   = TASK_RESUME_UNKNOW;                           /*  恢复类型                    */

    /*
     * 内核线程专属信息
     */
    task->stack_base    = (void *)kmalloc(stack_size, GFP_KERNEL);      /*  分配线程栈空间              */
    if (task->stack_base == NULL) {
        goto __exit0;
    }
    task->stack_size    = stack_size;                                   /*  线程栈大小                  */
    task->thread        = func;                                         /*  线程函数                    */
    task->arg           = arg;                                          /*  线程参数                    */

    task->reent         = idle ? reents : &reents[tid - PROCESS_NR + 1];/*  可重入结构指针              */

    _REENT_INIT_PTR(task->reent);                                       /*  初始化可重入结构            */

    /*
     * 进程专属信息
     */
    task->pinfo         = NULL;                                         /*  进程信息指针                */
    task->vmm           = NULL;                                         /*  VMM 信息                    */

    if (name != NULL) {                                                 /*  名字                        */
        strlcpy(task->name, name, sizeof(task->name));
    } else {
        strcpy(task->name, "???");
    }

    if (arch_build_context(task, kthread_shell) < 0) {                  /*  构建内核线程上下文          */
        goto __exit0;
    }

    if (ipc_task_init(task) < 0) {                                      /*  初始化任务的 IPC            */
        goto __exit1;
    }

    if (vfs_process_init(task->pid, tid, 100) < 0) {               /*  初始化任务的文件信息        */
        goto __exit2;
    }

    if (os_started) {                                                   /*  如果内核已经启动            */
        schedule();                                                     /*  重新调度                    */
    }

    interrupt_resume(reg);

    return tid;

    __exit2:


    __exit1:
    ipc_task_cleanup(task);

    __exit0:
    task->status = TASK_UNALLOCATE;
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           idle_thread
** Descriptions:            空闲线程
** input parameters:        arg                 参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void idle_thread(void *arg)
{
    soc_timer_init();

    while (1) {

    }
}
/*********************************************************************************************************
** Function name:           idle_process
** Descriptions:            创建空闲线程
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kidle_create(void)
{
    __kthread_create("idle", idle_thread, NULL, 0, 0, TRUE);
}
/*********************************************************************************************************
** Function name:           kthread_create
** Descriptions:            创建内核线程
** input parameters:        name                内核线程名字
**                          func                内核线程进入点函数
**                          arg                 内核线程参数
**                          stack_size          内核线程栈空间大小
**                          priority            内核线程优先级
** output parameters:       NONE
** Returned value:          内核线程 TID
*********************************************************************************************************/
int32_t kthread_create(const char *name,
                       void (*func)(void *),
                       void *arg,
                       size_t stack_size,
                       uint8_t priority)
{
    return __kthread_create(name, func, arg, stack_size, priority, FALSE);
}
/*********************************************************************************************************
** Function name:           task_cleanup
** Descriptions:            清理当前任务
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void task_cleanup(void)
{
    reg_t reg;
    task_t *task;

    reg  = interrupt_disable();

    task = current;

    ipc_task_cleanup(task);                                             /*  清理任务的 IPC              */

    vfs_process_cleanup(task->pid, task->tid);                          /*  清理任务的文件信息          */

    if (task->type == TASK_TYPE_PROCESS) {                              /*  如果任务是进程              */

    } else {                                                            /*  如果任务是内核线程          */
        kfree(task->stack_base);                                        /*  释放内核线程的栈空间        */
    }

    task->status = TASK_UNALLOCATE;                                     /*  释放任务的任务控制块        */

    schedule();                                                         /*  重新进行任务调度            */

    interrupt_resume(reg);
}
/*********************************************************************************************************
** Function name:           task_switch_hook
** Descriptions:            任务切换勾子函数
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void task_switch_hook(void)
{

}
/*********************************************************************************************************
** 下面供系统调用处理使用
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           task_schedule
** Descriptions:            任务调度
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void task_schedule(void)
{
    int         i;
    task_t     *next;
    int         flag;
    int32_t     max;
    task_t     *task;
    reg_t       reg;

    reg = interrupt_disable();

    if (!os_started) {                                                  /*  如果内核还没启动            */
        interrupt_resume(reg);
        return;                                                         /*  直接返回                    */
    }

    if (interrupt_nest > 0) {                                           /*  如果还没完全退出中断        */
        interrupt_resume(reg);
        return;                                                         /*  直接返回                    */
    }

    max  = -1;
    flag = FALSE;
    next = NULL;

    while (1) {
        /*
         * 先做内核线程调度, 再做进程调度
         */
        for (i = PROCESS_NR, task = tasks + PROCESS_NR; i < TASK_NR; i++, task++) {
            if ((task->status == TASK_RUNNING)
                    && (max < (int32_t)task->priority)) {
                max  = (int32_t)task->priority;                         /*  用优先级来做竞争            */
                next = task;
            }
        }

        if (max > 0) {                                                  /*  找到了一个就绪的内核线程    */
            break;
        }

#if CONFIG_PROCESS_EN > 0
        for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {    /*  进程 0 不参与竞争           */
            if ((task->status == TASK_RUNNING)
                    && (max < (int32_t)task->timeslice)) {
                max  = (int32_t)task->timeslice;                        /*  用剩余时间片来做竞争        */
                next = task;
            }
        }

        if (max > 0) {                                                  /*  找到了一个有剩余时间片的进程*/
            break;
        } else
#endif
        if (flag) {                                                     /*  如果没有一个任务就绪        */
            next = &tasks[0];                                           /*  则运行空闲进程              */
            break;
        }

        for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {    /*  重置所有就绪进程的剩余时间片*/
            if (task->status == TASK_RUNNING) {
                task->timeslice = task->priority;
            }
        }

        flag = TRUE;
    }

    task = current;
    if (task == next) {                                                 /*  如果不需要切换任务          */
        current->pid = 0;
        return;
    } else {
        current = next;                                                 /*  改写 current 指针           */

        if (current->pid != 0) {
            current->pid = 0;
        }

        _impure_ptr = current->reent;                                   /*  改写 _impure_ptr 指针       */
    }

    /*
     * 任务切换
     */
    if (task->status == TASK_UNALLOCATE) {
        arch_switch_context_to(task, next);
    } else {
        arch_switch_context(task, next);
    }

    interrupt_resume(reg);
}
/*********************************************************************************************************
** Function name:           task_sleep
** Descriptions:            任务休眠
** input parameters:        ticks               TICK 数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int task_sleep(tick_t ticks)
{
    reg_t reg;

    reg = interrupt_disable();

    current->delay = ticks == 0 ? 1 : ticks;                            /*  休睡 TICK 数                */

    current->status = TASK_SLEEPING;                                    /*  当前任务进入休睡态          */

    current->resume_type = TASK_RESUME_UNKNOW;                          /*  设置恢复类型为未知          */

    schedule();                                                         /*  任务调度                    */

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           task_getpid
** Descriptions:            获得当前任务的 PID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          当前任务的 PID
*********************************************************************************************************/
int32_t task_getpid(void)
{
    int32_t  ret;
    reg_t    reg;

    reg = interrupt_disable();

    ret = current->pid;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           gettid
** Descriptions:            获得当前任务的 TID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          当前任务的 TID
*********************************************************************************************************/
int32_t gettid(void)
{
    int32_t  ret;
    reg_t    reg;

    reg = interrupt_disable();

    ret = current->tid;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
