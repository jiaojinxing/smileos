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
** Descriptions:            �����ļ�ϵͳ�������Ͷ���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-24
** Version:                 1.0.0
** Descriptions:            �����ļ�
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

struct kern_mutex;

struct file;
struct driver;
struct device;
struct file_system;
struct mount_point;

typedef struct file         file_t;
typedef struct driver       driver_t;
typedef struct device       device_t;
typedef struct file_system  file_system_t;
typedef struct mount_point  mount_point_t;

/*
 * ����
 */
struct driver {
    const char             *name;
    struct driver          *next;

    /*
     * �����ӿ�
     */
    int     (*open)(void *ctx, file_t *file, int oflag, mode_t mode);
    ssize_t (*read)(void *ctx, file_t *file, void *buf, size_t len);
    ssize_t (*write)(void *ctx, file_t *file, const void *buf, size_t len);
    int     (*ioctl)(void *ctx, file_t *file, int cmd, void *arg);
    int     (*close)(void *ctx, file_t *file);
    int     (*fcntl)(void *ctx, file_t *file, int cmd, void *arg);
    int     (*fstat)(void *ctx, file_t *file, struct stat *buf);
    int     (*isatty)(void *ctx, file_t *file);
    int     (*fsync)(void *ctx, file_t *file);
    int     (*ftruncate)(void *ctx, file_t *file, off_t len);
    int     (*lseek)(void *ctx, file_t *file, off_t offset, int whence);
};

/*
 * �豸
 */
struct device {
    char                    name[NAME_MAX + 1];
    driver_t               *drv;
    /*
     * һ���������Ա�����豸ʹ��, ctx ����ά���豸��Ϣ
     */
    void                   *ctx;
    struct device          *next;
};

/*
 * �ļ�ϵͳ
 */
struct file_system {
    const char             *name;
    struct file_system     *next;

    /*
     * �ļ�ϵͳ�ӿ�
     */
    int     (*mount)(mount_point_t *point, device_t *dev, const char *dev_name);
    int     (*link)(mount_point_t *point, const char *path1, const char *path2);
    int     (*unlink)(mount_point_t *point, const char *path);
    int     (*mkdir)(mount_point_t *point, const char *path, mode_t mode);
    int     (*rmdir)(mount_point_t *point, const char *path);
    int     (*rename)(mount_point_t *point, const char *old, const char *new);
    int     (*stat)(mount_point_t *point, const char *path, struct stat *buf);
    int     (*access)(mount_point_t *point, const char *path, mode_t mode);
    int     (*sync)(mount_point_t *point);
    int     (*truncate)(mount_point_t *point, const char *path, off_t len);

    /*
     * �ļ��ӿ�
     */
    int     (*open)(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode);
    ssize_t (*read)(mount_point_t *point, file_t *file, void *buf, size_t len);
    ssize_t (*write)(mount_point_t *point, file_t *file, const void *buf, size_t len);
    int     (*ioctl)(mount_point_t *point, file_t *file, int cmd, void *arg);
    int     (*close)(mount_point_t *point, file_t *file);
    int     (*fcntl)(mount_point_t *point, file_t *file, int cmd, void *arg);
    int     (*fstat)(mount_point_t *point, file_t *file, struct stat *buf);
    int     (*isatty)(mount_point_t *point, file_t *file);
    int     (*fsync)(mount_point_t *point, file_t *file);
    int     (*fdatasync)(mount_point_t *point, file_t *file);
    int     (*ftruncate)(mount_point_t *point, file_t *file, off_t len);
    int     (*lseek)(mount_point_t *point, file_t *file, off_t offset, int whence);

    /*
     * Ŀ¼�ӿ�
     */
    int     (*opendir)(mount_point_t *point, file_t *file, const char *path);
    struct dirent *(*readdir)(mount_point_t *point, file_t *file);
    int     (*rewinddir)(mount_point_t *point, file_t *file);
    int     (*seekdir)(mount_point_t *point, file_t *file, long loc);
    long    (*telldir)(mount_point_t *point, file_t *file);
    int     (*closedir)(mount_point_t *point, file_t *file);
};

/*
 * ���ص�
 */
struct mount_point {
    char                    name[NAME_MAX + 1];
    file_system_t          *fs;
    device_t               *dev;
    /*
     * һ���ļ�ϵͳ����ͬʱ������豸����, ctx ����ά���ļ�ϵͳ��Ϣ
     */
    void                   *ctx;
    struct mount_point     *next;
};

/*
 * �ļ�
 */
struct file {
    /*
     * һ���ļ�����ͬʱ����δ�, ctx ����ά���ļ�ʵ����Ϣ
     */
    void                   *ctx;
    mount_point_t          *point;
    unsigned int            flag;
    struct kern_mutex      *lock;
};

#endif                                                                  /*  VFS_TYPES_H_                */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/