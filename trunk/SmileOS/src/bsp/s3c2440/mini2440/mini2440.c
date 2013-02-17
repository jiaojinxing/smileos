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
** File name:               mini2440.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            mini2440 开发板初始化
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
#include "arch/arm920t/mmu.h"
#include "vfs/vfs.h"
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
const mem_space_t bsp_resv_space[] = {
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
** Function name:           bsp_drivers_install
** Descriptions:            安装 BSP 驱动
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int bsp_drivers_install(void)
{
    extern int audio_init(void);
    audio_init();

    extern int lcd_init(void);
    lcd_init();

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
    extern int mtdblock_create(const char *path,
                               long mtd_no,
                               long start,
                               long end,
                               long reserved);

    mtdblock_create("/dev/mtdblock0", 0, 64,  255,  2);

    mtdblock_create("/dev/mtdblock1", 0, 256, 4095, 5);

    vfs_mount("/kern",   "/dev/mtdblock0", "yaffs1", "empty-lost-and-found-on,no-cache");

    vfs_mount("/rootfs", "/dev/mtdblock1", "yaffs1", "empty-lost-and-found-on,no-cache");

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
