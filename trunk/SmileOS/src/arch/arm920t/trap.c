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
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/kvars.h"
#include "module/module.h"
#include "arch/arm920t/mmu.h"
#include "arch/arm920t/arm.h"
#include <signal.h>
#include <string.h>
/*********************************************************************************************************
** Function name:           print_regs
** Descriptions:            打印寄存器组
** input parameters:        regs                寄存器组
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void print_regs(reg_t *regs)
{
    int         i;
    char        cpsr_str[32] = "\0";
    reg_t       cpsr;
    reg_t       lr;
    reg_t       fp;
    const char *func_name;
    mem_ptr_t   diff;

    cpsr = regs[0];

    if (cpsr & 0x80000000) {
        cpsr_str[0] = 'N';
    } else {
        cpsr_str[0] = 'n';
    }

    if (cpsr & 0x40000000) {
        cpsr_str[1] = 'Z';
    } else {
        cpsr_str[1] = 'z';
    }

    if (cpsr & 0x20000000) {
        cpsr_str[2] = 'C';
    } else {
        cpsr_str[2] = 'c';
    }

    if (cpsr & 0x10000000) {
        cpsr_str[3] = 'V';
    } else {
        cpsr_str[3] = 'v';
    }

    if (cpsr & 0x08000000) {
        cpsr_str[4] = 'Q';
    } else {
        cpsr_str[4] = 'q';
    }

    if (cpsr & 0x80) {
        cpsr_str[5] = 'I';
    } else {
        cpsr_str[5] = 'i';
    }

    if (cpsr & 0x40) {
        cpsr_str[6] = 'F';
    } else {
        cpsr_str[6] = 'f';
    }

    if (cpsr & 0x20) {
        cpsr_str[7] = 'T';
    } else {
        cpsr_str[7] = 't';
    }
    cpsr_str[8] = 0;

    cpsr &= 0x1F;

    switch (cpsr) {
    case ARM_USR_MODE:
        strcpy(&cpsr_str[8], "_USER");
        break;

    case ARM_FIQ_MODE:
        strcpy(&cpsr_str[8], "_FIQ");
        break;

    case ARM_IRQ_MODE:
        strcpy(&cpsr_str[8], "_IRQ");
        break;

    case ARM_SVC_MODE:
        strcpy(&cpsr_str[8], "_SVC");
        break;

    case ARM_ABT_MODE:
        strcpy(&cpsr_str[8], "_ABT");
        break;

    case ARM_UDF_MODE:
        strcpy(&cpsr_str[8], "_UND");
        break;

    case ARM_SYS_MODE:
        strcpy(&cpsr_str[8], "_SYS");
        break;

    default:
        strcpy(&cpsr_str[8], "_!!!");
        break;
    }

    printk("\n---------------- CPU REGISERS ----------------\n");

    printk("CPSR\t= %s\n", cpsr_str);
    printk("SP\t= 0x%x\n", regs[1]);
    printk("LR\t= 0x%x\n", regs[2]);
    printk("PC\t= 0x%x\n", regs[16]);

    for (i = 0; i < 13; i++) {
        printk("R%d\t= 0x%x\n", i, regs[3 + i]);
    }

    printk("\n---------------- CALL STACK ----------------\n");

    func_name = symbol_name(regs[16], &diff);

    printk("error function: %s, diff = 0x%x\n", func_name, diff);

    fp = regs[3 + 11];

    for (i = 0; i < 10; i++) {
        if (fp == (reg_t) -1) {
            break;
        }
        lr = *((reg_t *)(fp));
        fp = *((reg_t *)(fp - 4));

        func_name = symbol_name(lr, &diff);

        printk(" ^ function: %s, diff = 0x%x\n", func_name, diff);
    }

    printk("\n----------------  END  ----------------\n");
}
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
     * FIQ 快速中断并不是由当前任务引起的异常, 一旦发生, 说明出错了, 所以只能重启 SmileOS 了:-(
     */
    extern void _start(void);
    _start();
}
/*********************************************************************************************************
** Function name:           undf_c_handler
** Descriptions:            未定义指令异常处理程序
** input parameters:        regs                寄存器组
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void undf_c_handler(reg_t *regs)
{
    interrupt_enter();                                                  /*  进入中断                    */

    printk("%s, current TID = %d name=%s\n", __func__, current->tid, current->name);

    print_regs(regs);

    kill(current->tid, SIGILL);                                         /*  杀死当前任务                */

    interrupt_exit();                                                   /*  退出中断                    */
}
/*********************************************************************************************************
** Function name:           pabt_c_handler
** Descriptions:            预取指中止异常处理程序
** input parameters:        regs                寄存器组
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void pabt_c_handler(reg_t *regs)
{
    interrupt_enter();                                                  /*  进入中断                    */

    printk("%s, current TID = %d name=%s\n", __func__, current->tid, current->name);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());

    print_regs(regs);

    kill(current->tid, SIGSEGV);                                        /*  杀死当前任务                */

    interrupt_exit();                                                   /*  退出中断                    */
}
/*********************************************************************************************************
** Function name:           dabt_oops
** Descriptions:            数据访问中止 oops
** input parameters:        regs                寄存器组
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void dabt_oops(reg_t *regs)
{
    printk("%s, current TID = %d name=%s\n", __func__, current->tid, current->name);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_data_fault_status());

    print_regs(regs);
}
/*********************************************************************************************************
** Function name:           dabt_c_handler
** Descriptions:            数据访问中止异常处理程序
** input parameters:        regs                寄存器组
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void dabt_c_handler(reg_t *regs)
{
    interrupt_enter();                                                  /*  进入中断                    */

    dabt_oops(regs);

    kill(current->tid, SIGSEGV);                                        /*  杀死当前任务                */

    interrupt_exit();                                                   /*  退出中断                    */
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
