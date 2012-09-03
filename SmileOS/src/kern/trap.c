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
** File name:               trap.c
** Last modified Date:      2012-2-25
** Last Version:            1.0.0
** Descriptions:            异常处理程序
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-25
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-25
** Version:                 1.1.0
** Descriptions:            处理更多的异常, 异常改为杀死当前任务而非终止内核
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-8-29
** Version:                 1.3.0
** Descriptions:            增加注释
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/mmu.h"
#include "kern/vmm.h"
#include "kern/arm.h"

extern void get_sys_lr(uint32_t *LR);
/*********************************************************************************************************
** Function name:           fiq_c_handler
** Descriptions:            FIQ 快速中断处理程序
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void fiq_c_handler(void)
{
    /*
     * FIQ 快速中断并不是由当前任务引起的异常, 所以只能重启 SmileOS 了:-(
     */
    kcomplain("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    kcomplain("reboot SmileOS...\n");

    extern void _start(void);
    _start();
}
/*********************************************************************************************************
** Function name:           undf_c_handler
** Descriptions:            未定义指令异常处理程序
** input parameters:        PC                  寄存器 PC
**                          CPSR                寄存器 CPSR
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void undf_c_handler(uint32_t PC, uint32_t CPSR)
{
    uint32_t LR;

    interrupt_enter();                                                  /*  进入中断                    */

    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    printk("PC   = 0x%x\n", PC);
    printk("CPSR = 0x%x\n", CPSR);
    get_sys_lr(&LR);
    printk("LR = 0x%x\n", LR);
    if ((CPSR & ARM_MODE_MASK) == ARM_SVC_MODE && current->pid == 0) {  /*  如果是在 SVC 模式挂掉       */
        extern const char *last_syscall;
        printk("last system call = %s\n", last_syscall);
    }

    task_kill(current->tid, SIGILL);                                    /*  杀死当前任务                */

    interrupt_exit();                                                   /*  退出中断                    */
}
/*********************************************************************************************************
** Function name:           pabt_c_handler
** Descriptions:            预取指中止异常处理程序
** input parameters:        PC                  寄存器 PC
**                          CPSR                寄存器 CPSR
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void pabt_c_handler(uint32_t PC, uint32_t CPSR)
{
    uint32_t LR;

    interrupt_enter();                                                  /*  进入中断                    */

    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());
    printk("PC   = 0x%x\n", PC);
    printk("CPSR = 0x%x\n", CPSR);
    get_sys_lr(&LR);
    printk("LR = 0x%x\n", LR);
    if ((CPSR & ARM_MODE_MASK) == ARM_SVC_MODE && current->pid == 0) {  /*  如果是在 SVC 模式挂掉       */
        extern const char *last_syscall;
        printk("last system call = %s\n", last_syscall);
    }

    task_kill(current->tid, SIGSEGV);                                   /*  杀死当前任务                */

    interrupt_exit();                                                   /*  退出中断                    */
}
/*********************************************************************************************************
** Function name:           dabt_oops
** Descriptions:            数据访问中止 oops
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
#define dabt_oops()                                                                     \
    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);    \
    printk("fault address = 0x%x\n", mmu_get_fault_address());                          \
    printk("fault status  = 0x%x\n", mmu_get_data_fault_status());                      \
    printk("PC   = 0x%x\n", PC);                                                        \
    printk("CPSR = 0x%x\n", CPSR);                                                      \
    get_sys_lr(&LR);                                                                    \
    printk("LR   = 0x%x\n", LR);                                                        \
    if ((CPSR & ARM_MODE_MASK) == ARM_SVC_MODE && current->pid == 0) {      /*  如果是在 SVC 模式挂掉   */\
        extern const char *last_syscall;                                                \
        printk("last system call = %s\n", last_syscall);                                \
    }                                                                                   \
    task_kill(current->tid, SIGSEGV)                                        /*  杀死当前任务            */
/*********************************************************************************************************
** Function name:           dabt_c_handler
** Descriptions:            数据访问中止异常处理程序
** input parameters:        PC                  寄存器 PC
**                          CPSR                寄存器 CPSR
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void dabt_c_handler(uint32_t PC, uint32_t CPSR)
{
    uint32_t MVA;
    uint32_t LR;

    interrupt_enter();                                                  /*  进入中断                    */

    switch (mmu_get_data_fault_status() & 0x0F) {
    case 1:     /* Alignment */
    case 3:
        dabt_oops();
        break;

    case 5:     /* Translation */
    case 7:
        if (current->pid != 0) {                                        /*  如果是在进程里发生错误      */

            MVA = mmu_get_fault_address();
            /*
             * 真实硬件时, mmu_get_fault_address 应该返回的是
             *
             * MVA = VA < 32 * MB ? VA + PROCESS_SPACE_SIZE * current->pid : VA
             *
             * 而 qemu-system-arm.exe 有 bug, 始终返回的是 VA
             *
             * 所以加上下面的修正代码
             */
            if (MVA < PROCESS_SPACE_SIZE) {
                MVA = MVA + PROCESS_SPACE_SIZE * current->pid;
            }

            if (    MVA >= PROCESS_SPACE_SIZE *  current->pid           /*  判断出错地址是否在当前进程  */
                 && MVA <  PROCESS_SPACE_SIZE * (current->pid + 1)) {   /*  的虚拟地址空间范围内        */
                if (vmm_page_map(current, MVA) == 0) {                  /*  页面映射                    */
                    current->dabt_cnt++;                                /*  数据访问中止次数加一        */
                    interrupt_exit_no_sched();                          /*  退出中断, 但不要调度        */
                    return;
                }
            }
        } else {
            int32_t pid;

            /*
             * 如果是在内核里发生错误
             *
             * 注意在系统调用处理中, 必须将指针型参数经 va_to_mva 转换方可传入内核接口
             */

            MVA = mmu_get_fault_address();                              /*  必须就是 MVA                */

            pid = MVA / PROCESS_SPACE_SIZE;
            if (pid != 0 && pid < PROCESS_NR) {
                task_t *task = &tasks[pid];
                if (task->state != TASK_UNALLOCATE) {
                    if (vmm_page_map(task, MVA) == 0) {                 /*  页面映射                    */
                        task->dabt_cnt++;                               /*  数据访问中止次数加一        */
                        interrupt_exit_no_sched();                      /*  退出中断, 但不要调度        */
                        return;
                    }
                }
            }
        }
        dabt_oops();
        break;

    case 9:     /* Domain */
    case 11:
        dabt_oops();
        break;

    case 13:    /* Permission */
    case 15:
        dabt_oops();
        break;

                /*
                 * External abort on noncachable
                 * nonbufferable access or noncachable
                 * bufferable read
                 */
    case 8:
    case 10:
        dabt_oops();
        break;

    default:
        break;
    }

    interrupt_exit();                                                   /*  退出中断                    */
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
