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
** File name:               kern.h
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            �ں�ͷ�ļ�
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
#ifndef KERN_H_
#define KERN_H_

#include "kern/config.h"
#include "kern/types.h"

#if defined(SMILEOS_KERNEL) || defined(SMILEOS_MODULE)
/*********************************************************************************************************
  �ں���������
*********************************************************************************************************/
/*
 * ��������
 */
#define TASK_TYPE_PROCESS           0                                   /*  ����                        */
#define TASK_TYPE_KTHREAD           1                                   /*  �ں��߳�                    */

/*
 * ����״̬
 */
#define TASK_UNALLOCATE             ((uint32_t)-1)                      /*  δ����                      */
#define TASK_RUNNING                0                                   /*  ����                        */
#define TASK_SLEEPING               1                                   /*  ��˯                        */
#define TASK_SUSPEND                2                                   /*  ����                        */

/*
 * ����ָ�����
 */
#define TASK_RESUME_UNKNOW          0                                   /*  δ֪                        */
#define TASK_RESUME_MUTEX_COME      (1 << 1)                            /*  ����������                  */
#define TASK_RESUME_SEM_COME        (1 << 2)                            /*  �źŵ���                    */
#define TASK_RESUME_TIMEOUT         (1 << 3)                            /*  ��ʱ                        */
#define TASK_RESUME_MSG_COME        (1 << 4)                            /*  ��Ϣ����                    */
#define TASK_RESUME_MSG_OUT         (1 << 5)                            /*  ��Ϣ����ȡ                  */
#define TASK_RESUME_INTERRUPT       (1 << 6)                            /*  �ȴ����ж�                  */
#define TASK_RESUME_SELECT_EVENT    (1 << 7)                            /*  select �¼�                 */
#define TASK_RESUME_AGAIN           (1 << 8)                            /*  ��Ҫ������һ��              */

#include <signal.h>
/*
 * �ź�������
 */
typedef struct _signal_ctx_t {
    int                     ref;
    int                     sig;                                        /*  �ź�                        */
    struct _signal_ctx_t   *sig_int;
    uint32_t                regs[20];                                   /*  ������                      */
    uint32_t                stack[SIG_USR_STACK_SIZE];                  /*  ��ͨջ                      */
    uint32_t                kstack[SIG_KERN_STACK_SIZE];                /*  �ں�ջ                      */
} signal_ctx_t;

/*
 * ���� errno
 */
#include <errno.h>
#define seterrno(err)       errno = (err)

struct vmm_frame;

#include <sys/reent.h>                                                  /*  for struct _reent           */

/*
 * ������ƿ�
 */
typedef struct task {
/********************************************************************************************************/
    /*
     * �����޸ĸ�����ĳ�Ա����, λ��Ҳ���ܱ�!
     */
    int32_t                 pid;                                        /*  ���� ID                     */
    int32_t                 tid;                                        /*  ���� ID                     */
    uint32_t                status;                                     /*  ״̬                        */
    uint32_t                timeslice;                                  /*  ʣ��ʱ��Ƭ                  */
    uint32_t                delay;                                      /*  ��˯ʣ�� TICK ��            */
    uint32_t                priority;                                   /*  ���ȼ�                      */
    uint32_t                regs[20];                                   /*  ������                      */
    uint32_t                kstack[KERN_STACK_SIZE];                    /*  �ں�ջ                      */
    uint32_t                type;                                       /*  ��������                    */
    uint32_t                resume_type;                                /*  �ָ�����                    */
    uint32_t                cpu_usage;                                  /*  CPU ռ����                  */
    uint32_t                ticks;                                      /*  ���񱻶�ʱ���жϵĴ���      */
    char                    name[NAME_MAX];                             /*  ����                        */
    uint32_t                dabt_cnt;                                   /*  ���ݷ�����ֹ����            */
    struct _reent          *reent;                                      /*  ������ṹָ��              */
/********************************************************************************************************/
    /*
     * �ں��߳�ר����Ϣ
     */
    void                  (*thread)(void *arg);                         /*  �̺߳���                    */
    void                   *arg;                                        /*  �̲߳���                    */
    uint32_t                stack_base;                                 /*  �߳�ջ��ַ                  */
    uint32_t                stack_size;                                 /*  �߳�ջ��С                  */

    /*
     * ����ר����Ϣ
     */
    uint32_t                frame_nr;                                   /*  ҳ����                      */
    uint32_t                page_tbl_nr;                                /*  ҳ����                      */
    struct vmm_frame       *frame_list;                                 /*  ҳ������                    */
    uint32_t                mmu_backup[PROCESS_SPACE_SIZE / SECTION_SIZE];  /*  һ���α���            */
    uint32_t                file_size;                                  /*  ���̶������ļ���С          */
    void                   *pinfo;                                      /*  ������Ϣָ��                */

    /*
     * IPC
     */
    struct task            *next;                                       /*  ����                        */
    struct task           **wait_list;                                  /*  �ȴ�����                    */

    /*
     * �ź�
     */
    sigset_t                sigset_pend;                                /*  δ���ź�                    */
    sigset_t                sigset_mask;                                /*  δ���ź�                    */
    sigset_t                sigset_old;
    void                   *signal_queue;                               /*  �źŶ���                    */
    signal_ctx_t           *signal_ctx;                                 /*  �ź�������                  */
    struct sigaction        signal_action[NSIG];
    signal_ctx_t           *signal_current;
    uint32_t                signal_status;
    uint32_t                signal_delay;
    uint32_t                signal_resume;
} task_t;

