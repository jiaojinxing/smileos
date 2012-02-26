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
** File name:               mem.c
** Last modified Date:      2012-2-23
** Last Version:            1.0.0
** Descriptions:            �ڴ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-23
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
#include "config.h"
#include "types.h"
#include "kern.h"
#include "mem.h"

/*
 * �ڴ��״̬
 */
#define MEM_BLK_STAT_FREE           0
#define MEM_BLK_STAT_USED           (1 << 4)

/*
 * �ڴ��ħ��
 */
#define MEM_BLK_MAGIC0              0xA71B26E5

/*
 * �ڴ��
 */
struct _mem_block {
    uint32_t        magic0;
    mem_block      *prev;
    mem_block      *next;
    mem_block      *prev_free;
    mem_block      *next_free;
    uint32_t        size;
    uint8_t         status;
};

/*
 * ��ʼ���ڴ��
 */
int mem_heap_init(mem_heap *heap, uint8_t *base, uint32_t size)
{
    heap->base = MEM_ALIGN(base);                                       /*  �����ַ                    */

    heap->size = MEM_ALIGN_SIZE_LESS(size - (heap->base - base));       /*  �����С                    */

    heap->block_list            = (mem_block *)heap->base;              /*  �ڴ������                  */
    heap->block_list->magic0    = MEM_BLK_MAGIC0;
    heap->block_list->prev      = NULL;
    heap->block_list->next      = NULL;
    heap->block_list->prev_free = NULL;
    heap->block_list->next_free = NULL;
    heap->block_list->size      = heap->size - MEM_ALIGN_SIZE(sizeof(mem_block));
    heap->block_list->status    = MEM_BLK_STAT_FREE;

    heap->free_list             = heap->block_list;                     /*  �����ڴ������              */

    return 0;
}

/*
 * �����ڴ�
 */
void *mem_heap_alloc(mem_heap *heap, uint32_t size)
{
    mem_block *blk, *new_blk;

    if (heap->free_list != NULL) {                                      /*  �п����ڴ��                */

        size = MEM_ALIGN_SIZE(size);                                    /*  �����С                    */

        blk = heap->free_list;                                          /*  �ӿ����ڴ���������ҳ�һ��  */
        while (blk != NULL && blk->size < size) {                       /*  �״������С�Ŀ����ڴ��    */
            blk = blk->next_free;
        }

        if (blk == NULL) {                                              /*  û�ҵ�                      */
            goto error0;
        }

        if (blk->size <= MEM_ALIGN_SIZE(sizeof(mem_block)) + size) {    /*  ����ڴ���и��, ʣ��Ĵ�С*/
                                                                        /*  �����ֻ������һ���ڴ��ڵ�*/
                                                                        /*  ����������ȥ              */
            if (blk == heap->free_list) {                               /*  ��������ڴ��������ͷ      */
                heap->free_list = blk->next_free;
            }

            if (blk->prev_free != NULL) {                               /*  �ӿ����ڴ��������ɾ���ڴ��*/
                blk->prev_free->next_free = blk->next_free;
            }
            if (blk->next_free != NULL) {
                blk->next_free->prev_free = blk->prev_free;
            }

            blk->status = MEM_BLK_STAT_USED;                            /*  �ı��ڴ��״̬Ϊ����        */

            new_blk = blk;

        } else {

            new_blk = (mem_block *)((char *)blk + blk->size - size);    /*  ����һ���µ��ڴ��          */

            if (blk->next != NULL) {                                    /*  ���ڴ������ڴ������      */
                blk->next->prev = new_blk;
            }
            new_blk->next = blk->next;
            blk->next     = new_blk;
            new_blk->prev = blk;

            new_blk->size   = size;
            new_blk->status = MEM_BLK_STAT_USED;                        /*  ���ڴ��״̬Ϊ����          */
                                                                        /*  ���Բ��ڿ����ڴ������      */

            blk->size -= MEM_ALIGN_SIZE(sizeof(mem_block)) + size;      /*  ԭ�ڴ���С                */
        }

        new_blk->magic0 = MEM_BLK_MAGIC0;                               /*  ħ��                        */

        return (char *)new_blk + MEM_ALIGN_SIZE(sizeof(mem_block));
    }

    error0:
    printk("%s: process %d low memory!\n", __func__, current->pid);

    return NULL;
}

