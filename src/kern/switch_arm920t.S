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
** File name:               switch_arm920t.s
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            ARM920T 任务切换
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
/*********************************************************************************************************
  成员变量在 task_t 结构中的偏移量
*********************************************************************************************************/
/*
    int32_t         pid;
    int32_t         tid;
    uint32_t        state;
    uint32_t        count;
    uint32_t        timer;
    uint32_t        prio;
    uint32_t        content[20];
    uint32_t        kstack[KERN_STACK_SIZE];
*/
#define PID_OFFSET          0
#define TID_OFFSET          4
#define STATE_OFFSET        8
#define COUNT_OFFSET        12
#define TIMER_OFFSET        16
#define PRIORITY_OFFSET     20
#define CONTENT_OFFSET      24
#define STACK_OFFSET        104

#undef  ARM_MODE_MASK
#define ARM_MODE_MASK       0xFFFFFFE0
/*********************************************************************************************************
  任务切换
*********************************************************************************************************/
.text
.code 32
.align 2
.global __switch_to
__switch_to:                                    /*  __switch_to 可能在 irq 和 svc 模式下调用            */
    mrs    r2, cpsr
    bic    r2, r2, #ARM_MODE_MASK

    cmp    r2, #ARM_IRQ_MODE                    /*  判断是否在 irq 模式下                               */
    beq    irq_mode_save

    cmp    r2, #ARM_SVC_MODE                    /*  判断是否在 svc 模式下                               */
    beq    svc_mode_save

    mov    pc, lr                               /*  都不是, 返回                                        */
/*********************************************************************************************************
  irq 模式保存任务上下文
*********************************************************************************************************/
irq_mode_save:
/*
              +--------------------------------------+
              | 中断时的 pc (sys 模式)               |
              +--------------------------------------+
              | 中断时的 lr (sys 模式)               |
              +--------------------------------------+
              |                                      |
              | r0 - r12 (sys 模式)                  |
              |                                      |
              +--------------------------------------+
              | 中断时的 spsr = 0x13, svc 模式(无效) |
              +--------------------------------------+
              | 中断时的 sp (sys 模式)               |
              +--------------------------------------+
              | 中断时的 cpsr (sys 模式)             |
              +--------------------------------------+
              | 原有值                               |
              +--------------------------------------+
*/
    add     r0, r0, #CONTENT_OFFSET             /*  r0 = current->content                               */
    ldr     sp, =(IRQ_STACK_P_BASE - 14 * 4)    /*  sp 指向中断进入时保存的寄存器                       */

    add     r0, r0, #4                          /*  不修改 sp_backup                                    */

    mrs     r2, spsr                            /*  中断时的 cpsr (sys 模式)                            */
    stmia   r0!, {r2}

    stmia   r0!, {sp}^                          /*  中断时的 sp (sys 模式)                              */
    nop

    mov     r2, #ARM_SVC_MODE                   /*  中断时的 spsr = 0x13, svc 模式(无效)                */
    stmia   r0!, {r2}

    ldmia   sp!, {r2-r8}                        /*  把中断进入时保存的寄存器转移到 current->content 中  */
    stmia   r0!, {r2-r8}
    ldmia   sp!, {r2-r7}
    stmia   r0!, {r2-r7}

    stmia   r0!, {lr}^                          /*  中断时的 lr (sys 模式)                              */
    nop

    ldmia   sp!, {r2}                           /*  中断时的 pc (sys 模式)                              */
    stmia   r0!, {r2}

    b       do_switch                           /*  执行任务切换                                        */
/*********************************************************************************************************
  svc 模式保存任务上下文
*********************************************************************************************************/
svc_mode_save:
/*
              +--------------------------------------+
              | pc (svc 模式)                        |
              +--------------------------------------+
              | lr (svc 模式)                        |
              +--------------------------------------+
              |                                      |
              | r0 - r12 (svc 模式)                  |
              |                                      |
              +--------------------------------------+
              | 中断时的 spsr (sys 模式 cpsr != 0x13)|
              +--------------------------------------+
              | 中断时的 sp (svc 模式)               |
              +--------------------------------------+
              | 中断时的 cpsr (svc 模式)             |
              +--------------------------------------+
              | 系统调用时的 sp (sys 模式)           |
              +--------------------------------------+
*/
    add     r0, r0, #CONTENT_OFFSET             /*  r0 = current->content                               */

    stmia   r0!, {sp}^                          /*  系统调用时的 sp (sys 模式)                          */
    nop

    mrs     r2, cpsr                            /*  中断时的 cpsr (svc 模式)                            */
    stmia   r0!, {r2}

    stmia   r0!, {sp}                           /*  中断时的 sp (svc 模式)                              */

    mrs     r2, spsr                            /*  中断时的 spsr                                       */
    stmia   r0!, {r2}

    stmia   r0!, {r0-r12, lr}                   /*  保存寄存器到 current->content 中                    */
    nop
    stmia   r0!, {lr}                           /*  lr 代替 pc 保存到 current->content 中               */

    b       do_switch                           /*  执行任务切换                                        */
