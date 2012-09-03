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
** File name:               heap.h
** Last modified Date:      2012-2-24
** Last Version:            1.0.0
** Descriptions:            ���ڴ����
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
#ifndef HEAP_H_
#define HEAP_H_

#include "kern/types.h"
/*********************************************************************************************************
** ����
*********************************************************************************************************/
/*
 * �ڴ��
 */
struct mem_block;
typedef struct mem_block mem_block_t;

/*
 * �ڴ��
 */
typedef struct {
    uint32_t        magic0;                                             /*  ħ��                        */
    mem_block_t    *free_list;                                          /*  �����ڴ������              */
    mem_block_t    *block_list;                                         /*  �ڴ������                  */
    uint8_t        *base;                                               /*  ��ַ                        */
    uint32_t        size;                                               /*  ��С                        */
    uint32_t        block_nr;                                           /*  �ڴ����Ŀ                  */
    uint32_t        used_size;                                          /*  ���ô�С                    */
    uint32_t        alloc_cnt;                                          /*  �������                    */
    uint32_t        free_cnt;                                           /*  �ͷŴ���                    */
} heap_t;
/*********************************************************************************************************
** Function name:           heap_init
** Descriptions:            �����ڴ��
** input parameters:        heap                �ڴ��
**                          base                �ڴ�����ַ
**                          size                �ڴ�����С
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int heap_init(heap_t *heap, uint8_t *base, uint32_t size);
/*********************************************************************************************************
** Function name:           heap_alloc
** Descriptions:            �����ڴ�
** input parameters:        heap                �ڴ��
**                          func                �����ߵĺ�����
**                          line                �����ߵ��к�
**                          size                ��Ҫ����Ĵ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *heap_alloc(heap_t *heap, const char *func, int line, uint32_t size);
/*********************************************************************************************************
** Function name:           heap_free
** Descriptions:            �ͷ��ڴ�
** input parameters:        heap                �ڴ��
**                          func                �����ߵĺ�����
**                          line                �����ߵ��к�
**                          ptr                 �ڴ�ָ��
** output parameters:       NONE
** Returned value:          NULL OR �ڴ�ָ��
*********************************************************************************************************/
void *heap_free(heap_t *heap, const char *func, int line, void *ptr);
/*********************************************************************************************************
** Function name:           heap_show
** Descriptions:            ��ӡ�ڴ����Ϣ���ļ�
** input parameters:        heap                �ڴ��
**                          fd                  ��������ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int heap_show(heap_t *heap, int fd);

#endif                                                                  /*  HEAP_H_                     */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/