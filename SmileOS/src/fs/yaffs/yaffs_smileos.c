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
** File name:               yaffs_smileos.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            yaffs 文件系统系统模拟层
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-27
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
#include "vfs/types.h"
#include "./src/yaffsfs.h"
#include "./src/yportenv.h"
#include "./src/yaffs_flashif.h"
#include "./src/yaffs_guts.h"
#include "./src/devextras.h"

#include <errno.h>

unsigned int    yaffs_traceMask = 0xF0000000;                           /*  TARCE 屏蔽掩码              */

static mutex_t  yaffs_lock;                                             /*  锁                          */

/*********************************************************************************************************
** Function name:           yaffsfs_SetError
** Descriptions:            设置 errno
** input parameters:        err                 错误码
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_SetError(int err)
{
    seterrno(-err);
}
/*********************************************************************************************************
** Function name:           yaffsfs_Lock
** Descriptions:            上锁
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_Lock(void)
{
    mutex_lock(&yaffs_lock, 0);
}
/*********************************************************************************************************
** Function name:           yaffsfs_Unlock
** Descriptions:            解锁
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_Unlock(void)
{
    mutex_unlock(&yaffs_lock);
}
/*********************************************************************************************************
** Function name:           yaffsfs_CurrentTime
** Descriptions:            获得当前时间
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          当前时间
*********************************************************************************************************/
__u32 yaffsfs_CurrentTime(void)
{
    return 0;
}
/*********************************************************************************************************
** Function name:           yaffs_malloc
** Descriptions:            分配内存
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          内存
*********************************************************************************************************/
void *yaffs_malloc(size_t size)
{
    return kmalloc(size);
}
/*********************************************************************************************************
** Function name:           yaffs_free
** Descriptions:            释放内存
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffs_free(void *ptr)
{
    kfree(ptr);
}
/*********************************************************************************************************
** Function name:           yaffsfs_LocalInitialisation
** Descriptions:            本地初始化
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_LocalInitialisation(void)
{
    mutex_new(&yaffs_lock);
}
/*********************************************************************************************************
** 设备分区信息
*********************************************************************************************************/
typedef struct {
    int totalBytesPerChunk;         /* Should be >= 512, does not need to be a power of 2 */
    int nChunksPerBlock;            /* does not need to be a power of 2 */
    int spareBytesPerChunk;         /* spare area size */
    int startBlock;                 /* Start block we're allowed to use */
    int endBlock;                   /* End block we're allowed to use */
    int nReservedBlocks;            /* We want this tuneable so that we can reduce */
                                    /* reserved blocks on NOR and RAM. */
    int nShortOpCaches;             /* If <= 0, then short op caching is disabled, else
                                     * the number of short op caches (don't use too many).
                                     * 10 to 20 is a good bet.
                                     */
    int useNANDECC;                 /* Flag to decide whether or not to use NANDECC on data (yaffs1) */
    int noTagsECC;                  /* Flag to decide whether or not to do ECC on packed tags (yaffs2) */
    const char *name;
} yaffs_Info;
/*********************************************************************************************************
** Function name:           ydevice_GetInfo
** Descriptions:            获得信息
** input parameters:        ydev                yaffs 设备
** output parameters:       NONE
** Returned value:          YAFFS ERROR_CODE
*********************************************************************************************************/
int ydevice_GetInfo(yaffs_Device *ydev)
{
    device_t *dev = (device_t *)ydev->context;
    int ret;
    yaffs_Info info;

    if (dev == NULL ||
        dev->drv == NULL ||
        dev->drv->ioctl == NULL) {
        return YAFFS_FAIL;
    }

    ret = dev->drv->ioctl(dev->ctx, NULL, BLKDEV_CMD_INFO, &info);
    if (ret < 0) {
        return YAFFS_FAIL;
    }

    ydev->param.totalBytesPerChunk  = info.totalBytesPerChunk;
    ydev->param.nChunksPerBlock     = info.nChunksPerBlock;
    ydev->param.nReservedBlocks     = info.nReservedBlocks;
    ydev->param.startBlock          = info.startBlock;
    ydev->param.endBlock            = info.endBlock;
    ydev->param.useNANDECC          = info.useNANDECC;
    ydev->param.nShortOpCaches      = info.nShortOpCaches;
    ydev->param.name                = info.name;

    if (ydev->param.name == NULL) {
        return YAFFS_FAIL;
    }

    return YAFFS_OK;
}
/*********************************************************************************************************
** 读写参数
*********************************************************************************************************/
typedef struct {
    void       *data;
    void       *spare;
} yaffs_RwParam;
/*********************************************************************************************************
** Function name:           ydevice_WriteChunk
** Descriptions:            写一个 chuck
** input parameters:        ydev                yaffs 设备
**                          chunkInNAND         chuck 号
**                          data                数据
**                          spare               扩展数据
** output parameters:       NONE
** Returned value:          YAFFS ERROR_CODE
*********************************************************************************************************/
int ydevice_WriteChunk(yaffs_Device *ydev, int chunkInNAND, const __u8 *data, const yaffs_Spare *spare)
{
    device_t *dev = (device_t *)ydev->context;
    int ret;
    yaffs_RwParam param;

    if (dev == NULL ||
        dev->drv == NULL ||
        dev->drv->writeblk == NULL) {
        return YAFFS_FAIL;
    }

    param.data  = (void *)data;
    param.spare = (void *)spare;

    ret = dev->drv->writeblk(dev->ctx, NULL, chunkInNAND, 1, &param);
    if (ret < 0) {
        return YAFFS_FAIL;
    } else {
        return YAFFS_OK;
    }
}
/*********************************************************************************************************
** Function name:           ydevice_ReadChunk
** Descriptions:            读一个 chuck
** input parameters:        ydev                yaffs 设备
**                          chunkInNAND         chuck 号
**                          data                数据缓冲区
**                          spare               扩展数据缓冲区
** output parameters:       NONE
** Returned value:          YAFFS ERROR_CODE
*********************************************************************************************************/
int ydevice_ReadChunk(yaffs_Device *ydev, int chunkInNAND, __u8 *data, yaffs_Spare *spare)
{
    device_t *dev = (device_t *)ydev->context;
    int ret;
    yaffs_RwParam param;

    if (dev == NULL ||
        dev->drv == NULL ||
        dev->drv->readblk == NULL) {
        return YAFFS_FAIL;
    }

    param.data  = (void *)data;
    param.spare = (void *)spare;

    ret = dev->drv->readblk(dev->ctx, NULL, chunkInNAND, 1, &param);
    if (ret < 0) {
        return YAFFS_FAIL;
    } else {
        return YAFFS_OK;
    }
}
/*********************************************************************************************************
** Function name:           ydevice_EraseBlock
** Descriptions:            擦取一个 chuck
** input parameters:        ydev                yaffs 设备
**                          blockNumber         块号
** output parameters:       NONE
** Returned value:          YAFFS ERROR_CODE
*********************************************************************************************************/
int ydevice_EraseBlock(yaffs_Device *ydev, int blockNumber)
{
    device_t *dev = (device_t *)ydev->context;
    int ret;

    if (dev == NULL ||
        dev->drv == NULL ||
        dev->drv->ioctl == NULL) {
        return YAFFS_FAIL;
    }

    ret = dev->drv->ioctl(dev->ctx, NULL, BLKDEV_CMD_ERASE, (void *)&blockNumber);
    if (ret < 0) {
        return YAFFS_FAIL;
    } else {
        return YAFFS_OK;
    }
}
/*********************************************************************************************************
** Function name:           ydevice_Initialise
** Descriptions:            初始化 yaffs 设备
** input parameters:        ydev                yaffs 设备
** output parameters:       NONE
** Returned value:          YAFFS ERROR_CODE
*********************************************************************************************************/
int ydevice_Initialise(yaffs_Device *ydev)
{
    device_t *dev = (device_t *)ydev->context;
    int ret;

    if (dev == NULL ||
        dev->drv == NULL ||
        dev->drv->ioctl == NULL) {
        return YAFFS_FAIL;
    }

    ret = dev->drv->ioctl(dev->ctx, NULL, BLKDEV_CMD_INIT, NULL);
    if (ret < 0) {
        return YAFFS_FAIL;
    } else {
        return YAFFS_OK;
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
