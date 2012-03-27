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
** File name:               rootfs.c
** Last modified Date:      2012-3-22
** Last Version:            1.0.0
** Descriptions:            根文件系统
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
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * 挂载点链表
 */
extern mount_point_t *point_list;

/*
 * 根文件系统挂载点
 */
extern mount_point_t *rootfs_point;

/*
 * 挂载点管理锁
 */
extern kern_mutex_t pointmgr_lock;

/*
 * 私有信息
 */
typedef struct {
    struct dirent   entry;
    mount_point_t  *current;
    long            loc;
} privinfo_t;

static int rootfs_mount(mount_point_t *point, device_t *dev, const char *dev_name)
{
    return 0;
}

static int rootfs_stat(mount_point_t *point, const char *path, struct stat *buf)
{
    if (PATH_IS_ROOT_DIR(path)) {
        buf->st_dev     = (dev_t)0;
        buf->st_ino     = 0;
        buf->st_mode    = 0666;
        buf->st_nlink   = 0;
        buf->st_uid     = 0;
        buf->st_gid     = 0;
        buf->st_rdev    = (dev_t)0;
        buf->st_size    = 0;
        buf->st_atime   = 0;
        buf->st_spare1  = 0;
        buf->st_mtime   = 0;
        buf->st_spare2  = 0;
        buf->st_ctime   = 0;
        buf->st_spare3  = 0;
        buf->st_blksize = 0;
        buf->st_blocks  = 0;
        buf->st_spare4[0] = 0;
        buf->st_spare4[1] = 0;
        return 0;
    } else {
        return -1;
    }
}

static int rootfs_access(mount_point_t *point, const char *path, int amode)
{
    if (PATH_IS_ROOT_DIR(path)) {
        if (F_OK == amode || R_OK == amode) {
            return 0;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

static int rootfs_opendir(mount_point_t *point, file_t *file, const char *path)
{
    privinfo_t *priv;

    if (!PATH_IS_ROOT_DIR(path)) {
        return -1;
    }

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        file->ctx = priv;
        /*
         * 虽然上级有目录锁, 但仍须锁挂载点管理
         */
        kern_mutex_lock(&pointmgr_lock, 0);
        priv->current = point_list;
        kern_mutex_unlock(&pointmgr_lock);
        priv->loc     = 0;
    }

    return priv != NULL ? 0 : -1;
}

static struct dirent *rootfs_readdir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL && priv->current != rootfs_point) {
        strcpy(priv->entry.d_name, priv->current->name + 1);
        priv->entry.d_ino = priv->loc++;
        kern_mutex_lock(&pointmgr_lock, 0);
        priv->current = priv->current->next;
        kern_mutex_unlock(&pointmgr_lock);
        return &priv->entry;
    }
    return NULL;
}

static int rootfs_rewinddir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;
    int ret = -1;

    if (priv != NULL) {
        kern_mutex_lock(&pointmgr_lock, 0);
        priv->current = point_list;
        kern_mutex_unlock(&pointmgr_lock);
        priv->loc     = 0;
        ret           = 0;
    }
    return ret;
}

static int rootfs_seekdir(mount_point_t *point, file_t *file, long loc)
{
    privinfo_t *priv = file->ctx;
    int ret = -1;

    kern_mutex_lock(&pointmgr_lock, 0);
    if (priv != NULL) {
        mount_point_t *point = point_list;
        int i;

        for (i = 0; i < loc && point != rootfs_point; i++) {
            point = point->next;
        }

        if (point != rootfs_point) {
            priv->current = point;
            priv->loc     = loc;
            ret           = 0;
        }
    }
    kern_mutex_unlock(&pointmgr_lock);
    return ret;
}

static long rootfs_telldir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL) {
        return priv->loc;
    }

    return -1;
}

static int rootfs_closedir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL) {
        kfree(priv);
        return 0;
    }

    return -1;
}

file_system_t rootfs = {
        .name       = "rootfs",
        .mount      = rootfs_mount,
        .stat       = rootfs_stat,
        .access     = rootfs_access,

        .opendir    = rootfs_opendir,
        .readdir    = rootfs_readdir,
        .rewinddir  = rootfs_rewinddir,
        .seekdir    = rootfs_seekdir,
        .telldir    = rootfs_telldir,
        .closedir   = rootfs_closedir,
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
