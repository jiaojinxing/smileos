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
** File name:               null.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            NULL 驱动和设备
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
#include <errno.h>

/*
 * 打开 null
 */
static int null_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    return 0;
}

/*
 * 控制 null
 */
static int null_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    return 0;
}

/*
 * 关闭 null
 */
static int null_close(void *ctx, file_t *file)
{
    return 0;
}

/*
 * null 是不是一个 tty
 */
static int null_isatty(void *ctx, file_t *file)
{
    return 0;
}

/*
 * 读 null
 */
static ssize_t null_read(void *ctx, file_t *file, void *buf, size_t len)
{
    return 0;
}

/*
 * 写 null
 */
static ssize_t null_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    return len;
}

/*
 * null 驱动
 */
static driver_t null_drv = {
        .name   = "null",
        .open   = null_open,
        .write  = null_write,
        .read   = null_read,
        .isatty = null_isatty,
        .ioctl  = null_ioctl,
        .close  = null_close,
};

/*
 * 初始化 null
 */
int null_init(void)
{
    driver_install(&null_drv);

    device_create("/dev/null", "null", NULL);

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
