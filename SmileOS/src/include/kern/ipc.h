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

#ifdef __cplusplus
extern "C" {
#endif

#include "kern/types.h"
/*********************************************************************************************************
** ����
*********************************************************************************************************/
struct mutex;
struct sem;
struct mqueue;

typedef struct mutex  *     mutex_t;
typedef struct sem    *     sem_t;
typedef struct mqueue *     mqueue_t;
/*********************************************************************************************************
** ������
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           mutex_new
** Descriptions:            ����һ���µĻ�����
** input parameters:        NONE
** output parameters:       mutex               ������
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_new(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_trylock
** Descriptions:            ���ԶԻ��������м���
** input parameters:        mutex               ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_trylock(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_lock
** Descriptions:            �Ի��������м���
** input parameters:        mutex               ������
**                          timeout             ��ʱ TICK ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_lock(mutex_t *mutex, uint32_t timeout);
/*********************************************************************************************************
** Function name:           mutex_unlock
** Descriptions:            �Ի��������н���
** input parameters:        mutex               ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_unlock(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_abort
** Descriptions:            ��ֹ�ȴ�������
** input parameters:        mutex               ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_abort(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_free
** Descriptions:            ɾ��������
** input parameters:        mutex               ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_free(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_valid
** Descriptions:            �жϻ������Ƿ���Ч
** input parameters:        mutex               ������
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int mutex_valid(mutex_t *mutex);
/*********************************************************************************************************
** Function name:           mutex_set_valid
** Descriptions:            ���û���������Ч��
** input parameters:        mutex               ������
**                          valid               <= 0 : ��Ч, > 0 : ��Ч
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_set_valid(mutex_t *mutex, int valid);
/*********************************************************************************************************
** �ź���
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           sem_new
** Descriptions:            ����һ���µ��ź���
** input parameters:        count               �źų�ʼ����
** output parameters:       sem                 �ź���
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_new(sem_t *sem, uint32_t count);
/*********************************************************************************************************
** Function name:           sem_trywait
** Descriptions:            ���Ի���ź���
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_trywait(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_wait
** Descriptions:            ����ź���
** input parameters:        sem                 �ź���
**                          timeout             ��ʱ TICK ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_wait(sem_t *sem, uint32_t timeout);
/*********************************************************************************************************
** Function name:           sem_signal
** Descriptions:            ����һ���ź���
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_signal(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_sync
** Descriptions:            ����������ڵȴ�, ����һ���ź���
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          0 OR -1
**
** ע��: sem_sync ������ sem_wait �ĳ�ʱ�汾ʹ��
**
*********************************************************************************************************/
int sem_sync(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_abort
** Descriptions:            ��ֹ�ȴ��ź���
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_abort(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_free
** Descriptions:            ɾ���ź���
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_free(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_valid
** Descriptions:            �ж��ź����Ƿ���Ч
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int sem_valid(sem_t *sem);
/*********************************************************************************************************
** Function name:           sem_set_valid
** Descriptions:            �����ź�������Ч��
** input parameters:        sem                 �ź���
**                          valid               <= 0 : ��Ч, > 0 : ��Ч
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_set_valid(sem_t *sem, int valid);
/*********************************************************************************************************
** ��Ϣ����
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           mqueue_new
** Descriptions:            ����һ���µ���Ϣ����
** input parameters:        size                ��Ϣ���еĴ�С
** output parameters:       mqueue              ��Ϣ����
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_new(mqueue_t *mqueue, uint32_t size);
/*********************************************************************************************************
** Function name:           mqueue_trypost
** Descriptions:            ����Ͷ����Ϣ����Ϣ����
** input parameters:        mqueue              ��Ϣ����
**                          msg                 ��Ϣ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_trypost(mqueue_t *mqueue, void *msg);
/*********************************************************************************************************
** Function name:           mqueue_post
** Descriptions:            Ͷ����Ϣ����Ϣ����
** input parameters:        mqueue              ��Ϣ����
**                          msg                 ��Ϣ
**                          timeout             ��ʱ TICK ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_post(mqueue_t *mqueue, void *msg, uint32_t timeout);
/*********************************************************************************************************
** Function name:           mqueue_tryfetch
** Descriptions:            ���Դ���Ϣ������ȡ����Ϣ
** input parameters:        mqueue              ��Ϣ����
** output parameters:       msg                 ��Ϣ
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_tryfetch(mqueue_t *mqueue, void **msg);
/*********************************************************************************************************
** Function name:           mqueue_fetch
** Descriptions:            ����Ϣ������ȡ����Ϣ
** input parameters:        mqueue              ��Ϣ����
**                          timeout             ��ʱ TICK ��
** output parameters:       msg                 ��Ϣ
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_fetch(mqueue_t *mqueue, void **msg, uint32_t timeout);
/*********************************************************************************************************
** Function name:           mqueue_flush
** Descriptions:            �����Ϣ����
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_flush(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_msg_nr
** Descriptions:            ������Ϣ��������Ϣ����Ŀ
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          ��Ϣ����Ŀ OR -1
*********************************************************************************************************/
int mqueue_msg_nr(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_abort_fetch
** Descriptions:            ��ֹ�ȴ���ȡ��Ϣ
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort_fetch(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_abort_post
** Descriptions:            ��ֹ�ȴ�Ͷ����Ϣ
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort_post(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_abort
** Descriptions:            ��ֹ�ȴ���Ϣ����
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_free
** Descriptions:            ɾ����Ϣ����
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_free(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_valid
** Descriptions:            �ж���Ϣ�����Ƿ���Ч
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int mqueue_valid(mqueue_t *mqueue);
/*********************************************************************************************************
** Function name:           mqueue_set_valid
** Descriptions:            ������Ϣ���е���Ч��
** input parameters:        mqueue              ��Ϣ����
**                          valid               <= 0 : ��Ч, > 0 : ��Ч
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_set_valid(mqueue_t *mqueue, int valid);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  IPC_H_                      */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
