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
** Descriptions:            ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
** Version:                 1.0.0
** Descriptions:            �����ļ�
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
** Descriptions:            �ں��߳����
** input parameters:        task                ������ƿ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void kthread_shell(task_t *task)
{
#if CONFIG_VFS_EN > 0
    int fd = open("/dev/serial0", O_RDONLY);                            /*  ��������׼�ļ�            */
    stdin  = fdopen(fd,  "r");

    open("/dev/serial0", O_WRONLY);
    stdout = fdopen(fd, "w");

    open("/dev/serial0", O_WRONLY);
    stderr = fdopen(fd, "w");

    putenv("PATH=/");                                                   /*  ���û�������                */
    putenv("HOME=/");
#endif

    task->thread(task->arg);                                            /*  �����������ں��̺߳���      */

    _exit(0);                                                           /*  �˳��ں��߳�                */
}
/*********************************************************************************************************
** Function name:           __kthread_create
** Descriptions:            �����ں��߳�
** input parameters:        name                �ں��߳�����
**                          func                �ں��߳̽���㺯��
**                          arg                 �ں��̲߳���
**                          stack_size          �ں��߳�ջ�ռ��С
**                          priority            �ں��߳����ȼ�
**                          idle                �Ƿ�Ϊ���н���
** output parameters:       NONE
** Returned value:          �ں��߳� TID
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

    if (stack_size < 1 * KB) {                                          /*  ջ�ռ���СΪ 1 KB           */
        stack_size = 1 * KB;
    }

    stack_size = MEM_ALIGN_SIZE(stack_size);                            /*  ����ջ�ռ��С              */

    reg = interrupt_disable();

    /*
     * �����ں��߳̿��ƿ�
     */
    if (idle) {
        tid  = 0;
        task = tasks;
    } else {
        /*
         * �������е��ں��߳̿��ƿ�
         */
        for (tid = PROCESS_NR, task = tasks + PROCESS_NR; tid < TASK_NR; tid++, task++) {
            if (task->status == TASK_UNALLOCATE) {                      /*  ����ں��߳̿��ƿ�δ����    */
                break;
            }
        }
    }

    if (tid == TASK_NR) {                                               /*  û�п��е��ں��߳̿��ƿ�    */
        interrupt_resume(reg);
        return -1;
    }

    /*
     * ��ʼ���ں��߳̿��ƿ�
     */
    task->pid           = 0;                                            /*  PID                         */
    task->tid           = tid;                                          /*  TID                         */
    task->status        = TASK_RUNNING;                                 /*  ����̬                      */
    task->timeslice     = priority;                                     /*  ʱ��Ƭ                      */
    task->delay         = 0;                                            /*  ��ʱ                        */
    task->priority      = priority;                                     /*  ���ȼ�                      */
    task->type          = TASK_TYPE_KTHREAD;                            /*  ��������                    */
    task->resume_type   = TASK_RESUME_UNKNOW;                           /*  �ָ�����                    */

    /*
     * �ں��߳�ר����Ϣ
     */
    task->stack_base    = (void *)kmalloc(stack_size, GFP_KERNEL);      /*  �����߳�ջ�ռ�              */
    if (task->stack_base == NULL) {
        goto __exit0;
    }
    task->stack_size    = stack_size;                                   /*  �߳�ջ��С                  */
    task->thread        = func;                                         /*  �̺߳���                    */
    task->arg           = arg;                                          /*  �̲߳���                    */

    task->reent         = idle ? reents : &reents[tid - PROCESS_NR + 1];/*  ������ṹָ��              */

    _REENT_INIT_PTR(task->reent);                                       /*  ��ʼ��������ṹ            */

    /*
     * ����ר����Ϣ
     */
    task->pinfo         = NULL;                                         /*  ������Ϣָ��                */
    task->vmm           = NULL;                                         /*  VMM ��Ϣ                    */

    if (name != NULL) {                                                 /*  ����                        */
        strlcpy(task->name, name, sizeof(task->name));
    } else {
        strcpy(task->name, "???");
    }

    if (arch_build_context(task, kthread_shell) < 0) {                  /*  �����ں��߳�������          */
        goto __exit0;
    }

    if (ipc_task_init(task) < 0) {                                      /*  ��ʼ������� IPC            */
        goto __exit1;
    }

    if (vfs_process_init(task->pid, tid, 100) < 0) {               /*  ��ʼ��������ļ���Ϣ        */
        goto __exit2;
    }

    if (os_started) {                                                   /*  ����ں��Ѿ�����            */
        schedule();                                                     /*  ���µ���                    */
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
** Descriptions:            �����߳�
** input parameters:        arg                 ����
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
** Descriptions:            ���������߳�
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
** Descriptions:            �����ں��߳�
** input parameters:        name                �ں��߳�����
**                          func                �ں��߳̽���㺯��
**                          arg                 �ں��̲߳���
**                          stack_size          �ں��߳�ջ�ռ��С
**                          priority            �ں��߳����ȼ�
** output parameters:       NONE
** Returned value:          �ں��߳� TID
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
** Descriptions:            ����ǰ����
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

    ipc_task_cleanup(task);                                             /*  ��������� IPC              */

    vfs_process_cleanup(task->pid, task->tid);                          /*  ����������ļ���Ϣ          */

    if (task->type == TASK_TYPE_PROCESS) {                              /*  ��������ǽ���              */

    } else {                                                            /*  ����������ں��߳�          */
        kfree(task->stack_base);                                        /*  �ͷ��ں��̵߳�ջ�ռ�        */
    }

    task->status = TASK_UNALLOCATE;                                     /*  �ͷ������������ƿ�        */

    schedule();                                                         /*  ���½����������            */

    interrupt_resume(reg);
}
/*********************************************************************************************************
** Function name:           task_switch_hook
** Descriptions:            �����л����Ӻ���
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void task_switch_hook(void)
{

}
/*********************************************************************************************************
** ���湩ϵͳ���ô���ʹ��
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           task_schedule
** Descriptions:            �������
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

    if (!os_started) {                                                  /*  ����ں˻�û����            */
        interrupt_resume(reg);
        return;                                                         /*  ֱ�ӷ���                    */
    }

    if (interrupt_nest > 0) {                                           /*  �����û��ȫ�˳��ж�        */
        interrupt_resume(reg);
        return;                                                         /*  ֱ�ӷ���                    */
    }

    max  = -1;
    flag = FALSE;
    next = NULL;

    while (1) {
        /*
         * �����ں��̵߳���, �������̵���
         */
        for (i = PROCESS_NR, task = tasks + PROCESS_NR; i < TASK_NR; i++, task++) {
            if ((task->status == TASK_RUNNING)
                    && (max < (int32_t)task->priority)) {
                max  = (int32_t)task->priority;                         /*  �����ȼ���������            */
                next = task;
            }
        }

        if (max > 0) {                                                  /*  �ҵ���һ���������ں��߳�    */
            break;
        }

#if CONFIG_PROCESS_EN > 0
        for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {    /*  ���� 0 �����뾺��           */
            if ((task->status == TASK_RUNNING)
                    && (max < (int32_t)task->timeslice)) {
                max  = (int32_t)task->timeslice;                        /*  ��ʣ��ʱ��Ƭ��������        */
                next = task;
            }
        }

        if (max > 0) {                                                  /*  �ҵ���һ����ʣ��ʱ��Ƭ�Ľ���*/
            break;
        } else
#endif
        if (flag) {                                                     /*  ���û��һ���������        */
            next = &tasks[0];                                           /*  �����п��н���              */
            break;
        }

        for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {    /*  �������о������̵�ʣ��ʱ��Ƭ*/
            if (task->status == TASK_RUNNING) {
                task->timeslice = task->priority;
            }
        }

        flag = TRUE;
    }

    task = current;
    if (task == next) {                                                 /*  �������Ҫ�л�����          */
        current->pid = 0;
        return;
    } else {
        current = next;                                                 /*  ��д current ָ��           */

        if (current->pid != 0) {
            current->pid = 0;
        }

        _impure_ptr = current->reent;                                   /*  ��д _impure_ptr ָ��       */
    }

    /*
     * �����л�
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
** Descriptions:            ��������
** input parameters:        ticks               TICK ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int task_sleep(tick_t ticks)
{
    reg_t reg;

    reg = interrupt_disable();

    current->delay = ticks == 0 ? 1 : ticks;                            /*  ��˯ TICK ��                */

    current->status = TASK_SLEEPING;                                    /*  ��ǰ���������˯̬          */

    current->resume_type = TASK_RESUME_UNKNOW;                          /*  ���ûָ�����Ϊδ֪          */

    schedule();                                                         /*  �������                    */

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           task_getpid
** Descriptions:            ��õ�ǰ����� PID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ��ǰ����� PID
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
** Descriptions:            ��õ�ǰ����� TID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ��ǰ����� TID
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
