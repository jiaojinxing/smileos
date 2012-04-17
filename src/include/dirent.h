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
** File name:               dirent.h
** Last modified Date:      2012-3-22
** Last Version:            1.0.0
** Descriptions:            目录项
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-22
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
#ifndef DIRENT_H_
#define DIRENT_H_

#include "vfs/config.h"
#include "vfs/types.h"
#include <sys/types.h>

/*
 * 目录项
 */
struct dirent {
    ino_t  d_ino;
    char   d_name[NAME_MAX];
};

/*
 * 打开目录
 */
DIR *opendir(const char *path);

/*
* 关闭目录
*/
int closedir(DIR *dir);

/*
* 读目录项
*/
struct dirent *readdir(DIR *dir);

/*
* 重置目录读点
*/
int rewinddir(DIR *dir);

/*
* 调整目录读点
*/
int seekdir(DIR *dir, long loc);

/*
* 获得目录读点
*/
long telldir(DIR *dir);

#endif                                                                  /*  DIRENT_H_                   */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
