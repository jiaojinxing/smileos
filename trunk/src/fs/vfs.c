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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "vfs/vfs.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include <string.h>
#include <stdio.h>

/*
 * �ļ�ϵͳ����
 */
static file_system_t *fs_list;

/*
 * �����ļ�ϵͳ
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
 * ��װ�ļ�ϵͳ
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
 * ���ص�����
 */
static mount_point_t *point_list;

/*
 * ��װ���ص�
 */
static int mount_point_install(mount_point_t *point)
{
    point->next = point_list;
    point_list  = point;

    return 0;
}

/*
 * ���ҹ��ص�
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
 * ����
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

    point = mount_point_lookup(point_name);                             /*  ���ҹ��ص�                  */
    if (point == NULL) {                                                /*  û�ҵ�                      */
        fs = file_system_lookup(fs_name);                               /*  �����ļ�ϵͳ                */
        if (fs != NULL) {
            dev = device_lookup(dev_name);                              /*  �����豸                    */
            if (dev != NULL) {
                point = kmalloc(sizeof(mount_point_t));                 /*  ������ص�                  */
                if (point != NULL) {
                    if (point_name[0] == '/') {                         /*  ��֤���ص��� / �ſ�ʼ       */
                        strcpy(point->name, point_name);
                    } else {
                        sprintf(point->name, "/%s", point_name);
                    }
                    if (strchr(point->name + 1, '/') != NULL) {         /*  ��֤���ص㲻���ٳ��� / ��   */
                        kfree(point);
                        return -1;
                    }

                    point->fs   = fs;
                    point->dev  = dev;

                    ret = fs->mount(point);                             /*  ����                        */
                    if (ret < 0) {
                        kfree(point);
                        return -1;
                    } else {
                        mount_point_install(point);                     /*  ��װ���ص�                  */
                        return 0;
                    }
                }
            }
        }
    }

    return -1;
}

/*
 * ���ļ�
 */
int kopen(const char *path, int oflag, mode_t mode)
{
    mount_point_t *point;
    char           file_path[PATH_MAX];
    char          *tmp;

    if (path == NULL) {                                                 /*  PATH �Ϸ��Լ��             */
        return -1;
    }

    if (path[0] == '/') {                                               /*  ����Ǿ���·��              */
        strcpy(file_path, path);
    } else {                                                            /*  ��������·��              */
        /*
         * tasks[current->pid].cwd Ҫ�� / �ſ�ͷ�ͽ�β
         */
        sprintf(file_path, "%s%s", tasks[current->pid].cwd, path);      /*  ��ǰ����뵱ǰ����Ŀ¼      */
    }

    tmp = strchr(file_path + 1, '/');                                   /*  ���ҹ��ص������ / ��       */
    if (tmp == NULL) {                                                  /*  û�е�                      */
        return -1;
    }
    if (tmp[1] == '0') {                                                /*  ���ܴ򿪹��ص�              */
        return -1;
    }

    *tmp = '\0';                                                        /*  ��ʱȥ�� / ��               */

    point = mount_point_lookup(file_path);                              /*  ���ҹ��ص�                  */

    *tmp = '/';                                                         /*  �ָ� / ��                   */

    if (point != NULL) {
        int     fd;
        int     ret;
        file_t *file;
                                                                        /*  ����һ�����е��ļ��ṹ      */
        for (fd = 0, file = tasks[current->pid].files; fd < OPEN_MAX; fd++, file++) {
            if (!file->used) {
                break;
            }
        }

        if (fd == OPEN_MAX) {                                           /*  û�ҵ�                      */
            return -1;
        }

        file->point = point;

        ret = point->fs->open(file, tmp, oflag, mode);                  /*  ���ļ�                    */
        if (ret < 0) {
            return -1;
        }

        file->used = TRUE;                                              /*  ռ���ļ��ṹ                */

        return fd;                                                      /*  �����ļ�������              */
    } else {
        return -1;
    }
}

/*
 * ���ļ�
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

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return -1;
    }

    file = tasks[current->pid].files + fd;                              /*  ����ļ��ṹ                */

    point = file->point;                                                /*  ���ص�                      */

    ret = point->fs->read(file, buf, len);                              /*  ��                          */
    return ret;
}

/*
 * �ر��ļ�
 */
int kclose(int fd)
{
    mount_point_t *point;
    file_t *file;
    int ret;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return -1;
    }

    file = tasks[current->pid].files + fd;                              /*  ����ļ��ṹ                */

    point = file->point;                                                /*  ���ص�                      */

    ret = point->fs->close(file);                                       /*  �ر�                        */
    if (ret == 0) {
        file->used = FALSE;                                             /*  ����رճɹ�, �ͷ��ļ��ṹ  */
    }
    return ret;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
