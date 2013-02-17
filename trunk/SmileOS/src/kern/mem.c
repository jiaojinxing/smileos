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
** 内部变量
*********************************************************************************************************/
/*
 * 普通内存区, 可 Cache 及 WriteBuffer, 用户态不可读写
 *
 * 虚拟地址 != 物理地址
 */
static heap_t               kern_heap;                                  /*  普通内存堆                  */

/*
 * DMA 内存区, 不可 Cache 及 WriteBuffer, 用户态不可读写
 *
 * 虚拟地址 = 物理地址
 */
#if defined(DMA_MEM_BASE) && DMA_MEM_SIZE > 0
static heap_t               dma_heap;                                   /*  DMA 内存堆                  */
#endif

/*
 * HW_SHARE 内存区, 不可 Cache 及 WriteBuffer, 用户态可读写
 *
 * HW_SHARE 内存区作为 DMA 内存区的补充, DMA 内存区供硬件 DMA 使用, 所以不可 Cache 及 WriteBuffer
 *
 * 但 DMA 内存区不能供应用程序使用, 而 HW_SHARE 内存区则可以
 *
 * 像 FrameBuffer 设备驱动可以使用 HW_SHARE 内存区
 *
 * 虚拟地址 = 物理地址
 */
#if defined(HW_SHARE_MEM_BASE) && HW_SHARE_MEM_SIZE > 0
static heap_t               hw_share_heap;                              /*  硬件共享内存堆              */
#endif

/*
 * SW_SHARE 内存区, 可 Cache 及 WriteBuffer, 用户态可读写
 *
 * 像共享内存设备驱动可以使用 SW_SHARE 内存区
 *
 * SW_SHARE 内存区不必像 HW_SHARE 内存区那样虚拟地址 = 物理地址
 */
#if defined(SW_SHARE_MEM_BASE) && SW_SHARE_MEM_SIZE > 0
static heap_t               sw_share_heap;                              /*  软件共享内存堆              */
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
    heap_init(&kern_heap, "kern", (void *)KERN_HEAP_BASE, KERN_HEAP_SIZE);

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
** Function name:           kheap_check
** Descriptions:            检查内核内存堆
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kheap_check(void)
{
    reg_t reg;

    reg = interrupt_disable();

    heap_check(&kern_heap, __func__, __LINE__);

#if defined(DMA_MEM_BASE) && DMA_MEM_SIZE > 0
    heap_check(&dma_heap, __func__, __LINE__);
#endif

#if defined(HW_SHARE_MEM_BASE) && HW_SHARE_MEM_SIZE > 0
    heap_check(&hw_share_heap, __func__, __LINE__);
#endif

#if defined(SW_SHARE_MEM_BASE) && SW_SHARE_MEM_SIZE > 0
    heap_check(&sw_share_heap, __func__, __LINE__);
#endif

    interrupt_resume(reg);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
