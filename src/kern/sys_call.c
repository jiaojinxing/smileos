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
#include "config.h"
#include "types.h"
#include "sys_call.h"
#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>

/*
 * 进程退出
 */
int exit(int error_code)
{
    int ret;

    __asm__(
        "mov    r0, %1\n"                                               /*  r0 是参数 1                 */
        "mov    r7, %2\n"                                               /*  r7 是系统调用号             */
        "stmdb  sp!, {lr}\n"                                            /*  保存 LR 到堆栈              */
        "swi    0\n"                                                    /*  软件中断                    */
        "ldmia  sp!, {lr}\n"                                            /*  从堆栈恢复 LR               */
        "mov    %0, r0\n"                                               /*  r0 是返回值                 */
        :"=r"(ret)
        :"r"(error_code), "M"(SYS_CALL_EXIT)
        :"r0"
        );
    return ret;
}

void abort(void)
{

}

void usleep(uint32_t us)
{

}

int getpid(void)
{
    return 0;
}
/*
 * 进程休眠
 */
int sleep(int time)
{
    int ret;

    __asm__(
        "mov    r0, %1\n"                                               /*  r0 是参数 1                 */
        "mov    r7, %2\n"                                               /*  r7 是系统调用号             */
        "stmdb  sp!, {lr}\n"                                            /*  保存 LR 到堆栈              */
        "swi    0\n"                                                    /*  软件中断                    */
        "ldmia  sp!, {lr}\n"                                            /*  从堆栈恢复 LR               */
        "mov    %0, r0\n"                                               /*  r0 是返回值                 */
        :"=r"(ret)
        :"r"(time), "M"(SYS_CALL_SLEEP)
        :"r0"
        );
    return ret;
}

/*
 * 写
 */
int write(char *str)
{
    int ret;

    __asm__(
        "mov    r0, %1\n"
        "mov    r7, %2\n"
        "stmdb  sp!, {lr}\n"
        "swi    0\n"
        "ldmia  sp!, {lr}\n"
        "mov    %0, r0\n"
        :"=r"(ret)
        :"r"(str), "M"(SYS_CALL_WRITE)
        :"r0"
        );
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

    write(buf);

    va_end(va);

    return 0;
}

/*
 * malloc
 */
void *malloc(uint32_t size)
{
    void *ret;

    __asm__(
        "mov    r0, %1\n"
        "mov    r7, %2\n"
        "stmdb  sp!, {lr}\n"
        "swi    0\n"
        "ldmia  sp!, {lr}\n"
        "mov    %0, r0\n"
        :"=r"(ret)
        :"r"(size), "M"(SYS_CALL_MALLOC)
        :"r0"
        );
    return ret;
}

/*
 * free
 */
void *free(void *ptr)
{
    void *ret;

    __asm__(
        "mov    r0, %1\n"
        "mov    r7, %2\n"
        "stmdb  sp!, {lr}\n"
        "swi    0\n"
        "ldmia  sp!, {lr}\n"
        "mov    %0, r0\n"
        :"=r"(ret)
        :"r"(ptr), "M"(SYS_CALL_FREE)
        :"r0"
        );
    return ret;
}

/*
 * heap_init
 */
int heap_init(uint8_t *base, uint32_t size)
{
    void *ret;

    __asm__(
        "mov    r0, %1\n"
        "mov    r1, %2\n"
        "mov    r7, %3\n"
        "stmdb  sp!, {lr}\n"
        "swi    0\n"
        "ldmia  sp!, {lr}\n"
        "mov    %0, r0\n"
        :"=r"(ret)
        :"r"(base), "r"(size), "M"(SYS_CALL_HEAP_INIT)
        :"r0"
        );
    return ret;
}

int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
            struct timeval *timeout)
{
    return 0;
}

void _sbrk(void)
{

}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
