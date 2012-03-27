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
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-25
** Version:                 1.2.0
** Descriptions:            加入是否处于内核模式的判断, 以适合内核模式时的系统调用
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/sys_call.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <reent.h>
#ifdef SMILEOS_KERNEL
#include "kern/kern.h"
extern sys_do_t sys_do_table[];
#define debug_output(...)
#else
typedef int (*sys_do_t)();
static sys_do_t sys_do_table[1];
#define in_kernel()     0
#define debug_output(...)
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

    /*
     * 根据 APCS, 前四个参数使用 r0 - r3, 后面的参数使用堆栈
     * 因为切换了处理器模式, sp 也切换了, 所以多于 4 个参数时会有问题
     */
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
void _exit(int status)
{
    debug_output("%s\n", __func__);
    if (in_kernel()) {
        (sys_do_table[SYS_CALL_EXIT])(status);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(status));
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_EXIT));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("b      .");
    }
}

/*
 * yield
 */
void yield(void)
{
    debug_output("%s\n", __func__);
    if (in_kernel()) {
        (sys_do_table[SYS_CALL_YIELD])();
    } else {
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_YIELD));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
    }
}

/*
 * sleep_tick
 */
static void sleep_tick(unsigned int ticks)
{
    debug_output("%s\n", __func__);
    if (in_kernel()) {
        (sys_do_table[SYS_CALL_SLEEP])(ticks);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(ticks));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_SLEEP));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
    }
}

/*
 * sleep
 */
void sleep(unsigned int seconds)
{
    debug_output("%s\n", __func__);
    sleep_tick(TICK_PER_SECOND * seconds);
}

/*
 * usleep
 */
void usleep(useconds_t useconds)
{
    debug_output("%s\n", __func__);
    sleep_tick(TICK_PER_SECOND * useconds / 1000000);
}

/*
 * gettimeofday
 */
int _gettimeofday_r(struct _reent *ptr, struct timeval *tv, void *tzp)
{
    int ret;

    debug_output("%s\n", __func__);
    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_GETTIME])(tv, tzp);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(tv));
        __asm__ __volatile__("mov    r1,  %0": :"r"(tzp));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_GETTIME));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

/*
 * 获得 errno 指针
 */
int *__errno(void)
{
    int *ret;

    debug_output("%s\n", __func__);
    if (in_kernel()) {
        ret = (int *)(sys_do_table[SYS_CALL_ERRNO])();
    } else {
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_ERRNO));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

int _close_r(struct _reent *ptr, int fd)
{
    int ret;

    debug_output("%s\n", __func__);
    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_CLOSE])(fd);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(fd));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_CLOSE));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}


int _fcntl_r(struct _reent *ptr, int fd, int cmd, int arg)
{
    int ret;

    debug_output("%s\n", __func__);
    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_FCNTL])(fd, cmd, arg);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(fd));
        __asm__ __volatile__("mov    r1,  %0": :"r"(cmd));
        __asm__ __volatile__("mov    r2,  %0": :"r"(arg));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_FCNTL));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

int _fstat_r(struct _reent *ptr, int fd, struct stat *buf)
{
    int ret;

    debug_output("%s\n", __func__);
    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_FSTAT])(fd, buf);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(fd));
        __asm__ __volatile__("mov    r1,  %0": :"r"(buf));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_FSTAT));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

int _getpid_r(struct _reent *ptr)
{
    int ret;

    debug_output("%s\n", __func__);
    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_GETPID])();
    } else {
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_GETPID));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

int _isatty_r(struct _reent *ptr, int fd)
{
    int ret;

    debug_output("%s\n", __func__);
    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_ISATTY])(fd);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(fd));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_ISATTY));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

int _link_r(struct _reent *ptr, const char *path1, const char *path2)
{
    int ret;

    debug_output("%s\n", __func__);
    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_LINK])(path1, path2);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(path1));
        __asm__ __volatile__("mov    r1,  %0": :"r"(path2));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_LINK));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

_off_t _lseek_r(struct _reent *ptr, int fd, _off_t offset, int whence)
{
    _off_t ret;

    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_LSEEK])(fd, offset, whence);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(fd));
        __asm__ __volatile__("mov    r1,  %0": :"r"(offset));
        __asm__ __volatile__("mov    r2,  %0": :"r"(whence));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_LSEEK));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