/*
 * ϵͳ���ô���
 */
typedef int (*sys_do_t)();

/*
 * ϵͳ���ò���
 */
typedef struct {
    void *arg0;
    void *arg1;
    void *arg2;
    void *arg3;
    void *arg4;
    void *arg5;
    void *arg6;
    void *arg7;
    void *arg8;
    void *arg9;
} syscall_args_t;
/*********************************************************************************************************
  �ں˺���
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           kernel_init
** Descriptions:            ��ʼ���ں�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_init(void);
/*********************************************************************************************************
** Function name:           kernel_start
** Descriptions:            �����ں�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_start(void);
/*********************************************************************************************************
** Function name:           kernel_timer
** Descriptions:            �ں˶�ʱ��������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_timer(void);
/*********************************************************************************************************
** Function name:           process_create
** Descriptions:            ��������
** input parameters:        path                ���̶������ļ� PATH
**                          priority            ����ʱ��Ƭ
**                          argc                ��������
**                          argv                ��������
** output parameters:       NONE
** Returned value:          ���� PID
*********************************************************************************************************/
int32_t process_create(const char *path, uint32_t priority, int argc, char **argv);
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
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stack_size, uint32_t priority);
/*********************************************************************************************************
** Function name:           printk
** Descriptions:            ��Ϊ�������� kmalloc, ���Բ������� kmalloc ʧ��֮��,
**                          ��ֹ�ں�ǰ�ı���Ҳ����ʹ��
** input parameters:        fmt                 ��ʽ�ַ���
**                          ...                 �������
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void printk(const char *fmt, ...);
/*********************************************************************************************************
** Function name:           netjob_add
** Descriptions:            ����һ�����繤��
** input parameters:        func                ���繤������
**                          arg                 ���繤����������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int netjob_add(void (*func)(void *), void *arg);
/*********************************************************************************************************
** Function name:           kcomplain
** Descriptions:            �ں˱�Թ(������ʹ�� printk ʱʹ��)
** input parameters:        fmt                 ��ʽ�ַ���
**                          ...                 �������
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kcomplain(const char *fmt, ...);
/*********************************************************************************************************
** Function name:           __kmalloc
** Descriptions:            ���ں��ڴ��������ڴ�
** input parameters:        func                �����ߵĺ�����
**                          line                �����ߵ��к�
**                          size                ��Ҫ����Ĵ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *__kmalloc(const char *func, int line, size_t size);
#define GFP_KERNEL      0
#define kmalloc(a, b)   __kmalloc(__func__, __LINE__, a)
/*********************************************************************************************************
** Function name:           __kfree
** Descriptions:            �ͷ��ڴ���ں��ڴ��
** input parameters:        func                �����ߵĺ�����
**                          line                �����ߵ��к�
**                          ptr                 �ڴ�ָ��
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void __kfree(const char *func, int line, void *ptr);
#define kfree(a)        __kfree(__func__, __LINE__, a)
/*********************************************************************************************************
** Function name:           __kcalloc
** Descriptions:            ���ں��ڴ��������ڴ�
** input parameters:        func                �����ߵĺ�����
**                          line                �����ߵ��к�
**                          nelem               Ԫ�صĸ���
**                          elsize              Ԫ�صĴ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *__kcalloc(const char *func, int line, size_t nelem, size_t elsize);
#define kcalloc(a, b)   __kcalloc(__func__, __LINE__, a, b)
/*********************************************************************************************************
** Function name:           interrupt_enter
** Descriptions:            �����ж�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_enter(void);
/*********************************************************************************************************
** Function name:           interrupt_exit
** Descriptions:            �˳��ж�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_exit(void);
/*********************************************************************************************************
** Function name:           getticks
** Descriptions:            ��� TICK
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TICK
*********************************************************************************************************/
uint64_t getticks(void);
/*********************************************************************************************************
** Function name:           gettid
** Descriptions:            ��õ�ǰ����� TID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ��ǰ����� TID
*********************************************************************************************************/
int32_t gettid(void);
/*********************************************************************************************************
** Function name:           in_interrupt
** Descriptions:            �ж��Ƿ����жϴ��������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int in_interrupt(void);
/*********************************************************************************************************
** Function name:           schedule
** Descriptions:            ���µ���
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int schedule(void);
/*********************************************************************************************************
** Function name:           in_kernel
** Descriptions:            �ж��Ƿ����ں�ģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int in_kernel(void);
/*********************************************************************************************************
** Function name:           interrupt_disable
** Descriptions:            �����ٽ�����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          �����ٽ�����ǰ�� CPSR
*********************************************************************************************************/
uint32_t interrupt_disable(void);
/*********************************************************************************************************
** Function name:           interrupt_resume
** Descriptions:            �˳��ٽ�����
** input parameters:        reg                 �����ٽ�����ǰ�� CPSR
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_resume(register uint32_t reg);
/*********************************************************************************************************
** Function name:           kputc
** Descriptions:            ͨ���������һ���ַ�
** input parameters:        c                   �ַ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kputc(unsigned char c);
/*********************************************************************************************************
** Function name:           kgetc
** Descriptions:            ͨ�����ڶ�ȡһ���ַ�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          һ���ַ�
*********************************************************************************************************/
unsigned char kgetc(void);
/*********************************************************************************************************
** Function name:           va_to_mva
** Descriptions:            �����̿ռ�������ַת��Ϊ�޸ĺ�������ַ
** input parameters:        VA                  ���̿ռ�������ַ
** output parameters:       NONE
** Returned value:          �޸ĺ�������ַ
*********************************************************************************************************/
void *va_to_mva(const void *VA);
/*********************************************************************************************************
** Function name:           mva_to_va
** Descriptions:            ���޸ĺ�������ַת��Ϊ���̿ռ�������ַ
** input parameters:        MVA                 �޸ĺ�������ַ
** output parameters:       NONE
** Returned value:          ���̿ռ�������ַ
*********************************************************************************************************/
void *mva_to_va(const void *MVA);
/*********************************************************************************************************
** Function name:           vspace_usable
** Descriptions:            �ж������ַ�ռ��Ƿ����
** input parameters:        base                �����ַ�ռ�Ļ�ַ
**                          size                �����ַ�ռ�Ĵ�С
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int vspace_usable(uint32_t base, uint32_t size);
/*********************************************************************************************************
** �жϷ����������
*********************************************************************************************************/
typedef int (*isr_t)(uint32_t interrupt, void *arg);
/*********************************************************************************************************
** Function name:           isr_invaild
** Descriptions:            ��Ч�жϷ������
** input parameters:        interrupt           �жϺ�
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int isr_invaild(uint32_t interrupt, void *arg);
/*********************************************************************************************************
** Function name:           interrupt_init
** Descriptions:            ��ʼ���ж�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_init(void);
/*********************************************************************************************************
** Function name:           interrupt_mask
** Descriptions:            �����ж�
** input parameters:        interrupt           �жϺ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_mask(uint32_t interrupt);
/*********************************************************************************************************
** Function name:           interrupt_unmask
** Descriptions:            ȡ�������ж�
** input parameters:        interrupt           �жϺ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_unmask(uint32_t interrupt);
/*********************************************************************************************************
** Function name:           interrupt_install
** Descriptions:            ��װ�жϷ������
** input parameters:        interrupt           �жϺ�
**                          new_isr             �µ��жϷ������
**                          arg                 �жϷ���������
** output parameters:       old_isr             �ɵ��жϷ������
** Returned value:          NONE
*********************************************************************************************************/
int interrupt_install(uint32_t interrupt, isr_t new_isr, isr_t *old_isr, void *arg);

#endif

#endif                                                                  /*  KERN_H_                     */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
