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

#ifdef __cplusplus
extern "C" {
#endif

#include <syslimits.h>
#include <sys/types.h>
#include "kern/ipc.h"
#include "kern/atomic.h"

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
 * �ļ��ɶ�, ��д, ����, ���� select
 */
#define VFS_FILE_READABLE   (1 << 0)
#define VFS_FILE_WRITEABLE  (1 << 1)
#define VFS_FILE_ERROR      (1 << 2)

/*
 * select �ڵ�
 */
typedef struct _select_node {
    struct _select_node    *prev;
    struct _select_node    *next;
    void                   *task;
    int                     select_type;
} select_node_t;

/*
 * select ��Ա
 */
typedef struct {
    atomic_t                flags;
    select_node_t           wait_list;
} select_struct_t;

#define VFS_DEVICE_MEMBERS          \
    select_struct_t         select

#define device_init(priv)           \
    select_init(&priv->select)

/*
 * ����
 */
struct driver {
    const char             *name;
    struct driver          *next;
    atomic_t                ref;
    void                   *module;

    /*
     * �����ӿ�
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
    off_t   (*lseek)(void *ctx, file_t *file, off_t offset, int whence);
    int     (*scan)(void *ctx, file_t *file, int flags);
    int     (*select)(select_struct_t *select, file_t *file, int flags);
    int     (*unselect)(select_struct_t *select, file_t *file, int flags);
    int     (*unlink)(void *ctx);

    /*
     * ��д��
     */
    ssize_t (*readblk )(void *ctx, file_t *file, size_t blk_no, size_t blk_cnt, void *buf);
    ssize_t (*writeblk)(void *ctx, file_t *file, size_t blk_no, size_t blk_cnt, const void *buf);
};

/*
 * �豸
 */
struct device {
    char                    name[NAME_MAX];
    unsigned int            key;
    driver_t               *drv;
    /*
     * һ���������Ա�����豸ʹ��, ctx ����ά���豸��Ϣ
     */
    void                   *ctx;
    struct device          *next;
    dev_t                   devno;
    atomic_t                ref;
};

#define file_to_dev(file)   ((device_t *)(file->ctx))
#define dev_ref(file)       (&(file_to_dev(file)->ref))

/*
 * �ļ�ϵͳ
 */
struct file_system {
    const char             *name;
    struct file_system     *next;
    atomic_t                ref;
    void                   *module;

    /*
     * �ļ�ϵͳ�ӿ�
     */
    int     (*init)(void);
    int     (*mount)(mount_point_t *point, device_t *dev, const char *dev_name, const char *param);
    int     (*unmount)(mount_point_t *point, const char *param);
    int     (*mkfs)(mount_point_t *point, const char *param);

    int     (*link)(mount_point_t *point, const char *path1, const char *path2);
    int     (*unlink)(mount_point_t *point, const char *path);
    int     (*mkdir)(mount_point_t *point, const char *path, mode_t mode);
    int     (*rmdir)(mount_point_t *point, const char *path);
    int     (*rename)(mount_point_t *point, const char *old, const char *_new);
    int     (*sync)(mount_point_t *point);
    int     (*truncate)(mount_point_t *point, const char *path, off_t len);

    int     (*stat)(mount_point_t *point, const char *path, struct stat *buf);
    int     (*access)(mount_point_t *point, const char *path, int amode);

    /*
     * �ļ��ӿ�
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
    off_t   (*lseek)(mount_point_t *point, file_t *file, off_t offset, int whence);
    int     (*scan)(mount_point_t *point, file_t *file, int flags);
    int     (*select)(mount_point_t *point, file_t *file, int flags);
    int     (*unselect)(mount_point_t *point, file_t *file, int flags);

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
    char                    name[NAME_MAX];
    file_system_t          *fs;
    device_t               *dev;
    atomic_t                ref;
    /*
     * һ���ļ�ϵͳ����ͬʱ������豸����, ctx ����ά���ļ�ϵͳ��Ϣ
     */
    void                   *ctx;
    struct mount_point     *next;
};

#if 0
#define _FOPEN      (-1)    /* from sys/file.h, kernel use only */
#define _FREAD      0x0001  /* read enabled */
#define _FWRITE     0x0002  /* write enabled */
#define _FAPPEND    0x0008  /* append (writes guaranteed at the end) */
#define _FMARK      0x0010  /* internal; mark during gc() */
#define _FDEFER     0x0020  /* internal; defer for next gc pass */
#define _FASYNC     0x0040  /* signal pgrp when data ready */
#define _FSHLOCK    0x0080  /* BSD flock() shared lock present */
#define _FEXLOCK    0x0100  /* BSD flock() exclusive lock present */
#define _FCREAT     0x0200  /* open with file create */
#define _FTRUNC     0x0400  /* open with truncation */
#define _FEXCL      0x0800  /* error on open if file exists */
#define _FNBIO      0x1000  /* non blocking I/O (sys5 style) */
#define _FSYNC      0x2000  /* do all writes synchronously */
#define _FNONBLOCK  0x4000  /* non blocking I/O (POSIX style) */
#define _FNDELAY    _FNONBLOCK  /* non blocking I/O (4.2 style) */
#define _FNOCTTY    0x8000  /* don't assign a ctty on this open */

#define O_RDONLY    0       /* +1 == FREAD */
#define O_WRONLY    1       /* +1 == FWRITE */
#define O_RDWR      2       /* +1 == FREAD|FWRITE */
#endif

/*
 * �ļ�����
 */
#define VFS_FILE_TYPE_FREE  (0)
#define VFS_FILE_TYPE_FILE  (1 << 0)
#define VFS_FILE_TYPE_SOCK  ((1 << 1) | VFS_FILE_TYPE_FILE)
#define VFS_FILE_TYPE_DIR   (1 << 7)

/*
 * �ļ�
 */
struct file {
    /*
     * һ���ļ�����ͬʱ����δ�, ctx ����ά���ļ�ʵ����Ϣ
     */
    void                   *ctx;
    mount_point_t          *point;
    unsigned char           type;
    int                     flags;
    mutex_t                 lock;
    atomic_t                ref;
};

/*
 * �ж�·���ǲ��Ǹ�Ŀ¼
 */
#define VFS_PATH_IS_ROOT(path)          \
                            (path[0] == '/' && path[1] == '\0')

/*
 * ���豸��������״̬
 */
#include <sys/ioctl.h>

#define BLKDEV_CMD_STATUS   _IO('b', '0')
#define BLKDEV_CMD_INIT     _IO('b', '1')
#define BLKDEV_CMD_SYNC     _IO('b', '2')
#define BLKDEV_CMD_SECT_NR  _IO('b', '3')
#define BLKDEV_CMD_SECT_SZ  _IO('b', '4')
#define BLKDEV_CMD_BLK_SZ   _IO('b', '5')
#define BLKDEV_CMD_ERASE    _IO('b', '6')
#define BLKDEV_CMD_INFO     _IO('b', '7')

#define BLKDEV_STA_NOINIT   1
#define BLKDEV_STA_NODISK   2
#define BLKDEV_STA_PROTECT  3

#ifdef __cplusplus
#endif

#endif                                                                  /*  VFS_TYPES_H_                */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
