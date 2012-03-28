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
** Descriptions:            socket �������豸
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-28
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
#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>

/*
 * �� socket
 */
static int socket_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    if (file->ctx != NULL) {
        return -1;
    } else {
        file->ctx = (void *)TRUE;
        return 0;
    }
}

/*
 * ���� socket
 */
static int socket_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    return lwip_ioctl((int)ctx, cmd, arg);
}

/*
 * ���� socket
 */
static int socket_fcntl(void *ctx, file_t *file, int cmd, void *arg)
{
    return lwip_fcntl((int)ctx, cmd, (int)arg);
}

/*
 * �ر� socket
 */
static int socket_close(void *ctx, file_t *file)
{
    char buf[32];

    sprintf(buf, "/dev/socket%d", (int)ctx);

    device_remove(buf);

    lwip_close((int)ctx);

    return 0;
}

/*
 * isatty
 */
static int socket_isatty(void *ctx, file_t *file)
{
    return 1;
}

/*
 * �� socket
 */
static ssize_t socket_read(void *ctx, file_t *file, void *buf, size_t len)
{
    printf("%s try to read %d byte", __func__, len);

    return lwip_recv((int)ctx, buf, len, 0);
}

/*
 * д socket
 */
static ssize_t socket_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    char *tmp = (char *)buf;
    tmp[len] = 0;

    printf("%s try to write %d byte to socket %d", __func__, len, (int)ctx);

    return lwip_send((int)ctx, buf, len, 0);
}

/*
 * socket ����
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
};

/*
 * ��ʼ�� socket
 */
int socket_init(void)
{
    return driver_install(&socket_drv);
}

/*
 * ���� socket
 */
int socket_attach(int sockfd)
{
    char buf[32];
    int fd;

    sprintf(buf, "/dev/socket%d", sockfd);

    device_create(buf, "socket", (void *)sockfd);

    fd = open(buf, O_RDWR, 0666);
    if (fd < 0) {
        printf("failed to open %s\r\n", buf);
    }

    return fd;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

