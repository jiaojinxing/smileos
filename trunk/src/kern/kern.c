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
** Descriptions:            内核源文件
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
#include "config.h"
#include "types.h"
#include "kern.h"
#include "arm.h"
#include <string.h>
#include <stdarg.h>

/*
 * 任务控制块
 */
task_t task[TASK_NR];

/*
 * 当前运行的任务
 */
task_t *current;

/*
 * TICK
 */
uint64_t tick;

/*
 * 初始化调度器
 */
void sched_init(void)
{
    task_t *p;
    int i;

    /*
     * 初始化任务控制块
     */
    for (i = 0, p = &task[i]; i < TASK_NR; i++, p++) {
        p->pid      = -1;
        p->tid      = -1;
        p->state    = TASK_UNALLOCATE;
        p->count    = 0;
        p->timer    = 0;
        p->priority = 0;
        p->errno    = 0;
    }

    /*
     * 初始化空闲进程
     */
    p               = &task[0];
    p->pid          = 0;
    p->tid          = 0;
    p->state        = TASK_RUNNING;
    p->count        = 5;
    p->priority     = 5;

    p->content[0]   = (uint32_t)&p->kstack[KERN_STACK_SIZE];        /*  svc 模式的 sp (满堆栈递减)      */
    p->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;       /*  cpsr, sys 模式, 关 FIQ, 开 IRQ  */
    p->content[2]   = KERN_MEM_BASE + KERN_MEM_SIZE;                /*  sys 模式的 sp                   */
    p->content[3]   = ARM_SVC_MODE;                                 /*  spsr, svc 模式                  */
    p->content[17]  = 0;                                            /*  pc                              */

    current = p;

    tick = 0;
}

/*
 * 调度
 */
void schedule(void)
{
    int32_t max = -1;
    int i, next = 0;
    task_t *p;

    while (1) {
        for (i = 1, p = &task[i]; i < TASK_NR; i++, p++) {
            if ((p->state == TASK_RUNNING) && (max < (int32_t)p->count)) {
                max  = (int32_t)p->count;
                next = i;
            }
        }

        if (max != 0) {
            break;
        }

        for (i = 0, p = &task[i]; i < TASK_NR; i++, p++) {
            p->count = (p->count >> 1) + p->priority;
        }
    }

    if (current == &task[next]) {
        return;
    }

    p = current;

    current = &task[next];

#if 0
    printk("%s: switch to pid=%d, tid=%d, pc=0x%x, sp_sys=0x%x, sp_svc=0x%x\n",
            __func__
            current->pid,
            current->tid,
            current->content[17],
            current->content[2],
            current->content[0]);
#endif

    extern void __switch_to(task_t *cur, task_t *next);

    __switch_to(p, current);
}

/*
 * 内核定时器处理函数
 */
void do_timer(void)
{
    int i, wakeup = 0;
    task_t *p;

    tick++;

    for (i = 1, p = &task[i]; i < TASK_NR; i++, p++) {
        if (p->state == TASK_SLEEPING) {
            if (--p->timer == 0) {
                p->state = TASK_RUNNING;
                wakeup = 1;
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
 * 创建进程
 */
int create_process(uint8_t *code, uint32_t size, uint32_t priority)
{
    int i;
    task_t *p = &task[0];
    uint8_t *pa;

    if (code == NULL) {
        return -1;
    }

    for (i = 1, p = &task[i]; i <= PROCESS_NR; i++, p++){
        if (p->state == TASK_UNALLOCATE) {
            break;
        }
    }

    if (i > PROCESS_NR) {
        return -1;
    }

    pa = (uint8_t *)__virt_to_phy(0, i);

    memcpy(pa, code, size);

    p->pid          = i;
    p->tid          = i;
    p->state        = TASK_RUNNING;
    p->count        = 15;
    p->priority     = priority;
#ifdef SMILEOS_KTHREAD
    p->type         = TASK_TYPE_PROCESS;
#endif

    p->content[0]   = (uint32_t)&p->kstack[KERN_STACK_SIZE];        /*  svc 模式堆栈指针(满堆栈递减)    */
    p->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;       /*  cpsr, sys 模式, 关 FIQ, 开 IRQ  */
    p->content[2]   = PROCESS_MEM_SIZE;                             /*  sys 模式的 sp                   */
    p->content[3]   = ARM_SVC_MODE;                                 /*  spsr, svc 模式                  */
    p->content[17]  = 0;                                            /*  pc                              */

    return 0;
}

#ifdef SMILEOS_KTHREAD
/*
 * 创建线程
 */
int create_thread(uint32_t pc, uint32_t sp, uint32_t priority)
{
    int i;
    task_t *p = &task[0];

    for (i = PROCESS_NR + 1, p = &task[i]; i < TASK_NR; i++, p++){
        if (p->state == TASK_UNALLOCATE) {
            break;
        }
    }

    if (i == TASK_NR) {
        return -1;
    }

    p->pid          = current->pid;
    p->tid          = i;
    p->state        = TASK_RUNNING;
    p->count        = 15;
    p->priority     = priority;
    p->type         = TASK_TYPE_THREAD;

    p->content[0]   = (uint32_t)&p->kstack[KERN_STACK_SIZE];        /*  svc 模式堆栈指针(满堆栈递减)    */
    p->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;       /*  cpsr, sys 模式, 关 FIQ, 开 IRQ  */
    p->content[2]   = sp;                                           /*  sys 模式的 sp                   */
    p->content[3]   = ARM_SVC_MODE;                                 /*  spsr, svc 模式                  */
    p->content[17]  = pc;                                           /*  pc                              */

    return 0;
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
