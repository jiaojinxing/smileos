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
** File name:               pth_p.h
** Last modified Date:      2012-4-27
** Last Version:            1.0.0
** Descriptions:            GNU/pth 库私有头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-27
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
#ifndef _PTH_P_H_
#define _PTH_P_H_

/* mandatory system headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <time.h>
#include <poll.h>
#include <sys/uio.h>
#include <sys/select.h>

/* library version */
#define _PTH_VERS_C_AS_HEADER_
#include "pth_vers.c"
#undef  _PTH_VERS_C_AS_HEADER_

/* public API headers */
#define _PTH_PRIVATE
#include "pth.h"
#undef _PTH_PRIVATE

/* autoconf defines and macros */
#include "pth_acdef.h"
#include "pth_acmac.h"

/* optional system headers */
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#ifdef HAVE_NET_ERRNO_H
#include <net/errno.h>
#endif
#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

/* dmalloc support */
#ifdef PTH_DMALLOC
#include <dmalloc.h>
#endif

/* OSSP ex support */
#ifdef PTH_EX
#define __EX_NS_USE_CUSTOM__
#include "ex.h"
#endif

/* paths */
#ifdef HAVE_PATHS_H
#include <paths.h>
#endif
#ifdef _PATH_BSHELL
#define PTH_PATH_BINSH _PATH_BSHELL
#else
#define PTH_PATH_BINSH "/bin/sh"
#endif

/* non-blocking flags */
#ifdef  O_NONBLOCK
#define O_NONBLOCKING O_NONBLOCK
#else
#ifdef  O_NDELAY
#define O_NONBLOCKING O_NDELAY
#else
#ifdef  FNDELAY
#define O_NONBLOCKING FNDELAY
#else
#error "No O_NONBLOCK, O_NDELAY or FNDELAY flag available!"
#endif
#endif
#endif

/* fallback definition for fdset_t size */
#if !defined(FD_SETSIZE)
#define FD_SETSIZE 1024
#endif

/* fallback definition for struct timespec */
#ifndef HAVE_STRUCT_TIMESPEC
struct timespec {
    time_t  tv_sec;     /* seconds */
    long    tv_nsec;    /* and nanoseconds */
};
#endif

/* compiler happyness: avoid ``empty compilation unit'' problem */
#define COMPILER_HAPPYNESS(name) \
    int __##name##_unit = 0;
/*********************************************************************************************************
  私有数据类型定义
*********************************************************************************************************/
/*
 * 线程清理
 */
typedef struct pth_cleanup_st pth_cleanup_t;
struct pth_cleanup_st {
    pth_cleanup_t *next;
    void (*func)(void *);
    void *arg;
};

void pth_cleanup_popall(pth_t t, int execute);

/*
 * 机器上下文
 */
typedef struct pth_mctx_st pth_mctx_t;
struct pth_mctx_st {
    unsigned int   sp;
    unsigned int   errno_backup;
};

void pth_mctx_switch(pth_mctx_t *from, pth_mctx_t *to);
int pth_mctx_set(pth_mctx_t *mctx, void (*func)(void), char *sk_addr_lo, char *sk_addr_hi);

/*
 * 线程控制块
 */
#define PTH_TCB_NAMELEN 40

/* thread control block */
struct pth_st {
    /* priority queue handling */
    pth_t          q_next;               /* next thread in pool                         */
    pth_t          q_prev;               /* previous thread in pool                     */
    int            q_prio;               /* (relative) priority of thread when queued   */

    /* standard thread control block ingredients */
    int            prio;                 /* base priority of thread                     */
    char           name[PTH_TCB_NAMELEN];/* name of thread (mainly for debugging)       */
    int            dispatches;           /* total number of thread dispatches           */
    pth_state_t    state;                /* current state indicator for thread          */

    /* timing */
    pth_time_t     spawned;              /* time point at which thread was spawned      */
    pth_time_t     lastran;              /* time point at which thread was last running */
    pth_time_t     running;              /* time range the thread was already running   */

    /* event handling */
    pth_event_t    events;               /* events the tread is waiting for             */

