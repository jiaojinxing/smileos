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
#include "kern/ipc.h"
#include "vfs/config.h"
#include "vfs/types.h"
#include "module/module.h"
#include <string.h>
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
/*
 * 驱动链表
 */
static driver_t    *drv_list;

/*
 * 驱动管理锁
 */
mutex_t             drv_mgr_lock;
/*********************************************************************************************************
** Function name:           driver_lookup
** Descriptions:            查找驱动
** input parameters:        name                驱动名
** output parameters:       NONE
** Returned value:          驱动
*********************************************************************************************************/
driver_t *driver_lookup(const char *name)
{
    driver_t *drv;

    if (name == NULL) {
        return NULL;
    }

    mutex_lock(&drv_mgr_lock, 0);
    drv = drv_list;
    while (drv != NULL) {
        if (strcmp(drv->name, name) == 0) {
            break;
        }
        drv = drv->next;
    }
    mutex_unlock(&drv_mgr_lock);

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
    driver_t *prev, *temp;
    int ret = -1;

    if (drv == NULL) {
        return ret;
    }

    mutex_lock(&drv_mgr_lock, 0);

    if (atomic_read(&drv->ref) == 0) {
        prev = NULL;
        temp = drv_list;
        while (temp != NULL && temp != drv) {
            prev = temp;
            temp = temp->next;
        }

        if (temp != NULL) {
            if (prev != NULL) {
                prev->next = drv->next;
            } else {
                drv_list = drv->next;
            }
            ret = 0;
            module_unref(drv->module);
            kfree(drv);
        }
    }
    mutex_unlock(&drv_mgr_lock);

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
    module_t *module;

    if (drv == NULL || drv->name == NULL) {
        return -1;
    }

    module = module_ref_by_addr(drv);
    drv->module = module;

    mutex_lock(&drv_mgr_lock, 0);
    if (driver_lookup(drv->name) != NULL) {
        mutex_unlock(&drv_mgr_lock);
        module_unref(module);
        return -1;
    }

    atomic_set(&drv->ref, 0);

    drv->next = drv_list;
    drv_list  = drv;
    mutex_unlock(&drv_mgr_lock);

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
    drv_list = NULL;

    return mutex_new(&drv_mgr_lock);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
