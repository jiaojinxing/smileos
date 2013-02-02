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
#include "kern/heap.h"
#include <string.h>
#include <errno.h>
/*********************************************************************************************************
** �ں˶���
*********************************************************************************************************/
#ifdef SMILEOS_KERNEL

#include "kern/kern.h"

/*
 * �ں���ʹ�� gettid
 */
#define getpid      gettid

/*
 * printk ��ʹ���ڴ����, ��ʹ�� printk
 */
#define debug       kcomplain
/*********************************************************************************************************
** �ں��ڴ��
*********************************************************************************************************/
static heap_t       kern_heap;
static heap_t       dma_heap;
static heap_t       hw_share_heap;
static heap_t       sw_share_heap;
/*********************************************************************************************************
** Function name:           __kmalloc
** Descriptions:            ���ں��ڴ��������ڴ�
** input parameters:        func                �����ߵĺ�����
**                          line                �����ߵ��к�
**                          size                ��Ҫ����Ĵ�С
**                          flags               �����־
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *__kmalloc(const char *func, int line, size_t size, int flags)
{
    void    *ptr;
    uint32_t reg;

    reg = interrupt_disable();

    if (flags & GFP_SHARE) {

        if (flags & GFP_DMA) {
            ptr = heap_alloc(&hw_share_heap, func, line, size);
        } else {
            ptr = heap_alloc(&sw_share_heap, func, line, size);
        }

    } else if (flags & GFP_DMA) {

        ptr = heap_alloc(&dma_heap, func, line, size);

    } else {

        ptr = heap_alloc(&kern_heap, func, line, size);
    }

    interrupt_resume(reg);

    if (ptr != NULL) {
        if (flags & GFP_ZERO) {
            memset(ptr, 0, size);
        }
    }

    return ptr;
}
/*********************************************************************************************************
** Function name:           __kfree
** Descriptions:            �ͷ��ڴ���ں��ڴ��
** input parameters:        func                �����ߵĺ�����
**                          line                �����ߵ��к�
**                          ptr                 �ڴ�ָ��
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void __kfree(const char *func, int line, void *ptr)
{
    uint32_t reg;

    reg = interrupt_disable();

    if (((uint32_t)ptr >= DMA_MEM_BASE) &&
        ((uint32_t)ptr <  DMA_MEM_BASE + DMA_MEM_SIZE)) {

        heap_free(&dma_heap, func, line, ptr);

    } else if (((uint32_t)ptr >= HW_SHARE_MEM_BASE) &&
               ((uint32_t)ptr <  HW_SHARE_MEM_BASE + HW_SHARE_MEM_SIZE)) {

        heap_free(&hw_share_heap, func, line, ptr);

    } else if (((uint32_t)ptr >= SW_SHARE_MEM_BASE) &&
               ((uint32_t)ptr <  SW_SHARE_MEM_BASE + SW_SHARE_MEM_SIZE)) {

        heap_free(&sw_share_heap, func, line, ptr);

    } else {
        heap_free(&kern_heap, func, line, ptr);
    }

    interrupt_resume(reg);
}
/*********************************************************************************************************
** Function name:           _malloc_r
** Descriptions:            malloc ׮����
** input parameters:        reent               ������ṹ
**                          size                ��Ҫ����Ĵ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *_malloc_r(struct _reent *reent, size_t size)
{
    void    *ptr;
    uint32_t reg;

    reg = interrupt_disable();

    ptr = heap_alloc(&kern_heap, __func__, __LINE__, size);

    interrupt_resume(reg);

    if (ptr != NULL) {
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return ptr;
}
/*********************************************************************************************************
** Function name:           _free_r
** Descriptions:            free ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�ָ��
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void _free_r(struct _reent *reent, void *ptr)
{
    uint32_t reg;

    reg = interrupt_disable();

    heap_free(&kern_heap, __func__, __LINE__, ptr);

    interrupt_resume(reg);

    reent->_errno = 0;
}
/*********************************************************************************************************
** Function name:           _realloc_r
** Descriptions:            realloc ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�ָ��
**                          newsize             �µĴ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *_realloc_r(struct _reent *reent, void *ptr, size_t newsize)
{
    void    *newptr;
    uint32_t reg;

    reg = interrupt_disable();

    newptr = heap_alloc(&kern_heap, __func__, __LINE__, newsize);

    interrupt_resume(reg);

    if (newptr != NULL) {
        if (ptr != NULL) {
            memcpy(newptr, ptr, newsize);

            reg = interrupt_disable();

            heap_free(&kern_heap, __func__, __LINE__, ptr);

            interrupt_resume(reg);
        }
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return newptr;
}
/*********************************************************************************************************
** Function name:           _calloc_r
** Descriptions:            calloc ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�
**                          nelem               Ԫ�صĸ���
**                          elsize              Ԫ�صĴ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *_calloc_r(struct _reent *reent, size_t nelem, size_t elsize)
{
    void    *ptr;
    uint32_t reg;

    reg = interrupt_disable();

    ptr = heap_alloc(&kern_heap, __func__, __LINE__, nelem * MEM_ALIGN_SIZE(elsize));

    interrupt_resume(reg);

    if (ptr != NULL) {
        memset(ptr, 0, nelem * MEM_ALIGN_SIZE(elsize));
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return ptr;
}
/*********************************************************************************************************
** Function name:           kheap_create
** Descriptions:            �����ں��ڴ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kheap_create(void)
{
    extern unsigned char __bss_end;

    heap_init(&kern_heap, "kern", &__bss_end + 1, KERN_STACK_TOP - (uint32_t)&__bss_end - 1);

    heap_init(&dma_heap, "dma", (void *)DMA_MEM_BASE, DMA_MEM_SIZE);

    heap_init(&hw_share_heap, "hw_share", (void *)HW_SHARE_MEM_BASE, HW_SHARE_MEM_SIZE);

    heap_init(&sw_share_heap, "sw_share", (void *)SW_SHARE_MEM_BASE, SW_SHARE_MEM_SIZE);
}
#else
/*********************************************************************************************************
** �û��ռ������
*********************************************************************************************************/
#include <stdio.h>
#define debug       printf
/*********************************************************************************************************
** �û��ռ��ڴ��
*********************************************************************************************************/
static heap_t       user_heap;
/*********************************************************************************************************
** Function name:           _malloc_r
** Descriptions:            malloc ׮����
** input parameters:        reent               ������ṹ
**                          size                ��Ҫ����Ĵ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *_malloc_r(struct _reent *reent, size_t size)
{
    void *ptr;
    /*
     * �������ʹ�÷���ռ�� pthread, ��������ж�, ��ͬ
     */
    ptr = heap_alloc(&user_heap, __func__, __LINE__, size);
    if (ptr != NULL) {
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return ptr;
}
/*********************************************************************************************************
** Function name:           _free_r
** Descriptions:            free ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�ָ��
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void _free_r(struct _reent *reent, void *ptr)
{
    heap_free(&user_heap, __func__, __LINE__, ptr);

    reent->_errno = 0;
}
/*********************************************************************************************************
** Function name:           _realloc_r
** Descriptions:            realloc ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�ָ��
**                          newsize             �µĴ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *_realloc_r(struct _reent *reent, void *ptr, size_t newsize)
{
    void *newptr;

    newptr = heap_alloc(&user_heap, __func__, __LINE__, newsize);
    if (newptr != NULL) {
        if (ptr != NULL) {
            memcpy(newptr, ptr, newsize);
            heap_free(&user_heap, __func__, __LINE__, ptr);
        }
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return newptr;
}
/*********************************************************************************************************
** Function name:           _calloc_r
** Descriptions:            calloc ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�
**                          nelem               Ԫ�صĸ���
**                          elsize              Ԫ�صĴ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *_calloc_r(struct _reent *reent, size_t nelem, size_t elsize)
{
    void *ptr;

    ptr = heap_alloc(&user_heap, __func__, __LINE__, nelem * MEM_ALIGN_SIZE(elsize));
    if (ptr != NULL) {
        memset(ptr, 0, nelem * MEM_ALIGN_SIZE(elsize));
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return ptr;
}
/*********************************************************************************************************
** Function name:           uheap_create
** Descriptions:            �����û��ռ��ڴ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void uheap_create(void)
{
    extern unsigned char __bss_end;

    /*
     * �� __bss_end ��, ����ջ�ռ�ǰ, �����ڴ��
     */
    heap_init(&user_heap, &__bss_end + 1, PROCESS_SPACE_SIZE - (uint32_t)&__bss_end -
                                          PROCESS_STACK_SIZE - PROCESS_PARAM_SIZE - 1);
}
#endif
/*********************************************************************************************************
** ����
*********************************************************************************************************/
/*
 * �ڴ��ħ��
 */
