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
** Descriptions:            newlib ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-18
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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/*********************************************************************************************************
** �û��ռ������
*********************************************************************************************************/
#ifndef SMILEOS_KERNEL

#include "kern/pinfo.h"
#include "kern/addr_config.h"
#include "mm/heap.h"
#include <errno.h>
#include <fcntl.h>
/*********************************************************************************************************
** �û��ռ��ڴ��
*********************************************************************************************************/
static heap_t               user_heap;
/*********************************************************************************************************
** Function name:           _malloc_r
** Descriptions:            malloc ׮����
** input parameters:        reent               ������ṹ
**                          size                ��Ҫ����Ĵ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
*********************************************************************************************************/
void *_malloc_r(struct _reent *reent, size_t size)
{
    void *ptr;
    /*
     * �������ʹ�÷���ռ�� pthread, ��������ж�, ��ͬ
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
** Descriptions:            free ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�ָ��
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
** Descriptions:            realloc ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�ָ��
**                          newsize             �µĴ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
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
** Descriptions:            calloc ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�
**                          nelem               Ԫ�صĸ���
**                          elsize              Ԫ�صĴ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
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
** Descriptions:            ��ʼ�� C ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void libc_init(void)
{
    /*
     * ���ý�����Ϣ
     */
    static pinfo_t info;

    info.reent = _impure_ptr;

    extern int setpinfo(void *info);
    setpinfo(&info);

    /*
     * �����û��ռ��ڴ��
     */
    extern unsigned char __bss_end;

    /*
     * �� __bss_end ��, ����ջ�ռ�ǰ, �����ڴ��
     */
    heap_init(&user_heap, "user_heap", &__bss_end,
            PROCESS_SPACE_SIZE - (uint32_t)&__bss_end - PROCESS_STACK_SIZE - PROCESS_PARAM_SIZE);

    /*
     * ��������׼�ļ�
     */
    open("/dev/serial0", O_RDONLY, 0666);
    stdin  = fdopen(STDIN_FILENO,  "r");

    open("/dev/serial0", O_WRONLY, 0666);
    stdout = fdopen(STDOUT_FILENO, "w");

    open("/dev/serial0", O_WRONLY, 0666);
    stderr = fdopen(STDERR_FILENO, "w");
}
/*********************************************************************************************************
** �ں˶���
*********************************************************************************************************/
#else
#include "kern/kern.h"
/*********************************************************************************************************
** Function name:           _malloc_r
** Descriptions:            malloc ׮����
** input parameters:        reent               ������ṹ
**                          size                ��Ҫ����Ĵ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
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
** Descriptions:            free ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�ָ��
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
** Descriptions:            realloc ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�ָ��
**                          newsize             �µĴ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
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
** Descriptions:            calloc ׮����
** input parameters:        reent               ������ṹ
**                          ptr                 �ڴ�
**                          nelem               Ԫ�صĸ���
**                          elsize              Ԫ�صĴ�С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
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
** Function name:           MIN(�� newlib-1.19.0/newlib/libc/search/hash_bigkey.c �ļ���)
** Descriptions:            �����������еĽ�Сֵ
** input parameters:        x                   �� 1
**                          y                   �� 2
** output parameters:       NONE
** Returned value:          �������еĽ�Сֵ
*********************************************************************************************************/
int MIN(int x, int y)
{
    return x > y ? y : x;
}

/*
 * �� newlib-1.19.0/newlib/libc/misc/init.c �ļ���
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
** Descriptions:            �� __libc_init_array ����, ��ɳ�ʼ������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void _init(void)
{

}
/*********************************************************************************************************
** Function name:           _fini
** Descriptions:            __libc_fini_array ����, ���������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void _fini(void)
{

}
/*********************************************************************************************************
** Function name:           memrchr
** Descriptions:            ���ڴ����������ָ�����ַ�, �����������һ�γ��ֵ�λ��
** input parameters:        ptr                 �ڴ�
**                          ch                  ָ�����ַ�
**                          size                �ڴ�Ĵ�С
** output parameters:       NONE
** Returned value:          �ַ������һ�γ��ֵ�λ��
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
** Descriptions:            ���ַ����в���ָ�����ַ�, �������һ�γ��ֵ�λ��
** input parameters:        str                 �ַ���
**                          ch                  ָ�����ַ�
** output parameters:       NONE
** Returned value:          �ַ���һ�γ��ֵ�λ��
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
** Descriptions:            �����ַ���
** input parameters:        str                 �ַ���
**                          len                 �ַ�������
** output parameters:       NONE
** Returned value:          �µ��ַ�������
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
** Descriptions:            �����ַ���
** input parameters:        str                 �ַ���
** output parameters:       NONE
** Returned value:          �µ��ַ�������
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
** Descriptions:            ����һ���ڴ�
** input parameters:        size                ��С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
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
** Descriptions:            ����һ����ֵ�ڴ�
** input parameters:        size                ��С
** output parameters:       NONE
** Returned value:          �ڴ�ָ��
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
