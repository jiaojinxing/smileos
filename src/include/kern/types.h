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
** File name:               types.h
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            系统数据类型定义
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#ifndef TYPES_H_
#define TYPES_H_
/*********************************************************************************************************
  系统数据类型定义
*********************************************************************************************************/
typedef unsigned char               uint8_t;
typedef          char               int8_t;
typedef unsigned short              uint16_t;
typedef          short              int16_t;
typedef unsigned int                uint32_t;
typedef          int                int32_t;
typedef unsigned long long          uint64_t;
typedef          long long          int64_t;

#ifndef NULL
#define NULL                        0
#endif

#ifndef TRUE
#define TRUE                        1
#endif

#ifndef FALSE
#define FALSE                       0
#endif

/*
 * 虚拟地址空间
 */
typedef struct {
    uint32_t    base;
    uint32_t    size;
} virtual_space_t;
/*********************************************************************************************************
  为了更好地移植 LwIP, 加入以下数据类型定义
*********************************************************************************************************/
typedef uint8_t                     u8_t;
typedef int8_t                      s8_t;
typedef uint16_t                    u16_t;
typedef int16_t                     s16_t;
typedef uint32_t                    u32_t;
typedef int32_t                     s32_t;

typedef uint32_t                    mem_ptr_t;

#define U16_F                       "u"
#define U32_F                       "u"
#define S16_F                       "d"
#define S32_F                       "d"
#define X16_F                       "X"
#define X32_F                       "X"
#define SZT_F                       "u"
#define X8_F                        "02X"
/*********************************************************************************************************
  编译器结构缩排
*********************************************************************************************************/
#define PACK_STRUCT_FIELD(x)        x
#define PACK_STRUCT_STRUCT          __attribute__((packed))
#define PACK_STRUCT_BEGIN                                               /*  单字节缩排结构体            */
#define PACK_STRUCT_END                                                 /*  结束单字节缩排结构体        */
/*********************************************************************************************************
  字节序
*********************************************************************************************************/
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN               1234
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN                  4321
#endif

#define BYTE_ORDER                  LITTLE_ENDIAN
/*********************************************************************************************************
  处理内存对齐
*********************************************************************************************************/
#define MEM_ALIGNMENT               4
#define MEM_ALIGN_SIZE(size)        (((size) + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT - 1))
#define MEM_ALIGN_SIZE_LESS(size)   (((size) & ~(MEM_ALIGNMENT - 1)))
#define MEM_ALIGN(addr)             ((void *)(((uint32_t)(addr) + MEM_ALIGNMENT - 1) & ~(uint32_t)(MEM_ALIGNMENT - 1)))
#define MEM_ALIGN_LESS(addr)        ((void *)(((uint32_t)(addr)) & ~(uint32_t)(MEM_ALIGNMENT - 1)))
/*********************************************************************************************************
  为了更好地移植 FATFS, 加入以下数据类型定义
*********************************************************************************************************/
/* These types must be 16-bit, 32-bit or larger integer */
typedef int             INT;
typedef unsigned int    UINT;

/* These types must be 8-bit integer */
typedef char            CHAR;
typedef unsigned char   UCHAR;
typedef unsigned char   BYTE;

/* These types must be 16-bit integer */
typedef short           SHORT;
typedef unsigned short  USHORT;
typedef unsigned short  WORD;
typedef unsigned short  WCHAR;

/* These types must be 32-bit integer */
typedef long            LONG;
typedef unsigned int    ULONG;
typedef unsigned int    DWORD;

#endif                                                                  /*  TYPES_H_                    */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
