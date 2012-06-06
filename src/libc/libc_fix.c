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

/*
 * Reverse memchr()
 * Find the last occurrence of 'c' in the buffer 's' of size 'n'.
 */
void *
memrchr(const void *s, int c, size_t n)
{
    const unsigned char *cp;

    if (n != 0) {
        cp = (unsigned char *)s + n;
        do {
            if (*(--cp) == (unsigned char)c) {
                return ((void *)cp);
            }
        } while (--n != 0);
    }
    return (NULL);
}

/*
 * Find the first occurrence of C in S or the final NUL byte.
 */
char *
strchrnul(const char *s, int c_in)
{
    char c = c_in;

    while (*s && (*s != c)) {
        s++;
    }

    return (char *)s;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
