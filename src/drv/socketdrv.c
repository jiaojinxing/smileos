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
#include "kern/kern.h"
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

/*
 * 私有信息
 */
typedef struct {
    int         sockfd;
    int         ref;
} privinfo_t;

//#define debug        kcomplain
#define debug(...)

/*
 * 打开 socket
 */
static int socket_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    priv->ref++;

    return 0;
}

/*
 * 控制 socket
 */
static int socket_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;

    return lwip_ioctl(priv->sockfd, cmd, arg);
}

/*
 * 控制 socket
 */
static int socket_fcntl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;

    return lwip_fcntl(priv->sockfd, cmd, (int)arg);
}

/*
 * 关闭 socket
 */
static int socket_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;
    char buf[32];

    if (--priv->ref == 0) {
        sprintf(buf, "/dev/socket%d", priv->sockfd);
        device_remove(buf);

        lwip_close(priv->sockfd);

        kfree(priv);
    }

    return 0;
}

/*
 * socket 是不是一个 TTY
 */
static int socket_isatty(void *ctx, file_t *file)
{
    return 1;
}

/*
 * 读 socket
 */
static ssize_t socket_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    debug("%s %d\r\n", __func__, len);

    return lwip_recv(priv->sockfd, buf, len, 0);
}

/*
 * 写 socket
 */
static ssize_t socket_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    return lwip_send(priv->sockfd, buf, len, 0);
}

/*
 * 获得 socket 状态
 */
static int socket_fstat(void *ctx, file_t *file, struct stat *buf)
{
    buf->st_mode    = (buf->st_mode & (~S_IFMT)) | S_IFSOCK;
    buf->st_blksize = 1;

    return 0;
}

/*
 * socket 驱动
 */
driver_t socket_drv = {
        .name   = "socket",
        .open   = socket_open,
        .write  = socket_write,
        .read   = socket_read,
        .isatty = socket_isatty,
        .ioctl  = socket_ioctl,
        .fcntl  = socket_fcntl,
        .close  = socket_close,
        .fstat  = socket_fstat,
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
int socket_attach(int sockfd)
{
    char buf[32];
    int fd;
    privinfo_t *priv;

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        priv->sockfd = sockfd;
        priv->ref    = 0;

        sprintf(buf, "/dev/socket%d", sockfd);
        if (device_create(buf, "socket", priv) < 0) {
            kfree(priv);
            return -1;
        }

        fd = open(buf, O_RDWR, 0666);
        if (fd < 0) {
            device_remove(buf);
            kfree(priv);
            return -1;
        }
        return fd;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

