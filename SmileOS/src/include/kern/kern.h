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

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
** �û�������Ҫ֪���ں˵����ú��������ͼ��ӿ�
*********************************************************************************************************/
#include "kern/types.h"

#if defined(SMILEOS_KERNEL) || defined(SMILEOS_MODULE)

#include "kern/config.h"

/*********************************************************************************************************
** �ں���������
*********************************************************************************************************/
#define HZ                          (TICK_PER_SECOND)                   /*  HZ                          */

/*
 * ��������
 */
#define TASK_TYPE_PROCESS           0                                   /*  ����                        */
#define TASK_TYPE_KTHREAD           1                                   /*  �ں��߳�                    */

/*
 * ����״̬
 */
#define TASK_UNALLOCATE             ((uint8_t)-1)                       /*  δ����                      */
#define TASK_RUNNING                0                                   /*  ����                        */
#define TASK_SLEEPING               1                                   /*  ��˯                        */
#define TASK_SUSPEND                2                                   /*  ����                        */

/*
 * ����ָ�����
 */
#define TASK_RESUME_UNKNOW          (0)                                 /*  δ֪                        */
#define TASK_RESUME_MUTEX_COME      (1)                                 /*  ����������                  */
#define TASK_RESUME_SEM_COME        (2)                                 /*  �źŵ���                    */
#define TASK_RESUME_SELECT_EVENT    (7)                                 /*  select �¼�                 */
#define TASK_RESUME_MSG_COME        (4)                                 /*  ��Ϣ����                    */
#define TASK_RESUME_MSG_OUT         (5)                                 /*  ��Ϣ����ȡ                  */
#define TASK_RESUME_TIMEOUT         (6)                                 /*  ��ʱ                        */
#define TASK_RESUME_INTERRUPT       (7)                                 /*  �ȴ����ж�                  */

/*
 * ���� errno
 */
#include <errno.h>
#define seterrno(err)               errno = (err)
#define geterrno(err)               err = errno

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
#if CPU_REGS_SIZE > 0
    reg_t                   regs[CPU_REGS_SIZE];                        /*  ������                      */
#endif
#if KERN_STACK_SIZE > 0
    reg_t                   kstack[KERN_STACK_SIZE];                    /*  �ں�ջ                      */
#endif
    uint8_t                 type;                                       /*  ��������                    */
/********************************************************************************************************/
    uint8_t                 status;                                     /*  ״̬                        */
    uint8_t                 priority;                                   /*  ���ȼ�                      */
    uint8_t                 timeslice;                                  /*  ʣ��ʱ��Ƭ                  */
    tick_t                  delay;                                      /*  ��˯ʣ�� TICK ��            */
    uint8_t                 resume_type;                                /*  �ָ�����                    */
    char                    name[NAME_MAX];                             /*  ����                        */
    struct _reent          *reent;                                      /*  ������ṹָ��              */

    /*
     * �ں��߳�ר����Ϣ
     */
    void                  (*thread)(void *arg);                         /*  �̺߳���                    */
    void                   *arg;                                        /*  �̲߳���                    */
    void                   *stack_base;                                 /*  �߳�ջ��ַ                  */
    size_t                  stack_size;                                 /*  �߳�ջ��С                  */

    /*
     * IPC ר����Ϣ
     */
    struct task            *next;                                       /*  ����                        */
    struct task           **wait_list;                                  /*  �ȴ�����                    */

    /*
     * PROCESS ר����Ϣ
     */
    void                   *pinfo;                                      /*  ������Ϣָ��                */

    /*
     * VMM ר����Ϣ
     */
    void                   *vmm;
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

typedef tick_t mseconds_t;
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
**                          timeslice           ʱ��Ƭ
**                          argc                ��������
**                          argv                ��������
** output parameters:       NONE
** Returned value:          ���� PID
*********************************************************************************************************/
int32_t process_create(const char *path, uint8_t timeslice, int argc, char **argv);
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
                       uint8_t priority);
