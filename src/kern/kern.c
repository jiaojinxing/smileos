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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/arm.h"
#include "kern/vmm.h"
#include <string.h>
#include <stdarg.h>

/*
 * 任务控制块
 */
static task_t tasks[TASK_NR];

/*
 * 当前运行的任务
 */
task_t *current;

/*
 * TICK
 */
uint64_t tick;

/*
 * 内核内存堆
 */
static uint8_t kern_heap_mem[KERN_HEAP_SIZE];

/*
 * 内核变量
 */
static uint8_t int_level = 0;
static uint8_t started   = FALSE;
static uint8_t wakeup    = FALSE;

/*
 * 从内核内存堆分配内存
 */
void *kmalloc(uint32_t size)
{
    void    *ptr;
    uint32_t reg;

    reg = interrupt_disable();

    ptr = heap_alloc(&tasks[0].heap, size);

    interrupt_resume(reg);

    return ptr;
}

/*
 * 释放内存回内核内存堆
 */
void kfree(void *ptr)
{
    uint32_t reg;

    reg = interrupt_disable();

    heap_free(&tasks[0].heap, ptr);

    interrupt_resume(reg);
}

/*
 * 初始化调度器
 */
void sched_init(void)
{
    task_t *task;
    int     i;

    int_level = 0;
    tick      = 0;
    started   = FALSE;
    wakeup    = FALSE;

    /*
     * 初始化任务控制块
     */
    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
        task->pid          = -1;
        task->tid          = -1;
        task->state        = TASK_UNALLOCATE;
        task->count        = 0;
        task->timer        = 0;
        task->prio         = 0;
        task->errno        = 0;
        task->resume_type  = TASK_RESUME_UNKNOW;
        task->wait_list    = NULL;
        task->next         = NULL;
    }

    /*
     * 初始化空闲进程
     */
    task               = tasks;
    task->pid          = 0;
    task->tid          = 0;
    task->state        = TASK_RUNNING;
    task->count        = 5;
    task->prio         = 5;

    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];  /*  svc 模式的 sp (满堆栈递减)      */
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;    /*  cpsr, sys 模式, 关 FIQ, 开 IRQ  */
    task->content[2]   = PROCESS0_STACK_BASE;                       /*  sys 模式的 sp                   */
    task->content[3]   = ARM_SVC_MODE;                              /*  spsr, svc 模式                  */
    task->content[4]   = 0;                                         /*  lr                              */
    task->content[5]   = 0;                                         /*  r0 ~ r12                        */
    task->content[6]   = 1;
    task->content[7]   = 2;
    task->content[8]   = 3;
    task->content[9]   = 4;
    task->content[10]  = 5;
    task->content[11]  = 6;
    task->content[12]  = 7;
    task->content[13]  = 8;
    task->content[14]  = 9;
    task->content[15]  = 10;
    task->content[16]  = 11;
    task->content[17]  = 12;
    task->content[18]  = 0;                                         /*  pc                              */

    current = task;

    /*
     * 初始化内核内存堆
     */
    heap_init(&tasks[0].heap, kern_heap_mem, KERN_HEAP_SIZE);

    /*
     * 初始化虚拟内存管理
     */
    vmm_init();
}

/*
 * 任务调度, 调用之前必须关中断
 */
void schedule(void)
{
    int32_t max = -1;
    int     i, next = 0;
    task_t *task;

    if (!started) {
        return;
    }

    if (int_level > 0) {
        return;
    }

    while (1) {
        for (i = 1, task = tasks + 1; i < TASK_NR; i++, task++) {
            if ((task->state == TASK_RUNNING) && (max < (int32_t)task->count)) {
                max  = (int32_t)task->count;
                next = i;
            }
        }

        if (max != 0) {
            break;
        }

        for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
            task->count = (task->count >> 1) + task->prio;
        }
    }

    if (current == &tasks[next]) {
        return;
    }

    task = current;

    current = &tasks[next];

    if ((current->content[3] & ARM_MODE_MASK) == ARM_SVC_MODE) {        /*  重新设置该任务的内核栈地址  */
        current->content[0] = (uint32_t)&current->kstack[KERN_STACK_SIZE];
    }

#if 0
    if ((current->content[3] & ARM_MODE_MASK) == ARM_SVC_MODE) {
        printk("%s: switch to pid=%d, tid=%d, "
                "pc=0x%x, lr=0x%x, cpsr=0x%x, sp_sys=0x%x, sp_svc=0x%x, "
                "r0=0x%x, r1=0x%x, r2=0x%x, r3=0x%x, r4=0x%x, r5=0x%x, r6=0x%x, r7=0x%x, r8=0x%x, r9=0x%x, r10=0x%x, r11=0x%x, r12=0x%x, "
                "irq save\n",
                __func__,
                current->pid,
                current->tid,
                current->content[18],
                current->content[4],
                current->content[1],
                current->content[2],
                current->content[0],
                current->content[5],
                current->content[6],
                current->content[7],
                current->content[8],
                current->content[9],
                current->content[10],
                current->content[11],
                current->content[12],
                current->content[13],
                current->content[14],
                current->content[15],
                current->content[16],
                current->content[17]);
    } else {
        printk("%s: switch to pid=%d, tid=%d, "
                "pc=0x%x, lr=0x%x, cpsr=0x%x, sp_sys=0x%x, sp_svc=0x%x, "
                "r0=0x%x, r1=0x%x, r2=0x%x, r3=0x%x, r4=0x%x, r5=0x%x, r6=0x%x, r7=0x%x, r8=0x%x, r9=0x%x, r10=0x%x, r11=0x%x, r12=0x%x, "
                "svc save\n",
                __func__,
                current->pid,
                current->tid,
                current->content[18],
                current->content[4],
                current->content[1],
                current->content[0],
                current->content[2],
                current->content[5],
                current->content[6],
                current->content[7],
                current->content[8],
                current->content[9],
                current->content[10],
                current->content[11],
                current->content[12],
                current->content[13],
                current->content[14],
                current->content[15],
                current->content[16],
                current->content[17]);
    }
