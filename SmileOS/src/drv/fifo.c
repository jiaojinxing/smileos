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
** File name:               fifo.c
** Last modified Date:      2012-4-6
** Last Version:            1.0.0
** Descriptions:            FIFO 设备驱动
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-6
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
#include "kern/kfifo.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/utils.h"

/*
 * 私有信息
 */
typedef struct {
    VFS_DEVICE_MEMBERS;
    kfifo_t     fifo;
} privinfo_t;

static int fifo_scan(void *ctx, file_t *file, int flags);

/*
 * 打开 fifo
 */
static int fifo_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;
    int val;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    val = atomic_inc_return(dev_ref(file));
    if (val == 1) {
        /*
         * 第一次打开时的初始化代码
         */
        if (kfifo_init(&priv->fifo, 1 * KB) < 0) {
            atomic_dec(dev_ref(file));
            seterrno(ENOMEM);
            return -1;
        }
        return 0;
    } else if (val == 2) {
        return 0;
    } else {
        /*
         * 如果设备不允许同时打开多次, 请使用如下代码:
         */
        atomic_dec(dev_ref(file));
        seterrno(EBUSY);
        return -1;
    }
}

/*
 * 关闭 fifo
 */
static int fifo_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_read(dev_ref(file)) == 1) {
        /*
         * 加上最后一次关闭时的清理代码
         */
        kfifo_free(&priv->fifo);
    }
    atomic_dec(dev_ref(file));
    return 0;
}

/*
 * 读 fifo
 */
static ssize_t fifo_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    __again:
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    if (kfifo_is_empty(&priv->fifo)) {                                  /*  如果没有数据可读            */
        ret = select_helper(&priv->select, fifo_scan, ctx, file, VFS_FILE_READABLE);
        if (ret <= 0) {
            return ret;
        } else {
            goto __again;
        }
    }

    len = kfifo_get(&priv->fifo, buf, len);

    select_report(&priv->select, VFS_FILE_WRITEABLE);

    return len;
}

/*
 * 写 fifo
 */
static ssize_t fifo_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    __again:
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    if (kfifo_is_full(&priv->fifo)) {                                   /*  如果没有空间可写            */
        ret = select_helper(&priv->select, fifo_scan, ctx, file, VFS_FILE_WRITEABLE);
        if (ret <= 0) {
            return ret;
        } else {
            goto __again;
        }
    }

    len = kfifo_put(&priv->fifo, buf, len);

    select_report(&priv->select, VFS_FILE_READABLE);

    return len;
}

/*
 * 扫描 fifo
 */
static int fifo_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if ((!kfifo_is_empty(&priv->fifo)) && flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if ((!kfifo_is_full(&priv->fifo)) && flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    if (atomic_read(&priv->select.flags) & flags & VFS_FILE_ERROR) {
        ret |= VFS_FILE_ERROR;
    }
    return ret;
}

/*
 * fifo 驱动
 */
static driver_t fifo_drv = {
        .name     = "fifo",
        .open     = fifo_open,
        .write    = fifo_write,
        .read     = fifo_read,
        .close    = fifo_close,
        .scan     = fifo_scan,
        .select   = select_select,
        .unselect = select_unselect,
};
/*********************************************************************************************************
** Function name:           fifo_init
** Descriptions:            初始化 fifo
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int fifo_init(void)
{
    return driver_install(&fifo_drv);
}
/*********************************************************************************************************
** Function name:           fifo_create
** Descriptions:            创建 fifo
** input parameters:        name                fifo 设备路径
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int fifo_create(const char *name)
{
    privinfo_t *priv;

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);
        if (device_create(name, "fifo", priv) < 0) {
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
