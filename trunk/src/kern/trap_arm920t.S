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
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/arm.h"
#include "kern/sys_call.h"
/*********************************************************************************************************
  异常向量跳转表
*********************************************************************************************************/
.text
.code 32
.align 2

.global _start
_start:
    ldr     pc, _reset                                                  // reset - _reset
    ldr     pc, _undf                                                   // undefined - _undf
    ldr     pc, _swi                                                    // SWI - _swi
    ldr     pc, _pabt                                                   // program abort - _pabt
    ldr     pc, _dabt                                                   // data abort - _dabt
    nop                                                                 // reserved
    ldr     pc, _irq                                                    // IRQ - _irq
    ldr     pc, _fiq                                                    // FIQ - _fiq

_reset:     .word reset_handler                                         // reset
_undf:      .word undef_handler                                         // undefined
_swi:       .word swi_handler                                           // SWI
_pabt:      .word pabt_handler                                          // program abort
_dabt:      .word dabt_handler                                          // data abort
_irq:       .word irq_handler                                           // IRQ
_fiq:       .word fiq_handler                                           // FIQ
/*********************************************************************************************************
  复位处理程序
*********************************************************************************************************/
.extern cpu_init
.extern bsp_init
reset_handler:
    msr     cpsr_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)           /*  进入 svc 模式, 关中断       */

    ldr     sp, =(PROCESS0_STACK_BASE)                                  /*  设置 svc 模式的 sp          */

    bl      cpu_init                                                    /*  初始化 CPU                  */

    mov     r0, #0                                                      /*  对 bss 段进行清零           */
    ldr     r1, =__bss_start
    ldr     r2, =__bss_end

bss_loop:
    cmp     r1, r2
    strlo   r0, [r1], #4
    blo     bss_loop

    bl      bsp_init                                                    /*  初始化 BSP                  */

    ldr     pc, =main                                                   /*  进入 main 函数              */
/*********************************************************************************************************
  SWI 软件中断处理程序
*********************************************************************************************************/
.extern sys_do_table
swi_handler:
    stmfd   sp!, {r4-r12, lr}               /*  保存寄存器和被中断的地址到进程的内核栈                  */
                                            /*  根据 APCS, 前四个参数使用 r0 - r3, 后面的参数使用堆栈   */
                                            /*  因为切换了处理器模式, sp 也切换了, 所以多于 4 个参数时, */
                                            /*  会有问题                                                */
                                            /*
                                             *  扩展阅读:
                                             *  http://www.52rd.com/Blog/Detail_RD.Blog_zjhfqq_7625.html
                                             */

    cmp     r7, #SYS_CALL_NR                /*  根据 EABI, 系统调用号使用 r7                            */

    ldrls   r5, =sys_do_table               /*  系统调用处理跳转表                                      */

    ldrls   lr, =sys_do_return              /*  系统调用处理返回地址                                    */

    ldrls   pc, [r5, r7, lsl #2]            /*  跳转到相应系统调用处理                                  */

    mov     r0, #-1                         /*  出错, 返回 -1, r0 用于返回值                            */

sys_do_return:                              /*  一定会执行                                              */
    ldmfd   sp!, {r4-r12, pc}^              /*  系统调用处理返回, ^ 表示复制 spsr 到 cpsr               */
/*********************************************************************************************************
  IRQ 中断处理程序
*********************************************************************************************************/
.extern irq_c_handler
irq_handler:
    sub     lr, lr, #4                      /*  调整 lr, 被中断的地址                                   */

    ldr     sp, =(IRQ_STACK_P_BASE)         /*  设置 irq 模式的 sp                                      */

    stmfd   sp!, {r0-r12, lr}               /*  保存寄存器到 irq 堆栈                                   */

    ldr     lr, =irq_c_handler_return       /*  返回地址                                                */

    ldr     pc, =irq_c_handler              /*  进入 irq c 处理程序                                     */

irq_c_handler_return:                       /*  不一定会执行                                            */
    ldmfd   sp!, {r0-r12, pc}^              /*  irq 中断处理返回, ^ 表示复制 spsr 到 cpsr               */
/*********************************************************************************************************
  数据访问中止异常处理程序
*********************************************************************************************************/
.extern dabt_c_handler
dabt_handler:
    sub     lr, lr, #8                      /*  调整 lr, 数据访问中止异常的地址                         */

    ldr     sp, =(IRQ_STACK_P_BASE)         /*  设置 dabt 模式的 sp                                     */

    stmfd   sp!, {r0-r12, lr}               /*  保存寄存器和返回地址到 dabt 堆栈                        */

    ldr     lr, =dabt_c_handler_return      /*  返回地址                                                */

    mov     r0, lr

    mrs     r1, spsr

    ldr     pc, =dabt_c_handler             /*  进入 dabt c 处理程序                                    */

dabt_c_handler_return:
    ldmfd   sp!, {r0-r12, pc}^              /*  dabt 异常处理返回, ^ 表示复制 spsr 到 cpsr              */
/*********************************************************************************************************
  FIQ 快速中断处理程序
*********************************************************************************************************/
.extern fiq_c_handler
fiq_handler:
    ldr     sp, =(IRQ_STACK_P_BASE)         /*  设置 fiq 模式的 sp                                      */

    mov     r0, lr

    mrs     r1, spsr

    ldr     pc, =fiq_c_handler
/*********************************************************************************************************
  未定义指令异常处理程序
*********************************************************************************************************/
.extern undf_c_handler
undef_handler:
    ldr     sp, =(IRQ_STACK_P_BASE)         /*  设置 undef 模式的 sp                                    */

    mov     r0, lr

    mrs     r1, spsr

    ldr     pc, =undf_c_handler
/*********************************************************************************************************
  预取指中止异常处理程序
*********************************************************************************************************/
.extern pabt_c_handler
pabt_handler:
    ldr     sp, =(IRQ_STACK_P_BASE)         /*  设置 pabt 模式的 sp                                     */

    mov     r0, lr

    mrs     r1, spsr

    ldr     pc, =pabt_c_handler
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
