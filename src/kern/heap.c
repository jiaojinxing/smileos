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
** File name:               heap.c
** Last modified Date:      2012-2-23
** Last Version:            1.0.0
** Descriptions:            ��̬�ڴ��
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
#include "kern/heap.h"
#include <string.h>

/*
 * ���̴�����ĿӦ����һ�ݸ��ļ�, ����ʵ���û�̬�� malloc ��
 */
#ifdef SMILEOS_KERNEL

#include "kern/kern.h"

#define getpid()          current->tid
#define debug_output      printk

/*
 * �ں��ڴ��
 */
static heap_t  kern_heap;
static uint8_t kern_heap_mem[KERN_HEAP_SIZE];

/*
 * ���ں��ڴ�ѷ����ڴ�
 */
void *kmalloc(uint32_t size)
{
    void    *ptr;
    uint32_t reg;

    reg = interrupt_disable();

    ptr = heap_alloc(&kern_heap, size);

    interrupt_resume(reg);

    return ptr;
}

/*
 * �ͷ��ڴ���ں��ڴ��
 */
void kfree(void *ptr)
{
    uint32_t reg;

    reg = interrupt_disable();

    heap_free(&kern_heap, ptr);

    interrupt_resume(reg);
}

/*
 * kcalloc
 */
void *kcalloc(uint32_t nelem, uint32_t elsize)
{
    void *ptr;

    ptr = kmalloc(nelem * MEM_ALIGN_SIZE(elsize));
    if (ptr != NULL) {
        memset(ptr, 0, nelem * MEM_ALIGN_SIZE(elsize));
    }
    return ptr;
}

/*
 * �����ں��ڴ��
 */
void kern_heap_create(void)
{
    heap_init(&kern_heap, kern_heap_mem, KERN_HEAP_SIZE);
}

#else

extern int printf(const char *fmt, ...);
#define debug_output      printf

static heap_t  process_heap;

/*
 * malloc
 */
void *malloc(uint32_t size)
{
    void *ptr;
    /*
     * �������ʹ�÷���ռ�� pthread, ��������ж�
     */
    ptr = heap_alloc(&process_heap, size);

    return ptr;
}

/*
 * free
 */
void free(void *ptr)
{
    heap_free(&process_heap, ptr);
}

/*
 * calloc
 */
void *calloc(uint32_t nelem, uint32_t elsize)
{
    void *ptr;

    ptr = malloc(nelem * MEM_ALIGN_SIZE(elsize));
    if (ptr != NULL) {
        memset(ptr, 0, nelem * MEM_ALIGN_SIZE(elsize));
    }
    return ptr;
}

/*
 * ��ʼ�� C ��
 */
void libc_init(void)
{
    extern unsigned char __bss_end;

    /*
     * �� __bss_end ��, ����ջ�ռ�ǰ, �����ڴ��
     */
    heap_init(&process_heap, &__bss_end, PROCESS_SPACE_SIZE - (uint32_t)&__bss_end - PROCESS_STACK_SIZE);
}

#endif

/*
 * �ڴ��״̬
 */
#define MEM_BLOCK_STATE_FREE        0x00
#define MEM_BLOCK_STATE_USED        0xA5

/*
 * �ڴ��ħ��
 */
#define MEM_BLOCK_MAGIC0            0xA71B26E5

/*
 * �ڴ��
 */
struct mem_block {
    uint32_t        magic0;                                             /*  ħ��                        */
    mem_block_t    *prev;                                               /*  ǰ��                        */
    mem_block_t    *next;                                               /*  ����                        */
    mem_block_t    *prev_free;                                          /*  ����ǰ��                    */
    mem_block_t    *next_free;                                          /*  ���к���                    */
    uint32_t        size;                                               /*  ��С                        */
    uint8_t         status;                                             /*  ״̬                        */
};

/*
 * ��ʼ���ڴ��
 */
