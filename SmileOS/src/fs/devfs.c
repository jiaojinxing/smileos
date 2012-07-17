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
** Modified by:             JiaoJinXing
** Modified date:           2012-4-15
** Version:                 1.1.0
** Descriptions:            FIX 多任务下可能发生的 BUG(直接访问设备链表, 而设备链表发生变化)
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/ipc.h"
#include "vfs/config.h"
#include "vfs/types.h"
#include "vfs/vfs.h"
#include "vfs/device.h"
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static int devfs_mount(mount_point_t *point, device_t *dev, const char *dev_name)
{
    return 0;
}

static int devfs_open(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode)
{
    device_t *dev = device_lookup(vfs_path_add_mount_point(path));

    if (dev == NULL) {
        seterrno(ENODEV);
        return -1;
    }

    if (dev->drv->open == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    file->ctx = dev;
    return dev->drv->open(dev->ctx, file, oflag, mode);
}

static ssize_t devfs_read(mount_point_t *point, file_t *file, void *buf, size_t len)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->read == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    return dev->drv->read(dev->ctx, file, buf, len);
}

static off_t devfs_lseek(mount_point_t *point, file_t *file, off_t offset, int whence);

static ssize_t devfs_write(mount_point_t *point, file_t *file, const void *buf, size_t len)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->write == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    if (file->flags & O_APPEND) {
        devfs_lseek(point, file, 0, SEEK_END);
    }

    return dev->drv->write(dev->ctx, file, buf, len);
}

static int devfs_ioctl(mount_point_t *point, file_t *file, int cmd, void *arg)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->ioctl == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    return dev->drv->ioctl(dev->ctx, file, cmd, arg);
}

static int devfs_close(mount_point_t *point, file_t *file)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->close == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    return dev->drv->close(dev->ctx, file);
}

static int devfs_fcntl(mount_point_t *point, file_t *file, int cmd, int arg)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->fcntl == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    return dev->drv->fcntl(dev->ctx, file, cmd, arg);
}

static int devfs_fstat(mount_point_t *point, file_t *file, struct stat *buf)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    buf->st_dev     = (dev_t)dev->devno;
    buf->st_ino     = 0;
    buf->st_mode    = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IFCHR;
    buf->st_nlink   = 0;
    buf->st_uid     = 0;
    buf->st_gid     = 0;
    buf->st_rdev    = (dev_t)dev->devno;
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

    if (dev->drv->fstat == NULL) {
        return 0;
    }

    return dev->drv->fstat(dev->ctx, file, buf);
}

static int devfs_isatty(mount_point_t *point, file_t *file)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->isatty == NULL) {
        return 0;
    }

    return dev->drv->isatty(dev->ctx, file);
}

static int devfs_fsync(mount_point_t *point, file_t *file)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->fsync == NULL) {
        return 0;
    }

    return dev->drv->fsync(dev->ctx, file);
}

static int devfs_fdatasync(mount_point_t *point, file_t *file)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->fdatasync == NULL) {
        return 0;
    }

    return dev->drv->fdatasync(dev->ctx, file);
}

static int devfs_ftruncate(mount_point_t *point, file_t *file, off_t len)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->ftruncate == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    return dev->drv->ftruncate(dev->ctx, file, len);
}

static off_t devfs_lseek(mount_point_t *point, file_t *file, off_t offset, int whence)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->lseek == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    return dev->drv->lseek(dev->ctx, file, offset, whence);
}

static int devfs_scan(mount_point_t *point, file_t *file, int flags)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->scan == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    return dev->drv->scan(dev->ctx, file, flags);
}

static int devfs_select(mount_point_t *point, file_t *file, int flags)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->select == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    return dev->drv->select(dev->ctx, file, flags);
}

static int devfs_unselect(mount_point_t *point, file_t *file, int flags)
{
    device_t *dev = file->ctx;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (dev->drv->unselect == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    return dev->drv->unselect(dev->ctx, file, flags);
}

static int devfs_stat(mount_point_t *point, const char *path, struct stat *buf)
{
    if (VFS_PATH_IS_ROOT(path)) {
        buf->st_dev     = (dev_t)0;
        buf->st_ino     = 0;
        buf->st_mode    = S_IRWXU | S_IRWXG | S_IRWXO | S_IFDIR;
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
        int fd = vfs_open(vfs_path_add_mount_point(path), O_RDONLY, 0666);
        if (fd >= 0) {
            int ret = vfs_fstat(fd, buf);
            vfs_close(fd);
            return ret;
        } else {
            return -1;
        }
    }
}

static int devfs_access(mount_point_t *point, const char *path, int amode)
{
    struct stat buf;
    int ret;

    ret = devfs_stat(point, path, &buf);
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

/*
 * 私有信息
 */
typedef struct {
    struct dirent   entry;
    long            loc;
} privinfo_t;

static int devfs_opendir(mount_point_t *point, file_t *file, const char *path)
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

static struct dirent *devfs_readdir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;
    device_t   *dev;

    if (priv == NULL) {
        seterrno(EINVAL);
        return NULL;
    }

    extern mutex_t dev_mgr_lock;

    mutex_lock(&dev_mgr_lock, 0);
    dev = device_get(priv->loc);
    if (dev != NULL) {
        strcpy(priv->entry.d_name, dev->name + 5);                      /*  跳过 /dev/                  */
        mutex_unlock(&dev_mgr_lock);
        priv->entry.d_ino = priv->loc++;
        return &priv->entry;
    } else {
        mutex_unlock(&dev_mgr_lock);
        return NULL;
    }
}

static int devfs_rewinddir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    priv->loc = 0;
    return 0;
}

static int devfs_seekdir(mount_point_t *point, file_t *file, long loc)
{
    privinfo_t *priv = file->ctx;
    device_t *dev;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    dev = device_get(priv->loc);
    if (dev != NULL) {
        priv->loc = loc;
        return 0;
    } else {
        seterrno(EINVAL);
        return -1;
    }
}

static long devfs_telldir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    return priv->loc;
}

static int devfs_closedir(mount_point_t *point, file_t *file)
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

file_system_t devfs = {
        .name       = "devfs",
        .mount      = devfs_mount,
        .stat       = devfs_stat,
        .access     = devfs_access,

        .open       = devfs_open,
        .read       = devfs_read,
        .write      = devfs_write,
        .ioctl      = devfs_ioctl,
        .close      = devfs_close,
        .fcntl      = devfs_fcntl,
        .fstat      = devfs_fstat,
        .isatty     = devfs_isatty,
        .fsync      = devfs_fsync,
        .fdatasync  = devfs_fdatasync,
        .ftruncate  = devfs_ftruncate,
        .lseek      = devfs_lseek,

        .opendir    = devfs_opendir,
        .readdir    = devfs_readdir,
        .rewinddir  = devfs_rewinddir,
        .seekdir    = devfs_seekdir,
        .telldir    = devfs_telldir,
        .closedir   = devfs_closedir,

        .scan       = devfs_scan,
        .select     = devfs_select,
        .unselect   = devfs_unselect,
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
