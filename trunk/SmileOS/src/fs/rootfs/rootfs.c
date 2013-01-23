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
#include "vfs/mount.h"
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

/*
 * 私有信息
 */
typedef struct {
    struct dirent   entry;
    long            loc;
} privinfo_t;

static int rootfs_mount(mount_point_t *point, device_t *dev, const char *dev_name, const char *param)
{
    return 0;
}

static int rootfs_stat(mount_point_t *point, const char *path, struct stat *buf)
{
    if (VFS_PATH_IS_ROOT(path)) {
        buf->st_dev         = (dev_t)0;
        buf->st_ino         = 0;
        buf->st_mode        = S_IRWXU | S_IRWXG | S_IRWXO | S_IFDIR;
        buf->st_nlink       = 0;
        buf->st_uid         = 0;
        buf->st_gid         = 0;
        buf->st_rdev        = (dev_t)0;
        buf->st_size        = 0;
        buf->st_atime       = 0;
        buf->st_spare1      = 0;
        buf->st_mtime       = 0;
        buf->st_spare2      = 0;
        buf->st_ctime       = 0;
        buf->st_spare3      = 0;
        buf->st_blksize     = 0;
        buf->st_blocks      = 0;
        buf->st_spare4[0]   = 0;
        buf->st_spare4[1]   = 0;
        return 0;
    } else {
        seterrno(ENOENT);
        return -1;
    }
}

static int rootfs_access(mount_point_t *point, const char *path, int amode)
{
    struct stat buf;
    int         ret;

    ret = rootfs_stat(point, path, &buf);
    if (ret == 0) {
        if ((buf.st_mode & 0700) == (amode * 8 * 8)) {
            return 0;
        } else {
            seterrno(EACCES);
            return -1;
        }
    } else {
        return ret;
    }
}

static int rootfs_opendir(mount_point_t *point, file_t *file, const char *path)
{
    privinfo_t *priv;

    if (!VFS_PATH_IS_ROOT(path)) {
        seterrno(ENOENT);
        return -1;
    }

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        file->ctx = priv;
        priv->loc = 0;
        return 0;
    } else {
        seterrno(ENOMEM);
        return -1;
    }
}

static struct dirent *rootfs_readdir(mount_point_t *point, file_t *file)
{
    privinfo_t    *priv = file->ctx;
    mount_point_t *__point;

    if (priv == NULL) {
        seterrno(EINVAL);
        return NULL;
    }

    extern mutex_t point_mgr_lock;
    extern mount_point_t *rootfs_point;

    mutex_lock(&point_mgr_lock, 0);
    __point = mount_point_get(priv->loc);
    if (__point != NULL && __point != rootfs_point) {
        strcpy(priv->entry.d_name, __point->name + 1);                  /*  跳过 /                      */
        mutex_unlock(&point_mgr_lock);
        priv->entry.d_ino = priv->loc++;
        return &priv->entry;
    } else {
        mutex_unlock(&point_mgr_lock);
        return NULL;
    }
}

static int rootfs_rewinddir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    priv->loc = 0;
    return 0;
}

static int rootfs_seekdir(mount_point_t *point, file_t *file, long loc)
{
    privinfo_t    *priv = file->ctx;
    mount_point_t *__point;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    __point = mount_point_get(priv->loc);
    if (__point != NULL) {
        priv->loc = loc;
        return 0;
    } else {
        seterrno(EINVAL);
        return -1;
    }
}

static long rootfs_telldir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    return priv->loc;
}

static int rootfs_closedir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    kfree(priv);
    file->ctx = NULL;
    return 0;
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
** END FILE
*********************************************************************************************************/
