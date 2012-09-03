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
** File name:               file.h
** Last modified Date:      2012-7-11
** Last Version:            1.0.0
** Descriptions:            文件结构管理
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-11
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
#ifndef FILE_H_
#define FILE_H_

#include "vfs/types.h"

/*
 * 分配空闲文件结构
 */
file_t *file_alloc(void);
/*
 * 释放文件结构
 */
void file_free(file_t *file);

/*
 * 初始化文件结构管理
 */
int file_manager_init(void);

#endif                                                                  /*  FILE_H_                     */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
