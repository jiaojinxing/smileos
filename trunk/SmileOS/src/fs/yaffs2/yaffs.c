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
** File name:               yaffs.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            yaffs 文件系统
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-27
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
#include "vfs/vfs.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "./src/yaffsfs.h"

static int __yaffs_mount(mount_point_t *point, device_t *dev, const char *dev_name, const char *param)
{
    return yaffs_StartUp();
}

static int __yaffs_open(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode)
{
    file->ctx = (void *)yaffs_open(vfs_path_add_mount_point(path), oflag, mode);
    if (file->ctx == (void *)-1) {
        return -1;
    } else {
        return 0;
    }
}

static int __yaffs_dup(mount_point_t *point, const file_t *src, file_t *dest)
{
    dest->ctx = (void *)yaffs_dup((int)src->ctx);
    if (dest->ctx == (void *)-1) {
        return -1;
    } else {
        return 0;
    }
}

static int __yaffs_close(mount_point_t *point, file_t *file)
{
    return yaffs_close((int)file->ctx);
}

static ssize_t __yaffs_read(mount_point_t *point, file_t *file, void *buf, size_t len)
{
    return yaffs_read((int)file->ctx, buf, len);
}

static ssize_t __yaffs_write(mount_point_t *point, file_t *file, const void *buf, size_t len)
{
    return yaffs_write((int)file->ctx, buf, len);
}

static int __yaffs_ioctl(mount_point_t *point, file_t *file, int cmd, void *arg)
{
    seterrno(ENOSYS);
    return -1;
}

static int __yaffs_fcntl(mount_point_t *point, file_t *file, int cmd, int arg)
{    switch (cmd) {
    case F_GETFL:
        return file->flags;

    case F_SETFL:
        if ((!(file->flags & FWRITE)) && (arg & FWRITE)) {
            seterrno(EINVAL);
            return -1;
        }
        if (file->flags & VFS_FILE_TYPE_FILE) {
            arg        &= ~VFS_FILE_TYPE_DIR;
            file->flags = arg | VFS_FILE_TYPE_FILE;
        } else {
            arg        &= ~VFS_FILE_TYPE_FILE;
            file->flags = arg | VFS_FILE_TYPE_DIR;
        }
        return 0;

    default:
        seterrno(EINVAL);
        return -1;
    }
}

static int __yaffs_fstat(mount_point_t *point, file_t *file, struct stat *buf)
{
    struct yaffs_stat st;
    int ret;

    ret = yaffs_fstat((int)file->ctx, &st);
    if (ret == 0) {
        buf->st_dev         = st.st_dev;
        buf->st_ino         = st.st_ino;
        buf->st_mode        = st.st_mode;
        buf->st_nlink       = st.st_nlink;
        buf->st_uid         = st.st_uid;
        buf->st_gid         = st.st_gid;
        buf->st_rdev        = st.st_rdev;
        buf->st_size        = st.st_size;
        buf->st_atime       = st.yst_atime;
        buf->st_spare1      = 0;
        buf->st_mtime       = st.yst_mtime;
        buf->st_spare2      = 0;
        buf->st_ctime       = st.yst_ctime;
        buf->st_spare3      = 0;
        buf->st_blksize     = st.st_blksize;
        buf->st_blocks      = st.st_blocks;
        buf->st_spare4[0]   = 0;
        buf->st_spare4[1]   = 0;
    }
    return ret;
}

static int __yaffs_fsync(mount_point_t *point, file_t *file)
{
    return yaffs_fsync((int)file->ctx);
}

static int __yaffs_fdatasync(mount_point_t *point, file_t *file)
{
    return yaffs_fdatasync((int)file->ctx);
}

static int __yaffs_ftruncate(mount_point_t *point, file_t *file, off_t len)
{
    return yaffs_ftruncate((int)file->ctx, len);
}

static off_t __yaffs_lseek(mount_point_t *point, file_t *file, off_t offset, int whence)
{
    return yaffs_lseek((int)file->ctx, offset, whence);
}

