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
#include "vfs/mount.h"
#include "vfs/fs.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>

/*
 * �����ļ���Ϣ
 */
typedef struct {
    file_t              files[OPEN_MAX];                                /*  �ļ��ṹ��                  */
    char                cwd[PATH_MAX];                                  /*  ��ǰ����Ŀ¼                */
    kern_mutex_t        cwd_lock;                                       /*  ��ǰ����Ŀ¼��              */
} process_file_info_t;

process_file_info_t process_file_info[PROCESS_NR];

/*
 * ���ļ�
 */
int vfs_open(const char *path, int oflag, mode_t mode)
{
    mount_point_t *point;
    char file_path[PATH_MAX];
    char *tmp;

    if (path == NULL) {                                                 /*  PATH �Ϸ��Լ��             */
        return -1;
    }

    if (path[0] == '/') {                                               /*  ����Ǿ���·��              */
        if (path[1] == '\0') {                                          /*  ���ܴ򿪸�Ŀ¼              */
            return -1;
        }
        strcpy(file_path, path);
    } else {                                                            /*  ��������·��              */
        /*
         * cwd Ҫ�� / �ſ�ͷ�ͽ�β
         */
        kern_mutex_lock(&process_file_info[current->pid].cwd_lock, 0);  /*  ��ǰ����뵱ǰ����Ŀ¼      */
        sprintf(file_path, "%s%s", process_file_info[current->pid].cwd, path);
        kern_mutex_unlock(&process_file_info[current->pid].cwd_lock);
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
        int fd;
        int ret;
        file_t *file;
                                                                        /*  ����һ�����е��ļ��ṹ      */
        for (fd = 0, file = process_file_info[current->pid].files; fd < OPEN_MAX; fd++, file++) {
            kern_mutex_lock(&file->lock, 0);
            if (!file->used) {
                break;
            }
            kern_mutex_unlock(&file->lock);
        }

        if (fd == OPEN_MAX) {                                           /*  û�ҵ�                      */
            return -1;
        }

        file->point = point;

        ret = point->fs->open(point, file, tmp, oflag, mode);           /*  ���ļ�                    */
        if (ret < 0) {
            kern_mutex_unlock(&file->lock);
            return -1;
        }

        file->used = TRUE;                                              /*  ռ���ļ��ṹ                */

        kern_mutex_unlock(&file->lock);

        return fd;                                                      /*  �����ļ�������              */
    } else {
        return -1;
    }
}

/*
 * ���ļ�
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

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  ����ļ��ṹ                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  ���ص�                      */

    ret = point->fs->read(point, file, buf, len);                       /*  ���ļ�                      */

    kern_mutex_unlock(&file->lock);

    return ret;
}

/*
 * д�ļ�
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

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  ����ļ��ṹ                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  ���ص�                      */

    ret = point->fs->write(point, file, buf, len);                      /*  д�ļ�                      */

    kern_mutex_unlock(&file->lock);

    return ret;
}

/*
 * �����ļ�
 */
int vfs_ioctl(int fd, int cmd, void *arg)
{
    mount_point_t *point;
    file_t *file;
    int ret;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  ����ļ��ṹ                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  ���ص�                      */

    ret = point->fs->ioctl(point, file, cmd, arg);                      /*  �����ļ�                    */

    kern_mutex_unlock(&file->lock);

    return ret;
}

/*
 * �ر��ļ�
 */
int vfs_close(int fd)
{
    mount_point_t *point;
    file_t *file;
    int ret;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  ����ļ��ṹ                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  ���ص�                      */

    ret = point->fs->close(point, file);                                /*  �ر�                        */
    if (ret == 0) {
        file->used = FALSE;                                             /*  ����رճɹ�, �ͷ��ļ��ṹ  */
    }

    kern_mutex_unlock(&file->lock);

    return ret;
}

/*
 * ��Ŀ¼
 */
