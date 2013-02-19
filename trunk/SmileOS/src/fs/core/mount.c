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
** File name:               mount.c
** Last modified Date:      2012-3-22
** Last Version:            1.0.0
** Descriptions:            挂载点管理
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-22
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
#include "vfs/device.h"
#include "vfs/fs.h"
#include <string.h>
#include <stdio.h>
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
static LIST_HEAD(mount_point_list);                                     /*  挂载点链表                  */
mount_point_t          *rootfs_point;                                   /*  根文件系统挂载点            */
mutex_t                 point_mgr_lock;                                 /*  挂载点管理锁                */
/*********************************************************************************************************
** Function name:           mount_point_install
** Descriptions:            安装挂载点
** input parameters:        point               挂载点
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mount_point_install(mount_point_t *point)
{
    mutex_lock(&point_mgr_lock, 0);

    list_add(&point->point_list, &mount_point_list);

    mutex_unlock(&point_mgr_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           mount_point_lookup
** Descriptions:            查找挂载点
** input parameters:        name                挂载点名
** output parameters:       NONE
** Returned value:          挂载点
*********************************************************************************************************/
mount_point_t *mount_point_lookup(const char *name)
{
    mount_point_t *point;
    struct list_head *item;

    if (name == NULL) {
        return NULL;
    }

    mutex_lock(&point_mgr_lock, 0);

    list_for_each(item, &mount_point_list) {
        point = list_entry(item, mount_point_t, point_list);
        if (strcmp(point->name, name) == 0) {
            mutex_unlock(&point_mgr_lock);
            return point;
        }
    }

    mutex_unlock(&point_mgr_lock);

    return NULL;
}
/*********************************************************************************************************
** Function name:           mount_point_remove
** Descriptions:            删除挂载点
** input parameters:        point               挂载点
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mount_point_remove(mount_point_t *point)
{
    mount_point_t *_point;
    struct list_head *item, *save;
    int ret = -1;

    if (point == NULL) {
        return ret;
    }

    mutex_lock(&point_mgr_lock, 0);

    if (atomic_read(&point->ref) == 0) {
        list_for_each_safe(item, save, &mount_point_list) {
            _point = list_entry(item, mount_point_t, point_list);
            if (_point == point) {
                list_del(&point->point_list);
                kfree(point);
                ret = 0;
                break;
            }
        }
    }

    mutex_unlock(&point_mgr_lock);

    return ret;
}
/*********************************************************************************************************
** Function name:           mount_point_get
** Descriptions:            获得挂载点
** input parameters:        index               索引
** output parameters:       NONE
** Returned value:          挂载点
*********************************************************************************************************/
mount_point_t *mount_point_get(unsigned int index)
{
    int i;
    mount_point_t *point;
    struct list_head *item;

    mutex_lock(&point_mgr_lock, 0);

    i = 0;
    list_for_each(item, &mount_point_list) {
        point = list_entry(item, mount_point_t, point_list);
        if (i >= index) {
            mutex_unlock(&point_mgr_lock);
            return point;
        }
        i++;
    }

    mutex_unlock(&point_mgr_lock);

    return NULL;
}
/*********************************************************************************************************
** Function name:           mount_point_manager_init
** Descriptions:            初始化挂载点管理
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mount_point_manager_init(void)
{
    rootfs_point = NULL;

    INIT_LIST_HEAD(&mount_point_list);

    return mutex_create(&point_mgr_lock);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
