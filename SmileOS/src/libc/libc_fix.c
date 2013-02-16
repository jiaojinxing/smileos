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
** File name:               libc_fix.c
** Last modified Date:      2012-4-18
** Last Version:            1.0.0
** Descriptions:            newlib 修正
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
#include <unistd.h>
/*********************************************************************************************************
** 用户空间程序定义
*********************************************************************************************************/
#ifndef SMILEOS_KERNEL

#include "kern/pinfo.h"
#include "kern/addr_config.h"
#include "mm/heap.h"
#include <errno.h>
#include <fcntl.h>
/*********************************************************************************************************
** 用户空间内存堆
*********************************************************************************************************/
static heap_t       user_heap;
/*********************************************************************************************************
** Function name:           _malloc_r
** Descriptions:            malloc 桩函数
** input parameters:        reent               可重入结构
**                          size                需要分配的大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *_malloc_r(struct _reent *reent, size_t size)
{
    void *ptr;
    /*
     * 因进程里使用非抢占的 pthread, 免锁免关中断, 下同
     */
    ptr = heap_alloc(&user_heap, __func__, __LINE__, size);
    if (ptr != NULL) {
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return ptr;
}
/*********************************************************************************************************
** Function name:           _free_r
** Descriptions:            free 桩函数
** input parameters:        reent               可重入结构
**                          ptr                 内存指针
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void _free_r(struct _reent *reent, void *ptr)
{
    heap_free(&user_heap, __func__, __LINE__, ptr);

    reent->_errno = 0;
}
/*********************************************************************************************************
** Function name:           _realloc_r
** Descriptions:            realloc 桩函数
** input parameters:        reent               可重入结构
**                          ptr                 内存指针
**                          newsize             新的大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *_realloc_r(struct _reent *reent, void *ptr, size_t newsize)
{
    void *newptr;

    newptr = heap_alloc(&user_heap, __func__, __LINE__, newsize);
    if (newptr != NULL) {
        if (ptr != NULL) {
            memcpy(newptr, ptr, newsize);
            heap_free(&user_heap, __func__, __LINE__, ptr);
        }
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return newptr;
}
/*********************************************************************************************************
** Function name:           _calloc_r
** Descriptions:            calloc 桩函数
** input parameters:        reent               可重入结构
**                          ptr                 内存
**                          nelem               元素的个数
**                          elsize              元素的大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *_calloc_r(struct _reent *reent, size_t nelem, size_t elsize)
{
    void *ptr;

    ptr = heap_alloc(&user_heap, __func__, __LINE__, nelem * elsize);
    if (ptr != NULL) {
        memset(ptr, 0, nelem * elsize);
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return ptr;
}
/*********************************************************************************************************
** Function name:           libc_init
** Descriptions:            初始化 C 库
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void libc_init(void)
{
    /*
     * 设置进程信息
     */
    static pinfo_t info;

    info.reent = _impure_ptr;

    extern int setpinfo(void *info);
    setpinfo(&info);

    /*
     * 创建用户空间内存堆
     */
    extern unsigned char __bss_end;

    /*
     * 在 __bss_end 后, 进程栈空间前, 建立内存堆
     */
    heap_init(&user_heap, "user_heap", &__bss_end,
            PROCESS_SPACE_SIZE - (uint32_t)&__bss_end - PROCESS_STACK_SIZE - PROCESS_PARAM_SIZE);

    /*
     * 打开三个标准文件
     */
    open("/dev/serial0", O_RDONLY, 0666);
    stdin  = fdopen(STDIN_FILENO,  "r");

    open("/dev/serial0", O_WRONLY, 0666);
    stdout = fdopen(STDOUT_FILENO, "w");

    open("/dev/serial0", O_WRONLY, 0666);
    stderr = fdopen(STDERR_FILENO, "w");
}
#else
/*********************************************************************************************************
** 内核定义
*********************************************************************************************************/
#include "kern/kern.h"
/*********************************************************************************************************
** Function name:           _malloc_r
** Descriptions:            malloc 桩函数
** input parameters:        reent               可重入结构
**                          size                需要分配的大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *_malloc_r(struct _reent *reent, size_t size)
{
    void    *ptr;

    ptr = kmalloc(size, GFP_KERNEL);
    if (ptr != NULL) {
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return ptr;
}
/*********************************************************************************************************
** Function name:           _free_r
** Descriptions:            free 桩函数
** input parameters:        reent               可重入结构
**                          ptr                 内存指针
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void _free_r(struct _reent *reent, void *ptr)
{
    kfree(ptr);

    reent->_errno = 0;
}
/*********************************************************************************************************
** Function name:           _realloc_r
** Descriptions:            realloc 桩函数
** input parameters:        reent               可重入结构
**                          ptr                 内存指针
**                          newsize             新的大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *_realloc_r(struct _reent *reent, void *ptr, size_t newsize)
{
    void    *newptr;

    newptr = kmalloc(GFP_KERNEL, newsize);
    if (newptr != NULL) {
        if (ptr != NULL) {
            memcpy(newptr, ptr, newsize);
            kfree(ptr);
        }
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return newptr;
}
/*********************************************************************************************************
** Function name:           _calloc_r
** Descriptions:            calloc 桩函数
** input parameters:        reent               可重入结构
**                          ptr                 内存
**                          nelem               元素的个数
**                          elsize              元素的大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *_calloc_r(struct _reent *reent, size_t nelem, size_t elsize)
{
    void    *ptr;

    ptr = kmalloc(GFP_KERNEL, nelem * elsize);
    if (ptr != NULL) {
        memset(ptr, 0, nelem * elsize);
        reent->_errno = 0;
    } else {
        reent->_errno = ENOMEM;
    }
    return ptr;
}
#endif
/*********************************************************************************************************
** Function name:           MIN(在 newlib-1.19.0/newlib/libc/search/hash_bigkey.c 文件里)
** Descriptions:            返回两个数中的较小值
** input parameters:        x                   数 1
**                          y                   数 2
** output parameters:       NONE
** Returned value:          两个数中的较小值
*********************************************************************************************************/
int MIN(int x, int y)
{
    return x > y ? y : x;
}

