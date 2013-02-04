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
** File name:               pth_mctx_asm.c
** Last modified Date:      2012-2-25
** Last Version:            1.0.0
** Descriptions:            pthread 线程机器上下文切换与 CPU 体系结构相关的汇编源文件
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
/*********************************************************************************************************
** Function name:           pth_mctx_switch
** Descriptions:            pthread 线程机器上下文切换
** input parameters:        R0                  源   pthread 线程机器上下文
                            R1                  目的 pthread 线程机器上下文
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
.text
.code 32
.align 2

.global pth_mctx_switch
pth_mctx_switch:
    STMFD   SP!,  {LR}

    STMFD   SP!,  {R0-R12, LR}

    MRS     R2,   CPSR
    STMFD   SP!,  {R2}

    STR     SP,   [R0]

    LDR     SP,   [R1]

    LDMFD   SP!,  {R2}
    MSR     CPSR_cxsf, R2

    LDMFD   SP!,  {R0-R12, LR, PC}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
