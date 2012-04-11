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
** File name:               pty.c
** Last modified Date:      2012-4-11
** Last Version:            1.0.0
** Descriptions:            PTY 设备驱动
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-11
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
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>
#include "tty.h"

/*
 * 私有信息
 */
typedef struct {
    VFS_SELECT_MEMBERS
    struct tty      tty;
    int             fd;
} privinfo_t;

/*
 * 打开 pty
 */
static int pty_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * 控制 pty
 */
static int pty_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = tty_ioctl(&priv->tty, cmd, arg);
    if (ENOSYS == ret) {
        return ioctl(priv->fd, cmd, arg);
    } else if (ret != 0) {
        seterrno(ret);
        return -1;
    }
    return 0;
}

/*
 * 关闭 pty
 */
static int pty_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * pty 是不是一个 tty
 */
static int pty_isatty(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 1;
}

/*
 * 读 pty
 */
static ssize_t pty_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
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
 * 写 pty
 */
static ssize_t pty_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
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
 * 扫描 pty
 */
static int pty_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if (tty_readable(&priv->tty) & flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if (flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }

    return ret;
}

#include "selectdrv.h"

/*
 * pty 驱动
 */
static driver_t pty_drv = {
        .name     = "pty",
        .open     = pty_open,
        .write    = pty_write,
        .read     = pty_read,
        .isatty   = pty_isatty,
        .ioctl    = pty_ioctl,
        .close    = pty_close,
        .scan     = pty_scan,
        .select   = select_select,
        .unselect = select_unselect,
};

/*
 * 初始化 pty
 */
int pty_init(void)
{
    return driver_install(&pty_drv);;
}

/*
 * 启动 pty 输出
 */
static void pty_start(struct tty *tp)
{
    int c;
    privinfo_t *priv = struct_addr(tp, privinfo_t, tty);
    char buf[INPUT_MAX];
    int i;

    i = 0;
    while ((c = tty_getc(&tp->t_outq)) >= 0) {
        buf[i++] = (char)c;
    }

    write(priv->fd, buf, i);
}

#include <sys/socket.h>

/*
 * pty 线程
 */
void pty_thread(void *arg)
{
    privinfo_t *priv = arg;
    int on = 1;
    char c;
    fd_set rfds, efds;
    int ret;

    printk("%s: socket_attach\n", __func__);

    priv->fd = socket_attach(priv->fd, FALSE);
    if (priv->fd < 0) {
        return;
    }

    printk("%s: ioctl\n", __func__);

    ioctl(priv->fd, FIONBIO, &on);

    while (1) {
        FD_ZERO(&rfds);
        FD_ZERO(&efds);
        FD_SET(priv->fd, &rfds);
        FD_SET(priv->fd, &efds);

        printk("%s: select\n", __func__);

        ret = select(priv->fd + 1, &rfds, NULL, &efds, NULL);

        printk("%s: select ok\n", __func__);
        if (ret < 0) {
            printk("%s: ret < 0\n", __func__);
            select_report(priv, VFS_FILE_ERROR);
            break;
        } else if (ret == 0) {
            printk("%s: ret == 0\n", __func__);
            continue;
        } else {
            printk("%s: select_report ret == 0\n", __func__);
            if (FD_ISSET(priv->fd, &efds)) {
                printk("%s: FD_ISSET(priv->fd, &efds)\n", __func__);
                select_report(priv, VFS_FILE_ERROR);
                break;
            } else if (FD_ISSET(priv->fd, &rfds)) {
                printk("%s: FD_ISSET(priv->fd, &rfds)\n", __func__);
                while (read(priv->fd, &c, 1) == 1) {
                    tty_input((int)c, &priv->tty);
                }
                select_report(priv, VFS_FILE_READABLE);
            }
        }
    }
    close(priv->fd);
}

/*
 * 初始化 pty
 */
int pty_create(const char *name, int fd)
{
    privinfo_t *priv;
    uint32_t reg;

    reg = interrupt_disable();

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        select_init(priv);
        priv->fd = fd;

        if (device_create(name, "pty", priv) < 0) {
            kfree(priv);
            seterrno(ENOMEM);
            interrupt_resume(reg);
            return -1;
        }

        tty_attach(&priv->tty);
        priv->tty.t_oproc = pty_start;

        if (kthread_create(name, pty_thread, priv, 8 * KB, 20) < 0) {
            device_remove(name);
            kfree(priv);
            interrupt_resume(reg);
            return -1;
        }

        seterrno(0);
        interrupt_resume(reg);
        return 0;
    } else {
        seterrno(ENOMEM);
        interrupt_resume(reg);
        return -1;
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