DIR *vfs_opendir(const char *path)
{
    mount_point_t *point;
    char file_path[PATH_MAX];
    char *tmp;

    if (path == NULL) {                                                 /*  PATH �Ϸ��Լ��             */
        return (DIR *)-1;
    }

    if (path[0] == '/') {                                               /*  ����Ǿ���·��              */
        strcpy(file_path, path);
    } else {                                                            /*  ��������·��              */
        /*
         * cwd Ҫ�� / �ſ�ͷ�ͽ�β
         */
        kern_mutex_lock(&process_file_info[current->pid].cwd_lock, 0);  /*  ��ǰ����뵱ǰ����Ŀ¼      */
        sprintf(file_path, "%s%s", process_file_info[current->pid].cwd, path);
        kern_mutex_unlock(&process_file_info[current->pid].cwd_lock);
    }

    if (file_path[1] == '0') {                                          /*  �򿪸�Ŀ¼                  */
        point = mount_point_lookup(file_path);
        tmp = file_path;
    } else {
        tmp = strchr(file_path + 1, '/');                               /*  ���ҹ��ص������ / ��       */
        if (tmp == NULL) {                                              /*  �򿪹��ص�                  */
            point = mount_point_lookup(file_path);
            tmp = "/";
        } else {
            *tmp = '\0';                                                /*  ��ʱȥ�� / ��               */
            point = mount_point_lookup(file_path);                      /*  ���ҹ��ص�                  */
            *tmp = '/';                                                 /*  �ָ� / ��                   */
        }
    }

    if (point != NULL) {
        int fd;
        int ret;
        file_t *file;
                                                                        /*  ����һ�����е��ļ��ṹ      */
        for (fd = 0, file = process_file_info[current->pid].files; fd < OPEN_MAX; fd++, file++) {
            kern_mutex_lock(&file->lock, 0);
            if (!file->used) {
                break;
            }
            kern_mutex_unlock(&file->lock);
        }

        if (fd == OPEN_MAX) {                                           /*  û�ҵ�                      */
            return (DIR *)-1;
        }

        file->point = point;

        ret = point->fs->opendir(point, file, tmp);                     /*  ��Ŀ¼                    */
        if (ret < 0) {
            kern_mutex_unlock(&file->lock);
            return (DIR *)-1;
        }

        file->used = TRUE;                                              /*  ռ���ļ��ṹ                */

        kern_mutex_unlock(&file->lock);

        return (DIR *)fd;                                               /*  �����ļ�������              */
    }

    return (DIR *)-1;
}

/*
 * ��Ŀ¼��
 */
struct dirent *vfs_readdir(DIR *dir)
{
    mount_point_t *point;
    file_t *file;
    int fd = (int)dir;
    struct dirent *entry;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return NULL;
    }

    file = process_file_info[current->pid].files + fd;                  /*  ����ļ��ṹ                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return NULL;
    }

    point = file->point;                                                /*  ���ص�                      */

    entry = point->fs->readdir(point, file);                            /*  ��Ŀ¼��                    */

    kern_mutex_unlock(&file->lock);

    return entry;
}

/*
 * ����Ŀ¼����
 */
void vfs_rewinddir(DIR *dir)
{
    mount_point_t *point;
    file_t *file;
    int fd = (int)dir;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return;
    }

    file = process_file_info[current->pid].files + fd;                  /*  ����ļ��ṹ                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return;
    }

    point = file->point;                                                /*  ���ص�                      */

    point->fs->rewinddir(point, file);                                  /*  ����Ŀ¼����                */

    kern_mutex_unlock(&file->lock);
}

/*
 * ����Ŀ¼����
 */
void vfs_seekdir(DIR *dir, long loc)
{
    mount_point_t *point;
    file_t *file;
    int fd = (int)dir;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return;
    }

    file = process_file_info[current->pid].files + fd;                  /*  ����ļ��ṹ                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return;
    }

    point = file->point;                                                /*  ���ص�                      */

    point->fs->seekdir(point, file, loc);                               /*  ����Ŀ¼����                */

    kern_mutex_unlock(&file->lock);
}

/*
 * ���Ŀ¼����
 */
long vfs_telldir(DIR *dir)
{
    mount_point_t *point;
    file_t *file;
    int fd = (int)dir;
    long loc;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  ����ļ��ṹ                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  ���ص�                      */

    loc = point->fs->telldir(point, file);                              /*  ���Ŀ¼����                */

    kern_mutex_unlock(&file->lock);

    return loc;
}

/*
 * �ر�Ŀ¼
 */
int vfs_closedir(DIR *dir)
{
    mount_point_t *point;
    file_t *file;
    int fd = (int)dir;
    int ret;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */
        return -1;
    }

    file = process_file_info[current->pid].files + fd;                  /*  ����ļ��ṹ                */

    kern_mutex_lock(&file->lock, 0);

    if (!file->used) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    point = file->point;                                                /*  ���ص�                      */

    ret = point->fs->closedir(point, file);                             /*  �ر�Ŀ¼                    */
    if (ret == 0) {
        file->used = FALSE;                                             /*  ����رճɹ�, �ͷ��ļ��ṹ  */
    }

    kern_mutex_unlock(&file->lock);

    return ret;
}

/*
 * ��ʼ�������ļ�ϵͳ
 */
int vfs_init(void)
{
    process_file_info_t *info;
    file_t *file;
    int i;
    int j;

    driver_manager_init();

    device_manager_init();

    file_system_manager_init();

    mount_point_manager_init();

    extern file_system_t rootfs;
    file_system_install(&rootfs);

    extern file_system_t devfs;
    file_system_install(&devfs);

    mount("/",    NULL, "rootfs");

    mount("/dev", NULL, "devfs");

    for (i = 0, info = process_file_info; i < PROCESS_NR; i++, info++) {
        strcpy(info->cwd, "/");
        kern_mutex_new(&info->cwd_lock);

        for (j = 0, file = info->files; j < OPEN_MAX; j++, file++) {
            kern_mutex_new(&file->lock);
            file->ctx   = NULL;
            file->point = NULL;
            file->used  = FALSE;
        }
    }

    return 0;
}

void vfs_test(void)
{
    DIR *dir;
    struct dirent *entry;

    dir = vfs_opendir("/dev");

    while ((entry = vfs_readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    vfs_closedir(dir);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