/*
 * 在 newlib-1.19.0/newlib/libc/misc/init.c 文件里
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
/*********************************************************************************************************
** Function name:           _init
** Descriptions:            被 __libc_init_array 调用, 完成初始化工作
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void _init(void)
{

}
/*********************************************************************************************************
** Function name:           _fini
** Descriptions:            __libc_fini_array 调用, 完成清理工作
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void _fini(void)
{

}
/*********************************************************************************************************
** Function name:           memrchr
** Descriptions:            在内存里逆向查找指定的字符, 返回其逆向第一次出现的位置
** input parameters:        ptr                 内存
**                          ch                  指定的字符
**                          size                内存的大小
** output parameters:       NONE
** Returned value:          字符逆向第一次出现的位置
*********************************************************************************************************/
void *memrchr(const void *ptr, int ch, size_t size)
{
    const unsigned char *end;

    if (size != 0) {
        end = (unsigned char *)ptr + size;
        do {
            if (*(--end) == (unsigned char)ch) {
                return ((void *)end);
            }
        } while (--size != 0);
    }
    return (NULL);
}
/*********************************************************************************************************
** Function name:           strchrnul
** Descriptions:            在字符串中查找指定的字符, 返回其第一次出现的位置
** input parameters:        str                 字符串
**                          ch                  指定的字符
** output parameters:       NONE
** Returned value:          字符第一次出现的位置
*********************************************************************************************************/
char *strchrnul(const char *str, int ch)
{
    char tmp = ch;

    while (*str != '\0' && (*str != tmp)) {
        str++;
    }
    return (char *)str;
}
/*********************************************************************************************************
** Function name:           xstrndup
** Descriptions:            复制字符串
** input parameters:        str                 字符串
**                          len                 字符串长度
** output parameters:       NONE
** Returned value:          新的字符串副本
*********************************************************************************************************/
char *xstrndup(const char *str, size_t len)
{
    char *ptr;

    ptr = strndup(str, len);
    if (ptr == NULL) {
        fprintf(stderr, "%s error!\n", __func__);
    }
    return ptr;
}
/*********************************************************************************************************
** Function name:           xstrdup
** Descriptions:            复制字符串
** input parameters:        str                 字符串
** output parameters:       NONE
** Returned value:          新的字符串副本
*********************************************************************************************************/
char *xstrdup(const char *str)
{
    char *ptr;

    ptr = strdup(str);
    if (ptr == NULL) {
        fprintf(stderr, "%s error!\n", __func__);
    }
    return ptr;
}
/*********************************************************************************************************
** Function name:           xmalloc
** Descriptions:            分配一块内存
** input parameters:        size                大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *xmalloc(size_t size)
{
    void *ptr;

    ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "%s error!\n", __func__);
    }
    return ptr;
}
/*********************************************************************************************************
** Function name:           xzalloc
** Descriptions:            分配一块零值内存
** input parameters:        size                大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *xzalloc(size_t size)
{
    void *ptr;

    ptr = xmalloc(size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
    }
    return ptr;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
