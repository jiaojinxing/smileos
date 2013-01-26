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
** File name:               fatfs.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            FAT 文件系统
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
#include "ff.h"

/*
 * 私有信息
 */
typedef struct {
    union {
        DIR         dir;
        FIL         file;
    };
    struct dirent   entry;
} privinfo_t;

static void fatfs_result_to_errno(FRESULT result)
{
    switch (result) {
    case FR_OK:
        seterrno(0);
        break;

    case FR_NO_FILE:
    case FR_NO_PATH:
    case FR_NO_FILESYSTEM:
        seterrno(ENOENT);
        break;

    case FR_INVALID_NAME:
        seterrno(EINVAL);
        break;

    case FR_EXIST:
    case FR_INVALID_OBJECT:
        seterrno(EEXIST);
        break;

    case FR_DISK_ERR:
    case FR_NOT_READY:
    case FR_INT_ERR:
        seterrno(EIO);
        break;

    case FR_WRITE_PROTECTED:
    case FR_DENIED:
        seterrno(EROFS);
        break;

    case FR_MKFS_ABORTED:
        seterrno(EINVAL);
        break;

    default:
        seterrno(-1);
        break;
    }
}

static int fatfs_mount(mount_point_t *point, device_t *dev, const char *dev_name, const char *param)
{
    FATFS *fs;
    FRESULT res;

    if (dev_name == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    fs = (FATFS *)kmalloc(sizeof(FATFS), GFP_KERNEL);
    if (fs != NULL) {
        memset(fs, 0, sizeof(FATFS));
        res = f_mount(0, fs);
        if (res != FR_OK) {
            fatfs_result_to_errno(res);
            kfree(fs);
            return -1;
        } else {
            point->ctx = fs;
            fs->drv    = dev;
            return 0;
        }
    } else {
        seterrno(ENOMEM);
        return -1;
    }
}

static int fatfs_open(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode)
{
    privinfo_t *priv;
    int fatfs_mode = FA_READ;
    FRESULT res;

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        memset(priv, 0, sizeof(privinfo_t));
        file->ctx = priv;
        /*
         * oflag -> FATFS mode
         */
        if (oflag & O_WRONLY) {
            fatfs_mode |= FA_WRITE;
        }

        if ((oflag & O_ACCMODE) & O_RDWR) {
            fatfs_mode |= FA_WRITE;
        }

        if (oflag & O_CREAT) {
            fatfs_mode |= FA_OPEN_ALWAYS;
        }

        if (oflag & O_TRUNC) {
            fatfs_mode |= FA_CREATE_ALWAYS;
        }

        if (oflag & O_EXCL) {
            fatfs_mode |= FA_CREATE_NEW;
        }

        res = f_open(point->ctx, &priv->file, path, fatfs_mode);
        if (res != FR_OK) {
            fatfs_result_to_errno(res);
            kfree(priv);
            return -1;
        } else {
            return 0;
        }
    } else {
        seterrno(ENOMEM);
        return -1;
    }
}

static int fatfs_dup(mount_point_t *point, const file_t *src, file_t *dest)
{
    privinfo_t *priv;

    if (src->flags & FWRITE) {
        seterrno(EACCES);
        return -1;
    }

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        dest->ctx = priv;
        memcpy(dest->ctx, src->ctx, sizeof(privinfo_t));
        extern int fatfs_file_sem_lock(FATFS *fs, int id);
        fatfs_file_sem_lock(point->ctx, priv->file.lockid);
        return 0;
    } else {
        seterrno(ENOMEM);
        return -1;
    }
}

