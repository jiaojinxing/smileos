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
** File name:               s3c2440_uart.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            S3C2440 UART
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
#include "kern/kern.h"
#include "s3c2440.h"
#include "s3c2440_clock.h"
/*********************************************************************************************************
** Function name:           uart_init
** Descriptions:            初始化 UART
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void uart_init(void)
{
    GPHCON  = (GPHCON & (~(0x0F << 4))) | (0x0A << 4);

    GPHUP  |= 0x0F;

    UFCON0  = 0x00;

    UMCON0  = 0x00;

    ULCON0  = 0x03;

    UCON0   = 0x245;

    /*
     * (int)(UART clock / (baud rate x 16)) –1
     */
    UBRD0   = ((uint32_t)(PCLK / 16.0 / UART_BAUD_RATE + 0.5) - 1);
}
/*********************************************************************************************************
** Function name:           kputc
** Descriptions:            通过串口输出一个字符
** input parameters:        c                   字符
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kputc(unsigned char c)
{
#define TXD0READY   (1 << 1)

    while (!(USTAT0 & TXD0READY)) {
        ;
    }

    UTXH0 = c;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
