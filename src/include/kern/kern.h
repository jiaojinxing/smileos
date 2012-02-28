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

#include "config.h"
#include "types.h"
#include "mem.h"

#ifdef SMILEOS_KERNEL

/*
 * ��������
 */
#define TASK_TYPE_PROCESS       0
#ifdef SMILEOS_KTHREAD
#define TASK_TYPE_THREAD        1
#endif

/*
 * ����״̬
 */
#define TASK_UNALLOCATE         ((uint32_t)-1)
#define TASK_RUNNING            0
#define TASK_SLEEPING           1
#define TASK_SUSPEND            2

#define TASK_RESUME_UNKNOW      0
#define TASK_RESUME_MUTEX_COME  (1 << 1)
#define TASK_RESUME_SEM_COME    (1 << 2)
#define TASK_RESUME_TIMEOUT     (1 << 3)
#define TASK_RESUME_MSG_COME    (1 << 4)
#define TASK_RESUME_MSG_OUT     (1 << 5)

/*
 * ������ƿ�
 */
typedef struct _task {
    int32_t         pid;
    int32_t         tid;
    uint32_t        state;
    uint32_t        count;
    uint32_t        timer;
    uint32_t        prio;
    uint32_t        content[20];
    uint32_t        kstack[KERN_STACK_SIZE];
#ifdef SMILEOS_KTHREAD
    int             type;
#endif
    heap_t          heap;
    int             errno;
    int             resume_type;
    struct _task   *next;
    struct _task  **wait_list;
} task_t __attribute__((packed));

/*
 * ��ǰ���е�����
 */
extern task_t *current;

/*
 * ������ƿ�
 */
extern task_t task[TASK_NR];

/*
 * TICK
 */
extern uint64_t tick;

/*
 * ϵͳ���ô���
 */
typedef uint32_t sys_do_t;

/*
 * ��ʼ��������
 */
void sched_init(void);

/*
 * ����������
 */
void __switch_to_process0(register uint32_t sp_svc);
#define sched_start() __switch_to_process0(current->content[0])

/*
 * ����
 */
void schedule(void);

/*
 * �ں˶�ʱ��������
 */
void do_timer(void);

/*
 * ��������
 */
int32_t process_create(uint8_t *code, uint32_t size, uint32_t prio);

#ifdef SMILEOS_KTHREAD
/*
 * �����߳�
 */
int32_t kthread_create(void (*func)(void *), void *arg, uint32_t stk_size, uint32_t prio);
#endif

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
 * ���ں˶ѷ����ڴ�
 */
void *kmalloc(uint32_t size);

/*
 * �ͷ��ڴ���ں˶�
 */
void kfree(void *ptr);

/*
 * �����ַת�����ַ
 */
static inline uint32_t virt_to_phy(uint32_t va)
{
    if (current->pid > 0) {
        return PROCESS_MEM_BASE + PROCESS_MEM_SIZE * (current->pid - 1) + va;
    } else {
        return va;
    }
}

/*
 * �����ַת�����ַ
 */
static inline uint32_t __virt_to_phy(uint32_t va, int pid)
{
    if (pid > 0) {
        return PROCESS_MEM_BASE + PROCESS_MEM_SIZE * (pid - 1) + va;
    } else {
        return va;
    }
}

/*
 * �����ٽ�����
 */
uint32_t interrupt_disable(void);

/*
 * �˳��ٽ�����
 */
void interrupt_resume(register uint32_t reg);

#endif

#endif                                                                  /*  KERN_H_                     */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
