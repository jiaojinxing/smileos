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
** File name:               s3c2440_int.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            S3C2440 �ж�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
** ��չ�Ķ�:
** http://hi.baidu.com/ch314156/blog/item/027cdd62feb97f3caa184c29.html
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "s3c2440.h"
/*********************************************************************************************************
** Function name:           irq_c_handler
** Descriptions:            IRQ �жϴ������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void irq_c_handler(void)
{
    intno_t interrupt;

    interrupt_enter();                                                  /*  �����ж�                    */

    interrupt = INTOFFSET;                                              /*  ����жϺ�                  */

    if (interrupt >= INTGLOBAL) {
        interrupt_exit();                                               /*  �˳��ж�                    */
        return;
    }

    interrupt_exec(interrupt);

    SRCPND  = 1 << interrupt;                                           /*  ����ж�Դ�ȴ�              */

    INTPND  = INTPND;                                                   /*  ����жϵȴ�                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}
/*********************************************************************************************************
** Function name:           interrupt_mode_init
** Descriptions:            ��ʼ���ж�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_mode_init(void)
{
    SRCPND      = 0x00;                                                 /*  ��������ж�Դ�ȴ�          */

    SUBSRCPND   = 0x00;                                                 /*  ����������ж�Դ�ȴ�        */

    INTMOD      = 0x00;                                                 /*  ���������ж�ģʽΪ IRQ      */

    INTMSK      = BIT_ALLMSK;                                           /*  ���������ж�                */

    INTSUBMSK   = BIT_SUB_ALLMSK;                                       /*  �����������ж�              */

    INTPND      = INTPND;                                               /*  ��������жϵȴ�            */
}
/*********************************************************************************************************
** Function name:           interrupt_mask
** Descriptions:            �����ж�
** input parameters:        interrupt           �жϺ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_mask(intno_t interrupt)
{
    reg_t reg;

    if (interrupt < INTGLOBAL) {

        reg = interrupt_disable();

        INTMSK |= 1 << interrupt;

        interrupt_resume(reg);
    }
}
/*********************************************************************************************************
** Function name:           interrupt_unmask
** Descriptions:            ȡ�������ж�
** input parameters:        interrupt           �жϺ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_unmask(intno_t interrupt)
{
    reg_t reg;

    if (interrupt < INTGLOBAL) {

        reg = interrupt_disable();

        INTMSK &= ~(1 << interrupt);

        interrupt_resume(reg);
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
