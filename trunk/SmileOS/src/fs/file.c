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
** File name:               file.c
** Last modified Date:      2012-7-11
** Last Version:            1.0.0
** Descriptions:            文件结构管理
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-11
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

/*
 * 空闲文件结构链表
 */
static file_t *free_file_list;

/*
 * 文件结构池
 */
static file_t file_pool[VFS_FILE_NR];

/*
 * 文件结构管理锁
 */
static mutex_t file_mgr_lock;

/*
 * 分配空闲文件结构
 */
file_t *file_alloc(void)
{
    file_t *file;

    mutex_lock(&file_mgr_lock, 0);
    file = free_file_list;
    if (file != NULL) {
        free_file_list = file->next;
        file->ref = 1;
    }
    mutex_unlock(&file_mgr_lock);
    return file;
}

/*
 * 释放文件结构
 */
void file_free(file_t *file)
{
    mutex_lock(&file_mgr_lock, 0);
    if (file != NULL && file->ref > 0) {
        file->ref--;
        if (file->ref == 0) {
            file->next = free_file_list;
            free_file_list = file;
        }
    }
    mutex_unlock(&file_mgr_lock);
}

/*
 * 初始化文件结构管理
 */
int file_manager_init(void)
{
    int i;

    free_file_list = file_pool;

    for (i = 0; i < VFS_FILE_NR - 1; i++) {
        file_pool[i].next = &file_pool[i + 1];
    }

    file_pool[VFS_FILE_NR - 1].next = NULL;

    return mutex_new(&file_mgr_lock);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
