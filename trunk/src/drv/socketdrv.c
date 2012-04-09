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
** File name:               socketdrv.c
** Last modified Date:      2012-3-28
** Last Version:            1.0.0
** Descriptions:            socket 驱动和设备
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-28
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
#include "vfs/vfs.h"
#include "kern/kern.h"
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

/*
 * 私有信息
 */
typedef struct {
    VFS_SELECT_MEMBERS
    int             sock_fd;
    int             ref;
} privinfo_t;

//#define debug        kcomplain
#define debug(...)

/*
 * 打开 socket
 */
static int socket_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    priv->ref++;
    return 0;
}

/*
 * 控制 socket
 */
static int socket_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return lwip_ioctl(priv->sock_fd, cmd, arg);
}

/*
 * 控制 socket
 */
static int socket_fcntl(void *ctx, file_t *file, int cmd, int arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return lwip_fcntl(priv->sock_fd, cmd, arg);
}

/*
 * 关闭 socket
 */
static int socket_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;
    char buf[32];

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (--priv->ref == 0) {
        sprintf(buf, "/dev/socket%d", priv->sock_fd);
        device_remove(buf);

        lwip_close(priv->sock_fd);

        kfree(priv);
    }
    return 0;
}

/*
 * socket 是不是一个 TTY
 */
static int socket_isatty(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 1;
}

/*
 * 读 socket
 */
static ssize_t socket_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return lwip_recv(priv->sock_fd, buf, len, 0);
}

/*
 * 写 socket
 */
static ssize_t socket_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return lwip_send(priv->sock_fd, buf, len, 0);
}

/*
 * 获得 socket 状态
 */
static int socket_fstat(void *ctx, file_t *file, struct stat *buf)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    buf->st_mode    = (buf->st_mode & (~S_IFMT)) | S_IFSOCK;
    buf->st_blksize = 1;
    return 0;
}

/*
 * 扫描
 */
static int socket_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;
    int readable;
    int writeable;
    int error;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    extern int socket_stat(int sock_fd, int *readable, int *writeable, int *error);
    ret = socket_stat(priv->sock_fd, &readable, &writeable, &error);
    if (ret < 0) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;

    if (readable && flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if (writeable && flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    if (error && flags & VFS_FILE_ERROR) {
        ret |= VFS_FILE_ERROR;
    }
    return ret;
}

#include "selectdrv.h"

/*
 * 回报事件
 *
 */
void smileos_socket_report(int sock_fd, int type)
{
    char buf[32];
    device_t *dev;

    sprintf(buf, "/dev/socket%d", sock_fd);
    dev = device_lookup(buf);
    if (dev != NULL) {
        select_report(dev->ctx, type);
    }
}

/*
 * socket 驱动
 */
driver_t socket_drv = {
        .name     = "socket",
        .open     = socket_open,
        .write    = socket_write,
        .read     = socket_read,
        .isatty   = socket_isatty,
        .ioctl    = socket_ioctl,
        .fcntl    = socket_fcntl,
        .close    = socket_close,
        .fstat    = socket_fstat,
        .scan     = socket_scan,
        .select   = select_select,
        .unselect = select_unselect,
};

/*
 * 初始化 socket
 */
int socket_init(void)
{
    return driver_install(&socket_drv);
}

/*
 * 联结 socket
 */
int socket_attach(int sock_fd)
{
    char buf[32];
    int fd;
    privinfo_t *priv;
    uint32_t reg;

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        select_init(priv);
        priv->sock_fd = sock_fd;
        priv->ref     = 0;

        sprintf(buf, "/dev/socket%d", sock_fd);

        reg = interrupt_disable();
        if (device_create(buf, "socket", priv) < 0) {
            interrupt_resume(reg);
            kfree(priv);
            seterrno(ENOMEM);
            return -1;
        }

        fd = vfs_open(buf, O_RDWR, 0666);
        if (fd < 0) {
            device_remove(buf);
            interrupt_resume(reg);
            kfree(priv);
            return -1;
        }
        interrupt_resume(reg);
        seterrno(0);
        return fd;
    } else {
        seterrno(ENOMEM);
        return -1;
    }
}

/*
 * 获得 socket 的私有文件描述符
 */
int socket_priv_fd(int fd)
{
    file_t *file;
    device_t *dev;
    privinfo_t *priv;
    uint32_t reg;
    int sock_fd;

    reg = interrupt_disable();
    file = vfs_get_file(fd);
    if (file != NULL) {
        dev = file->ctx;
        if (dev != NULL) {
            priv = dev->ctx;
            if (priv != NULL && priv->ref > 0) {
                sock_fd = priv->sock_fd;
                interrupt_resume(reg);
                seterrno(0);
                return sock_fd;
            }
        }
    }
    interrupt_resume(reg);
    seterrno(EBADFD);
    return -1;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

