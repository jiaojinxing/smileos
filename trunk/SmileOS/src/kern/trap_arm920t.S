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
** File name:               trap_arm920t.s
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            ARM920T 异常处理
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
** Descriptions:            处理更多的异常, 软件中断加入内核模式的设置
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-6-12
** Version:                 1.1.0
** Descriptions:            去除软件中断中内核模式的设置
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-8-29
** Version:                 1.2.0
** Descriptions:            增加注释
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/arm.h"

#define SYS_CALL_NR         100             /*  系统调用数                                              */
/*********************************************************************************************************
** 异常向量跳转表
*********************************************************************************************************/
.text
.code 32
.align 2

.global _start
_start:
    LDR     PC, _reset                      /*  复位                                                    */
    LDR     PC, _undf                       /*  未定义指令异常                                          */
    LDR     PC, _swi                        /*  软件中断                                                */
    LDR     PC, _pabt                       /*  预取指中止异常                                          */
    LDR     PC, _dabt                       /*  数据访问中断异常                                        */
    NOP                                     /*  保留                                                    */
    LDR     PC, _irq                        /*  IRQ 中断                                                */
    LDR     PC, _fiq                        /*  FIQ 中断                                                */

_reset:     .word reset_handler
_undf:      .word undef_handler
_swi:       .word swi_handler
_pabt:      .word pabt_handler
_dabt:      .word dabt_handler
_irq:       .word irq_handler
_fiq:       .word fiq_handler
/*********************************************************************************************************
** 复位处理程序
*********************************************************************************************************/
.extern cpu_init
.extern bsp_init

reset_handler:
                                            /*  进入 SVC 模式, 关中断                                   */
    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)

    MOV     R0, #0                          /*  写 PID = 0                                              */
    MCR     p15, 0, R0, c13, c0, 0
    NOP
    NOP
    NOP

    LDR     SP, =(PROCESS0_STACK_BASE)      /*  设置 SVC 模式的 SP                                      */

    BL      cpu_init                        /*  初始化 CPU                                              */

    MOV     R0, #0                          /*  对 BSS 段进行清零                                       */
    LDR     R1, =__bss_start
    LDR     R2, =__bss_end

bss_loop:
    CMP     R1, R2
    STRLO   R0, [R1], #4
    BLO     bss_loop

    BL      bsp_init                        /*  初始化 BSP                                              */

    LDR     PC, =main                       /*  进入 main 函数                                          */
