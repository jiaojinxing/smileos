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
     * 获得进程的 reent 结构, 赋于 _impure_ptr
     */
    extern struct _reent *getreent(void);
    _impure_ptr = getreent();

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
 * _fini
 */
void _fini(void)
{

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