/*********************************************************************************************************
** Function name:           printk
** Descriptions:            ��ֹ�ں�ǰ�ı���Ҳ����ʹ��
** input parameters:        fmt                 ��ʽ�ַ���
**                          ...                 �������
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void printk(const char *fmt, ...);
#define KERN_EMERG      "<0>"
#define KERN_ALERT      "<1>"
#define KERN_CRIT       "<2>"
#define KERN_ERR        "<3>"
#define KERN_WARNING    "<4>"
#define KERN_NOTICE     "<5>"
#define KERN_INFO       "<6>"
#define KERN_DEBUG      "<7>"
/*********************************************************************************************************
** Function name:           __kmalloc
** Descriptions:            ���ں��ڴ��������ڴ�
** input parameters:        func                �����ߵĺ�����
**                          line                �����ߵ��к�
**                          size                ��Ҫ����Ĵ�С
**                          flags               �����־
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *__kmalloc(const char *func, int line, size_t size, int flags);
#define GFP_ATOMIC      (0)
#define GFP_NOFS        (0)
#define GFP_KERNEL      GFP_ATOMIC
#define GFP_DMA         (1 << 0)
#define GFP_SHARE       (1 << 1)
#define GFP_ZERO        (1 << 8)
#define kmalloc(a, b)   __kmalloc(__func__, __LINE__, a, b)
#define kzalloc(a, b)   __kmalloc(__func__, __LINE__, a, b | GFP_ZERO)
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
** Function name:           getticks
** Descriptions:            ��� TICK
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TICK
*********************************************************************************************************/
tick_t getticks(void);
/*********************************************************************************************************
** Function name:           schedule
** Descriptions:            ���µ���
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int schedule(void);
/*********************************************************************************************************
** �жϷ����������
*********************************************************************************************************/
typedef int (*isr_t)(intno_t interrupt, void *arg);
/*********************************************************************************************************
** Function name:           in_interrupt
** Descriptions:            �ж��Ƿ����жϴ��������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
bool_t in_interrupt(void);
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
** Function name:           isr_invaild
** Descriptions:            ��Ч���жϷ������
** input parameters:        interrupt           �жϺ�
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int isr_invaild(intno_t interrupt, void *arg);
/*********************************************************************************************************
** Function name:           interrupt_install
** Descriptions:            ��װ�жϷ������
** input parameters:        interrupt           �жϺ�
**                          new_isr             �µ��жϷ������
**                          arg                 �жϷ���������
** output parameters:       old_isr             �ɵ��жϷ������
** Returned value:          0 OR -1
*********************************************************************************************************/
int interrupt_install(intno_t interrupt, isr_t new_isr, isr_t *old_isr, void *arg);
/*********************************************************************************************************
** Function name:           interrupt_exec
** Descriptions:            ִ���жϷ������
** input parameters:        interrupt           �жϺ�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int interrupt_exec(intno_t interrupt);
/*********************************************************************************************************
** Function name:           msleep
** Descriptions:            ����ָ���ĺ�����
** input parameters:        mseconds            ������
** output parameters:       NONE
** Returned value:          0 OR -1
**
** ��Ϊϵͳ����Ǻ��뼶, �����ṩ��ϵͳ����
**
*********************************************************************************************************/
int msleep(mseconds_t mseconds);
/*********************************************************************************************************
** Function name:           gettid
** Descriptions:            ��õ�ǰ����� TID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ��ǰ����� TID
*********************************************************************************************************/
int32_t gettid(void);
/*********************************************************************************************************
** Function name:           bkdr_hash
** Descriptions:            BKDR Hash Function
**                          �����ַ��� Hash �����Ƚ�
**                          http://www.byvoid.com/blog/string-hash-compare/
** input parameters:        str                 �ַ���
** output parameters:       NONE
** Returned value:          BKDR Hash
*********************************************************************************************************/
unsigned int bkdr_hash(const char *str);
/*********************************************************************************************************
** ��������
*********************************************************************************************************/
struct workqueue;
typedef struct workqueue workqueue_t;
/*********************************************************************************************************
** Function name:           workqueue_create
** Descriptions:            ������������
** input parameters:        name                ����
**                          size                ��С
**                          priority            ���ȼ�
** output parameters:       wq                  ��������
** Returned value:          0 OR -1
*********************************************************************************************************/
int workqueue_create(workqueue_t *wq, const char *name, size_t size, uint8_t priority);
/*********************************************************************************************************
** Function name:           workqueue_add
** Descriptions:            ����һ������
** input parameters:        wq                  ��������
**                          func                ��������
**                          arg                 ������������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int workqueue_add(workqueue_t *wq, void (*func)(void *), void *arg);
/*********************************************************************************************************
** Function name:           ms_to_tick
** Descriptions:            ������ת���� tick
** input parameters:        ms                  ����
** output parameters:       NONE
** Returned value:          tick
*********************************************************************************************************/
static inline tick_t ms_to_tick(mseconds_t ms)
{
    if (ms != 0) {
        if (ms <= 1000 / HZ) {
            ms = 1;
        } else {
            ms = ms * HZ / 1000;
        }
    }
    return ms;
}
/*********************************************************************************************************
** Function name:           tick_to_ms
** Descriptions:            �� tick ת���ɺ���
** input parameters:        tick                  tick
** output parameters:       NONE
** Returned value:          ����
*********************************************************************************************************/
static inline mseconds_t tick_to_ms(tick_t tick)
{
    if (tick != 0) {
        tick = tick * 1000 / HZ;
    }
    return tick;
}
/*********************************************************************************************************
** Function name:           kheap_check
** Descriptions:            ����ں��ڴ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kheap_check(void);

#include "arch/if.h"

#endif

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  KERN_H_                     */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
