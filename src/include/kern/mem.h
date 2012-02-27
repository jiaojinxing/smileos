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
** File name:               mem.h
** Last modified Date:      2012-2-24
** Last Version:            1.0.0
** Descriptions:            内存管理
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-24
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
#ifndef MEM_H_
#define MEM_H_

#include "types.h"

/*
 * 内存块
 */
struct _mem_block_t;
typedef struct _mem_block_t mem_block_t;

/*
 * 内存堆
 */
typedef struct {
    mem_block_t    *free_list;
    mem_block_t    *block_list;
    uint8_t        *base;
    uint32_t        size;
    uint32_t        block_cnt;
    uint32_t        used_size;
    uint32_t        alloc_cnt;
    uint32_t        free_cnt;
} heap_t;

/*
 * 初始化内存堆
 */
int heap_init(heap_t *heap, uint8_t *base, uint32_t size);

/*
 * 分配内存
 */
void *heap_alloc(heap_t *heap, uint32_t size);

/*
 * 释放内存
 */
void *heap_free(heap_t *heap, void *ptr);

#endif                                                                  /*  MEM_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
