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
** File name:               klogd.c
** Last modified Date:      2012-3-25
** Last Version:            1.0.0
** Descriptions:            内核日志
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-25
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-8-22
** Version:                 1.1.0
** Descriptions:            增加注释, 修改 kcomplain, 不调用 C 库函数 vsnprintf 实现
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/ipc.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
/*********************************************************************************************************
** 内核日志消息队列
*********************************************************************************************************/
static mqueue_t  mqueue;
/*********************************************************************************************************
** 内核日志消息
*********************************************************************************************************/
typedef struct {
    int     len;
    char    buf[LINE_MAX];
} msg_t;
/*********************************************************************************************************
** Function name:           klogd
** Descriptions:            内核日志线程
** input parameters:        arg                 参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void klogd(void *arg)
{
    msg_t *msg;

    while (1) {
        if (mqueue_fetch(&mqueue, (void **)&msg, 0) == 0) {
            write(STDOUT_FILENO, msg->buf, msg->len);
            kfree(msg);
        }
    }
}
/*********************************************************************************************************
** Function name:           klogd_create
** Descriptions:            创建内核日志线程
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void klogd_create(void)
{
    mqueue_new(&mqueue, 100);

    kthread_create("klogd", klogd, NULL, 4 * KB, 5);
}
/*********************************************************************************************************
** Function name:           printk
** Descriptions:            因为里面用了 kmalloc, 所以不能用在 kmalloc 失败之后,
**                          终止内核前的报警也不能使用
** input parameters:        fmt                 格式字符串
**                          ...                 其余参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void printk(const char *fmt, ...)
{
    va_list va;
    msg_t  *msg;

    msg = kmalloc(sizeof(msg_t));
    if (msg == NULL) {
        return;
    }

    va_start(va, fmt);

    msg->len = vsnprintf(msg->buf, sizeof(msg->buf), fmt, va);

    va_end(va);

    mqueue_post(&mqueue, msg, 0);
}
/*********************************************************************************************************
** Function name:           kcomplain
** Descriptions:            内核抱怨(供不能使用 printk 时使用)
** input parameters:        fmt                 格式字符串
**                          ...                 其余参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kcomplain(const char *fmt, ...)
{
    static const char digits[] = "0123456789abcdef";
    va_list ap;
    char buf[10];
    char *s;
    char c;
    unsigned u;
    int i, pad;

    va_start(ap, fmt);
    while ((c = *fmt++) != '\0') {
        if (c == '%') {
            c = *fmt++;
            /*
             * ignore long
             */
            if (c == 'l') {
                c = *fmt++;
            }
            switch (c) {
            case 'c':
                kputc(va_arg(ap, int));
                continue;

            case 's':
                s = va_arg(ap, char *);
                if (s == NULL) {
                    s = "<NULL>";
                }
                for (; *s != '\0'; s++) {
                    kputc(*s);
                }
                continue;

            case 'd':
                c = 'u';
            case 'u':
            case 'x':
                u = va_arg(ap, unsigned);
                s = buf;
                if (c == 'u') {
                    do {
                        *s++ = digits[u % 10U];
                    } while (u /= 10U);
                } else {
                    pad = 0;
                    for (i = 0; i < 8; i++) {
                        if (pad) {
                            *s++ = '0';
                        } else {
                            *s++ = digits[u % 16U];
                            if ((u /= 16U) == 0) {
                                pad = 1;
                            }
                        }
                    }
                }
                while (--s >= buf) {
                    kputc(*s);
                }
                continue;
            }
        }
        if (c == '\n') {
            kputc('\r');
        }
        kputc((int) c);
    }
    va_end(ap);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
