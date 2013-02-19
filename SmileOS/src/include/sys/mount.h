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
** Last modified Date:      2012-4-18
** Last Version:            1.0.0
** Descriptions:            挂载头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-18
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
#ifndef SYS_MOUNT_H_
#define SYS_MOUNT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/stat.h>
/*********************************************************************************************************
** Function name:           mount
** Descriptions:            挂载
** input parameters:        point_name          挂载点名
**                          dev_name            设备名
**                          fs_name             文件系统名
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mount(const char *point_name, const char *dev_name, const char *fs_name);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  SYS_MOUNT_H_                */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
