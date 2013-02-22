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
** File name:               crt0.S
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            进入点
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
#include "arch/arm920t/arm.h"
#include "kern/addr_config.h"
/*********************************************************************************************************
** 系统调用
*********************************************************************************************************/
#define SYSCALL_BAD         4
#define SYSCALL_NR          100
/*********************************************************************************************************
** 外部函数与变量
*********************************************************************************************************/
.extern cpu_reset_init
.extern irq_c_handler
.extern fiq_c_handler
.extern dabt_c_handler
.extern undf_c_handler
.extern pabt_c_handler
.extern sys_do_table

                            .text
                            .code 32
                            .align 2
/*********************************************************************************************************
** Function name:           _start
** Descriptions:            异常向量跳转表
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
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
** Function name:           reset_handler
** Descriptions:            复位处理程序
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
reset_handler:
                                            /*  进入 SVC 模式, 关中断                                   */
    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)

    MOV     R0, #0                          /*  写 PID = 0                                              */
    MCR     p15, 0, R0, c13, c0, 0
    NOP
    NOP
    NOP

    LDR     SP, =(IRQ_STACK_P_BASE)         /*  设置 SVC 模式的 SP                                      */

    BL      cpu_reset_init                  /*  初始化 CPU                                              */

    MOV     R0, #0                          /*  对 BSS 段进行清零                                       */
    LDR     R1, =__bss_start
    LDR     R2, =__bss_end

bss_loop:
    CMP     R1, R2
    STRLO   R0, [R1], #4
    BLO     bss_loop

    LDR     PC, =main                       /*  进入 main 函数                                          */
/*********************************************************************************************************
** Function name:           swi_handler
** Descriptions:            SWI 软件中断处理程序
** input parameters:        R0                  系统调用参数指针
                            R1                  系统调用号
** output parameters:       NONE
** Returned value:          系统调用返回值
*********************************************************************************************************/
                            .global swi_handler
swi_handler:
    STMFD   SP!, {R2-R12, LR}               /*  保存寄存器和返回地址到任务的 SVC 堆栈                   */

    STMFD   SP, {SP, LR}^                   /*  保存 SYS 或 USR 模式的 SP 和 LR                         */
    NOP
    SUB     SP, SP, #8

    CMP     R1, #SYSCALL_NR                 /*  系统调用号使用 R1                                       */

bad_syscall:
    MOVGE   R0, R1
    MOVGE   R1, #SYSCALL_BAD                /*  大于或等于 SYSCALL_NR 时置为 SYSCALL_BAD                */

    LDR     R2, =sys_do_table               /*  系统调用处理跳转表                                      */

    LDR     R3, [R2, R1, lsl #2]            /*  得到系统调用处理函数地址                                */

    CMP     R3, #0
    BEQ     bad_syscall                     /*  为 NULL 时跳到 bad_syscall                              */

    LDR     LR, =sys_do_return              /*  系统调用处理返回地址                                    */

    MRS     R2, SPSR
    BIC     R2, R2, #(ARM_MODE_MASK)
    ORR     R2, R2, #(ARM_SVC_MODE)
    MSR     CPSR_c, R2

    MOV     PC, R3

sys_do_return:                              /*  一定会执行                                              */
    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)

    LDMFD   SP!, {R2-R3}

    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SYS_MODE)

    MOV     SP, R2                          /*  恢复 SYS 或 USR 模式的 SP                               */

    MOV     LR, R3                          /*  恢复 SYS 或 USR 模式的 LR                               */

    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)

    LDMFD   SP!, {R2-R12, PC}^              /*  系统调用处理返回, ^ 表示复制 SPSR 到 CPSR               */
/*********************************************************************************************************
** Function name:           irq_handler
** Descriptions:            IRQ 中断处理程序
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
irq_handler:
    SUB     LR, LR, #4                      /*  调整 LR, 被 IRQ 中断的地址                              */

    LDR     SP, =(IRQ_STACK_V_BASE)         /*  设置 IRQ 模式的 SP                                      */

    STMFD   SP!, {R0-R12, LR}               /*  保存寄存器到 IRQ 堆栈                                   */

    LDR     LR, =irq_c_handler_return       /*  返回地址                                                */

    LDR     PC, =irq_c_handler              /*  进入 IRQ c 处理程序                                     */

irq_c_handler_return:                       /*  不一定会执行                                            */
    LDMFD   SP!, {R0-R12, PC}^              /*  IRQ 中断处理返回, ^ 表示复制 SPSR 到 CPSR               */
