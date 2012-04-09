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

static int fatfs_mount(mount_point_t *point, device_t *dev, const char *dev_name)
{
    FATFS *fs;

    if (dev_name == NULL) {
        return -1;
    }

    fs = (FATFS *)kmalloc(sizeof(FATFS));
    if (fs != NULL) {
        memset(fs, 0, sizeof(FATFS));
        if (f_mount(0, fs) != FR_OK) {
            kfree(fs);
            return -1;
        } else {
            return 0;
        }
    } else {
        return -1;
    }
}

static int fatfs_open(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode)
{
    privinfo_t *priv;

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        memset(priv, 0, sizeof(privinfo_t));
        file->ctx = priv;
        /*
         * TODO: oflag -> FATFS mode
         */
        if (f_open(&priv->file, path, FA_READ | FA_WRITE | FA_OPEN_ALWAYS) != FR_OK) {
            kfree(priv);
            return -1;
        } else {
            if (oflag & O_TRUNC) {
                f_truncate(&priv->file);
            }
            return 0;
        }
    } else {
        return -1;
    }
}

static int fatfs_close(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        return -1;
    }

    if (f_close(&priv->file) == FR_OK) {
        return 0;
    } else {
        return -1;
    }
}

static ssize_t fatfs_read(mount_point_t *point, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = file->ctx;
    UINT rlen;

    if (priv == NULL) {
        return -1;
    }

    if (f_read(&priv->file, buf, len, &rlen) == FR_OK) {
        return rlen;
    } else {
        return -1;
    }
}

static ssize_t fatfs_write(mount_point_t *point, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = file->ctx;
    UINT wlen;

    if (priv == NULL) {
        return -1;
    }

    if (file->flag & O_APPEND) {
        f_lseek(&priv->file, f_size(&priv->file));
    }

    if (f_write(&priv->file, buf, len, &wlen) == FR_OK) {
        return wlen;
    } else {
        return -1;
    }
}

static int fatfs_ioctl(mount_point_t *point, file_t *file, int cmd, void *arg)
{
    return -1;
}

static int fatfs_fcntl(mount_point_t *point, file_t *file, int cmd, int arg)
{
    return -1;
}

static int fatfs_fstat(mount_point_t *point, file_t *file, struct stat *buf)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        return -1;
    }

    buf->st_dev     = (dev_t)point->dev->devno;
    buf->st_ino     = 0;
    buf->st_mode    = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IFREG;
    buf->st_nlink   = 0;
    buf->st_uid     = 0;
    buf->st_gid     = 0;
    buf->st_rdev    = (dev_t)point->dev->devno;
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

    buf->st_size    = f_size(&priv->file);
    buf->st_ino     = priv->file.sclust;
#if _MAX_SS != 512
    buf->st_blksize = priv->file.fs->ssize;
#else
    buf->st_blksize = _MAX_SS;
#endif
    buf->st_blocks  = f_size(&priv->file) / buf->st_blksize;

    return 0;
}

static int fatfs_fsync(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        return -1;
    }

    if (f_sync(&priv->file) == FR_OK) {
        return 0;
    } else {
        return -1;
    }
}

static int fatfs_fdatasync(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        return -1;
    }

    if (f_sync(&priv->file) == FR_OK) {
        return 0;
    } else {
        return -1;
    }
}

static int fatfs_ftruncate(mount_point_t *point, file_t *file, off_t len)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        return -1;
    }

    if (len == 0) {
        if (f_truncate(&priv->file) == FR_OK) {
            return 0;
        } else {
            return -1;
        }
    } else {
        if (len > priv->file.fsize) {
            DWORD cur = f_tell(&priv->file);

            f_lseek(&priv->file, len);

            f_lseek(&priv->file, cur);
        }
        return 0;
    }
}

