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
#include "kern/heap.h"
#include <string.h>
#include <stdarg.h>

/*
 * 任务控制块
 */
task_t tasks[TASK_NR];

/*
 * 当前运行的任务
 */
task_t *current;

/*
 * 内核内存堆
 */
static heap_t  kernel_heap;
static uint8_t kernel_heap_mem[KERN_HEAP_SIZE];

/*
 * 内核变量
 */
static uint64_t tick           = 0;
static uint8_t  interrupt_nest = 0;
static uint8_t  kernel_running = FALSE;

/*
 * 获得 TICK
 */
uint64_t get_tick(void)
{
    uint64_t __tick;
    uint32_t reg;

    reg = interrupt_disable();

    __tick = tick;

    interrupt_resume(reg);

    return __tick;
}

/*
 * 是否在中断处理中
 */
int in_interrupt(void)
{
    int      ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = interrupt_nest > 0 ? TRUE : FALSE;

    interrupt_resume(reg);

    return ret;
}

/*
 * 从内核内存堆分配内存
 */
void *kmalloc(uint32_t size)
{
    void    *ptr;
    uint32_t reg;

    reg = interrupt_disable();

    ptr = heap_alloc(&kernel_heap, size);

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

    heap_free(&kernel_heap, ptr);

    interrupt_resume(reg);
}

/*
 * kcalloc
 */
void *kcalloc(uint32_t nelem, uint32_t elsize)
{
    void *ptr;

    ptr = kmalloc(nelem * MEM_ALIGN_SIZE(elsize));
    if (ptr != NULL) {
        memset(ptr, 0, nelem * MEM_ALIGN_SIZE(elsize));
    }
    return ptr;
}

/*
 * 初始化内核
 */
