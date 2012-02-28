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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/mem.h"
#include "kern/sys_call.h"

/*
 * ���̴�����ĿӦ����һ�ݸ��ļ�, ����ʵ���û�̬�� malloc ��, �����ʹ�÷���ռ�� pthread, ��������ж�
 */
/*
 * ���̲���ʹ�� printk
 */
#ifdef SMILEOS_KERNEL
#define getpid()    0
#else
extern int printf(const char *fmt, ...);
#define printk      printf

static heap_t  process_heap;

static uint8_t process_heap_mem[PROCESS_HEAP_SIZE];

/*
 * �Ӷѷ����ڴ�
 */
void *malloc(uint32_t size)
{
    void *ptr;

    ptr = heap_alloc(&process_heap, size);

    return ptr;
}

/*
 * �ͷ��ڴ�ض�
 */
void free(void *ptr)
{
    heap_free(&process_heap, ptr);
}

/*
 * calloc
 */
void *calloc(unsigned int nelem, unsigned int elsize)
{
    void *ptr = malloc(nelem * MEM_ALIGN_SIZE(elsize));
    if (ptr != NULL) {
        memset(ptr, 0, nelem * MEM_ALIGN_SIZE(elsize));
    }
    return ptr;
}

/*
 * ��ʼ�� c ��
 */
void libc_init(void)
{
    heap_init(&process_heap, process_heap_mem, PROCESS_HEAP_SIZE);
}
#endif

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
struct _mem_block_t {
    uint32_t        magic0;
    mem_block_t    *prev;
    mem_block_t    *next;
    mem_block_t    *prev_free;
    mem_block_t    *next_free;
    uint32_t        size;
    uint8_t         status;
};

/*
 * ��ʼ���ڴ��
 */
int heap_init(heap_t *heap, uint8_t *base, uint32_t size)
{
    heap->base = MEM_ALIGN(base);                                       /*  �����ַ                    */

    heap->size = MEM_ALIGN_SIZE_LESS(size - (heap->base - base));       /*  �����С                    */

    heap->block_list            = (mem_block_t *)heap->base;            /*  �ڴ������                  */
    heap->block_list->magic0    = MEM_BLK_MAGIC0;
    heap->block_list->prev      = NULL;
    heap->block_list->next      = NULL;
    heap->block_list->prev_free = NULL;
    heap->block_list->next_free = NULL;
    heap->block_list->size      = heap->size - MEM_ALIGN_SIZE(sizeof(mem_block_t));
    heap->block_list->status    = MEM_BLK_STAT_FREE;

    heap->free_list             = heap->block_list;                     /*  �����ڴ������              */

    heap->block_cnt             = 1;
    heap->used_size             = 0;
    heap->alloc_cnt             = 0;
    heap->free_cnt              = 0;

    return 0;
}

/*
 * �����ڴ�
 */
void *heap_alloc(heap_t *heap, uint32_t size)
{
    mem_block_t *blk, *new_blk;

    if (heap->free_list != NULL) {                                      /*  �п����ڴ��                */

        size = MEM_ALIGN_SIZE(size);                                    /*  �����С                    */
        if (size == 0) {
            size = MEM_ALIGNMENT;
        }

        blk = heap->free_list;                                          /*  �ӿ����ڴ���������ҳ�һ��  */
        while (blk != NULL && blk->size < size) {                       /*  �״������С�Ŀ����ڴ��    */
            blk = blk->next_free;
        }

        if (blk == NULL) {                                              /*  û�ҵ�                      */
            goto error0;
        }

        if (blk->size <= MEM_ALIGN_SIZE(sizeof(mem_block_t)) + size) {  /*  ����ڴ���и��, ʣ��Ĵ�С*/
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

            new_blk = (mem_block_t *)((char *)blk + blk->size - size);  /*  ����һ���µ��ڴ��          */

            if (blk->next != NULL) {                                    /*  ���ڴ������ڴ������      */
                blk->next->prev = new_blk;
            }
            new_blk->next = blk->next;
            blk->next     = new_blk;
            new_blk->prev = blk;

            new_blk->size   = size;
            new_blk->status = MEM_BLK_STAT_USED;                        /*  ���ڴ��״̬Ϊ����          */
                                                                        /*  ���Բ��ڿ����ڴ������      */

            blk->size -= MEM_ALIGN_SIZE(sizeof(mem_block_t)) + size;    /*  ԭ�ڴ���С                */

            heap->block_cnt++;
        }

        new_blk->magic0 = MEM_BLK_MAGIC0;                               /*  ħ��                        */

        heap->alloc_cnt++;
        heap->used_size += new_blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

        return (char *)new_blk + MEM_ALIGN_SIZE(sizeof(mem_block_t));
    }

    error0:
    printk("%s: process %d low memory!\n", __func__, getpid());

    return NULL;
}