#endif

    extern void __switch_to(register task_t *from, register task_t *to);

    __switch_to(task, current);
}

/*
 * 内核定时器处理函数
 */
void do_timer(void)
{
    int      i;
    task_t  *t;
    uint32_t reg;

    reg = interrupt_disable();

    tick++;
    wakeup = FALSE;

    for (i = 1, t = tasks + 1; i < TASK_NR; i++, t++) {
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

    interrupt_resume(reg);
}

/*
 * 进入中断
 */
void interrupt_enter(void)
{
    uint32_t reg;

    reg = interrupt_disable();

    int_level++;

    interrupt_resume(reg);
}

/*
 * 退出中断
 */
void interrupt_exit(void)
{
    uint32_t reg;

    reg = interrupt_disable();

    if (--int_level == 0) {
        if ((current->state != TASK_RUNNING) || (current->count == 0) || wakeup) {
            wakeup = FALSE;
            schedule();
        }
    }
    interrupt_resume(reg);
}

/*
 * 启动调度器
 */
void sched_start(void)
{
    started = TRUE;

    void __switch_to_process0(register uint32_t sp_svc);

    __switch_to_process0(current->content[0]);
}

/*
 * 创建进程
 */
int32_t process_create(uint8_t *code, uint32_t size, uint32_t prio)
{
    int      i;
    task_t  *task;
    uint32_t reg;

    if (code == NULL || size == 0) {
        return -1;
    }

    reg = interrupt_disable();

    for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++){
        if (task->state == TASK_UNALLOCATE) {
            break;
        }
    }

    if (i == PROCESS_NR) {
        interrupt_resume(reg);
        return -1;
    }

    task->pid          = i;
    task->tid          = i;
    task->state        = TASK_RUNNING;
    task->count        = 15;
    task->prio         = prio;
#ifdef SMILEOS_KTHREAD
    task->type         = TASK_TYPE_PROCESS;
#endif

    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];  /*  svc 模式的 sp (满堆栈递减)      */
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;    /*  cpsr, sys 模式, 关 FIQ, 开 IRQ  */
    task->content[2]   = PROCESS_SPACE_SIZE;                        /*  sys 模式的 sp                   */
    task->content[3]   = ARM_SVC_MODE;                              /*  spsr, svc 模式                  */
    task->content[4]   = 0;                                         /*  lr                              */
    task->content[5]   = 0;                                         /*  r0 ~ r12                        */
    task->content[6]   = 1;
    task->content[7]   = 2;
    task->content[8]   = 3;
    task->content[9]   = 4;
    task->content[10]  = 5;
    task->content[11]  = 6;
    task->content[12]  = 7;
    task->content[13]  = 8;
    task->content[14]  = 9;
    task->content[15]  = 10;
    task->content[16]  = 11;
    task->content[17]  = 12;
    task->content[18]  = 0;                                         /*  pc                              */

    /*
     * 为拷贝代码到进程的地址空间, 预先映射好页面
     */
    for (i = 0; i < (size + PAGE_SIZE - 1) / PAGE_SIZE; i++) {
        process_space_page_map(task, task->pid * PROCESS_SPACE_SIZE + i * PAGE_SIZE);
    }

    /*
     * 为进程栈空间映射一个页面
     */
    process_space_page_map(task, (task->pid + 1) * PROCESS_SPACE_SIZE - PAGE_SIZE);

    memcpy((char *)(task->pid * PROCESS_SPACE_SIZE), code, size);

    schedule();

    interrupt_resume(reg);

    return i;
}

#ifdef SMILEOS_KTHREAD
/*
 * 创建内核线程
 */
int32_t kthread_create(void (*func)(void *), void *arg, uint32_t stk_size, uint32_t prio)
{
    int      i;
    task_t  *task;
    uint32_t stk;
    uint32_t reg;

    reg = interrupt_disable();

    for (i = PROCESS_NR, task = tasks + PROCESS_NR; i < TASK_NR; i++, task++) {
        if (task->state == TASK_UNALLOCATE) {
            break;
        }
    }

    if (i == TASK_NR) {
        interrupt_resume(reg);
        return -1;
    }

    stk = (uint32_t)((char *)kmalloc(stk_size) + stk_size);

    task->pid          = current->pid;
    task->tid          = i;
    task->state        = TASK_RUNNING;
    task->count        = 15;
    task->prio         = prio;
    task->type         = TASK_TYPE_THREAD;

    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];  /*  svc 模式堆栈指针(满堆栈递减)    */
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;    /*  cpsr, sys 模式, 关 FIQ, 开 IRQ  */
    task->content[2]   = stk;                                       /*  sys 模式的 sp                   */
    task->content[3]   = ARM_SVC_MODE;                              /*  spsr, svc 模式                  */
    task->content[4]   = (uint32_t)func;                            /*  lr                              */
    task->content[5]   = (uint32_t)arg;                             /*  r0 ~ r12                        */
    task->content[6]   = 1;
    task->content[7]   = 2;
    task->content[8]   = 3;
    task->content[9]   = 4;
    task->content[10]  = 5;
    task->content[11]  = 6;
    task->content[12]  = 7;
    task->content[13]  = 8;
    task->content[14]  = 9;
    task->content[15]  = 10;
    task->content[16]  = 11;
    task->content[17]  = 12;
    task->content[18]  = (uint32_t)func;;                           /*  pc                              */

    schedule();

    interrupt_resume(reg);

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