static int __yaffs_scan(mount_point_t *point, file_t *file, int flags)
{
    int ret;

    ret = 0;
    if (flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if (flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    return ret;
}

static int __yaffs_stat(mount_point_t *point, const char *path, struct stat *buf)
{
    struct yaffs_stat st;
    int ret;

    ret = yaffs_stat(vfs_path_add_mount_point(path), &st);
    if (ret == 0) {
        buf->st_dev         = st.st_dev;
        buf->st_ino         = st.st_ino;
        buf->st_mode        = st.st_mode;
        buf->st_nlink       = st.st_nlink;
        buf->st_uid         = st.st_uid;
        buf->st_gid         = st.st_gid;
        buf->st_rdev        = st.st_rdev;
        buf->st_size        = st.st_size;
        buf->st_atime       = st.yst_atime;
        buf->st_spare1      = 0;
        buf->st_mtime       = st.yst_mtime;
        buf->st_spare2      = 0;
        buf->st_ctime       = st.yst_ctime;
        buf->st_spare3      = 0;
        buf->st_blksize     = st.st_blksize;
        buf->st_blocks      = st.st_blocks;
        buf->st_spare4[0]   = 0;
        buf->st_spare4[1]   = 0;
    }
    return ret;
}

static int __yaffs_access(mount_point_t *point, const char *path, int amode)
{
    return yaffs_access(vfs_path_add_mount_point(path), amode);
}

static int __yaffs_opendir(mount_point_t *point, file_t *file, const char *path)
{
    file->ctx = yaffs_opendir(vfs_path_add_mount_point(path));
    if (file->ctx == NULL) {
        return -1;
    } else {
        return 0;
    }
}

static struct dirent *__yaffs_readdir(mount_point_t *point, file_t *file)
{
    /*
     *  重要: 必须要调整 struct yaffs_dirent 结构成员位置后才能转换类型
     */
    return (struct dirent *)yaffs_readdir(file->ctx);
}

static int __yaffs_rewinddir(mount_point_t *point, file_t *file)
{
    yaffs_rewinddir(file->ctx);

    return 0;
}

static int __yaffs_seekdir(mount_point_t *point, file_t *file, long loc)
{
    seterrno(ENOSYS);
    return -1;
}

static long __yaffs_telldir(mount_point_t *point, file_t *file)
{
    seterrno(ENOSYS);
    return -1;
}

static int __yaffs_closedir(mount_point_t *point, file_t *file)
{
    return yaffs_closedir(file->ctx);
}

static int __yaffs_link(mount_point_t *point, const char *path1, const char *path2)
{
    return yaffs_link(vfs_path_add_mount_point(path1), vfs_path_add_mount_point(path2));
}

static int __yaffs_unlink(mount_point_t *point, const char *path)
{
    return yaffs_unlink(vfs_path_add_mount_point(path));
}

static int __yaffs_mkdir(mount_point_t *point, const char *path, mode_t mode)
{
    return yaffs_mkdir(vfs_path_add_mount_point(path), mode);
}

static int __yaffs_rmdir(mount_point_t *point, const char *path)
{
    return yaffs_rmdir(vfs_path_add_mount_point(path));
}

static int __yaffs_rename(mount_point_t *point, const char *old, const char *new)
{
    return yaffs_rename(vfs_path_add_mount_point(old), vfs_path_add_mount_point(new));
}

static int __yaffs_sync(mount_point_t *point)
{
    return yaffs_sync(point->name);
}

static int __yaffs_truncate(mount_point_t *point, const char *path, off_t len)
{
    return yaffs_truncate(vfs_path_add_mount_point(path), len);
}

static int __yaffs_mkfs(mount_point_t *point, const char *param)
{
    seterrno(ENOSYS);
    return -1;
}

file_system_t yaffs = {
        .name       = "yaffs",
        .mount      = __yaffs_mount,
        .stat       = __yaffs_stat,
        .access     = __yaffs_access,

        .link       = __yaffs_link,
        .unlink     = __yaffs_unlink,
        .mkdir      = __yaffs_mkdir,
        .rmdir      = __yaffs_rmdir,
        .rename     = __yaffs_rename,
        .sync       = __yaffs_sync,
        .truncate   = __yaffs_truncate,

        .open       = __yaffs_open,
        .dup        = __yaffs_dup,
        .read       = __yaffs_read,
        .write      = __yaffs_write,
        .ioctl      = __yaffs_ioctl,
        .close      = __yaffs_close,
        .fcntl      = __yaffs_fcntl,
        .fstat      = __yaffs_fstat,
        .fsync      = __yaffs_fsync,
        .fdatasync  = __yaffs_fdatasync,
        .ftruncate  = __yaffs_ftruncate,
        .lseek      = __yaffs_lseek,

        .opendir    = __yaffs_opendir,
        .readdir    = __yaffs_readdir,
        .rewinddir  = __yaffs_rewinddir,
        .seekdir    = __yaffs_seekdir,
        .telldir    = __yaffs_telldir,
        .closedir   = __yaffs_closedir,

        .scan       = __yaffs_scan,

        .mkfs       = __yaffs_mkfs,
};
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
