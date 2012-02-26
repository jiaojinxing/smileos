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
** Descriptions:            pthread 线程头文件
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

#include <sys/types.h>
#include <sys/socket.h>

/*
 * Extra structure definitions
 */
struct timeval;
struct timespec;

/*
 * Run-time invariant values
 */
#define PTHREAD_DESTRUCTOR_ITERATIONS   4
#define PTHREAD_KEYS_MAX                256
#define PTHREAD_STACK_MIN               8192
#define PTHREAD_THREADS_MAX             10000 /* actually yet no restriction */

/*
 * Flags for threads and thread attributes.
 */
#define PTHREAD_CREATE_DETACHED         0x01
#define PTHREAD_CREATE_JOINABLE         0x02
#define PTHREAD_SCOPE_SYSTEM            0x03
#define PTHREAD_SCOPE_PROCESS           0x04
#define PTHREAD_INHERIT_SCHED           0x05
#define PTHREAD_EXPLICIT_SCHED          0x06

/*
 * Values for cancellation
 */
#define PTHREAD_CANCEL_ENABLE           0x01
#define PTHREAD_CANCEL_DISABLE          0x02
#define PTHREAD_CANCEL_ASYNCHRONOUS     0x01
#define PTHREAD_CANCEL_DEFERRED         0x02
#define PTHREAD_CANCELED                ((void *)-1)

/*
 * Flags for mutex priority attributes
 */
#define PTHREAD_PRIO_INHERIT            0x00
#define PTHREAD_PRIO_NONE               0x01
#define PTHREAD_PRIO_PROTECT            0x02

/*
 * Flags for read/write lock attributes
 */
#define PTHREAD_PROCESS_PRIVATE         0x00
#define PTHREAD_PROCESS_SHARED          0x01

/*
 * Forward structure definitions.
 * These are mostly opaque to the application.
 */
struct pthread_st;
struct pthread_attr_st;
struct pthread_cond_st;
struct pthread_mutex_st;
struct pthread_rwlock_st;
struct pthread_msgport_st;
struct sched_param;

/*
 * Primitive system data type definitions required by P1003.1c
 */
typedef struct  pthread_st              *pthread_t;
typedef struct  pthread_attr_st         *pthread_attr_t;
typedef int                              pthread_key_t;
typedef int                              pthread_once_t;
typedef int                              pthread_mutexattr_t;
typedef struct  pthread_mutex_st        *pthread_mutex_t;
typedef int                              pthread_condattr_t;
typedef struct  pthread_cond_st         *pthread_cond_t;
typedef int                              pthread_rwlockattr_t;
typedef struct  pthread_rwlock_st       *pthread_rwlock_t;
typedef struct  pthread_msgport_st      *pthread_msgport_t;

/*
 * Once support.
 */
#define PTHREAD_ONCE_INIT 0

/*
 * Mutex static initialization values.
 */
enum pthread_mutextype {
    PTHREAD_MUTEX_DEFAULT = 1,
    PTHREAD_MUTEX_RECURSIVE,
    PTHREAD_MUTEX_NORMAL,
    PTHREAD_MUTEX_ERRORCHECK
};

/*
 * Mutex/CondVar/RWLock static initialization values.
 */
#define PTHREAD_MUTEX_INITIALIZER   (pthread_mutex_t)(NULL)
#define PTHREAD_COND_INITIALIZER    (pthread_cond_t)(NULL)
#define PTHREAD_RWLOCK_INITIALIZER  (pthread_rwlock_t)(NULL)

/*
 * IEEE (``POSIX'') Std 1003.1 Second Edition 1996-07-12
 */

/* thread attribute routines */
extern int       pthread_attr_init(pthread_attr_t *);
extern int       pthread_attr_destroy(pthread_attr_t *);
extern int       pthread_attr_setinheritsched(pthread_attr_t *, int);
extern int       pthread_attr_getinheritsched(const pthread_attr_t *, int *);
extern int       pthread_attr_setschedparam(pthread_attr_t *, const struct sched_param *);
extern int       pthread_attr_getschedparam(const pthread_attr_t *, struct sched_param *);
extern int       pthread_attr_setschedpolicy(pthread_attr_t *, int);
extern int       pthread_attr_getschedpolicy(const pthread_attr_t *, int *);
extern int       pthread_attr_setscope(pthread_attr_t *, int);
extern int       pthread_attr_getscope(const pthread_attr_t *, int *);
extern int       pthread_attr_setstacksize(pthread_attr_t *, size_t);
extern int       pthread_attr_getstacksize(const pthread_attr_t *, size_t *);
extern int       pthread_attr_setstackaddr(pthread_attr_t *, void *);
extern int       pthread_attr_getstackaddr(const pthread_attr_t *, void **);
extern int       pthread_attr_setdetachstate(pthread_attr_t *, int);
extern int       pthread_attr_getdetachstate(const pthread_attr_t *, int *);
extern int       pthread_attr_setguardsize(pthread_attr_t *, int);
extern int       pthread_attr_getguardsize(const pthread_attr_t *, int *);
extern int       pthread_attr_setname_np(pthread_attr_t *, char *);
extern int       pthread_attr_getname_np(const pthread_attr_t *, char **);
extern int       pthread_attr_setprio_np(pthread_attr_t *, int);
extern int       pthread_attr_getprio_np(const pthread_attr_t *, int *);

/* thread routines */
extern int       pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
extern int       __pthread_detach(pthread_t);
#define          pthread_detach(t) __pthread_detach(t)
extern pthread_t pthread_self(void);
extern int       pthread_equal(pthread_t, pthread_t);
extern int       pthread_yield_np(void);
extern void      pthread_exit(void *);
extern int       pthread_join(pthread_t, void **);
extern int       pthread_once(pthread_once_t *, void (*)(void));
//extern int       pthread_sigmask(int, const sigset_t *, sigset_t *);
extern int       pthread_kill(pthread_t, int);

