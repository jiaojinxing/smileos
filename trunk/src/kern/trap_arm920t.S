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
#include "config.h"
#include "arm.h"
#include "sys_call.h"
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
_undf:      .word __undf                                                // undefined
_swi:       .word swi_handler                                           // SWI
_pabt:      .word __pabt                                                // program abort
_dabt:      .word __dabt                                                // data abort
_irq:       .word irq_handler                                           // IRQ
_fiq:       .word __fiq                                                 // FIQ
/*********************************************************************************************************
  复位处理程序
*********************************************************************************************************/
.extern cpu_init
.extern bsp_init
reset_handler:
    msr     cpsr_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)           /*  进入 svc 模式, 关中断       */

    ldr     sp, =(KERN_MEM_BASE + KERN_MEM_SIZE)                        /*  设置堆栈指针                */

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
  软件中断处理程序
*********************************************************************************************************/
.extern sys_do_table
swi_handler:
    stmdb   sp!, {r4-r12, lr}               /*  保存寄存器和返回地址到进程的内核栈                      */
                                            /*  根据 ATPCS, 前四个参数使用 r0 - r3, 后面的参数使用堆栈  */
                                            /*  因为切换了处理器模式, SP 也切换了, 所以多于 4 个参数时, */
                                            /*  会有问题                                                */

    cmp     r7, #SYS_CALL_NR                /*  根据 EABI, 系统调用号使用 r7                            */

    ldrls   r5, =sys_do_table               /*  系统调用处理跳转表                                      */

    ldrls   lr, =sys_do_return              /*  返回地址                                                */

    ldrls   pc, [r5, r7, lsl #2]            /*  跳转到相应系统调用处理                                  */

    mov     r0, #-1                         /*  出错, 返回 -1, r0 用于返回值                            */

sys_do_return:
    ldmia   sp!, {r4-r12, pc}^              /*  系统调用处理返回, ^ 表示复制 spsr 到 cpsr               */
/*********************************************************************************************************
  IRQ 中断处理程序
*********************************************************************************************************/
.extern irq_c_handler
irq_handler:
    sub     lr, lr, #4

    ldr     sp, =(IRQ_STACK_P_BASE)

    stmdb   sp!, {r0-r12, lr}

    ldr     lr, =irq_c_handler_return

    ldr     pc, =irq_c_handler

irq_c_handler_return:
    ldmia   sp!, {r0-r12, pc}^
/*********************************************************************************************************
  其它异常处理程序
*********************************************************************************************************/
__undf:     b     .                                                     // undefined
__pabt:     b     .                                                     // program abort
__dabt:     b     .                                                     // data abort
__fiq:      b     .                                                     // FIQ
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
