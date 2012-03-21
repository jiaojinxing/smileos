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
#include <string.h>
#include <stdio.h>

/*
 * 文件系统链表
 */
static file_system_t *fs_list;

/*
 * 查找文件系统
 */
file_system_t *file_system_lookup(const char *name)
{
    file_system_t *fs = fs_list;

    if (name == NULL) {
        return NULL;
    }

    while (fs != NULL) {
        if (strcmp(fs->name, name) == 0) {
            break;
        }
        fs = fs->next;
    }
    return fs;
}

/*
 * 安装文件系统
 */
int file_system_install(file_system_t *fs)
{
    if (fs == NULL || fs->name == NULL) {
        return -1;
    }

    if (file_system_lookup(fs->name) != NULL) {
        return -1;
    }

    fs->next = fs_list;
    fs_list  = fs;

    return 0;
}

/*
 * 挂载点链表
 */
static mount_point_t *point_list;

/*
 * 安装挂载点
 */
static int mount_point_install(mount_point_t *point)
{
    point->next = point_list;
    point_list  = point;

    return 0;
}

/*
 * 查找挂载点
 */
mount_point_t *mount_point_lookup(const char *name)
{
    mount_point_t *point = point_list;

    if (name == NULL) {
        return NULL;
    }

    while (point != NULL) {
        if (strcmp(point->name, name) == 0) {
            break;
        }
        point = point->next;
    }
    return point;
}

/*
 * 挂载
 */
int mount(const char *point_name, const char *dev_name, const char *fs_name)
{
    mount_point_t *point;
    file_system_t *fs;
    device_t      *dev;
    int            ret;

    if (point_name == NULL ||
        dev_name   == NULL ||
        fs_name    == NULL) {
        return -1;
    }

    point = mount_point_lookup(point_name);                             /*  查找挂载点                  */
    if (point == NULL) {                                                /*  没找到                      */
        fs = file_system_lookup(fs_name);                               /*  查找文件系统                */
        if (fs != NULL) {
            dev = device_lookup(dev_name);                              /*  查找设备                    */
            if (dev != NULL) {
                point = kmalloc(sizeof(mount_point_t));                 /*  分配挂载点                  */
                if (point != NULL) {
                    if (point_name[0] == '/') {                         /*  保证挂载点以 / 号开始       */
                        strcpy(point->name, point_name);
                    } else {
                        sprintf(point->name, "/%s", point_name);
                    }
                    if (strchr(point->name + 1, '/') != NULL) {         /*  保证挂载点不能再出现 / 号   */
                        kfree(point);
                        return -1;
                    }

                    point->fs   = fs;
                    point->dev  = dev;

                    ret = fs->mount(point);                             /*  挂载                        */
                    if (ret < 0) {
                        kfree(point);
                        return -1;
                    } else {
                        mount_point_install(point);                     /*  安装挂载点                  */
                        return 0;
                    }
                }
            }
        }
    }

    return -1;
}

/*
 * 打开文件
 */
int kopen(const char *path, int oflag, mode_t mode)
{
    mount_point_t *point;
    char           file_path[PATH_MAX];
    char          *tmp;

    if (path == NULL) {                                                 /*  PATH 合法性检查             */
        return -1;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        strcpy(file_path, path);
    } else {                                                            /*  如果是相对路径              */
        /*
         * tasks[current->pid].cwd 要以 / 号开头和结尾
         */
        sprintf(file_path, "%s%s", tasks[current->pid].cwd, path);      /*  在前面加入当前工作目录      */
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
        for (fd = 0, file = tasks[current->pid].files; fd < OPEN_MAX; fd++, file++) {
            if (!file->used) {
                break;
            }
        }

        if (fd == OPEN_MAX) {                                           /*  没找到                      */
            return -1;
        }

        file->point = point;

        ret = point->fs->open(file, tmp, oflag, mode);                  /*  打开文件                    */
        if (ret < 0) {
            return -1;
        }

        file->used = TRUE;                                              /*  占用文件结构                */

        return fd;                                                      /*  返回文件描述符              */
    } else {
        return -1;
    }
}

/*
 * 读文件
 */
ssize_t kread(int fd, void *buf, size_t len)
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

    file = tasks[current->pid].files + fd;                              /*  获得文件结构                */

    point = file->point;                                                /*  挂载点                      */

    ret = point->fs->read(file, buf, len);                              /*  读                          */
    return ret;
}

/*
 * 关闭文件
 */
int kclose(int fd)
{
    mount_point_t *point;
    file_t *file;
    int ret;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */
        return -1;
    }

    file = tasks[current->pid].files + fd;                              /*  获得文件结构                */

    point = file->point;                                                /*  挂载点                      */

    ret = point->fs->close(file);                                       /*  关闭                        */
    if (ret == 0) {
        file->used = FALSE;                                             /*  如果关闭成功, 释放文件结构  */
    }
    return ret;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
