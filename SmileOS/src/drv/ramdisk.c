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
** File name:               ramdisk.c
** Last modified Date:      2012-11-27
** Last Version:            1.0.0
** Descriptions:            内存盘驱动和设备
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-11-27
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/utils.h"
#include "kern/kern.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define DISK_SZ     (1440 * KB)                                         /*  磁盘大小                    */
#define SECT_SZ     (512)                                               /*  扇区大小                    */
#define SECT_NR     (DISK_SZ / SECT_SZ)                                 /*  扇区数目                    */

/*
 * 私有信息
 */
typedef struct {
    VFS_DEVICE_MEMBERS;
    uint8_t         buf[DISK_SZ];
} privinfo_t;

/*
 * 打开 ramdisk
 */
static int ramdisk_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_inc_return(&(((device_t *)file->ctx)->ref)) == 1) {
        /*
         * 第一次打开时的初始化代码
         */
        return 0;
    } else {
        /*
         * 如果设备不允许同时打开多次, 请使用如下代码:
         */
        atomic_dec(&(((device_t *)file->ctx)->ref));
        seterrno(EBUSY);
        return -1;
    }
}

/*
 * 关闭 ramdisk
 */
static int ramdisk_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&(((device_t *)file->ctx)->ref)) == 1) {
        /*
         * 加上最后一次关闭时的清理代码
         */
    }
    atomic_dec(&(((device_t *)file->ctx)->ref));
    return 0;
}

/*
 * 控制 ramdisk
 */
static int ramdisk_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    arg = va_to_mva(arg);

    switch (cmd) {
    case BLKDEV_CMD_STATUS:
        *(uint32_t *)arg = 0;
        break;

    case BLKDEV_CMD_INIT:
        printk("ramdisk: init\n");
        break;

    case BLKDEV_CMD_SYNC:
        printk("ramdisk: sync\n");
        break;

    case BLKDEV_CMD_SECT_NR:
        *(uint32_t *)arg = SECT_NR;
        break;

    case BLKDEV_CMD_SECT_SZ:
        *(uint32_t *)arg = SECT_SZ;
        break;

    case BLKDEV_CMD_BLK_SZ:
        *(uint32_t *)arg = SECT_SZ;
        break;

    case BLKDEV_CMD_ERASE:
        memset(priv->buf + *(uint32_t *)arg * SECT_SZ, 0,
                (*((uint32_t *)arg + 1) - *(uint32_t *)arg + 1)* SECT_SZ);
        break;

    default:
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * 读 ramdisk 块
 */
static ssize_t ramdisk_readblk(void *ctx, file_t *file, size_t blk_no, size_t blk_cnt, void *buf)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }
    memcpy(buf, priv->buf + blk_no * SECT_SZ, blk_cnt * SECT_SZ);
    return blk_cnt * SECT_SZ;
}

/*
 * 写 ramdisk 块
 */
static ssize_t ramdisk_writeblk(void *ctx, file_t *file, size_t blk_no, size_t blk_cnt, const void *buf)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }
    memcpy(priv->buf + blk_no * SECT_SZ, buf, blk_cnt * SECT_SZ);
    return blk_cnt * SECT_SZ;
}

/*
 * ramdisk 驱动
 */
static driver_t ramdisk_drv = {
        .name     = "ramdisk",
        .open     = ramdisk_open,
        .ioctl    = ramdisk_ioctl,
        .close    = ramdisk_close,
        .readblk  = ramdisk_readblk,
        .writeblk = ramdisk_writeblk,
};
/*********************************************************************************************************
** Function name:           ramdisk_init
** Descriptions:            初始化 ramdisk
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int ramdisk_init(void)
{
    privinfo_t *priv;

    driver_install(&ramdisk_drv);

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);
        if (device_create("/dev/ramdisk", "ramdisk", priv) < 0) {
            kfree(priv);
            return -1;
        }
        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
