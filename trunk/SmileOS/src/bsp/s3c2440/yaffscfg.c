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
** File name:               yaffscfg.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            配置 yaffs
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
#include "vfs/vfs.h"
#include "../../fs/yaffs2/src/yaffscfg.h"
#include "../../fs/yaffs2/src/yaffsfs.h"
#include "../../fs/yaffs2/src/yaffs_guts.h"
#include "../../fs/yaffs2/src/yaffs_mtdif.h"
#include "../../fs/yaffs2/src/yaffs_mtdif1.h"
#include "../../fs/yaffs2/src/yaffs_mtdif2.h"
#include "../../fs/yaffs2/src/yaffs_linux.h"
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>

static yaffs_Device kern_part;
static struct yaffs_LinuxContext kern_part_ctx;

static yaffs_Device root_part;
static struct yaffs_LinuxContext root_part_ctx;

static yaffsfs_DeviceConfiguration yaffsfs_config[] = {

    { "/kimage", &kern_part},

    { "/rootfs", &root_part},

    {(void *)0, (void *)0}
};
/*********************************************************************************************************
** Function name:           yaffs_StartUp
** Descriptions:            启动 yaffs
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int yaffs_StartUp(void)
{
    yaffsfs_DeviceConfiguration *cfg;
    struct mtd_info *mtd;

    extern void yaffsfs_LocalInitialisation(void);
    yaffsfs_LocalInitialisation();

    mtd = get_mtd_device(NULL, 0);
    if (mtd == NULL) {
        return -1;
    }

    kern_part_ctx.mtd = mtd;
    root_part_ctx.mtd = mtd;

    kern_part.param.name = yaffsfs_config[0].prefix;
    kern_part.param.totalBytesPerChunk = mtd->writesize;
    kern_part.param.nChunksPerBlock = mtd->erasesize / mtd->writesize;
    kern_part.param.nReservedBlocks = 2;
    kern_part.param.startBlock = 64;
    kern_part.param.endBlock = 255;
    kern_part.param.useNANDECC = 0;
    kern_part.param.noTagsECC = 0;
    kern_part.param.nShortOpCaches = 10;
    kern_part.context = (void *)&kern_part_ctx;
    kern_part.param.writeChunkWithTagsToNAND = nandmtd1_WriteChunkWithTagsToNAND;
    kern_part.param.readChunkWithTagsFromNAND = nandmtd1_ReadChunkWithTagsFromNAND;
    kern_part.param.markNANDBlockBad = nandmtd1_MarkNANDBlockBad;
    kern_part.param.queryNANDBlock = nandmtd1_QueryNANDBlock;
    kern_part.param.eraseBlockInNAND = nandmtd_EraseBlockInNAND;
    kern_part.param.initialiseNAND = nandmtd_InitialiseNAND;
    kern_part.param.isYaffs2 = 0;

    root_part.param.name = yaffsfs_config[1].prefix;
    root_part.param.totalBytesPerChunk = mtd->writesize;
    root_part.param.nChunksPerBlock = mtd->erasesize / mtd->writesize;
    root_part.param.nReservedBlocks = 5;
    root_part.param.startBlock = 256;
    root_part.param.endBlock = 4095;
    root_part.param.useNANDECC = 0;
    root_part.param.noTagsECC = 0;
    root_part.param.nShortOpCaches = 10;
    root_part.context = (void *)&root_part_ctx;
    root_part.param.writeChunkWithTagsToNAND = nandmtd1_WriteChunkWithTagsToNAND;
    root_part.param.readChunkWithTagsFromNAND = nandmtd1_ReadChunkWithTagsFromNAND;
    root_part.param.markNANDBlockBad = nandmtd1_MarkNANDBlockBad;
    root_part.param.queryNANDBlock = nandmtd1_QueryNANDBlock;
    root_part.param.eraseBlockInNAND = nandmtd_EraseBlockInNAND;
    root_part.param.initialiseNAND = nandmtd_InitialiseNAND;
    root_part.param.isYaffs2 = 0;

    yaffs_initialise(yaffsfs_config);

    for (cfg = yaffsfs_config; cfg->prefix != NULL; cfg++) {
        if (yaffs_mount(cfg->prefix) < 0) {
            continue;
        }
        extern file_system_t yaffs;
        if (vfs_mount_point_create(cfg->prefix, &yaffs, NULL) < 0) {
            yaffs_unmount(cfg->prefix);
        }
    }
    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
