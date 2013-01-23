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
** Modified by:
** Modified date:
** Version:
** Descriptions:
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
    uint32_t        status;
    uint32_t        timeslice;
    uint32_t        delay;
    uint32_t        priority;
    uint32_t        regs[20];
    uint32_t        kstack[KERN_STACK_SIZE];
*/
#define PID_OFFSET          0
#define TID_OFFSET          4
#define STATUS_OFFSET       8
#define TIMESLICE_OFFSET    12
#define DELAY_OFFSET        16
#define PRIORITY_OFFSET     20
#define REGS_OFFSET         24
#define KSTACK_OFFSET       104
/*********************************************************************************************************
** ARM 处理器模式掩码
*********************************************************************************************************/
#undef  ARM_MODE_MASK
#define ARM_MODE_MASK       0xFFFFFFE0
/*********************************************************************************************************
** 外部函数与变量
*********************************************************************************************************/
.extern task_switch_hook
/*********************************************************************************************************
** Function name:           task_switch
** Descriptions:            任务切换
** input parameters:        R0                  原任务的任务控制块
                            R1                  新任务的任务控制块
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
                            .text
                            .code 32
                            .align 2

                            .global task_switch
task_switch:                                    /*  task_switch 可能在异常和 SVC 模式下调用             */
    MRS     R2, CPSR
    BIC     R2, R2, #ARM_MODE_MASK

    CMP     R2, #ARM_SVC_MODE                   /*  判断是否在 SVC 模式下                               */
    BEQ     svc_mode_ctx_save                   /*  跳到 SVC 模式保存任务上下文                         */

    B       trap_mode_ctx_save                  /*  跳到异常模式保存任务上下文                          */
/*********************************************************************************************************
** Function name:           trap_mode_ctx_save
** Descriptions:            异常模式保存任务上下文
** input parameters:        R0                  原任务的任务控制块
                            R1                  新任务的任务控制块
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
trap_mode_ctx_save:
/*
              +-------------------------------------------+
              | 异常时的 PC                               |
              +-------------------------------------------+
              |                                           |
              | R0 - R12                                  |
              |                                           |
              +-------------------------------------------+
              | 异常时的 LR                               |
              +-------------------------------------------+
              | 异常时的 SPSR                             |
              | SYS / USR 模式无 SPSR, 设置为 ARM_SVC_MODE|
              | SVC 模式有 SPSR, SPSR 的模式为 SYS / USR  |
              +-------------------------------------------+
              | 异常时的 SP                               |
              +-------------------------------------------+
              | 异常时的 CPSR                             |
              +-------------------------------------------+
              | 原有值                                    |
              +-------------------------------------------+
*/
    ADD     R0, R0, #REGS_OFFSET                /*  R0 = current->regs                                  */

    ADD     R0, R0, #4                          /*  不修改 SP_backup                                    */

    MRS     R2, SPSR                            /*  保存异常时的 CPSR                                   */
    STMIA   R0!, {R2}

    STMIA   R0, {SP}^                           /*  保存异常时的 SP                                     */
    NOP
    ADD     R0, R0, #4

    BIC     R2, R2, #ARM_MODE_MASK
    CMP     R2, #ARM_SVC_MODE                   /*  判断异常前是否在 SVC 模式下                         */
    BEQ     svc_spsr_save                       /*  跳到保存 SVC 模式 SPSR                              */

    MOV     R2, #ARM_SVC_MODE                   /*  异常前处于 SYS 或 USR 模式, 保存一个 ARM_SVC_MODE   */
    STMIA   R0!, {R2}

    B       lp_save                             /*  跳到保存异常时的 LR                                 */

svc_spsr_save:                                  /*  保存 SVC 模式 SPSR                                  */
    MRS     R3, CPSR
    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)
    MRS     R2, SPSR
    MSR     CPSR_cxsf, R3
    STMIA   R0!, {R2}

lp_save:                                        /*  保存异常时的 LR                                     */
    STMIA   R0, {LR}^
    NOP
    ADD     R0, R0, #4

    LDR     SP, =(IRQ_STACK_P_BASE - 14 * 4)    /*  SP 指向异常进入时保存的寄存器                       */
    LDMIA   SP!, {R2-R8}                        /*  把异常进入时保存的寄存器转移到 current->regs 中     */
    STMIA   R0!, {R2-R8}
    LDMIA   SP!, {R2-R8}
    STMIA   R0!, {R2-R8}

    B       task_switch_to                      /*  执行任务切换                                        */
