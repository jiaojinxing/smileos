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
** File name:               xxxdrv.c
** Last modified Date:      2012-4-6
** Last Version:            1.0.0
** Descriptions:            �豸����ģ��
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-6
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "kern/kern.h"
#include <errno.h>
#include <fcntl.h>

/*
 * ˽����Ϣ
 */
typedef struct {
    VFS_SELECT_MEMBERS;
    int             mode;
} privinfo_t;

/*
 * �� xxx
 */
static int xxx_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (oflag & O_NONBLOCK) {
        priv->mode |= O_NONBLOCK;
    }
    return 0;
}

/*
 * ���� xxx
 */
static int xxx_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }
    return 0;
}

/*
 * �ر� xxx
 */
static int xxx_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * xxx �ǲ���һ�� tty
 */
static int xxx_isatty(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * �� xxx
 */
static ssize_t xxx_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    /*
     * ���û�����ݿɶ�
     */
    if (0) {
        if (priv->mode & O_NONBLOCK) {
            seterrno(EAGAIN);
            return 0;
        } else {
            /*
             * TODO: ��������
             */
            {

            }

            /*
             * ����ȴ������
             */
            if (0) {
                seterrno(EINTR);
                return -1;
            }
        }
    }

    {
        /*
         * ��ɶ�����
         */
        return 0;
    }
}

/*
 * д xxx
 */
static ssize_t xxx_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    /*
     * ���û�пռ��д
     */
    if (0) {
        if (priv->mode & O_NONBLOCK) {
            seterrno(EAGAIN);
            return 0;
        } else {
            /*
             * TODO: ��������
             */
            {

            }

            /*
             * ����ȴ������
             */
            if (0) {
                seterrno(EINTR);
                return -1;
            }
        }
    }

    {
        /*
         * ���д����
         */
        return 0;
    }
}

/*
 * ɨ��
 */
static int xxx_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
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

/*
 * ����
 */
static int xxx_fcntl(void *ctx, file_t *file, int cmd, int arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    switch (cmd) {
    case F_GETFL:
        return priv->mode;

    case F_SETFL:
        priv->mode = arg;
        return 0;

    default:
        seterrno(EINVAL);
        return -1;
    }
}

#include "drv/selectdrv.h"

/*
 * xxx ����
 */
static driver_t xxx_drv = {
        .name     = "xxx",
        .open     = xxx_open,
        .write    = xxx_write,
        .read     = xxx_read,
        .isatty   = xxx_isatty,
        .ioctl    = xxx_ioctl,
        .close    = xxx_close,
        .fcntl    = xxx_fcntl,
        .scan     = xxx_scan,
        .select   = select_select,
        .unselect = select_unselect,
};

/*
 * ��ʼ�� xxx
 */
int xxx_init(void)
{
    privinfo_t *priv;

    driver_install(&xxx_drv);

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        priv->mode = O_NONBLOCK;
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
** END FILE
*********************************************************************************************************/
