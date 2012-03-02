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
** Descriptions:            堆内存管理
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-23
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/heap.h"
#include "kern/sys_call.h"

/*
 * 进程代码项目应享有一份该文件, 用于实现用户态的 malloc 等, 因进程里使用非抢占的 pthread, 免锁免关中断
 */
#ifdef SMILEOS_KERNEL
#define getpid()          current->tid
#define debug_output      printk
#else
extern int printf(const char *fmt, ...);
#define debug_output      printf

static heap_t  process_heap;

static uint8_t process_heap_mem[PROCESS_HEAP_SIZE];

/*
 * 从堆分配内存
 */
void *malloc(uint32_t size)
{
    void *ptr;

    ptr = heap_alloc(&process_heap, size);

    return ptr;
}

/*
 * 释放内存回堆
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
 * 初始化 c 库
 */
void libc_init(void)
{
    heap_init(&process_heap, process_heap_mem, PROCESS_HEAP_SIZE);
}
#endif

/*
 * 内存块状态
 */
#define MEM_BLOCK_STATE_FREE        0
#define MEM_BLOCK_STATE_USED        (1 << 4)

/*
 * 内存块魔数
 */
#define MEM_BLOCK_MAGIC0            0xA71B26E5

/*
 * 内存块
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
 * 初始化内存堆
 */
int heap_init(heap_t *heap, uint8_t *base, uint32_t size)
{
    heap->base = MEM_ALIGN(base);                                       /*  对齐基址                    */

    heap->size = MEM_ALIGN_SIZE_LESS(size - (heap->base - base));       /*  对齐大小                    */

    heap->block_list            = (mem_block_t *)heap->base;            /*  内存块链表                  */
    heap->block_list->magic0    = MEM_BLOCK_MAGIC0;
    heap->block_list->prev      = NULL;
    heap->block_list->next      = NULL;
    heap->block_list->prev_free = NULL;
    heap->block_list->next_free = NULL;
    heap->block_list->size      = heap->size - MEM_ALIGN_SIZE(sizeof(mem_block_t));
    heap->block_list->status    = MEM_BLOCK_STATE_FREE;

    heap->free_list             = heap->block_list;                     /*  空闲内存块链表              */

    heap->block_cnt             = 1;
    heap->used_size             = 0;
    heap->alloc_cnt             = 0;
    heap->free_cnt              = 0;

    return 0;
}

/*
 * 分配内存
 */
void *heap_alloc(heap_t *heap, uint32_t size)
{
    mem_block_t *blk, *new_blk;

    if (heap->free_list != NULL) {                                      /*  有空闲内存块                */

        size = MEM_ALIGN_SIZE(size);                                    /*  对齐大小                    */
        if (size == 0) {
            size = MEM_ALIGNMENT;
        }

        blk = heap->free_list;                                          /*  从空闲内存块链表里找出一个  */
        while (blk != NULL && blk->size < size) {                       /*  首次满足大小的空闲内存块    */
            blk = blk->next_free;
        }

        if (blk == NULL) {                                              /*  没找到                      */
            goto error0;
        }

        if (blk->size <= MEM_ALIGN_SIZE(sizeof(mem_block_t)) + size) {  /*  如果内存块切割后, 剩余的大小*/
                                                                        /*  不足或只够建立一个内存块节点*/
                                                                        /*  则整块分配出去              */
            if (blk == heap->free_list) {                               /*  处理空闲内存块链表链头      */
                heap->free_list = blk->next_free;
            }

            if (blk->prev_free != NULL) {                               /*  从空闲内存块链表中删除内存块*/
                blk->prev_free->next_free = blk->next_free;
            }
            if (blk->next_free != NULL) {
                blk->next_free->prev_free = blk->prev_free;
            }

            blk->status = MEM_BLOCK_STATE_USED;                         /*  改变内存块状态为已用        */

            new_blk = blk;

        } else {

            new_blk = (mem_block_t *)((char *)blk + blk->size - size);  /*  建立一个新的内存块          */

            if (blk->next != NULL) {                                    /*  新内存块加入内存块链表      */
                blk->next->prev = new_blk;
            }
            new_blk->next = blk->next;
            blk->next     = new_blk;
            new_blk->prev = blk;

            new_blk->size   = size;
            new_blk->status = MEM_BLOCK_STATE_USED;                     /*  新内存块状态为已用          */
                                                                        /*  所以不在空闲内存块链表      */

            blk->size -= MEM_ALIGN_SIZE(sizeof(mem_block_t)) + size;    /*  原内存块变小                */

            heap->block_cnt++;
        }

        new_blk->magic0 = MEM_BLOCK_MAGIC0;                             /*  魔数                        */

        heap->alloc_cnt++;
        heap->used_size += new_blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

        return (char *)new_blk + MEM_ALIGN_SIZE(sizeof(mem_block_t));
    }

    error0:
    debug_output("%s: process %d low memory!\n", __func__, getpid());

    return NULL;
}

