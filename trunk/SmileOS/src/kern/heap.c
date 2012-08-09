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
** Modified by:             JiaoJinXing
** Modified date:           2012-3-26
** Version:                 1.1.0
** Descriptions:            �޸��ڴ��ͷź��ڴ�ͳ�Ƶ�һ�� BUG, ���Ӷ�ָ�����Ͱ�ȫ���
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-28
** Version:                 1.2.0
** Descriptions:            ���� newlib ��Ҫ�Ŀ�����汾 _malloc_r �Ⱥ���,
**                          �޸� heap_show �����ӡ�ڴ����Ϣ��ָ�����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-4-15
** Version:                 1.3.0
** Descriptions:            heap_alloc �� heap_free ���� func �� line ����, ���������ڴ�ʹ�ô���!
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-7-20
** Version:                 1.4.0
** Descriptions:            ������ͬ�� ���� ����ָ�����ڴ��ͷź�������ڴ��ϲ�����! ������ڴ���Ƭ,
**                          �ڴ˱�ʾ THANKS!
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/heap.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

/*
 * ���̴�����ĿӦ����һ�ݸ��ļ�, ����ʵ���û�̬�� malloc ��
 */
#ifdef SMILEOS_KERNEL

#include "kern/kern.h"

#define getpid      gettid

/*
 * printk ��ʹ���ڴ����, ��ʹ�� printk
 */
#define debug       kcomplain

/*
 * �ں��ڴ��
 */
static heap_t  kheap;
static uint8_t kheap_mem[KERN_HEAP_SIZE];

/*
 * ���ں��ڴ�ѷ����ڴ�
 */
void *__kmalloc(const char *func, int line, uint32_t size)
{
    void    *ptr;
    uint32_t reg;

    reg = interrupt_disable();

    ptr = heap_alloc(&kheap, func, line, size);

    interrupt_resume(reg);

    return ptr;
}

/*
 * �ͷ��ڴ���ں��ڴ��
 */
void __kfree(const char *func, int line, void *ptr)
{
    uint32_t reg;

    reg = interrupt_disable();

    heap_free(&kheap, func, line, ptr);

    interrupt_resume(reg);
}

/*
 * kcalloc
 */
void *__kcalloc(const char *func, int line, uint32_t nelem, uint32_t elsize)
{
    void    *ptr;
    uint32_t reg;

    reg = interrupt_disable();

    ptr = heap_alloc(&kheap, func, line, nelem * MEM_ALIGN_SIZE(elsize));

    interrupt_resume(reg);

    if (ptr != NULL) {
        memset(ptr, 0, nelem * MEM_ALIGN_SIZE(elsize));
    }
    return ptr;
}

/*
 * _malloc_r
 */
void *_malloc_r(struct _reent *reent, size_t size)
{
    void    *ptr;
    uint32_t reg;

    reg = interrupt_disable();

    ptr = heap_alloc(&kheap, __func__, __LINE__, size);

    interrupt_resume(reg);

    if (ptr == NULL) {
        reent->_errno = ENOMEM;
    }
    return ptr;
}

/*
 * _free_r
 */
void _free_r(struct _reent *reent, void *ptr)
{
    uint32_t reg;

    reg = interrupt_disable();

    heap_free(&kheap, __func__, __LINE__, ptr);

    interrupt_resume(reg);
}

/*
 * _realloc_r
 */
void *_realloc_r(struct _reent *reent, void *ptr, size_t newsize)
{
    void    *newptr;
    uint32_t reg;

    reg = interrupt_disable();

    newptr = heap_alloc(&kheap, __func__, __LINE__, newsize);

    interrupt_resume(reg);

    if (newptr != NULL) {
        if (ptr != NULL) {
            memcpy(newptr, ptr, newsize);

            reg = interrupt_disable();

            heap_free(&kheap, __func__, __LINE__, ptr);

            interrupt_resume(reg);
        }
    } else {
        reent->_errno = ENOMEM;
    }
    return newptr;
}

