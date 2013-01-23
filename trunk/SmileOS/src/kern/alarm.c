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
** File name:               alarmd.c
** Last modified Date:      2013-1-7
** Last Version:            1.0.0
** Descriptions:            alarm 线程
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-1-7
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
#include <signal.h>
/*********************************************************************************************************
** alarm 消息队列
*********************************************************************************************************/
static mqueue_t     mqueue;
/*********************************************************************************************************
** alarm 消息
*********************************************************************************************************/
typedef struct _msg_t {
    struct _msg_t  *next;
    uint32_t        ms;
    int32_t         tid;
} msg_t;
/*********************************************************************************************************
** Function name:           alarmd
** Descriptions:            alarm 线程
** input parameters:        arg                 参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void alarmd(void *arg)
{
    msg_t      *msg, *prev, *pos;
    uint32_t    ret;
    uint64_t    tick;
    uint32_t    ms;
    uint32_t    tv;
    msg_t      *list;
    msg_t       head_node;

    list = &head_node;
    head_node.next = NULL;

    tv = 0;

    while (1) {
        tick = getticks();

        ret  = mqueue_fetch(&mqueue, (void **)&msg, tv);

        tick = getticks() - tick;

        ms   = tick * 1000 / TICK_PER_SECOND;

        prev = list;
        pos  = list->next;

        tv   = -1;

        while (pos != NULL) {
            if (pos->ms <= ms) {
                kill(pos->tid, SIGALRM);
                prev->next = pos->next;
                kfree(pos);
                pos = prev->next;
            } else {
                pos->ms -= ms;
                prev = pos;
                pos  = pos->next;
                if (pos->ms < tv) {
                    tv = pos->ms;
                }
             }
        }

        if (ret == 0) {
            prev = list;
            pos  = list->next;
            while (pos != NULL) {
                if (pos->tid == msg->tid) {
                    prev->next = pos->next;
                    kfree(pos);
                    break;
                } else {
                    prev = pos;
                    pos  = pos->next;
                 }
            }

            if (msg->ms != 0) {
                msg->next  = list->next;
                list->next = msg;
                if (msg->ms < tv) {
                    tv = msg->ms;
                }
            } else {
                /*
                 * TODO: 清除所有闹钟
                 */
                kfree(msg);
            }
        }

        if (tv > 1000 / TICK_PER_SECOND) {
            tv = tv * TICK_PER_SECOND / 1000;
        } else {
            tv = 1;
        }
    }
}
/*********************************************************************************************************
** Function name:           alarmd_create
** Descriptions:            创建 alarm 线程
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void alarmd_create(void)
{
    mqueue_new(&mqueue, 100);

    kthread_create("alarmd", alarmd, NULL, 4 * KB, 100);
}
/*********************************************************************************************************
** Function name:           task_alarm
** Descriptions:            任务定时
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
int task_alarm(uint32_t ms)
{
    msg_t  *msg;

    msg = kmalloc(sizeof(msg_t));
    if (msg == NULL) {
        return -1;
    }

    msg->ms  = ms;
    msg->tid = gettid();

    if (mqueue_post(&mqueue, msg, 0) < 0) {
        kfree(msg);
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
