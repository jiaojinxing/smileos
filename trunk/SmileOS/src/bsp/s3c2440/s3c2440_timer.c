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
** File name:               s3c2440_timer.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            S3C2440 ��ʱ��
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
*********************************************************************************************************/
#include "kern/kern.h"
#include "s3c2440.h"
#include "s3c2440_clock.h"
/*********************************************************************************************************
** Function name:           timer4_isr
** Descriptions:            ��ʱ�� 4 �жϷ������
** input parameters:        interrupt           �жϺ�
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int timer4_isr(intno_t interrupt, void *arg)
{
    kernel_timer();

    return 0;
}
/*********************************************************************************************************
** Function name:           soc_timer_init
** Descriptions:            ��ʼ����ʱ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void soc_timer_init(void)
{
    /*
     * Timer input clock Frequency = PCLK / {prescaler value + 1} / {divider value}
     * {prescaler value} = 0 ~ 255
     * {divider value}   = 2, 4, 8, 16
     */

    TCFG0 = (TCFG0 & (~(0xFF << 8))) | (15 << 8);                       /*  Timer4, prescaler value = 15*/

    TCFG1 = (TCFG1 & (~(0x0F << 16))) | (1 << 16);                      /*  Set timer4 MUX 1/4          */

    TCNTB4 = (uint32_t)(PCLK / (4 * (15 + 1) * TICK_PER_SECOND)) - 1;

    TCON = (TCON & (~(0x03 << 20))) | (2 << 20);                        /*  Manual update TCNTB4        */

    interrupt_install(INTTIMER4, timer4_isr, NULL, NULL);               /*  Install isr                 */

    interrupt_unmask(INTTIMER4);

    TCON = (TCON & (~(0x03 << 20))) | (5 << 20);                        /*  Start timer4, Auto reload   */
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
