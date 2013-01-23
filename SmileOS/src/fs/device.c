/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <jiaojinxing1987@gmail.com>
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
#include "kern/kern.h"
#include "kern/ipc.h"
#include "vfs/config.h"
#include "vfs/types.h"
#include "vfs/driver.h"
#include "vfs/utils.h"
#include <string.h>
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
/*
 * 设备链表
 */
static device_t    *dev_list;

/*
 * 设备管理锁
 */
mutex_t             dev_mgr_lock;

extern mutex_t      drv_mgr_lock;
/*********************************************************************************************************
** Function name:           device_install
** Descriptions:            安装设备
** input parameters:        dev                 设备
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int device_install(device_t *dev)
{
    mutex_lock(&dev_mgr_lock, 0);

    dev->next = dev_list;
    dev_list  = dev;

    mutex_unlock(&dev_mgr_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           device_get
** Descriptions:            获得设备
** input parameters:        index               设备索引
** output parameters:       NONE
** Returned value:          设备 OR NULL
*********************************************************************************************************/
device_t *device_get(unsigned int index)
{
    int i;
    device_t *dev;

    mutex_lock(&dev_mgr_lock, 0);

    for (i = 0, dev = dev_list; i < index && dev != NULL; i++, dev = dev->next) {
    }

    mutex_unlock(&dev_mgr_lock);

    return dev;
}
/*********************************************************************************************************
** Function name:           device_lookup
** Descriptions:            查找设备
** input parameters:        name                设备名
** output parameters:       NONE
** Returned value:          设备 OR NULL
*********************************************************************************************************/
device_t *device_lookup(const char *name)
{
    device_t *dev;
    unsigned int key;

    if (name == NULL) {
        return NULL;
    }

    key = BKDRHash(name);

    mutex_lock(&dev_mgr_lock, 0);
    dev = dev_list;
    while (dev != NULL) {
        if (key == dev->key) {
            break;
        }
        dev = dev->next;
    }
    mutex_unlock(&dev_mgr_lock);

    return dev;
}
/*********************************************************************************************************
** Function name:           device_remove
** Descriptions:            删除设备
** input parameters:        name                设备名
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int device_remove(const char *name)
{
    device_t *dev, *prev;
    unsigned int key;
    int ret = -1;

    if (name == NULL) {
        return ret;
    }

    key = BKDRHash(name);

    mutex_lock(&dev_mgr_lock, 0);
    prev = NULL;
    dev  = dev_list;
    while (dev != NULL) {
        if (key == dev->key) {
            break;
        }
        prev = dev;
        dev  = dev->next;
    }

    if (dev != NULL && atomic_read(&dev->ref) == 0) {
        if (prev != NULL) {
            prev->next = dev->next;
        } else {
            dev_list = dev->next;
        }
        atomic_dec(&dev->drv->ref);
        kfree(dev);
        ret = 0;
    }
    mutex_unlock(&dev_mgr_lock);

    return ret;
}
/*********************************************************************************************************
** Function name:           device_create
** Descriptions:            创建设备
** input parameters:        dev_name            设备名
**                          drv_name            驱动名
**                          ctx                 上下文
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int device_create(const char *dev_name, const char *drv_name, void *ctx)
{
    driver_t *drv;
    device_t *dev;

    if (dev_name == NULL || drv_name == NULL) {
        return -1;
    }

    if (strncmp(dev_name, "/dev/", 5) != 0) {
        return -1;
    }

    if (dev_name[5] == '\0') {
        return -1;
    }

    mutex_lock(&dev_mgr_lock, 0);                                       /*  保证查找到安装之间是原子的  */
    if (device_lookup(dev_name) != NULL) {
        mutex_unlock(&dev_mgr_lock);
        return -1;
    }

    mutex_lock(&drv_mgr_lock, 0);

    drv = driver_lookup(drv_name);
    if (drv != NULL) {

        atomic_inc(&drv->ref);

        mutex_unlock(&drv_mgr_lock);

        dev = kmalloc(sizeof(device_t));
        if (dev != NULL) {
            strlcpy(dev->name, dev_name, sizeof(dev->name));
            dev->drv   = drv;
            dev->ctx   = ctx;
            dev->devno = 0;
            dev->key   = BKDRHash(dev_name);
            atomic_set(&dev->ref, 0);
            device_install(dev);
            mutex_unlock(&dev_mgr_lock);
            return 0;
        } else {
            atomic_dec(&drv->ref);
        }
    }
    mutex_unlock(&dev_mgr_lock);
    return -1;
}
/*********************************************************************************************************
** Function name:           device_manager_init
** Descriptions:            初始化设备管理
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int device_manager_init(void)
{
    dev_list = NULL;

    return mutex_new(&dev_mgr_lock);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
