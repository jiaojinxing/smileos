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
** File name:               netjob.c
** Last modified Date:      2012-3-25
** Last Version:            1.0.0
** Descriptions:            ���繤���߳�
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
#include "kern/kern.h"
#include "kern/ipc.h"
/*********************************************************************************************************
** ���繤����Ϣ����
*********************************************************************************************************/
static mqueue_t     mqueue;
/*********************************************************************************************************
** ���繤����Ϣ
*********************************************************************************************************/
typedef struct {
    void          (*func)(void *);
    void           *arg;
} msg_t;
/*********************************************************************************************************
** Function name:           netjob
** Descriptions:            ���繤���߳�
** input parameters:        arg                 ����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void netjob(void *arg)
{
    msg_t *msg;

    while (1) {
        if (mqueue_fetch(&mqueue, (void **)&msg, 0) == 0) {

            msg->func(msg->arg);

            kfree(msg);
        }
    }
}
/*********************************************************************************************************
** Function name:           netjob_create
** Descriptions:            �������繤���߳�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void netjob_create(void)
{
    mqueue_new(&mqueue, 100);

    kthread_create("netjob", netjob, NULL, 8 * KB, 80);
}
/*********************************************************************************************************
** Function name:           netjob_add
** Descriptions:            ����һ�����繤��
** input parameters:        func                ���繤������
**                          arg                 ���繤����������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int netjob_add(void (*func)(void *), void *arg)
{
    msg_t  *msg;

    if (func == NULL) {
        return -1;
    }

    msg = kmalloc(sizeof(msg_t));
    if (msg == NULL) {
        return -1;
    }

    msg->func = func;
    msg->arg  = arg;

    if (mqueue_post(&mqueue, msg, 0) < 0) {
        kfree(msg);
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
