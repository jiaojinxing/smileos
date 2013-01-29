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
#include "kern/kern.h"
#define KVARS_INC
#include "kern/kvars.h"
#include "kern/vmm.h"
#include "kern/mmu.h"
#include <string.h>
/*********************************************************************************************************
** Function name:           kvars_init
** Descriptions:            初始化内核变量
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void kvars_init(void)
{
    task_t *task;
    int     i;

    running        = FALSE;                                             /*  内核还没启动                */
    interrupt_nest = 0;                                                 /*  中断嵌套层次为 0            */
    ticks          = 0;                                                 /*  TICK 为 0                   */
    current        = &tasks[0];                                         /*  当前任务为进程 0            */
    _impure_ptr    = &reents[0];                                        /*  初始化 _impure_ptr 指针     */

    /*
     * 初始化所有的任务控制块
     */
    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
        memset(task, 0, sizeof(task_t));
        task->status = TASK_UNALLOCATE;
    }
}
/*********************************************************************************************************
** Function name:           kernel_init
** Descriptions:            初始化内核
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_init(void)
{
    extern void bsp_init(void);
    bsp_init();                                                         /*  初始化 BSP                  */

    mmu_init();                                                         /*  初始化 MMU                  */

    kvars_init();                                                       /*  初始化内核变量              */

    extern void kheap_create(void);
    kheap_create();                                                     /*  创建内核内存堆              */

    extern void kidle_create(void);
    kidle_create();                                                     /*  创建空闲进程                */

    extern void klogd_create(void);
    klogd_create();                                                     /*  创建内核日志线程            */

    extern void alarmd_create(void);
    alarmd_create();                                                    /*  创建 alarm 线程             */

    extern void netjob_create(void);
    netjob_create();                                                    /*  创建网络工作线程            */

    vmm_init();                                                         /*  初始化虚拟内存管理          */
}
/*********************************************************************************************************
** Function name:           kernel_start
** Descriptions:            启动内核
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_start(void)
{
    if (!running) {
        running = TRUE;                                                 /*  内核已经启动                */

        extern void process0_enter(register uint32_t sp_svc);
        process0_enter(current->regs[0]);                               /*  进入进程 0 (空闲进程)       */
    }
}
/*********************************************************************************************************
** Function name:           kernel_timer
** Descriptions:            内核定时器处理函数
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_timer(void)
{
    uint32_t reg;
    int      i;
    task_t  *task;

    reg = interrupt_disable();

    ticks++;                                                            /*  内核 TICK 加一              */

    current->ticks++;                                                   /*  当前任务被中断的次数加一    */

    if (current->type == TASK_TYPE_PROCESS) {                           /*  如果当前任务是进程类型      */
        if (current->timeslice > 0) {                                   /*  如果当前进程还有剩余时间片  */
            current->timeslice--;                                       /*  当前进程的剩余时间片减一    */
        }
    }

    if (ticks % TICK_PER_SECOND == 0) {                                 /*  如果已经过去了一秒          */
        for (i = 0, task = tasks; i < TASK_NR; i++, task++) {           /*  遍历所有任务                */
            if (task->status != TASK_UNALLOCATE) {                      /*  如果任务有效                */
                task->cpu_usage = task->ticks;                          /*  统计任务的 CPU 占用率       */
                task->ticks     = 0;                                    /*  重置该任务被定时器中断的次数*/
            }
        }
    }

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {               /*  遍历所有任务                */
        if (task->status == TASK_SLEEPING) {                            /*  如果任务正在休睡            */
            task->delay--;                                              /*  任务延时减一                */
            if (task->delay == 0) {                                     /*  如果任务延时到期            */
                task->status      = TASK_RUNNING;                       /*  任务进入就绪态              */
                task->resume_type = TASK_RESUME_TIMEOUT;                /*  设置任务的恢复类型为超时    */
                extern int ipc_task_cleanup(task_t *task);
                ipc_task_cleanup(task);                                 /*  清理任务 IPC                */
            }
        }
    }

    interrupt_resume(reg);
}
/*********************************************************************************************************
** Function name:           getticks
** Descriptions:            获得 TICK
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TICK
*********************************************************************************************************/
uint64_t getticks(void)
{
    uint64_t ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = ticks;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           gettid
** Descriptions:            获得当前任务的 TID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          当前任务的 TID
*********************************************************************************************************/
int32_t gettid(void)
{
    int32_t  ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = current->tid;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           va_to_mva
** Descriptions:            将进程空间的虚拟地址转换为修改后的虚拟地址
** input parameters:        VA                  进程空间的虚拟地址
** output parameters:       NONE
** Returned value:          修改后的虚拟地址
*********************************************************************************************************/
void *va_to_mva(const void *VA)
{
    uint32_t reg;
    void *ret;

    reg = interrupt_disable();

    if ((current->pid != 0) && (uint32_t)VA < PROCESS_SPACE_SIZE && VA != NULL) {
        ret = (uint8_t *)VA + current->pid * PROCESS_SPACE_SIZE;
    } else {
        ret = (void *)VA;
    }

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           mva_to_va
** Descriptions:            将修改后的虚拟地址转换为进程空间的虚拟地址
** input parameters:        MVA                 修改后的虚拟地址
** output parameters:       NONE
** Returned value:          进程空间的虚拟地址
*********************************************************************************************************/
void *mva_to_va(const void *MVA)
{
    return (void *)((uint32_t)MVA % PROCESS_SPACE_SIZE);
}
/*********************************************************************************************************
** Function name:           vspace_usable
** Descriptions:            判断虚拟地址空间是否可用
** input parameters:        base                虚拟地址空间的基址
**                          size                虚拟地址空间的大小
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int vspace_usable(uint32_t base, uint32_t size)
{
    uint32_t end = base + size;
    uint32_t high, low;
    int      i;

    /*
     * 虚拟地址空间不能和系统保留的虚拟地址空间重叠
     */
    extern const space_t sys_resv_space[];
    for (i = 0; sys_resv_space[i].size != 0; i++) {
        high = max(sys_resv_space[i].base, base);
        low  = min(sys_resv_space[i].base + sys_resv_space[i].size, end);
        if (high < low) {
            return FALSE;
        }
    }

    /*
     * 虚拟地址空间不能和 BSP 保留的虚拟地址空间重叠
     */
    extern const space_t bsp_resv_space[];
    for (i = 0; bsp_resv_space[i].size != 0; i++) {
        high = max(bsp_resv_space[i].base, base);
        low  = min(bsp_resv_space[i].base + bsp_resv_space[i].size, end);
        if (high < low) {
            return FALSE;
        }
    }

    return TRUE;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