/*
 * �ͷ��ڴ�
 */
void *mem_heap_free(mem_heap *heap, void *ptr)
{
    mem_block *blk;
    mem_block *prev;
    mem_block *next;

    if (ptr == NULL) {
        printk("%s: memory pointer is NULL!\n", __func__, current->tid);
        return ptr;
    }

    if (ptr != MEM_ALIGN(ptr)) {
        printk("%s: memory pointer is not aligned!\n", __func__, current->tid);
        return ptr;
    }

    if (((uint8_t *)ptr < (heap->base + MEM_ALIGN_SIZE(sizeof(mem_block)))) ||
        ((uint8_t *)ptr >= heap->base + heap->size)) {
        printk("%s: memory pointer dose not belong to this heap!\n", __func__, current->tid);
        return ptr;
    }
                                                                        /*  ָ�����ڵ��ڴ��ڵ�        */
    blk  = (mem_block *)((char *)ptr - MEM_ALIGN_SIZE(sizeof(mem_block)));

    if (blk->magic0 != MEM_BLK_MAGIC0 || blk->status != MEM_BLK_STAT_USED) {
        printk("%s: memory pointer is invalid!\n", __func__, current->tid);
        return ptr;
    }

    prev = blk->prev;
    next = blk->next;

    if (next != NULL && next->magic0 != MEM_BLK_MAGIC0) {               /*  д���������                */
        printk("%s: write buffer over!\n", __func__, current->tid);
        return ptr;
    }

    if (prev != NULL && prev->status == MEM_BLK_STAT_FREE) {            /*  ǰһ���ڴ�����, �ϲ�֮    */
        prev->size += MEM_ALIGN_SIZE(sizeof(mem_block)) + blk->size;    /*  ǰһ���ڴ����            */

        prev->next = blk->next;                                         /*  ���ڴ��������ɾ���ڴ��    */
        if (next != NULL) {
            next->prev = prev;
        }

    } else if (next != NULL && next->status == MEM_BLK_STAT_FREE) {     /*  ��һ���ڴ�����, �ϲ�֮    */

        blk->size  += MEM_ALIGN_SIZE(sizeof(mem_block)) + next->size;   /*  �ڴ����                  */
        blk->status = MEM_BLK_STAT_FREE;                                /*  �ı��ڴ��״̬Ϊ����        */

        if (next->next != NULL) {                                       /*  ���ڴ��������ɾ�����ڴ��  */
            next->next->prev = blk;
        }
        blk->next = next->next;
        if (heap->block_list == next) {
            heap->block_list = blk;
        }

        if (next->next_free != NULL) {                                  /*  �ӿ��п�������ɾ�����ڴ��  */
            next->next_free->prev_free = blk;
        }

        if (next->prev_free != NULL) {
            next->prev_free->next_free = blk;
        }

        blk->prev_free = next->prev_free;                               /*  ���ڴ���������ڴ������  */
        blk->next_free = next->next_free;
        if (next == heap->free_list) {
            heap->free_list = blk;
        }
    } else {
        blk->prev_free = NULL;                                          /*  ���ڴ���������ڴ������  */
        blk->next_free = heap->free_list;
        if (heap->free_list != NULL) {
            heap->free_list->prev_free = blk;
        } else {
            heap->free_list = blk;
        }
        blk->status = MEM_BLK_STAT_FREE;                                /*  �ı��ڴ��״̬Ϊ����        */
    }

    return NULL;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
