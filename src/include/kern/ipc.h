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

struct kern_mutex;
struct kern_sem;
struct kern_mbox;

typedef struct kern_mutex *          kern_mutex_t;
typedef struct kern_sem   *          kern_sem_t;
typedef struct kern_mbox  *          kern_mbox_t;
/*********************************************************************************************************
  ������
*********************************************************************************************************/
/*
 * ����һ���µĻ�����
 */
int kern_mutex_new(kern_mutex_t *mutex);

/*
 * ����������
 */
int kern_mutex_lock(kern_mutex_t *mutex);

/*
 * ����������
 */
int kern_mutex_unlock(kern_mutex_t *mutex);

/*
 * ɾ��������
 */
void kern_mutex_free(kern_mutex_t *mutex);

/*
 * �жϻ������Ƿ���Ч
 */
int kern_mutex_valid(kern_mutex_t *mutex);

/*
 * ���û���������Ч��
 */
void kern_mutex_set_valid(kern_mutex_t *mutex, int valid);
/*********************************************************************************************************
  �ź���
*********************************************************************************************************/
/*
 * ����һ���µ��ź���
 */
int kern_sem_new(kern_sem_t *sem, uint32_t count);

/*
 * �ȴ��ź�
 */
int kern_sem_wait(kern_sem_t *sem, uint32_t timeout);

/*
 * ����һ���ź�
 */
int kern_sem_signal(kern_sem_t *sem);

/*
 * ɾ���ź���
 */
void kern_sem_free(kern_sem_t *sem);

/*
 * �ж��ź����Ƿ���Ч
 */
int kern_sem_valid(kern_sem_t *sem);

/*
 * �����ź�������Ч��
 */
void kern_sem_set_valid(kern_sem_t *sem, int valid);
/*********************************************************************************************************
  ����
*********************************************************************************************************/
/*
 * ����һ���µ�����
 */
int kern_mbox_new(kern_mbox_t *mbox, uint32_t size);
/*
 * ����Ͷ���ʼ�������
 */
int kern_mbox_trypost(kern_mbox_t *mbox, void *msg);

/*
 * Ͷ���ʼ�������
 */
int kern_mbox_post(kern_mbox_t *mbox, void *msg);

/*
 * ���Դ�������ȡ���ʼ�
 */
int kern_mbox_tryfetch(kern_mbox_t *mbox, void **msg);

/*
 * ��������ȡ���ʼ�
 */
int kern_mbox_fetch(kern_mbox_t *mbox, void **msg, uint32_t timeout);

/*
 * ɾ������
 */
void kern_mbox_free(kern_mbox_t *mbox);

/*
 * �ж������Ƿ���Ч
 */
int kern_mbox_valid(kern_mbox_t *mbox);

/*
 * �����������Ч��
 */
void kern_mbox_set_invalid(kern_mbox_t *mbox, int valid);

#endif                                                                  /*  IPC_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
