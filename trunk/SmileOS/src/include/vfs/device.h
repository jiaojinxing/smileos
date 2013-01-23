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
** Descriptions:            �豸����
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
#ifndef DEVICE_H_
#define DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "vfs/types.h"
/*********************************************************************************************************
** Function name:           device_get
** Descriptions:            ����豸
** input parameters:        index               �豸����
** output parameters:       NONE
** Returned value:          �豸 OR NULL
*********************************************************************************************************/
device_t *device_get(unsigned int index);
/*********************************************************************************************************
** Function name:           device_lookup
** Descriptions:            �����豸
** input parameters:        name                �豸��
** output parameters:       NONE
** Returned value:          �豸 OR NULL
*********************************************************************************************************/
device_t *device_lookup(const char *name);
/*********************************************************************************************************
** Function name:           device_remove
** Descriptions:            ɾ���豸
** input parameters:        name                �豸��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int device_remove(const char *name);
/*********************************************************************************************************
** Function name:           device_create
** Descriptions:            �����豸
** input parameters:        dev_name            �豸��
**                          drv_name            ������
**                          ctx                 ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int device_create(const char *dev_name, const char *drv_name, void *ctx);
/*********************************************************************************************************
** Function name:           device_manager_init
** Descriptions:            ��ʼ���豸����
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

