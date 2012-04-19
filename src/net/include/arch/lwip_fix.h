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
** Descriptions:            lwIP ����ͷ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-22
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
#ifndef LWIP_FIX_H_
#define LWIP_FIX_H_
/*********************************************************************************************************
  ͷ�ļ�
*********************************************************************************************************/
#include "kern/types.h"
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
/*********************************************************************************************************
  �����������ֽ�ת�� (Ϊ���ٶ�����ʹ�ú�, ���ǲ�����ֱ��ʹ��, ����: htonl(x++) �ͻ����)
*********************************************************************************************************/
#define LWIP_PLATFORM_BYTESWAP      0                                   /*  ʹ�� lwIP htonl() ...       */

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
  �������
*********************************************************************************************************/
extern void printk(const char *fmt, ...);

#define LWIP_PLATFORM_DIAG(x)       { printk x; }
#define LWIP_PLATFORM_ASSERT(x)     { printk("lwip assert: %s\n", x); }
/*********************************************************************************************************
  �ٽ�������
*********************************************************************************************************/
typedef uint32_t sys_prot_t;
extern  uint32_t interrupt_disable(void);
extern  void     interrupt_resume(register uint32_t reg);

#define sys_arch_protect            interrupt_disable
#define sys_arch_unprotect          interrupt_resume
/*********************************************************************************************************
  OS ��������
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

#define LWIP_COMPAT_MUTEX           0                                   /*  lwIP ��ʹ�ü��ݵĻ�����     */
#define LWIP_TIMEVAL_PRIVATE        0                                   /*  lwIP ������ struct timeval  */
//#define LWIP_PROVIDE_ERRNO        0                                   /*  lwIP ���ṩ�������         */
#define ENSRNOTFOUND                163                                 /*  Domain name not found       */
/*********************************************************************************************************
  ʱ�����ܲ���������
*********************************************************************************************************/
#define PERF_START
#define PERF_STOP(x)

#endif                                                                  /*  LWIP_FIX_H_                 */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