/*********************************************************************************************************
** Function name:           fiq_handler
** Descriptions:            FIQ 中断处理程序
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
fiq_handler:
    SUB     LR, LR, #4                      /*  调整 LR, 被 FIQ 中断的地址                              */

    LDR     SP, =(IRQ_STACK_V_BASE)         /*  设置 FIQ 模式的 SP                                      */

    STMFD   SP!, {R0-R12, LR}               /*  保存寄存器到 FIQ 堆栈                                   */

    LDR     LR, =fiq_c_handler_return       /*  返回地址                                                */

    LDR     PC, =fiq_c_handler              /*  进入 FIQ c 处理程序                                     */

fiq_c_handler_return:                       /*  不一定会执行                                            */
    LDMFD   SP!, {R0-R12, PC}^              /*  FIQ 中断处理返回, ^ 表示复制 SPSR 到 CPSR               */
/*********************************************************************************************************
** Function name:           dabt_handler
** Descriptions:            数据访问中止异常处理程序
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
dabt_handler:
    SUB     LR, LR, #8                      /*  调整 LR, 数据访问中止异常的地址                         */

    LDR     SP, =(IRQ_STACK_V_BASE)         /*  设置 ABT 模式的 SP                                      */

    STMFD   SP!, {R0-R12, LR}               /*  保存寄存器和返回地址到 ABT 堆栈                         */

    STMFD   SP, {SP, LR}^                   /*  保存 SYS 或 USR 模式的 SP 和 LR                         */
    NOP
    SUB     SP, SP, #8

    MRS     R0, SPSR                        /*  保存 SYS 或 USR 模式的 CPSR                             */
    STMFD   SP!, {R0}

    MOV     R0, SP

    LDR     LR, =dabt_c_handler_return      /*  返回地址                                                */

    LDR     PC, =dabt_c_handler             /*  进入 DABT c 处理程序                                    */

dabt_c_handler_return:
    LDMFD   SP!, {R0-R2}
    LDMFD   SP!, {R0-R12, PC}^              /*  DABT 异常处理返回, ^ 表示复制 SPSR 到 CPSR              */
/*********************************************************************************************************
** Function name:           pabt_handler
** Descriptions:            预取指中止异常处理程序
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
pabt_handler:
    SUB     LR, LR, #4                      /*  调整 LR, 预取指中止异常的地址                           */

    LDR     SP, =(IRQ_STACK_V_BASE)         /*  设置 ABT 模式的 SP                                      */

    STMFD   SP!, {R0-R12, LR}               /*  保存寄存器和返回地址到 ABT 堆栈                         */

    STMFD   SP, {SP, LR}^                   /*  保存 SYS 或 USR 模式的 SP 和 LR                         */
    NOP
    SUB     SP, SP, #8

    MRS     R0, SPSR                        /*  保存 SYS 或 USR 模式的 CPSR                             */
    STMFD   SP!, {R0}

    MOV     R0, SP

    LDR     LR, =pabt_c_handler_return      /*  返回地址                                                */

    LDR     PC, =pabt_c_handler             /*  进入 PABT c 处理程序                                    */

pabt_c_handler_return:
    LDMFD   SP!, {R0-R2}
    LDMFD   SP!, {R0-R12, PC}^              /*  PABT 异常处理返回, ^ 表示复制 SPSR 到 CPSR              */
/*********************************************************************************************************
** Function name:           undef_handler
** Descriptions:            未定义指令异常处理程序
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
undef_handler:
    LDR     SP, =(IRQ_STACK_V_BASE)         /*  设置 UNDEF 模式的 SP                                    */

    STMFD   SP!, {R0-R12, LR}               /*  保存寄存器和返回地址到 UNDEF 堆栈                       */

    STMFD   SP, {SP, LR}^                   /*  保存 SYS 或 USR 模式的 SP 和 LR                         */
    NOP
    SUB     SP, SP, #8

    MRS     R0, SPSR                        /*  保存 SYS 或 USR 模式的 CPSR                             */
    STMFD   SP!, {R0}

    MOV     R0, SP

    LDR     LR, =undf_c_handler_return      /*  返回地址                                                */

    LDR     PC, =undf_c_handler             /*  进入 UNDEF c 处理程序                                   */

undf_c_handler_return:
    LDMFD   SP!, {R0-R2}
    LDMFD   SP!, {R0-R12, PC}^              /*  UNDEF 异常处理返回, ^ 表示复制 SPSR 到 CPSR             */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
