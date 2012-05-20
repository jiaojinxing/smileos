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
** File name:               ttyS0.c
** Last modified Date:      2012-3-31
** Last Version:            1.0.0
** Descriptions:            TTYS0 驱动和设备
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-31
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
#include <errno.h>
#include "drv/tty.h"
#include <fcntl.h>

/*
 * 私有信息
 */
typedef struct {
    VFS_SELECT_MEMBERS
    struct tty      tty;
    int             mode;
} privinfo_t;

/*
 * 打开 ttyS0
 */
static int ttyS0_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * 控制 ttyS0
 */
static int ttyS0_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    ret = tty_ioctl(&priv->tty, cmd, arg);
    if (ret != 0) {
        seterrno(ret);
        return -1;
    }
    return 0;
}

/*
 * 关闭 ttyS0
 */
static int ttyS0_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * ttyS0 是一个 tty
 */
static int ttyS0_isatty(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 1;
}

/*
 * 读 ttyS0
 */
static ssize_t ttyS0_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    ret = tty_read(&priv->tty, buf, &len);
    if (ret != 0) {
        seterrno(ret);
        return -1;
    } else {
        return len;
    }
}

/*
 * 写 ttyS0
 */
static ssize_t ttyS0_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    ret = tty_write(&priv->tty, buf, &len);
    if (ret != 0) {
        seterrno(ret);
        return -1;
    } else {
        return len;
    }
}

/*
 * 启动输出
 */
static void ttyS0_start(struct tty *tp)
{
    int c;

    while ((c = tty_getc(&tp->t_outq)) >= 0) {
        kputc(c);
    }
}

/*
 * 扫描
 */
static int ttyS0_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if (tty_readable(&priv->tty) && (flags & VFS_FILE_READABLE)) {
        ret |= VFS_FILE_READABLE;
    }
    if (flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    return ret;
}

static int ttyS0_fcntl(void *ctx, file_t *file, int cmd, int arg)
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

#include "selectdrv.h"

/*
 * ttyS0 驱动
 */
static driver_t ttyS0_drv = {
        .name     = "ttyS0",
        .open     = ttyS0_open,
        .write    = ttyS0_write,
        .read     = ttyS0_read,
        .isatty   = ttyS0_isatty,
        .ioctl    = ttyS0_ioctl,
        .close    = ttyS0_close,
        .scan     = ttyS0_scan,
        .fcntl    = ttyS0_fcntl,
        .select   = select_select,
        .unselect = select_unselect,
};

/*
 * 初始化 ttyS0
 */
int ttyS0_init(void)
{
    privinfo_t *priv;

    driver_install(&ttyS0_drv);

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        select_init(priv);
        priv->mode = O_NONBLOCK;
        if (device_create("/dev/ttyS0", "ttyS0", priv) < 0) {
            kfree(priv);
            return -1;
        }
        tty_attach(&priv->tty);
        priv->tty.t_oproc = ttyS0_start;
        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
