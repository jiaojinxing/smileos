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

#include "kern/types.h"

struct mutex;
struct sem;
struct mqueue;

typedef struct mutex  *     mutex_t;
typedef struct sem    *     sem_t;
typedef struct mqueue *     mqueue_t;
/*********************************************************************************************************
  互斥量
*********************************************************************************************************/
/*
 * 创建一个新的互斥量
 */
int mutex_new(mutex_t *mutex);

/*
 * 尝试对互斥量进行加锁
 */
int mutex_trylock(mutex_t *mutex);

/*
 * 对互斥量进行加锁
 */
int mutex_lock(mutex_t *mutex, uint32_t timeout);

/*
 * 对互斥量进行解锁
 */
int mutex_unlock(mutex_t *mutex);

/*
 * 终止等待互斥量
 */
int mutex_abort(mutex_t *mutex);

/*
 * 删除互斥量
 */
int mutex_free(mutex_t *mutex);

/*
 * 判断互斥量是否有效
 */
int mutex_valid(mutex_t *mutex);

/*
 * 设置互斥量的有效性
 */
int mutex_set_valid(mutex_t *mutex, int valid);
/*********************************************************************************************************
  信号量
*********************************************************************************************************/
/*
 * 创建一个新的信号量
 */
int sem_new(sem_t *sem, uint32_t count);

/*
 * 尝试获得信号量
 */
int sem_trywait(sem_t *sem);

/*
 * 获得信号量
 */
int sem_wait(sem_t *sem, uint32_t timeout);

/*
 * 发送一个信号量
 */
int sem_signal(sem_t *sem);

/*
 * 终止等待信号量
 */
int sem_abort(sem_t *sem);

/*
 * 删除信号量
 */
int sem_free(sem_t *sem);

/*
 * 判断信号量是否有效
 */
int sem_valid(sem_t *sem);

/*
 * 设置信号量的有效性
 */
int sem_set_valid(sem_t *sem, int valid);
/*********************************************************************************************************
  消息队列
*********************************************************************************************************/
/*
 * 创建一个新的消息队列
 */
int mqueue_new(mqueue_t *mqueue, uint32_t size);

/*
 * 尝试投递消息到消息队列
 */
int mqueue_trypost(mqueue_t *mqueue, void *msg);

/*
 * 投递消息到消息队列
 */
int mqueue_post(mqueue_t *mqueue, void *msg, uint32_t timeout);

/*
 * 尝试从消息队列里取出消息
 */
int mqueue_tryfetch(mqueue_t *mqueue, void **msg);

/*
 * 从消息队列里取出消息
 */
int mqueue_fetch(mqueue_t *mqueue, void **msg, uint32_t timeout);

/*
 * 清空消息队列
 */
int mqueue_flush(mqueue_t *mqueue);

/*
 * 终止等待读取消息
 */
int mqueue_abort_fetch(mqueue_t *mqueue);

/*
 * 终止等待投递消息
 */
int mqueue_abort_post(mqueue_t *mqueue);

/*
 * 终止等待消息队列
 */
int mqueue_abort(mqueue_t *mqueue);

/*
 * 删除消息队列
 */
int mqueue_free(mqueue_t *mqueue);

/*
 * 判断消息队列是否有效
 */
int mqueue_valid(mqueue_t *mqueue);

/*
 * 设置消息队列的有效性
 */
int mqueue_set_valid(mqueue_t *mqueue, int valid);

#endif                                                                  /*  IPC_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
