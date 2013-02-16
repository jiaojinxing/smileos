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
** File name:               workqueue.c
** Last modified Date:      2012-3-25
** Last Version:            1.0.0
** Descriptions:            ��������
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
** ����
*********************************************************************************************************/
#define WQ_MIN_SIZE                 (100)
#define WQ_THREAD_STACKSIZE         (16 * KB)
#define WQ_THREAD_DEFAULT_PRIO      (90)
/*********************************************************************************************************
** ������Ϣ
*********************************************************************************************************/
typedef struct {
    void                  (*func)(void *);
    void                   *arg;
} msg_t;
/*********************************************************************************************************
** ��������
*********************************************************************************************************/
struct workqueue {
    mqueue_t                wq;
};
/*********************************************************************************************************
** �ڲ�����
*********************************************************************************************************/
static  workqueue_t         default_wq;                                 /*  ȱʡ�Ĺ�������              */
/*********************************************************************************************************
** Function name:           workqueue_thread
** Descriptions:            ���������߳�
** input parameters:        arg                 ����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void workqueue_thread(void *arg)
{
    workqueue_t *wq = arg;
    msg_t       *msg;

    while (1) {
        if (mqueue_fetch((mqueue_t *)wq, (void **)&msg, 0) == 0) {

            msg->func(msg->arg);

            kfree(msg);
        }
    }
}
/*********************************************************************************************************
** Function name:           workqueue_create
** Descriptions:            ������������
** input parameters:        name                ����
**                          size                ��С
**                          priority            ���ȼ�
** output parameters:       wq                  ��������
** Returned value:          0 OR -1
*********************************************************************************************************/
int workqueue_create(workqueue_t *wq, const char *name, size_t size, uint8_t priority)
{
    if (wq == NULL) {
        return -1;
    }

    if (size < WQ_MIN_SIZE) {
        size = WQ_MIN_SIZE;
    }

    if (mqueue_create((mqueue_t *)wq, size) < 0) {
        return -1;
    }

    if (priority == 0) {
        priority = WQ_THREAD_DEFAULT_PRIO;
    }

    if (kthread_create(name, workqueue_thread, wq, WQ_THREAD_STACKSIZE, priority) < 0) {
        mqueue_destroy((mqueue_t *)wq);
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           workqueue_add
** Descriptions:            ����һ������
** input parameters:        wq                  ��������
**                          func                ��������
**                          arg                 ������������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int workqueue_add(workqueue_t *wq, void (*func)(void *), void *arg)
{
    msg_t  *msg;

    if (func == NULL) {
        return -1;
    }

    msg = kmalloc(sizeof(msg_t), GFP_KERNEL);
    if (msg == NULL) {
        return -1;
    }

    msg->func = func;
    msg->arg  = arg;

    if (wq == NULL) {
        wq = &default_wq;
    }

    if (mqueue_post((mqueue_t *)wq, msg, 0) < 0) {
        kfree(msg);
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           default_workqueue_create
** Descriptions:            ����ȱʡ�Ĺ�������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
void default_workqueue_create(void)
{
    workqueue_create(&default_wq, "default_wq", 0, 0);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
