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
** Modified by:
** Modified date:
** Version:
** Descriptions:
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
/*********************************************************************************************************
** ȫ�ֱ���
*********************************************************************************************************/
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
/*********************************************************************************************************
** Function name:           mount_point_install
** Descriptions:            ��װ���ص�
** input parameters:        point               ���ص�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mount_point_install(mount_point_t *point)
{
    mutex_lock(&point_mgr_lock, 0);
    point->next = point_list;
    point_list  = point;
    mutex_unlock(&point_mgr_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           mount_point_lookup
** Descriptions:            ���ҹ��ص�
** input parameters:        name                ���ص���
** output parameters:       NONE
** Returned value:          ���ص�
*********************************************************************************************************/
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
/*********************************************************************************************************
** Function name:           mount_point_remove
** Descriptions:            ɾ�����ص�               ���ص�
** input parameters:        point
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mount_point_remove(mount_point_t *point)
{
    mount_point_t *prev, *temp;
    int ret = -1;

    if (point == NULL) {
        return ret;
    }

    mutex_lock(&point_mgr_lock, 0);

    if (atomic_read(&point->ref) == 0) {
        prev = NULL;
        temp = point_list;
        while (temp != NULL && temp != point) {
            prev = temp;
            temp = temp->next;
        }

        if (temp != NULL) {
            if (prev != NULL) {
                prev->next = point->next;
            } else {
                point_list = point->next;
            }
            ret = 0;
            kfree(point);
        }
    }
    mutex_unlock(&point_mgr_lock);

    return ret;
}
/*********************************************************************************************************
** Function name:           mount_point_get
** Descriptions:            ��ù��ص�
** input parameters:        index               ����
** output parameters:       NONE
** Returned value:          ���ص�
*********************************************************************************************************/
mount_point_t *mount_point_get(unsigned int index)
{
    int i;
    mount_point_t *point;

    mutex_lock(&point_mgr_lock, 0);

    for (i = 0, point = point_list; i < index && point != NULL; i++, point = point->next) {
    }

    mutex_unlock(&point_mgr_lock);

    return point;
}
/*********************************************************************************************************
** Function name:           mount_point_manager_init
** Descriptions:            ��ʼ�����ص����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mount_point_manager_init(void)
{
    rootfs_point = NULL;

    point_list   = NULL;

    return mutex_new(&point_mgr_lock);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
