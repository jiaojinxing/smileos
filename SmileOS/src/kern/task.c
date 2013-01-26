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
** Descriptions:            �������н���
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kidle_create(void)
{
    task_t *task;

    /*
     * ��ʼ�����̿��ƿ� 0
     */
    task                = &tasks[0];

    task->pid           = 0;                                            /*  PID                         */
    task->tid           = 0;                                            /*  TID                         */
    task->status        = TASK_RUNNING;                                 /*  ����̬                      */
    task->timeslice     = 0;                                            /*  ʱ��Ƭ                      */
    task->delay         = 0;                                            /*  ��ʱ                        */
    task->priority      = 0;                                            /*  ���ȼ�                      */
    task->type          = TASK_TYPE_PROCESS;                            /*  ��������Ϊ����              */
    task->resume_type   = TASK_RESUME_UNKNOW;                           /*  �ָ�����                    */
    task->ticks         = 0;                                            /*  ����ʱ���жϴ���            */
    task->cpu_usage     = 0;                                            /*  CPU ռ����                  */
    task->dabt_cnt      = 0;                                            /*  ���ݷ�����ֹ����            */

    /*
     * �ں��߳�ר����Ϣ
     */
    task->stack_base    = 0;                                            /*  �߳�ջ��ַ                  */
    task->stack_size    = 0;                                            /*  �߳�ջ��С                  */
    task->thread        = NULL;                                         /*  �̺߳���                    */
    task->arg           = NULL;                                         /*  �̲߳���                    */

    task->reent         = &reents[0];                                   /*  ������ṹָ��              */
    _REENT_INIT_PTR(task->reent);                                       /*  ��ʼ��������ṹ            */

    /*
     * ����ר����Ϣ
     */
    task->frame_nr      = 0;                                            /*  ҳ����                      */
    task->page_tbl_nr   = 0;                                            /*  ҳ����                      */
    task->frame_list    = NULL;                                         /*  ҳ������                    */
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));              /*  һ���α���                */

    task->pinfo         = NULL;                                         /*  ������Ϣָ��                */
    task->file_size     = 0;                                            /*  ���̶������ļ���С          */

    /*
     * ��ʼ������������
     */
    task->regs[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];         /*  SVC ģʽ�� SP (����ջ�ݼ�)  */
    task->regs[1]   = ARM_SYS_MODE | ARM_FIQ_EN | ARM_IRQ_EN;           /*  CPSR, SYS ģʽ              */
    task->regs[2]   = PROCESS0_STACK_BASE;                              /*  SYS ģʽ�� SP (����ջ�ݼ�)  */
    task->regs[3]   = ARM_SVC_MODE;                                     /*  SPSR, SVC ģʽ              */
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

    strcpy(task->name, "idle");                                         /*  ����                        */

    ipc_task_init(task);                                                /*  ��ʼ������� IPC            */

    signal_task_init(task);                                             /*  ��ʼ��������ź�ϵͳ        */

    vfs_task_init(0);                                                   /*  ��ʼ��������ļ���Ϣ        */
}
/*********************************************************************************************************
** Function name:           kthread_shell
** Descriptions:            �ں��߳����
** input parameters:        task                ������ƿ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void kthread_shell(task_t *task)
{
    open("/dev/null", O_RDONLY);                                        /*  ��������׼�ļ�            */
    stdin  = fdopen(STDIN_FILENO,  "r");

    open("/dev/null", O_WRONLY);
    stdout = fdopen(STDOUT_FILENO, "w");

    open("/dev/null", O_WRONLY);
    stderr = fdopen(STDERR_FILENO, "w");

    putenv("PATH=/");                                                   /*  ���û�������                */
    putenv("HOME=/");

    task->thread(task->arg);                                            /*  �����������ں��̺߳���      */

    _exit(0);                                                           /*  �˳��ں��߳�                */
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
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stack_size, uint32_t priority)
{
    int32_t  tid;
    task_t  *task;
    uint32_t reg;

    if (func == NULL) {
        return -1;
    }

    if (stack_size < 1 * KB) {                                          /*  ջ�ռ���СΪ 1 KB           */
        stack_size = 1 * KB;
    } else {
        stack_size = MEM_ALIGN_SIZE(stack_size);                        /*  ����ջ�ռ��С              */
    }

    reg = interrupt_disable();

    /*
     * �����ں��߳̿��ƿ�
     */
                                                                        /*  �������е��ں��߳̿��ƿ�    */
    for (tid = PROCESS_NR, task = tasks + PROCESS_NR; tid < TASK_NR; tid++, task++) {
        if (task->status == TASK_UNALLOCATE) {                          /*  ����ں��߳̿��ƿ���Ч      */
            break;
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
    task->type          = TASK_TYPE_KTHREAD;                            /*  ��������Ϊ����              */
    task->resume_type   = TASK_RESUME_UNKNOW;                           /*  �ָ�����                    */
    task->ticks         = 0;                                            /*  ����ʱ���жϴ���            */
    task->cpu_usage     = 0;                                            /*  CPU ռ����                  */
    task->dabt_cnt      = 0;                                            /*  ���ݷ�����ֹ����            */

    /*
     * �ں��߳�ר����Ϣ
     */
    task->stack_base    = (uint32_t)kmalloc(stack_size, GFP_KERNEL);    /*  �����߳�ջ�ռ�              */
    if (task->stack_base == 0) {
        goto __exit0;
    }
    task->stack_size    = stack_size;                                   /*  �߳�ջ��С                  */
    task->thread        = func;                                         /*  �̺߳���                    */
    task->arg           = arg;                                          /*  �̲߳���                    */

    task->reent         = &reents[tid - PROCESS_NR + 1];                /*  ������ṹָ��              */
    _REENT_INIT_PTR(task->reent);                                       /*  ��ʼ��������ṹ            */

    /*
     * ����ר����Ϣ
     */
    task->frame_nr      = 0;                                            /*  ҳ����                      */
    task->page_tbl_nr   = 0;                                            /*  ҳ����                      */
    task->frame_list    = NULL;                                         /*  ҳ������                    */
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));              /*  һ���α���                */

    task->pinfo         = NULL;                                         /*  ������Ϣָ��                */
    task->file_size     = 0;                                            /*  ���̶������ļ���С          */

    /*
     * ��ʼ���ں��߳�������
     */
    task->regs[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];         /*  SVC ģʽ�� SP (����ջ�ݼ�)  */
    task->regs[1]   = ARM_SYS_MODE | ARM_FIQ_EN | ARM_IRQ_EN;           /*  CPSR, SYS ģʽ              */
    task->regs[2]   = task->stack_base + stack_size;                    /*  SYS ģʽ�� SP (����ջ�ݼ�)  */
    task->regs[3]   = ARM_SVC_MODE;                                     /*  SPSR, SVC ģʽ              */
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

    if (name != NULL) {                                                 /*  ����                        */
        strlcpy(task->name, name, sizeof(task->name));
    } else {
        strcpy(task->name, "???");
    }

    if (ipc_task_init(task) < 0) {                                      /*  ��ʼ������� IPC            */
        goto __exit0;
    }

    if (vfs_task_init(tid) < 0) {                                       /*  ��ʼ��������ļ���Ϣ        */
        goto __exit1;
    }

    if (signal_task_init(task) < 0) {                                   /*  ��ʼ��������ź�ϵͳ        */
        goto __exit2;
    }

    if (running) {                                                      /*  ����ں��Ѿ�����            */
        schedule();                                                     /*  ���µ���                    */
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
** Descriptions:            ��ʼ�����̵Ĳ���
** input parameters:        task                ���̿��ƿ�
**                          argc                ��������
**                          argv                ����ָ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int process_arg_init(task_t *task, int argc, char **argv)
{
    /*
     * ��ý��̵�ַ�ռ��в����������Ļ�ַ(MVA), �������������ڽ��̵�ַ�ռ�����
     */
    uint32_t *ptr = (uint32_t *)((task->pid + 1) * PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE);

    /*
     * �������������������õ�:
     * �������� argc, ����ָ������ argv[argc], NULL, ���� arg0, ���� arg1, ���� arg2 ...
     * ����� arg Ϊ���� arg0 �Ĵ����ַ
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
** Descriptions:            �������̴���
** input parameters:        task                ���̿��ƿ�
**                          path                ���̶������ļ�·��
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
     * �������뵽���̵������ַ�ռ�
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
     * �ղſ����Ĵ��뻹�� D-Cache, ��������ʱ��ȡ I-Cache ������, �����ǲ�һ�µ�!
     * ���Ա���Ҫ��� D-Cache, ���д����, ��Ҫ��Ч I-Cache,
     * ��Ч D-Cache ���ó� D-Cache, �����������̵� D-Cache �������� D-Cache Miss, ���������ٶ�
     *
     * ARM ���ݷ��ʵĻ�������ͼ:
     * http://www.ibm.com/developerworks/cn/linux/l-skyeye/part3/s3/index.html
     */
    /*
     * �������Ч D-Cache
     */
    for (i = 0; i < (task->file_size + 31) / 32; i++) {
        mmu_clean_invalidate_dcache_mva(task->pid * PROCESS_SPACE_SIZE + i * 32);
    }

    /*
     * ���д����
     */
    mmu_drain_write_buffer();

    /*
     * ��Ч I-Cache
     */
    for (i = 0; i < (task->file_size + 31) / 32; i++) {
        mmu_invalidate_icache_mva(task->pid * PROCESS_SPACE_SIZE + i * 32);
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           process_create
** Descriptions:            ��������
** input parameters:        path                ���̶������ļ�·��
**                          priority            ����ʱ��Ƭ
**                          argc                ��������
**                          argv                ��������
** output parameters:       NONE
** Returned value:          ���� PID
*********************************************************************************************************/
int32_t process_create(const char *path, uint32_t priority, int argc, char **argv)
{
    int32_t     pid;
    task_t     *task;
    uint32_t    reg;
    struct stat st;

    if (path == NULL) {                                                 /*  ���� PATH Ϊ NULL ��        */
        return -1;
    }

    if (stat(path, &st) < 0) {                                          /*  �����ļ����ڷ�              */
        return -1;
    }

    if (!S_ISREG(st.st_mode)) {                                         /*  �����ļ�����ͨ�ļ���        */
        return -1;
    }

    if (priority < 5) {                                                 /*  ���ȼ���СΪ 5              */
        priority = 5;
    }

    reg = interrupt_disable();

    /*
     * ������̿��ƿ�
     */
    for (pid = 1, task = tasks + 1; pid < PROCESS_NR; pid++, task++) {  /*  �������еĽ��̿��ƿ�        */
        if (task->status == TASK_UNALLOCATE) {                          /*  ������̿��ƿ���Ч          */
                                                                        /*  ������̵������ַ�ռ����  */
            if (vspace_usable(pid * PROCESS_SPACE_SIZE, PROCESS_SPACE_SIZE)) {
                break;
            }
        }
    }

    if (pid == PROCESS_NR) {                                            /*  û�п��еĽ��̿��ƿ�        */
        interrupt_resume(reg);
        return -1;
    }

    /*
     * ��ʼ�����̿��ƿ�
     */
    task->pid           = pid;                                          /*  PID                         */
    task->tid           = pid;                                          /*  TID                         */
    task->status        = TASK_SUSPEND;                                 /*  ����̬                      */
    task->timeslice     = priority;                                     /*  ʱ��Ƭ                      */
    task->delay         = 0;                                            /*  ��ʱ                        */
    task->priority      = priority;                                     /*  ���ȼ�                      */
    task->type          = TASK_TYPE_PROCESS;                            /*  ��������Ϊ����              */
    task->resume_type   = TASK_RESUME_UNKNOW;                           /*  �ָ�����                    */
    task->ticks         = 0;                                            /*  ����ʱ���жϴ���            */
    task->cpu_usage     = 0;                                            /*  CPU ռ����                  */
    task->dabt_cnt      = 0;                                            /*  ���ݷ�����ֹ����            */

    /*
     * �ں��߳�ר����Ϣ
     */
    task->stack_base    = 0;                                            /*  �߳�ջ��ַ                  */
    task->stack_size    = 0;                                            /*  �߳�ջ��С                  */
    task->thread        = NULL;                                         /*  �̺߳���                    */
    task->arg           = NULL;                                         /*  �̲߳���                    */

    task->reent         = NULL;                                         /*  ������ṹָ��              */

    /*
     * ����ר����Ϣ
     */
    task->frame_nr      = 0;                                            /*  ҳ����                      */
    task->page_tbl_nr   = 0;                                            /*  ҳ����                      */
    task->frame_list    = NULL;                                         /*  ҳ������                    */
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));              /*  һ���α���                */

    task->pinfo         = NULL;                                         /*  ������Ϣָ��                */
    task->file_size     = st.st_size;                                   /*  ���̶������ļ���С          */

    /*
     * ��ʼ������������
     */
    task->regs[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];         /*  SVC ģʽ�� SP (����ջ�ݼ�)  */
    task->regs[1]   = ARM_USR_MODE | ARM_FIQ_EN | ARM_IRQ_EN;           /*  CPSR, USR ģʽ              */
    task->regs[2]   = PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE;          /*  SYS ģʽ�� SP (����ջ�ݼ�)  */
    task->regs[3]   = ARM_SVC_MODE;                                     /*  SPSR, SVC ģʽ              */
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

    if (ipc_task_init(task) < 0) {                                      /*  ��ʼ������� IPC            */
        goto __exit0;
    }

    if (vfs_task_init(pid) < 0) {                                       /*  ��ʼ��������ļ���Ϣ        */
        goto __exit1;
    }

    if (signal_task_init(task) < 0) {                                   /*  ��ʼ��������ź�ϵͳ        */
        goto __exit2;
    }

    if (vmm_process_init(task) < 0) {                                   /*  ��ʼ�����̵������ڴ���Ϣ    */
        goto __exit3;
    }

    if (process_arg_init(task, argc, argv) < 0) {                       /*  ��ʼ�����̵Ĳ���            */
        goto __exit4;
    }

    interrupt_resume(reg);

    if (process_code_copy(task, path) < 0) {                            /*  �������̴���                */
        goto __exit5;
    }

    reg = interrupt_disable();

    task->status = TASK_RUNNING;                                        /*  ���̽������̬              */

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
** Descriptions:            ����ǰ����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void task_cleanup(task_t *task)
{
    uint32_t reg;

    reg  = interrupt_disable();

    ipc_task_cleanup(task);                                             /*  ��������� IPC              */

    vfs_task_cleanup(task->tid);                                        /*  ����������ļ���Ϣ          */

    signal_task_cleanup(task);                                          /*  ����������ź�ϵͳ          */

    if (task->type == TASK_TYPE_PROCESS) {                              /*  ��������ǽ���              */
        vmm_process_cleanup(task);                                      /*  ������̵������ڴ���Ϣ      */
    } else {                                                            /*  ����������ں��߳�          */
        kfree((void *)task->stack_base);                                /*  �ͷ��ں��̵߳�ջ�ռ�        */
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
    signal_switch(current);                                             /*  �źŴ������л�            */

    _impure_ptr = current->reent;                                       /*  ��д _impure_ptr ָ��       */
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
    uint32_t    reg;

    reg = interrupt_disable();

    if (!running) {                                                     /*  ����ں˻�û����            */
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
            if ((task->status == TASK_RUNNING || signal_schedule(task, NULL))
                    && (max < (int32_t)task->priority)) {
                max  = (int32_t)task->priority;                         /*  �����ȼ���������            */
                next = task;
            }

            if (task->signal_ctx != NULL) {
                task->signal_ctx->ref++;
            }
        }

        if (max > 0) {                                                  /*  �ҵ���һ���������ں��߳�    */
            break;
        }

        for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {    /*  ���� 0 �����뾺��           */
            if ((task->status == TASK_RUNNING || signal_schedule(task, NULL))
                    && (max < (int32_t)task->timeslice)) {
                max  = (int32_t)task->timeslice;                        /*  ��ʣ��ʱ��Ƭ��������        */
                next = task;
            }

            if (task->signal_ctx != NULL) {
                task->signal_ctx->ref++;
            }
        }

        if (max > 0) {                                                  /*  �ҵ���һ����ʣ��ʱ��Ƭ�Ľ���*/
            break;
        } else if (flag) {                                              /*  ���û��һ���������        */
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
        if (task->signal_ctx == NULL) {                                 /*  Ҳ�����л����ź�������      */
            interrupt_resume(reg);
            return;                                                     /*  ֱ�ӷ���                    */
        }
    } else {
        current = next;                                                 /*  ��д current ָ��           */
    }

    if (next->signal_ctx != NULL) {                                     /*  ��Ҫ�л����ź�������        */
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
     * �����л�
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
** Descriptions:            ��������
** input parameters:        ticks               TICK ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int task_sleep(uint32_t ticks)
{
    uint32_t reg;

    reg = interrupt_disable();

    current->delay = ticks == 0 ? 1 : ticks;                            /*  ��˯ TICK ��                */

    current->status = TASK_SLEEPING;                                    /*  ��ǰ���������˯̬          */

    current->resume_type = TASK_RESUME_UNKNOW;                          /*  ���ûָ�����Ϊδ֪          */

    schedule();                                                         /*  �������                    */

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           task_pause
** Descriptions:            ��ͣ��ǰ����
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

    schedule();                                                         /*  �������                    */

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           task_setpinfo
** Descriptions:            ���õ�ǰ����Ľ�����Ϣ
** input parameters:        info                ������Ϣָ��
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
** Descriptions:            ��õ�ǰ����� PID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ��ǰ����� PID
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
