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
** Descriptions:            �����ڴ��������豸
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/utils.h"
#include <fcntl.h>

/*
 * ˽����Ϣ
 */
typedef struct {
    VFS_DEVICE_MEMBERS;
    void       *mem;
    size_t      size;
} privinfo_t;

/*
 * �� sharemem
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
 * �ر� sharemem
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
 * ��� sharemem ״̬
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
 * ɾ�� sharemem
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
 * sharemem ����
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
** Descriptions:            ��ʼ�������ڴ�
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
** Descriptions:            ���������ڴ�
** input parameters:        path                �����ڴ��豸·��
**                          size                �����ڴ��С
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
