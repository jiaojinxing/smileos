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
 * 安全写
 */
ssize_t FAST_FUNC safe_write(int fd, const void *buf, size_t len)
{
    ssize_t n;

    do {
        n = write(fd, buf, len);
    } while (n < 0 && errno == EINTR);

    return n;
}

/*
 * 安全读
 */
ssize_t FAST_FUNC safe_read(int fd, void *buf, size_t len)
{
    ssize_t n;

    do {
        n = read(fd, buf, len);
    } while (n < 0 && errno == EINTR);

    return n;
}

/*
 * 完全写
 */
ssize_t FAST_FUNC full_write(int fd, const void *buf, size_t len)
{
    ssize_t n;
    ssize_t total;

    total = 0;

    while (len > 0) {
        n = safe_write(fd, buf, len);

        if (n < 0) {
            if (total > 0) {
                return total;
            }
            return n;
        }

        total += n;
        buf = ((const char *)buf) + n;
        len -= n;
    }

    return total;
}

/*
 * 获得终端宽高
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
        *height = (int)win.ws_row;
    }

    if (width) {
        *width = (int)win.ws_col;
    }
}

/*
 * 判断最后一个字符是否指定的字符
 */
char* FAST_FUNC last_char_is(const char *s, int c)
{
    if (s != NULL && *s != '\0') {
        size_t len = strlen(s) - 1;
        s += len;
        if ((unsigned char)*s == c) {
            return (char *)s;
        }
    }
    return NULL;
}

/*
 * 输出一个字符
 */
int FAST_FUNC bb_putchar(int c)
{
    return fputc(c, stdout);
}

/*
 * 释放 getopt 分配的内存
 */
void getopt_free(void)
{

}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
