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
** Descriptions:            S3C2440 初始化
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
    WTCON       = 0x00;                                                 /*  关闭看门狗                  */

    INTMSK      = BIT_ALLMSK;                                           /*  屏蔽所有中断                */

    INTSUBMSK   = BIT_SUB_ALLMSK;                                       /*  屏蔽所有子中断              */

    mmu_disable();

    mmu_disable_dcache();

    mmu_disable_icache();
}

/*
 * BSP 内存映射
 */
void bsp_mem_map(void)
{
    /*
     * 特殊功能寄存器
     */
    mmu_map_region(0x48000000,
                   0x48000000,
                   0x60000000 - 0x48000000,
                   SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));

    /*
     * DM9000
     */
    mmu_map_region(0x20000000,
                   0x20000000,
                   1 * MB,
                   SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));
}

/*
 * BSP 保留空间
 */
space_t bsp_resv_space[] = {
        {
            0x48000000,                                                 /*  特殊功能寄存器              */
            0x60000000 - 0x48000000
        },
        {
            0x20000000,                                                 /*  DM9000                      */
            PAGE_SIZE
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
}

#include "vfs/driver.h"

/*
 * BSP 安装驱动
 */
int bsp_drivers_install(void)
{
    extern driver_t fb_drv;
    driver_install(&fb_drv);

    extern driver_t sd_drv;
    driver_install(&sd_drv);

    return 0;
}

/*
 * BSP 创建设备
 */
int bsp_devices_create(void)
{
    extern int fb_create(void);
    fb_create();

    extern int sd_create(void);
    sd_create();

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