/*********************************************************************************************************
** SWI 软件中断处理程序
*********************************************************************************************************/
.extern sys_do_table
swi_handler:
    STMFD   SP!, {R4-R12, LR}               /*  保存寄存器和返回地址到任务的 SVC 堆栈                   */

    CMP     R7, #SYS_CALL_NR                /*  根据 EABI, 系统调用号使用 R7                            */

    LDRLS   R4, =sys_do_table               /*  系统调用处理跳转表                                      */

    LDRLS   LR, =sys_do_return              /*  系统调用处理返回地址                                    */

    LDRLS   PC, [R4, R7, lsl #2]            /*  跳转到相应系统调用处理                                  */

    MOV     R0, #-1                         /*  出错, 返回 -1, R0 用于返回值                            */

sys_do_return:                              /*  一定会执行                                              */
    LDMFD   SP!, {R4-R12, PC}^              /*  系统调用处理返回, ^ 表示复制 SPSR 到 CPSR               */
/*********************************************************************************************************
** 获得 SYS 模式的 LR
*********************************************************************************************************/
    .global get_sys_lr
get_sys_lr:
    STMIA   R0!, {LR}^                      /*  获得 SYS 模式的 LR                                      */
    NOP
    BX      LR
/*********************************************************************************************************
** IRQ 中断处理程序
*********************************************************************************************************/
.extern irq_c_handler
irq_handler:
    SUB     LR, LR, #4                      /*  调整 LR, 被 IRQ 中断的地址                              */

    LDR     SP, =(IRQ_STACK_P_BASE)         /*  设置 IRQ 模式的 SP                                      */

    STMFD   SP!, {R0-R12, LR}               /*  保存寄存器到 IRQ 堆栈                                   */

    LDR     LR, =irq_c_handler_return       /*  返回地址                                                */

    LDR     PC, =irq_c_handler              /*  进入 IRQ c 处理程序                                     */

irq_c_handler_return:                       /*  不一定会执行                                            */
    LDMFD   SP!, {R0-R12, PC}^              /*  IRQ 中断处理返回, ^ 表示复制 SPSR 到 CPSR               */
/*********************************************************************************************************
** FIQ 中断处理程序
*********************************************************************************************************/
.extern fiq_c_handler
fiq_handler:
    SUB     LR, LR, #4                      /*  调整 LR, 被 FIQ 中断的地址                              */

    LDR     SP, =(IRQ_STACK_P_BASE)         /*  设置 FIQ 模式的 SP                                      */

    STMFD   SP!, {R0-R12, LR}               /*  保存寄存器到 FIQ 堆栈                                   */

    LDR     LR, =fiq_c_handler_return       /*  返回地址                                                */

    LDR     PC, =fiq_c_handler              /*  进入 FIQ c 处理程序                                     */

fiq_c_handler_return:                       /*  不一定会执行                                            */
    LDMFD   SP!, {R0-R12, PC}^              /*  FIQ 中断处理返回, ^ 表示复制 SPSR 到 CPSR               */
/*********************************************************************************************************
** 数据访问中止异常处理程序
*********************************************************************************************************/
.extern dabt_c_handler
dabt_handler:
    SUB     LR, LR, #8                      /*  调整 LR, 数据访问中止异常的地址                         */

    LDR     SP, =(IRQ_STACK_P_BASE)         /*  设置 DABT 模式的 SP                                     */

    STMFD   SP!, {R0-R12, LR}               /*  保存寄存器和返回地址到 DABT 堆栈                        */

    MOV     R0, LR

    MRS     R1, SPSR

    LDR     LR, =dabt_c_handler_return      /*  返回地址                                                */

    LDR     PC, =dabt_c_handler             /*  进入 DABT c 处理程序                                    */

dabt_c_handler_return:
    LDMFD   SP!, {R0-R12, PC}^              /*  DABT 异常处理返回, ^ 表示复制 SPSR 到 CPSR              */
/*********************************************************************************************************
** 预取指中止异常处理程序
*********************************************************************************************************/
.extern pabt_c_handler
pabt_handler:
    SUB     LR, LR, #4                      /*  调整 LR, 预取指中止异常的地址                           */

    LDR     SP, =(IRQ_STACK_P_BASE)         /*  设置 PABT 模式的 SP                                     */

    STMFD   SP!, {R0-R12, LR}               /*  保存寄存器和返回地址到 PABT 堆栈                        */

    MOV     R0, LR

    MRS     R1, SPSR

    LDR     LR, =pabt_c_handler_return      /*  返回地址                                                */

    LDR     PC, =pabt_c_handler             /*  进入 PABT c 处理程序                                    */

pabt_c_handler_return:
    LDMFD   SP!, {R0-R12, PC}^              /*  PABT 异常处理返回, ^ 表示复制 SPSR 到 CPSR              */
/*********************************************************************************************************
** 未定义指令异常处理程序
*********************************************************************************************************/
.extern undf_c_handler
undef_handler:
    LDR     SP, =(IRQ_STACK_P_BASE)         /*  设置 UNDEF 模式的 SP                                    */

    STMFD   SP!, {R0-R12, LR}               /*  保存寄存器和返回地址到 UNDEF 堆栈                       */

    MOV     R0, LR

    MRS     R1, SPSR

    LDR     LR, =undf_c_handler_return      /*  返回地址                                                */

    LDR     PC, =undf_c_handler             /*  进入 UNDEF c 处理程序                                   */

undf_c_handler_return:
    LDMFD   SP!, {R0-R12, PC}^              /*  UNDEF 异常处理返回, ^ 表示复制 SPSR 到 CPSR             */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
