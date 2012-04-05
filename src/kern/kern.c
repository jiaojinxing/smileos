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
** Modified by:             JiaoJinXing
** Modified date:           2012-3-25
** Version:                 1.1.0
** Descriptions:            加入内核模式和内核日志线程及 interrupt_exit_no_sched 函数,
**                          strcpy 改用更安全的 strlcpy,
**                          拷贝代码到进程的虚拟地址空间时开中断以提高实时性
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-26
** Version:                 1.2.0
** Descriptions:            加入进程虚拟地址空间的保护和内核线程栈溢出检查
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-28
** Version:                 1.3.0
** Descriptions:            按 newlib 需求, 任务控制块增加 struct _reent 成员,
**                          任务切换时也切换 _impure_ptr 指针
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-31
** Version:                 1.4.0
** Descriptions:            修改任务调度算法的一处错误: 重置所有就绪任务的剩余时间片而不是所有任务,
**                          内核线程的调度算法改为基于优先级而非剩余时间片,
**                          为保障中断底半部处理及时地运行, 请把中断底半部处理线程的优先级抬到较高水平
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/arm.h"
#include "kern/vmm.h"
#include "kern/mmu.h"
#include "vfs/vfs.h"
#include <string.h>
#include <fcntl.h>
/*********************************************************************************************************
  内核变量
*********************************************************************************************************/
task_t              tasks[TASK_NR];                                     /*  任务控制块                  */
task_t             *current;                                            /*  指向当前运行的任务          */
static uint64_t     tick;                                               /*  TICK                        */
static uint8_t      interrupt_nest;                                     /*  中断嵌套层次                */
static uint8_t      running;                                            /*  内核是否正在运行            */
uint8_t             kernel_mode;                                        /*  当前是否处于内核模式        */

#define THREAD_STACK_MAGIC0         0xAA                                /*  内核线程栈魔数              */

static void idle_create(void);
/*********************************************************************************************************
  内核函数
*********************************************************************************************************/
/*
 * 初始化内核变量
 */
static void kvars_init(void)
{
    task_t *task;
    int     i;

    running        = FALSE;                                             /*  内核还没启动                */
    kernel_mode    = FALSE;                                             /*  当前不在内核模式            */
    interrupt_nest = 0;                                                 /*  中断嵌套层次为 0            */
    tick           = 0;                                                 /*  TICK 为 0                   */
    current        = &tasks[0];                                         /*  当前任务为进程 0            */
    _impure_ptr    = &current->reent;

    /*
     * 初始化所有的任务控制块
     */
    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
        memset(task, 0, sizeof(task_t));
        task->state = TASK_UNALLOCATE;
    }
}

/*
 * 初始化内核
 */
void kernel_init(void)
{
    mmu_init();                                                         /*  初始化 MMU                  */

    kvars_init();                                                       /*  初始化内核变量              */

    extern void kheap_create(void);
    kheap_create();                                                     /*  创建内核内存堆              */

    idle_create();                                                      /*  创建空闲进程                */

    extern void klogd_create(void);
    klogd_create();                                                     /*  创建内核日志线程            */

    vmm_init();                                                         /*  初始化虚拟内存管理          */
}

/*
 * 启动内核
 */
void kernel_start(void)
{
    running = TRUE;                                                     /*  内核已经启动                */

    extern void __switch_to_process0(register uint32_t sp_svc);
    __switch_to_process0(current->content[0]);                          /*  切换到进程 0 (空闲进程)     */
}

/*
 * 任务调度
 * 调用之前必须关中断
 */
