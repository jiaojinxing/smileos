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
** Descriptions:            pthread �̻߳��������Ĺ����� CPU ��ϵ�ṹ��ص� C ����Դ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-25
** Version:                 1.0.0
** Descriptions:            �����ļ�
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
/*********************************************************************************************************
** Function name:           pth_mctx_set
** Descriptions:            ���� pthread �̻߳���������
** input parameters:        mctx                pthread �̻߳���������
**                          func                �̺߳���
**                          sk_addr_lo          ջ��
**                          sk_addr_hi          ջ��
** output parameters:       NONE
** Returned value:          1
*********************************************************************************************************/
int pth_mctx_set(pth_mctx_t *mctx, void (*func)(void), char *sk_addr_lo, char *sk_addr_hi)
{
    uint32_t *stk;

    stk    = MEM_ALIGN_LESS(sk_addr_hi - sizeof(uint32_t));
    *stk-- = (uint32_t)func;                                /*  PC                                      */
    *stk-- = (uint32_t)func;                                /*  LR                                      */
    *stk-- = 0;                                             /*  R12                                     */
    *stk-- = 0;                                             /*  R11                                     */
    *stk-- = 0;                                             /*  R10                                     */
    *stk-- = 0;                                             /*  R9                                      */
    *stk-- = 0;                                             /*  R8                                      */
    *stk-- = 0;                                             /*  R7                                      */
    *stk-- = 0;                                             /*  R6                                      */
    *stk-- = 0;                                             /*  R5                                      */
    *stk-- = 0;                                             /*  R4                                      */
    *stk-- = 0;                                             /*  R3                                      */
    *stk-- = 0;                                             /*  R2                                      */
    *stk-- = 0;                                             /*  R1                                      */
    *stk-- = 0;                                             /*  R0                                      */
    *stk   = ARM_USR_MODE | ARM_FIQ_EN | ARM_IRQ_EN;        /*  CPSR: USR MODE, IRQ ENABLE, FIQ ENABLE  */

    mctx->sp = (uint32_t)stk;

    return 1;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
