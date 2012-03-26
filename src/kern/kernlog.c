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
** File name:               kernlog.c
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
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/ipc.h"

#include <stdio.h>
#include <stdarg.h>

/*
 * 内核日志邮箱
 */
static kern_mbox_t  kernlog_mbox;

/*
 * 内核日志邮件
 */
typedef struct {
    int         len;
    char        buf[LINE_MAX];
} kernlog_msg_t;

/*
 * 内核日志线程
 */
static void kernlog_thread(void *arg)
{
    kernlog_msg_t *msg;
    int            i;

    while (1) {
        if (kern_mbox_fetch(&kernlog_mbox, (void **)&msg, 0) == 0) {
            for (i = 0; i < msg->len && msg->buf[i] != '\0'; i++) {
                kputc(msg->buf[i]);
            }
            kfree(msg);
        }
    }
}

/*
 * 创建内核日志线程
 */
void kernlog_thread_create(void)
{
    kern_mbox_new(&kernlog_mbox, 100);

    kthread_create("kernlog", kernlog_thread, NULL, 16 * KB, 10);
}

/*
 * printk
 * 因为里面用了 kmalloc, 所以不能用在 kmalloc 失败之后, 终止内核前的报警也不能使用
 */
void printk(const char *fmt, ...)
{
    va_list        va;
    kernlog_msg_t *msg;

    msg = kmalloc(sizeof(kernlog_msg_t));
    if (msg == NULL) {
        return;
    }

    va_start(va, fmt);

    msg->len = vsnprintf(msg->buf, LINE_MAX, fmt, va);

    kern_mbox_post(&kernlog_mbox, msg, 0);

    va_end(va);
}

/*
 * 内核抱怨
 * 供不能用 printk 时使用
 */
void kcomplain(const char *fmt, ...)
{
    va_list va;
    char    buf[LINE_MAX];
    int     i;

    va_start(va, fmt);

    vsnprintf(buf, LINE_MAX, fmt, va);

    for (i = 0; buf[i] != '\0'; i++) {
        kputc(buf[i]);
    }

    va_end(va);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
