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
** File name:               mtdblock.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            MTD 块驱动和设备
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/utils.h"
#include <sys/stat.h>
/*********************************************************************************************************
** 私有信息
*********************************************************************************************************/
typedef struct {
    VFS_DEVICE_MEMBERS;
    int startBlock;                                     /* Start block we're allowed to use             */
    int endBlock;                                       /* End block we're allowed to use               */
    int nReservedBlocks;                                /* We want this tuneable so that we can reduce  */
} privinfo_t;
/*********************************************************************************************************
** Function name:           mtdblock_open
** Descriptions:            打开 mtdblock
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          oflag               打开标志
**                          mode                模式
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int mtdblock_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    atomic_inc(dev_ref(file));
    return 0;
}
/*********************************************************************************************************
** Function name:           mtdblock_close
** Descriptions:            关闭 mtdblock
** input parameters:        ctx                 私有信息
**                          file                文件结构
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int mtdblock_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    atomic_dec(dev_ref(file));
    return 0;
}
/*********************************************************************************************************
** Function name:           mtdblock_fstat
** Descriptions:            获得 mtdblock 状态
** input parameters:        ctx                 私有信息
**                          file                文件结构
** output parameters:       buf                 状态结构
** Returned value:          0 OR -1
*********************************************************************************************************/
static int mtdblock_fstat(void *ctx, file_t *file, struct stat *buf)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    buf->st_blocks    = priv->endBlock - priv->startBlock + 1;
    buf->st_spare4[0] = priv->nReservedBlocks;
    buf->st_spare4[1] = priv->startBlock;

    return 0;
}
/*********************************************************************************************************
** mtdblock 驱动
*********************************************************************************************************/
static driver_t mtdblock_drv = {
        .name     = "mtdblock",
        .open     = mtdblock_open,
        .close    = mtdblock_close,
        .fstat    = mtdblock_fstat,
};
/*********************************************************************************************************
** Function name:           mtdblock_init
** Descriptions:            初始化 MTD 块
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mtdblock_init(void)
{
    privinfo_t *priv;

    driver_install(&mtdblock_drv);

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);
        priv->nReservedBlocks = 2;
        priv->startBlock      = 64;
        priv->endBlock        = 255;
        if (device_create("/dev/mtdblock0", "mtdblock", priv) < 0) {
            kfree(priv);
            return -1;
        }
    } else {
        seterrno(ENOMEM);
        return -1;
    }

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);
        priv->nReservedBlocks = 5;
        priv->startBlock      = 256;
        priv->endBlock        = 4095;
        if (device_create("/dev/mtdblock1", "mtdblock", priv) < 0) {
            kfree(priv);
            return -1;
        }
    } else {
        seterrno(ENOMEM);
        return -1;
    }

    seterrno(0);
    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
