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
** File name:               null.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            NULL 设备驱动
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
#include <fcntl.h>

/*
 * 私有信息
 */
typedef struct {
    VFS_DEVICE_MEMBERS;
} privinfo_t;

/*
 * 打开 null
 */
static int null_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    atomic_inc(dev_ref(file));
    return 0;
}

/*
 * 关闭 null
 */
static int null_close(void *ctx, file_t *file)
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
 * 读 null
 */
static ssize_t null_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (!(file->flags & O_NONBLOCK)) {
        /*
         * TODO: 阻塞任务
         */
        seterrno(EAGAIN);
        return 0;
    } else {
        seterrno(EAGAIN);
        return 0;
    }
}

/*
 * 写 null
 */
static ssize_t null_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    return len;
}

/*
 * 扫描 null
 */
static int null_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if (flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if (flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    return ret;
}

/*
 * null 驱动
 */
static driver_t null_drv = {
        .name     = "null",
        .open     = null_open,
        .write    = null_write,
        .read     = null_read,
        .close    = null_close,
        .scan     = null_scan,
        .select   = select_select,
        .unselect = select_unselect,
};
/*********************************************************************************************************
** Function name:           null_init
** Descriptions:            初始化 NULL 设备
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int null_init(void)
{
    privinfo_t *priv;

    driver_install(&null_drv);

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);
        if (device_create("/dev/null", "null", priv) < 0) {
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
