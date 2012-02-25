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
** File name:               pthread.h
** Last modified Date:      2012-2-25
** Last Version:            1.0.0
** Descriptions:            posix 线程接口
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-25
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
#ifndef PTHREAD_H_
#define PTHREAD_H_

typedef void * pthread_attr_t;
typedef void * pthread_t;
typedef int    pthread_once_t;
typedef int    pthread_key_t;
typedef void   pthread_mutexattr_t;
typedef void * pthread_mutex_t;
typedef void   pthread_rwlockattr_t;
typedef void * pthread_rwlock_t;
typedef void   pthread_condattr_t;
typedef void * pthread_cond_t;

struct sched_param {
    int i;
};

#define PTHREAD_COND_INITIALIZER    ((pthread_cond_t)-1)
#define PTHREAD_MUTEX_INITIALIZER   ((pthread_mutex_t)-1)
#define PTHREAD_RWLOCK_INITIALIZER  ((pthread_rwlock_t)-1)

#define PTHREAD_THREADS_MAX         1024

#define PTHREAD_CANCEL_DEFERRED     1
#define PTHREAD_CANCEL_ASYNCHRONOUS 0

#define PTHREAD_CANCEL_ENABLE       1
#define PTHREAD_CANCEL_DISABLE      0

#define PTHREAD_CANCELED            ((void *)-1)

#define PTHREAD_CREATE_JOINABLE     0
#define PTHREAD_CREATE_DETACHED     1

#endif                                                                  /*  PTHREAD_H_                  */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
