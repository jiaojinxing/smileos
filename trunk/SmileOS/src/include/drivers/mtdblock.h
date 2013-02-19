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
** File name:               mtdblock.h
** Last modified Date:      2013-2-19
** Last Version:            1.0.0
** Descriptions:            MTD 块头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-19
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
#ifndef MTDBLOCK_H_
#define MTDBLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SMILEOS_KERNEL
#include "kern/types.h"
/*********************************************************************************************************
** Function name:           mtdblock_create
** Descriptions:            创建 MTD 块设备
** input parameters:        path                MTD 块设备路径
**                          mtd_no              MTD 设备号
**                          start               开始块号
**                          end                 结束块号
**                          reserved            保留块数
**                          readonly            是否只读
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mtdblock_create(const char *path,
                    long mtd_no,
                    long start,
                    long end,
                    long reserved,
                    bool_t readonly);
#endif

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  MTDBLOCK_H_                 */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
