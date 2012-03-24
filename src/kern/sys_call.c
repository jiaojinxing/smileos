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
** Modified by:             JiaoJinXing
** Modified date:           2012-3-23
** Version:                 1.1.0
** Descriptions:            修改没有保存 R7 的错误
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/sys_call.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#ifdef SMILEOS_KERNEL
#include "kern/kern.h"
#endif

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

    __asm__ __volatile__("mov    r0,  %0": :"r"(param1));               /*  R0 传递参数 1               */
    __asm__ __volatile__("mov    r1,  %0": :"r"(param2));               /*  R1 传递参数 2               */
    __asm__ __volatile__("mov    r2,  %0": :"r"(param3));               /*  R2 传递参数 3               */
    __asm__ __volatile__("mov    r3,  %0": :"r"(param4));               /*  R3 传递参数 4               */
    __asm__ __volatile__("stmdb  sp!, {r7, lr}");                       /*  保存 R7, LR 到堆栈          */
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_EXIT));        /*  R7 传递系统调用号           */
    __asm__ __volatile__("swi    0");                                   /*  软件中断                    */
    __asm__ __volatile__("ldmia  sp!, {r7, lr}");                       /*  从堆栈恢复 R7, LR           */
    __asm__ __volatile__("mov    %0,  r0": "=r"(ret));                  /*  R0 传递返回值               */

    return ret;
}

/*
 * exit
 */
void exit(int error) __attribute__ ((noreturn));
void exit(int error)
{
    __asm__ __volatile__("mov    r0,  %0": :"r"(error));
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
 * yield
 */
void yield(void)
{
    __asm__ __volatile__("stmdb  sp!, {r7, lr}");
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_YIELD));
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {r7, lr}");
}

/*
 * sleep_tick
 */
static void sleep_tick(unsigned int tick)
{
    __asm__ __volatile__("mov    r0,  %0": :"r"(tick));
    __asm__ __volatile__("stmdb  sp!, {r7, lr}");
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_SLEEP));
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {r7, lr}");
}

/*
 * sleep
 */
void sleep(unsigned int s)
{
    sleep_tick(TICK_PER_SECOND * s);
}

/*
 * usleep
 */
void usleep(unsigned int us)
{
    sleep_tick(TICK_PER_SECOND * us / 1000000);
}

/*
 * write
 */
int write(int fd, const char *data, unsigned int size)
{
    int ret;

    __asm__ __volatile__("mov    r0,  %0": :"r"(fd));
    __asm__ __volatile__("mov    r1,  %0": :"r"(data));
    __asm__ __volatile__("mov    r2,  %0": :"r"(size));
    __asm__ __volatile__("stmdb  sp!, {r7, lr}");
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_WRITE));
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {r7, lr}");
    __asm__ __volatile__("mov    %0,  r0": "=r"(ret));

    return ret;
}

/*
 * printf
 */
int printf(const char *fmt, ...)
{
    va_list va;
    char    buf[256];

    va_start(va, fmt);

    vsprintf(buf, fmt, va);

    write(0, buf, strlen(buf));

    va_end(va);

    return 0;
}

/*
 * puts
 */
int puts(const char *str)
{
    write(0, str, strlen(str));
    write(0, "\n", 1);

    return 0;
}

/*
 * select
 */
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
#ifdef SMILEOS_KERNEL
    printk("can't call %s()!, kill kthread %d abort\n", __func__, current->tid);

    abort();
#else
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
#endif

    return -1;
}

/*
 * getpid
 */
int getpid(void)
{
    int ret;

    __asm__ __volatile__("stmdb  sp!, {r7, lr}");
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_GETPID));
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {r7, lr}");
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
    __asm__ __volatile__("stmdb  sp!, {r7, lr}");
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_GETTIME));
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {r7, lr}");
    __asm__ __volatile__("mov    %0,  r0": "=r"(ret));

    return ret;
}

/*
 * 获得 errno 指针
 */
#ifndef SMILEOS_KERNEL
int *__errno(void)
{
    int *ret;

    __asm__ __volatile__("stmdb  sp!, {r7, lr}");
    __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_ERRNO));
    __asm__ __volatile__("swi    0");
    __asm__ __volatile__("ldmia  sp!, {r7, lr}");
    __asm__ __volatile__("mov    %0,  r0": "=r"(ret));

    return ret;
}
#else
int *__errno(void)
{
    int *ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = &current->errno;

    interrupt_resume(reg);

    return ret;
}
#endif

/*
 * _sbrk
 */
void _sbrk(void)
{
#ifdef SMILEOS_KERNEL
    printk("can't call %s()!, kill kthread %d abort\n", __func__, current->tid);

    abort();
#else
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
#endif
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
