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
** File name:               devfs.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            设备文件系统
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-20
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
#include "kern/config.h"
#include "kern/types.h"
#include "vfs/vfs.h"
#include <string.h>

static int devfs_mount(mount_point_t *point, device_t *dev, const char *dev_name)
{
    return 0;
}

static int devfs_open(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode)
{
    return 0;
}

static ssize_t devfs_read(mount_point_t *point, file_t *file, void *buf, size_t len)
{
    return 0;
}

static ssize_t devfs_write(mount_point_t *point, file_t *file, const void *buf, size_t len)
{
    return 0;
}

static int devfs_ioctl(mount_point_t *point, file_t *file, int cmd, void *arg)
{
    return 0;
}

static int devfs_close(mount_point_t *point, file_t *file)
{
    return 0;
}

file_system_t devfs = {
        .name  = "devfs",
        .mount = devfs_mount,
        .open  = devfs_open,
        .read  = devfs_read,
        .write = devfs_write,
        .ioctl = devfs_ioctl,
        .close = devfs_close,
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
