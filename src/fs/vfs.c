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
** File name:               vfs.c
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "vfs/vfs.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/mount.h"
#include <string.h>
#include <stdio.h>

/*
 * 进程文件信息
 */
typedef struct {
    file_t              files[OPEN_MAX];                                /*  文件结构表                  */
    char                cwd[PATH_MAX];                                  /*  当前工作目录                */
    kern_mutex_t        cwd_lock;                                       /*  当前工作目录锁              */
} process_file_info_t;

process_file_info_t process_file_info[PROCESS_NR];

/*
 * 打开文件
 */
int vfs_open(const char *path, int oflag, mode_t mode)
{
    mount_point_t *point;
    char           file_path[PATH_MAX];
    char          *tmp;

    if (path == NULL) {                                                 /*  PATH 合法性检查             */
        return -1;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        if (path[1] == '\0') {                                          /*  不能打开根目录              */
            return -1;
        }
        strcpy(file_path, path);
    } else {                                                            /*  如果是相对路径              */
        /*
         * cwd 要以 / 号开头和结尾
         */
        kern_mutex_lock(&process_file_info[current->pid].cwd_lock, 0);  /*  在前面加入当前工作目录      */
        sprintf(file_path, "%s%s", process_file_info[current->pid].cwd, path);
        kern_mutex_unlock(&process_file_info[current->pid].cwd_lock);
    }

    tmp = strchr(file_path + 1, '/');                                   /*  查找挂载点名后的 / 号       */
    if (tmp == NULL) {                                                  /*  没有到                      */
        return -1;
    }
    if (tmp[1] == '0') {                                                /*  不能打开挂载点              */
        return -1;
    }

    *tmp = '\0';                                                        /*  暂时去掉 / 号               */
    point = mount_point_lookup(file_path);                              /*  查找挂载点                  */
    *tmp = '/';                                                         /*  恢复 / 号                   */

    if (point != NULL) {
        int     fd;
        int     ret;
        file_t *file;
                                                                        /*  查找一个空闲的文件结构      */
        for (fd = 0, file = process_file_info[current->pid].files; fd < OPEN_MAX; fd++, file++) {
            kern_mutex_lock(&file->lock, 0);
            if (!file->used) {
                break;
            }
            kern_mutex_unlock(&file->lock);
        }

        if (fd == OPEN_MAX) {                                           /*  没找到                      */
            return -1;
        }

        file->point = point;

        ret = point->fs->open(point, file, tmp, oflag, mode);           /*  打开文件                    */
        if (ret < 0) {
            kern_mutex_unlock(&file->lock);
            return -1;
        }

        file->used = TRUE;                                              /*  占用文件结构                */

        kern_mutex_unlock(&file->lock);

        return fd;                                                      /*  返回文件描述符              */
    } else {
        return -1;
    }
}

/*
 * 读文件
 */
ssize_t vfs_read(int fd, void *buf, size_t len)
{
    mount_point_t *point;
    file_t *file;
    int ret;

    if (buf == NULL || len < 0) {
        return -1;
    }

    if (len == 0) {
        return 0;
    }

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  获得文件结构                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  挂载点                      */

    ret = point->fs->read(point, file, buf, len);                       /*  读文件                      */

    kern_mutex_unlock(&file->lock);

    return ret;
}

/*
 * 写文件
 */
ssize_t vfs_write(int fd, const void *buf, size_t len)
{
    mount_point_t *point;
    file_t *file;
    int ret;

    if (buf == NULL || len < 0) {
        return -1;
    }

    if (len == 0) {
        return 0;
    }

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  获得文件结构                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  挂载点                      */

    ret = point->fs->write(point, file, buf, len);                      /*  写文件                      */

    kern_mutex_unlock(&file->lock);

    return ret;
}

/*
 * 控制文件
 */
int vfs_ioctl(int fd, int cmd, void *arg)
{
    mount_point_t *point;
    file_t *file;
    int ret;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  获得文件结构                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  挂载点                      */

    ret = point->fs->ioctl(point, file, cmd, arg);                      /*  控制文件                    */

    kern_mutex_unlock(&file->lock);

    return ret;
}

/*
 * 关闭文件
 */
int vfs_close(int fd)
{
    mount_point_t *point;
    file_t *file;
    int ret;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  获得文件结构                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  挂载点                      */

    ret = point->fs->close(point, file);                                /*  关闭                        */
    if (ret == 0) {
        file->used = FALSE;                                             /*  如果关闭成功, 释放文件结构  */
    }

    kern_mutex_unlock(&file->lock);

    return ret;
}

/*
 * 打开目录
 */