void schedule(void)
{
    int     i;
    int     next = 0;
    int     flag = FALSE;
    int32_t max  = -1;
    task_t *task;

    if (!running) {                                                     /*  如果内核还没启动            */
        return;                                                         /*  直接返回                    */
    }

    if (interrupt_nest > 0) {                                           /*  如果还没完全退出中断        */
        return;                                                         /*  直接返回                    */
    }

    kernel_mode = FALSE;                                                /*  退出内核模式                */

    while (1) {
        /*
         * 先做内核线程调度, 再做进程调度
         */
        for (i = PROCESS_NR, task = tasks + PROCESS_NR; i < TASK_NR; i++, task++) {
            if ((task->state == TASK_RUNNING) && (max < (int32_t)task->priority)) {
                max  = (int32_t)task->priority;                         /*  用优先级来做竞争            */
                next = i;
            }
        }

        if (max > 0) {                                                  /*  找到了一个就绪的线程        */
            break;
        }

        for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {    /*  进程 0 不参与竞争           */
            if ((task->state == TASK_RUNNING) && (max < (int32_t)task->counter)) {
                max  = (int32_t)task->counter;                          /*  用剩余时间片来做竞争        */
                next = i;
            }
        }

        if (max > 0) {                                                  /*  找到了一个有剩余时间片的进程*/
            break;
        } else if (flag) {                                              /*  如果没有一个任务就绪        */
            next = 0;                                                   /*  则运行空闲进程              */
            break;
        }

        for (i = 0, task = tasks; i < TASK_NR; i++, task++) {           /*  重置所有就绪任务的剩余时间片*/
            if (task->state == TASK_RUNNING) {
                task->counter = task->priority;
            }
        }

        flag = TRUE;
    }

    if (current == &tasks[next]) {                                      /*  如果不需要切换任务          */
        return;                                                         /*  直接返回                    */
    }

    task        = current;                                              /*  暂存 current 指针           */
    current     = &tasks[next];                                         /*  改写 current 指针           */
    _impure_ptr = &current->reent;                                      /*  改写 _impure_ptr 指针       */

    if ((current->content[3] & ARM_MODE_MASK) == ARM_SVC_MODE) {        /*  重新设置新任务的内核栈指针  */
        current->content[0] = (uint32_t)&current->kstack[KERN_STACK_SIZE];
    }

    if (task->pid != current->pid) {                                    /*  如果需要切换进程            */
        for (i = 0; i < PROCESS_SPACE_SIZE / SECTION_SIZE; i++) {       /*  保护原进程的虚拟地址空间    */
            mmu_map_section(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + i, 0);
        }

        for (i = 0; i < PROCESS_SPACE_SIZE / SECTION_SIZE; i++) {       /*  恢复新进程的一级段表        */
            mmu_map_section(current->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + i, current->mmu_backup[i]);
        }
    }

    extern void __switch_to(register task_t *from, register task_t *to);
    __switch_to(task, current);                                         /*  任务切换                    */
}

/*
 * 内核定时器处理函数
 */
void kernel_timer(void)
{
    int      i;
    int      flag;
    task_t  *task;
    uint32_t reg;

    reg = interrupt_disable();

    current->tick++;                                                    /*  当前任务被中断的次数加一    */

    tick++;                                                             /*  内核 TICK 加一              */
    if (tick % TICK_PER_SECOND == 0) {                                  /*  如果已经过去了一秒          */
        flag = TRUE;
    } else {
        flag = FALSE;
    }

    if (current->counter > 0) {                                         /*  如果当前任务还有剩余时间片  */
        current->counter--;                                             /*  当前任务的剩余时间片减一    */
    }

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {               /*  遍历所有任务                */
        if (task->state != TASK_UNALLOCATE) {                           /*  如果任务有效                */
            /*
             * TODO: 这样统计任务的 CPU 占用率不是很准确
             */
            if (flag) {
                task->cpu_rate = task->tick;                            /*  任务的 CPU 占用率           */
                task->tick     = 0;                                     /*  重置该任务被定时器中断的次数*/

                if (task->type == TASK_TYPE_THREAD) {                   /*  如果任务是内核线程          */
                    /*
                     * 统计内核线程的栈使用率, 栈溢出检查
                     *
                     * 注意: 如果加了编译参数 -fsigned-char, char 是有符号的!
                     *
                     * THREAD_STACK_MAGIC0 = 0xAA, 必须要用 uint8_t 类型, 否则...
                     */
                    uint8_t *p   = (uint8_t *)task->stack_base;
                    uint8_t *end = (uint8_t *)(task->stack_base + task->stack_size);

                    while (*p == THREAD_STACK_MAGIC0 && p < end) {
                        p++;
                    }

                    if (p < end) {
                        task->stack_rate = 100 * ((uint32_t)(end - p)) / task->stack_size;
                    } else {
                        printk("kthread %s tid=%d stack overflow!\n", task->name, task->tid);
                        task_kill(task->tid);
                        continue;
                    }
                }
            }

            if (task->state == TASK_SLEEPING) {                         /*  如果任务正在休睡            */
                task->timer--;                                          /*  任务延时减一                */
                if (task->timer == 0) {                                 /*  如果任务延时到期            */
                    task->state       = TASK_RUNNING;                   /*  任务进入就绪态              */
                    task->resume_type = TASK_RESUME_TIMEOUT;            /*  设置任务的恢复类型为超时    */
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
                        task->next      = NULL;
                        task->wait_list = NULL;
                    }
                }
            }
        }
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
 * 退出中断, 但不要进行任务调度
 */
void interrupt_exit_no_sched(void)
{
    uint32_t reg;

    reg = interrupt_disable();

    interrupt_nest--;                                                   /*  中断嵌套层次减一            */

    interrupt_resume(reg);
}

/*
 * 判断虚拟地址空间是否可用
 */
static int vspace_usable(uint32_t base, uint32_t size)
{
    uint32_t end = base + size;
    uint32_t high, low;
    int      i;

    extern space_t sys_resv_space[];
    extern space_t bsp_resv_space[];

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
 * 创建空闲进程
 */
static void idle_create(void)
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
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->cpu_rate     = 0;
    task->frame_nr     = 0;
    task->stack_base   = 0;
    task->stack_size   = 0;
    task->stack_rate   = 0;
    task->thread       = NULL;
    task->arg          = NULL;
    task->dabt_cnt     = 0;
    _REENT_INIT_PTR(&task->reent);
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));

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
 * 创建进程
 */
