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
** File name:               driver.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            驱动管理
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
#include "module/module.h"
#include <string.h>
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
static LIST_HEAD(driver_list);                                          /*  驱动链表                    */
static mutex_t      driver_lock;                                        /*  驱动管理锁                  */
/*********************************************************************************************************
** Function name:           driver_lookup
** Descriptions:            查找驱动
** input parameters:        name                驱动名
** output parameters:       NONE
** Returned value:          驱动
*********************************************************************************************************/
driver_t *driver_lookup(const char *name)
{
    struct list_head *item;
    driver_t *drv;

    if (name == NULL) {
        return NULL;
    }

    mutex_lock(&driver_lock, 0);

    list_for_each(item, &driver_list) {
        drv = list_entry(item, driver_t, node);
        if (strcmp(drv->name, name) == 0) {
            mutex_unlock(&driver_lock);
            return drv;
        }
    }

    mutex_unlock(&driver_lock);

    return NULL;
}
/*********************************************************************************************************
** Function name:           driver_ref_by_name
** Descriptions:            通过名字引用驱动
** input parameters:        name                驱动名
** output parameters:       NONE
** Returned value:          驱动
*********************************************************************************************************/
driver_t *driver_ref_by_name(const char *name)
{
    driver_t *drv;

    mutex_lock(&driver_lock, 0);

    drv = driver_lookup(name);
    if (drv != NULL) {
        atomic_inc(&drv->ref);
    }

    mutex_unlock(&driver_lock);

    return drv;
}
/*********************************************************************************************************
** Function name:           driver_remove
** Descriptions:            删除驱动
** input parameters:        drv                 驱动
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int driver_remove(driver_t *drv)
{
    struct list_head *item, *save;
    driver_t *_drv;
    int ret = -1;

    if (drv == NULL) {
        return ret;
    }

    mutex_lock(&driver_lock, 0);

    if (atomic_read(&drv->ref) == 0) {
        list_for_each_safe(item, save, &driver_list) {
            _drv = list_entry(item, driver_t, node);
            if (_drv == drv) {
                list_del(&drv->node);
                module_unref(drv->module);
                ret = 0;
                break;
            }
        }
    }

    mutex_unlock(&driver_lock);

    return ret;
}
/*********************************************************************************************************
** Function name:           driver_install
** Descriptions:            安装驱动
** input parameters:        drv                 驱动
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int driver_install(driver_t *drv)
{
    if (drv == NULL || drv->name == NULL) {
        return -1;
    }

    drv->module = module_ref_by_addr(drv);

    mutex_lock(&driver_lock, 0);

    if (driver_lookup(drv->name) != NULL) {
        mutex_unlock(&driver_lock);
        module_unref(drv->module);
        return -1;
    }

    atomic_set(&drv->ref, 0);

    list_add_tail(&drv->node, &driver_list);

    mutex_unlock(&driver_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           driver_manager_init
** Descriptions:            初始化驱动管理
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int driver_manager_init(void)
{
    INIT_LIST_HEAD(&driver_list);

    return mutex_create(&driver_lock);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
