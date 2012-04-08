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
** File name:               heap.h
** Last modified Date:      2012-2-24
** Last Version:            1.0.0
** Descriptions:            堆内存管理
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
#ifndef HEAP_H_
#define HEAP_H_

#include "kern/types.h"

/*
 * 内存块
 */
struct mem_block;
typedef struct mem_block mem_block_t;

/*
 * 内存堆
 */
typedef struct {
    uint32_t        magic0;                                             /*  魔数                        */
    mem_block_t    *free_list;                                          /*  空闲内存块链表              */
    mem_block_t    *block_list;                                         /*  内存块链表                  */
    uint8_t        *base;                                               /*  基址                        */
    uint32_t        size;                                               /*  大小                        */
    uint32_t        block_nr;                                           /*  内存块数目                  */
    uint32_t        used_size;                                          /*  已用大小                    */
    uint32_t        alloc_cnt;                                          /*  分配次数                    */
    uint32_t        free_cnt;                                           /*  释放次数                    */
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

/*
 * 打印内存堆信息到文件
 */
int heap_show(heap_t *heap, int fd);

#endif                                                                  /*  HEAP_H_                     */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
