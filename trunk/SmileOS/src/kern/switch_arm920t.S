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
** Modified by:             JiaoJinXing
** Modified date:           2012-3-25
** Version:                 1.1.0
** Descriptions:            简化 __switch_to 中对 CPU 模式的判断, 以适合各种异常处理程序退出时的任务切换
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
/*********************************************************************************************************
** 成员变量在 task_t 结构中的偏移量
*********************************************************************************************************/
/*
    int32_t         pid;
    int32_t         tid;
    uint32_t        state;
    uint32_t        counter;
    uint32_t        timer;
    uint32_t        priority;
    uint32_t        content[20];
    uint32_t        kstack[KERN_STACK_SIZE];
*/
#define PID_OFFSET          0
#define TID_OFFSET          4
#define STATE_OFFSET        8
#define COUNTER_OFFSET      12
#define TIMER_OFFSET        16
#define PRIORITY_OFFSET     20
#define CONTENT_OFFSET      24
#define STACK_OFFSET        104

#undef  ARM_MODE_MASK
#define ARM_MODE_MASK       0xFFFFFFE0
/*********************************************************************************************************
** 任务切换
*********************************************************************************************************/
.text
.code 32
.align 2
.global __switch_to
__switch_to:                                    /*  __switch_to 可能在异常和 SVC 模式下调用             */
    MRS     R2, CPSR
    BIC     R2, R2, #ARM_MODE_MASK

    CMP     R2, #ARM_SVC_MODE                   /*  判断是否在 SVC 模式下                               */
    BEQ     svc_mode_save

    B       trap_mode_save
/*********************************************************************************************************
** trap 模式保存任务上下文
*********************************************************************************************************/
trap_mode_save:
/*
              +--------------------------------------+
              | 中断时的 PC (SYS 模式)               |
              +--------------------------------------+
              |                                      |
              | R0 - R12 (SYS 模式)                  |
              |                                      |
              +--------------------------------------+
              | 中断时的 LR (SYS 模式)               |
              +--------------------------------------+
              | 中断时的 SPSR = 0x13, SVC 模式(无效) |
              +--------------------------------------+
              | 中断时的 SP (SYS 模式)               |
              +--------------------------------------+
              | 中断时的 CPSR (SYS 模式)             |
              +--------------------------------------+
              | 原有值                               |
              +--------------------------------------+
*/
    ADD     R0, R0, #CONTENT_OFFSET             /*  R0 = current->content                               */
    LDR     SP, =(IRQ_STACK_P_BASE - 14 * 4)    /*  SP 指向中断进入时保存的寄存器                       */

    ADD     R0, R0, #4                          /*  不修改 SP_backup                                    */

    MRS     R2, SPSR                            /*  中断时的 CPSR (SYS 模式)                            */
    STMIA   R0!, {R2}

    STMIA   R0!, {SP}^                          /*  中断时的 SP (SYS 模式)                              */
    NOP

    MOV     R2, #ARM_SVC_MODE                   /*  中断时的 SPSR = 0x13, SVC 模式(无效)                */
    STMIA   R0!, {R2}

    STMIA   R0!, {LR}^                          /*  中断时的 LR (SYS 模式)                              */
    NOP

    LDMIA   SP!, {R2-R8}                        /*  把中断进入时保存的寄存器转移到 current->content 中  */
    STMIA   R0!, {R2-R8}
    LDMIA   SP!, {R2-R8}
    STMIA   R0!, {R2-R8}

    B       do_switch                           /*  执行任务切换                                        */
/*********************************************************************************************************
** SVC 模式保存任务上下文
*********************************************************************************************************/
svc_mode_save:
/*
              +--------------------------------------+
              | PC (SVC 模式)                        |
              +--------------------------------------+
              |                                      |
              | R0 - R12 (SVC 模式)                  |
              |                                      |
              +--------------------------------------+
              | 中断时的 LR (SVC 模式)               |
              +--------------------------------------+
              | 中断时的 SPSR (SYS 模式 CPSR != 0x13)|
              +--------------------------------------+
              | 中断时的 SP (SVC 模式)               |
              +--------------------------------------+
              | 中断时的 CPSR (SVC 模式)             |
              +--------------------------------------+
              | 系统调用时的 SP (SYS 模式)           |
              +--------------------------------------+
*/
    ADD     R0, R0, #CONTENT_OFFSET             /*  R0 = current->content                               */

    STMIA   R0!, {SP}^                          /*  系统调用时的 SP (SYS 模式)                          */
    NOP

    MRS     R2, CPSR                            /*  中断时的 CPSR (SVC 模式)                            */
    STMIA   R0!, {R2}

    STMIA   R0!, {SP}                           /*  中断时的 SP (SVC 模式)                              */

    MRS     R2, SPSR                            /*  中断时的 SPSR (SYS 模式 CPSR != 0x13)               */
    STMIA   R0!, {R2}

    STMIA   R0!, {LR}                           /*  中断时的 LR (SVC 模式)                              */

    STMIA   R0!, {R0-R12, LR}                   /*  保存寄存器到 current->content 中                    */

    B       do_switch                           /*  执行任务切换                                        */
