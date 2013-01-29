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
** Descriptions:            �ڴ����������豸
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-11-27
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/utils.h"
#include <string.h>

#define DISK_SZ     (1440 * KB)                                         /*  ���̴�С                    */
#define SECT_SZ     (512)                                               /*  ������С                    */
#define SECT_NR     (DISK_SZ / SECT_SZ)                                 /*  ������Ŀ                    */

/*
 * ˽����Ϣ
 */
typedef struct {
    VFS_DEVICE_MEMBERS;
    unsigned char   buf[DISK_SZ];
} privinfo_t;

/*
 * �� ramdisk
 */
static int ramdisk_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_inc_return(dev_ref(file)) == 1) {
        return 0;
    } else {
        /*
         * ����豸������ͬʱ�򿪶��, ��ʹ�����´���:
         */
        atomic_dec(dev_ref(file));
        seterrno(EBUSY);
        return -1;
    }
}

/*
 * �ر� ramdisk
 */
static int ramdisk_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    atomic_dec(dev_ref(file));
    return 0;
}

/*
 * ���� ramdisk
 */
static int ramdisk_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;
    int *val = va_to_mva(arg);

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    switch (cmd) {
    case BLKDEV_CMD_STATUS:
        *val = 0;
        break;

    case BLKDEV_CMD_INIT:
        break;

    case BLKDEV_CMD_SYNC:
        break;

    case BLKDEV_CMD_SECT_NR:
        *val = SECT_NR;
        break;

    case BLKDEV_CMD_SECT_SZ:
        *val = SECT_SZ;
        break;

    case BLKDEV_CMD_BLK_SZ:
        *val = SECT_SZ;
        break;

    case BLKDEV_CMD_ERASE:
        memset(priv->buf + val[0] * SECT_SZ, 0, (val[1] - val[0] + 1) * SECT_SZ);
        break;

    default:
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * �� ramdisk ��
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
 * д ramdisk ��
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
 * ramdisk ����
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
** Descriptions:            ��ʼ�� ramdisk
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
        seterrno(0);
        return 0;
    } else {
        seterrno(ENOMEM);
        return -1;
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
