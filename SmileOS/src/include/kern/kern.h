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
    uint32_t                state;                                      /*  ״̬                        */
    uint32_t                counter;                                    /*  ʣ��ʱ��Ƭ                  */
    uint32_t                timer;                                      /*  ��˯ʣ�� TICK ��            */
    uint32_t                priority;                                   /*  ���ȼ�                      */
    uint32_t                content[20];                                /*  ������                      */
    uint32_t                kstack[KERN_STACK_SIZE];                    /*  �ں�ջ                      */
/********************************************************************************************************/
    void                  (*thread)(void *arg);                         /*  �̺߳���                    */
    void                   *arg;                                        /*  �̲߳���                    */
    uint32_t                stack_base;                                 /*  �߳�ջ��ַ                  */
    uint32_t                stack_size;                                 /*  �߳�ջ��С                  */
    uint32_t                stack_rate;                                 /*  �߳�ջռ����                */
    uint32_t                type;                                       /*  ��������                    */
    uint32_t                resume_type;                                /*  �ָ�����                    */
    uint32_t                frame_nr;                                   /*  ҳ����                      */
    uint32_t                page_tbl_nr;
    uint32_t                cpu_rate;                                   /*  CPU ռ����                  */
    uint32_t                tick;                                       /*  ���񱻶�ʱ���жϵĴ���      */
    char                    name[NAME_MAX];                             /*  ����                        */
    struct task            *next;                                       /*  ����                        */
    struct task           **wait_list;                                  /*  �ȴ�����                    */
    struct vmm_frame       *frame_list;                                 /*  ҳ������                    */
    uint32_t                dabt_cnt;                                   /*  ���ݷ�����ֹ����            */
    uint32_t                mmu_backup[PROCESS_SPACE_SIZE / SECTION_SIZE];  /*  һ���α���            */
    struct _reent          *reent;                                      /*  ������ṹ                  */
    uint32_t                file_size;
} task_t;

/*
 * ϵͳ���ô���
 */
typedef int (*sys_do_t)();

/*
 * ϵͳ���ô������
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
} sys_do_args_t;
/*********************************************************************************************************
  �ں˱���
*********************************************************************************************************/
extern task_t               tasks[TASK_NR];                             /*  ������ƿ�                  */
extern task_t              *current;                                    /*  ָ��ǰ���е�����          */
extern const char           smileos_logo[];                             /*  LOGO                        */
/*********************************************************************************************************
  �ں˺���
*********************************************************************************************************/
/*
 * ��ʼ���ں�
 */
void kernel_init(void);

/*
 * �����ں�
 */
void kernel_start(void);

/*
 * �������
 * ����֮ǰ������ж�
 */
void task_schedule(void);

/*
 * �ں˶�ʱ��������
 */
void kernel_timer(void);

/*
 * ��������
 */
int32_t process_create(const char *path, uint32_t priority, int argc, char **argv);

/*
 * fork һ���ӽ���
 */
int process_fork(void);

/*
 * �����ں��߳�
 */
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
** Function name:           kcomplain
** Descriptions:            �ں˱�Թ(������ʹ�� printk ʱʹ��)
** input parameters:        fmt                 ��ʽ�ַ���
**                          ...                 �������
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kcomplain(const char *fmt, ...);

/*
 * kputc
 */
void kputc(unsigned char c);

/*
 * kgetc
 */
unsigned char kgetc(void);

/*
 * ���ں��ڴ�ѷ����ڴ�
 */
void *__kmalloc(const char *func, int line, uint32_t size);

/*
 * �ͷ��ڴ���ں��ڴ��
 */
void __kfree(const char *func, int line, void *ptr);

/*
 * __kcalloc
 */
void *__kcalloc(const char *func, int line, uint32_t nelem, uint32_t elsize);

#define kmalloc(a)      __kmalloc(__func__, __LINE__, a)
#define kfree(a)        __kfree(__func__, __LINE__, a)
#define kcalloc(a, b)   __kcalloc(__func__, __LINE__, a, b)

/*
 * ��ӡ�ں��ڴ����Ϣ
 */
void kheap_show(int fd);

/*
 * �����ٽ�����
 */
uint32_t interrupt_disable(void);

/*
 * �˳��ٽ�����
 */
void interrupt_resume(register uint32_t reg);

/*
 * �����ж�
 */
void interrupt_enter(void);

/*
 * �˳��ж�
 */
void interrupt_exit(void);

/*
 * �˳��ж�, ����Ҫ�����������
 */
void interrupt_exit_no_sched(void);

/*
 * ��� TICK
 */
uint64_t gettick(void);

/*
 * ������� ID
 */
int32_t gettid(void);

/*
 * �������ź�
 */
#include <signal.h>
int task_kill(int32_t tid, int sig);

/*
 * ��������
 */
int task_sleep(uint32_t ticks);

/*
 * �������� reent �ṹָ��
 */
int task_setreent(struct _reent *reent);

/*
 * �������� PID
 */
int32_t task_getpid(void);

/*
 * �ж��Ƿ����жϴ��������
 */
int in_interrupt(void);

/*
 * �ж��Ƿ����ں�ģʽ
 */
int in_kernel(void);

/*
 * �ͷ� CPU ʹ��Ȩ
 */
void yield(void);

/*
 * ���������Ϣ
 */
int task_stat(int tid, char *buf);

#endif

#endif                                                                  /*  KERN_H_                     */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
