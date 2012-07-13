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
** File name:               device.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            �豸����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-20
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-27
** Version:                 1.1.0
** Descriptions:            ���ҵ���װ�ڼ��������
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-4-6
** Version:                 1.2.0
** Descriptions:            ʹ�� HASH �����豸����, ��������
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/ipc.h"
#include "vfs/config.h"
#include "vfs/types.h"
#include "vfs/driver.h"
#include <string.h>

/*
 * �豸����
 */
static device_t *dev_list;

/*
 * �豸������
 */
mutex_t dev_mgr_lock;

/*
 * ��װ�豸
 */
static int device_install(device_t *dev)
{
    mutex_lock(&dev_mgr_lock, 0);
    dev->next = dev_list;
    dev_list  = dev;
    mutex_unlock(&dev_mgr_lock);

    return 0;
}

/*
 * ����豸
 */
device_t *device_get(int index)
{
    int i;
    device_t *dev;

    mutex_lock(&dev_mgr_lock, 0);

    for (i = 0, dev = dev_list; i < index && dev != NULL; i++, dev = dev->next) {
    }

    mutex_unlock(&dev_mgr_lock);

    return dev;
}

/*
 * �����ַ��� Hash �����Ƚ�
 * http://www.byvoid.com/blog/string-hash-compare/
 */
/*
 * BKDR Hash Function
 */
unsigned int BKDRHash(const char *str)
{
    unsigned int seed = 131;                                            /*  31 131 1313 13131 131313 etc*/
    unsigned int hash = 0;
    char         ch;

    while ((ch = *str++) != 0) {
        hash = hash * seed + ch;
    }

    return (hash & 0x7FFFFFFF);
}

/*
 * �����豸
 */
device_t *device_lookup(const char *name)
{
    device_t *dev;
    unsigned int key;

    if (name == NULL) {
        return NULL;
    }

    key = BKDRHash(name);

    mutex_lock(&dev_mgr_lock, 0);
    dev = dev_list;
    while (dev != NULL) {
        if (key == dev->key) {
            break;
        }
        dev = dev->next;
    }
    mutex_unlock(&dev_mgr_lock);

    return dev;
}

/*
 * ɾ���豸
 */
int device_remove(const char *name)
{
    device_t *dev, *prev;
    unsigned int key;

    if (name == NULL) {
        return -1;
    }

    key = BKDRHash(name);

    mutex_lock(&dev_mgr_lock, 0);
    prev = NULL;
    dev  = dev_list;
    while (dev != NULL) {
        if (key == dev->key) {
            break;
        }
        prev = dev;
        dev = dev->next;
    }

    if (dev != NULL) {
        if (prev != NULL) {
            prev->next = dev->next;
        } else {
            dev_list = dev->next;
        }
        kfree(dev);
    }
    mutex_unlock(&dev_mgr_lock);

    return 0;
}

/*
 * �����豸
 */
int device_create(const char *dev_name, const char *drv_name, void *ctx)
{
    driver_t *drv;
    device_t *dev;

    if (dev_name == NULL || drv_name == NULL) {
        return -1;
    }

    if (strncmp(dev_name, "/dev/", 5) != 0) {
        return -1;
    }

    if (dev_name[5] == '\0') {
        return -1;
    }

    mutex_lock(&dev_mgr_lock, 0);
    if (device_lookup(dev_name) != NULL) {
        mutex_unlock(&dev_mgr_lock);
        return -1;
    }

    drv = driver_lookup(drv_name);
    if (drv != NULL) {
        dev = kmalloc(sizeof(device_t));
        if (dev != NULL) {
            strlcpy(dev->name, dev_name, sizeof(dev->name));
            dev->drv    = drv;
            dev->ctx    = ctx;
            dev->devno  = 0;
            dev->key    = BKDRHash(dev_name);
            device_install(dev);
            mutex_unlock(&dev_mgr_lock);
            return 0;
        }
    }
    mutex_unlock(&dev_mgr_lock);
    return -1;
}

/*
 * ��ʼ���豸����
 */
int device_manager_init(void)
{
    dev_list = NULL;

    return mutex_new(&dev_mgr_lock);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
