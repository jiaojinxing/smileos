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
** File name:               driver.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            驱动管理
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

/*
 * 驱动链表
 */
static driver_t *drv_list;

/*
 * 查找驱动
 */
driver_t *driver_lookup(const char *name)
{
    driver_t *drv = drv_list;

    if (name == NULL) {
        return NULL;
    }

    while (drv != NULL) {
        if (strcmp(drv->name, name) == 0) {
            break;
        }
        drv = drv->next;
    }
    return drv;
}

/*
 * 安装驱动
 */
int driver_install(driver_t *drv)
{
    if (drv == NULL || drv->name == NULL) {
        return -1;
    }

    if (driver_lookup(drv->name) != NULL) {
        return -1;
    }

    drv->next = drv_list;
    drv_list  = drv;

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