int32_t process_create(const char *name, uint8_t *code, uint32_t size, uint32_t priority)
{
    int32_t  pid;
    task_t  *task;
    uint32_t reg;

    if (code == NULL || size == 0) {
        return -1;
    }

    if (priority < 5) {                                                 /*  优先级最小为 5              */
        priority = 5;
    }

    reg = interrupt_disable();

    /*
     * 分配进程控制块
     */
    for (pid = 1, task = tasks + 1; pid < PROCESS_NR; pid++, task++) {  /*  遍历所有的进程控制块        */
        if (task->state == TASK_UNALLOCATE) {                           /*  如果进程控制块无效          */
                                                                        /*  如果进程的虚拟地址空间可用  */
            if (vspace_usable(pid * PROCESS_SPACE_SIZE, PROCESS_SPACE_SIZE)) {
                break;
            }
        }
    }

    if (pid == PROCESS_NR) {                                            /*  没有空闲的进程控制块        */
        interrupt_resume(reg);
        return -1;
    }

    /*
     * 初始化进程控制块
     */
    task->pid          = pid;
    task->tid          = pid;
    task->state        = TASK_SUSPEND;                                  /*  等拷贝完代码后再就绪        */
    task->counter      = priority;
    task->timer        = 0;
    task->priority     = priority;
    task->type         = TASK_TYPE_PROCESS;                             /*  任务类型为进程              */
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->cpu_rate     = 0;
    task->frame_nr     = 0;
    task->stack_base   = 0;
    task->stack_size   = 0;
    task->stack_rate   = 0;
    task->thread       = NULL;
    task->arg          = NULL;
    task->dabt_cnt     = 0;
    _REENT_INIT_PTR(&task->reent);
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));

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
        strlcpy(task->name, name, sizeof(task->name));
    } else {
        strcpy(task->name, "???");
    }

    if (vfs_task_init(pid)) {                                           /*  初始化进程的文件信息        */
        task->state = TASK_UNALLOCATE;
        interrupt_resume(reg);
        return -1;
    }

    if (vmm_process_init(task, size)) {                                 /*  初始化进程的虚拟内存信息    */
        vfs_task_cleanup(pid);
        task->state = TASK_UNALLOCATE;
        interrupt_resume(reg);
        return -1;
    }

    interrupt_resume(reg);

    /*
     * 拷贝代码到进程的虚拟地址空间
     */
    memcpy((char *)(pid * PROCESS_SPACE_SIZE), code, size);

    reg = interrupt_disable();

    task->state = TASK_RUNNING;                                         /*  进程进入就绪态              */

    interrupt_resume(reg);

    return pid;
}

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * 内核线程外壳
 */
static void kthread_shell(task_t *task)
{
    vfs_task_init(task->tid);                                           /*  初始化任务的文件信息        */

    open("/dev/ttyS0", O_RDONLY, 0666);                                 /*  打开三个标准文件            */
    stdin = fdopen(STDIN_FILENO, "r");

    open("/dev/ttyS0", O_WRONLY, 0666);
    stdout = fdopen(STDOUT_FILENO, "w");

    open("/dev/ttyS0", O_WRONLY, 0666);
    stderr = fdopen(STDERR_FILENO, "w");

    task->thread(task->arg);                                            /*  进入真正的内核线程函数      */

    _exit(0);                                                           /*  退出内核线程                */
}

