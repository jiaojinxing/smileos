/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <JiaoJinXing1987@gmail.com>
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
** File name:               s3c2440.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            s3c2440 初始化
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
#include "kern/mmu.h"
#include "kern/kern.h"
#include "s3c2440.h"
#include "s3c2440_clock.h"
#include "s3c2440_uart.h"
#include "s3c2440_int.h"
#include "s3c2440_timer.h"

/*
 * 初始化 CPU
 */
void cpu_init(void)
{
    WTCON = 0;                                                          /*  关闭看门狗                  */

    INTMSK = 0xffffffff;                                                /*  屏蔽所有中断                */

    INTSUBMSK = 0x7fff;                                                 /*  屏蔽所有子中断              */
}

/*
 * BSP 内存映射
 */
void bsp_mem_map(void)
{
    /*
     * sfr
     */
    mmu_map_sections(0x48000000,
                     0x48000000,
                     0x60000000 - 0x48000000,
                     SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));

    /*
     * dm9000
     */
    mmu_map_sections(0x20000000,
                     0x20000000,
                     1 * MB,
                     SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));
}

/*
 * BSP 保留空间
 */
virtual_space_t bsp_resv_space[] = {
        {
            0x48000000,
            0x60000000 - 0x48000000
        },
        {
            0x20000000,
            1 * MB
        },
        {
            0,
            0
        }
};

/*
 * 初始化 BSP
 */
void bsp_init(void)
{
    clock_init();

    uart_init();

    interrupt_init();

    timer_init();

    /*
     * Set GPA15 as nGCS4
     */
    GPACON |= 1 << 15;

    /*
     * DM9000 width 16, wait enable
     */
    BWSCON   = (BWSCON & (~(7 << 16))) | (5 << 16);

    BANKCON4 = (1 << 13) | (1 << 11) | (6 << 8) | (1 << 6) | (1 << 4) | (0 << 2) | (0 << 0);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