    /* per-thread signal handling */
    sigset_t       sigpending;           /* set    of pending signals                   */
    int            sigpendcnt;           /* number of pending signals                   */

    /* machine context */
    pth_mctx_t     mctx;                 /* last saved machine state of thread          */
    char          *stack;                /* pointer to thread stack                     */
    unsigned int   stacksize;            /* size of thread stack                        */
    long          *stackguard;           /* stack overflow guard                        */
    int            stackloan;            /* stack type                                  */
    void        *(*start_func)(void *);  /* start routine                               */
    void          *start_arg;            /* start argument                              */

    /* thread joining */
    int            joinable;             /* whether thread is joinable                  */
    void          *join_arg;             /* joining argument                            */

    /* per-thread specific storage */
    const void   **data_value;           /* thread specific  values                     */
    int            data_count;           /* number of stored values                     */

    /* cancellation support */
    int            cancelreq;            /* cancellation request is pending             */
    unsigned int   cancelstate;          /* cancellation state of thread                */
    pth_cleanup_t *cleanups;             /* stack of thread cleanup handlers            */

    /* mutex ring */
    pth_ring_t     mutexring;            /* ring of aquired mutex structures            */

#ifdef PTH_EX
    /* per-thread exception handling */
    ex_ctx_t       ex_ctx;               /* exception handling context                  */
#endif
};

pth_t pth_tcb_alloc(unsigned int stacksize, void *stackaddr);
void pth_tcb_free(pth_t t);

/*
 * ERRNO 相关
 */
extern int pth_errno_storage;
extern int pth_errno_flag;

/* enclose errno in a block */
#define pth_shield \
        for ( pth_errno_storage = errno, \
              pth_errno_flag = TRUE; \
              pth_errno_flag; \
              errno = pth_errno_storage, \
              pth_errno_flag = FALSE )

/* return plus setting an errno value */
#if defined(PTH_DEBUG)
#define pth_error(return_val,errno_val) \
        (errno = (errno_val), \
        pth_debug4("return 0x%lx with errno %d(\"%s\")", \
                   (unsigned long)(return_val), (errno), strerror((errno))), \
        (return_val))
#else
#define pth_error(return_val,errno_val) \
        (errno = (errno_val), (return_val))
#endif

/*
 * 线程属性
 */
enum {
    PTH_ATTR_GET,
    PTH_ATTR_SET
};

struct pth_attr_st {
    pth_t        a_tid;
    int          a_prio;
    int          a_dispatches;
    char         a_name[PTH_TCB_NAMELEN];
    int          a_joinable;
    unsigned int a_cancelstate;
    unsigned int a_stacksize;
    char        *a_stackaddr;
};

int pth_attr_ctrl(int cmd, pth_attr_t a, int op, va_list ap);

/*
 * 时间相关
 */
extern pth_time_t pth_time_zero;

#define PTH_TIME_NOW  (pth_time_t *)(0)
#define PTH_TIME_ZERO &pth_time_zero
#define PTH_TIME(sec,usec) { sec, usec }

#define pth_time_equal(t1,t2) \
        (((t1).tv_sec == (t2).tv_sec) && ((t1).tv_usec == (t2).tv_usec))

#define pth_time_set(t1,t2) \
    do { \
        if ((t2) == PTH_TIME_NOW) \
            __gettimeofday((t1)); \
        else { \
            (t1)->tv_sec  = (t2)->tv_sec; \
            (t1)->tv_usec = (t2)->tv_usec; \
        } \
    } while (0)

#define pth_time_add(t1,t2) \
    (t1)->tv_sec  += (t2)->tv_sec; \
    (t1)->tv_usec += (t2)->tv_usec; \
    if ((t1)->tv_usec > 1000000) { \
        (t1)->tv_sec  += 1; \
        (t1)->tv_usec -= 1000000; \
    }

#define pth_time_sub(t1,t2) \
    (t1)->tv_sec  -= (t2)->tv_sec; \
    (t1)->tv_usec -= (t2)->tv_usec; \
    if ((t1)->tv_usec < 0) { \
        (t1)->tv_sec  -= 1; \
        (t1)->tv_usec += 1000000; \
    }