/* concurrency routines */
extern int       pthread_getconcurrency(void);
extern int       pthread_setconcurrency(int);

/* context routines */
extern int       pthread_key_create(pthread_key_t *, void (*)(void *));
extern int       pthread_key_delete(pthread_key_t);
extern int       pthread_setspecific(pthread_key_t, const void *);
extern void     *pthread_getspecific(pthread_key_t);

/* cancel routines */
extern int       pthread_cancel(pthread_t);
extern void      pthread_testcancel(void);
extern int       pthread_setcancelstate(int, int *);
extern int       pthread_setcanceltype(int, int *);

/* scheduler routines */
extern int       pthread_setschedparam(pthread_t, int, const struct sched_param *);
extern int       pthread_getschedparam(pthread_t, int *, struct sched_param *);

/* cleanup routines */
extern void      pthread_cleanup_push(void (*)(void *), void *);
extern void      pthread_cleanup_pop(int);
extern int       pthread_atfork(void (*)(void), void (*)(void), void (*)(void));

/* mutex attribute routines */
extern int       pthread_mutexattr_init(pthread_mutexattr_t *);
extern int       pthread_mutexattr_destroy(pthread_mutexattr_t *);
extern int       pthread_mutexattr_setprioceiling(pthread_mutexattr_t *, int);
extern int       pthread_mutexattr_getprioceiling(pthread_mutexattr_t *, int *);
extern int       pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
extern int       pthread_mutexattr_getprotocol(pthread_mutexattr_t *, int *);
extern int       pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
extern int       pthread_mutexattr_getpshared(pthread_mutexattr_t *, int *);
extern int       pthread_mutexattr_settype(pthread_mutexattr_t *, int);
extern int       pthread_mutexattr_gettype(pthread_mutexattr_t *, int *);

/* mutex routines */
extern int       pthread_mutex_init(pthread_mutex_t *, const pthread_mutexattr_t *);
extern int       pthread_mutex_destroy(pthread_mutex_t *);
extern int       pthread_mutex_setprioceiling(pthread_mutex_t *, int, int *);
extern int       pthread_mutex_getprioceiling(pthread_mutex_t *, int *);
extern int       pthread_mutex_lock(pthread_mutex_t *);
extern int       pthread_mutex_trylock(pthread_mutex_t *);
extern int       pthread_mutex_unlock(pthread_mutex_t *);

/* rwlock attribute routines */
extern int       pthread_rwlockattr_init(pthread_rwlockattr_t *);
extern int       pthread_rwlockattr_destroy(pthread_rwlockattr_t *);
extern int       pthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int);
extern int       pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *, int *);

/* rwlock routines */
extern int       pthread_rwlock_init(pthread_rwlock_t *, const pthread_rwlockattr_t *);
extern int       pthread_rwlock_destroy(pthread_rwlock_t *);
extern int       pthread_rwlock_rdlock(pthread_rwlock_t *);
extern int       pthread_rwlock_tryrdlock(pthread_rwlock_t *);
extern int       pthread_rwlock_wrlock(pthread_rwlock_t *);
extern int       pthread_rwlock_trywrlock(pthread_rwlock_t *);
extern int       pthread_rwlock_unlock(pthread_rwlock_t *);

/* condition attribute routines */
extern int       pthread_condattr_init(pthread_condattr_t *);
extern int       pthread_condattr_destroy(pthread_condattr_t *);
extern int       pthread_condattr_setpshared(pthread_condattr_t *, int);
extern int       pthread_condattr_getpshared(pthread_condattr_t *, int *);

/* condition routines */
extern int       pthread_cond_init(pthread_cond_t *, const pthread_condattr_t *);
extern int       pthread_cond_destroy(pthread_cond_t *);
extern int       pthread_cond_broadcast(pthread_cond_t *);
extern int       pthread_cond_signal(pthread_cond_t *);
extern int       pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
extern int       pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *, const struct timespec *);

/* use vendor poll(2) environment */
#include <poll.h>

/* use vendor readv(2)/writev(2) environment */
#include <sys/uio.h>

/*
 * Replacement Functions (threading aware)
 */
extern pid_t              __pthread_fork(void);
extern unsigned int       __pthread_sleep(unsigned int);
extern int                __pthread_nanosleep(const struct timespec *, struct timespec *);
extern int                __pthread_usleep(unsigned int);
extern int                __pthread_system(const char *);
//extern int                __pthread_sigwait(const sigset_t *, int *);
extern pid_t              __pthread_waitpid(pid_t, int *, int);
extern int                __pthread_connect(int, struct sockaddr *, socklen_t);
extern int                __pthread_accept(int, struct sockaddr *, socklen_t *);
extern int                __pthread_select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
extern int                __pthread_poll(struct pollfd *, nfds_t, int);
extern ssize_t            __pthread_read(int, void *, size_t);
extern ssize_t            __pthread_write(int, const void *, size_t);
extern ssize_t            __pthread_readv(int, const struct iovec *, int);
extern ssize_t            __pthread_writev(int, const struct iovec *, int);
extern ssize_t            __pthread_recv(int, void *, size_t, int);
extern ssize_t            __pthread_send(int, const void *, size_t, int);
extern ssize_t            __pthread_recvfrom(int, void *, size_t, int, struct sockaddr *, socklen_t *);
extern ssize_t            __pthread_sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t);
extern ssize_t            __pthread_pread(int, void *, size_t, off_t);
extern ssize_t            __pthread_pwrite(int, const void *, size_t, off_t);

#endif                                                                  /*  PTHREAD_H_                  */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
