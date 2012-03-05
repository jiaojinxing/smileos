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
** Descriptions:            LwIP ����ͷ�ļ�
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
  �������Ͷ���
*********************************************************************************************************/
#include "kern/types.h"
#include <sys/time.h>
#include <sys/errno.h>
/*********************************************************************************************************
  �����������ֽ�ת�� (Ϊ���ٶ�����ʹ�ú�, ���ǲ�����ֱ��ʹ��, ����: htonl(x++) �ͻ����.)
*********************************************************************************************************/
#define LWIP_PLATFORM_BYTESWAP      0                                   /*  use lwip htonl() ...        */

#if BYTE_ORDER == LITTLE_ENDIAN

#ifndef __GNUC__
#ifdef  __arm
#define inline  __inline
#else
#define inline
#endif                                                                  /*  __arm (armcc compiler)      */
#endif                                                                  /*  __GNUC__                    */

static inline u16_t  __LW_HTONS (u16_t  x)
{
    return  (u16_t)((((x) & 0x00ff) << 8) |
                    (((x) & 0xff00) >> 8));
}
static inline u32_t  __LW_HTONL (u32_t  x)
{
    return  (u32_t)((((x) & 0x000000ff) << 24) |
                    (((x) & 0x0000ff00) <<  8) |
                    (((x) & 0x00ff0000) >>  8) |
                    (((x) & 0xff000000) >> 24));
}

#define LWIP_PLATFORM_HTONS(x)      __LW_HTONS(x)
#define LWIP_PLATFORM_HTONL(x)      __LW_HTONL(x)
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

typedef uint32_t sys_prot_t;

/*
 * �����ٽ�����
 */
uint32_t interrupt_disable(void);

/*
 * �˳��ٽ�����
 */
void interrupt_resume(register uint32_t reg);

#define sys_arch_protect            interrupt_disable
#define sys_arch_unprotect          interrupt_resume
/*********************************************************************************************************
  OS ��������
*********************************************************************************************************/
struct sys_mutex;
struct sys_sem;
struct sys_mbox;

typedef struct sys_mutex *          sys_mutex_t;
typedef struct sys_sem   *          sys_sem_t;
typedef struct sys_mbox  *          sys_mbox_t;
typedef int32_t                     sys_thread_t;

#define SYS_MUTEX_NULL              0ul
#define SYS_SEM_NULL                0ul
#define SYS_MBOX_NULL               0ul
/*********************************************************************************************************
  Measurement calls made throughout lwip, these can be defined to nothing.
*********************************************************************************************************/
#define PERF_START                                                      /* null definition              */
#define PERF_STOP(x)                                                    /* null definition              */
/*********************************************************************************************************
  LwIP �ṩ ERRNO
*********************************************************************************************************/
//#define LWIP_PROVIDE_ERRNO          0
#define LWIP_TIMEVAL_PRIVATE        0
/*********************************************************************************************************
  DNS thread safe function
*********************************************************************************************************/
struct hostent  *sys_thread_hostent(struct hostent  *phostent);
/*********************************************************************************************************
  fast OS function
*********************************************************************************************************/
#define __u_char_defined                                                /*  do not use PPP lib type     */
void  sys_arch_msleep(u32_t ms);                                        /*  XXX can instead sem timeout */
/*********************************************************************************************************
  rand()
*********************************************************************************************************/
//#define LWIP_RAND()
/*********************************************************************************************************
  mutex
*********************************************************************************************************/
#define LWIP_COMPAT_MUTEX           0                                   /*  use mutex method            */

#endif                                                                  /*  LWIP_FIX_H_                 */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
