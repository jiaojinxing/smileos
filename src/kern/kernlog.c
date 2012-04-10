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
** Descriptions:            �ں���־
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-25
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/ipc.h"
#include <stdio.h>
#include <stdarg.h>

/*
 * �ں���־��Ϣ����
 */
static mqueue_t  mqueue;

/*
 * �ں���־��Ϣ
 */
typedef struct {
    int     len;
    char    buf[LINE_MAX];
} msg_t;

/*
 * �ں���־�߳�
 */
static void klogd(void *arg)
{
    msg_t *msg;
    int    i;

    while (1) {
        if (mqueue_fetch(&mqueue, (void **)&msg, 0) == 0) {
            for (i = 0; i < msg->len && msg->buf[i] != '\0'; i++) {
                kputc(msg->buf[i]);
            }
            kfree(msg);
        }
    }
}

/*
 * �����ں���־�߳�
 */
void klogd_create(void)
{
    mqueue_new(&mqueue, 100);

    kthread_create("klogd", klogd, NULL, 4 * KB, 5);
}

/*
 * printk
 * ��Ϊ�������� kmalloc, ���Բ������� kmalloc ʧ��֮��, ��ֹ�ں�ǰ�ı���Ҳ����ʹ��
 */
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

    mqueue_post(&mqueue, msg, 0);

    va_end(va);
}

/*
 * �ں˱�Թ
 * ������ʹ�� printk ʱʹ��
 */
void kcomplain(const char *fmt, ...)
{
    va_list va;
    char    buf[LINE_MAX];
    int     i;

    va_start(va, fmt);

    vsnprintf(buf, sizeof(buf), fmt, va);

    for (i = 0; buf[i] != '\0'; i++) {
        kputc(buf[i]);
    }

    va_end(va);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
