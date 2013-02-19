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
** File name:               ramdisk.h
** Last modified Date:      2013-2-19
** Last Version:            1.0.0
** Descriptions:            �ڴ���ͷ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-19
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
#ifndef RAMDISK_H_
#define RAMDISK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

#ifdef SMILEOS_KERNEL
/*********************************************************************************************************
** Function name:           ramdisk_create
** Descriptions:            �����ڴ����豸
** input parameters:        path                �ڴ����豸·��
**                          size                �ڴ��̴�С
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int ramdisk_create(const char *path, size_t size);
#endif

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  RAMDISK_H_                  */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