int heap_init(heap_t *heap, uint8_t *base, uint32_t size)
{
    heap->base = MEM_ALIGN(base);                                       /*  �����ַ                    */

    heap->size = MEM_ALIGN_SIZE_LESS(size - (heap->base - base));       /*  �����С                    */

    heap->block_list            = (mem_block_t *)heap->base;            /*  �ڴ������                  */
    heap->block_list->magic0    = MEM_BLOCK_MAGIC0;
    heap->block_list->prev      = NULL;
    heap->block_list->next      = NULL;
    heap->block_list->prev_free = NULL;
    heap->block_list->next_free = NULL;
    heap->block_list->size      = heap->size - MEM_ALIGN_SIZE(sizeof(mem_block_t));
    heap->block_list->status    = MEM_BLOCK_STATE_FREE;

    heap->free_list             = heap->block_list;                     /*  �����ڴ������              */

    heap->block_cnt             = 1;                                    /*  ��ʼ��ͳ����Ϣ              */
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
    mem_block_t *blk;
    mem_block_t *new_blk;

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

            blk->status = MEM_BLOCK_STATE_USED;                         /*  �ı��ڴ��״̬Ϊ����        */

            new_blk = blk;

        } else {
                                                                        /*  ���ڴ��ĸ߶˵�ַ��        */
            new_blk = (mem_block_t *)((char *)blk + blk->size - size);  /*  ����һ���µ��ڴ��          */

            if (blk->next != NULL) {                                    /*  ���ڴ������ڴ������      */
                blk->next->prev = new_blk;
            }
            new_blk->next = blk->next;
            blk->next     = new_blk;
            new_blk->prev = blk;

            new_blk->size   = size;
            new_blk->status = MEM_BLOCK_STATE_USED;                     /*  ���ڴ��״̬Ϊ����          */
                                                                        /*  ���Բ��ڿ����ڴ������      */

            blk->size -= MEM_ALIGN_SIZE(sizeof(mem_block_t)) + size;    /*  ԭ�ڴ���С                */

            heap->block_cnt++;
        }

        new_blk->magic0 = MEM_BLOCK_MAGIC0;                             /*  ����ħ��                    */

        heap->alloc_cnt++;
        heap->used_size += new_blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

        return (char *)new_blk + MEM_ALIGN_SIZE(sizeof(mem_block_t));
    }

    error0:
    debug_output("%s: process %d low memory!\n", __func__, getpid());

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
        debug_output("%s: process %d memptr=NULL!\n", __func__, getpid());
        return ptr;
    }

    if (ptr != MEM_ALIGN(ptr)) {
        debug_output("%s: process %d memptr=0x%x is not aligned!\n", __func__, getpid(), ptr);
        return ptr;
    }

    if (((uint8_t *)ptr < (heap->base + MEM_ALIGN_SIZE(sizeof(mem_block_t)))) ||
        ((uint8_t *)ptr > heap->base + heap->size - MEM_ALIGNMENT)) {
        debug_output("%s: process %d memptr=0x%x dose not belong to this heap!\n", __func__, getpid(), ptr);
        return ptr;
    }
                                                                        /*  ָ�����ڵ��ڴ��ڵ�        */
    blk  = (mem_block_t *)((char *)ptr - MEM_ALIGN_SIZE(sizeof(mem_block_t)));

    if (blk->magic0 != MEM_BLOCK_MAGIC0 || blk->status != MEM_BLOCK_STATE_USED) {
        debug_output("%s: process %d memptr=0x%x is invalid!\n", __func__, getpid(), ptr);
        return ptr;
    }

    prev = blk->prev;
    next = blk->next;

    if (next != NULL && next->magic0 != MEM_BLOCK_MAGIC0) {             /*  д���������                */
        debug_output("%s: process %d write buffer over, memptr=0x%x!\n", __func__, getpid(), ptr);
        return ptr;
    }

    if (prev != NULL && prev->status == MEM_BLOCK_STATE_FREE) {         /*  ǰһ���ڴ�����, �ϲ�֮    */
        prev->size += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + blk->size;  /*  ǰһ���ڴ����            */

        prev->next = blk->next;                                         /*  ���ڴ��������ɾ���ڴ��    */
        if (next != NULL) {
            next->prev = prev;
        }

        heap->block_cnt--;

    } else if (next != NULL && next->status == MEM_BLOCK_STATE_FREE) {  /*  ��һ���ڴ�����, �ϲ�֮    */

        blk->size  += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + next->size; /*  �ڴ����                  */
        blk->status = MEM_BLOCK_STATE_FREE;                             /*  �ı��ڴ��״̬Ϊ����        */

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
        /*
         * ���ڴ���������ڴ���������ͷ, ����ʱ������ʹ���Ѿ�ʹ�ù����ڴ�,
         * ��Ϊʹ�ù�, ��ô��Щ�ڴ��Ѿ�ӳ���, ���Ѿ�������ҳ��,
         * �������˳���������ٶ�, �ּ����˱�����ʹ�õ�ҳ����
         */
        blk->prev_free = NULL;
        blk->next_free = heap->free_list;
        if (heap->free_list != NULL) {
            heap->free_list->prev_free = blk;
        } else {
            heap->free_list = blk;
        }
        blk->status = MEM_BLOCK_STATE_FREE;                             /*  �ı��ڴ��״̬Ϊ����        */
    }

    heap->free_cnt++;
    heap->used_size -= blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

    return NULL;
}

/*
 * ��ӡ�ڴ����Ϣ
 */
void heap_print_info(heap_t *heap)
{
    debug_output("heap block count = %d\n", heap->block_cnt);
    debug_output("heap alloc count = %d\n", heap->alloc_cnt);
    debug_output("heap free  count = %d\n", heap->free_cnt);
    debug_output("heap leak  count = %d\n", heap->alloc_cnt - heap->free_cnt);
    debug_output("heap used  size  = %dMB.%dKB.%dB\n",
            heap->used_size / MB,
            heap->used_size % MB / KB,
            heap->used_size % KB);
    debug_output("heap free  size  = %dMB.%dKB.%dB\n",
            (heap->size-heap->used_size) / MB,
            (heap->size-heap->used_size) % MB / KB,
            (heap->size-heap->used_size) % KB);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
