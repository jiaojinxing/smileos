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
** File name:               asm.S
** Last modified Date:      2013-3-13
** Last Version:            1.0.0
** Descriptions:            汇编文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-3-13
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
                            .text
                            .code 16
                            .align 2
/*********************************************************************************************************
** Function name:           interrupt_disable
** Descriptions:            进入临界区域
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          进入临界区域前的当前程序状态寄存器
*********************************************************************************************************/
                            .globl interrupt_disable
interrupt_disable:
    MRS     R0, PRIMASK
    CPSID   I
    BX      LR
/*********************************************************************************************************
** Function name:           interrupt_resume
** Descriptions:            退出临界区域
** input parameters:        reg                 进入临界区域前的当前程序状态寄存器
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
                            .globl interrupt_resume
interrupt_resume:
    MSR     PRIMASK, R0
    BX      LR

                            .globl arch_in_kernel
arch_in_kernel:
    MOV     R0, #1
    BX      LR

                            .globl arch_kernel_enter
arch_kernel_enter:
    BX      LR
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
