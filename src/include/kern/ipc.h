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
** Descriptions:            �����ͨ�Ż���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-13
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
  ������
*********************************************************************************************************/
/*
 * ����һ���µĻ�����
 */
int mutex_new(mutex_t *mutex);

/*
 * ���ԶԻ��������м���
 */
int mutex_trylock(mutex_t *mutex);

/*
 * �Ի��������м���
 */
int mutex_lock(mutex_t *mutex, uint32_t timeout);

/*
 * �Ի��������н���
 */
int mutex_unlock(mutex_t *mutex);

/*
 * ��ֹ�ȴ�������
 */
int mutex_abort(mutex_t *mutex);

/*
 * ɾ��������
 */
int mutex_free(mutex_t *mutex);

/*
 * �жϻ������Ƿ���Ч
 */
int mutex_valid(mutex_t *mutex);

/*
 * ���û���������Ч��
 */
int mutex_set_valid(mutex_t *mutex, int valid);
/*********************************************************************************************************
  �ź���
*********************************************************************************************************/
/*
 * ����һ���µ��ź���
 */
int sem_new(sem_t *sem, uint32_t count);

/*
 * ���Ի���ź���
 */
int sem_trywait(sem_t *sem);

/*
 * ����ź���
 */
int sem_wait(sem_t *sem, uint32_t timeout);

/*
 * ����һ���ź���
 */
int sem_signal(sem_t *sem);

/*
 * ��ֹ�ȴ��ź���
 */
int sem_abort(sem_t *sem);

/*
 * ɾ���ź���
 */
int sem_free(sem_t *sem);

/*
 * �ж��ź����Ƿ���Ч
 */
int sem_valid(sem_t *sem);

/*
 * �����ź�������Ч��
 */
int sem_set_valid(sem_t *sem, int valid);
/*********************************************************************************************************
  ��Ϣ����
*********************************************************************************************************/
/*
 * ����һ���µ���Ϣ����
 */
int mqueue_new(mqueue_t *mqueue, uint32_t size);

/*
 * ����Ͷ����Ϣ����Ϣ����
 */
int mqueue_trypost(mqueue_t *mqueue, void *msg);

/*
 * Ͷ����Ϣ����Ϣ����
 */
int mqueue_post(mqueue_t *mqueue, void *msg, uint32_t timeout);

/*
 * ���Դ���Ϣ������ȡ����Ϣ
 */
int mqueue_tryfetch(mqueue_t *mqueue, void **msg);

/*
 * ����Ϣ������ȡ����Ϣ
 */
int mqueue_fetch(mqueue_t *mqueue, void **msg, uint32_t timeout);

/*
 * �����Ϣ����
 */
int mqueue_flush(mqueue_t *mqueue);

/*
 * ��ֹ�ȴ���ȡ��Ϣ
 */
int mqueue_abort_fetch(mqueue_t *mqueue);

/*
 * ��ֹ�ȴ�Ͷ����Ϣ
 */
int mqueue_abort_post(mqueue_t *mqueue);

/*
 * ��ֹ�ȴ���Ϣ����
 */
int mqueue_abort(mqueue_t *mqueue);

/*
 * ɾ����Ϣ����
 */
int mqueue_free(mqueue_t *mqueue);

/*
 * �ж���Ϣ�����Ƿ���Ч
 */
int mqueue_valid(mqueue_t *mqueue);

/*
 * ������Ϣ���е���Ч��
 */
int mqueue_set_valid(mqueue_t *mqueue, int valid);

#endif                                                                  /*  IPC_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
