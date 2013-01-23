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
** Descriptions:            网络工作线程
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
#include "kern/kern.h"
#include "kern/ipc.h"
/*********************************************************************************************************
** 网络工作消息队列
*********************************************************************************************************/
static mqueue_t     mqueue;
/*********************************************************************************************************
** 网络工作消息
*********************************************************************************************************/
typedef struct {
    void          (*func)(void *);
    void           *arg;
} msg_t;
/*********************************************************************************************************
** Function name:           netjob
** Descriptions:            网络工作线程
** input parameters:        arg                 参数
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
** Descriptions:            创建网络工作线程
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
** Descriptions:            增加一个网络工作
** input parameters:        func                网络工作函数
**                          arg                 网络工作函数参数
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
