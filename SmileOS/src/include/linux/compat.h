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

typedef uint32_t                u_int32_t;
typedef uint16_t                u16;

#define __user
#define __iomem

#define ndelay(x)	            udelay(1)

#define KERN_EMERG
#define KERN_ALERT
#define KERN_CRIT
#define KERN_ERR
#define KERN_WARNING
#define KERN_NOTICE
#define KERN_INFO
#define KERN_DEBUG

#define vmalloc(size)		    kmalloc(size, GFP_KERNEL)
#define vfree(ptr)		        kfree(ptr)

#define DECLARE_WAITQUEUE(...)	do { } while (0)
#define add_wait_queue(...)	    do { } while (0)
#define remove_wait_queue(...)	do { } while (0)

#define KERNEL_VERSION(a,b,c)	(((a) << 16) + ((b) << 8) + (c))

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
	printf("U-Boot BUG at %s:%d!\n", __FILE__, __LINE__); \
} while (0)

#define BUG_ON(condition)       do { if (condition) BUG(); } while(0)
#endif /* BUG */

#define likely(x)	            __builtin_expect(!!(x), 1)
#define unlikely(x)	            __builtin_expect(!!(x), 0)

#ifdef __cplusplus
}
#endif

#endif
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
