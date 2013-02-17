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
#include "kern/func_config.h"
#include <string.h>
#include <stdarg.h>
/*********************************************************************************************************
** 内部变量
*********************************************************************************************************/
static tick_t               os_ticks;                                   /*  TICK                        */
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

    os_started     = FALSE;                                             /*  内核还没启动                */
    os_ticks       = 0;                                                 /*  TICK 为 0                   */
    interrupt_nest = 0;                                                 /*  中断嵌套层次为 0            */
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
    arch_mmu_init();                                                    /*  初始化 MMU                  */

    kvars_init();                                                       /*  初始化内核变量              */

    extern void interrupt_init(void);
    interrupt_init();                                                   /*  初始化中断                  */

    extern void kheap_create(void);
    kheap_create();                                                     /*  创建内核内存堆              */

    extern void vmm_init(void);
    vmm_init();                                                         /*  初始化虚拟内存管理          */

#if CONFIG_VFS_EN > 0
    extern int vfs_init(void);
    vfs_init();                                                         /*  初始化虚拟文件系统          */
#endif

    extern void kidle_create(void);
    kidle_create();                                                     /*  创建空闲进程                */

    extern void klogd_create(void);
    klogd_create();                                                     /*  创建内核日志线程            */

    extern void default_workqueue_create(void);
    default_workqueue_create();                                         /*  创建缺省工作队列            */
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
    if (os_started) {                                                   /*  内核已经启动                */
        printk(KERN_ERR"%s: kernel has started\n", __func__);
    } else {
        os_started = TRUE;

        arch_switch_context_to(NULL, current);
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
    reg_t   reg;
    int     i;
    task_t *task;

    reg = interrupt_disable();

    os_ticks++;                                                         /*  内核 TICK 加一              */

    if (current->type == TASK_TYPE_PROCESS) {                           /*  如果当前任务是进程类型      */
        if (current->timeslice > 0) {                                   /*  如果当前进程还有剩余时间片  */
            current->timeslice--;                                       /*  当前进程的剩余时间片减一    */
        }
    }

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {               /*  遍历所有任务                */
        if (task->status == TASK_SLEEPING) {                            /*  如果任务正在休睡            */
            if (task->delay != 0) {
                task->delay--;                                          /*  任务延时减一                */
            }
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
tick_t getticks(void)
{
    tick_t ret;
    reg_t  reg;

    reg = interrupt_disable();

    ret = os_ticks;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           bkdr_hash
** Descriptions:            BKDR Hash Function
**                          各种字符串 Hash 函数比较
**                          http://www.byvoid.com/blog/string-hash-compare/
** input parameters:        str                 字符串
** output parameters:       NONE
** Returned value:          BKDR Hash
*********************************************************************************************************/
unsigned int bkdr_hash(const char *str)
{
    unsigned int seed = 131;                                            /*  31 131 1313 13131 131313 etc*/
    unsigned int hash = 0;
    char ch;

    while ((ch = *str++) != 0) {
        hash = hash * seed + ch;
    }

    return (hash & 0x7FFFFFFF);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
