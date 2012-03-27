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
** Descriptions:            �ں��������Ͷ���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#ifndef KERN_TYPES_H_
#define KERN_TYPES_H_
/*********************************************************************************************************
  ϵͳ�������Ͷ���
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

#ifndef KB
#define KB                          (1024)
#define MB                          (1024 * KB)
#define GB                          (1024 * MB)
#endif

#define INT_MAX                     2147483647

/*
 * �����ַ�ռ�
 */
typedef struct {
    uint32_t    base;
    uint32_t    size;
} virtual_space_t;
/*********************************************************************************************************
  Ϊ�˸��õ���ֲ LwIP, ���������������Ͷ���
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
  �������ṹ����
*********************************************************************************************************/
#define PACK_STRUCT_FIELD(x)        x
#define PACK_STRUCT_STRUCT          __attribute__((packed))
#define PACK_STRUCT_BEGIN                                               /*  ���ֽ����Žṹ��            */
#define PACK_STRUCT_END                                                 /*  �������ֽ����Žṹ��        */
/*********************************************************************************************************
  �ֽ���
*********************************************************************************************************/
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN               1234
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN                  4321
#endif

#define BYTE_ORDER                  LITTLE_ENDIAN
/*********************************************************************************************************
  �����ڴ����
*********************************************************************************************************/
#define MEM_ALIGNMENT               4
#define MEM_ALIGN_SIZE(size)        (((size) + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT - 1))
#define MEM_ALIGN_SIZE_LESS(size)   (((size) & ~(MEM_ALIGNMENT - 1)))
#define MEM_ALIGN(addr)             ((void *)(((uint32_t)(addr) + MEM_ALIGNMENT - 1) & ~(uint32_t)(MEM_ALIGNMENT - 1)))
#define MEM_ALIGN_LESS(addr)        ((void *)(((uint32_t)(addr)) & ~(uint32_t)(MEM_ALIGNMENT - 1)))

#endif                                                                  /*  KERN_TYPES_H_               */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
