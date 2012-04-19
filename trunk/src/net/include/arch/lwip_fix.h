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
** File name:               lwip_fix.h
** Last modified Date:      2012-2-22
** Last Version:            1.0.0
** Descriptions:            lwIP 修正头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-22
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
#ifndef LWIP_FIX_H_
#define LWIP_FIX_H_
/*********************************************************************************************************
  头文件
*********************************************************************************************************/
#include "kern/types.h"
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
/*********************************************************************************************************
  主机与网络字节转换 (为了速度这里使用宏, 但是不可以直接使用, 例如: htonl(x++) 就会错误)
*********************************************************************************************************/
#define LWIP_PLATFORM_BYTESWAP      0                                   /*  使用 lwIP htonl() ...       */

#if BYTE_ORDER == LITTLE_ENDIAN
static inline u16_t __HTONS(u16_t x)
{
    return (u16_t)((((x) & 0x00ff) << 8) |
                   (((x) & 0xff00) >> 8));
}
static inline u32_t __HTONL(u32_t x)
{
    return (u32_t)((((x) & 0x000000ff) << 24) |
                   (((x) & 0x0000ff00) <<  8) |
                   (((x) & 0x00ff0000) >>  8) |
                   (((x) & 0xff000000) >> 24));
}
#define LWIP_PLATFORM_HTONS(x)      __HTONS(x)
#define LWIP_PLATFORM_HTONL(x)      __HTONL(x)
#else
#define LWIP_PLATFORM_HTONS(x)      x
#define LWIP_PLATFORM_HTONL(x)      x
#endif                                                                  /*  BYTE_ORDER == LITTLE_ENDIAN */
/*********************************************************************************************************
  调试输出
*********************************************************************************************************/
extern void printk(const char *fmt, ...);

#define LWIP_PLATFORM_DIAG(x)       { printk x; }
#define LWIP_PLATFORM_ASSERT(x)     { printk("lwip assert: %s\n", x); }
/*********************************************************************************************************
  临界区保护
*********************************************************************************************************/
typedef uint32_t sys_prot_t;
extern  uint32_t interrupt_disable(void);
extern  void     interrupt_resume(register uint32_t reg);

#define sys_arch_protect            interrupt_disable
#define sys_arch_unprotect          interrupt_resume
/*********************************************************************************************************
  OS 数据类型
*********************************************************************************************************/
struct mutex;
struct sem;
struct mqueue;

typedef struct mutex  *             sys_mutex_t;
typedef struct sem    *             sys_sem_t;
typedef struct mqueue *             sys_mbox_t;
typedef int32_t                     sys_thread_t;

#define SYS_MUTEX_NULL              0ul
#define SYS_SEM_NULL                0ul
#define SYS_MBOX_NULL               0ul

#define LWIP_COMPAT_MUTEX           0                                   /*  lwIP 不使用兼容的互斥量     */
#define LWIP_TIMEVAL_PRIVATE        0                                   /*  lwIP 不定义 struct timeval  */
//#define LWIP_PROVIDE_ERRNO        0                                   /*  lwIP 不提供错误代号         */
#define ENSRNOTFOUND                163                                 /*  Domain name not found       */
/*********************************************************************************************************
  时间性能测量操作宏
*********************************************************************************************************/
#define PERF_START
#define PERF_STOP(x)

#endif                                                                  /*  LWIP_FIX_H_                 */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
