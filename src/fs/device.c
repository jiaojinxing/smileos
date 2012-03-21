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
** File name:               device.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            设备管理
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
#include "kern/kern.h"
#include "vfs/vfs.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include <string.h>

/*
 * 设备链表
 */
static device_t *dev_list;

/*
 * 安装设备
 */
static int device_install(device_t *dev)
{
    dev->next = dev_list;
    dev_list  = dev;

    return 0;
}

/*
 * 查找驱动
 */
device_t *device_lookup(const char *name)
{
    device_t *dev = dev_list;

    if (name == NULL) {
        return NULL;
    }

    while (dev != NULL) {
        if (strcmp(dev->name, name) == 0) {
            break;
        }
        dev = dev->next;
    }
    return dev;
}

/*
 * 创建设备
 */
int device_create(const char *dev_name, const char *drv_name, void *ctx)
{
    driver_t *drv;
    device_t *dev;

    if (dev_name == NULL) {
        return -1;
    }

    if (strncmp(dev_name, "/dev/", 5) != 0) {
        return -1;
    }

    if (dev_name[5] == '\0') {
        return -1;
    }

    if (device_lookup(dev_name) != NULL) {
        return -1;
    }

    drv = driver_lookup(drv_name);
    if (drv != NULL) {
        dev = kmalloc(sizeof(device_t));
        if (dev != NULL) {
            strcpy(dev->name, dev_name);
            dev->ctx = ctx;
            device_install(dev);
            return 0;
        }
    }
    return -1;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
