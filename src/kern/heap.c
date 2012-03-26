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
** Descriptions:            动态内存堆
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
#include "kern/heap.h"
#include <string.h>

/*
 * 进程代码项目应享有一份该文件, 用于实现用户态的 malloc 等
 */
#ifdef SMILEOS_KERNEL

#include "kern/kern.h"

#define getpid()          current->tid
/*
 * printk 会使用内存分配, 不使用 printk
 */
#define debug_output      kcomplain

/*
 * 内核内存堆
 */
static heap_t  kern_heap;
static uint8_t kern_heap_mem[KERN_HEAP_SIZE];

/*
 * 从内核内存堆分配内存
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
 * 释放内存回内核内存堆
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
 * 打印内核内存堆信息
 */
void kern_heap_show(void)
{
    uint32_t reg;

    reg = interrupt_disable();

    heap_show(&kern_heap);

    interrupt_resume(reg);
}

/*
 * 创建内核内存堆
 */
void kern_heap_create(void)
{
    heap_init(&kern_heap, kern_heap_mem, KERN_HEAP_SIZE);
}

#else

#include "kern/sys_call.h"

#define debug_output      printf

static heap_t process_heap;

/*
 * malloc
 */
void *malloc(uint32_t size)
{
    void *ptr;
    /*
     * 因进程里使用非抢占的 pthread, 免锁免关中断
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
 * 初始化 C 库
 */
void libc_init(void)
{
    extern unsigned char __bss_end;

    /*
     * 在 __bss_end 后, 进程栈空间前, 建立内存堆
     */
    heap_init(&process_heap, &__bss_end, PROCESS_SPACE_SIZE - (uint32_t)&__bss_end - PROCESS_STACK_SIZE);
}

#endif

/*
 * 内存块状态
 */
#define MEM_BLOCK_STATE_FREE        0x00
#define MEM_BLOCK_STATE_USED        0xA5

/*
 * 内存块魔数
 */
#define MEM_BLOCK_MAGIC0            0xA71B26E5

/*
 * 内存块
 */
struct mem_block {
    uint32_t        magic0;                                             /*  魔数                        */
    mem_block_t    *prev;                                               /*  前趋                        */
    mem_block_t    *next;                                               /*  后趋                        */
    mem_block_t    *prev_free;                                          /*  空闲前趋                    */
    mem_block_t    *next_free;                                          /*  空闲后趋                    */
    uint32_t        size;                                               /*  大小                        */
    uint8_t         status;                                             /*  状态                        */
};

/*
 * 创建内存堆
 */
int heap_init(heap_t *heap, uint8_t *base, uint32_t size)
{
    if (heap == NULL) {
        debug_output("%s: process %d heap=NULL!\n", __func__, getpid());
        return -1;
    }

    if (base == NULL) {
        debug_output("%s: process %d base=NULL!\n", __func__, getpid());
        return -1;
    }

    if (size < 1 * KB) {
        debug_output("%s: process %d size=%d too small!\n", __func__, getpid(), size);
        return -1;
    }

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

    heap->block_cnt             = 1;                                    /*  初始化统计信息              */
    heap->used_size             = 0;
    heap->alloc_cnt             = 0;
    heap->free_cnt              = 0;

    heap->magic0                = MEM_BLOCK_MAGIC0;

    return 0;
}

/*
 * 分配内存
 */
void *heap_alloc(heap_t *heap, uint32_t size)
{
    mem_block_t *blk;
    mem_block_t *new_blk;

    if (heap == NULL) {
        debug_output("%s: process %d heap=NULL!\n", __func__, getpid());
        return NULL;
    }

    if (heap->magic0 != MEM_BLOCK_MAGIC0) {
        debug_output("%s: process %d heap magic != %d!\n", __func__, getpid(), MEM_BLOCK_MAGIC0);
        return NULL;
    }

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
            heap_show(heap);
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
                                                                        /*  在内存块的高端地址处        */
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

        new_blk->magic0 = MEM_BLOCK_MAGIC0;                             /*  加入魔数                    */

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

    if (heap == NULL) {
        debug_output("%s: process %d heap=NULL!\n", __func__, getpid());
        return NULL;
    }

    if (ptr == NULL) {
        debug_output("%s: process %d memptr=NULL!\n", __func__, getpid());
        return ptr;
    }

    if (ptr != MEM_ALIGN(ptr)) {
        debug_output("%s: process %d memptr=0x%x is not aligned!\n", __func__, getpid(), (uint32_t)ptr);
        return ptr;
    }

    if (((uint8_t *)ptr < (heap->base + MEM_ALIGN_SIZE(sizeof(mem_block_t)))) ||
        ((uint8_t *)ptr > heap->base + heap->size - MEM_ALIGNMENT)) {
        debug_output("%s: process %d memptr=0x%x dose not belong to this heap!\n", __func__, getpid(), (uint32_t)ptr);
        return ptr;
    }
                                                                        /*  指针所在的内存块节点        */
    blk  = (mem_block_t *)((char *)ptr - MEM_ALIGN_SIZE(sizeof(mem_block_t)));

    if (blk->magic0 != MEM_BLOCK_MAGIC0 || blk->status != MEM_BLOCK_STATE_USED) {
        debug_output("%s: process %d memptr=0x%x is invalid!\n", __func__, getpid(), (uint32_t)ptr);
        return ptr;
    }

    prev = blk->prev;
    next = blk->next;

    if (next != NULL && next->magic0 != MEM_BLOCK_MAGIC0) {             /*  写缓冲区溢出                */
        debug_output("%s: process %d write buffer over, memptr=0x%x!\n", __func__, getpid(), (uint32_t)ptr);
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
        /*
         * 把内存块加入空闲内存块链表的链头, 分配时尽可能使用已经使用过的内存,
         * 因为使用过, 那么这些内存已经映射好, 即已经引入了页框,
         * 既提升了程序的运行速度, 又减少了本进程使用的页框数
         */
        blk->prev_free = NULL;
        blk->next_free = heap->free_list;
        if (heap->free_list != NULL) {
            heap->free_list->prev_free = blk;
        }
        heap->free_list = blk;
        blk->status = MEM_BLOCK_STATE_FREE;                             /*  改变内存块状态为空闲        */
    }

    heap->free_cnt++;
    heap->used_size -= blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

    return NULL;
}

/*
 * 打印内存堆信息
 */
void heap_show(heap_t *heap)
{
    heap_t tmp;

    /*
     * 为了避免信息波动, 先拷贝到局部变量
     */
    memcpy(&tmp, heap, sizeof(heap_t));

    debug_output("\n********** heap info **********\n");
    debug_output("heap block count = %d\n", tmp.block_cnt);
    debug_output("heap alloc count = %d\n", tmp.alloc_cnt);
    debug_output("heap free  count = %d\n", tmp.free_cnt);
    debug_output("heap leak  count = %d\n", tmp.alloc_cnt - tmp.free_cnt);
    debug_output("heap used  size  = %dMB.%dKB.%dB\n",
            tmp.used_size / MB,
            tmp.used_size % MB / KB,
            tmp.used_size % KB);
    debug_output("heap free  size  = %dMB.%dKB.%dB\n",
            (tmp.size-tmp.used_size) / MB,
            (tmp.size-tmp.used_size) % MB / KB,
            (tmp.size-tmp.used_size) % KB);
    debug_output("************* end *************\n\n");
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
