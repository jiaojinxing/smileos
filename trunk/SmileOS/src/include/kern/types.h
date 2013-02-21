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

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
** ϵͳ�������Ͷ���
*********************************************************************************************************/
#include <stdint.h>
#include <stddef.h>

/*
 * ��Щ���������� stdint.h ͷ�ļ����ж���, �ʲ��ٶ���
 */
#if 0
typedef unsigned char               uint8_t;
typedef          char               int8_t;
typedef unsigned short              uint16_t;
typedef          short              int16_t;
typedef unsigned int                uint32_t;
typedef          int                int32_t;
typedef unsigned long long          uint64_t;
typedef          long long          int64_t;
#endif

typedef uint8_t                     u8_t;
typedef int8_t                      s8_t;
typedef uint16_t                    u16_t;
typedef int16_t                     s16_t;
typedef uint32_t                    u32_t;
typedef int32_t                     s32_t;

typedef uint8_t                     u8;
typedef uint16_t                    u16;
typedef uint32_t                    u32;
typedef uint64_t                    u64;

typedef unsigned char               uchar;
typedef unsigned int                uint;
typedef unsigned short              ushort;
typedef unsigned long               ulong;

typedef uint8_t                     u_int8_t;
typedef uint16_t                    u_int16_t;
typedef uint32_t                    u_int32_t;
typedef uint64_t                    u_int64_t;
typedef uint64_t                    u_quad_t;
typedef int64_t                     quad_t;
typedef quad_t                     *qaddr_t;

#ifndef NULL
#define NULL                        0
#endif

#ifndef TRUE
#define TRUE                        1
#endif

#ifndef FALSE
#define FALSE                       0
#endif
/*********************************************************************************************************
** ��С
*********************************************************************************************************/
#ifndef KB
#define KB                          (1024)
#define MB                          (1024 * KB)
#define GB                          (1024 * MB)
#endif
/*********************************************************************************************************
** �ֽ���
*********************************************************************************************************/
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN               1234
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN                  4321
#endif

#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN                BIG_ENDIAN
#endif

#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN             LITTLE_ENDIAN
#endif

#ifndef FLOAT_WORD_ORDER
#define FLOAT_WORD_ORDER            BYTE_ORDER
#endif

#ifndef __FLOAT_WORD_ORDER
#define __FLOAT_WORD_ORDER          FLOAT_WORD_ORDER
#endif
/*********************************************************************************************************
** ARCH �������Ͷ���
*********************************************************************************************************/
#include "arch/types.h"
/*********************************************************************************************************
** �ڴ�ռ�
*********************************************************************************************************/
typedef mem_ptr_t                   mem_size_t;

typedef struct {
    mem_ptr_t                       base;
    mem_size_t                      size;
} mem_space_t;
/*********************************************************************************************************
** TICK ��������
*********************************************************************************************************/
typedef uint64_t                    tick_t;
/*********************************************************************************************************
** �����ڴ����
*********************************************************************************************************/
#define MEM_ALIGN_SIZE(size)        (((size) + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT - 1))
#define MEM_ALIGN_SIZE_LESS(size)   (((size) & ~(MEM_ALIGNMENT - 1)))
#define MEM_ALIGN(addr)             ((void *)(((mem_ptr_t)(addr) + MEM_ALIGNMENT - 1) & ~(mem_ptr_t)(MEM_ALIGNMENT - 1)))
#define MEM_ALIGN_LESS(addr)        ((void *)(((mem_ptr_t)(addr)) & ~(mem_ptr_t)(MEM_ALIGNMENT - 1)))

#define ALIGN(size, align)          (((size) + align - 1) & ~(align - 1))
/*********************************************************************************************************
** �������ṹ����
*********************************************************************************************************/
#define PACK_STRUCT_FIELD(x)        x
#define PACK_STRUCT_STRUCT          __attribute__((packed))
#define PACK_STRUCT_BEGIN                                               /*  ���ֽ����Žṹ��            */
#define PACK_STRUCT_END                                                 /*  �������ֽ����Žṹ��        */
/*********************************************************************************************************
** ��������Сֵ
*********************************************************************************************************/
#define max(a, b)                   (a) > (b) ? (a) : (b)
#define min(a, b)                   (a) < (b) ? (a) : (b)
/*********************************************************************************************************
** ��д�ڴ��ַ
*********************************************************************************************************/
#define writeb(d, r)                (*(volatile uint8_t  *)r) = (d)
#define writew(d, r)                (*(volatile uint16_t *)r) = (d)
#define writel(d, r)                (*(volatile uint32_t *)r) = (d)
#define readb(r)                    (*(volatile uint8_t  *)r)
#define readw(r)                    (*(volatile uint16_t *)r)
#define readl(r)                    (*(volatile uint8_t  *)r)
/*********************************************************************************************************
** �����С
*********************************************************************************************************/
#define ARRAY_SIZE(a)               (sizeof(a) / sizeof(a[0]))
/*********************************************************************************************************
** �ֽ���ת��
*********************************************************************************************************/
#define uswap_16(x) \
    ((((x) & 0xff00) >> 8) | \
     (((x) & 0x00ff) << 8))
#define uswap_32(x) \
    ((((x) & 0xff000000) >> 24) | \
     (((x) & 0x00ff0000) >>  8) | \
     (((x) & 0x0000ff00) <<  8) | \
     (((x) & 0x000000ff) << 24))
#define _uswap_64(x, sfx) \
    ((((x) & 0xff00000000000000##sfx) >> 56) | \
     (((x) & 0x00ff000000000000##sfx) >> 40) | \
     (((x) & 0x0000ff0000000000##sfx) >> 24) | \
     (((x) & 0x000000ff00000000##sfx) >>  8) | \
     (((x) & 0x00000000ff000000##sfx) <<  8) | \
     (((x) & 0x0000000000ff0000##sfx) << 24) | \
     (((x) & 0x000000000000ff00##sfx) << 40) | \
     (((x) & 0x00000000000000ff##sfx) << 56))
#if defined(__GNUC__)
# define uswap_64(x) _uswap_64(x, ull)
#else
# define uswap_64(x) _uswap_64(x, )
#endif
/*********************************************************************************************************
** �����ֽ���ת��
*********************************************************************************************************/
#if BYTE_ORDER == LITTLE_ENDIAN
# define cpu_to_le16(x)     (x)
# define cpu_to_le32(x)     (x)
# define cpu_to_le64(x)     (x)
# define le16_to_cpu(x)     (x)
# define le32_to_cpu(x)     (x)
# define le64_to_cpu(x)     (x)
# define cpu_to_be16(x)     uswap_16(x)
# define cpu_to_be32(x)     uswap_32(x)
# define cpu_to_be64(x)     uswap_64(x)
# define be16_to_cpu(x)     uswap_16(x)
# define be32_to_cpu(x)     uswap_32(x)
# define be64_to_cpu(x)     uswap_64(x)
#else
# define cpu_to_le16(x)     uswap_16(x)
# define cpu_to_le32(x)     uswap_32(x)
# define cpu_to_le64(x)     uswap_64(x)
# define le16_to_cpu(x)     uswap_16(x)
# define le32_to_cpu(x)     uswap_32(x)
# define le64_to_cpu(x)     uswap_64(x)
# define cpu_to_be16(x)     (x)
# define cpu_to_be32(x)     (x)
# define cpu_to_be64(x)     (x)
# define be16_to_cpu(x)     (x)
# define be32_to_cpu(x)     (x)
# define be64_to_cpu(x)     (x)
#endif

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  KERN_TYPES_H_               */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
