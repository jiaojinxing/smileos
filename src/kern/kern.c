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
** File name:               kern.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            �ں�Դ�ļ�
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/arm.h"
#include <string.h>
#include <stdarg.h>

/*
 * ������ƿ�
 */
task_t task[TASK_NR];

/*
 * ��ǰ���е�����
 */
task_t *current;

/*
 * TICK
 */
uint64_t tick;

/*
 * �ں˶�
 */
static uint8_t kern_heap_mem[KERN_HEAP_SIZE];

/*
 * ���ں˶ѷ����ڴ�
 */
void *kmalloc(uint32_t size)
{
    void *ptr;
    uint32_t reg;

    reg = critical_enter();

    ptr = heap_alloc(&task[0].heap, size);

    critical_exit(reg);

    return ptr;
}

/*
 * �ͷ��ڴ���ں˶�
 */
void kfree(void *ptr)
{
    uint32_t reg;

    reg = critical_enter();

    heap_free(&task[0].heap, ptr);

    critical_exit(reg);
}

/*
 * ��ʼ��������
 */
void sched_init(void)
{
    task_t *t;
    int i;

    /*
     * ��ʼ��������ƿ�
     */
    for (i = 0, t = &task[i]; i < TASK_NR; i++, t++) {
        t->pid          = -1;
        t->tid          = -1;
        t->state        = TASK_UNALLOCATE;
        t->count        = 0;
        t->timer        = 0;
        t->prio         = 0;
        t->errno        = 0;
        t->resume_type  = TASK_RESUME_UNKNOW;
        t->wait_list    = NULL;
        t->next         = NULL;
    }

    /*
     * ��ʼ�����н���
     */
    t               = &task[0];
    t->pid          = 0;
    t->tid          = 0;
    t->state        = TASK_RUNNING;
    t->count        = 5;
    t->prio         = 5;

    t->content[0]   = (uint32_t)&t->kstack[KERN_STACK_SIZE];        /*  svc ģʽ�� sp (����ջ�ݼ�)      */
    t->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;       /*  cpsr, sys ģʽ, �� FIQ, �� IRQ  */
    t->content[2]   = PROCESS0_STACK_BASE;                          /*  sys ģʽ�� sp                   */
    t->content[3]   = ARM_SVC_MODE;                                 /*  spsr, svc ģʽ                  */
    t->content[17]  = 0;                                            /*  lr                              */
    t->content[18]  = 0;                                            /*  pc                              */

    current = t;

    tick = 0;

    /*
     * ��ʼ���ں˶�
     */
    heap_init(&task[0].heap, kern_heap_mem, KERN_HEAP_SIZE);
}

/*
 * ����
 */
void schedule(void)
{
    int32_t max = -1;
    int i, next = 0;
    task_t *t;

    while (1) {
        for (i = 1, t = &task[i]; i < TASK_NR; i++, t++) {
            if ((t->state == TASK_RUNNING) && (max < (int32_t)t->count)) {
                max  = (int32_t)t->count;
                next = i;
            }
        }

        if (max != 0) {
            break;
        }

        for (i = 0, t = &task[i]; i < TASK_NR; i++, t++) {
            t->count = (t->count >> 1) + t->prio;
        }
    }

    if (current == &task[next]) {
        return;
    }

    t = current;

    current = &task[next];

#if 1
    if ((current->content[3] & ARM_MODE_MASK) == ARM_SVC_MODE) {
        printk("%s: switch to pid=%d, tid=%d, pc=0x%x, sp_sys=0x%x, sp_svc=0x%x, irq save\n",
                __func__,
                current->pid,
                current->tid,
                current->content[18],
                current->content[2],
                current->content[0]);
    } else {
        printk("%s: switch to pid=%d, tid=%d, pc=0x%x, sp_sys=0x%x, sp_svc=0x%x, svc save\n",
                __func__,
                current->pid,
                current->tid,
                current->content[18],
                current->content[0],
                current->content[2]);
    }
#endif

    extern void __switch_to(register task_t *cur, register task_t *next);

    __switch_to(t, current);
}

/*
 * �ں˶�ʱ��������
 */
void do_timer(void)
{
    int i, wakeup = FALSE;
    task_t *t;

    tick++;

    for (i = 1, t = &task[i]; i < TASK_NR; i++, t++) {
        if (t->state == TASK_SLEEPING) {
            if (--t->timer == 0) {
                t->state = TASK_RUNNING;
                if (t->wait_list != NULL) {
                    task_t *prev = *t->wait_list;

                    if (t == prev) {
                        *t->wait_list = t->next;
                    } else {
                        while (prev != NULL && prev->next != t) {
                            prev = prev->next;
                        }

                        if (prev != NULL) {
                            prev->next = t->next;
                        }
                    }

                    t->next        = NULL;
                    t->wait_list   = NULL;
                    t->resume_type = TASK_RESUME_TIMEOUT;
                }
                wakeup = TRUE;
            }
        }
    }

    if (current->count > 0) {
        current->count--;
    }

    if ((current->state != TASK_RUNNING) || (current->count == 0) || wakeup) {
        schedule();
    }
}