#if defined(HAVE_GETTIMEOFDAY_ARGS1)
#define __gettimeofday(t) gettimeofday(t)
#else
#define __gettimeofday(t) gettimeofday(t, NULL)
#endif

int pth_time_cmp(pth_time_t *t1, pth_time_t *t2);

/*
 * 线程优先级队列
 */
/* thread priority queue */
struct pth_pqueue_st {
    pth_t q_head;
    int   q_num;
};
typedef struct pth_pqueue_st pth_pqueue_t;

extern pth_t        pth_main;       /* the main thread                       */
extern pth_t        pth_sched;      /* the permanent scheduler thread        */
extern pth_t        pth_current;    /* the currently running thread          */
extern pth_pqueue_t pth_NQ;         /* queue of new threads                  */
extern pth_pqueue_t pth_RQ;         /* queue of threads ready to run         */
extern pth_pqueue_t pth_WQ;         /* queue of threads waiting for an event */
extern pth_pqueue_t pth_SQ;         /* queue of suspended threads            */
extern pth_pqueue_t pth_DQ;         /* queue of terminated threads           */
extern int          pth_favournew;  /* favour new threads on startup         */
extern float        pth_loadval;    /* average scheduler load value          */

#define pth_pqueue_elements(q) \
    ((q) == NULL ? (-1) : (q)->q_num)

#define pth_pqueue_head(q) \
    ((q) == NULL ? NULL : (q)->q_head)

#define pth_pqueue_favorite_prio(q) \
    ((q)->q_head != NULL ? (q)->q_head->q_prio + 1 : PTH_PRIO_MAX)

int pth_pqueue_contains(pth_pqueue_t *q, pth_t t);
void pth_pqueue_delete(pth_pqueue_t *q, pth_t t);
void pth_pqueue_insert(pth_pqueue_t *q, int prio, pth_t t);
pth_t pth_pqueue_walk(pth_pqueue_t *q, pth_t t, int direction);
void pth_pqueue_init(pth_pqueue_t *q);
pth_t pth_pqueue_delmax(pth_pqueue_t *q);
pth_t pth_pqueue_tail(pth_pqueue_t *q);
void pth_pqueue_increase(pth_pqueue_t *q);
int pth_pqueue_favorite(pth_pqueue_t *q, pth_t t);

/*
 * 事件
 */
/* pre-declare type of function event callback
   (mainly to workaround va_arg(3) problems below) */
typedef int (*pth_event_func_t)(void *);

/* event structure */
struct pth_event_st {
    struct pth_event_st *ev_next;
    struct pth_event_st *ev_prev;
    pth_status_t ev_status;
    int ev_type;
    int ev_goal;
    union {
        struct { int fd; }                                          FD;
        struct { int *n; int nfd; fd_set *rfds, *wfds, *efds; }     SELECT;
        struct { sigset_t *sigs; int *sig; }                        SIGS;
        struct { pth_time_t tv; }                                   TIME;
        struct { pth_msgport_t mp; }                                MSG;
        struct { pth_mutex_t *mutex; }                              MUTEX;
        struct { pth_cond_t *cond; }                                COND;
        struct { pth_t tid; }                                       TID;
        struct { pth_event_func_t func; void *arg; pth_time_t tv; } FUNC;
    } ev_args;
};

/*
 * 消息端口
 */
/* message port structure */
struct pth_msgport_st {
    pth_ringnode_t mp_node;  /* maintainance node handle */
    const char    *mp_name;  /* optional name of message port */
    pth_t          mp_tid;   /* corresponding thread */
    pth_ring_t     mp_queue; /* queue of messages pending on port */
};

/*
 * 环形队列
 */
/* return number of nodes in ring; O(1) */
#define pth_ring_elements(r) \
    ((r) == NULL ? (-1) : (r)->r_nodes)

/* return first node in ring; O(1) */
#define pth_ring_first(r) \
    ((r) == NULL ? NULL : (r)->r_hook)

