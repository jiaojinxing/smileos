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
** File name:               fs.c
** Last modified Date:      2012-3-22
** Last Version:            1.0.0
** Descriptions:            文件系统管理
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
#include "module/module.h"
#include <string.h>
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
static LIST_HEAD(file_system_list);                                     /*  文件系统链表                */
mutex_t                     file_system_lock;                           /*  文件系统管理锁              */
/*********************************************************************************************************
** Function name:           file_system_lookup
** Descriptions:            查找文件系统
** input parameters:        name                文件系统名
** output parameters:       NONE
** Returned value:          文件系统 OR NULL
*********************************************************************************************************/
file_system_t *file_system_lookup(const char *name)
{
    file_system_t *fs;
    struct list_head *item;

    if (name == NULL) {
        return NULL;
    }

    mutex_lock(&file_system_lock, 0);

    list_for_each(item, &file_system_list) {
        fs = list_entry(item, file_system_t, node);
        if (strcmp(fs->name, name) == 0) {
            mutex_unlock(&file_system_lock);
            return fs;
        }
    }

    mutex_unlock(&file_system_lock);

    return NULL;
}
/*********************************************************************************************************
** Function name:           file_system_remove
** Descriptions:            删除文件系统
** input parameters:        fs                  文件系统
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int file_system_remove(file_system_t *fs)
{
    struct list_head *item, *save;
    file_system_t *_fs;
    int ret = -1;

    if (fs == NULL) {
        return ret;
    }

    mutex_lock(&file_system_lock, 0);

    if (atomic_read(&fs->ref) == 0) {
        list_for_each_safe(item, save, &file_system_list) {
            _fs = list_entry(item, file_system_t, node);
            if (_fs == fs) {
                list_del(&fs->node);
                module_unref(fs->module);
                ret = 0;
                break;
            }
        }
    }

    mutex_unlock(&file_system_lock);

    return ret;
}
/*********************************************************************************************************
** Function name:           file_system_install
** Descriptions:            安装文件系统
** input parameters:        fs                  文件系统
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int file_system_install(file_system_t *fs)
{
    if (fs == NULL || fs->name == NULL || fs->mount == NULL) {
        return -1;
    }

    fs->module = module_ref_by_addr(fs);

    mutex_lock(&file_system_lock, 0);

    if (file_system_lookup(fs->name) != NULL) {
        mutex_unlock(&file_system_lock);
        module_unref(fs->module);
        return -1;
    }

    atomic_set(&fs->ref, 0);

    list_add_tail(&fs->node, &file_system_list);

    mutex_unlock(&file_system_lock);

    if (fs->init != NULL) {
        fs->init();
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           file_system_manager_init
** Descriptions:            初始化文件系统管理
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int file_system_manager_init(void)
{
    INIT_LIST_HEAD(&file_system_list);

    return mutex_create(&file_system_lock);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
