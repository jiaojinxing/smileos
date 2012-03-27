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
** Modified by:             JiaoJinXing
** Modified date:           2012-3-27
** Version:                 1.1.0
** Descriptions:            查找到安装期间必须上锁
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

/*
 * 挂载点链表
 */
mount_point_t *point_list;

/*
 * 根文件系统挂载点
 */
mount_point_t *rootfs_point;

/*
 * 挂载点管理锁
 */
kern_mutex_t pointmgr_lock;

/*
 * 安装挂载点
 */
static int mount_point_install(mount_point_t *point)
{
    kern_mutex_lock(&pointmgr_lock, 0);
    point->next = point_list;
    point_list  = point;
    kern_mutex_unlock(&pointmgr_lock);

    return 0;
}

/*
 * 查找挂载点
 */
mount_point_t *mount_point_lookup(const char *name)
{
    mount_point_t *point;

    if (name == NULL) {
        return NULL;
    }

    kern_mutex_lock(&pointmgr_lock, 0);
    point = point_list;
    while (point != NULL) {
        if (strcmp(point->name, name) == 0) {
            break;
        }
        point = point->next;
    }
    kern_mutex_unlock(&pointmgr_lock);

    return point;
}

/*
 * 挂载
 */
int mount(const char *point_name, const char *dev_name, const char *fs_name)
{
    mount_point_t *point;
    file_system_t *fs;
    device_t *dev;
    int ret;

    if (point_name == NULL ||
        fs_name    == NULL) {
        return -1;
    }

    kern_mutex_lock(&pointmgr_lock, 0);

    point = mount_point_lookup(point_name);                             /*  查找挂载点                  */
    if (point == NULL) {                                                /*  没找到                      */
        fs = file_system_lookup(fs_name);                               /*  查找文件系统                */
        if (fs != NULL) {
            dev = device_lookup(dev_name);                              /*  查找设备                    */

            point = kmalloc(sizeof(mount_point_t));                     /*  分配挂载点                  */
            if (point != NULL) {
                if (point_name[0] == '/') {                             /*  保证挂载点以 / 号开始       */
                    strlcpy(point->name, point_name, sizeof(point->name));
                } else {
                    snprintf(point->name, sizeof(point->name), "/%s", point_name);
                }

                if (point->name[1] != '\0') {                           /*  如果不是根文件系统          */
                    if (strchr(point->name + 1, '/') != NULL) {         /*  保证挂载点不能再出现 / 号   */
                        kfree(point);                                   /*  因为我不知道 / 号还有几个   */
                        kern_mutex_unlock(&pointmgr_lock);
                        return -1;                                      /*  所以当作出错来处理          */
                    }
                } else {
                    if (rootfs_point != NULL) {                         /*  不能再次挂载根文件系统      */
                        kfree(point);
                        kern_mutex_unlock(&pointmgr_lock);
                        return -1;
                    }
                    rootfs_point = point;
                }

                point->fs   = fs;
                point->dev  = dev;

                /*
                 * 当设备为空时, 设备名可作挂载参数
                 */
                ret = fs->mount(point, dev, dev_name);                  /*  挂载                        */
                if (ret < 0) {
                    kfree(point);
                    kern_mutex_unlock(&pointmgr_lock);
                    return -1;
                } else {
                    mount_point_install(point);                         /*  安装挂载点                  */
                    kern_mutex_unlock(&pointmgr_lock);
                    return 0;
                }
            }
        }
    }

    kern_mutex_unlock(&pointmgr_lock);
    return -1;
}

/*
 * 初始化挂载点管理
 */
int mount_point_manager_init(void)
{
    rootfs_point = NULL;
    return kern_mutex_new(&pointmgr_lock);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
