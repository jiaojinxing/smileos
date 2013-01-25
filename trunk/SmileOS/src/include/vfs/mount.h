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
** File name:               mount.h
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
#ifndef MOUNT_H_
#define MOUNT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "vfs/types.h"

/*********************************************************************************************************
** Function name:           mount_point_install
** Descriptions:            ��װ���ص�
** input parameters:        point               ���ص�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mount_point_install(mount_point_t *point);
/*********************************************************************************************************
** Function name:           mount_point_lookup
** Descriptions:            ���ҹ��ص�
** input parameters:        name                ���ص���
** output parameters:       NONE
** Returned value:          ���ص�
*********************************************************************************************************/
mount_point_t *mount_point_lookup(const char *name);
/*********************************************************************************************************
** Function name:           mount_point_remove
** Descriptions:            ɾ�����ص�
** input parameters:        point               ���ص�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mount_point_remove(mount_point_t *point);
/*********************************************************************************************************
** Function name:           mount_point_get
** Descriptions:            ��ù��ص�
** input parameters:        index               ����
** output parameters:       NONE
** Returned value:          ���ص�
*********************************************************************************************************/
mount_point_t *mount_point_get(unsigned int index);
/*********************************************************************************************************
** Function name:           mount_point_manager_init
** Descriptions:            ��ʼ�����ص����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mount_point_manager_init(void);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  MOUNT_H_                    */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
