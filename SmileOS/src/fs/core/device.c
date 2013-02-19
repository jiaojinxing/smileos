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
#include <string.h>
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
static LIST_HEAD(device_list);                                          /*  设备链表                    */
mutex_t             dev_mgr_lock;                                       /*  设备管理锁                  */
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

    list_add_tail(&dev->dev_list, &device_list);

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
    struct list_head *item;
    device_t *dev;
    int i;

    mutex_lock(&dev_mgr_lock, 0);

    i = 0;
    list_for_each(item, &device_list) {
        dev = list_entry(item, device_t, dev_list);
        if (i >= index) {
            mutex_unlock(&dev_mgr_lock);
            return dev;
        }
        i++;
    }

    mutex_unlock(&dev_mgr_lock);

    return NULL;
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
    struct list_head *item;
    device_t *dev;

    if (name == NULL) {
        return NULL;
    }

    mutex_lock(&dev_mgr_lock, 0);

    list_for_each(item, &device_list) {
        dev = list_entry(item, device_t, dev_list);
        if (strcmp(name, dev->name) == 0) {
            mutex_unlock(&dev_mgr_lock);
            return dev;
        }
    }

    mutex_unlock(&dev_mgr_lock);

    return NULL;
}
/*********************************************************************************************************
** Function name:           device_remove
** Descriptions:            删除设备
** input parameters:        dev                 设备
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int device_remove(device_t *dev)
{
    struct list_head *item, *save;
    device_t *_dev;
    int ret = -1;

    if (dev == NULL) {
        return ret;
    }

    mutex_lock(&dev_mgr_lock, 0);

    if (atomic_read(&dev->ref) == 0) {
        list_for_each_safe(item, save, &device_list) {
            _dev = list_entry(item, device_t, dev_list);
            if (_dev == dev) {
                list_del(&dev->dev_list);
                atomic_dec(&dev->drv->ref);
                kfree(dev);
                ret = 0;
                break;
            }
        }
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

    if (dev_name == NULL || drv_name == NULL || strlen(dev_name) > sizeof(dev->name) - 1) {
        seterrno(EINVAL);
        return -1;
    }

    if (strncmp(dev_name, "/dev/", 5) != 0) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev_name[5] == '\0') {
        seterrno(EINVAL);
        return -1;
    }

    mutex_lock(&dev_mgr_lock, 0);                                       /*  保证查找到安装之间是原子的  */

    if (device_lookup(dev_name) != NULL) {
        mutex_unlock(&dev_mgr_lock);
        seterrno(EEXIST);
        return -1;
    }

    drv = driver_ref_by_name(drv_name);
    if (drv == NULL) {
        mutex_unlock(&dev_mgr_lock);
        seterrno(EINVAL);
        return -1;
    }

    dev = kmalloc(sizeof(device_t), GFP_KERNEL);
    if (dev == NULL) {
        atomic_dec(&drv->ref);
        seterrno(ENOMEM);
        return -1;
    }

    strlcpy(dev->name, dev_name, sizeof(dev->name));
    dev->drv   = drv;
    dev->ctx   = ctx;
    dev->devno = bkdr_hash(dev_name);
    atomic_set(&dev->ref, 0);
    device_install(dev);
    mutex_unlock(&dev_mgr_lock);
    seterrno(0);
    return 0;
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
    INIT_LIST_HEAD(&device_list);

    return mutex_create(&dev_mgr_lock);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
