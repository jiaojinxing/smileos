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
** File name:               devfs.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            设备文件系统
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "vfs/vfs.h"
#include <dirent.h>
#include <string.h>

/*
 * 设备链表
 */
extern device_t *dev_list;

/*
 * 设备管理锁
 */
extern kern_mutex_t devmgr_lock;

/*
 * 私有信息
 */
typedef struct {
    struct dirent   entry;
    device_t       *current;
    long            loc;
} privinfo_t;

static int devfs_mount(mount_point_t *point, device_t *dev, const char *dev_name)
{
    return 0;
}

static int devfs_open(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode)
{
    return 0;
}

static ssize_t devfs_read(mount_point_t *point, file_t *file, void *buf, size_t len)
{
    return 0;
}

static ssize_t devfs_write(mount_point_t *point, file_t *file, const void *buf, size_t len)
{
    return 0;
}

static int devfs_ioctl(mount_point_t *point, file_t *file, int cmd, void *arg)
{
    return 0;
}

static int devfs_close(mount_point_t *point, file_t *file)
{
    return 0;
}

static int devfs_opendir(mount_point_t *point, file_t *file, const char *path)
{
    privinfo_t *priv = kmalloc(sizeof(privinfo_t));

    file->ctx = priv;

    if (priv != NULL) {
        kern_mutex_lock(&devmgr_lock, 0);
        priv->current = dev_list;
        kern_mutex_unlock(&devmgr_lock);
        priv->loc     = 0;
    }

    return priv != NULL ? 0 : -1;
}

static struct dirent *devfs_readdir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL && priv->current != NULL) {
        strcpy(priv->entry.d_name, priv->current->name);
        priv->entry.d_ino = priv->loc++;
        kern_mutex_lock(&devmgr_lock, 0);
        priv->current = priv->current->next;
        kern_mutex_unlock(&devmgr_lock);
        return &priv->entry;
    }
    return NULL;
}

static void devfs_rewinddir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL) {
        kern_mutex_lock(&devmgr_lock, 0);
        priv->current = dev_list;
        kern_mutex_unlock(&devmgr_lock);
        priv->loc     = 0;
    }
}

static void devfs_seekdir(mount_point_t *point, file_t *file, long loc)
{
    privinfo_t *priv = file->ctx;

    kern_mutex_lock(&devmgr_lock, 0);
    if (priv != NULL) {
        device_t *dev = dev_list;
        int i;

        for (i = 0; i < loc && dev != NULL; i++) {
            dev = dev->next;
        }

        if (point != NULL) {
            priv->current = dev;
            priv->loc     = loc;
        }
    }
    kern_mutex_unlock(&devmgr_lock);
}

static long devfs_telldir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL) {
        return priv->loc;
    }

    return -1;
}

static int devfs_closedir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL) {
        kfree(priv);
        return 0;
    }

    return -1;
}

file_system_t devfs = {
        .name       = "devfs",
        .mount      = devfs_mount,
        .open       = devfs_open,
        .read       = devfs_read,
        .write      = devfs_write,
        .ioctl      = devfs_ioctl,
        .close      = devfs_close,
        .opendir    = devfs_opendir,
        .readdir    = devfs_readdir,
        .rewinddir  = devfs_rewinddir,
        .seekdir    = devfs_seekdir,
        .telldir    = devfs_telldir,
        .closedir   = devfs_closedir,
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
