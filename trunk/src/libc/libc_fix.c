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

void *memrchr(const void *s, int c, size_t len)
{
    const unsigned char *e;

    if (len != 0) {
        e = (unsigned char *)s + len;
        do {
            if (*(--e) == (unsigned char)c) {
                return ((void *)e);
            }
        } while (--len != 0);
    }
    return (NULL);
}

char *strchrnul(const char *s, int c)
{
    char ch = c;

    while (*s != '\0' && (*s != ch)) {
        s++;
    }

    return (char *)s;
}

char *xstrndup(const char *s, size_t len)
{
    char *ptr;

    ptr = strndup(s, len);
    if (ptr == NULL) {
        fprintf(stderr, "%s error!\n", __func__);
    }
    return ptr;
}

char *xstrdup(const char *s)
{
    char *ptr;

    ptr = strdup(s);
    if (ptr == NULL) {
        fprintf(stderr, "%s error!\n", __func__);
    }
    return ptr;
}

void *xmalloc(size_t s)
{
    void *ptr;

    ptr = malloc(s);
    if (ptr == NULL) {
        fprintf(stderr, "%s error!\n", __func__);
    }
    return ptr;
}

void *xzalloc(size_t s)
{
    void *ptr;

    ptr = xmalloc(s);
    if (ptr != NULL) {
        memset(ptr, 0, s);
    }
    return ptr;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
