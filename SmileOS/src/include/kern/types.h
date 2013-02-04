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

/*
 * �ֽ���
 */
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN               1234
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN                  4321
#endif

#include "arch/types.h"

typedef mem_ptr_t                   mem_size_t;

/*
 * �ڴ�ռ�
 */
typedef struct {
    mem_ptr_t                       base;
    mem_size_t                      size;
} mem_space_t;

/*
 * TICK ��������
 */
typedef uint64_t                    tick_t;
/*********************************************************************************************************
** ϵͳ�������Ͷ���
*********************************************************************************************************/
/*
 * �����ڴ����
 */
#define MEM_ALIGN_SIZE(size)        (((size) + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT - 1))
#define MEM_ALIGN_SIZE_LESS(size)   (((size) & ~(MEM_ALIGNMENT - 1)))
#define MEM_ALIGN(addr)             ((void *)(((mem_ptr_t)(addr) + MEM_ALIGNMENT - 1) & ~(mem_ptr_t)(MEM_ALIGNMENT - 1)))
#define MEM_ALIGN_LESS(addr)        ((void *)(((mem_ptr_t)(addr)) & ~(mem_ptr_t)(MEM_ALIGNMENT - 1)))

/*
 * �������ṹ����
 */
#define PACK_STRUCT_FIELD(x)        x
#define PACK_STRUCT_STRUCT          __attribute__((packed))
#define PACK_STRUCT_BEGIN                                               /*  ���ֽ����Žṹ��            */
#define PACK_STRUCT_END                                                 /*  �������ֽ����Žṹ��        */

/*
 * ��������Сֵ
 */
#define max(a, b)                   (a) > (b) ? (a) : (b)
#define min(a, b)                   (a) < (b) ? (a) : (b)

/*
 * ��д�ڴ��ַ
 */
#define writeb(d, r)                (*(volatile uint8_t  *)r) = (d)
#define writew(d, r)                (*(volatile uint16_t *)r) = (d)
#define writel(d, r)                (*(volatile uint32_t *)r) = (d)
#define readb(r)                    (*(volatile uint8_t  *)r)
#define readw(r)                    (*(volatile uint16_t *)r)
#define readl(r)                    (*(volatile uint8_t  *)r)

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  KERN_TYPES_H_               */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