/* return last node in ring; O(1) */
#define pth_ring_last(r) \
    ((r) == NULL ? NULL : ((r)->r_hook == NULL ? NULL : (r)->r_hook->rn_prev))

/* walk to next node in ring; O(1) */
#define pth_ring_next(r, rn) \
    (((r) == NULL || (rn) == NULL) ? NULL : ((rn)->rn_next == (r)->r_hook ? NULL : (rn)->rn_next))

/* walk to previous node in ring; O(1) */
#define pth_ring_prev(r, rn) \
    (((r) == NULL || (rn) == NULL) ? NULL : ((rn)->rn_prev == (r)->r_hook->rn_prev ? NULL : (rn)->rn_prev))

/* insert node into ring; O(1) */
#define pth_ring_insert(r, rn) \
    pth_ring_append((r), (rn))

void pth_ring_init(pth_ring_t *r);
pth_ringnode_t *pth_ring_pop(pth_ring_t *r);
void pth_ring_append(pth_ring_t *r, pth_ringnode_t *rn);
void pth_ring_delete(pth_ring_t *r, pth_ringnode_t *rn);
/*********************************************************************************************************
  其它私有 API 定义
*********************************************************************************************************/
char *pth_util_cpystrn(char *dst, const char *src, size_t dst_size);
int pth_util_fd_valid(int fd);
void pth_util_fds_merge(int nfd,
        fd_set *ifds1, fd_set *ofds1,
        fd_set *ifds2, fd_set *ofds2,
        fd_set *ifds3, fd_set *ofds3);
int pth_util_fds_test(int nfd,
        fd_set *ifds1, fd_set *ofds1,
        fd_set *ifds2, fd_set *ofds2,
        fd_set *ifds3, fd_set *ofds3);
int pth_util_fds_select(int nfd,
        fd_set *ifds1, fd_set *ofds1,
        fd_set *ifds2, fd_set *ofds2,
        fd_set *ifds3, fd_set *ofds3);
#define pth_util_min(a,b) \
        ((a) > (b) ? (b) : (a))

int
pth_snprintf(
    char *str,
    size_t count,
    const char *fmt,
    ...);
int
pth_vsnprintf(
    char *str,
    size_t count,
    const char *fmt,
    va_list args);

void pth_thread_cleanup(pth_t thread);

void pth_key_destroydata(pth_t t);

void pth_mutex_releaseall(pth_t thread);

void pth_implicit_init(void);

void pth_syscall_init(void);
void pth_syscall_kill(void);

int pth_scheduler_init(void);
void pth_scheduler_kill(void);
void pth_scheduler_drop(void);
void *pth_scheduler(void *dummy);
void pth_sched_eventmanager(pth_time_t *now, int dopoll);

int pth_select(int nfds, fd_set *rfds, fd_set *wfds,
               fd_set *efds, struct timeval *timeout);
int pth_select_ev(int nfd, fd_set *rfds, fd_set *wfds,
                  fd_set *efds, struct timeval *timeout, pth_event_t ev_extra);

int pth_poll(struct pollfd *pfd, nfds_t nfd, int timeout);
int pth_poll_ev(struct pollfd *pfd, nfds_t nfd, int timeout, pth_event_t ev_extra);

int pth_connect(int s, const struct sockaddr *addr, socklen_t addrlen);
int pth_connect_ev(int s, const struct sockaddr *addr, socklen_t addrlen, pth_event_t ev_extra);

int pth_accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int pth_accept_ev(int s, struct sockaddr *addr, socklen_t *addrlen, pth_event_t ev_extra);

ssize_t pth_read(int fd, void *buf, size_t nbytes);
ssize_t pth_read_ev(int fd, void *buf, size_t nbytes, pth_event_t ev_extra);

ssize_t pth_write(int fd, const void *buf, size_t nbytes);
ssize_t pth_write_ev(int fd, const void *buf, size_t nbytes, pth_event_t ev_extra);

ssize_t pth_send(int s, const void *buf, size_t len, int flags);
ssize_t pth_send_ev(int s, const void *buf, size_t len, int flags, pth_event_t ev);