/*********************************************************************************************************
  执行任务切换
*********************************************************************************************************/
do_switch:
    mov     r0, #0
    mcr     p15, 0, r0, c7, c10, 4

    ldr     r0, [r1, #PID_OFFSET]               /*  要切换到的任务的 PID                                */
    mov     r0, r0, lsl #25                     /*  PID 在寄存器的最高 7 位                             */
    mcr     p15, 0, r0, c13, c0, 0              /*  写 PID, MVA 发生变化了!!!                           */
    nop
    nop
    nop

    add     r1, r1, #CONTENT_OFFSET             /*  r1 = task[next].content                             */

    ldmia   r1!, {r2-r5}                        /*  r2=sp_backup, r3=cpsr_int, r4=sp_int, r5=spsr_int   */

    mov     r0, r5
    bic     r0, r0, #ARM_MODE_MASK
    cmp     r0, #ARM_SVC_MODE                   /*  判断 spsr_int 中的模式是否为 svc 模式               */
    beq     svc_mode_switch

/*
              +--------------------------------------+
              | pc (svc 模式)                        |
              +--------------------------------------+
              | lr (svc 模式)                        |
              +--------------------------------------+
              |                                      |
              | r0 - r12 (svc 模式)                  |
              |                                      |
              +--------------------------------------+
              | 中断时的 spsr (sys 模式 cpsr != 0x13)|
              +--------------------------------------+
              | 中断时的 sp (svc 模式)               |
              +--------------------------------------+
              | 中断时的 cpsr (svc 模式)             |
              +--------------------------------------+
              | 系统调用时的 sp (sys 模式)           |
              +--------------------------------------+
*/
    msr     cpsr_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SYS_MODE)   /*  进入 sys 模式, 关中断               */
    mov     sp, r2                              /*  设置 sys 模式的sp                                   */

    msr     cpsr_cxsf, r3                       /*  进入 svc 模式                                       */
    mov     sp, r4                              /*  中断时的 sp                                         */

    msr     spsr_cxsf, r5                       /*  中断时的 spsr                                       */

    mov     r0, r1
    ldmia   r0, {r0-r12, lr, pc}                /*  切换到新的任务                                      */

svc_mode_switch:
/*
              +--------------------------------------+
              | 中断时的 pc (sys 模式)               |
              +--------------------------------------+
              | 中断时的 lr (sys 模式)               |
              +--------------------------------------+
              |                                      |
              | r0 - r12 (sys 模式)                  |
              |                                      |
              +--------------------------------------+
              | 中断时的 spsr = 0x13, svc 模式(无效) |
              +--------------------------------------+
              | 中断时的 sp (sys 模式)               |
              +--------------------------------------+
              | 中断时的 cpsr (sys 模式)             |
              +--------------------------------------+
              | 原有值                               |
              +--------------------------------------+
*/
    orr     r6, r3, #(ARM_FIQ_NO + ARM_IRQ_NO)  /*  中断时的 cpsr, 关中断                               */
    msr     cpsr_cxsf, r6
    mov     sp, r4                              /*  中断时的 sp                                         */

    msr     cpsr_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)   /*  进入 svc 模式, 关中断               */
    mov     sp, r2                              /*  设置 svc 模式的sp                                   */

    msr     spsr_cxsf, r3

    mov     r0, r1
    ldmia   r0, {r0-r12, lr, pc}^               /*  切换到新的任务                                      */
/*********************************************************************************************************
  切换到进程 0, 并开中断
*********************************************************************************************************/
.global __switch_to_process0
__switch_to_process0:
    mov     r1, sp                              /*  设置 svc 模式的 sp                                  */

    mov     sp, r0

    mov     r2, lr

    msr     cpsr_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SYS_MODE)    /*  进入 sys 模式                      */

    mov     lr, r2                              /*  设置 sys 模式的 sp                                  */

    mov     sp, r1

    msr     cpsr_c, #(ARM_FIQ_NO + ARM_IRQ_EN + ARM_SYS_MODE)  /*  开 irq 中断                                         */

    bx      lr                                   /*  loop here                                           */
/*********************************************************************************************************
  pthread 机器上下文切换
*********************************************************************************************************/
.global pth_mctx_switch
pth_mctx_switch:
    stmfd   sp!,  {lr}
    stmfd   sp!,  {r0-r12, lr}
    mrs     r2,   cpsr
    stmfd   sp!,  {r2}
    str     sp,   [r0]

    ldr     sp,   [r1]
    ldmfd   sp!,  {r2}
    msr     cpsr_cxsf, r2
    ldmfd   sp!,  {r0-r12, lr, pc}
/*********************************************************************************************************
  进退临界区域
*********************************************************************************************************/
.globl interrupt_disable
interrupt_disable:
    mrs     r0, cpsr
    orr     r1, r0, #(ARM_FIQ_NO + ARM_IRQ_NO)
    msr     cpsr_c, r1
    mov     pc, lr

.globl interrupt_resume
interrupt_resume:
    msr     cpsr, r0
    mov     pc, lr
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
