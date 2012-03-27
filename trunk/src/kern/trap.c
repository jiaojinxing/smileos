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
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/mmu.h"
#include "kern/vmm.h"

/*
 * FIQ 快速中断处理程序
 */
void fiq_c_handler(void)
{
    /*
     * FIQ 快速中断处理程序并不是由当前任务引起的异常, 所以只能重启 SmileOS 了:-)
     */
    kcomplain("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    kcomplain("reboot SmileOS...\n");

    extern void _start(void);
    _start();
}

/*
 * 未定义指令异常处理程序
 */
void undf_c_handler(uint32_t lr, uint32_t spsr)
{
    interrupt_enter();                                                  /*  进入中断                    */

    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    printk("lr   = 0x%x\n", lr);
    printk("spsr = 0x%x\n", spsr);

    task_kill(current->tid);                                            /*  杀死当前任务                */

    interrupt_exit();                                                   /*  退出中断                    */
}

/*
 * 预取指中止异常处理程序
 */
void pabt_c_handler(uint32_t lr, uint32_t spsr)
{
    interrupt_enter();                                                  /*  进入中断                    */

    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());
    printk("lr   = 0x%x\n", lr);
    printk("spsr = 0x%x\n", spsr);

    task_kill(current->tid);                                            /*  杀死当前任务                */

    interrupt_exit();                                                   /*  退出中断                    */
}

/*
 * 数据访问中止异常处理程序
 */
void dabt_c_handler(uint32_t lr, uint32_t spsr)
{
    uint32_t mva;

    interrupt_enter();                                                  /*  进入中断                    */

    switch (mmu_get_data_fault_status() & 0x0F) {
    case 1:     /* Alignment */
    case 3:
        printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        task_kill(current->tid);                                        /*  杀死当前任务                */
        break;

    case 5:     /* Translation */
    case 7:
        /*
         * qemu-system-arm.exe 的 bug..., 真实硬件使用 #if 1, qemu 使用 #if 0
         */
#if 1
        mva = mmu_get_fault_address();
#warning "you can't use qemu-system-arm.exe"
#else
        mva = mmu_get_fault_address() + PROCESS_SPACE_SIZE * current->pid;
#warning "you must use qemu-system-arm.exe"
#endif

        if (    mva >= PROCESS_SPACE_SIZE *  current->pid               /*  判断出错地址是否在当前进程  */
             && mva <  PROCESS_SPACE_SIZE * (current->pid + 1)) {       /*  的虚拟地址空间范围内        */
            if (vmm_map_process_page(current, mva) == 0) {              /*  页面映射                    */
                current->dabt_cnt++;                                    /*  数据终止次数加一            */
                interrupt_exit_no_schedule();                           /*  退出中断, 但不要调度        */
                return;
            }
        }
        printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        task_kill(current->tid);                                        /*  杀死当前任务                */
        break;

    case 9:     /* Domain */
    case 11:
        printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        task_kill(current->tid);                                        /*  杀死当前任务                */
        break;

    case 13:    /* Permission */
    case 15:
        printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        task_kill(current->tid);                                        /*  杀死当前任务                */
        break;

                /*
                 * External abort on noncachable
                 * nonbufferable access or noncachable
                 * bufferable read
                 */
    case 8:
    case 10:
        printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        task_kill(current->tid);                                        /*  杀死当前任务                */
        break;

    default:
        break;
    }

    interrupt_exit();                                                   /*  退出中断                    */
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