/*
 * _calloc_r
 */
void *_calloc_r(struct _reent *reent, size_t nelem, size_t elsize)
{
    void    *ptr;
    uint32_t reg;

    reg = interrupt_disable();

    ptr = heap_alloc(&kheap, __func__, __LINE__, nelem * MEM_ALIGN_SIZE(elsize));

    interrupt_resume(reg);

    if (ptr != NULL) {
        memset(ptr, 0, nelem * MEM_ALIGN_SIZE(elsize));
    } else {
        reent->_errno = ENOMEM;
    }
    return ptr;
}

/*
 * ��ӡ�ں��ڴ����Ϣ
 */
void kheap_show(int fd)
{
    heap_show(&kheap, fd);
}

/*
 * �����ں��ڴ��
 */
void kheap_create(void)
{
    heap_init(&kheap, kheap_mem, KERN_HEAP_SIZE);
}

#else

#define debug       printf

static heap_t uheap;

/*
 * _malloc_r
 */
void *_malloc_r(struct _reent *reent, size_t size)
{
    void *ptr;
    /*
     * �������ʹ�÷���ռ�� pthread, ��������ж�
     */
    ptr = heap_alloc(&uheap, __func__, __LINE__, size);
    if (ptr == NULL) {
        reent->_errno = ENOMEM;
    }
    return ptr;
}

/*
 * _free_r
 */
void _free_r(struct _reent *reent, void *ptr)
{
    heap_free(&uheap, __func__, __LINE__, ptr);
}

/*
 * _realloc_r
 */
void *_realloc_r(struct _reent *reent, void *ptr, size_t newsize)
{
    void *newptr;

    newptr = heap_alloc(&uheap, __func__, __LINE__, newsize);
    if (newptr != NULL) {
        if (ptr != NULL) {
            memcpy(newptr, ptr, newsize);
            heap_free(&uheap, __func__, __LINE__, ptr);
        }
    } else {
        reent->_errno = ENOMEM;
    }
    return newptr;
}

/*
 * _calloc_r
 */
void *_calloc_r(struct _reent *reent, size_t nelem, size_t elsize)
{
    void *ptr;

    ptr = heap_alloc(&uheap, __func__, __LINE__, nelem * MEM_ALIGN_SIZE(elsize));
    if (ptr != NULL) {
        memset(ptr, 0, nelem * MEM_ALIGN_SIZE(elsize));
    } else {
        reent->_errno = ENOMEM;
    }
    return ptr;
}

/*
 * �����û��ռ��ڴ��
 */
