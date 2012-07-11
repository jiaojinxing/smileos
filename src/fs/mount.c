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
** File name:               mount.c
** Last modified Date:      2012-3-22
** Last Version:            1.0.0
** Descriptions:            ���ص����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-22
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-27
** Version:                 1.1.0
** Descriptions:            ���ҵ���װ�ڼ��������
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/ipc.h"
#include "vfs/config.h"
#include "vfs/types.h"
#include "vfs/device.h"
#include "vfs/fs.h"
#include <string.h>
#include <stdio.h>

/*
 * ���ص�����
 */
static mount_point_t *point_list;

/*
 * ���ļ�ϵͳ���ص�
 */
mount_point_t *rootfs_point;

/*
 * ���ص������
 */
mutex_t point_mgr_lock;

/*
 * ��װ���ص�
 */
static int mount_point_install(mount_point_t *point)
{
    mutex_lock(&point_mgr_lock, 0);
    point->next = point_list;
    point_list  = point;
    mutex_unlock(&point_mgr_lock);

    return 0;
}

/*
 * ���ҹ��ص�
 */
mount_point_t *mount_point_lookup(const char *name)
{
    mount_point_t *point;

    if (name == NULL) {
        return NULL;
    }

    mutex_lock(&point_mgr_lock, 0);
    point = point_list;
    while (point != NULL) {
        if (strcmp(point->name, name) == 0) {
            break;
        }
        point = point->next;
    }
    mutex_unlock(&point_mgr_lock);

    return point;
}

/*
 * ��ù��ص�
 */
mount_point_t *mount_point_get(int index)
{
    int i;
    mount_point_t *point;

    mutex_lock(&point_mgr_lock, 0);

    for (i = 0, point = point_list; i < index && point != NULL; i++, point = point->next) {
    }

    mutex_unlock(&point_mgr_lock);

    return point;
}

/*
 * ����
 */
int mount(const char *point_name, const char *dev_name, const char *fs_name)
{
    mount_point_t *point;
    file_system_t *fs;
    device_t *dev;
    int ret;

    if (point_name == NULL ||
        fs_name    == NULL) {
        return -1;
    }

    mutex_lock(&point_mgr_lock, 0);

    point = mount_point_lookup(point_name);                             /*  ���ҹ��ص�                  */
    if (point == NULL) {                                                /*  û�ҵ�                      */
        fs = file_system_lookup(fs_name);                               /*  �����ļ�ϵͳ                */
        if (fs != NULL) {
            dev = device_lookup(dev_name);                              /*  �����豸                    */

            point = kmalloc(sizeof(mount_point_t));                     /*  ������ص�                  */
            if (point != NULL) {
                if (point_name[0] == '/') {                             /*  ��֤���ص��� / �ſ�ʼ       */
                    strlcpy(point->name, point_name, sizeof(point->name));
                } else {
                    snprintf(point->name, sizeof(point->name), "/%s", point_name);
                }

                if (point->name[1] != '\0') {                           /*  ������Ǹ��ļ�ϵͳ          */
                    if (strchr(point->name + 1, '/') != NULL) {         /*  ��֤���ص㲻���ٳ��� / ��   */
                        kfree(point);                                   /*  ��Ϊ�Ҳ�֪�� / �Ż��м���   */
                        mutex_unlock(&point_mgr_lock);
                        return -1;                                      /*  ���Ե�������������          */
                    }
                } else {
                    rootfs_point = point;
                }

                point->fs  = fs;
                point->dev = dev;

                /*
                 * ���豸Ϊ��ʱ, �豸���������ز���
                 */
                ret = fs->mount(point, dev, dev_name);                  /*  ����                        */
                if (ret < 0) {
                    if (rootfs_point == point) {
                        rootfs_point =  NULL;
                    }
                    kfree(point);
                    mutex_unlock(&point_mgr_lock);
                    return -1;
                } else {
                    mount_point_install(point);                         /*  ��װ���ص�                  */
                    mutex_unlock(&point_mgr_lock);
                    return 0;
                }
            }
        }
    }

    mutex_unlock(&point_mgr_lock);
    return -1;
}

/*
 * ��ʼ�����ص����
 */
int mount_point_manager_init(void)
{
    rootfs_point = NULL;

    point_list   = NULL;

    return mutex_new(&point_mgr_lock);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
