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
/*********************************************************************************************************
** Function name:           cpu_init
** Descriptions:            初始化 CPU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void cpu_init(void)
{
    WTCON       = 0x00;                                                 /*  关闭看门狗                  */

    INTMSK      = BIT_ALLMSK;                                           /*  屏蔽所有中断                */

    INTSUBMSK   = BIT_SUB_ALLMSK;                                       /*  屏蔽所有子中断              */

    mmu_disable();

    mmu_disable_dcache();

    mmu_disable_icache();
}
/*********************************************************************************************************
** Function name:           bsp_mem_map
** Descriptions:            BSP 内存映射
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void bsp_mem_map(void)
{
    /*
     * 特殊功能寄存器
     */
    mmu_map_region(0x48000000,
                   0x48000000,
                   0x60000000 - 0x48000000,
                   SECTION_ATTR(AP_USER_NO, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));

    /*
     * DM9000
     */
    mmu_map_region(0x20000000,
                   0x20000000,
                   1 * MB,
                   SECTION_ATTR(AP_USER_NO, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));
}
/*********************************************************************************************************
** BSP 保留空间
*********************************************************************************************************/
space_t bsp_resv_space[] = {
        {
            0x48000000,                                                 /*  特殊功能寄存器              */
            0x60000000 - 0x48000000
        },
        {
            0x20000000,                                                 /*  DM9000                      */
            1 * MB
        },
        {
            0,
            0
        }
};
/*********************************************************************************************************
** Function name:           bsp_init
** Descriptions:            初始化 BSP
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void bsp_init(void)
{
    extern void clock_init(void);
    clock_init();

    extern void interrupt_init(void);
    interrupt_init();

    extern void timer_init(void);
    timer_init();
}
/*********************************************************************************************************
** Function name:           bsp_drivers_install
** Descriptions:            安装 BSP 驱动
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int bsp_drivers_install(void)
{
    extern int serial0_init(void);
    serial0_init();

    extern void nand_init(void);
    nand_init();

    extern int fb_init(void);
    fb_init();

    extern int audio_init(void);
    audio_init();

    return 0;
}
/*********************************************************************************************************
** Function name:           bsp_devices_create
** Descriptions:            创建 BSP 设备
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int bsp_devices_create(void)
{
    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
