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

/*
 * 打开 ttyS0
 */
static int ttyS0_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    return 0;
}

/*
 * 控制 ttyS0
 */
static int ttyS0_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    return 0;
}

/*
 * 关闭 ttyS0
 */
static int ttyS0_close(void *ctx, file_t *file)
{
    return 0;
}

/*
 * ttyS0 是不是一个 tty
 */
static int ttyS0_isatty(void *ctx, file_t *file)
{
    return 1;
}

/*
 * 读 ttyS0
 */
static ssize_t ttyS0_read(void *ctx, file_t *file, void *buf, size_t len)
{
    return 0;
}

/*
 * 写 ttyS0
 */
static ssize_t ttyS0_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    char *tmp = (char *)buf;
    char  backup;

    backup   = tmp[len];
    tmp[len] = '\0';

    printk(tmp);

    tmp[len] = backup;

    return len;
}

/*
 * ttyS0 驱动
 */
static driver_t ttyS0_drv = {
        .name   = "ttyS0",
        .open   = ttyS0_open,
        .write  = ttyS0_write,
        .read   = ttyS0_read,
        .isatty = ttyS0_isatty,
        .ioctl  = ttyS0_ioctl,
        .close  = ttyS0_close,
};

/*
 * 初始化 ttyS0
 */
int ttyS0_init(void)
{
    driver_install(&ttyS0_drv);

    device_create("/dev/ttyS0",  "ttyS0", NULL);

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