/*
 * 释放内存
 */
void *heap_free(heap_t *heap, void *ptr)
{
    mem_block_t *blk;
    mem_block_t *prev;
    mem_block_t *next;

    if (ptr == NULL) {
        debug_output("%s: process %d memory pointer is NULL!\n", __func__, getpid());
        return ptr;
    }

    if (ptr != MEM_ALIGN(ptr)) {
        debug_output("%s: process %d memory pointer is not aligned!\n", __func__, getpid());
        return ptr;
    }

    if (((uint8_t *)ptr < (heap->base + MEM_ALIGN_SIZE(sizeof(mem_block_t)))) ||
        ((uint8_t *)ptr >= heap->base + heap->size)) {
        debug_output("%s: process %d memory pointer dose not belong to this heap!\n", __func__, getpid());
        return ptr;
    }
                                                                        /*  指针所在的内存块节点        */
    blk  = (mem_block_t *)((char *)ptr - MEM_ALIGN_SIZE(sizeof(mem_block_t)));

    if (blk->magic0 != MEM_BLOCK_MAGIC0 || blk->status != MEM_BLOCK_STATE_USED) {
        debug_output("%s: process %d memory pointer is invalid!\n", __func__, getpid());
        return ptr;
    }

    prev = blk->prev;
    next = blk->next;

    if (next != NULL && next->magic0 != MEM_BLOCK_MAGIC0) {             /*  写缓冲区溢出                */
        debug_output("%s: process %d write buffer over!\n", __func__, getpid());
        return ptr;
    }

    if (prev != NULL && prev->status == MEM_BLOCK_STATE_FREE) {         /*  前一个内存块空闲, 合并之    */
        prev->size += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + blk->size;  /*  前一个内存块变大            */

        prev->next = blk->next;                                         /*  从内存块链表中删除内存块    */
        if (next != NULL) {
            next->prev = prev;
        }

        heap->block_cnt--;

    } else if (next != NULL && next->status == MEM_BLOCK_STATE_FREE) {  /*  后一个内存块空闲, 合并之    */

        blk->size  += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + next->size; /*  内存块变大                  */
        blk->status = MEM_BLOCK_STATE_FREE;                             /*  改变内存块状态为空闲        */

        if (next->next != NULL) {                                       /*  从内存块链表中删除后内存块  */
            next->next->prev = blk;
        }
        blk->next = next->next;
        if (heap->block_list == next) {
            heap->block_list = blk;
        }

        if (next->next_free != NULL) {                                  /*  从空闲块链表中删除后内存块  */
            next->next_free->prev_free = blk;
        }

        if (next->prev_free != NULL) {
            next->prev_free->next_free = blk;
        }

        blk->prev_free = next->prev_free;                               /*  把内存块加入空闲内存块链表  */
        blk->next_free = next->next_free;
        if (next == heap->free_list) {
            heap->free_list = blk;
        }

        heap->block_cnt--;
    } else {
        blk->prev_free = NULL;                                          /*  把内存块加入空闲内存块链表  */
        blk->next_free = heap->free_list;
        if (heap->free_list != NULL) {
            heap->free_list->prev_free = blk;
        } else {
            heap->free_list = blk;
        }
        blk->status = MEM_BLOCK_STATE_FREE;                             /*  改变内存块状态为空闲        */
    }

    heap->free_cnt++;
    heap->used_size -= blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

#if 0
    debug_output("heap block count = %d\n", heap->block_cnt);
    debug_output("heap alloc count = %d\n", heap->alloc_cnt);
    debug_output("heap free  count = %d\n", heap->free_cnt);
    debug_output("heap leak  count = %d\n", heap->alloc_cnt - heap->free_cnt);
    debug_output("heap used  size  = %dMB.%dKB.%dB\n",
            heap->used_size/MB,
            heap->used_size%MB/KB,
            heap->used_size%KB);
    debug_output("heap free  size  = %dMB.%dKB.%dB\n",
            (heap->size-heap->used_size)/MB,
            (heap->size-heap->used_size)%MB/KB,
            (heap->size-heap->used_size)%KB);
#endif

    return NULL;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
