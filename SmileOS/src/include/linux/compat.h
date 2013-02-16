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
** File name:               compat.h
** Last modified Date:      2013-01-26
** Last Version:            1.0.0
** Descriptions:            Linux 兼容头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-01-26
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
#ifndef _LINUX_COMPAT_H_
#define _LINUX_COMPAT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <kern/kern.h>
#include <string.h>
#include <unistd.h>

typedef uint32_t                u_int32_t;
typedef uint8_t                 u8;
typedef uint16_t                u16;
typedef uint32_t                u32;
typedef mem_ptr_t               phys_addr_t;
typedef long                    loff_t;
typedef unsigned long           ulong;

#define EUCLEAN                 135

#define __user
#define __iomem

#define udelay(x)	            usleep(x)
#define ndelay(x)               usleep(1)
#define vmalloc(size)		    kmalloc(size, GFP_KERNEL)
#define vfree(ptr)		        kfree(ptr)

#define __cpu_to_le16(x)        ((u16)(x))
#define cpu_to_le16             __cpu_to_le16

#define DECLARE_WAITQUEUE(...)	do { } while (0)
#define add_wait_queue(...)	    do { } while (0)
#define remove_wait_queue(...)	do { } while (0)

#define KERNEL_VERSION(a,b,c)	(((a) << 16) + ((b) << 8) + (c))

#define LINUX_VERSION_CODE      KERNEL_VERSION(2, 6, 30)

/*
 * ..and if you can't take the strict
 * types, you can specify one yourself.
 *
 * Or not use min/max at all, of course.
 */
#define min_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })

#define max_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

#ifndef BUG
#define BUG() do { \
	printk("SmileOS BUG at %s:%d!\n", __FILE__, __LINE__); \
} while (0)

#define BUG_ON(condition)       do { if (condition) BUG(); } while(0)
#endif /* BUG */

#define likely(x)	            __builtin_expect(!!(x), 1)
#define unlikely(x)	            __builtin_expect(!!(x), 0)

#define compile_time_assertion(assertion) \
                                ({ int x = __builtin_choose_expr(assertion, 0, (void)0); (void) x; })

#define IS_ERR_VALUE(x)         unlikely((x) >= (unsigned long)-__ELASTERROR)

static inline void *ERR_PTR(long error)
{
    return (void *) error;
}

static inline long PTR_ERR(const void *ptr)
{
    return (long) ptr;
}

static inline long IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)ptr);
}

#ifdef __cplusplus
}
#endif

#endif
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