#define MEM_BLOCK_MAGIC             0xAA55

/*
 * �ڴ��״̬
 */
#define MEM_BLOCK_FREE              0x5566
#define MEM_BLOCK_USED              0x7788

/*
 * �ڴ��
 */
struct mem_block {
    uint16_t        magic;                                              /*  ħ��                        */
    uint16_t        status;                                             /*  ״̬                        */
    mem_block_t    *prev;                                               /*  ǰ��                        */
    mem_block_t    *next;                                               /*  ����                        */
    mem_block_t    *prev_free;                                          /*  ����ǰ��                    */
    mem_block_t    *next_free;                                          /*  ���к���                    */
    size_t          size;                                               /*  ��С                        */
} __attribute__((packed));
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

    heap->free_list             = heap->block_list;                     /*  �����ڴ������              */

    heap->block_nr              = 1;                                    /*  ��ʼ��ͳ����Ϣ              */
    heap->used_size             = 0;
    heap->alloc_cnt             = 0;
    heap->free_cnt              = 0;

    heap->magic                 = MEM_BLOCK_MAGIC;                      /*  ����ħ��                    */

    strlcpy(heap->name, name, sizeof(heap->name));                      /*  ����                        */

    return 0;
}

static void blk_dump(heap_t *heap, mem_block_t *blk, const char *func, int line)
{
    mem_block_t *temp;
    mem_block_t *prev;

    debug("%s: process %d heap %s error: blk 0x%x magic=0x%x status=0x%x, call by %s() line %d\n",
            __func__, getpid(), heap->name, blk, blk->magic, blk->status, func, line);
    blk->magic  = MEM_BLOCK_MAGIC;
    blk->status = MEM_BLOCK_FREE;

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
        debug("%s: process %d heap=NULL, call by %s() line %d\n",
                __func__, getpid(), func, line);
        return NULL;
    }

    if (heap->magic != MEM_BLOCK_MAGIC) {
        debug("%s: process %d heap magic %d != %d, call by %s() line %d\n",
                __func__, getpid(), heap->magic, MEM_BLOCK_MAGIC, func, line);
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
                blk_dump(heap, blk, func, line);
                goto error;
            }
            blk = blk->next_free;
        }

        if (blk == NULL) {                                              /*  û�ҵ�                      */
            debug("%s: process %d heap %s low memory, call by %s() line %d\n",
                    __func__, getpid(), heap->name, func, line);
            goto error;
        }

        if (blk->magic != MEM_BLOCK_MAGIC || blk->status != MEM_BLOCK_FREE) {
            blk_dump(heap, blk, func, line);
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
                                                                        /*  ָ�����ڵ��ڴ��ڵ�        */
    blk  = (mem_block_t *)((char *)ptr - MEM_ALIGN_SIZE(sizeof(mem_block_t)));

    if (blk->magic != MEM_BLOCK_MAGIC || blk->status != MEM_BLOCK_USED) {
        debug("%s: process %d heap %s memptr=0x%x is invalid, call by %s() line %d\n",
                __func__, getpid(), heap->name, (uint32_t)ptr, func, line);
        return ptr;
    }

    prev = blk->prev;
    next = blk->next;

    if (next != NULL && next->magic != MEM_BLOCK_MAGIC) {               /*  д���������                */
        debug("%s: process %d heap %s write buffer over, memptr=0x%x, call by %s() line %d\n",
                __func__, getpid(), heap->name, (uint32_t)ptr, func, line);
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
         * ��Ϊʹ�ù�, ��ô��Щ�ڴ��Ѿ�ӳ���, ���Ѿ�������ҳ��,
         * �������˳���������ٶ�, �ּ����˱�����ʹ�õ�ҳ����
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
** Function name:           mem_size
** Descriptions:            ����ڴ�Ĵ�С
** input parameters:        heap                �ڴ��
**                          func                �����ߵĺ�����
**                          line                �����ߵ��к�
**                          ptr                 �ڴ�ָ��
** output parameters:       NONE
** Returned value:          �ڴ�Ĵ�С
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
                                                                        /*  ָ�����ڵ��ڴ��ڵ�        */
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
