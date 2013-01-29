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
** File name:               zero.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            ZERO 驱动和设备
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
#include <string.h>

/*
 * 私有信息
 */
typedef struct {
    VFS_DEVICE_MEMBERS;
} privinfo_t;

/*
 * 打开 zero
 */
static int zero_open(void *ctx, file_t *file, int oflag, mode_t mode)
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
 * 关闭 zero
 */
static int zero_close(void *ctx, file_t *file)
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
 * 读 zero
 */
static ssize_t zero_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    memset(buf, 0, len);

    return len;
}

/*
 * 扫描
 */
static int zero_scan(void *ctx, file_t *file, int flags)
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
    return ret;
}

/*
 * zero 驱动
 */
static driver_t zero_drv = {
        .name     = "zero",
        .open     = zero_open,
        .read     = zero_read,
        .close    = zero_close,
        .scan     = zero_scan,
        .select   = select_select,
        .unselect = select_unselect,
};
/*********************************************************************************************************
** Function name:           zero_init
** Descriptions:            初始化 ZERO
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int zero_init(void)
{
    privinfo_t *priv;

    driver_install(&zero_drv);

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);
        if (device_create("/dev/zero", "zero", priv) < 0) {
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