/*********************************************************************************************************
** Function name:           svc_mode_ctx_save
** Descriptions:            SVC 模式保存任务上下文
** input parameters:        R0                  原任务的任务控制块
                            R1                  新任务的任务控制块
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
svc_mode_ctx_save:
/*
              +--------------------------------------+
              | PC                                   |
              +--------------------------------------+
              |                                      |
              | R0 - R12                             |
              |                                      |
              +--------------------------------------+
              | 切换时的 LR                          |
              +--------------------------------------+
              | 切换时的 SPSR                        |
              +--------------------------------------+
              | 切换时的 SP                          |
              +--------------------------------------+
              | 切换时的 CPSR                        |
              +--------------------------------------+
              | 原有值                               |
              +--------------------------------------+
*/
    ADD     R0, R0, #REGS_OFFSET                /*  R0 = current->regs                                  */

    ADD     R0, R0, #4                          /*  不修改 SP_backup                                    */

    MRS     R2, CPSR                            /*  保存切换时的 CPSR                                   */
    STMIA   R0!, {R2}

    STMIA   R0!, {SP}                           /*  保存切换时的 SP                                     */

    MRS     R2, SPSR                            /*  保存切换时的 SPSR                                   */
    STMIA   R0!, {R2}

    STMIA   R0!, {LR}                           /*  保存切换时的 LR                                     */

    STMIA   R0!, {R0-R12, LR}                   /*  保存切换时的寄存器到 current->regs 中               */

    B       task_switch_to                      /*  执行任务切换                                        */
/*********************************************************************************************************
** Function name:           task_switch_to
** Descriptions:            直接切换到新任务
** input parameters:        R1                  新任务的任务控制块
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
                            .global task_switch_to
task_switch_to:
    LDR     SP, =(IRQ_STACK_P_BASE)             /*  使用 IRQ 堆栈                                       */
    PUSH    {R1}
    MOV     LR, PC
    LDR     PC, =task_switch_hook               /*  调用勾子函数                                        */
    POP     {R1}

    LDR     R0, [R1, #PID_OFFSET]               /*  新任务的 PID                                        */
    MOV     R0, R0, LSL #25                     /*  PID 在寄存器的最高 7 位                             */
    MCR     p15, 0, R0, c13, c0, 0              /*  写 PID, MVA 发生变化了!!!                           */
    NOP
    NOP
    NOP

    ADD     R1, R1, #REGS_OFFSET                /*  R1 = task[next].regs                                */
/*********************************************************************************************************
** Function name:           signal_handler_exit
** Descriptions:            信号处理退出
** input parameters:        R1                  新任务的寄存器组
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
                            .global signal_handler_exit
signal_handler_exit:
    LDMIA   R1!, {R2-R6}                        /*  R2=SP_backup, R3=CPSR_int, R4=SP_int, R5=SPSR_int   */
                                                /*  R6=LR_int                                           */

    MOV     R0, R5                              /*  下面可能还要用到 R5, 使用 R0 来作比较               */
    BIC     R0, R0, #ARM_MODE_MASK
    CMP     R0, #ARM_SVC_MODE                   /*  判断 SPSR_int 中的模式是否为 SVC 模式               */
    BEQ     sys_usr_mode_ctx_switch             /*  跳到 SYS 或 USR 模式切换                            */
/*********************************************************************************************************
** Function name:           svc_mode_ctx_switch
** Descriptions:            SVC 模式任务上下文切换
** input parameters:        R1                  新任务的寄存器组
                            R2                  SP_backup
                            R3                  CPSR_int
                            R4                  SP_int
                            R5                  SPSR_int
                            R6                  LR_int
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
svc_mode_ctx_switch:
/*
              +--------------------------------------+
              | PC                                   |
              +--------------------------------------+
              |                                      |
              | R0 - R12                             |
              |                                      |
              +--------------------------------------+
              | 切换时的 LR                          |
              +--------------------------------------+
              | 切换时的 SPSR                        |
              +--------------------------------------+
              | 切换时的 SP                          |
              +--------------------------------------+
              | 切换时的 CPSR                        |
              +--------------------------------------+
              | 原有值                               |
              +--------------------------------------+
*/
    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)   /*  进入 SVC 模式, 关中断               */

    MOV     SP, R4                              /*  恢复切换时的 SP                                     */

    MSR     SPSR_cxsf, R5                       /*  恢复切换时的 SPSR                                   */

    MOV     LR, R6                              /*  恢复切换时的 LR                                     */

    MOV     R0, R1

    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_UDF_MODE)   /*  进入 UNDEF 模式, 关中断             */

    MSR     SPSR_cxsf, R3                       /*  切换时的 CPSR -> UNDEF 模式的 SPSR                  */

    LDMIA   R0, {R0-R12, PC}^                   /*  切换到新的任务, SPSR -> CPSR                        */
