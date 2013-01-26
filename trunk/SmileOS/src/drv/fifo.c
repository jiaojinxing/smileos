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
** Descriptions:            设备驱动模板
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/utils.h"
#include "kern/kern.h"
#include <errno.h>
#include <string.h>

/*
 * 私有信息
 */
typedef struct {
    VFS_DEVICE_MEMBERS;
    unsigned char  *buffer;
    unsigned int    size;
    unsigned int    in;
    unsigned int    out;
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

    val = atomic_inc_return(&(((device_t *)file->ctx)->ref));
    if (val == 1) {
        /*
         * 第一次打开时的初始化代码
         */

        priv->in  = 0;
        priv->out = 0;
        priv->buffer = kmalloc(8 * KB, GFP_KERNEL);
        if (priv->buffer == NULL) {
            atomic_dec(&(((device_t *)file->ctx)->ref));
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
        atomic_dec(&(((device_t *)file->ctx)->ref));
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
    if (atomic_read(&(((device_t *)file->ctx)->ref)) == 1) {
        /*
         * 加上最后一次关闭时的清理代码
         */
        kfree(priv->buffer);
        priv->buffer = NULL;
    }
    atomic_dec(&(((device_t *)file->ctx)->ref));
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

    /*
     * 如果没有数据可读
     */
    if (priv->in == priv->out) {
        ret = select_helper(&priv->select, fifo_scan, ctx, file, VFS_FILE_READABLE);
        if (ret <= 0) {
            return ret;
        } else {
            goto __again;
        }
    }

    {
        /*
         * 完成读操作
         */
        unsigned int l;

        len = min(len, priv->in - priv->out);

        /*
         * first get the data from fifo->out until the end of the buffer
         */
        l = min(len, priv->size - (priv->out & (priv->size - 1)));

        memcpy(buf, priv->buffer + (priv->out & (priv->size - 1)), l);

        /*
         * then get the rest (if any) from the beginning of the buffer
         */
        memcpy((char *)buf + l, priv->buffer, len - l);

        priv->out += len;

        select_report(&priv->select, VFS_FILE_WRITEABLE);

        return len;
    }
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

    /*
     * 如果没有空间可写
     */
    if (priv->size - priv->in + priv->out == 0) {
        ret = select_helper(&priv->select, fifo_scan, ctx, file, VFS_FILE_WRITEABLE);
        if (ret <= 0) {
            return ret;
        } else {
            goto __again;
        }
    }

    {
        /*
         * 完成写操作
         */
        unsigned int l;

        len = min(len, priv->size - priv->in + priv->out);

        /*
         * first put the data starting from priv->in to buffer end
         */
        l = min(len, priv->size - (priv->in & (priv->size - 1)));

        memcpy(priv->buffer + (priv->in & (priv->size - 1)), buf, l);

        /*
         * then put the rest (if any) at the beginning of the buffer
         */
        memcpy(priv->buffer, (const char *)buf + l, len - l);

        priv->in += len;

        select_report(&priv->select, VFS_FILE_READABLE);

        return len;
    }
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
    if ((priv->in - priv->out > 0) && flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if ((priv->size - priv->in + priv->out > 0) && flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    if (atomic_read(&priv->select.flags) && flags & VFS_FILE_ERROR) {
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
** input parameters:        NONE
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
