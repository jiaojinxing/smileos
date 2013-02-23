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
** File name:               vfs.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            虚拟文件系统
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
#define FS_VAR_INC
#include "vfs_common.h"
/*********************************************************************************************************
** Function name:           vfs_file_alloc
** Descriptions:            释放文件结构
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          文件结构 OR NULL
*********************************************************************************************************/
file_t *vfs_file_alloc(void)
{
    file_t *file = kzalloc(sizeof(file_t), GFP_KERNEL);
    if (file != NULL) {
        if (mutex_create(&file->lock) < 0) {
            kfree(file);
            file = NULL;
        }
        atomic_set(&file->ref, 1);
    }
    return file;
}
/*********************************************************************************************************
** Function name:           vfs_file_free
** Descriptions:            释放文件结构
** input parameters:        file                文件结构
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
void vfs_file_free(file_t *file)
{
    mutex_destroy(&file->lock);
    kfree(file);
}
/*********************************************************************************************************
** Function name:           vfs_init
** Descriptions:            初始化虚拟文件系统
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_init(void)
{
    int i;

    driver_manager_init();

    device_manager_init();

    file_system_manager_init();

    mount_point_manager_init();

    for (i = 0; i < PROCESS_NR; i++) {
        mutex_create(&info_lock[i]);
    }

    memset(infos, 0, sizeof(infos));
    memset(cwd,   0, sizeof(cwd));

    extern file_system_t rootfs;
    file_system_install(&rootfs);

    extern file_system_t devfs;
    file_system_install(&devfs);

    extern file_system_t fatfs;
    file_system_install(&fatfs);

    extern file_system_t yaffs1;
    file_system_install(&yaffs1);

    extern file_system_t yaffs2;
    file_system_install(&yaffs2);

    extern file_system_t nfs;
    file_system_install(&nfs);

    vfs_mount("/",      NULL, "rootfs", NULL);

    vfs_mount("/dev",   NULL, "devfs", NULL);

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