/*********************************************************************************************************
** Function name:           sys_usr_mode_ctx_switch
** Descriptions:            SYS 或 USR 模式任务上下文切换
** input parameters:        R1                  新任务的存器组
                            R2                  SP_backup
                            R3                  CPSR_int
                            R4                  SP_int
                            R5                  SPSR_int
                            R6                  LR_int
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
sys_usr_mode_ctx_switch:
/*
              +-------------------------------------------+
              | 异常时的 PC                               |
              +-------------------------------------------+
              |                                           |
              | R0 - R12                                  |
              |                                           |
              +-------------------------------------------+
              | 异常时的 LR                               |
              +-------------------------------------------+
              | 异常时的 SPSR                             |
              | SYS / USR 模式无 SPSR, 设置为 ARM_SVC_MODE|
              | SVC 模式有 SPSR, SPSR 的模式为 SYS / USR  |
              +-------------------------------------------+
              | 异常时的 SP                               |
              +-------------------------------------------+
              | 异常时的 CPSR                             |
              +-------------------------------------------+
              | 原有值                                    |
              +-------------------------------------------+
*/
    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SYS_MODE)   /*  进入 SYS 模式, 关中断               */

    MOV     SP, R4                              /*  恢复异常时的 SP                                     */

    MOV     LR, R6                              /*  恢复异常时的 LR                                     */

    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SVC_MODE)   /*  进入 SVC 模式, 关中断               */

    MOV     SP, R2                              /*  设置 SVC 模式的 SP                                  */

    MSR     SPSR_cxsf, R3                       /*  异常时的 CPSR ->  SVC 模式的 SPSR                   */

    MOV     R0, R1
    LDMIA   R0, {R0-R12, PC}^                   /*  切换到新的任务, SPSR -> CPSR                        */
/*********************************************************************************************************
** Function name:           process0_enter
** Descriptions:            切换到进程 0, 并开中断
** input parameters:        R0                  新任务的 SVC 模式的 SP
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
                            .global process0_enter
process0_enter:
    MOV     R1, SP                              /*  暂存 SP 到 R1                                       */

    MOV     R2, LR                              /*  暂存 LR 到 R2                                       */

    MOV     SP, R0                              /*  设置 SVC 模式的 SP                                  */

    MSR     CPSR_c, #(ARM_FIQ_NO + ARM_IRQ_NO + ARM_SYS_MODE)   /*  进入 SYS 模式, 关中断               */

    MOV     SP, R1                              /*  恢复 SP                                             */

    MOV     LR, R2                              /*  恢复 LR                                             */

    MSR     CPSR_c, #(ARM_FIQ_EN + ARM_IRQ_EN + ARM_SYS_MODE)  /*  进入 SYS 模式, 开 IRQ 与 FIQ 中断    */

    MOV     PC, LR                              /*  返回                                                */
/*********************************************************************************************************
** Function name:           interrupt_disable
** Descriptions:            进入临界区域
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          进入临界区域前的 CPSR
*********************************************************************************************************/
                            .globl interrupt_disable
interrupt_disable:
    MRS     R0, CPSR

    ORR     R1, R0, #(ARM_FIQ_NO + ARM_IRQ_NO)

    MSR     CPSR_c, R1

    MOV     PC, LR
/*********************************************************************************************************
** Function name:           interrupt_resume
** Descriptions:            退出临界区域
** input parameters:        R0                  进入临界区域前的 CPSR
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
                            .globl interrupt_resume
interrupt_resume:
    MSR     CPSR_c, R0

    MOV     PC, LR
/*********************************************************************************************************
** Function name:           in_kernel
** Descriptions:            判断是否在内核里
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
                            .global in_kernel
in_kernel:
    MRS     R0, CPSR
    BIC     R0, R0, #ARM_MODE_MASK

    CMP     R0, #ARM_USR_MODE                   /*  判断是否在 USR 模式下                               */
    BEQ     user_mode

    CMP     R0, #ARM_SYS_MODE                   /*  判断是否在 SYS 模式下                               */
    BEQ     sys_mode

    MOV     R0, #1                              /*  返回 TRUE                                           */
    MOV     PC, LR

user_mode:
sys_mode:
    MOV     R0, #0                              /*  返回 FALSE                                          */
    MOV     PC, LR
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
