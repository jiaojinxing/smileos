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
/*********************************************************************************************************
  内核变量
*********************************************************************************************************/
task_t          tasks[TASK_NR];                                         /*  任务控制块                  */
task_t         *current;                                                /*  指向当前运行的任务          */
static uint64_t tick;                                                   /*  TICK                        */
static uint8_t  interrupt_nest;                                         /*  中断嵌套层次                */
static uint8_t  running;                                                /*  内核是否正在运行            */
/*********************************************************************************************************
  内核函数
*********************************************************************************************************/
/*
 * 初始化内核变量
 */
static void kern_vars_init(void)
{
    task_t *task;
    int     i;

    running        = FALSE;                                             /*  内核还没启动                */
    interrupt_nest = 0;                                                 /*  中断嵌套层次为 0            */
    tick           = 0;                                                 /*  TICK 为 0                   */
    current        = &tasks[0];                                         /*  当前任务为进程 0            */

    /*
     * 初始化所有的任务控制块
     */
    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
        task->pid          = -1;
        task->tid          = -1;
        task->state        = TASK_UNALLOCATE;
        task->counter      = 0;
        task->timer        = 0;
        task->priority     = 0;
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
        task->thread       = NULL;
        task->arg          = NULL;
        memset(task->name, 0, sizeof(task->name));
    }
}

/*
 * 创建空闲进程
 */
static void idle_process_create(void)
{
    task_t *task;

    /*
     * 初始化进程控制块 0
     */
    task               = &tasks[0];
    task->pid          = 0;
    task->tid          = 0;
    task->state        = TASK_RUNNING;
    task->counter      = 20;
    task->timer        = 0;
    task->priority     = 20;
    task->type         = TASK_TYPE_PROCESS;                             /*  任务类型为进程              */
    task->errno        = 0;
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->utilization  = 0;
    task->frame_nr     = 0;
    task->stack        = 0;
    task->thread       = NULL;
    task->arg          = NULL;

    /*
     * 初始化进程上下文
     */
    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];      /*  svc 模式的 sp (满堆栈递减)  */
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;        /*  cpsr, sys 模式, 开 IRQ      */
    task->content[2]   = PROCESS0_STACK_BASE;                           /*  sys 模式的 sp               */
    task->content[3]   = ARM_SVC_MODE;                                  /*  spsr, svc 模式              */
    task->content[4]   = 0;                                             /*  lr                          */
    task->content[5]   = 0;                                             /*  r0 ~ r12                    */
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
    task->content[18]  = 0;                                             /*  pc                          */

    strcpy(task->name, "idle");
}

/*
 * 初始化内核
 */
void kernel_init(void)
{
    kern_vars_init();                                                   /*  初始化内核变量              */

    extern void kern_heap_create(void);
    kern_heap_create();                                                 /*  创建内核内存堆              */

    idle_process_create();                                              /*  创建空闲进程                */

    vmm_init();                                                         /*  初始化虚拟内存管理          */
}

/*
 * 启动内核
 */
void kernel_start(void)
{
    running = TRUE;                                                     /*  内核已经启动                */

    extern void __switch_to_process0(register uint32_t sp_svc);

    __switch_to_process0(current->content[0]);                          /*  切换到进程 0                */
}

/*
 * 任务调度
 * 调用之前必须关中断
 */
