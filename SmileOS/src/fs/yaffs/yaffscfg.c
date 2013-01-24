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
#include "kern/kern.h"
#include "kern/ipc.h"
#include "vfs/config.h"
#include "vfs/types.h"
#include "vfs/vfs.h"
#include "vfs/device.h"
#include "./src/yaffscfg.h"
#include "./src/yaffsfs.h"
#include "./src/yaffs_flashif.h"

extern void yaffsfs_LocalInitialisation(void);
extern int ydevice_GetInfo(yaffs_Device *ydev);
extern int ydevice_WriteChunk(yaffs_Device *ydev, int chunkInNAND, const __u8 *data, const yaffs_Spare *spare);
extern int ydevice_ReadChunk(yaffs_Device *ydev, int chunkInNAND, __u8 *data, yaffs_Spare *spare);
extern int ydevice_EraseBlock(yaffs_Device *ydev, int blockNumber);
extern int ydevice_Initialise(yaffs_Device *ydev);
extern mutex_t dev_mgr_lock;

/*********************************************************************************************************
** Function name:           yaffs_StartUp
** Descriptions:            启动 yaffs
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int yaffs_StartUp(void)
{
    yaffsfs_DeviceConfiguration *cfg, *cfg_bak;
    yaffs_Device                *ydev;
    device_t                    *dev;
    int                          i;
    int                          max;
    char                         buf[64];

	yaffsfs_LocalInitialisation();

    for (i = 0; ; i++) {
        snprintf(buf, sizeof(buf), "/dev/mtdblock%d", i);
        dev = device_lookup(buf);
        if (dev == NULL) {
            break;
        }
    }
    max = i;
    if (max == 0) {
        return -1;
    }

    cfg_bak = cfg = kmalloc(sizeof(yaffsfs_DeviceConfiguration) * (max + 1));
    if (cfg == NULL) {
        return -1;
    }
    memset(cfg_bak, 0, sizeof(yaffsfs_DeviceConfiguration) * (max + 1));

	for (i = 0; i < max; i++) {

	    snprintf(buf, sizeof(buf), "/dev/mtdblock%d", i);
        mutex_lock(&dev_mgr_lock, 0);
        dev = device_lookup(buf);
        if (dev == NULL) {
            continue;
        }
        if (atomic_inc_return(&dev->ref) != 1) {
            atomic_dec(&dev->ref);
            continue;
        }
        mutex_unlock(&dev_mgr_lock);

	    ydev = (yaffs_Device *)kmalloc(sizeof(yaffs_Device));
	    if (ydev == NULL) {
            atomic_dec(&dev->ref);
            continue;
	    }
	    memset(ydev, 0, sizeof(yaffs_Device));

        ydev->context = (void *)dev;

	    if (ydevice_GetInfo(ydev) != YAFFS_OK) {
            kfree(ydev);
            atomic_dec(&dev->ref);
            continue;
	    }

	    ydev->param.writeChunkToNAND  = ydevice_WriteChunk;
	    ydev->param.readChunkFromNAND = ydevice_ReadChunk;
	    ydev->param.eraseBlockInNAND  = ydevice_EraseBlock;
	    ydev->param.initialiseNAND    = ydevice_Initialise;

        cfg->dev    = ydev;
        cfg->prefix = ydev->param.name;
        if (cfg->prefix == NULL) {
            snprintf(buf, sizeof(buf), "/p%d", i);
            cfg->prefix = strdup(buf);
            if (cfg->prefix == NULL) {
                kfree(ydev);
                atomic_dec(&dev->ref);
                continue;
            }
        }
        cfg++;
	}

	cfg->prefix = NULL;
	cfg->dev    = NULL;

	if (cfg == cfg_bak) {
	    kfree(cfg_bak);
	    return -1;
	} else {
	    yaffs_initialise(cfg_bak);
	    return 0;
	}
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
