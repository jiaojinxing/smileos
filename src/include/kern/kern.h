/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <JiaoJinXing1987@gmail.com>
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

#include "types.h"

#ifdef SMILEOS_KERNEL
/*********************************************************************************************************
  �ں���������
*********************************************************************************************************/
/*
 * ��������
 */
#define TASK_TYPE_PROCESS       0                                       /*  ����                        */
#define TASK_TYPE_THREAD        1                                       /*  �ں��߳�                    */

/*
 * ����״̬
 */
#define TASK_UNALLOCATE         ((uint32_t)-1)                          /*  δ����                      */
#define TASK_RUNNING            0                                       /*  ����                        */
#define TASK_SLEEPING           1                                       /*  ��˯                        */
#define TASK_SUSPEND            2                                       /*  ����                        */

/*
 * ����ָ�����
 */
#define TASK_RESUME_UNKNOW      0                                       /*  δ֪                        */
#define TASK_RESUME_MUTEX_COME  (1 << 1)                                /*  ����������                  */
#define TASK_RESUME_SEM_COME    (1 << 2)                                /*  �źŵ���                    */
#define TASK_RESUME_TIMEOUT     (1 << 3)                                /*  ��ʱ                        */
#define TASK_RESUME_MSG_COME    (1 << 4)                                /*  ��Ϣ����                    */
#define TASK_RESUME_MSG_OUT     (1 << 5)                                /*  ��Ϣ����ȡ                  */

struct _vmm_frame_t;
struct _page_table_t;
/*
 * ������ƿ�
 */
typedef struct _task {
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
    uint32_t                stack;                                      /*  �߳�ջ��ַ                  */
    uint32_t                type;                                       /*  ��������                    */
    uint32_t                resume_type;                                /*  �ָ�����                    */
    uint32_t                frame_nr;                                   /*  ҳ����                      */
    uint32_t                utilization;                                /*  CPU ռ����                  */
    uint32_t                tick;                                       /*  ���񱻶�ʱ���жϵĴ���      */
    int                     errno;                                      /*  �����                      */
    char                    name[16];                                   /*  ����                        */
    struct _task           *next;                                       /*  ����                        */
    struct _task          **wait_list;                                  /*  �ȴ�����                    */
    struct _vmm_frame_t    *frame_list;                                 /*  ҳ������                    */
} task_t;

/*
 * ϵͳ���ô���
 */
typedef uint32_t sys_do_t;
/*********************************************************************************************************
  �ں˱���
*********************************************************************************************************/
extern task_t               tasks[TASK_NR];                             /*  ������ƿ�                  */
extern task_t              *current;                                    /*  ָ��ǰ���е�����          */
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
void schedule(void);

/*
 * �ں˶�ʱ��������
 */
void do_timer(void);

/*
 * ��������
 */
int32_t process_create(const char *name, uint8_t *code, uint32_t size, uint32_t priority);

/*
 * �����ں��߳�
 */
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stk_size, uint32_t priority);

/*
 * printk
 */
void printk(const char *fmt, ...);

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
void *kmalloc(uint32_t size);

/*
 * �ͷ��ڴ���ں��ڴ��
 */
void kfree(void *ptr);

/*
 * kcalloc
 */
void *kcalloc(uint32_t nelem, uint32_t elsize);

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
 * ��� TICK
 */
uint64_t get_tick(void);

/*
 * �ж��Ƿ����жϴ�����
 */
int in_interrupt(void);

#endif

#endif                                                                  /*  KERN_H_                     */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
