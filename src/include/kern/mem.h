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
** Descriptions:            �ڴ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-24
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
#ifndef MEM_H_
#define MEM_H_

#include "types.h"

/*
 * �ڴ��
 */
struct _mem_block;
typedef struct _mem_block mem_block;

/*
 * �ڴ��
 */
typedef struct {
    mem_block      *free_list;
    mem_block      *block_list;
    uint8_t        *base;
    uint32_t        size;
    uint32_t        block_cnt;
    uint32_t        used_size;
    uint32_t        alloc_cnt;
    uint32_t        free_cnt;
} mem_heap;

/*
 * ��ʼ���ڴ��
 */
int mem_heap_init(mem_heap *heap, uint8_t *base, uint32_t size);

/*
 * �����ڴ�
 */
void *mem_heap_alloc(mem_heap *heap, uint32_t size);

/*
 * �ͷ��ڴ�
 */
void *mem_heap_free(mem_heap *heap, void *ptr);

#endif                                                                  /*  MEM_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
