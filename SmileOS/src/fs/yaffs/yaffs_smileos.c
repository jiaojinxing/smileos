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
** Descriptions:            yaffs �ļ�ϵͳϵͳģ���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-27
** Version:                 1.0.0
** Descriptions:            �����ļ�
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

unsigned int    yaffs_traceMask = 0xF0000000;                           /*  TARCE ��������              */

static mutex_t  yaffs_lock;                                             /*  ��                          */

/*********************************************************************************************************
** Function name:           yaffsfs_SetError
** Descriptions:            ���� errno
** input parameters:        err                 ������
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_SetError(int err)
{
    seterrno(-err);
}
/*********************************************************************************************************
** Function name:           yaffsfs_Lock
** Descriptions:            ����
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
** Descriptions:            ����
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
** Descriptions:            ��õ�ǰʱ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ��ǰʱ��
*********************************************************************************************************/
__u32 yaffsfs_CurrentTime(void)
{
    return 0;
}
/*********************************************************************************************************
** Function name:           yaffs_malloc
** Descriptions:            �����ڴ�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          �ڴ�
*********************************************************************************************************/
void *yaffs_malloc(size_t size)
{
    return kmalloc(size);
}
/*********************************************************************************************************
** Function name:           yaffs_free
** Descriptions:            �ͷ��ڴ�
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
** Descriptions:            ���س�ʼ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_LocalInitialisation(void)
{
    mutex_new(&yaffs_lock);
}
/*********************************************************************************************************
** �豸������Ϣ
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
} yaffs_Info;
/*********************************************************************************************************
** Function name:           ydevice_GetInfo
** Descriptions:            �����Ϣ
** input parameters:        ydev                yaffs �豸
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
    } else {
        return YAFFS_OK;
    }

    ydev->param.totalBytesPerChunk  = info.totalBytesPerChunk;
    ydev->param.nChunksPerBlock     = info.nChunksPerBlock;
    ydev->param.nReservedBlocks     = info.nReservedBlocks;
    ydev->param.startBlock          = info.startBlock;
    ydev->param.endBlock            = info.endBlock;
    ydev->param.useNANDECC          = info.useNANDECC;
    ydev->param.nShortOpCaches      = info.nShortOpCaches;

    return YAFFS_OK;
}
/*********************************************************************************************************
** ��д����
*********************************************************************************************************/
typedef struct {
    void       *data;
    void       *spare;
} yaffs_RwParam;
/*********************************************************************************************************
** Function name:           ydevice_WriteChunk
** Descriptions:            дһ�� chuck
** input parameters:        ydev                yaffs �豸
**                          chunkInNAND         chuck ��
**                          data                ����
**                          spare               ��չ����
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
** Descriptions:            ��һ�� chuck
** input parameters:        ydev                yaffs �豸
**                          chunkInNAND         chuck ��
**                          data                ���ݻ�����
**                          spare               ��չ���ݻ�����
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
** Descriptions:            ��ȡһ�� chuck
** input parameters:        ydev                yaffs �豸
**                          blockNumber         ���
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
** Descriptions:            ��ʼ�� yaffs �豸
** input parameters:        ydev                yaffs �豸
** output parameters:       NONE
** Returned value:          YAFFS ERROR_CODE
*********************************************************************************************************/
int ydevice_Initialise(yaffs_Device *ydev)
{
    device_t *dev = (device_t *)ydev;
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