/*********************************************************************************************************
** 执行任务切换
*********************************************************************************************************/
do_switch:
    LDR     R0, [R1, #PID_OFFSET]               /*  新任务的 PID                                        */
    MOV     R0, R0, LSL #25                     /*  PID 在寄存器的最高 7 位                             */
    MCR     p15, 0, R0, c13, c0, 0              /*  写 PID, MVA 发生变化了!!!                           */
    NOP
    NOP
    NOP

    ADD     R1, R1, #CONTENT_OFFSET             /*  R1 = task[next].content                             */

    LDMIA   R1!, {R2-R6}                        /*  R2=SP_backup, r3=CPSR_int, R4=SP_int, R5=SPSR_int   */
                                                /*  R6=LR_int                                           */

    MOV     R0, R5                              /*  下面可能还要用到 R5, 使用 R0 来作比较               */
    BIC     R0, R0, #ARM_MODE_MASK
    CMP     R0, #ARM_SVC_MODE                   /*  判断 SPSR_int 中的模式是否为 SVC 模式               */
    BEQ     svc_mode_switch
/*********************************************************************************************************
** SYS 模式任务切换
*********************************************************************************************************/
sys_mode_switch:
/*
              +--------------------------------------+
              | PC (SVC 模式)                        |
              +--------------------------------------+
              |                                      |
              | R0 - R12 (SVC 模式)                  |
              |                                      |
              +--------------------------------------+
              | 中断时的 LR (SVC 模式)               |
              +--------------------------------------+
              | 中断时的 SPSR (SYS 模式 CPSR != 0x13)|
              +--------------------------------------+
              | 中断时的 SP (SVC 模式)               |
              +--------------------------------------+
              | 中断时的 CPSR (SVC 模式)             |
              +--------------------------------------+
              | 系统调用时的 SP (SYS 模式)           |
              +--------------------------------------+
*/
    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SYS_MODE)   /*  进入 SYS 模式, 关中断               */

    MOV     SP, R2                              /*  设置 SYS 模式的 SP                                  */

    MSR     CPSR_cxsf, R3                       /*  进入 SVC 模式                                       */

    MOV     SP, R4                              /*  中断时的 SP                                         */

    MSR     SPSR_cxsf, R5                       /*  中断时的 SPSR                                       */

    MOV     LR, R6                              /*  中断时的 LR                                         */

    MOV     R0, R1
    LDMIA   R0, {R0-R12, PC}                    /*  切换到新的任务                                      */
/*********************************************************************************************************
** SVC 模式任务切换
*********************************************************************************************************/
svc_mode_switch:
/*
              +--------------------------------------+
              | 中断时的 PC (SYS 模式)               |
              +--------------------------------------+
              |                                      |
              | R0 - R12 (SYS 模式)                  |
              |                                      |
              +--------------------------------------+
              | 中断时的 LR (SYS 模式)               |
              +--------------------------------------+
              | 中断时的 SPSR = 0x13, SVC 模式(无效) |
              +--------------------------------------+
              | 中断时的 SP (SYS 模式)               |
              +--------------------------------------+
              | 中断时的 CPSR (SYS 模式)             |
              +--------------------------------------+
              | 原有值                               |
              +--------------------------------------+
*/
    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SYS_MODE)   /*  进入 SYS 模式, 关中断               */

    MOV     SP, R4                              /*  中断时的 SP                                         */

    MOV     LR, R6                              /*  中断时的 LR                                         */

    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)   /*  进入 SVC 模式, 关中断               */

    MOV     SP, R2                              /*  设置 SVC 模式的 SP                                  */

    MSR     SPSR_cxsf, R3                       /*  将 SYS 模式的 CPSR 保存到 SVC 模式的 SPSR           */

    MOV     R0, R1
    LDMIA   R0, {R0-R12, PC}^                   /*  切换到新的任务, ^ 表示复制 SPSR 到 CPSR             */
    /*
     * 记录以前犯下的一个错误: LDMIA   R0, {R0-R12, LR, PC}^
     * 你知道错在哪里吗?
     */
/*********************************************************************************************************
** 切换到进程 0, 并开中断
*********************************************************************************************************/
.global __switch_to_process0
__switch_to_process0:
    MOV     R1, SP                              /*  暂存 SP 到 R1                                       */

    MOV     R2, LR                              /*  暂存 LR 到 R2                                       */

    MOV     SP, R0                              /*  设置 SVC 模式的 SP                                  */

    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SYS_MODE)   /*  进入 SYS 模式, 关中断               */

    MOV     LR, R2                              /*  恢复 LR                                             */

    MOV     SP, R1                              /*  恢复 SP                                             */

    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_EN + ARM_SYS_MODE)   /*  进入 SYS 模式, 开 IRQ 中断          */

    BX      LR                                  /*  返回                                                */
/*********************************************************************************************************
** 进入临界区域
*********************************************************************************************************/
.globl interrupt_disable
interrupt_disable:
    MRS     R0, CPSR

    ORR     R1, R0, #(ARM_FIQ_NO + ARM_IRQ_NO)

    MSR     CPSR_c, R1

    MOV     PC, LR
/*********************************************************************************************************
** 退出临界区域
*********************************************************************************************************/
.globl interrupt_resume
interrupt_resume:
    MSR     CPSR_c, R0

    MOV     PC, LR
/*********************************************************************************************************
** 判断是否在内核里
*********************************************************************************************************/
.global in_kernel
in_kernel:
    MRS     R0, CPSR
    BIC     R0, R0, #ARM_MODE_MASK

    CMP     R0, #ARM_SYS_MODE                   /*  判断是否在 SYS 模式下                               */
    BEQ     sys_mode

    MOV     R0, #1
    MOV     PC, LR

sys_mode:
    MOV     R0, #0
    MOV     PC, LR
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
