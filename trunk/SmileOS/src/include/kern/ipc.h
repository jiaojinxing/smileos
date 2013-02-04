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
** File name:               ipc.h
** Last modified Date:      2012-3-13
** Last Version:            1.0.0
** Descriptions:            任务间通信机制
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-13
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
#ifndef IPC_H_
#define IPC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "kern/types.h"
/*********************************************************************************************************
** 定义
*********************************************************************************************************/
struct mutex;
struct sem;
struct mqueue;

typedef struct mutex  *     mutex_t;
typedef struct sem    *     sem_t;
typedef struct mqueue *     mqueue_t;
/*********************************************************************************************************
** 互斥量
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           mutex_new
** Descriptions:            创建一个新的互斥量
** input parameters:        NONE
** output parameters:       mutex               互斥量
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_new(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_trylock
** Descriptions:            尝试对互斥量进行加锁
** input parameters:        mutex               互斥量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_trylock(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_lock
** Descriptions:            对互斥量进行加锁
** input parameters:        mutex               互斥量
**                          timeout             超时 TICK 数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_lock(mutex_t *mutex, tick_t timeout);
/*********************************************************************************************************
** Function name:           mutex_unlock
** Descriptions:            对互斥量进行解锁
** input parameters:        mutex               互斥量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_unlock(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_abort
** Descriptions:            终止等待互斥量
** input parameters:        mutex               互斥量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_abort(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_free
** Descriptions:            删除互斥量
** input parameters:        mutex               互斥量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_free(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_valid
** Descriptions:            判断互斥量是否有效
** input parameters:        mutex               互斥量
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
bool_t mutex_valid(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_set_valid
** Descriptions:            设置互斥量的有效性
** input parameters:        mutex               互斥量
**                          valid               有效性
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_set_valid(mutex_t *mutex, bool_t valid);
/*********************************************************************************************************
** 信号量
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           sem_new
** Descriptions:            创建一个新的信号量
** input parameters:        count               信号初始计数
** output parameters:       sem                 信号量
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_new(sem_t *sem, uint32_t count);
/*********************************************************************************************************
** Function name:           sem_trywait
** Descriptions:            尝试获得信号量
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_trywait(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_wait
** Descriptions:            获得信号量
** input parameters:        sem                 信号量
**                          timeout             超时 TICK 数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_wait(sem_t *sem, tick_t timeout);
/*********************************************************************************************************
** Function name:           sem_signal
** Descriptions:            发送一个信号量
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_signal(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_sync
** Descriptions:            如果有任务在等待, 则发送一个信号量
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          0 OR -1
**
** 注意: sem_sync 必须与 sem_wait 的超时版本使用
**
*********************************************************************************************************/
int sem_sync(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_abort
** Descriptions:            终止等待信号量
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_abort(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_free
** Descriptions:            删除信号量
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_free(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_valid
** Descriptions:            判断信号量是否有效
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
bool_t sem_valid(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_set_valid
** Descriptions:            设置信号量的有效性
** input parameters:        sem                 信号量
**                          有效性
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_set_valid(sem_t *sem, bool_t valid);
/*********************************************************************************************************
** 消息队列
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           mqueue_new
** Descriptions:            创建一个新的消息队列
** input parameters:        size                消息队列的大小
** output parameters:       mqueue              消息队列
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_new(mqueue_t *mqueue, uint32_t size);
/*********************************************************************************************************
** Function name:           mqueue_trypost
** Descriptions:            尝试投递消息到消息队列
** input parameters:        mqueue              消息队列
**                          msg                 消息
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_trypost(mqueue_t *mqueue, void *msg);
/*********************************************************************************************************
** Function name:           mqueue_post
** Descriptions:            投递消息到消息队列
** input parameters:        mqueue              消息队列
**                          msg                 消息
**                          timeout             超时 TICK 数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_post(mqueue_t *mqueue, void *msg, tick_t timeout);
/*********************************************************************************************************
** Function name:           mqueue_tryfetch
** Descriptions:            尝试从消息队列里取出消息
** input parameters:        mqueue              消息队列
** output parameters:       msg                 消息
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_tryfetch(mqueue_t *mqueue, void **msg);
/*********************************************************************************************************
** Function name:           mqueue_fetch
** Descriptions:            从消息队列里取出消息
** input parameters:        mqueue              消息队列
**                          timeout             超时 TICK 数
** output parameters:       msg                 消息
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_fetch(mqueue_t *mqueue, void **msg, tick_t timeout);
/*********************************************************************************************************
** Function name:           mqueue_flush
** Descriptions:            清空消息队列
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_flush(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_msg_nr
** Descriptions:            返回消息队列中消息的数目
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          消息的数目 OR -1
*********************************************************************************************************/
int mqueue_msg_nr(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_abort_fetch
** Descriptions:            终止等待读取消息
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort_fetch(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_abort_post
** Descriptions:            终止等待投递消息
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort_post(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_abort
** Descriptions:            终止等待消息队列
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_free
** Descriptions:            删除消息队列
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_free(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_valid
** Descriptions:            判断消息队列是否有效
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
bool_t mqueue_valid(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_set_valid
** Descriptions:            设置消息队列的有效性
** input parameters:        mqueue              消息队列
**                          valid               有效性
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_set_valid(mqueue_t *mqueue, bool_t valid);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  IPC_H_                      */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
