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
** File name:               libc_fix.c
** Last modified Date:      2012-4-18
** Last Version:            1.0.0
** Descriptions:            libc 修正
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-18
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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef SMILEOS_KERNEL
/*
 * 初始化 C 库
 */
void libc_init(void)
{
    /*
     * 用 _impure_ptr 设置进程的 reent 结构指针
     */
    extern int setreent(struct _reent *reent);
    setreent(_impure_ptr);

    /*
     * 创建用户空间内存堆
     */
    extern void uheap_create(void);
    uheap_create();

    /*
     * 打开三个标准文件
     */
    open("/dev/ttyS0", O_RDONLY, 0666);
    stdin  = fdopen(STDIN_FILENO,  "r");

    open("/dev/ttyS0", O_WRONLY, 0666);
    stdout = fdopen(STDOUT_FILENO, "w");

    open("/dev/ttyS0", O_WRONLY, 0666);
    stderr = fdopen(STDERR_FILENO, "w");
}



/*
 * printk
 */
void printk(const char *fmt, ...)
{
    va_list va;
    char    buffer[256];

    va_start(va, fmt);

    vsnprintf(buffer, 256, fmt, va);

    fputs(buffer, stderr);

    va_end(va);
}

/*
 * 在 newlib-1.19.0/newlib/libc/search/hash_bigkey.c
 */
int MIN(int x, int y)
{
    return x > y ? y : x;
}

void sigprocmask(void)
{

}

/*
 * 在 newlib-1.19.0/newlib/libc/misc/init.c
 */
#if 0
/* These magic symbols are provided by the linker.  */
extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));
extern void (*__fini_array_start []) (void) __attribute__((weak));
extern void (*__fini_array_end []) (void) __attribute__((weak));

extern void _init (void);
extern void _fini (void);

/* Iterate over all the init routines.  */
void
__libc_init_array (void)
{
  size_t count;
  size_t i;

  count = __preinit_array_end - __preinit_array_start;
  for (i = 0; i < count; i++)
    __preinit_array_start[i] ();

  _init ();

  count = __init_array_end - __init_array_start;
  for (i = 0; i < count; i++)
    __init_array_start[i] ();
}

/* Run all the cleanup routines.  */
void
__libc_fini_array (void)
{
  size_t count;
  size_t i;

  count = __fini_array_end - __fini_array_start;
  for (i = 0; i < count; i++)
    __fini_array_start[i] ();

  _fini ();
}
#endif
/*
 * _init
 */
void _init(void)
{

}

/*
 * _fini
 */
void _fini(void)
{

}

#endif

void *memrchr(const void *ptr, int ch, size_t len)
{
    const unsigned char *end;

    if (len != 0) {
        end = (unsigned char *)ptr + len;
        do {
            if (*(--end) == (unsigned char)ch) {
                return ((void *)end);
            }
        } while (--len != 0);
    }
    return (NULL);
}

char *strchrnul(const char *str, int ch)
{
    char tmp = ch;

    while (*str != '\0' && (*str != tmp)) {
        str++;
    }
    return (char *)str;
}

char *xstrndup(const char *str, size_t len)
{
    char *ptr;

    ptr = strndup(str, len);
    if (ptr == NULL) {
        fprintf(stderr, "%s error!\n", __func__);
    }
    return ptr;
}

char *xstrdup(const char *str)
{
    char *ptr;

    ptr = strdup(str);
    if (ptr == NULL) {
        fprintf(stderr, "%s error!\n", __func__);
    }
    return ptr;
}

void *xmalloc(size_t len)
{
    void *ptr;

    ptr = malloc(len);
    if (ptr == NULL) {
        fprintf(stderr, "%s error!\n", __func__);
    }
    return ptr;
}

void *xzalloc(size_t len)
{
    void *ptr;

    ptr = xmalloc(len);
    if (ptr != NULL) {
        memset(ptr, 0, len);
    }
    return ptr;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
