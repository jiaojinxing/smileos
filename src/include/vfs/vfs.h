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
** Descriptions:            �����ļ�ϵͳ
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-20
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
#ifndef VFS_H_
#define VFS_H_

#include <sys/types.h>

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
    int     (*open)(void *ctx, int oflag, mode_t mode);
    ssize_t (*read)(void *ctx, void *buf, size_t len);
    ssize_t (*write)(void *ctx, const void *buf, size_t len);
    int     (*ioctl)(void *ctx, int cmd, void *arg);
    int     (*close)(void *ctx);
};

/*
 * �豸
 */
struct device {
    char                    name[NAME_MAX];
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
    int     (*mount)(mount_point_t *point);
    int     (*open)(file_t *file, const char *path, int oflag, mode_t mode);
    ssize_t (*read)(file_t *file, void *buf, size_t len);
    ssize_t (*write)(file_t *file, const void *buf, size_t len);
    int     (*ioctl)(file_t *file, int cmd, void *arg);
    int     (*close)(file_t *file);
};

/*
 * ���ص�
 */
struct mount_point {
    char                    name[NAME_MAX];
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
    uint8_t                 used;
};

#endif                                                                  /*  VFS_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