void schedule(void)
{
    int32_t max = -1;
    int     i;
    int     next = 0;
    task_t *task;

    if (!running) {                                                     /*  如果内核还没启动            */
        return;                                                         /*  直接返回                    */
    }

    if (interrupt_nest > 0) {                                           /*  如果还没完全退出中断        */
        return;                                                         /*  直接返回                    */
    }

    while (1) {
        for (i = 1, task = tasks + 1; i < TASK_NR; i++, task++) {       /*  任务 0 不参与竞争           */
            if ((task->state == TASK_RUNNING) && (max < (int32_t)task->counter)) {
                max  = (int32_t)task->counter;                          /*  用剩余时间片来做竞争        */
                next = i;
            }
        }

        if (max != 0) {                                                 /*  找到了一个有剩余时间片的任务*/
            break;                                                      /*  跳出                        */
        }

        for (i = 0, task = tasks; i < TASK_NR; i++, task++) {           /*  重置所有任务的剩余时间片    */
            task->counter = task->priority;
        }                                                               /*  再次查找                    */
    }

    if (current == &tasks[next]) {                                      /*  如果不需要切换任务          */
        return;                                                         /*  直接返回                    */
    }

    task    = current;                                                  /*  暂存 current 指针           */
    current = &tasks[next];                                             /*  改写 current 指针           */

    if ((current->content[3] & ARM_MODE_MASK) == ARM_SVC_MODE) {        /*  重新设置新任务的内核栈指针  */
        current->content[0] = (uint32_t)&current->kstack[KERN_STACK_SIZE];
    }

    extern void __switch_to(register task_t *from, register task_t *to);
    __switch_to(task, current);                                         /*  任务切换                    */
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

    current->tick++;                                                    /*  当前任务被中断的次数加一    */

    tick++;                                                             /*  内核 TICK 加一              */
    if (tick % TICK_PER_SECOND == 0) {                                  /*  如果已经过去了一秒          */
        flag = 1;
    }

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {               /*  遍历所有任务                */
        if (task->state != TASK_UNALLOCATE) {                           /*  如果任务有效                */
            if (flag) {
                task->utilization = task->tick;                         /*  任务的 CPU 占用率           */
                task->tick        = 0;                                  /*  重置该任务被定时器中断的次数*/
            }

            if (task->state == TASK_SLEEPING) {                         /*  如果任务正在休睡            */
                task->timer--;                                          /*  任务延时减一                */
                if (task->timer == 0) {                                 /*  如果任务延时到期            */
                    task->state = TASK_RUNNING;                         /*  任务进入就绪态              */
                    if (task->wait_list != NULL) {                      /*  如果任务在某个等待链表      */
                        task_t *prev = *task->wait_list;                /*  获得等待链表的链头任务      */
                        if (task == prev) {                             /*  如果任务就是链头任务        */
                            *task->wait_list = task->next;              /*  修改等待链表的链头          */
                        } else {
                            while (prev != NULL && prev->next != task) {/*  找出任务在等待链表中的前趋  */
                                prev = prev->next;
                            }
                            if (prev != NULL) {                         /*  如果有找到                  */
                                prev->next = task->next;                /*  修改前趋的后趋              */
                            }
                        }
                        task->next        = NULL;
                        task->wait_list   = NULL;
                        task->resume_type = TASK_RESUME_TIMEOUT;        /*  设置任务的恢复类型为超时    */
                    }
                }
            }
        }
    }

    if (current->counter > 0) {                                         /*  如果当前任务还有剩余时间片  */
        current->counter--;                                             /*  当前任务的剩余时间片减一    */
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

    interrupt_nest++;                                                   /*  中断嵌套层次加一            */

    interrupt_resume(reg);
}

/*
 * 退出中断
 */
void interrupt_exit(void)
{
    uint32_t reg;

    reg = interrupt_disable();

    interrupt_nest--;                                                   /*  中断嵌套层次减一            */
    if (interrupt_nest == 0) {                                          /*  如果已经完全退出了中断      */
        schedule();                                                     /*  任务调度                    */
    }
    interrupt_resume(reg);
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
            return FALSE;
        }
    }

    for (i = 0; bsp_resv_space[i].size != 0; i++) {
        high = max(bsp_resv_space[i].base, base);
        low  = min(bsp_resv_space[i].base + bsp_resv_space[i].size, end);
        if (high < low) {
            return FALSE;
        }
    }

    return TRUE;
}

/*
 * 创建进程
 */
