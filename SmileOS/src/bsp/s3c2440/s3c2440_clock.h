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
** File name:               s3c2440_clock.h
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            S3C2440 时钟
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
#ifndef S3C2440_CLOCK_H_
#define S3C2440_CLOCK_H_

#include "kern/types.h"

#define CONFIG_SYS_CLK_FREQ     (12000000)                              /*  Fin = 12.00MHz              */

#if CONFIG_SYS_CLK_FREQ == (12000000)
    /*
     * Mpll = 2 * m * Fin / (p * 2^s), m = MDIV + 8, p = PDIV + 2, s = SDIV
     */
    #define     MPLL_MIDV       92                                      /*  m = MPLL_MDIV + 8 = 100     */
    #define     MPLL_PDIV       4                                       /*  p = MPLL_PDIV + 2 = 6       */
    #define     MPLL_SDIV       0                                       /*  s = MPLL_SDIV = 0           */
    /*
     * Mpll = 2 * 100 * 12 / 6 = 400MHz
     */

    /*
     * Upll = m * Fin / (p * 2^s), m = MDIV + 8, p = PDIV + 2, s = SDIV
     */
    #define     UPLL_MDIV       56                                      /*  m = UPLL_MDIV + 8 = 64      */
    #define     UPLL_PDIV       2                                       /*  p = UPLL_PDIV + 2 = 4       */
    #define     UPLL_SDIV       1                                       /*  s = UPLL_SDIV = 1           */
    /*
     * Upll = 64 * 12 / 8 = 96MHz
     */

    /*
     * System clock divider, FCLK : HCLK : PCLK = 8 : 2 : 1
     */
    /*
     * UCLK must be 48MHz for USB
     */
    #define     DIVN_UPLL       1                                       /*  UCLK = UPLL / 2             */
    #define     HDIVN           2                                       /*  HCLK = FCLK / 4             */
    #define     PDIVN           1                                       /*  PCLK = HCLK / 2             */

    #define     FCLK            (400000000)
    #define     HCLK            (FCLK / 4)
    #define     PCLK            (HCLK / 2)
    #define     UCLK            (48000000)
#endif

/*
 * 设置时钟
 */
void clock_init(void);

#endif                                                                  /*  S3C2440_CLOCK_H_            */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