void kernel_init(void)
{
    task_t *task;
    int     i;

    interrupt_nest = 0;
    tick           = 0;
    kernel_running = FALSE;

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
        task->type         = TASK_TYPE_PROCESS;
        task->errno        = 0;
        task->resume_type  = TASK_RESUME_UNKNOW;
        task->next         = NULL;
        task->wait_list    = NULL;
        task->frame_list   = NULL;
        task->tick         = 0;
        task->utilization  = 0;
        task->frame_nr     = 0;
        task->stack        = 0;
        memset(task->name, 0, sizeof(task->name));
    }

    /*
     * 初始化空闲进程
     */
    task               = tasks;
    task->pid          = 0;
    task->tid          = 0;
    task->state        = TASK_RUNNING;
    task->count        = 5;
    task->timer        = 0;
    task->prio         = 5;
    task->type         = TASK_TYPE_PROCESS;
    task->errno        = 0;
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->utilization  = 0;
    task->frame_nr     = 0;
    task->stack        = 0;

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

    strcpy(task->name, "idle");

    current = task;

    /*
     * 初始化内核内存堆
     */
    heap_init(&kernel_heap, kernel_heap_mem, KERN_HEAP_SIZE);

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
    int     i;
    int     next = 0;
    task_t *task;

    if (!kernel_running) {
        return;
    }

    if (interrupt_nest > 0) {
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
            task->count = task->prio;
        }
    }

    if (current == &tasks[next]) {
        return;
    }

    task = current;

    current = &tasks[next];

    if ((current->content[3] & ARM_MODE_MASK) == ARM_SVC_MODE) {        /*  重新设置该任务的内核栈指针  */
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
    task_t  *task;
    uint32_t reg;
    int      flag = 0;

    reg = interrupt_disable();

    tick++;

    current->tick++;

    if (tick % TICK_PER_SECOND == 0) {
        flag = 1;
    }

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
        if (task->state != TASK_UNALLOCATE) {
            if (flag) {
                task->utilization = task->tick;
                task->tick        = 0;
            }

            if (task->state == TASK_SLEEPING) {
                if (--task->timer == 0) {
                    task->state = TASK_RUNNING;
                    if (task->wait_list != NULL) {
                        task_t *prev = *task->wait_list;
                        if (task == prev) {
                            *task->wait_list = task->next;
                        } else {
                            while (prev != NULL && prev->next != task) {
                                prev = prev->next;
                            }
                            if (prev != NULL) {
                                prev->next = task->next;
                            }
                        }
                        task->next        = NULL;
                        task->wait_list   = NULL;
                        task->resume_type = TASK_RESUME_TIMEOUT;
                    }
                }
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

    interrupt_nest++;

    interrupt_resume(reg);
}

/*
 * 退出中断
 */
void interrupt_exit(void)
{
    uint32_t reg;

    reg = interrupt_disable();

    if (--interrupt_nest == 0) {
        schedule();
    }
    interrupt_resume(reg);
}

/*
 * 启动内核
 */
void kernel_start(void)
{
    kernel_running = TRUE;

    extern void __switch_to_process0(register uint32_t sp_svc);

    __switch_to_process0(current->content[0]);
}

/*
 * 判断虚拟地址空间是否可用
 */
int virtual_space_usable(uint32_t base, uint32_t size)
{
    uint32_t end = base + size;
    uint32_t high, low;
    int i;

    extern virtual_space_t sys_resv_space[];
    extern virtual_space_t bsp_resv_space[];

#define max(a, b)   (a) > (b) ? (a) : (b)
#define min(a, b)   (a) < (b) ? (a) : (b)

    for (i = 0; sys_resv_space[i].size != 0; i++) {
        high = max(sys_resv_space[i].base, base);
        low  = min(sys_resv_space[i].base + sys_resv_space[i].size, end);
        if (high < low) {
            return 0;
        }
    }

    for (i = 0; bsp_resv_space[i].size != 0; i++) {
        high = max(bsp_resv_space[i].base, base);
        low  = min(bsp_resv_space[i].base + bsp_resv_space[i].size, end);
        if (high < low) {
            return 0;
        }
    }

    return 1;
}

/*
 * 创建进程
 */
int32_t process_create(const char *name, uint8_t *code, uint32_t size, uint32_t prio)
{
    int      i;
    task_t  *task;
    uint32_t reg;

    if (code == NULL || size == 0) {
        return -1;
    }

    reg = interrupt_disable();

    for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {
        if (task->state == TASK_UNALLOCATE) {
            if (virtual_space_usable(i * PROCESS_SPACE_SIZE, PROCESS_SPACE_SIZE)) {
                break;
            }
        }
    }

    if (i == PROCESS_NR) {
        interrupt_resume(reg);
        return -1;
    }

    task->pid          = i;
    task->tid          = i;
    task->state        = TASK_RUNNING;
    task->count        = 5;
    task->timer        = 0;
    task->prio         = prio;
    task->type         = TASK_TYPE_PROCESS;
    task->errno        = 0;
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->utilization  = 0;
    task->frame_nr     = 0;
    task->stack        = 0;

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

    if (name != NULL) {
        strcpy(task->name, name);
    } else {
        strcpy(task->name, "???");
    }

    /*
     * 为拷贝代码到进程的虚拟地址空间, 预先映射好页面
     */
    for (i = 0; i < (size + PAGE_SIZE - 1) / PAGE_SIZE; i++) {
        /*
         * TODO: 出错处理
         */
        vmm_map_process_page(task, task->pid * PROCESS_SPACE_SIZE + i * PAGE_SIZE);
    }

    /*
     * 为进程栈空间映射一个页面
     */
    vmm_map_process_page(task, (task->pid + 1) * PROCESS_SPACE_SIZE - PAGE_SIZE);

    memcpy((char *)(task->pid * PROCESS_SPACE_SIZE), code, size);

    interrupt_resume(reg);

    return i;
}

/*
 * 创建内核线程
 */
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stk_size, uint32_t prio)
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

    if (stk_size < 8 * KB) {
        stk_size = 8 * KB;
    }
    stk_size = MEM_ALIGN_SIZE(stk_size);

    task->stack = (uint32_t)kmalloc(stk_size);
    if (task->stack == 0) {
        interrupt_resume(reg);
        return -1;
    }

    stk                = task->stack + stk_size;

    task->pid          = current->pid;
    task->tid          = i;
    task->state        = TASK_RUNNING;
    task->count        = 5;
    task->timer        = 0;
    task->prio         = prio;
    task->type         = TASK_TYPE_THREAD;
    task->errno        = 0;
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->utilization  = 0;
    task->frame_nr     = 0;

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
    task->content[18]  = (uint32_t)func;                            /*  pc                              */

    if (name != NULL) {
        strcpy(task->name, name);
    } else {
        strcpy(task->name, "???");
    }

    interrupt_resume(reg);

    return i;
}

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
