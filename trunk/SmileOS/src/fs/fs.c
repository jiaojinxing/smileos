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
#include "kern/ipc.h"
#include "vfs/config.h"
#include "vfs/types.h"
#include "module/module.h"
#include <string.h>
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
/*
 * 文件系统链表
 */
static file_system_t   *fs_list;

/*
 * 文件系统管理锁
 */
mutex_t                 fs_mgr_lock;
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

    if (name == NULL) {
        return NULL;
    }

    mutex_lock(&fs_mgr_lock, 0);
    fs = fs_list;
    while (fs != NULL) {
        if (strcmp(fs->name, name) == 0) {
            break;
        }
        fs = fs->next;
    }
    mutex_unlock(&fs_mgr_lock);

    return fs;
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
    file_system_t *prev, *temp;
    int ret = -1;

    if (fs == NULL) {
        return ret;
    }

    mutex_lock(&fs_mgr_lock, 0);

    if (atomic_read(&fs->ref) == 0) {
        prev = NULL;
        temp = fs_list;
        while (temp != NULL && temp != fs) {
            prev = temp;
            temp = temp->next;
        }

        if (temp != NULL) {
            if (prev != NULL) {
                prev->next = fs->next;
            } else {
                fs_list = fs->next;
            }
            ret = 0;
            module_unref(fs->module);
            kfree(fs);
        }
    }
    mutex_unlock(&fs_mgr_lock);

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
    module_t *module;

    if (fs == NULL || fs->name == NULL || fs->mount == NULL) {
        return -1;
    }

    module = module_ref_by_addr(fs);
    fs->module = module;

    mutex_lock(&fs_mgr_lock, 0);
    if (file_system_lookup(fs->name) != NULL) {
        mutex_unlock(&fs_mgr_lock);
        module_unref(module);
        return -1;
    }

    atomic_set(&fs->ref, 0);

    fs->next = fs_list;
    fs_list  = fs;
    mutex_unlock(&fs_mgr_lock);

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
    fs_list = NULL;

    return mutex_new(&fs_mgr_lock);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
