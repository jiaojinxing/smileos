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
** File name:               heap.c
** Last modified Date:      2012-2-23
** Last Version:            1.0.0
** Descriptions:            �ڴ��
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
#include "mm/heap.h"
#include <string.h>
#include <errno.h>
/*********************************************************************************************************
** ����
*********************************************************************************************************/
#ifdef SMILEOS_KERNEL
#include "kern/kern.h"
#define debug               printk
#else
#define debug               printf
#endif

/*
 * �ڴ��ħ��
 */
#define MEM_BLOCK_MAGIC     0xAA5555AA

/*
 * �ڴ��״̬(�൱�ں�ħ��)
 */
#define MEM_BLOCK_FREE      0x12345678
#define MEM_BLOCK_USED      0x87654321

/*
 * �ڴ��
 */
struct mem_block {
    uint32_t                magic;                                      /*  ħ��                        */
    mem_block_t            *prev;                                       /*  ǰ��                        */
    mem_block_t            *next;                                       /*  ����                        */
    mem_block_t            *prev_free;                                  /*  ����ǰ��                    */
    mem_block_t            *next_free;                                  /*  ���к���                    */
    size_t                  size;                                       /*  ��С                        */
    const char             *func;                                       /*  ���亯��                    */
    int                     line;                                       /*  �����к�                    */
    uint32_t                status;                                     /*  ״̬                        */
};
/*********************************************************************************************************
** Function name:           heap_init
** Descriptions:            �����ڴ��
** input parameters:        heap                �ڴ��
**                          name                ����
**                          base                �ڴ�����ַ
**                          size                �ڴ�����С
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int heap_init(heap_t *heap, const char *name, uint8_t *base, size_t size)
{
    if (heap == NULL) {
        debug("%s: heap=NULL!\n", __func__);
        return -1;
    }

    if (name == NULL) {
        debug("%s: heap name=NULL!\n", __func__);
        return -1;
    }

    if (strlen(name) > sizeof(heap->name) - 1) {
        debug("%s: heap %s name too long!\n", __func__, name);
        return -1;
    }

    if (base == NULL) {
        debug("%s: heap %s base=NULL!\n", __func__, name);
        return -1;
    }

    if (size < 256) {
        debug("%s: heap %s size=%d too small!\n", __func__, name, size);
        return -1;
    }

    heap->base = MEM_ALIGN(base);                                       /*  �����ַ                    */

    heap->size = MEM_ALIGN_SIZE_LESS(size - (heap->base - base));       /*  �����С                    */

    heap->block_list            = (mem_block_t *)heap->base;            /*  �ڴ������                  */
    heap->block_list->magic     = MEM_BLOCK_MAGIC;
    heap->block_list->prev      = NULL;
    heap->block_list->next      = NULL;
    heap->block_list->prev_free = NULL;
    heap->block_list->next_free = NULL;
    heap->block_list->size      = heap->size - MEM_ALIGN_SIZE(sizeof(mem_block_t));
    heap->block_list->status    = MEM_BLOCK_FREE;
    heap->block_list->func      = __func__;
    heap->block_list->line      = __LINE__;

    heap->free_list             = heap->block_list;                     /*  �����ڴ������              */

    heap->block_nr              = 1;                                    /*  ��ʼ��ͳ����Ϣ              */
    heap->used_size             = 0;
    heap->alloc_cnt             = 0;
    heap->free_cnt              = 0;

    heap->magic                 = MEM_BLOCK_MAGIC;                      /*  ����ħ��                    */

    strlcpy(heap->name, name, sizeof(heap->name));                      /*  ����                        */

    return 0;
}
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
void *heap_alloc(heap_t *heap, const char *func, int line, size_t size)
{
    mem_block_t *blk;
    mem_block_t *new_blk;

    if (heap == NULL) {
        debug("%s: heap=NULL, call by %s() line %d\n",
                __func__, func, line);
        return NULL;
    }

    if (heap->magic != MEM_BLOCK_MAGIC) {
        debug("%s: heap magic %d != %d, call by %s() line %d\n",
                __func__, heap->magic, MEM_BLOCK_MAGIC, func, line);
        return NULL;
    }

    if (heap->free_list != NULL) {                                      /*  �п����ڴ��                */

        size = MEM_ALIGN_SIZE(size);                                    /*  �����С                    */
        if (size == 0) {
            size = MEM_ALIGNMENT;
        }

        blk = heap->free_list;                                          /*  �ӿ����ڴ���������ҳ�һ��  */
        while (blk != NULL && blk->size < size) {                       /*  �״������С�Ŀ����ڴ��    */
            if (blk->magic != MEM_BLOCK_MAGIC || blk->status != MEM_BLOCK_FREE) {
                heap_check(heap, func, line);
                goto error;
            }
            blk = blk->next_free;
        }

        if (blk == NULL) {                                              /*  û�ҵ�                      */
            debug("%s: heap %s low memory, call by %s() line %d, request %d byte\n",
                    __func__, heap->name, func, line, size);
            goto error;
        }

        if (blk->magic != MEM_BLOCK_MAGIC || blk->status != MEM_BLOCK_FREE) {
            heap_check(heap, func, line);
            goto error;
        }

        if (blk->size <= MEM_ALIGN_SIZE(sizeof(mem_block_t)) + size) {  /*  ����ڴ���и��, ʣ��Ĵ�С*/
                                                                        /*  �����ֻ������һ���ڴ��ڵ�*/
                                                                        /*  ����������ȥ              */
            if (heap->free_list == blk) {                               /*  �ӿ����ڴ��������ɾ���ڴ��*/
                heap->free_list = blk->next_free;
            }

            if (blk->prev_free != NULL) {
                blk->prev_free->next_free = blk->next_free;
            }
            if (blk->next_free != NULL) {
                blk->next_free->prev_free = blk->prev_free;
            }

            blk->status = MEM_BLOCK_USED;                               /*  �ı��ڴ��״̬Ϊ����        */

            new_blk = blk;
        } else {
                                                                        /*  ���ڴ��ĸ߶˵�ַ��        */
            new_blk = (mem_block_t *)((char *)blk + blk->size - size);  /*  ����һ���µ��ڴ��          */
            /*
             * ����һ�����������ļ򻯰汾:
             *
             * new_blk = (mem_block_t *)((char *)blk + MEM_ALIGN_SIZE(sizeof(mem_block_t)) +
             *           blk->size - size - MEM_ALIGN_SIZE(sizeof(mem_block_t)));
             */

            if (blk->next != NULL) {                                    /*  ���ڴ������ڴ������      */
                blk->next->prev = new_blk;
            }
            new_blk->next = blk->next;
            blk->next     = new_blk;
            new_blk->prev = blk;

            new_blk->size   = size;
            new_blk->status = MEM_BLOCK_USED;                           /*  ���ڴ��״̬Ϊ����          */
                                                                        /*  ���Բ��ڿ����ڴ������      */

            blk->size -= MEM_ALIGN_SIZE(sizeof(mem_block_t)) + size;    /*  ԭ�ڴ���С                */

            heap->block_nr++;
        }

        new_blk->func  = func;
        new_blk->line  = line;

        new_blk->magic = MEM_BLOCK_MAGIC;                               /*  ����ħ��                    */

        heap->alloc_cnt++;
        heap->used_size += new_blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

        return (char *)new_blk + MEM_ALIGN_SIZE(sizeof(mem_block_t));
    }

    error:
    return NULL;
}
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
void *heap_free(heap_t *heap, const char *func, int line, void *ptr)
{
    mem_block_t *blk;
    mem_block_t *prev;
    mem_block_t *next;

    if (heap == NULL) {
        debug("%s: heap=NULL, call by %s() line %d\n",
                __func__, func, line);
        return NULL;
    }

    if (heap->magic != MEM_BLOCK_MAGIC) {
        debug("%s: heap magic %d != %d, call by %s() line %d\n",
                __func__, heap->magic, MEM_BLOCK_MAGIC, func, line);
        return NULL;
    }

    if (ptr == NULL) {
#ifdef SMILEOS_KERNEL
        debug("%s: memptr=NULL, call by %s() line %d\n",
                __func__, func, line);
#endif
        return ptr;
    }

    if (ptr != MEM_ALIGN(ptr)) {
        debug("%s: memptr=0x%x is not aligned, call by %s() line %d\n",
                __func__, (uint32_t)ptr, func, line);
        return ptr;
    }

    if (((uint8_t *)ptr < (heap->base + MEM_ALIGN_SIZE(sizeof(mem_block_t)))) ||
        ((uint8_t *)ptr > heap->base + heap->size - MEM_ALIGNMENT)) {
        debug("%s: memptr=0x%x dose not belong to heap %s, call by %s() line %d\n",
                __func__, (uint32_t)ptr, heap->name, func, line);
        return ptr;
    }
                                                                        /*  ָ�����ڵ��ڴ��ڵ�        */
    blk  = (mem_block_t *)((char *)ptr - MEM_ALIGN_SIZE(sizeof(mem_block_t)));

    if (blk->magic != MEM_BLOCK_MAGIC || blk->status != MEM_BLOCK_USED) {
        debug("%s: memptr=0x%x is invalid, call by %s() line %d\n",
                __func__, (uint32_t)ptr, func, line);
        /*
         * Ҳ������ǰһ���ڴ��д�������ڴ����ǰд���
         */
        return ptr;
    }

    prev = blk->prev;
    next = blk->next;

    if (next != NULL && next->magic != MEM_BLOCK_MAGIC) {               /*  д���������                */
        debug("%s: write mem=0x%x(size=%d @heap %s) overflow, alloc by %s() line %d\n",
                __func__, (uint32_t)ptr, blk->size, heap->name, blk->func, blk->line);
        return ptr;
    }

    heap->free_cnt++;
    heap->used_size -= blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

    if (prev != NULL && prev->status == MEM_BLOCK_FREE) {               /*  ǰһ���ڴ�����, �ϲ�֮    */

        prev->size += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + blk->size;  /*  ǰһ���ڴ����            */

        prev->next = blk->next;                                         /*  ���ڴ��������ɾ���ڴ��    */
        if (next != NULL) {
            next->prev = prev;
        }
        heap->block_nr--;

        if (next != NULL && next->status == MEM_BLOCK_FREE) {           /*  ��һ���ڴ�����, �ϲ�֮    */

            blk = prev;

            blk->size += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + next->size;  /*  �ڴ����              */

            blk->next = next->next;                                     /*  ���ڴ��������ɾ�����ڴ��  */
            if (next->next != NULL) {
                next->next->prev = blk;
            }
            heap->block_nr--;

            if (next->next_free != NULL) {                              /*  �ӿ��п�������ɾ�����ڴ��  */
                next->next_free->prev_free = next->prev_free;
            }

            if (next->prev_free != NULL) {
                next->prev_free->next_free = next->next_free;
            }

            if (heap->free_list == next) {
                heap->free_list = next->next_free;
            }
        }
    } else if (next != NULL && next->status == MEM_BLOCK_FREE) {        /*  ��һ���ڴ�����, �ϲ�֮    */

        blk->size  += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + next->size; /*  �ڴ����                  */
        blk->status = MEM_BLOCK_FREE;                                   /*  �ı��ڴ��״̬Ϊ����        */

        blk->next = next->next;                                         /*  ���ڴ��������ɾ�����ڴ��  */
        if (next->next != NULL) {
            next->next->prev = blk;
        }
        heap->block_nr--;

        if (heap->free_list == next) {                                  /*  �ӿ��п�������ɾ�����ڴ��  */
            heap->free_list = next->next_free;
        }

        if (next->next_free != NULL) {
            next->next_free->prev_free = next->prev_free;
        }

        if (next->prev_free != NULL) {
            next->prev_free->next_free = next->next_free;
        }

        blk->next_free = heap->free_list;                               /*  ���ڴ���������ڴ������  */
        blk->prev_free = NULL;
        if (heap->free_list != NULL) {
            heap->free_list->prev_free = blk;
        }
        heap->free_list = blk;
    } else {
        /*
         * ���ڴ���������ڴ���������ͷ, ����ʱ������ʹ���Ѿ�ʹ�ù����ڴ�,
         * ��Ϊʹ�ù�, ��ô��Щ�ڴ��Ѿ�ӳ���, ���Ѿ�����������ҳ��,
         * �������˳���������ٶ�, �ּ����˱�����ʹ�õ�����ҳ����
         */
        blk->status = MEM_BLOCK_FREE;                                   /*  �ı��ڴ��״̬Ϊ����        */
        blk->prev_free = NULL;
        blk->next_free = heap->free_list;
        if (heap->free_list != NULL) {
            heap->free_list->prev_free = blk;
        }
        heap->free_list = blk;
    }

    return NULL;
}
/*********************************************************************************************************
** Function name:           heap_check
** Descriptions:            ����ڴ�ѵ�ʹ�����
** input parameters:        heap                �ڴ��
**                          func                �����ߵĺ�����
**                          line                �����ߵ��к�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int heap_check(heap_t *heap, const char *func, int line)
{
    mem_block_t *blk;
    mem_block_t *prev;

    if (heap == NULL) {
        debug("%s: heap=NULL, call by %s() line %d\n",
                __func__, func, line);
        return -1;
    }

    if (heap->magic != MEM_BLOCK_MAGIC) {
        debug("%s: heap magic %d != %d, call by %s() line %d\n",
                __func__, heap->magic, MEM_BLOCK_MAGIC, func, line);
        return -1;
    }

    blk  = heap->block_list;
    prev = NULL;

    while (blk != NULL) {

        if (blk->magic != MEM_BLOCK_MAGIC) {
            if (prev != NULL) {
                debug("%s: write mem=0x%x(size=%d @heap %s) overflow, alloc by %s() line %d\n",
                        __func__, (uint32_t)(char *)prev + MEM_ALIGN_SIZE(sizeof(mem_block_t)),
                        prev->size, heap->name, prev->func, prev->line);
            } else {

            }
            return -1;
        }

        prev = blk;
        blk  = blk->next;
    }

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