int _mkdir_r(struct _reent *ptr, const char *path, int mode)
{
    int ret;

    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_MKDIR])(path, mode);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(path));
        __asm__ __volatile__("mov    r1,  %0": :"r"(mode));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_MKDIR));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

int _open_r(struct _reent *ptr, const char *path, int oflag, int mode)
{
    int ret;

    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_OPEN])(path, oflag, mode);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(path));
        __asm__ __volatile__("mov    r1,  %0": :"r"(oflag));
        __asm__ __volatile__("mov    r2,  %0": :"r"(mode));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_OPEN));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

_ssize_t _read_r(struct _reent *ptr, int fd, void *buf, size_t nbytes)
{
    _ssize_t ret;

    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_READ])(fd, buf, nbytes);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(fd));
        __asm__ __volatile__("mov    r1,  %0": :"r"(buf));
        __asm__ __volatile__("mov    r2,  %0": :"r"(nbytes));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_READ));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

_ssize_t _write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
    _ssize_t ret;

    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_WRITE])(fd, buf, nbytes);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(fd));
        __asm__ __volatile__("mov    r1,  %0": :"r"(buf));
        __asm__ __volatile__("mov    r2,  %0": :"r"(nbytes));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_WRITE));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

int _rename_r(struct _reent *ptr, const char *old, const char *new)
{
    int ret;

    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_RENAME])(old, new);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(old));
        __asm__ __volatile__("mov    r1,  %0": :"r"(new));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_RENAME));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

int _stat_r(struct _reent *ptr, const char *path, struct stat *buf)
{
    int ret;

    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_STAT])(path, buf);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(path));
        __asm__ __volatile__("mov    r1,  %0": :"r"(buf));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_STAT));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

int _unlink_r(struct _reent *ptr, const char *path)
{
    int ret;

    if (in_kernel()) {
        ret = (sys_do_table[SYS_CALL_UNLINK])(path);
    } else {
        __asm__ __volatile__("mov    r0,  %0": :"r"(path));
        __asm__ __volatile__("stmdb  sp!, {r7, lr}");
        __asm__ __volatile__("mov    r7,  %0": :"M"(SYS_CALL_UNLINK));
        __asm__ __volatile__("swi    0");
        __asm__ __volatile__("ldmia  sp!, {r7, lr}");
        __asm__ __volatile__("mov    %0,  r0": "=r"(ret));
    }

    return ret;
}

/*
 * select
 */
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
#ifdef SMILEOS_KERNEL
    printk("can't call %s()!, kill kthread %s tid=%d abort\n", __func__, current->name, current->tid);

    abort();
#else
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
#endif

    return -1;
}

/*
 * _sbrk
 */
void *_sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
#ifdef SMILEOS_KERNEL
    printk("can't call %s()!, kill kthread %s tid=%d abort\n", __func__, current->name, current->tid);

    abort();
#else
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
#endif
}

int _fork_r(struct _reent *ptr)
{
#ifdef SMILEOS_KERNEL
    printk("can't call %s()!, kill kthread %s tid=%d abort\n", __func__, current->name, current->tid);

    abort();
#else
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
#endif
}

_CLOCK_T_ _times_r(struct _reent *ptr, struct tms *buf)
{
#ifdef SMILEOS_KERNEL
    printk("can't call %s()!, kill kthread %s tid=%d abort\n", __func__, current->name, current->tid);

    abort();
#else
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
#endif
}

int _wait_r(struct _reent *ptr, int *status)
{
#ifdef SMILEOS_KERNEL
    printk("can't call %s()!, kill kthread %s tid=%d abort\n", __func__, current->name, current->tid);

    abort();
#else
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
#endif
}

int _execve_r(struct _reent *ptr, const char *path, char *const *argv, char *const *env)
{
#ifdef SMILEOS_KERNEL
    printk("can't call %s()!, kill kthread %s tid=%d abort\n", __func__, current->name, current->tid);

    abort();
#else
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
#endif
}

int _kill_r(struct _reent *ptr, int pid, int sig)
{
#ifdef SMILEOS_KERNEL
    printk("can't call %s()!, kill kthread %s tid=%d abort\n", __func__, current->name, current->tid);

    abort();
#else
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
#endif
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
