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
** File name:               ttydrv.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            TTY 驱动和设备
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "kern/kern.h"

/*
 * 打开 tty
 */
static int tty_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    return 0;
}

/*
 * 控制 tty
 */
static int tty_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    int ret = 0;

    switch (cmd) {
    case 0:
        break;

    default:
        ret = -1;
        break;
    }
    return ret;
}

/*
 * 关闭 tty
 */
static int tty_close(void *ctx, file_t *file)
{
    return 0;
}

/*
 * tty
 */
static int tty_isatty(void *ctx, file_t *file)
{
    return 1;
}

/*
 * 读 tty
 */
static ssize_t tty_read(void *ctx, file_t *file, void *buf, size_t len)
{
    return -1;
}

/*
 * 写 tty
 */
static ssize_t tty_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    char *tmp = (char *)buf;
    tmp[len] = 0;

    printk(tmp);

    return len;
}

/*
 * tty 驱动
 */
static driver_t tty_drv = {
        .name   = "tty",
        .open   = tty_open,
        .write  = tty_write,
        .read   = tty_read,
        .isatty = tty_isatty,
        .ioctl  = tty_ioctl,
        .close  = tty_close,
};

/*
 * 初始化 tty
 */
int tty_init(void)
{
    driver_install(&tty_drv);

    device_create("/dev/tty0", "tty", NULL);

    device_create("/dev/tty1", "tty", NULL);

    device_create("/dev/tty2", "tty", NULL);

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
