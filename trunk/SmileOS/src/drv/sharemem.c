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
** File name:               sharemem.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            共享内存设备驱动
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
    void       *mem;
    size_t      size;
} privinfo_t;

/*
 * 打开 sharemem
 */
static int sharemem_open(void *ctx, file_t *file, int oflag, mode_t mode)
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
 * 关闭 sharemem
 */
static int sharemem_close(void *ctx, file_t *file)
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
 * 获得 sharemem 状态
 */
static int sharemem_fstat(void *ctx, file_t *file, struct stat *buf)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    buf->st_size = priv->size;

    return 0;
}

/*
 * 删除 sharemem
 */
static int sharemem_unlink(void *ctx)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    kfree(priv->mem);

    kfree(priv);

    return 0;
}

/*
 * sharemem 驱动
 */
static driver_t sharemem_drv = {
        .name     = "sharemem",
        .open     = sharemem_open,
        .close    = sharemem_close,
        .fstat    = sharemem_fstat,
        .unlink   = sharemem_unlink,
};
/*********************************************************************************************************
** Function name:           sharemem_init
** Descriptions:            初始化共享内存驱动
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sharemem_init(void)
{
    return driver_install(&sharemem_drv);
}
/*********************************************************************************************************
** Function name:           sharemem_create
** Descriptions:            创建共享内存设备
** input parameters:        path                共享内存设备路径
**                          size                共享内存大小
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sharemem_create(const char *path, size_t size)
{
    privinfo_t *priv;

    if (path == NULL || size == 0) {
        seterrno(EINVAL);
        return -1;
    }

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);

        priv->size = size;

        priv->mem  = kmalloc(size, GFP_SHARE);
        if (priv->mem == NULL) {
            kfree(priv);
            seterrno(ENOMEM);
            return -1;
        }

        if (device_create(path, "sharemem", priv) < 0) {
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
