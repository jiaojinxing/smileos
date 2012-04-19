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
#include <string.h>
#include <sys/select.h>
#include "drv/tty.h"

/*
 * 私有信息
 */
typedef struct {
    VFS_SELECT_MEMBERS
    struct tty      tty;
    int             fd;
    char            name[PATH_MAX];
    int             ref;
    int  (*attach)(void *arg);
    void (*fail)(void *arg);
    void           *arg;
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
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
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
 * pty 是一个 tty
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
    if (tty_readable(&priv->tty) && (flags & VFS_FILE_READABLE)) {
        ret |= VFS_FILE_READABLE;
    }
    if (tty_writeable(&priv->tty) && (flags & VFS_FILE_WRITEABLE)) {
        ret |= VFS_FILE_WRITEABLE;
    }
    if (priv->flags & flags & VFS_FILE_ERROR) {
        ret |= VFS_FILE_ERROR;
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
    privinfo_t *priv = struct_addr(tp, privinfo_t, tty);

    select_report(priv, VFS_FILE_WRITEABLE);
}

#include <sys/socket.h>

/*
 * pty 线程
 */
static void pty_thread(void *arg)
{
    privinfo_t *priv = arg;
    int on = 1;
    fd_set rfds, wfds, efds;
    int ret;
    char buf[TTYQ_SIZE];
    int i;
    int host_fd;
    int dev_fd;

    dev_fd = priv->attach(priv->arg);
    if (dev_fd < 0) {
        priv->fail(priv->arg);
        goto __exit0;
    }

    host_fd = open(priv->name, O_RDWR, 0666);
    if (host_fd < 0) {
        goto __exit1;
    }

    ioctl(dev_fd, FIONBIO, &on);

    while (1) {
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_ZERO(&efds);

        FD_SET(dev_fd,  &rfds);
        FD_SET(dev_fd,  &efds);

        FD_SET(host_fd, &wfds);
        FD_SET(host_fd, &efds);

        ret = select(host_fd + 1, &rfds, &wfds, &efds, NULL);
        if (ret < 0) {
            select_report(priv, VFS_FILE_ERROR);
            break;
        } else if (ret == 0) {
            continue;
        } else {
            if (FD_ISSET(host_fd, &efds)) {
                break;
            }
            if (FD_ISSET(dev_fd, &efds)) {
                select_report(priv, VFS_FILE_ERROR);
                break;
            }

            if (FD_ISSET(dev_fd, &rfds)) {
                while ((ret = read(dev_fd, buf, TTYQ_SIZE)) > 0) {
                    for (i = 0; i < ret; i++) {
                        tty_input((int)buf[i], &priv->tty);
                    }

                    if (tty_readable(&priv->tty)) {
                        select_report(priv, VFS_FILE_READABLE);
                    }
                }
            }

            if (FD_ISSET(host_fd, &wfds)) {
                int c;

                i = 0;
                while ((c = tty_getc(&priv->tty.t_outq)) >= 0) {
                    buf[i++] = (char)c;
                }
                write(dev_fd, buf, i);
                tty_done(&priv->tty);
            }
        }
    }
    close(host_fd);

    __exit1:
    close(dev_fd);

    __exit0:
    device_remove(priv->name);
    tty_detach(&priv->tty);
    kfree(priv);
}

/*
 * 创建 pty
 */
int pty_create(const char *name, int (*attach)(void *arg), void (*fail)(void *arg), void *arg)
{
    privinfo_t *priv;
    uint32_t reg;

    if (attach == NULL || fail == NULL || arg == NULL) {
        return -1;
    }

    reg = interrupt_disable();

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        select_init(priv);
        priv->attach = attach;
        priv->fail   = fail;
        priv->arg    = arg;

        if (device_create(name, "pty", priv) < 0) {
            kfree(priv);
            seterrno(ENOMEM);
            interrupt_resume(reg);
            return -1;
        }

        tty_attach(&priv->tty);
        priv->tty.t_oproc = pty_start;

        strlcpy(priv->name, name, sizeof(priv->name));

        /*
         * PTY 线程优先级设置为较低水平, 保证写操作一次完成
         */
        if (kthread_create(name, pty_thread, priv, 8 * KB, 5) < 0) {
            device_remove(name);
            kfree(priv);
            seterrno(ENOMEM);
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