static int fatfs_close(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;
    FRESULT res;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    res = f_close(&priv->file);
    if (res == FR_OK) {
        kfree(priv);
        file->ctx = NULL;
        return 0;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

static ssize_t fatfs_read(mount_point_t *point, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = file->ctx;
    UINT rlen;
    FRESULT res;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    res = f_read(&priv->file, buf, len, &rlen);
    if (res == FR_OK) {
        return rlen;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

static ssize_t fatfs_write(mount_point_t *point, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = file->ctx;
    UINT wlen;
    FRESULT res;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (file->flags & O_APPEND && (f_tell(&priv->file) < f_size(&priv->file))) {
        f_lseek(&priv->file, f_size(&priv->file));
    }

    res = f_write(&priv->file, buf, len, &wlen);
    if (res == FR_OK) {
        return wlen;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

static int fatfs_ioctl(mount_point_t *point, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    seterrno(ENOSYS);
    return -1;
}

static int fatfs_fcntl(mount_point_t *point, file_t *file, int cmd, int arg)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    switch (cmd) {
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

static int fatfs_fstat(mount_point_t *point, file_t *file, struct stat *buf)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    buf->st_dev         = (dev_t)point->dev->devno;
    buf->st_ino         = 0;
    buf->st_mode        = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IFREG;
    buf->st_nlink       = 0;
    buf->st_uid         = 0;
    buf->st_gid         = 0;
    buf->st_rdev        = (dev_t)point->dev->devno;
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

    buf->st_size        = f_size(&priv->file);
    buf->st_ino         = priv->file.sclust;
#if _MAX_SS != 512
    buf->st_blksize     = priv->file.fs->ssize;
#else
    buf->st_blksize     = _MAX_SS;
#endif
    buf->st_blocks      = f_size(&priv->file) / buf->st_blksize;

    return 0;
}

static int fatfs_fsync(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;
    FRESULT res;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    res = f_sync(&priv->file);
    if (res == FR_OK) {
        return 0;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

static int fatfs_fdatasync(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;
    FRESULT res;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    res = f_sync(&priv->file);
    if (res == FR_OK) {
        return 0;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

static int fatfs_ftruncate(mount_point_t *point, file_t *file, off_t len)
{
    privinfo_t *priv = file->ctx;
    FRESULT res;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    res = f_lseek(&priv->file, len);
    if (res != FR_OK) {
        fatfs_result_to_errno(res);
        return -1;
    }

    res = f_truncate(&priv->file);
    if (res == FR_OK) {
        return 0;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

static off_t fatfs_lseek(mount_point_t *point, file_t *file, off_t offset, int whence)
{
    privinfo_t *priv = file->ctx;
    FRESULT res;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    switch (whence) {
    case SEEK_SET:
        res = f_lseek(&priv->file, offset);
        if (res == FR_OK) {
            return f_tell(&priv->file);
        } else {
            fatfs_result_to_errno(res);
            return -1;
        }

    case SEEK_CUR:
        res = f_lseek(&priv->file, f_tell(&priv->file) + offset);
        if (res == FR_OK) {
            return f_tell(&priv->file);
        } else {
            fatfs_result_to_errno(res);
            return -1;
        }

    case SEEK_END:
        res = f_lseek(&priv->file, f_size(&priv->file) + offset);
        if (res == FR_OK) {
            return f_tell(&priv->file);
        } else {
            fatfs_result_to_errno(res);
            return -1;
        }

    default:
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

static int fatfs_scan(mount_point_t *point, file_t *file, int flags)
{
    privinfo_t *priv = file->ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if (flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if (flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    return ret;
}

static int fatfs_stat(mount_point_t *point, const char *path, struct stat *buf)
{
    FRESULT res;

    if (VFS_PATH_IS_ROOT(path)) {
        buf->st_dev         = (dev_t)point->dev->devno;
        buf->st_ino         = 0;
        buf->st_mode        = S_IRWXU | S_IRWXG | S_IRWXO | S_IFDIR;
        buf->st_nlink       = 0;
        buf->st_uid         = 0;
        buf->st_gid         = 0;
        buf->st_rdev        = (dev_t)point->dev->devno;
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
        FILINFO info;

#if _USE_LFN
        info.lfname = NULL;                                             /*  必须要这样做, 否则出错      */
        info.lfsize = 0;
#endif
        res = f_stat(point->ctx, path, &info);
        if (res == FR_OK) {
            buf->st_dev         = (dev_t)point->dev->devno;
            buf->st_ino         = 0;
            if (info.fattrib & AM_DIR) {
                buf->st_mode    = S_IRWXU | S_IRWXG | S_IRWXO | S_IFDIR;
            } else {
                buf->st_mode    = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IFREG;
            }

            if (info.fattrib & AM_RDO) {
                buf->st_mode   &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
            }

            buf->st_nlink       = 0;
            buf->st_uid         = 0;
            buf->st_gid         = 0;
            buf->st_rdev        = (dev_t)point->dev->devno;
            buf->st_size        = info.fsize;
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
            fatfs_result_to_errno(res);
            return -1;
        }
    }
}

static int fatfs_access(mount_point_t *point, const char *path, int amode)
{
    struct stat buf;
    int ret;

    ret = fatfs_stat(point, path, &buf);
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

static int fatfs_opendir(mount_point_t *point, file_t *file, const char *path)
{
    privinfo_t *priv;
    FRESULT res;

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        memset(priv, 0, sizeof(privinfo_t));
        file->ctx = priv;
        res = f_opendir(point->ctx, &priv->dir, path);
        if (res == FR_OK) {
            return 0;
        } else {
            fatfs_result_to_errno(res);
            kfree(priv);
            return -1;
        }
    } else {
        seterrno(ENOMEM);
        return -1;
    }
}

static struct dirent *fatfs_readdir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;
    FRESULT res;

    if (priv != NULL) {
        FILINFO info;

        priv->entry.d_name[0] = '\0';

#if _USE_LFN
        info.lfname = priv->entry.d_name;
        info.lfsize = sizeof(priv->entry.d_name);
#endif

        res = f_readdir(&priv->dir, &info);
        if (res == FR_OK && info.fname[0] != '\0') {
#if _USE_LFN
            if (info.lfname[0] == '\0') {
                strlcpy(priv->entry.d_name, info.fname, sizeof(priv->entry.d_name));
            }
#else
            strlcpy(priv->entry.d_name, info.fname, sizeof(priv->entry.d_name));
#endif
            priv->entry.d_ino = 0;
            return &priv->entry;
        } else {
            fatfs_result_to_errno(res);
            return NULL;
        }
    } else {
        seterrno(EINVAL);
        return NULL;
    }
}

static int fatfs_rewinddir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;
    FRESULT res;

    if (priv != NULL) {
        res = f_readdir(&priv->dir, NULL);
        if (res == FR_OK) {
            return 0;
        } else {
            fatfs_result_to_errno(res);
            return -1;
        }
    } else {
        seterrno(EINVAL);
        return -1;
    }
}

static int fatfs_seekdir(mount_point_t *point, file_t *file, long loc)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    seterrno(ENOSYS);
    return -1;
}

static long fatfs_telldir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    seterrno(ENOSYS);
    return -1;
}

static int fatfs_closedir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL) {
        kfree(priv);
        file->ctx = NULL;
        return 0;
    } else {
        seterrno(EINVAL);
        return -1;
    }
}

static int fatfs_link(mount_point_t *point, const char *path1, const char *path2)
{
    seterrno(ENOSYS);
    return -1;
}

static int fatfs_unlink(mount_point_t *point, const char *path)
{
    FRESULT res;

    res = f_unlink(point->ctx, path);
    if (res == FR_OK) {
        return 0;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

static int fatfs_mkdir(mount_point_t *point, const char *path, mode_t mode)
{
    FRESULT res;

    res = f_mkdir(point->ctx, path);
    if (res == FR_OK) {
        return 0;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

static int fatfs_rmdir(mount_point_t *point, const char *path)
{
    FRESULT res;

    res = f_unlink(point->ctx, path);
    if (res == FR_OK) {
        return 0;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

static int fatfs_rename(mount_point_t *point, const char *old, const char *new)
{
    FRESULT res;

    res = f_rename(point->ctx, old, new);
    if (res == FR_OK) {
        return 0;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

static int fatfs_sync(mount_point_t *point)
{
    return 0;
}

static int fatfs_truncate(mount_point_t *point, const char *path, off_t len)
{
    int fd = vfs_open(vfs_path_add_mount_point(path), O_WRONLY, 0666);
    if (fd >= 0) {
        int ret = vfs_ftruncate(fd, len);
        vfs_close(fd);
        return ret;
    } else {
        return -1;
    }
}

static int fatfs_mkfs(mount_point_t *point, const char *param)
{
    FRESULT res;

    res = f_mkfs(0, point->ctx, 1, 0);
    if (res == FR_OK) {
        return 0;
    } else {
        fatfs_result_to_errno(res);
        return -1;
    }
}

file_system_t fatfs = {
        .name       = "fatfs",
        .mount      = fatfs_mount,
        .stat       = fatfs_stat,
        .access     = fatfs_access,

        .link       = fatfs_link,
        .unlink     = fatfs_unlink,
        .mkdir      = fatfs_mkdir,
        .rmdir      = fatfs_rmdir,
        .rename     = fatfs_rename,
        .sync       = fatfs_sync,
        .truncate   = fatfs_truncate,

        .open       = fatfs_open,
        .dup        = fatfs_dup,
        .read       = fatfs_read,
        .write      = fatfs_write,
        .ioctl      = fatfs_ioctl,
        .close      = fatfs_close,
        .fcntl      = fatfs_fcntl,
        .fstat      = fatfs_fstat,
        .fsync      = fatfs_fsync,
        .fdatasync  = fatfs_fdatasync,
        .ftruncate  = fatfs_ftruncate,
        .lseek      = fatfs_lseek,

        .opendir    = fatfs_opendir,
        .readdir    = fatfs_readdir,
        .rewinddir  = fatfs_rewinddir,
        .seekdir    = fatfs_seekdir,
        .telldir    = fatfs_telldir,
        .closedir   = fatfs_closedir,

        .scan       = fatfs_scan,

        .mkfs       = fatfs_mkfs,
};
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
