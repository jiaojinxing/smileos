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
** File name:               types.h
** Last modified Date:      2012-3-24
** Last Version:            1.0.0
** Descriptions:            虚拟文件系统数据类型定义
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-24
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
#ifndef VFS_TYPES_H_
#define VFS_TYPES_H_

#include "vfs/config.h"
#include <sys/types.h>

struct stat;
struct dirent;

struct mutex;

struct file;
struct driver;
struct device;
struct file_system;
struct mount_point;
struct _DIR;

typedef struct _DIR         DIR;
typedef struct file         file_t;
typedef struct driver       driver_t;
typedef struct device       device_t;
typedef struct file_system  file_system_t;
typedef struct mount_point  mount_point_t;

/*
 * 驱动
 */
struct driver {
    const char             *name;
    struct driver          *next;

    /*
     * 驱动接口
     */
    int     (*open)(void *ctx, file_t *file, int oflag, mode_t mode);
    ssize_t (*read)(void *ctx, file_t *file, void *buf, size_t len);
    ssize_t (*write)(void *ctx, file_t *file, const void *buf, size_t len);
    int     (*ioctl)(void *ctx, file_t *file, int cmd, void *arg);
    int     (*close)(void *ctx, file_t *file);
    int     (*fcntl)(void *ctx, file_t *file, int cmd, int arg);
    int     (*fstat)(void *ctx, file_t *file, struct stat *buf);
    int     (*isatty)(void *ctx, file_t *file);
    int     (*fsync)(void *ctx, file_t *file);
    int     (*fdatasync)(void *ctx, file_t *file);
    int     (*ftruncate)(void *ctx, file_t *file, off_t len);
    int     (*lseek)(void *ctx, file_t *file, off_t offset, int whence);
    int     (*scan)(mount_point_t *point, file_t *file, int flags);
    int     (*select)(mount_point_t *point, file_t *file, int flags);
    int     (*unselect)(mount_point_t *point, file_t *file, int flags);

    /*
     * 读写块
     */
    int     (*readb )(void *ctx, file_t *file, size_t blk_nr, size_t blk_cnt, void *buf);
    int     (*writeb)(void *ctx, file_t *file, size_t blk_nr, size_t blk_cnt, const void *buf);
};

/*
 * 设备
 */
struct device {
    char                    name[NAME_MAX];
    driver_t               *drv;
    /*
     * 一个驱动可以被多个设备使用, ctx 用于维护设备信息
     */
    void                   *ctx;
    struct device          *next;
    dev_t                   devno;
};

/*
 * 文件系统
 */
struct file_system {
    const char             *name;
    struct file_system     *next;

    /*
     * 文件系统接口
     */
    int     (*mount)(mount_point_t *point, device_t *dev, const char *dev_name);
    int     (*unmount)(mount_point_t *point);
    int     (*mkfs)(mount_point_t *point, const char *param);

    int     (*link)(mount_point_t *point, const char *path1, const char *path2);
    int     (*unlink)(mount_point_t *point, const char *path);
    int     (*mkdir)(mount_point_t *point, const char *path, mode_t mode);
    int     (*rmdir)(mount_point_t *point, const char *path);
    int     (*rename)(mount_point_t *point, const char *old, const char *new);
    int     (*sync)(mount_point_t *point);
    int     (*truncate)(mount_point_t *point, const char *path, off_t len);

    int     (*stat)(mount_point_t *point, const char *path, struct stat *buf);
    int     (*access)(mount_point_t *point, const char *path, int amode);

    /*
     * 文件接口
     */
    int     (*open)(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode);
    ssize_t (*read)(mount_point_t *point, file_t *file, void *buf, size_t len);
    ssize_t (*write)(mount_point_t *point, file_t *file, const void *buf, size_t len);
    int     (*ioctl)(mount_point_t *point, file_t *file, int cmd, void *arg);
    int     (*close)(mount_point_t *point, file_t *file);
    int     (*fcntl)(mount_point_t *point, file_t *file, int cmd, int arg);
    int     (*fstat)(mount_point_t *point, file_t *file, struct stat *buf);
    int     (*isatty)(mount_point_t *point, file_t *file);
    int     (*fsync)(mount_point_t *point, file_t *file);
    int     (*fdatasync)(mount_point_t *point, file_t *file);
    int     (*ftruncate)(mount_point_t *point, file_t *file, off_t len);
    int     (*lseek)(mount_point_t *point, file_t *file, off_t offset, int whence);
    int     (*scan)(mount_point_t *point, file_t *file, int flags);
    int     (*select)(mount_point_t *point, file_t *file, int flags);
    int     (*unselect)(mount_point_t *point, file_t *file, int flags);

    /*
     * 目录接口
     */
    int     (*opendir)(mount_point_t *point, file_t *file, const char *path);
    struct dirent *(*readdir)(mount_point_t *point, file_t *file);
    int     (*rewinddir)(mount_point_t *point, file_t *file);
    int     (*seekdir)(mount_point_t *point, file_t *file, long loc);
    long    (*telldir)(mount_point_t *point, file_t *file);
    int     (*closedir)(mount_point_t *point, file_t *file);
};

/*
 * 挂载点
 */
struct mount_point {
    char                    name[NAME_MAX];
    file_system_t          *fs;
    device_t               *dev;
    /*
     * 一个文件系统可以同时被多个设备挂载, ctx 用于维护文件系统信息
     */
    void                   *ctx;
    struct mount_point     *next;
};

/*
 * 文件
 */
struct file {
    /*
     * 一个文件可以同时被多次打开, ctx 用于维护文件实例信息
     */
    void                   *ctx;
    mount_point_t          *point;
    unsigned int            flag;
    struct mutex      *lock;
};

/*
 * 判断路径是不是根目录
 */
#define PATH_IS_ROOT_DIR(path)  (path[0] == '/' && path[1] == '\0')

#define VFS_FILE_READBLE        (1 << 0)
#define VFS_FILE_WRITEBLE       (1 << 1)
#define VFS_FILE_ERROR          (1 << 2)

/*
 * select 节点
 */
typedef struct _select_node {
    void                   *task;
    int                     select_type;
    struct _select_node    *next;
} select_node_t;

#endif                                                                  /*  VFS_TYPES_H_                */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
