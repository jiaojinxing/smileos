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
** File name:               s3c2440_clock.c
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
#include "kern/config.h"
#include "kern/types.h"
#include "s3c2440.h"
#include "s3c2440_clock.h"
/*********************************************************************************************************
** Function name:           set_mpll_clock
** Descriptions:            设置 MPLL
** input parameters:        sdiv                S 倍频因子
**                          pdiv                P 倍频因子
**                          mdiv                M 倍频因子
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void set_mpll_clock(uint32_t sdiv, uint32_t pdiv, uint32_t mdiv)
{
    MPLLCON = (mdiv << 12) | (pdiv << 4) | (sdiv << 0);
}
/*********************************************************************************************************
** Function name:           set_upll_clock
** Descriptions:            设置 UPLL
** input parameters:        sdiv                S 倍频因子
**                          pdiv                P 倍频因子
**                          mdiv                M 倍频因子
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void set_upll_clock(uint32_t sdiv, uint32_t pdiv, uint32_t mdiv)
{
    UPLLCON = (mdiv << 12) | (pdiv << 4) | (sdiv << 0);
}
/*********************************************************************************************************
** Function name:           set_divider
** Descriptions:            设置分频器
** input parameters:        divn_upll            UPLL 分频因子
**                          hdivn                HCLK 分频因子
**                          pdivn                PCLK 分频因子
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void set_divider(uint32_t divn_upll, uint32_t hdivn, uint32_t pdivn)
{
    CLKDIVN = (divn_upll << 3) | (hdivn << 1) | (pdivn << 0);
}
/*********************************************************************************************************
** Function name:           clock_init
** Descriptions:            设置时钟
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void clock_init(void)
{
    LOCKTIME = 0xFFFFFFFF;

    set_mpll_clock(MPLL_SDIV, MPLL_PDIV, MPLL_MIDV);

    set_upll_clock(UPLL_SDIV, UPLL_PDIV, UPLL_MDIV);

    set_divider(DIVN_UPLL, HDIVN, PDIVN);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
