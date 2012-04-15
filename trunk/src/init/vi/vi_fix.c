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
** File name:               vi_fix.c
** Last modified Date:      2012-4-14
** Last Version:            1.0.0
** Descriptions:            vi 编辑器移植层
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-14
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
#include "libbb.h"

/*
 * Write all of the supplied buffer out to a file.
 * This does multiple writes as necessary.
 * Returns the amount written, or -1 on an error.
 */
ssize_t FAST_FUNC full_write(int fd, const void *buf, size_t len)
{
    ssize_t cc;
    ssize_t total;

    total = 0;

    while (len) {
        cc = safe_write(fd, buf, len);

        if (cc < 0) {
            if (total) {
                /* we already wrote some! */
                /* user can do another write to know the error code */
                return total;
            }
            return cc;  /* write() returns -1 on failure. */
        }

        total += cc;
        buf = ((const char *)buf) + cc;
        len -= cc;
    }

    return total;
}

/*
 * It is perfectly ok to pass in a NULL for either width or for
 * height, in which case that value will not be set.  It is also
 * perfectly ok to have CONFIG_FEATURE_AUTOWIDTH disabled, in
 * which case you will always get 80x24
 */
void FAST_FUNC get_terminal_width_height(int fd, int *width, int *height)
{
    struct winsize win = {0, 0, 0, 0};

    if (ioctl(fd, TIOCGWINSZ, &win) != 0) {
        win.ws_row = 24;
        win.ws_col = 80;
    }

    if (win.ws_row <= 1) {
        win.ws_row = 24;
    }

    if (win.ws_col <= 1) {
        win.ws_col = 80;
    }

    if (height) {
        *height = (int) win.ws_row;
    }

    if (width) {
        *width = (int) win.ws_col;
    }
}

/*
 * Find out if the last character of a string matches the one given.
 * Don't underrun the buffer if the string length is 0.
 */
char* FAST_FUNC last_char_is(const char *s, int c)
{
    if (s && *s) {
        size_t sz = strlen(s) - 1;
        s += sz;
        if ( (unsigned char)*s == c)
            return (char*)s;
    }
    return NULL;
}

ssize_t FAST_FUNC safe_write(int fd, const void *buf, size_t count)
{
    ssize_t n;

    do {
        n = write(fd, buf, count);
    } while (n < 0 && errno == EINTR);

    return n;
}

ssize_t FAST_FUNC safe_read(int fd, void *buf, size_t count)
{
    ssize_t n;

    do {
        n = read(fd, buf, count);
    } while (n < 0 && errno == EINTR);

    return n;
}

int FAST_FUNC bb_putchar(int ch)
{
    return fputc(ch, stdout);
}

void *xzalloc(size_t s)
{
    return memset(xmalloc(s), 0, s);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