/*
 * ��������
 */
int32_t process_create(uint8_t *code, uint32_t size, uint32_t prio)
{
    int i;
    task_t *t = &task[0];
    uint8_t *pa;

    if (code == NULL) {
        return -1;
    }

    for (i = 1, t = &task[i]; i <= PROCESS_NR; i++, t++){
        if (t->state == TASK_UNALLOCATE) {
            break;
        }
    }

    if (i > PROCESS_NR) {
        return -1;
    }

    pa = (uint8_t *)__virt_to_phy(0, i);

    memcpy(pa, code, size);

    mmu_drain_write_buffer();

    t->pid          = i;
    t->tid          = i;
    t->state        = TASK_RUNNING;
    t->count        = 15;
    t->prio         = prio;
#ifdef SMILEOS_KTHREAD
    t->type         = TASK_TYPE_PROCESS;
#endif

    t->content[0]   = (uint32_t)&t->kstack[KERN_STACK_SIZE];        /*  svc ģʽ��ջָ��(����ջ�ݼ�)    */
    t->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;       /*  cpsr, sys ģʽ, �� FIQ, �� IRQ  */
    t->content[2]   = PROCESS_MEM_SIZE;                             /*  sys ģʽ�� sp                   */
    t->content[3]   = ARM_SVC_MODE;                                 /*  spsr, svc ģʽ                  */
    t->content[17]  = 0;                                            /*  lr                              */
    t->content[18]  = 0;                                            /*  pc                              */

    return i;
}

#ifdef SMILEOS_KTHREAD
/*
 * �����߳�
 */
int32_t kthread_create(void (*func)(void *), void *arg, uint32_t stk_size, uint32_t prio)
{
    int i;
    task_t *t = &task[0];
    uint32_t stk;

    for (i = PROCESS_NR + 1, t = &task[i]; i < TASK_NR; i++, t++){
        if (t->state == TASK_UNALLOCATE) {
            break;
        }
    }

    if (i == TASK_NR) {
        return -1;
    }

    stk = (uint32_t)((char *)kmalloc(stk_size) + stk_size);

    t->pid          = current->pid;
    t->tid          = i;
    t->state        = TASK_RUNNING;
    t->count        = 15;
    t->prio         = prio;
    t->type         = TASK_TYPE_THREAD;

    t->content[0]   = (uint32_t)&t->kstack[KERN_STACK_SIZE];        /*  svc ģʽ��ջָ��(����ջ�ݼ�)    */
    t->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;       /*  cpsr, sys ģʽ, �� FIQ, �� IRQ  */
    t->content[2]   = stk;                                          /*  sys ģʽ�� sp                   */
    t->content[3]   = ARM_SVC_MODE;                                 /*  spsr, svc ģʽ                  */
    t->content[4]   = (uint32_t)arg;                                /*  r0 = arg                        */
    t->content[17]  = (uint32_t)func;                               /*  lr                              */
    t->content[18]  = (uint32_t)func;                               /*  pc                              */

    return i;
}
#endif

/*
 * printk
 */
void printk(const char *fmt, ...)
{
    static const char digits[] = "0123456789abcdef";
    va_list ap;
    char buf[10];
    char *s;
    unsigned u;
    int c, i, pad;

    va_start(ap, fmt);
    while ((c = *fmt++) != 0) {
        if (c == '%') {
            c = *fmt++;
            /* ignore long */
            if (c == 'l') {
                c = *fmt++;
            }
            switch (c) {
            case 'c':
                kputc(va_arg(ap, int));
                continue;
            case 's':
                s = va_arg(ap, char *);
                if (s == NULL) {
                    s = "<NULL>";
                }
                for (; *s; s++) {
                    kputc((int)*s);
                }
                continue;
            case 'd':
                c = 'u';
            case 'u':
            case 'x':
                u = va_arg(ap, unsigned);
                s = buf;
                if (c == 'u') {
                    do {
                        *s++ = digits[u % 10U];
                    } while (u /= 10U);
                } else {
                    pad = 0;
                    for (i = 0; i < 8; i++) {
                        if (pad) {
                            *s++ = '0';
                        } else {
                            *s++ = digits[u % 16U];
                            if ((u /= 16U) == 0) {
                                pad = 1;
                            }
                        }
                    }
                }
                while (--s >= buf) {
                    kputc((int)*s);
                }
                continue;
            }
        }
        if (c == '\n') {
            kputc('\r');
        }
        kputc((int)c);
    }
    va_end(ap);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