/*
 * �ͷ��ڴ�
 */
void *heap_free(heap_t *heap, void *ptr)
{
    mem_block_t *blk;
    mem_block_t *prev;
    mem_block_t *next;

    if (ptr == NULL) {
        printk("%s: process %d memory pointer is NULL!\n", __func__, getpid());
        return ptr;
    }

    if (ptr != MEM_ALIGN(ptr)) {
        printk("%s: process %d memory pointer is not aligned!\n", __func__, getpid());
        return ptr;
    }

    if (((uint8_t *)ptr < (heap->base + MEM_ALIGN_SIZE(sizeof(mem_block_t)))) ||
        ((uint8_t *)ptr >= heap->base + heap->size)) {
        printk("%s: process %d memory pointer dose not belong to this heap!\n", __func__, getpid());
        return ptr;
    }
                                                                        /*  ָ�����ڵ��ڴ��ڵ�        */
    blk  = (mem_block_t *)((char *)ptr - MEM_ALIGN_SIZE(sizeof(mem_block_t)));

    if (blk->magic0 != MEM_BLK_MAGIC0 || blk->status != MEM_BLK_STAT_USED) {
        printk("%s: process %d memory pointer is invalid!\n", __func__, getpid());
        return ptr;
    }

    prev = blk->prev;
    next = blk->next;

    if (next != NULL && next->magic0 != MEM_BLK_MAGIC0) {               /*  д���������                */
        printk("%s: process %d write buffer over!\n", __func__, getpid());
        return ptr;
    }

    if (prev != NULL && prev->status == MEM_BLK_STAT_FREE) {            /*  ǰһ���ڴ�����, �ϲ�֮    */
        prev->size += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + blk->size;  /*  ǰһ���ڴ����            */

        prev->next = blk->next;                                         /*  ���ڴ��������ɾ���ڴ��    */
        if (next != NULL) {
            next->prev = prev;
        }

        heap->block_cnt--;

    } else if (next != NULL && next->status == MEM_BLK_STAT_FREE) {     /*  ��һ���ڴ�����, �ϲ�֮    */

        blk->size  += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + next->size; /*  �ڴ����                  */
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

        heap->block_cnt--;
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

    heap->free_cnt++;
    heap->used_size -= blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

#if 0
    printk("heap block count = %d\n", heap->block_cnt);
    printk("heap alloc count = %d\n", heap->alloc_cnt);
    printk("heap free  count = %d\n", heap->free_cnt);
    printk("heap leak  count = %d\n", heap->alloc_cnt - heap->free_cnt);
    printk("heap used  size  = %dMB.%dKB.%dB\n",
            heap->used_size/MB,
            heap->used_size%MB/KB,
            heap->used_size%KB);
    printk("heap free  size  = %dMB.%dKB.%dB\n",
            (heap->size-heap->used_size)/MB,
            (heap->size-heap->used_size)%MB/KB,
            (heap->size-heap->used_size)%KB);
#endif

    return NULL;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
