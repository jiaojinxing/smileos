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
** File name:               device.h
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            设备管理
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
#ifndef DEVICE_H_
#define DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "vfs/types.h"
/*********************************************************************************************************
** Function name:           device_get
** Descriptions:            获得设备
** input parameters:        index               设备索引
** output parameters:       NONE
** Returned value:          设备 OR NULL
*********************************************************************************************************/
device_t *device_get(unsigned int index);
/*********************************************************************************************************
** Function name:           device_lookup
** Descriptions:            查找设备
** input parameters:        name                设备名
** output parameters:       NONE
** Returned value:          设备 OR NULL
*********************************************************************************************************/
device_t *device_lookup(const char *name);
/*********************************************************************************************************
** Function name:           device_remove
** Descriptions:            删除设备
** input parameters:        name                设备名
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int device_remove(const char *name);
/*********************************************************************************************************
** Function name:           device_create
** Descriptions:            创建设备
** input parameters:        dev_name            设备名
**                          drv_name            驱动名
**                          ctx                 上下文
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int device_create(const char *dev_name, const char *drv_name, void *ctx);
/*********************************************************************************************************
** Function name:           device_manager_init
** Descriptions:            初始化设备管理
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int device_manager_init(void);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  DEVICE_H_                   */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/