void uheap_create(void)
{
    extern unsigned char __bss_end;

    /*
     * �� __bss_end ��, ����ջ�ռ�ǰ, �����ڴ��
     */
    heap_init(&uheap, &__bss_end, PROCESS_SPACE_SIZE - (uint32_t)&__bss_end -
                                  PROCESS_STACK_SIZE - PROCESS_PARAM_SIZE);
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
#define MEM_BLOCK_MAGIC0            0x001B2600

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
 * �����ڴ��
 */
int heap_init(heap_t *heap, uint8_t *base, uint32_t size)
{
    if (heap == NULL) {
        debug("%s: process %d heap=NULL!\n", __func__, getpid());
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
    heap->block_list->magic0    = MEM_BLOCK_MAGIC0;
    heap->block_list->prev      = NULL;
    heap->block_list->next      = NULL;
    heap->block_list->prev_free = NULL;
    heap->block_list->next_free = NULL;
    heap->block_list->size      = heap->size - MEM_ALIGN_SIZE(sizeof(mem_block_t));
    heap->block_list->status    = MEM_BLOCK_STATE_FREE;

    heap->free_list             = heap->block_list;                     /*  �����ڴ������              */

    heap->block_nr              = 1;                                    /*  ��ʼ��ͳ����Ϣ              */
    heap->used_size             = 0;
    heap->alloc_cnt             = 0;
    heap->free_cnt              = 0;

    heap->magic0                = MEM_BLOCK_MAGIC0;                     /*  ����ħ��                    */

    return 0;
}

/*
 * �����ڴ�
 */
void *heap_alloc(heap_t *heap, const char *func, int line, uint32_t size)
{
    mem_block_t *blk;
    mem_block_t *new_blk;

    if (heap == NULL) {
        debug("%s: process %d heap=NULL, call by %s() line %d\n",
                __func__, getpid(), func, line);
        return NULL;
    }

    if (heap->magic0 != MEM_BLOCK_MAGIC0) {
        debug("%s: process %d heap magic %d != %d, call by %s() line %d\n",
                __func__, getpid(), heap->magic0, MEM_BLOCK_MAGIC0, func, line);
        return NULL;
    }

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
            if (heap->free_list == blk) {                               /*  ��������ڴ��������ͷ      */
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

            heap->block_nr++;
        }

        new_blk->magic0 = MEM_BLOCK_MAGIC0;                             /*  ����ħ��                    */

        heap->alloc_cnt++;
        heap->used_size += new_blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

        return (char *)new_blk + MEM_ALIGN_SIZE(sizeof(mem_block_t));
    }

    error0:
    debug("%s: process %d low memory, call by %s() line %d\n", __func__, getpid(), func, line);

    return NULL;
}

/*
 * �ͷ��ڴ�
 */
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

    if (heap->magic0 != MEM_BLOCK_MAGIC0) {
        debug("%s: process %d heap magic %d != %d, call by %s() line %d\n",
                __func__, getpid(), heap->magic0, MEM_BLOCK_MAGIC0, func, line);
        return NULL;
    }

    if (ptr == NULL) {
#ifdef SMILEOS_KERNEL
        debug("%s: process %d memptr=NULL, call by %s() line %d\n",
                __func__, getpid(), func, line);
#endif
        return ptr;
    }

    if (ptr != MEM_ALIGN(ptr)) {
        debug("%s: process %d memptr=0x%x is not aligned, call by %s() line %d\n",
                __func__, getpid(), (uint32_t)ptr, func, line);
        return ptr;
    }

    if (((uint8_t *)ptr < (heap->base + MEM_ALIGN_SIZE(sizeof(mem_block_t)))) ||
        ((uint8_t *)ptr > heap->base + heap->size - MEM_ALIGNMENT)) {
        debug("%s: process %d memptr=0x%x dose not belong to this heap, call by %s() line %d\n",
                __func__, getpid(), (uint32_t)ptr, func, line);
        return ptr;
    }
                                                                        /*  ָ�����ڵ��ڴ��ڵ�        */
    blk  = (mem_block_t *)((char *)ptr - MEM_ALIGN_SIZE(sizeof(mem_block_t)));

    if (blk->magic0 != MEM_BLOCK_MAGIC0 || blk->status != MEM_BLOCK_STATE_USED) {
        debug("%s: process %d memptr=0x%x is invalid, call by %s() line %d\n",
                __func__, getpid(), (uint32_t)ptr, func, line);
        return ptr;
    }

    prev = blk->prev;
    next = blk->next;

    if (next != NULL && next->magic0 != MEM_BLOCK_MAGIC0) {             /*  д���������                */
        debug("%s: process %d write buffer over, memptr=0x%x, call by %s() line %d\n",
                __func__, getpid(), (uint32_t)ptr, func, line);
        return ptr;
    }

    heap->free_cnt++;
    heap->used_size -= blk->size + MEM_ALIGN_SIZE(sizeof(mem_block_t));

    if (prev != NULL && prev->status == MEM_BLOCK_STATE_FREE) {         /*  ǰһ���ڴ�����, �ϲ�֮    */

        prev->size += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + blk->size;  /*  ǰһ���ڴ����            */

        prev->next = blk->next;                                         /*  ���ڴ��������ɾ���ڴ��    */
        if (next != NULL) {
            next->prev = prev;
        }
        heap->block_nr--;

        blk = prev;
        if (next != NULL && next->status == MEM_BLOCK_STATE_FREE) {     /*  ��һ���ڴ�����, �ϲ�֮    */

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
    } else if (next != NULL && next->status == MEM_BLOCK_STATE_FREE) {  /*  ��һ���ڴ�����, �ϲ�֮    */

        blk->size  += MEM_ALIGN_SIZE(sizeof(mem_block_t)) + next->size; /*  �ڴ����                  */
        blk->status = MEM_BLOCK_STATE_FREE;                             /*  �ı��ڴ��״̬Ϊ����        */

        blk->next = next->next;                                         /*  ���ڴ��������ɾ�����ڴ��  */
        if (next->next != NULL) {
            next->next->prev = blk;
        }
        heap->block_nr--;

        if (next->next_free != NULL) {                                  /*  �ӿ��п�������ɾ�����ڴ��  */
            next->next_free->prev_free = blk;
        }

        if (next->prev_free != NULL) {
            next->prev_free->next_free = blk;
        }

        blk->prev_free = next->prev_free;                               /*  ���ڴ���������ڴ������  */
        blk->next_free = next->next_free;
        if (heap->free_list == next) {
            heap->free_list = blk;
        }
    } else {
        /*
         * ���ڴ���������ڴ���������ͷ, ����ʱ������ʹ���Ѿ�ʹ�ù����ڴ�,
         * ��Ϊʹ�ù�, ��ô��Щ�ڴ��Ѿ�ӳ���, ���Ѿ�������ҳ��,
         * �������˳���������ٶ�, �ּ����˱�����ʹ�õ�ҳ����
         */
        blk->status = MEM_BLOCK_STATE_FREE;                             /*  �ı��ڴ��״̬Ϊ����        */
        blk->prev_free = NULL;
        blk->next_free = heap->free_list;
        if (heap->free_list != NULL) {
            heap->free_list->prev_free = blk;
        }
        heap->free_list = blk;
    }

    return NULL;
}

/*
 * ��ӡ�ڴ����Ϣ���ļ�
 */
int heap_show(heap_t *heap, int fd)
{
    heap_t tmp;
    char   buf[LINE_MAX];
    int    len;

    if (fd < 0) {
        debug("%s: process %d fd=%d invalid!\n", __func__, getpid(), fd);
        return -1;
    }

    if (heap == NULL) {
        debug("%s: process %d heap=NULL!\n", __func__, getpid());
        return -1;
    }

    if (heap->magic0 != MEM_BLOCK_MAGIC0) {
        debug("%s: process %d heap magic %d != %d!\n", __func__, getpid(), heap->magic0, MEM_BLOCK_MAGIC0);
        return -1;
    }

    /*
     * Ϊ�˱�����Ϣ����, �ȿ������ֲ�����
     */
    memcpy(&tmp, heap, sizeof(heap_t));

    len = sprintf(buf, "********** heap info **********\n");
    write(fd, buf, len);

    len = sprintf(buf, "heap block amount = %d\n", tmp.block_nr);
    write(fd, buf, len);

    len = sprintf(buf, "heap alloc count  = %d\n", tmp.alloc_cnt);
    write(fd, buf, len);

    len = sprintf(buf, "heap free  count  = %d\n", tmp.free_cnt);
    write(fd, buf, len);

    len = sprintf(buf, "heap leak  count  = %d\n", tmp.alloc_cnt - tmp.free_cnt);
    write(fd, buf, len);

    len = sprintf(buf, "heap used  size   = %dMB.%dKB.%dB\n",
            tmp.used_size / MB,
            tmp.used_size % MB / KB,
            tmp.used_size % KB);
    write(fd, buf, len);

    len = sprintf(buf, "heap free  size   = %dMB.%dKB.%dB\n",
            (tmp.size - tmp.used_size) / MB,
            (tmp.size - tmp.used_size) % MB / KB,
            (tmp.size - tmp.used_size) % KB);
    write(fd, buf, len);

    len = sprintf(buf, "************* end *************\n");
    write(fd, buf, len);

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
