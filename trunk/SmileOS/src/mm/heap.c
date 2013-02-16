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
** Descriptions:            内存堆
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
#include "mm/heap.h"
#include <string.h>
#include <errno.h>
/*********************************************************************************************************
** 定义
*********************************************************************************************************/
#ifdef SMILEOS_KERNEL
#include "kern/kern.h"
#define debug                       printk
#define getpid                      gettid
#else
#define debug                       printf
#endif

/*
 * 内存块魔数
 */
#define MEM_BLOCK_MAGIC             0xAA5555AA

/*
 * 内存块状态(相当于后魔数)
 */
#define MEM_BLOCK_FREE              0x12345678
#define MEM_BLOCK_USED              0x87654321

/*
 * 内存块
 */
struct mem_block {
    uint32_t        magic;                                              /*  魔数                        */
    mem_block_t    *prev;                                               /*  前趋                        */
    mem_block_t    *next;                                               /*  后趋                        */
    mem_block_t    *prev_free;                                          /*  空闲前趋                    */
    mem_block_t    *next_free;                                          /*  空闲后趋                    */
    size_t          size;                                               /*  大小                        */
    uint32_t        status;                                             /*  状态                        */
};
/*********************************************************************************************************
** Function name:           heap_init
** Descriptions:            创建内存堆
** input parameters:        heap                内存堆
**                          name                名字
**                          base                内存区基址
**                          size                内存区大小
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int heap_init(heap_t *heap, const char *name, uint8_t *base, size_t size)
{
    if (heap == NULL) {
        debug("%s: process %d heap=NULL!\n", __func__, getpid());
        return -1;
    }

    if (name == NULL) {
        debug("%s: process %d name=NULL!\n", __func__, getpid());
        return -1;
    }

    if (base == NULL) {
        debug("%s: process %d base=NULL!\n", __func__, getpid());
        return -1;
    }

    if (size < 1 * KB) {
        debug("%s: process %d size=%d too small!\n", __func__, getpid(), size);
        return -1;
    }

    heap->base = MEM_ALIGN(base);                                       /*  对齐基址                    */

    heap->size = MEM_ALIGN_SIZE_LESS(size - (heap->base - base));       /*  对齐大小                    */

    heap->block_list            = (mem_block_t *)heap->base;            /*  内存块链表                  */
    heap->block_list->magic     = MEM_BLOCK_MAGIC;
    heap->block_list->prev      = NULL;
    heap->block_list->next      = NULL;
    heap->block_list->prev_free = NULL;
    heap->block_list->next_free = NULL;
    heap->block_list->size      = heap->size - MEM_ALIGN_SIZE(sizeof(mem_block_t));
    heap->block_list->status    = MEM_BLOCK_FREE;

    heap->free_list             = heap->block_list;                     /*  空闲内存块链表              */

    heap->block_nr              = 1;                                    /*  初始化统计信息              */
    heap->used_size             = 0;
    heap->alloc_cnt             = 0;
    heap->free_cnt              = 0;

    heap->magic                 = MEM_BLOCK_MAGIC;                      /*  加入魔数                    */

    strlcpy(heap->name, name, sizeof(heap->name));                      /*  名字                        */

    return 0;
}
/*********************************************************************************************************
** Function name:           bad_blk_dump
** Descriptions:            dump 坏块信息
** input parameters:        heap                内存堆
**                          func                调用者的函数名
**                          line                调用者的行号
**                          blk                 坏块
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void bad_blk_dump(heap_t *heap, const char *func, int line, mem_block_t *blk)
{
    mem_block_t *temp;
    mem_block_t *prev;

    debug("%s: process %d heap %s error: blk 0x%x magic=0x%x status=0x%x, call by %s() line %d\n",
            __func__, getpid(), heap->name, blk, blk->magic, blk->status, func, line);

    temp = heap->block_list;
    prev = NULL;

    while (temp && temp != blk) {
        prev = temp;
        temp = temp->next;
    }

    if (temp && prev) {
        debug("%s: process %d heap %s error: prev blk 0x%x magic=0x%x status=0x%x, call by %s() line %d\n",
                __func__, getpid(), heap->name, prev, prev->magic, prev->status, func, line);
    }
}
/*********************************************************************************************************
** Function name:           heap_alloc
** Descriptions:            分配内存
** input parameters:        heap                内存堆
**                          func                调用者的函数名
**                          line                调用者的行号
**                          size                需要分配的大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *heap_alloc(heap_t *heap, const char *func, int line, size_t size)
{
    mem_block_t *blk;
    mem_block_t *new_blk;

    if (heap == NULL) {
        debug("%s: process %d heap=NULL, call by %s() line %d\n",
                __func__, getpid(), func, line);
        return NULL;
    }

    if (heap->magic != MEM_BLOCK_MAGIC) {
        debug("%s: process %d heap magic %d != %d, call by %s() line %d\n",
                __func__, getpid(), heap->magic, MEM_BLOCK_MAGIC, func, line);
        return NULL;
    }

    if (heap->free_list != NULL) {                                      /*  有空闲内存块                */

        size = MEM_ALIGN_SIZE(size);                                    /*  对齐大小                    */
        if (size == 0) {
            size = MEM_ALIGNMENT;
        }

        blk = heap->free_list;                                          /*  从空闲内存块链表里找出一个  */
        while (blk != NULL && blk->size < size) {                       /*  首次满足大小的空闲内存块    */
            if (blk->magic != MEM_BLOCK_MAGIC || blk->status != MEM_BLOCK_FREE) {
                bad_blk_dump(heap, func, line, blk);
                goto error;
            }
            blk = blk->next_free;
        }

        if (blk == NULL) {                                              /*  没找到                      */
            debug("%s: process %d heap %s low memory, call by %s() line %d\n",
                    __func__, getpid(), heap->name, func, line);
            goto error;
        }

        if (blk->magic != MEM_BLOCK_MAGIC || blk->status != MEM_BLOCK_FREE) {
            bad_blk_dump(heap, func, line, blk);
            goto error;
        }

        if (blk->size <= MEM_ALIGN_SIZE(sizeof(mem_block_t)) + size) {  /*  如果内存块切割后, 剩余的大小*/
                                                                        /*  不足或只够建立一个内存块节点*/
                                                                        /*  则整块分配出去              */
            if (heap->free_list == blk) {                               /*  从空闲内存块链表中删除内存块*/
                heap->free_list = blk->next_free;
            }

            if (blk->prev_free != NULL) {
                blk->prev_free->next_free = blk->next_free;
            }
            if (blk->next_free != NULL) {
                blk->next_free->prev_free = blk->prev_free;
            }

            blk->status = MEM_BLOCK_USED;                               /*  改变内存块状态为已用        */

            new_blk = blk;

        } else {
                                                                        /*  在内存块的高端地址处        */
            new_blk = (mem_block_t *)((char *)blk + blk->size - size);  /*  建立一个新的内存块          */
            /*
             * 上面一句代码是下面的简化版本:
             *
             * new_blk = (mem_block_t *)((char *)blk + MEM_ALIGN_SIZE(sizeof(mem_block_t)) +
             *           blk->size - size - MEM_ALIGN_SIZE(sizeof(mem_block_t)));
             */

            if (blk->next != NULL) {                                    /*  新内存块加入内存块链表      */
                blk->next->prev = new_blk;
            }
            new_blk->next = blk->next;
            blk->next     = new_blk;
            new_blk->prev = blk;

            new_blk->size   = size;
            new_blk->status = MEM_BLOCK_USED;                           /*  新内存块状态为已用          */
                                                                        /*  所以不在空闲内存块链表      */

            blk->size -= MEM_ALIGN_SIZE(sizeof(mem_block_t)) + size;    /*  原内存块变小                */

            heap->block_nr++;
        }

        new_blk->magic = MEM_BLOCK_MAGIC;                               /*  加入魔数                    */

        heap->alloc_cnt++;
        heap->used_size += new_blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

        return (char *)new_blk + MEM_ALIGN_SIZE(sizeof(mem_block_t));
    }

    error:
    return NULL;
}
/*********************************************************************************************************
** Function name:           heap_free
** Descriptions:            释放内存
** input parameters:        heap                内存堆
**                          func                调用者的函数名
**                          line                调用者的行号
**                          ptr                 内存指针
** output parameters:       NONE
** Returned value:          NULL OR 内存指针
*********************************************************************************************************/
void *heap_free(heap_t *heap, const char *func, int line, void *ptr)
{
    mem_block_t *blk;
    mem_block_t *prev;
    mem_block_t *next;

    if (heap == NULL) {
        debug("%s: process %d heap=NULL, call by %s() line %d\n",
                __func__, getpid(), func, line);
        return NULL;
    }

    if (heap->magic != MEM_BLOCK_MAGIC) {
        debug("%s: process %d heap magic %d != %d, call by %s() line %d\n",
                __func__, getpid(), heap->magic, MEM_BLOCK_MAGIC, func, line);
        return NULL;
    }

    if (ptr == NULL) {
#ifdef SMILEOS_KERNEL
        debug("%s: process %d heap %s memptr=NULL, call by %s() line %d\n",
                __func__, getpid(), heap->name, func, line);
#endif
        return ptr;
    }

    if (ptr != MEM_ALIGN(ptr)) {
        debug("%s: process %d heap %s memptr=0x%x is not aligned, call by %s() line %d\n",
                __func__, getpid(), heap->name, (uint32_t)ptr, func, line);
        return ptr;
    }

    if (((uint8_t *)ptr < (heap->base + MEM_ALIGN_SIZE(sizeof(mem_block_t)))) ||
        ((uint8_t *)ptr > heap->base + heap->size - MEM_ALIGNMENT)) {
        debug("%s: process %d heap %s memptr=0x%x dose not belong to this heap, call by %s() line %d\n",
                __func__, getpid(), heap->name, (uint32_t)ptr, func, line);
        return ptr;
    }
                                                                        /*  指针所在的内存块节点        */
    blk  = (mem_block_t *)((char *)ptr - MEM_ALIGN_SIZE(sizeof(mem_block_t)));

    if (blk->magic != MEM_BLOCK_MAGIC || blk->status != MEM_BLOCK_USED) {
        debug("%s: process %d heap %s memptr=0x%x is invalid, call by %s() line %d\n",
                __func__, getpid(), heap->name, (uint32_t)ptr, func, line);
        return ptr;
    }

    prev = blk->prev;
    next = blk->next;

    if (next != NULL && next->magic != MEM_BLOCK_MAGIC) {               /*  写缓冲区溢出                */
        debug("%s: process %d heap %s write buffer over, memptr=0x%x, call by %s() line %d\n",
                __func__, getpid(), heap->name, (uint32_t)ptr, func, line);
        return ptr;
    }

    heap->free_cnt++;
    heap->used_size -= blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

    if (prev != NULL && prev->status == MEM_BLOCK_FREE) {               /*  前一个内存块空闲, 合并之    */

        prev->size += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + blk->size;  /*  前一个内存块变大            */

        prev->next = blk->next;                                         /*  从内存块链表中删除内存块    */
        if (next != NULL) {
            next->prev = prev;
        }
        heap->block_nr--;

        if (next != NULL && next->status == MEM_BLOCK_FREE) {           /*  后一个内存块空闲, 合并之    */

            blk = prev;

            blk->size += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + next->size;  /*  内存块变大              */

            blk->next = next->next;                                     /*  从内存块链表中删除后内存块  */
            if (next->next != NULL) {
                next->next->prev = blk;
            }
            heap->block_nr--;

            if (next->next_free != NULL) {                              /*  从空闲块链表中删除后内存块  */
                next->next_free->prev_free = next->prev_free;
            }

            if (next->prev_free != NULL) {
                next->prev_free->next_free = next->next_free;
            }

            if (heap->free_list == next) {
                heap->free_list = next->next_free;
            }
        }
    } else if (next != NULL && next->status == MEM_BLOCK_FREE) {        /*  后一个内存块空闲, 合并之    */

        blk->size  += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + next->size; /*  内存块变大                  */
        blk->status = MEM_BLOCK_FREE;                                   /*  改变内存块状态为空闲        */

        blk->next = next->next;                                         /*  从内存块链表中删除后内存块  */
        if (next->next != NULL) {
            next->next->prev = blk;
        }
        heap->block_nr--;

        if (heap->free_list == next) {                                  /*  从空闲块链表中删除后内存块  */
            heap->free_list = next->next_free;
        }

        if (next->next_free != NULL) {
            next->next_free->prev_free = next->prev_free;
        }

        if (next->prev_free != NULL) {
            next->prev_free->next_free = next->next_free;
        }

        blk->next_free = heap->free_list;                               /*  把内存块加入空闲内存块链表  */
        blk->prev_free = NULL;
        if (heap->free_list != NULL) {
            heap->free_list->prev_free = blk;
        }
        heap->free_list = blk;
    } else {
        /*
         * 把内存块加入空闲内存块链表的链头, 分配时尽可能使用已经使用过的内存,
         * 因为使用过, 那么这些内存已经映射好, 即已经引入了物理页面,
         * 既提升了程序的运行速度, 又减少了本进程使用的物理页面数
         */
        blk->status = MEM_BLOCK_FREE;                                   /*  改变内存块状态为空闲        */
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
** Function name:           mem_size
** Descriptions:            获得内存的大小
** input parameters:        heap                内存堆
**                          func                调用者的函数名
**                          line                调用者的行号
**                          ptr                 内存指针
** output parameters:       NONE
** Returned value:          内存的大小
*********************************************************************************************************/
size_t mem_size(heap_t *heap, const char *func, int line, void *ptr)
{
    mem_block_t *blk;

    if (heap == NULL) {
        debug("%s: process %d heap=NULL, call by %s() line %d\n",
                __func__, getpid(), func, line);
        return 0;
    }

    if (heap->magic != MEM_BLOCK_MAGIC) {
        debug("%s: process %d heap magic %d != %d, call by %s() line %d\n",
                __func__, getpid(), heap->magic, MEM_BLOCK_MAGIC, func, line);
        return 0;
    }

    if (ptr == NULL) {
#ifdef SMILEOS_KERNEL
        debug("%s: process %d heap %s memptr=NULL, call by %s() line %d\n",
                __func__, getpid(), heap->name, func, line);
#endif
        return 0;
    }

    if (ptr != MEM_ALIGN(ptr)) {
        debug("%s: process %d heap %s memptr=0x%x is not aligned, call by %s() line %d\n",
                __func__, getpid(), heap->name, (uint32_t)ptr, func, line);
        return 0;
    }

    if (((uint8_t *)ptr < (heap->base + MEM_ALIGN_SIZE(sizeof(mem_block_t)))) ||
        ((uint8_t *)ptr > heap->base + heap->size - MEM_ALIGNMENT)) {
        debug("%s: process %d heap %s memptr=0x%x dose not belong to this heap, call by %s() line %d\n",
                __func__, getpid(), heap->name, (uint32_t)ptr, func, line);
        return 0;
    }
                                                                        /*  指针所在的内存块节点        */
    blk  = (mem_block_t *)((char *)ptr - MEM_ALIGN_SIZE(sizeof(mem_block_t)));

    if (blk->magic != MEM_BLOCK_MAGIC || blk->status != MEM_BLOCK_USED) {
        debug("%s: process %d heap %s memptr=0x%x is invalid, call by %s() line %d\n",
                __func__, getpid(), heap->name, (uint32_t)ptr, func, line);
        return 0;
    }

    return blk->size;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