static int fatfs_lseek(mount_point_t *point, file_t *file, off_t offset, int whence)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        return -1;
    }

    switch (whence) {
    case SEEK_SET:
        f_lseek(&priv->file, offset);
        break;

    case SEEK_CUR:
        f_lseek(&priv->file, f_tell(&priv->file) + offset);
        break;

    case SEEK_END:
        f_lseek(&priv->file, f_size(&priv->file) + offset);
        break;

    default:
        return -1;
    }
    return 0;
}

static int fatfs_stat(mount_point_t *point, const char *path, struct stat *buf)
{
    if (PATH_IS_ROOT_DIR(path)) {
        buf->st_dev     = (dev_t)point->dev->devno;
        buf->st_ino     = 0;
        buf->st_mode    = S_IRWXU | S_IRWXG | S_IRWXO | S_IFDIR;
        buf->st_nlink   = 0;
        buf->st_uid     = 0;
        buf->st_gid     = 0;
        buf->st_rdev    = (dev_t)point->dev->devno;
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
        FILINFO info;

        if (f_stat(path, &info) == FR_OK) {
            buf->st_dev     = (dev_t)point->dev->devno;
            buf->st_ino     = 0;
            if (info.fattrib & AM_DIR) {
                buf->st_mode = S_IRWXU | S_IRWXG | S_IRWXO | S_IFDIR;
            } else {
                buf->st_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IFREG;
            }

            if (info.fattrib & AM_RDO) {
                buf->st_mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
            }

            buf->st_nlink   = 0;
            buf->st_uid     = 0;
            buf->st_gid     = 0;
            buf->st_rdev    = (dev_t)point->dev->devno;
            buf->st_size    = info.fsize;
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
            return -1;
        }
    } else {
        return ret;
    }
}

static int fatfs_opendir(mount_point_t *point, file_t *file, const char *path)
{
    privinfo_t *priv;

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        memset(priv, 0, sizeof(privinfo_t));
        file->ctx = priv;
        if (f_opendir(&priv->dir, path) == FR_OK) {
            return 0;
        } else {
            kfree(priv);
            return -1;
        }
    } else {
        return -1;
    }
}

static struct dirent *fatfs_readdir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL) {
        FILINFO info;
        WCHAR lbuf[_MAX_LFN + 1];

        info.lfname = (TCHAR *)lbuf;
        info.lfsize = sizeof(lbuf);

        if (f_readdir(&priv->dir, &info) == FR_OK && info.fname[0] != '\0') {
            if (info.lfname[0] != '\0') {
                strlcpy(priv->entry.d_name, info.lfname, sizeof(priv->entry.d_name));
            } else {
                strlcpy(priv->entry.d_name, info.fname, sizeof(priv->entry.d_name));
            }

            priv->entry.d_ino = 0;
            return &priv->entry;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

static int fatfs_rewinddir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL) {
        if (f_readdir(&priv->dir, NULL) == FR_OK) {
            return 0;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

static int fatfs_seekdir(mount_point_t *point, file_t *file, long loc)
{
    return -1;
}

static long fatfs_telldir(mount_point_t *point, file_t *file)
{
    return -1;
}

static int fatfs_closedir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv != NULL) {
        kfree(priv);
        return 0;
    } else {
        return -1;
    }
}

static int fatfs_link(mount_point_t *point, const char *path1, const char *path2)
{
    return -1;
}

static int fatfs_unlink(mount_point_t *point, const char *path)
{
    if (f_unlink(path) == FR_OK) {
        return 0;
    } else {
        return -1;
    }
}

static int fatfs_mkdir(mount_point_t *point, const char *path, mode_t mode)
{
    if (f_mkdir(path) == FR_OK) {
        return 0;
    } else {
        return -1;
    }
}

static int fatfs_rmdir(mount_point_t *point, const char *path)
{
    if (f_unlink(path) == FR_OK) {
        return 0;
    } else {
        return -1;
    }
}

static int fatfs_rename(mount_point_t *point, const char *old, const char *new)
{
    if (f_rename(old, new) == FR_OK) {
        return 0;
    } else {
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
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
