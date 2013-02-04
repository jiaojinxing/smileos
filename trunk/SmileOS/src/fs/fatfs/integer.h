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
** File name:               integer.h
** Last modified Date:      2012-3-17
** Last Version:            1.0.0
** Descriptions:            FatFS 数据类型定义头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-17
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
#ifndef _INTEGER
#define _INTEGER

#include "vfs/types.h"
#include <stdint.h>

/* These types must be 16-bit, 32-bit or larger integer */
typedef int32_t         INT;
typedef uint32_t        UINT;

/* These types must be 8-bit integer */
typedef int8_t          CHAR;
typedef uint8_t         UCHAR;
typedef uint8_t         BYTE;

/* These types must be 16-bit integer */
typedef int16_t         SHORT;
typedef uint16_t        USHORT;
typedef uint16_t        WORD;
typedef uint16_t        WCHAR;

/* These types must be 32-bit integer */
typedef int32_t         LONG;
typedef uint32_t        ULONG;
typedef uint32_t        DWORD;

#endif
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