/*
 * 创建内核线程
 */
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stack_size, uint32_t priority)
{
    int32_t  tid;
    task_t  *task;
    uint32_t reg;

    if (func == NULL) {
        return -1;
    }

    if (priority < 5) {                                                 /*  优先级最小为 5              */
        priority = 5;
    }

    if (stack_size < 1 * KB) {                                          /*  栈空间最小为 1 KB           */
        stack_size = 1 * KB;
    }
    stack_size = MEM_ALIGN_SIZE(stack_size);                            /*  对齐栈空间大小              */

    reg = interrupt_disable();
                                                                        /*  遍历所有的内核线程控制块    */
    for (tid = PROCESS_NR, task = tasks + PROCESS_NR; tid < TASK_NR; tid++, task++) {
        if (task->state == TASK_UNALLOCATE) {                           /*  如果内核线程控制块无效      */
            break;
        }
    }

    if (tid == TASK_NR) {                                               /*  没有空闲的内核线程控制块    */
        interrupt_resume(reg);
        return -1;
    }

    task->stack_base = (uint32_t)kmalloc(stack_size);                   /*  分配栈空间                  */
    if (task->stack_base == 0) {
        interrupt_resume(reg);
        return -1;
    }

    memset((char *)task->stack_base, THREAD_STACK_MAGIC0, stack_size);  /*  初始化栈空间                */

    task->pid          = current->pid;                                  /*  进程 ID 为当前任务的进程 ID */
    task->tid          = tid;                                           /*  任务 ID                     */
    task->state        = TASK_RUNNING;
    task->counter      = priority;
    task->timer        = 0;
    task->priority     = priority;
    task->type         = TASK_TYPE_THREAD;                              /*  任务类型为内核线程          */
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->cpu_rate     = 0;
    task->frame_nr     = 0;
    task->stack_size   = stack_size;
    task->stack_rate   = 0;
    task->thread       = func;
    task->arg          = arg;
    task->dabt_cnt     = 0;
    _REENT_INIT_PTR(&task->reent);
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));

    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];      /*  svc 模式堆栈指针(满堆栈递减)*/
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;        /*  cpsr, sys 模式, 开 IRQ      */
    task->content[2]   = task->stack_base + stack_size;                 /*  sys 模式的 sp               */
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
        strlcpy(task->name, name, sizeof(task->name));
    } else {
        strcpy(task->name, "???");
    }

    interrupt_resume(reg);

    return tid;
}

/*
 * 杀死任务
 * 只能在任务出错导致进入异常处理程序或任务主动退出通过软件中断进入内核时才能调用
 */
void task_kill(int32_t tid)
{
    task_t *task;

    if (tid > 0 && tid < TASK_NR) {                                     /*  任务 ID 合法性判断          */

        task = &tasks[tid];                                             /*  获得任务控制块              */

        vfs_task_cleanup(tid);                                          /*  清理任务的文件信息          */

        if (task->type == TASK_TYPE_PROCESS) {                          /*  如果任务是进程              */

            printk("kill process %s pid=%d!\r\n", task->name, task->pid);

            vmm_process_cleanup(task);                                  /*  清理进程的虚拟内存信息      */

        } else {                                                        /*  如果任务是内核线程          */
            printk("kill kthread %s tid=%d!\r\n", task->name, tid);

            kfree((void *)task->stack_base);                            /*  释放内核线程的栈空间        */
        }

        task->state = TASK_UNALLOCATE;                                  /*  释放任务的任务控制块        */

        if (tid == current->tid) {                                      /*  如果杀死的是当前运行的任务  */
            schedule();                                                 /*  任务调度                    */
        }
    }
}

/*
 * 获得 TICK
 */
uint64_t gettick(void)
{
    uint64_t ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = tick;

    interrupt_resume(reg);

    return ret;
}

/*
 * 获得任务 ID
 */
int32_t gettid(void)
{
    int32_t  ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = current->tid;

    interrupt_resume(reg);

    return ret;
}

/*
 * 判断是否在中断处理程序中
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
 * 判断是否在内核模式
 */
int in_kernel(void)
{
    int      ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = kernel_mode;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
