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
#include "kern/arm.h"
#include "kern/vmm.h"
#include "kern/mmu.h"
#include "vfs/vfs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include "kern/pinfo.h"
#include "kern/ipc.h"

extern int ipc_task_init(task_t *task);
extern int ipc_task_cleanup(task_t *task);
extern int signal_task_init(task_t *task);
extern int signal_task_cleanup(task_t *task);
extern int signal_schedule(task_t *task, signal_ctx_t *octx);
extern void signal_switch(task_t *task);
/*********************************************************************************************************
** Function name:           kidle_create
** Descriptions:            创建空闲进程
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kidle_create(void)
{
    task_t *task;

    /*
     * 初始化进程控制块 0
     */
    task                = &tasks[0];

    task->pid           = 0;                                            /*  PID                         */
    task->tid           = 0;                                            /*  TID                         */
    task->status        = TASK_RUNNING;                                 /*  就绪态                      */
    task->timeslice     = 0;                                            /*  时间片                      */
    task->delay         = 0;                                            /*  延时                        */
    task->priority      = 0;                                            /*  优先级                      */
    task->type          = TASK_TYPE_PROCESS;                            /*  任务类型为进程              */
    task->resume_type   = TASK_RESUME_UNKNOW;                           /*  恢复类型                    */
    task->ticks         = 0;                                            /*  被定时器中断次数            */
    task->cpu_usage     = 0;                                            /*  CPU 占用率                  */
    task->dabt_cnt      = 0;                                            /*  数据访问终止次数            */

    /*
     * 内核线程专属信息
     */
    task->stack_base    = 0;                                            /*  线程栈基址                  */
    task->stack_size    = 0;                                            /*  线程栈大小                  */
    task->thread        = NULL;                                         /*  线程函数                    */
    task->arg           = NULL;                                         /*  线程参数                    */

    task->reent         = &reents[0];                                   /*  可重入结构指针              */
    _REENT_INIT_PTR(task->reent);                                       /*  初始化可重入结构            */

    /*
     * 进程专属信息
     */
    task->frame_nr      = 0;                                            /*  页框数                      */
    task->page_tbl_nr   = 0;                                            /*  页表数                      */
    task->frame_list    = NULL;                                         /*  页框链表                    */
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));              /*  一级段表备份                */

    task->pinfo         = NULL;                                         /*  进程信息指针                */
    task->file_size     = 0;                                            /*  进程二进制文件大小          */

    /*
     * 初始化进程上下文
     */
    task->regs[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];         /*  SVC 模式的 SP (满堆栈递减)  */
    task->regs[1]   = ARM_SYS_MODE | ARM_FIQ_EN | ARM_IRQ_EN;           /*  CPSR, SYS 模式              */
    task->regs[2]   = PROCESS0_STACK_BASE;                              /*  SYS 模式的 SP (满堆栈递减)  */
    task->regs[3]   = ARM_SVC_MODE;                                     /*  SPSR, SVC 模式              */
    task->regs[4]   = 0;                                                /*  LR                          */
    task->regs[5]   = 0;                                                /*  R0 ~ R12                    */
    task->regs[6]   = 1;
    task->regs[7]   = 2;
    task->regs[8]   = 3;
    task->regs[9]   = 4;
    task->regs[10]  = 5;
    task->regs[11]  = 6;
    task->regs[12]  = 7;
    task->regs[13]  = 8;
    task->regs[14]  = 9;
    task->regs[15]  = 10;
    task->regs[16]  = 11;
    task->regs[17]  = 12;
    task->regs[18]  = 0;                                                /*  PC                          */

    strcpy(task->name, "idle");                                         /*  名字                        */

    ipc_task_init(task);                                                /*  初始化任务的 IPC            */

    signal_task_init(task);                                             /*  初始化任务的信号系统        */

    vfs_task_init(0);                                                   /*  初始化任务的文件信息        */
}
/*********************************************************************************************************
** Function name:           kthread_shell
** Descriptions:            内核线程外壳
** input parameters:        task                任务控制块
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void kthread_shell(task_t *task)
{
    open("/dev/null", O_RDONLY);                                        /*  打开三个标准文件            */
    stdin  = fdopen(STDIN_FILENO,  "r");

    open("/dev/null", O_WRONLY);
    stdout = fdopen(STDOUT_FILENO, "w");

    open("/dev/null", O_WRONLY);
    stderr = fdopen(STDERR_FILENO, "w");

    putenv("PATH=/");                                                   /*  设置环境变量                */
    putenv("HOME=/");

    task->thread(task->arg);                                            /*  进入真正的内核线程函数      */

    _exit(0);                                                           /*  退出内核线程                */
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
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stack_size, uint32_t priority)
{
    int32_t  tid;
    task_t  *task;
    uint32_t reg;

    if (func == NULL) {
        return -1;
    }

    if (stack_size < 1 * KB) {                                          /*  栈空间最小为 1 KB           */
        stack_size = 1 * KB;
    } else {
        stack_size = MEM_ALIGN_SIZE(stack_size);                        /*  对齐栈空间大小              */
    }

    reg = interrupt_disable();

    /*
     * 分配内核线程控制块
     */
                                                                        /*  遍历所有的内核线程控制块    */
    for (tid = PROCESS_NR, task = tasks + PROCESS_NR; tid < TASK_NR; tid++, task++) {
        if (task->status == TASK_UNALLOCATE) {                          /*  如果内核线程控制块无效      */
            break;
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
    task->type          = TASK_TYPE_KTHREAD;                            /*  任务类型为进程              */
    task->resume_type   = TASK_RESUME_UNKNOW;                           /*  恢复类型                    */
    task->ticks         = 0;                                            /*  被定时器中断次数            */
    task->cpu_usage     = 0;                                            /*  CPU 占用率                  */
    task->dabt_cnt      = 0;                                            /*  数据访问终止次数            */

    /*
     * 内核线程专属信息
     */
    task->stack_base    = (uint32_t)kmalloc(stack_size);                /*  分配线程栈空间              */
    if (task->stack_base == 0) {
        goto __exit0;
    }
    task->stack_size    = stack_size;                                   /*  线程栈大小                  */
    task->thread        = func;                                         /*  线程函数                    */
    task->arg           = arg;                                          /*  线程参数                    */

    task->reent         = &reents[tid - PROCESS_NR + 1];                /*  可重入结构指针              */
    _REENT_INIT_PTR(task->reent);                                       /*  初始化可重入结构            */

    /*
     * 进程专属信息
     */
    task->frame_nr      = 0;                                            /*  页框数                      */
    task->page_tbl_nr   = 0;                                            /*  页表数                      */
    task->frame_list    = NULL;                                         /*  页框链表                    */
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));              /*  一级段表备份                */

    task->pinfo         = NULL;                                         /*  进程信息指针                */
    task->file_size     = 0;                                            /*  进程二进制文件大小          */

    /*
     * 初始化内核线程上下文
     */
    task->regs[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];         /*  SVC 模式的 SP (满堆栈递减)  */
    task->regs[1]   = ARM_SYS_MODE | ARM_FIQ_EN | ARM_IRQ_EN;           /*  CPSR, SYS 模式              */
    task->regs[2]   = task->stack_base + stack_size;                    /*  SYS 模式的 SP (满堆栈递减)  */
    task->regs[3]   = ARM_SVC_MODE;                                     /*  SPSR, SVC 模式              */
    task->regs[4]   = (uint32_t)kthread_shell;                          /*  LR                          */
    task->regs[5]   = (uint32_t)task;                                   /*  R0 ~ R12                    */
    task->regs[6]   = 1;
    task->regs[7]   = 2;
    task->regs[8]   = 3;
    task->regs[9]   = 4;
    task->regs[10]  = 5;
    task->regs[11]  = 6;
    task->regs[12]  = 7;
    task->regs[13]  = 8;
    task->regs[14]  = 9;
    task->regs[15]  = 10;
    task->regs[16]  = 11;
    task->regs[17]  = 12;
    task->regs[18]  = (uint32_t)kthread_shell;                          /*  PC                          */

    if (name != NULL) {                                                 /*  名字                        */
        strlcpy(task->name, name, sizeof(task->name));
    } else {
        strcpy(task->name, "???");
    }

    if (ipc_task_init(task) < 0) {                                      /*  初始化任务的 IPC            */
        goto __exit0;
    }

    if (vfs_task_init(tid) < 0) {                                       /*  初始化任务的文件信息        */
        goto __exit1;
    }

    if (signal_task_init(task) < 0) {                                   /*  初始化任务的信号系统        */
        goto __exit2;
    }

    if (running) {                                                      /*  如果内核已经启动            */
        schedule();                                                     /*  重新调度                    */
    }

    interrupt_resume(reg);

    return tid;

    __exit2:
    vfs_task_cleanup(tid);

    __exit1:
    ipc_task_cleanup(task);

    __exit0:
    task->status = TASK_UNALLOCATE;
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           process_arg_init
** Descriptions:            初始化进程的参数
** input parameters:        task                进程控制块
**                          argc                参数个数
**                          argv                参数指针数组
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int process_arg_init(task_t *task, int argc, char **argv)
{
    /*
     * 获得进程地址空间中参数储存区的基址(MVA), 参数储存区放在进程地址空间的最后
     */
    uint32_t *ptr = (uint32_t *)((task->pid + 1) * PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE);

    /*
     * 参数储存区是这样利用的:
     * 参数个数 argc, 参数指针数组 argv[argc], NULL, 参数 arg0, 参数 arg1, 参数 arg2 ...
     * 下面的 arg 为参数 arg0 的储存地址
     */
    char     *arg = (char *)(ptr + 1 + argc + 1);
    int       i;

    *ptr++ = argc;

    for (i = 0; i < argc; i++) {
        strcpy(arg, argv[i]);
        *ptr++ = (uint32_t)arg % PROCESS_SPACE_SIZE;
        arg   += strlen(argv[i]) + 1;
    }
    *ptr++ = 0;

    return 0;
}
/*********************************************************************************************************
** Function name:           process_code_copy
** Descriptions:            拷贝进程代码
** input parameters:        task                进程控制块
**                          path                进程二进制文件路径
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int process_code_copy(task_t *task, const char *path)
{
    int         i;
    int         fd;
    ssize_t     len;
    size_t      total;

    /*
     * 拷贝代码到进程的虚拟地址空间
     */
    fd = open(path, O_RDONLY, 0666);
    if (fd < 0) {
        return -1;
    }

    total = 0;

    while (task->file_size - total > 0) {
        len = read(fd, (char *)(task->pid * PROCESS_SPACE_SIZE) + total, task->file_size - total);
        if (len < 0) {
            close(fd);
            return -1;
        }
        total += len;
    }
    close(fd);

    /*
     * 刚才拷贝的代码还在 D-Cache, 进程运行时读取 I-Cache 或主存, 它们是不一致的!
     * 所以必须要清除 D-Cache, 清除写缓冲, 还要无效 I-Cache,
     * 无效 D-Cache 可让出 D-Cache, 减少其它进程的 D-Cache 被换出和 D-Cache Miss, 提升运行速度
     *
     * ARM 数据访问的基本流程图:
     * http://www.ibm.com/developerworks/cn/linux/l-skyeye/part3/s3/index.html
     */
    /*
     * 清除并无效 D-Cache
     */
    for (i = 0; i < (task->file_size + 31) / 32; i++) {
        mmu_clean_invalidate_dcache_mva(task->pid * PROCESS_SPACE_SIZE + i * 32);
    }

    /*
     * 清除写缓冲
     */
    mmu_drain_write_buffer();

    /*
     * 无效 I-Cache
     */
    for (i = 0; i < (task->file_size + 31) / 32; i++) {
        mmu_invalidate_icache_mva(task->pid * PROCESS_SPACE_SIZE + i * 32);
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           process_create
** Descriptions:            创建进程
** input parameters:        path                进程二进制文件路径
**                          priority            进程时间片
**                          argc                参数个数
**                          argv                参数数组
** output parameters:       NONE
** Returned value:          进程 PID
*********************************************************************************************************/
int32_t process_create(const char *path, uint32_t priority, int argc, char **argv)
{
    int32_t     pid;
    task_t     *task;
    uint32_t    reg;
    struct stat st;

    if (path == NULL) {                                                 /*  看下 PATH 为 NULL 否        */
        return -1;
    }

    if (stat(path, &st) < 0) {                                          /*  看下文件存在否              */
        return -1;
    }

    if (!S_ISREG(st.st_mode)) {                                         /*  看下文件是普通文件否        */
        return -1;
    }

    if (priority < 5) {                                                 /*  优先级最小为 5              */
        priority = 5;
    }

    reg = interrupt_disable();

    /*
     * 分配进程控制块
     */
    for (pid = 1, task = tasks + 1; pid < PROCESS_NR; pid++, task++) {  /*  遍历所有的进程控制块        */
        if (task->status == TASK_UNALLOCATE) {                          /*  如果进程控制块无效          */
                                                                        /*  如果进程的虚拟地址空间可用  */
            if (vspace_usable(pid * PROCESS_SPACE_SIZE, PROCESS_SPACE_SIZE)) {
                break;
            }
        }
    }

    if (pid == PROCESS_NR) {                                            /*  没有空闲的进程控制块        */
        interrupt_resume(reg);
        return -1;
    }

    /*
     * 初始化进程控制块
     */
    task->pid           = pid;                                          /*  PID                         */
    task->tid           = pid;                                          /*  TID                         */
    task->status        = TASK_SUSPEND;                                 /*  就绪态                      */
    task->timeslice     = priority;                                     /*  时间片                      */
    task->delay         = 0;                                            /*  延时                        */
    task->priority      = priority;                                     /*  优先级                      */
    task->type          = TASK_TYPE_PROCESS;                            /*  任务类型为进程              */
    task->resume_type   = TASK_RESUME_UNKNOW;                           /*  恢复类型                    */
    task->ticks         = 0;                                            /*  被定时器中断次数            */
    task->cpu_usage     = 0;                                            /*  CPU 占用率                  */
    task->dabt_cnt      = 0;                                            /*  数据访问终止次数            */

    /*
     * 内核线程专属信息
     */
    task->stack_base    = 0;                                            /*  线程栈基址                  */
    task->stack_size    = 0;                                            /*  线程栈大小                  */
    task->thread        = NULL;                                         /*  线程函数                    */
    task->arg           = NULL;                                         /*  线程参数                    */

    task->reent         = NULL;                                         /*  可重入结构指针              */

    /*
     * 进程专属信息
     */
    task->frame_nr      = 0;                                            /*  页框数                      */
    task->page_tbl_nr   = 0;                                            /*  页表数                      */
    task->frame_list    = NULL;                                         /*  页框链表                    */
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));              /*  一级段表备份                */

    task->pinfo         = NULL;                                         /*  进程信息指针                */
    task->file_size     = st.st_size;                                   /*  进程二进制文件大小          */

    /*
     * 初始化进程上下文
     */
    task->regs[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];         /*  SVC 模式的 SP (满堆栈递减)  */
    task->regs[1]   = ARM_USR_MODE | ARM_FIQ_EN | ARM_IRQ_EN;           /*  CPSR, USR 模式              */
    task->regs[2]   = PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE;          /*  SYS 模式的 SP (满堆栈递减)  */
    task->regs[3]   = ARM_SVC_MODE;                                     /*  SPSR, SVC 模式              */
    task->regs[4]   = 0;                                                /*  LR                          */
    task->regs[5]   = 0;                                                /*  R0 ~ R12                    */
    task->regs[6]   = 1;
    task->regs[7]   = 2;
    task->regs[8]   = 3;
    task->regs[9]   = 4;
    task->regs[10]  = 5;
    task->regs[11]  = 6;
    task->regs[12]  = 7;
    task->regs[13]  = 8;
    task->regs[14]  = 9;
    task->regs[15]  = 10;
    task->regs[16]  = 11;
    task->regs[17]  = 12;
    task->regs[18]  = 0;                                                /*  PC                          */

    if (path != NULL) {
        const char *name = memrchr(path, '/', strlen(path));
        if (name != NULL) {
            strlcpy(task->name, name + 1, sizeof(task->name));
        } else {
            strlcpy(task->name, path, sizeof(task->name));
        }
    } else {
        strcpy(task->name, "???");
    }

    if (ipc_task_init(task) < 0) {                                      /*  初始化任务的 IPC            */
        goto __exit0;
    }

    if (vfs_task_init(pid) < 0) {                                       /*  初始化任务的文件信息        */
        goto __exit1;
    }

    if (signal_task_init(task) < 0) {                                   /*  初始化任务的信号系统        */
        goto __exit2;
    }

    if (vmm_process_init(task) < 0) {                                   /*  初始化进程的虚拟内存信息    */
        goto __exit3;
    }

    if (process_arg_init(task, argc, argv) < 0) {                       /*  初始化进程的参数            */
        goto __exit4;
    }

    interrupt_resume(reg);

    if (process_code_copy(task, path) < 0) {                            /*  拷贝进程代码                */
        goto __exit5;
    }

    reg = interrupt_disable();

    task->status = TASK_RUNNING;                                        /*  进程进入就绪态              */

    interrupt_resume(reg);

    return pid;

    __exit5:
    __exit4:
    vmm_process_cleanup(task);

    __exit3:
    signal_task_cleanup(task);

    __exit2:
    vfs_task_cleanup(pid);

    __exit1:
    ipc_task_cleanup(task);

    __exit0:
    task->status = TASK_UNALLOCATE;
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           task_cleanup
** Descriptions:            清理当前任务
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void task_cleanup(task_t *task)
{
    uint32_t reg;

    reg  = interrupt_disable();

    ipc_task_cleanup(task);                                             /*  清理任务的 IPC              */

    vfs_task_cleanup(task->tid);                                        /*  清理任务的文件信息          */

    signal_task_cleanup(task);                                          /*  清理任务的信号系统          */

    if (task->type == TASK_TYPE_PROCESS) {                              /*  如果任务是进程              */
        vmm_process_cleanup(task);                                      /*  清理进程的虚拟内存信息      */
    } else {                                                            /*  如果任务是内核线程          */
        kfree((void *)task->stack_base);                                /*  释放内核线程的栈空间        */
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
    signal_switch(current);                                             /*  信号处理函数切换            */

    _impure_ptr = current->reent;                                       /*  改写 _impure_ptr 指针       */
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
    uint32_t    reg;

    reg = interrupt_disable();

    if (!running) {                                                     /*  如果内核还没启动            */
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
            if ((task->status == TASK_RUNNING || signal_schedule(task, NULL))
                    && (max < (int32_t)task->priority)) {
                max  = (int32_t)task->priority;                         /*  用优先级来做竞争            */
                next = task;
            }

            if (task->signal_ctx != NULL) {
                task->signal_ctx->ref++;
            }
        }

        if (max > 0) {                                                  /*  找到了一个就绪的内核线程    */
            break;
        }

        for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {    /*  进程 0 不参与竞争           */
            if ((task->status == TASK_RUNNING || signal_schedule(task, NULL))
                    && (max < (int32_t)task->timeslice)) {
                max  = (int32_t)task->timeslice;                        /*  用剩余时间片来做竞争        */
                next = task;
            }

            if (task->signal_ctx != NULL) {
                task->signal_ctx->ref++;
            }
        }

        if (max > 0) {                                                  /*  找到了一个有剩余时间片的进程*/
            break;
        } else if (flag) {                                              /*  如果没有一个任务就绪        */
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
        if (task->signal_ctx == NULL) {                                 /*  也无需切换到信号上下文      */
            interrupt_resume(reg);
            return;                                                     /*  直接返回                    */
        }
    } else {
        current = next;                                                 /*  改写 current 指针           */
    }

    if (next->signal_ctx != NULL) {                                     /*  需要切换到信号上下文        */
        next->signal_ctx->ref--;
        if (task->signal_current == NULL) {
            task->signal_status = task->status;
            task->signal_delay  = task->delay;
            task->signal_resume = task->resume_type;
            task->delay         = 0;
            task->status        = TASK_RUNNING;
            task->resume_type   = TASK_RESUME_UNKNOW;
            ipc_task_cleanup(task);
        }
    }

    /*
     * 任务切换
     */
    __task_switch:
    if (task->status == TASK_UNALLOCATE) {
        extern void task_switch_to(register task_t *from, register task_t *to);
        task_switch_to(task, next);
    } else {
        extern void task_switch(register task_t *from, register task_t *to);
        task_switch(task, next);
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
int task_sleep(uint32_t ticks)
{
    uint32_t reg;

    reg = interrupt_disable();

    current->delay = ticks == 0 ? 1 : ticks;                            /*  休睡 TICK 数                */

    current->status = TASK_SLEEPING;                                    /*  当前任务进入休睡态          */

    current->resume_type = TASK_RESUME_UNKNOW;                          /*  设置恢复类型为未知          */

    schedule();                                                         /*  任务调度                    */

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           task_pause
** Descriptions:            暂停当前任务
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int task_pause(void)
{
    uint32_t reg;

    reg  = interrupt_disable();

    current->delay = 0;

    current->status = TASK_SUSPEND;

    current->resume_type = TASK_RESUME_UNKNOW;

    schedule();                                                         /*  任务调度                    */

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           task_setpinfo
** Descriptions:            设置当前任务的进程信息
** input parameters:        info                进程信息指针
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int task_setpinfo(pinfo_t *info)
{
    uint32_t reg;

    if (info == NULL) {
        return -1;
    }

    reg = interrupt_disable();

    _impure_ptr = current->reent = va_to_mva(info->reent);

    current->pinfo = info;

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
    uint32_t reg;

    reg = interrupt_disable();

    ret = current->pid;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
