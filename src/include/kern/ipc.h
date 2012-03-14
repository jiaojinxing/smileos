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

struct kern_mutex;
struct kern_sem;
struct kern_mbox;

typedef struct kern_mutex *          kern_mutex_t;
typedef struct kern_sem   *          kern_sem_t;
typedef struct kern_mbox  *          kern_mbox_t;
/*********************************************************************************************************
  互斥量
*********************************************************************************************************/
/*
 * 创建一个新的互斥量
 */
int kern_mutex_new(kern_mutex_t *mutex);

/*
 * 尝试对互斥量进行加锁
 */
int kern_mutex_trylock(kern_mutex_t *mutex);

/*
 * 对互斥量进行加锁
 */
int kern_mutex_lock(kern_mutex_t *mutex, uint32_t timeout);

/*
 * 对互斥量进行解锁
 */
int kern_mutex_unlock(kern_mutex_t *mutex);

/*
 * 终止等待互斥量
 */
int kern_mutex_abort(kern_mutex_t *mutex);

/*
 * 删除互斥量
 */
int kern_mutex_free(kern_mutex_t *mutex);

/*
 * 判断互斥量是否有效
 */
int kern_mutex_valid(kern_mutex_t *mutex);

/*
 * 设置互斥量的有效性
 */
int kern_mutex_set_valid(kern_mutex_t *mutex, int valid);
/*********************************************************************************************************
  信号量
*********************************************************************************************************/
/*
 * 创建一个新的信号量
 */
int kern_sem_new(kern_sem_t *sem, uint32_t count);

/*
 * 尝试获得信号量
 */
int kern_sem_trywait(kern_sem_t *sem);

/*
 * 获得信号量
 */
int kern_sem_wait(kern_sem_t *sem, uint32_t timeout);

/*
 * 发送一个信号量
 */
int kern_sem_signal(kern_sem_t *sem);

/*
 * 终止等待信号量
 */
int kern_sem_abort(kern_sem_t *sem);

/*
 * 删除信号量
 */
int kern_sem_free(kern_sem_t *sem);

/*
 * 判断信号量是否有效
 */
int kern_sem_valid(kern_sem_t *sem);

/*
 * 设置信号量的有效性
 */
int kern_sem_set_valid(kern_sem_t *sem, int valid);
/*********************************************************************************************************
  邮箱
*********************************************************************************************************/
/*
 * 创建一个新的邮箱
 */
int kern_mbox_new(kern_mbox_t *mbox, uint32_t size);

/*
 * 尝试投递邮件到邮箱
 */
int kern_mbox_trypost(kern_mbox_t *mbox, void *msg);

/*
 * 投递邮件到邮箱
 */
int kern_mbox_post(kern_mbox_t *mbox, void *msg, uint32_t timeout);

/*
 * 尝试从邮箱里取出邮件
 */
int kern_mbox_tryfetch(kern_mbox_t *mbox, void **msg);

/*
 * 从邮箱里取出邮件
 */
int kern_mbox_fetch(kern_mbox_t *mbox, void **msg, uint32_t timeout);

/*
 * 清空邮箱
 */
int kern_mbox_flush(kern_mbox_t *mbox);

/*
 * 终止等待读取邮件
 */
int kern_mbox_abort_fetch(kern_mbox_t *mbox);

/*
 * 终止等待投递邮件
 */
int kern_mbox_abort_post(kern_mbox_t *mbox);

/*
 * 终止等待邮箱
 */
int kern_mbox_abort(kern_mbox_t *mbox);

/*
 * 删除邮箱
 */
int kern_mbox_free(kern_mbox_t *mbox);

/*
 * 判断邮箱是否有效
 */
int kern_mbox_valid(kern_mbox_t *mbox);

/*
 * 设置邮箱的有效性
 */
int kern_mbox_set_valid(kern_mbox_t *mbox, int valid);

#endif                                                                  /*  IPC_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
