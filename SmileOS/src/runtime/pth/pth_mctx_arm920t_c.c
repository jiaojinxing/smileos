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
** File name:               pth_mctx_arm920t_c.c
** Last modified Date:      2012-2-25
** Last Version:            1.0.0
** Descriptions:            pthread 线程机器上下文构建与 CPU 体系结构相关的 C 语言源文件
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
#include "pth_p.h"
#include "kern/types.h"
#include "kern/arm.h"

/*
 * 构建 pthread 线程机器上下文
 */
int pth_mctx_set(pth_mctx_t *mctx, void (*func)(void), char *sk_addr_lo, char *sk_addr_hi)
{
    uint32_t *stk;

    stk    = MEM_ALIGN_LESS(sk_addr_hi - sizeof(uint32_t));
    *stk-- = (uint32_t)func;    /* pc   */
    *stk-- = (uint32_t)func;    /* lr   */
    *stk-- = 0;                 /* r12  */
    *stk-- = 0;                 /* r11  */
    *stk-- = 0;                 /* r10  */
    *stk-- = 0;                 /* r9   */
    *stk-- = 0;                 /* r8   */
    *stk-- = 0;                 /* r7   */
    *stk-- = 0;                 /* r6   */
    *stk-- = 0;                 /* r5   */
    *stk-- = 0;                 /* r4   */
    *stk-- = 0;                 /* r3   */
    *stk-- = 0;                 /* r2   */
    *stk-- = 0;                 /* r1   */
    *stk-- = 0;                 /* r0   */
    *stk   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;        /* cpsr: sys mode, irq enable, fiq disable  */

    mctx->sp = (uint32_t)stk;

    return 1;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