int32_t process_create(const char *name, uint8_t *code, uint32_t size, uint32_t priority)
{
    int      i;
    task_t  *task;
    uint32_t reg;

    if (code == NULL || size == 0 || priority == 0) {
        return -1;
    }

    reg = interrupt_disable();

    /*
     * 分配进程控制块
     */
    for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {        /*  遍历所有的进程控制块        */
        if (task->state == TASK_UNALLOCATE) {                           /*  如果进程控制块无效          */
                                                                        /*  如果进程的虚拟地址空间可用  */
            if (virtual_space_usable(i * PROCESS_SPACE_SIZE, PROCESS_SPACE_SIZE)) {
                break;
            }
        }
    }

    if (i == PROCESS_NR) {                                              /*  没有空闲的进程控制块        */
        interrupt_resume(reg);
        return -1;
    }

    /*
     * 初始化进程控制块
     */
    task->pid          = i;
    task->tid          = i;
    task->state        = TASK_RUNNING;
    task->counter      = priority;
    task->timer        = 0;
    task->priority     = priority;
    task->type         = TASK_TYPE_PROCESS;                             /*  任务类型为进程              */
    task->errno        = 0;
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->utilization  = 0;
    task->frame_nr     = 0;
    task->stack        = 0;
    task->thread       = NULL;
    task->arg          = NULL;

    /*
     * 初始化任务上下文
     */
    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];      /*  svc 模式的 sp (满堆栈递减)  */
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;        /*  cpsr, sys 模式, 开 IRQ      */
    task->content[2]   = PROCESS_SPACE_SIZE;                            /*  sys 模式的 sp               */
    task->content[3]   = ARM_SVC_MODE;                                  /*  spsr, svc 模式              */
    task->content[4]   = 0;                                             /*  lr                          */
    task->content[5]   = 0;                                             /*  r0 ~ r12                    */
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
    task->content[18]  = 0;                                             /*  pc                          */

    if (name != NULL) {
        strcpy(task->name, name);
    } else {
        strcpy(task->name, "???");
    }

    /*
     * 为拷贝代码到进程的虚拟地址空间, 预先映射好页面
     */
    for (i = 0; i < (size + PAGE_SIZE - 1) / PAGE_SIZE; i++) {
        if (vmm_map_process_page(task, task->pid * PROCESS_SPACE_SIZE + i * PAGE_SIZE) < 0) {
            vmm_free_process_space(task);
            task->state = TASK_UNALLOCATE;
            interrupt_resume(reg);
            return -1;
        }
    }

    /*
     * 为进程栈空间映射一个页面
     */
    if (vmm_map_process_page(task, (task->pid + 1) * PROCESS_SPACE_SIZE - PAGE_SIZE) < 0) {
        vmm_free_process_space(task);
        task->state = TASK_UNALLOCATE;
        interrupt_resume(reg);
        return -1;
    }

    /*
     * 拷贝代码到进程的虚拟地址空间
     */
    memcpy((char *)(task->pid * PROCESS_SPACE_SIZE), code, size);

    interrupt_resume(reg);

    return i;
}

/*
 * 内核线程外壳
 */
static void kthread_shell(task_t *task)
{
    task->thread(task->arg);

    extern void exit(int error);
    exit(0);
}

/*
 * 创建内核线程
 */
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stk_size, uint32_t priority)
{
    int      i;
    task_t  *task;
    uint32_t reg;

    if (func == NULL) {
        return -1;
    }

    reg = interrupt_disable();

                                                                        /*  遍历所有的线程控制块        */
    for (i = PROCESS_NR, task = tasks + PROCESS_NR; i < TASK_NR; i++, task++) {
        if (task->state == TASK_UNALLOCATE) {                           /*  如果线程控制块无效          */
            break;
        }
    }

    if (i == TASK_NR) {                                                 /*  没有空闲的线程控制块        */
        interrupt_resume(reg);
        return -1;
    }

    if (priority < 2) {                                                 /*  优先级最小为 2              */
        stk_size = 2;
    }

    if (stk_size < 8 * KB) {                                            /*  堆栈空间最小为 8 KB         */
        stk_size = 8 * KB;
    }
    stk_size = MEM_ALIGN_SIZE(stk_size);                                /*  对齐堆栈空间大小            */

    task->stack = (uint32_t)kmalloc(stk_size);                          /*  分配堆栈空间                */
    if (task->stack == 0) {
        interrupt_resume(reg);
        return -1;
    }

    task->pid          = current->pid;                                  /*  进程 ID 为当前任务的进程 ID */
    task->tid          = i;
    task->state        = TASK_RUNNING;
    task->counter      = priority;
    task->timer        = 0;
    task->priority     = priority;
    task->type         = TASK_TYPE_THREAD;                              /*  任务类型为线程              */
    task->errno        = 0;
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->utilization  = 0;
    task->frame_nr     = 0;
    task->thread       = func;
    task->arg          = arg;

    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];      /*  svc 模式堆栈指针(满堆栈递减)*/
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;        /*  cpsr, sys 模式, 开 IRQ      */
    task->content[2]   = task->stack + stk_size;                        /*  sys 模式的 sp               */
    task->content[3]   = ARM_SVC_MODE;                                  /*  spsr, svc 模式              */
    task->content[4]   = (uint32_t)kthread_shell;                       /*  lr                          */
    task->content[5]   = (uint32_t)task;                                /*  r0 ~ r12                    */
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
    task->content[18]  = (uint32_t)kthread_shell;                       /*  pc                          */

    if (name != NULL) {
        strcpy(task->name, name);
    } else {
        strcpy(task->name, "???");
    }

    interrupt_resume(reg);

    return i;
}

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
 * 判断是否在中断处理中
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
 * printk
 */
void printk(const char *fmt, ...)
{
#if 0
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
#endif

    va_list va;
    char    buf[256];
    int     i;

    va_start(va, fmt);

    vsprintf(buf, fmt, va);

    i = 0;
    while (buf[i]) {
        kputc(buf[i]);
        i++;
    }

    va_end(va);

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