DIR *vfs_opendir(const char *path)
{
    mount_point_t *point;
    char           file_path[PATH_MAX];
    char          *tmp;

    if (path == NULL) {                                                 /*  PATH 合法性检查             */
        return (DIR *)-1;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        strcpy(file_path, path);
    } else {                                                            /*  如果是相对路径              */
        /*
         * cwd 要以 / 号开头和结尾
         */
        kern_mutex_lock(&process_file_info[current->pid].cwd_lock, 0);  /*  在前面加入当前工作目录      */
        sprintf(file_path, "%s%s", process_file_info[current->pid].cwd, path);
        kern_mutex_unlock(&process_file_info[current->pid].cwd_lock);
    }

    if (file_path[1] == '0') {                                          /*  打开根目录                  */
        point = mount_point_lookup(file_path);
        tmp = file_path;
    } else {
        tmp = strchr(file_path + 1, '/');                               /*  查找挂载点名后的 / 号       */
        if (tmp == NULL) {                                              /*  打开挂载点                  */
            point = mount_point_lookup(file_path);
            tmp = "/";
        } else {
            *tmp = '\0';                                                /*  暂时去掉 / 号               */
            point = mount_point_lookup(file_path);                      /*  查找挂载点                  */
            *tmp = '/';                                                 /*  恢复 / 号                   */
        }
    }

    if (point != NULL) {
        int     fd;
        int     ret;
        file_t *file;
                                                                        /*  查找一个空闲的文件结构      */
        for (fd = 0, file = process_file_info[current->pid].files; fd < OPEN_MAX; fd++, file++) {
            kern_mutex_lock(&file->lock, 0);
            if (!file->used) {
                break;
            }
            kern_mutex_unlock(&file->lock);
        }

        if (fd == OPEN_MAX) {                                           /*  没找到                      */
            return (DIR *)-1;
        }

        file->point = point;

        ret = point->fs->opendir(point, file, tmp);                     /*  打开目录                    */
        if (ret < 0) {
            kern_mutex_unlock(&file->lock);
            return (DIR *)-1;
        }

        file->used = TRUE;                                              /*  占用文件结构                */

        kern_mutex_unlock(&file->lock);

        return (DIR *)fd;                                               /*  返回文件描述符              */
    }

    return (DIR *)-1;
}

/*
 * 读目录项
 */
struct dirent *vfs_readdir(DIR *dir)
{
    mount_point_t *point;
    file_t *file;
    int fd = (int)dir;
    struct dirent *entry;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */
        return NULL;
    }

    file = process_file_info[current->pid].files + fd;                  /*  获得文件结构                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return NULL;
    }

    point = file->point;                                                /*  挂载点                      */

    entry = point->fs->readdir(point, file);                            /*  读目录项                    */

    kern_mutex_unlock(&file->lock);

    return entry;
}

/*
 * 重置目录读点
 */
void vfs_rewinddir(DIR *dir)
{
    mount_point_t *point;
    file_t *file;
    int fd = (int)dir;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */
        return;
    }

    file = process_file_info[current->pid].files + fd;                  /*  获得文件结构                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return;
    }

    point = file->point;                                                /*  挂载点                      */

    point->fs->rewinddir(point, file);                                  /*  重置目录读点                */

    kern_mutex_unlock(&file->lock);
}

/*
 * 调整目录读点
 */
void vfs_seekdir(DIR *dir, long loc)
{
    mount_point_t *point;
    file_t *file;
    int fd = (int)dir;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */
        return;
    }

    file = process_file_info[current->pid].files + fd;                  /*  获得文件结构                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return;
    }

    point = file->point;                                                /*  挂载点                      */

    point->fs->seekdir(point, file, loc);                               /*  调整目录读点                */

    kern_mutex_unlock(&file->lock);
}

/*
 * 获得目录读点
 */
long vfs_telldir(DIR *dir)
{
    mount_point_t *point;
    file_t *file;
    int fd = (int)dir;
    long loc;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  获得文件结构                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  挂载点                      */

    loc = point->fs->telldir(point, file);                              /*  获得目录读点                */

    kern_mutex_unlock(&file->lock);

    return loc;
}

/*
 * 关闭目录
 */
int vfs_closedir(DIR *dir)
{
    mount_point_t *point;
    file_t *file;
    int fd = (int)dir;
    int ret;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  获得文件结构                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  挂载点                      */

    ret = point->fs->closedir(point, file);                             /*  关闭目录                    */
    if (ret == 0) {
        file->used = FALSE;                                             /*  如果关闭成功, 释放文件结构  */
    }

    kern_mutex_unlock(&file->lock);

    return ret;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
