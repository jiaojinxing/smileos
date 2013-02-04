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
** File name:               mem.c
** Last modified Date:      2012-2-23
** Last Version:            1.0.0
** Descriptions:            内存分配
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
#include "kern/kern.h"
#include "kern/addr_config.h"
#include "mm/heap.h"
#include <string.h>
/*********************************************************************************************************
** 内核内存堆
*********************************************************************************************************/
static heap_t       kern_heap;

#if defined(DMA_MEM_BASE) && DMA_MEM_SIZE > 0
static heap_t       dma_heap;
#endif

#if defined(HW_SHARE_MEM_BASE) && HW_SHARE_MEM_SIZE > 0
static heap_t       hw_share_heap;
#endif

#if defined(SW_SHARE_MEM_BASE) && SW_SHARE_MEM_SIZE > 0
static heap_t       sw_share_heap;
#endif
/*********************************************************************************************************
** Function name:           __kmalloc
** Descriptions:            从内核内存堆里分配内存
** input parameters:        func                调用者的函数名
**                          line                调用者的行号
**                          size                需要分配的大小
**                          flags               分配标志
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *__kmalloc(const char *func, int line, size_t size, int flags)
{
    void  *ptr;
    reg_t  reg;

    reg = interrupt_disable();

    if (flags & GFP_SHARE) {

        if (flags & GFP_DMA) {
#if defined(HW_SHARE_MEM_BASE) && HW_SHARE_MEM_SIZE > 0
            ptr = heap_alloc(&hw_share_heap, func, line, size);
#else
            ptr = heap_alloc(&kern_heap, func, line, size);
#endif
        } else {
#if defined(SW_SHARE_MEM_BASE) && SW_SHARE_MEM_SIZE > 0
            ptr = heap_alloc(&sw_share_heap, func, line, size);
#else
            ptr = heap_alloc(&kern_heap, func, line, size);
#endif
        }

    } else if (flags & GFP_DMA) {
#if defined(DMA_MEM_BASE) && DMA_MEM_SIZE > 0
        ptr = heap_alloc(&dma_heap, func, line, size);
#else
        ptr = heap_alloc(&kern_heap, func, line, size);
#endif
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
** Descriptions:            释放内存回内核内存堆
** input parameters:        func                调用者的函数名
**                          line                调用者的行号
**                          ptr                 内存指针
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void __kfree(const char *func, int line, void *ptr)
{
    reg_t reg;

    reg = interrupt_disable();

#if defined(DMA_MEM_BASE) && DMA_MEM_SIZE > 0
    if (((mem_ptr_t)ptr >= DMA_MEM_BASE) &&
        ((mem_ptr_t)ptr <  DMA_MEM_BASE + DMA_MEM_SIZE)) {

        heap_free(&dma_heap, func, line, ptr);

    } else
#endif

#if defined(HW_SHARE_MEM_BASE) && HW_SHARE_MEM_SIZE > 0
    if (((mem_ptr_t)ptr >= HW_SHARE_MEM_BASE) &&
        ((mem_ptr_t)ptr <  HW_SHARE_MEM_BASE + HW_SHARE_MEM_SIZE)) {

        heap_free(&hw_share_heap, func, line, ptr);

    } else
#endif

#if defined(SW_SHARE_MEM_BASE) && SW_SHARE_MEM_SIZE > 0
    if (((mem_ptr_t)ptr >= SW_SHARE_MEM_BASE) &&
        ((mem_ptr_t)ptr <  SW_SHARE_MEM_BASE + SW_SHARE_MEM_SIZE)) {

        heap_free(&sw_share_heap, func, line, ptr);

    } else
#endif

    {

        heap_free(&kern_heap, func, line, ptr);
    }

    interrupt_resume(reg);
}

/*********************************************************************************************************
** Function name:           kheap_create
** Descriptions:            创建内核内存堆
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kheap_create(void)
{
    extern unsigned char __bss_end;

    heap_init(&kern_heap, "kern", &__bss_end, KERN_MEM_TOP - (mem_ptr_t)&__bss_end);

#if defined(DMA_MEM_BASE) && DMA_MEM_SIZE > 0
    heap_init(&dma_heap, "dma", (void *)DMA_MEM_BASE, DMA_MEM_SIZE);
#endif

#if defined(HW_SHARE_MEM_BASE) && HW_SHARE_MEM_SIZE > 0
    heap_init(&hw_share_heap, "hw_share", (void *)HW_SHARE_MEM_BASE, HW_SHARE_MEM_SIZE);
#endif

#if defined(SW_SHARE_MEM_BASE) && SW_SHARE_MEM_SIZE > 0
    heap_init(&sw_share_heap, "sw_share", (void *)SW_SHARE_MEM_BASE, SW_SHARE_MEM_SIZE);
#endif
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
