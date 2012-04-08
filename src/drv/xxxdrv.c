/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <JiaoJinXing1987@gmail.com>
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
** File name:               xxxdrv.c
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
#include "kern/kern.h"

/*
 * 私有信息
 */
typedef struct {
    VFS_SELECT_MEMBERS
} privinfo_t;

/*
 * 打开 xxx
 */
static int xxx_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 0;
}

/*
 * 控制 xxx
 */
static int xxx_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 0;
}

/*
 * 关闭 xxx
 */
static int xxx_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 0;
}

/*
 * xxx 是不是一个 TTY
 */
static int xxx_isatty(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 1;
}

/*
 * 读 xxx
 */
static ssize_t xxx_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 0;
}

/*
 * 写 xxx
 */
static ssize_t xxx_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 0;
}

/*
 * 扫描
 */
static int xxx_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        return -1;
    }

    ret = 0;
    if (priv->flags & flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if (priv->flags & flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    if (priv->flags & flags & VFS_FILE_ERROR) {
        ret |= VFS_FILE_ERROR;
    }
    return ret;
}

#include "selectdrv.h"

/*
 * xxx 驱动
 */
static driver_t xxx_drv = {
        .name     = "xxx",
        .open     = xxx_open,
        .write    = xxx_write,
        .read     = xxx_read,
        .isatty   = xxx_isatty,
        .ioctl    = xxx_ioctl,
        .close    = xxx_close,
        .scan     = xxx_scan,
        .select   = select_select,
        .unselect = select_unselect,
};

/*
 * 初始化 xxx
 */
int xxx_init(void)
{
    privinfo_t *priv;

    driver_install(&xxx_drv);

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        select_init(priv);
        if (device_create("/dev/xxx", "xxx", priv) < 0) {
            kfree(priv);
            return -1;
        }
        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
