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
** File name:               sys_call.c
** Last modified Date:      2012-2-22
** Last Version:            1.0.0
** Descriptions:            系统调用
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/sys_call.h"
#include "kern/mem.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

/*
 * 系统调用模板
 */
int syscall_template(void)
{
    int param1 = 0;
    int param2 = 0;
    int param3 = 0;
    int param4 = 0;
    int ret;

    __asm__ __volatile__("mov    r0,  %0": :"r"(param1));
    __asm__ __volatile__("mov    r1,  %0": :"r"(param2));
    __asm__ __volatile__("mov    r2,  %0": :"r"(param3));
    __asm__ __volatile__("mov    r3,  %0": :"r"(param4));
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_EXIT));
    __asm__ __volatile__("stmdb  sp!, {lr}");
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {lr}");
    __asm__ __volatile__("mov    %0,  r0": "=r"(ret));

    return ret;
}

/*
 * exit
 */
void exit(int error_code) __attribute__ ((noreturn));
void exit(int error_code)
{
    __asm__ __volatile__("mov    r0,  %0": :"r"(error_code));
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_EXIT));
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("b      .");
}

/*
 * abort
 */
void abort(void)
{
    exit(0);
}

/*
 * tick_sleep
 */
static void tick_sleep(unsigned int t)
{
    __asm__ __volatile__("mov    r0,  %0": :"r"(t));
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_SLEEP));
    __asm__ __volatile__("stmdb  sp!, {lr}");
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {lr}");
}

/*
 * sleep
 */
void sleep(unsigned int s)
{
    tick_sleep(TICK_PER_SECOND * s);
}

/*
 * usleep
 */
void usleep(unsigned int us)
{
    tick_sleep(TICK_PER_SECOND * us / 1000000);
}

/*
 * write
 */
int write(int fd, char *data, unsigned int size)
{
    int ret;

    __asm__ __volatile__("mov    r0,  %0": :"r"(fd));
    __asm__ __volatile__("mov    r1,  %0": :"r"(data));
    __asm__ __volatile__("mov    r2,  %0": :"r"(size));
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_WRITE));
    __asm__ __volatile__("stmdb  sp!, {lr}");
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {lr}");
    __asm__ __volatile__("mov    %0,  r0": "=r"(ret));

    return ret;
}

/*
 * printf
 */
int printf(const char *fmt, ...)
{
    va_list va;
    char    buf[128];

    va_start(va, fmt);

    vsprintf(buf, fmt, va);

    write(0, buf, strlen(buf));

    va_end(va);

    return 0;
}

/*
 * select
 */
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();

    return -1;
}

/*
 * getpid
 */
int getpid(void)
{
    int ret;

    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_GETPID));
    __asm__ __volatile__("stmdb  sp!, {lr}");
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {lr}");
    __asm__ __volatile__("mov    %0,  r0": "=r"(ret));

    return ret;
}

/*
 * gettimeofday
 */
int _gettimeofday(struct timeval *tv, void *tzp)
{
    int ret;

    __asm__ __volatile__("mov    r0,  %0": :"r"(tv));
    __asm__ __volatile__("mov    r1,  %0": :"r"(tzp));
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_GETTIME));
    __asm__ __volatile__("stmdb  sp!, {lr}");
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {lr}");
    __asm__ __volatile__("mov    %0,  r0": "=r"(ret));

    return ret;
}

/*
 * 获得 errno 指针
 */
int *__errno(void)
{
    int *ret;

    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_ERRNO));
    __asm__ __volatile__("stmdb  sp!, {lr}");
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {lr}");
    __asm__ __volatile__("mov    %0,  r0": "=r"(ret));

    return ret;
}

/*
 * _sbrk
 */
void _sbrk(void)
{
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