ssize_t pth_sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen);
ssize_t pth_sendto_ev(int fd, const void *buf, size_t nbytes, int flags, const struct sockaddr *to, socklen_t tolen, pth_event_t ev_extra);

ssize_t pth_recv(int s, void *buf, size_t len, int flags);
ssize_t pth_recv_ev(int s, void *buf, size_t len, int flags, pth_event_t ev);

ssize_t pth_recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
ssize_t pth_recvfrom_ev(int fd, void *buf, size_t nbytes, int flags, struct sockaddr *from, socklen_t *fromlen, pth_event_t ev_extra);

ssize_t pth_readv(int fd, const struct iovec *iov, int iovcnt);
ssize_t pth_readv_ev(int fd, const struct iovec *iov, int iovcnt, pth_event_t ev_extra);

ssize_t pth_writev(int fd, const struct iovec *iov, int iovcnt);
ssize_t pth_writev_ev(int fd, const struct iovec *iov, int iovcnt, pth_event_t ev_extra);

ssize_t pth_writev_iov_bytes(const struct iovec *iov, int iovcnt);
void pth_writev_iov_advance(const struct iovec *riov, int riovcnt, size_t advance,
                                   struct iovec **liov, int *liovcnt,
                                   struct iovec *tiov, int tiovcnt);

ssize_t pth_readv_faked(int fd, const struct iovec *iov, int iovcnt);
ssize_t pth_writev_faked(int fd, const struct iovec *iov, int iovcnt);

unsigned int pth_sleep(unsigned int sec);
int pth_nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
int pth_usleep(unsigned int usec);

ssize_t pth_pread(int fd, void *buf, size_t nbytes, off_t offset);
ssize_t pth_pwrite(int fd, const void *buf, size_t nbytes, off_t offset);

void pth_dumpstate(FILE *fp);
void pth_dumpqueue(FILE *fp, const char *qn, pth_pqueue_t *q);
void pth_debug(const char *file, int line, int argc, const char *fmt, ...);
#ifndef PTH_DEBUG
#define pth_debug1(a1)                     /* NOP */
#define pth_debug2(a1, a2)                 /* NOP */
#define pth_debug3(a1, a2, a3)             /* NOP */
#define pth_debug4(a1, a2, a3, a4)         /* NOP */
#define pth_debug5(a1, a2, a3, a4, a5)     /* NOP */
#define pth_debug6(a1, a2, a3, a4, a5, a6) /* NOP */
#else
#define pth_debug1(a1)                     pth_debug(__FILE__, __LINE__, 1, a1)
#define pth_debug2(a1, a2)                 pth_debug(__FILE__, __LINE__, 2, a1, a2)
#define pth_debug3(a1, a2, a3)             pth_debug(__FILE__, __LINE__, 3, a1, a2, a3)
#define pth_debug4(a1, a2, a3, a4)         pth_debug(__FILE__, __LINE__, 4, a1, a2, a3, a4)
#define pth_debug5(a1, a2, a3, a4, a5)     pth_debug(__FILE__, __LINE__, 5, a1, a2, a3, a4, a5)
#define pth_debug6(a1, a2, a3, a4, a5, a6) pth_debug(__FILE__, __LINE__, 6, a1, a2, a3, a4, a5, a6)
#endif /* PTH_DEBUG */

#define  pth_sc(func) syscall_##func
unsigned pth_sc(sleep)(unsigned int seconds);
_ssize_t pth_sc(write)(int fd, const void *buf, size_t nbytes);
int      pth_sc(accept)(int s, struct sockaddr *addr, socklen_t *addrlen);
int      pth_sc(connect)(int s, const struct sockaddr *name, socklen_t namelen);
_ssize_t pth_sc(read)(int fd, void *buf, size_t nbytes);
int      pth_sc(recvfrom)(int s, void *mem, size_t len, int flags,
                          struct sockaddr *from, socklen_t *fromlen);
int      pth_sc(select)(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
                        struct timeval *timeout);
int      pth_sc(sendto)(int s, const void *dataptr, size_t size, int flags,
                        const struct sockaddr *to, socklen_t tolen);

#endif                                                                  /*  _PTH_P_H_                   */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
