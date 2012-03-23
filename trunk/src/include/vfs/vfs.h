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
** File name:               vfs.h
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
#ifndef VFS_H_
#define VFS_H_

#include "kern/config.h"
#include "kern/types.h"
#include "kern/ipc.h"
#include <sys/types.h>

struct stat;
struct dirent;
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
};

/*
 * 文件系统
 */
struct file_system {
    const char             *name;
    struct file_system     *next;

    /*
     * 文件接口
     */
    int     (*mount)(mount_point_t *point, device_t *dev, const char *dev_name);
    int     (*open)(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode);
    ssize_t (*read)(mount_point_t *point, file_t *file, void *buf, size_t len);
    ssize_t (*write)(mount_point_t *point, file_t *file, const void *buf, size_t len);
    int     (*ioctl)(mount_point_t *point, file_t *file, int cmd, void *arg);
    int     (*close)(mount_point_t *point, file_t *file);
    int     (*fcntl)(mount_point_t *point, file_t *file, int cmd, void *arg);
    int     (*fstat)(mount_point_t *point, file_t *file, struct stat *buf);
    int     (*isatty)(mount_point_t *point, file_t *file);
    int     (*lseek)(mount_point_t *point, file_t *file, off_t offset, int whence);

    /*
     * 文件系统接口
     */
    int     (*link)(mount_point_t *point, const char *path1, const char *path2);
    int     (*mkdir)(mount_point_t *point, const char *path, mode_t mode);
    int     (*rename)(mount_point_t *point, const char *old, const char *new);
    int     (*stat)(mount_point_t *point, const char *path, struct stat *buf);
    int     (*unlink)(mount_point_t *point, const char *path);

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
    uint8_t                 used;
    kern_mutex_t            lock;
};


#endif                                                                  /*  VFS_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
