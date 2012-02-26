/*
**  GNU Pth - The GNU Portable Threads
**  Copyright (c) 1999-2006 Ralf S. Engelschall <rse@engelschall.com>
**
**  This file is part of GNU Pth, a non-preemptive thread scheduling
**  library which can be found at http://www.gnu.org/software/pth/.
**
**  This library is free software; you can redistribute it and/or
**  modify it under the terms of the GNU Lesser General Public
**  License as published by the Free Software Foundation; either
**  version 2.1 of the License, or (at your option) any later version.
**
**  This library is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**  Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License along with this library; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
**  USA, or contact Ralf S. Engelschall <rse@engelschall.com>.
**
**  pth.h: Pth public API definitions
*/
                             /* ``What you see is all you get.''
                                          -- Brian Kernighan      */
#ifndef _PTH_H_
#define _PTH_H_

    /* the library version */
#ifndef PTH_VERSION_STR
#define PTH_VERSION_STR "@PTH_VERSION_STR@"
#endif
#ifndef PTH_VERSION_HEX
#define PTH_VERSION_HEX 209
#endif
#ifndef PTH_VERSION
#define PTH_VERSION PTH_VERSION_HEX
#endif

#define PTH_FDSETSIZE   128
#define _PTHREAD_PRIVATE
#define intern
//#define PTH_DEBUG

    /* essential headers */
#include <sys/types.h>     /* for ssize_t, off_t  */
#include <time.h>          /* for struct timespec */
#include <sys/time.h>      /* for struct timeval  */
#include <sys/socket.h>    /* for sockaddr        */
#include <sys/signal.h>    /* for sigset_t        */

    /* fallbacks for essential typedefs */
#ifndef _PTHREAD_PRIVATE
#endif /* !_PTHREAD_PRIVATE */

    /* extra structure definitions */
struct timeval;
struct timespec;

    /* essential values */
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#ifndef NUL
#define NUL '\0'
#endif
#ifndef NULL
#define NULL (void *)0
#endif

    /* bitmask generation */
#define _BIT(n) (1<<(n))

    /* C++ support */
#ifdef __cplusplus
#define BEGIN_DECLARATION extern "C" {
#define END_DECLARATION   }
#else
#define BEGIN_DECLARATION /*nop*/
#define END_DECLARATION   /*nop*/
#endif

    /* check if the user requests a bigger FD_SETSIZE than we can handle */
#if defined(FD_SETSIZE)
#if FD_SETSIZE > PTH_FDSETSIZE
#error "FD_SETSIZE is larger than what GNU Pth can handle."
#endif
#endif

BEGIN_DECLARATION

    /* some global constants */
#define PTH_KEY_MAX                  256
#define PTH_ATFORK_MAX               128
#define PTH_DESTRUCTOR_ITERATIONS    4

    /* system call mapping support type (soft variant can be overridden) */
#define PTH_SYSCALL_HARD 1
#ifndef PTH_SYSCALL_SOFT
#define PTH_SYSCALL_SOFT 0
#endif

    /* queries for pth_ctrl() */
#define PTH_CTRL_GETAVLOAD            _BIT(1)
#define PTH_CTRL_GETPRIO              _BIT(2)
#define PTH_CTRL_GETNAME              _BIT(3)
#define PTH_CTRL_GETTHREADS_NEW       _BIT(4)
#define PTH_CTRL_GETTHREADS_READY     _BIT(5)
#define PTH_CTRL_GETTHREADS_RUNNING   _BIT(6)
#define PTH_CTRL_GETTHREADS_WAITING   _BIT(7)
#define PTH_CTRL_GETTHREADS_SUSPENDED _BIT(8)
#define PTH_CTRL_GETTHREADS_DEAD      _BIT(9)
#define PTH_CTRL_GETTHREADS           (PTH_CTRL_GETTHREADS_NEW|\
                                       PTH_CTRL_GETTHREADS_READY|\
                                       PTH_CTRL_GETTHREADS_RUNNING|\
                                       PTH_CTRL_GETTHREADS_WAITING|\
                                       PTH_CTRL_GETTHREADS_SUSPENDED|\
                                       PTH_CTRL_GETTHREADS_DEAD)
#define PTH_CTRL_DUMPSTATE            _BIT(10)
#define PTH_CTRL_FAVOURNEW            _BIT(11)

    /* the time value structure */
typedef struct timeval pth_time_t;

    /* the unique thread id/handle */
typedef struct pth_st *pth_t;
struct pth_st;

    /* thread states */
typedef enum pth_state_en {
    PTH_STATE_SCHEDULER = 0,         /* the special scheduler thread only       */
    PTH_STATE_NEW,                   /* spawned, but still not dispatched       */
    PTH_STATE_READY,                 /* ready, waiting to be dispatched         */
    PTH_STATE_WAITING,               /* suspended, waiting until event occurred */
    PTH_STATE_DEAD                   /* terminated, waiting to be joined        */
} pth_state_t;

    /* thread priority values */
#define PTH_PRIO_MAX                 +5
#define PTH_PRIO_STD                  0
#define PTH_PRIO_MIN                 -5

    /* the thread attribute structure */
typedef struct pth_attr_st *pth_attr_t;
struct pth_attr_st;

    /* attribute set/get commands for pth_attr_{get,set}() */
enum {
    PTH_ATTR_PRIO,           /* RW [int]               priority of thread                */
    PTH_ATTR_NAME,           /* RW [char *]            name of thread                    */
    PTH_ATTR_JOINABLE,       /* RW [int]               thread detachment type            */
    PTH_ATTR_CANCEL_STATE,   /* RW [unsigned int]      thread cancellation state         */
    PTH_ATTR_STACK_SIZE,     /* RW [unsigned int]      stack size                        */
    PTH_ATTR_STACK_ADDR,     /* RW [char *]            stack lower address               */
    PTH_ATTR_DISPATCHES,     /* RO [int]               total number of thread dispatches */
    PTH_ATTR_TIME_SPAWN,     /* RO [pth_time_t]        time thread was spawned           */
    PTH_ATTR_TIME_LAST,      /* RO [pth_time_t]        time thread was last dispatched   */
    PTH_ATTR_TIME_RAN,       /* RO [pth_time_t]        time thread was running           */
    PTH_ATTR_START_FUNC,     /* RO [void *(*)(void *)] thread start function             */
    PTH_ATTR_START_ARG,      /* RO [void *]            thread start argument             */
    PTH_ATTR_STATE,          /* RO [pth_state_t]       scheduling state                  */
    PTH_ATTR_EVENTS,         /* RO [pth_event_t]       events the thread is waiting for  */
    PTH_ATTR_BOUND           /* RO [int]               whether object is bound to thread */
};

    /* default thread attribute */
#define PTH_ATTR_DEFAULT (pth_attr_t)(0)

    /* the event structure */
typedef struct pth_event_st *pth_event_t;
struct pth_event_st;

    /* event subject classes */
#define PTH_EVENT_FD                 _BIT(1)
#define PTH_EVENT_SELECT             _BIT(2)
#define PTH_EVENT_SIGS               _BIT(3)
#define PTH_EVENT_TIME               _BIT(4)
#define PTH_EVENT_MSG                _BIT(5)
#define PTH_EVENT_MUTEX              _BIT(6)
#define PTH_EVENT_COND               _BIT(7)
#define PTH_EVENT_TID                _BIT(8)
#define PTH_EVENT_FUNC               _BIT(9)

    /* event occurange restrictions */
#define PTH_UNTIL_OCCURRED           _BIT(11)
#define PTH_UNTIL_FD_READABLE        _BIT(12)
#define PTH_UNTIL_FD_WRITEABLE       _BIT(13)
#define PTH_UNTIL_FD_EXCEPTION       _BIT(14)
#define PTH_UNTIL_TID_NEW            _BIT(15)
#define PTH_UNTIL_TID_READY          _BIT(16)
#define PTH_UNTIL_TID_WAITING        _BIT(17)
#define PTH_UNTIL_TID_DEAD           _BIT(18)

    /* event structure handling modes */
#define PTH_MODE_REUSE               _BIT(20)
#define PTH_MODE_CHAIN               _BIT(21)
#define PTH_MODE_STATIC              _BIT(22)

    /* event deallocation types */
enum { PTH_FREE_THIS, PTH_FREE_ALL };

    /* event walking directions */
#define PTH_WALK_NEXT                _BIT(1)
#define PTH_WALK_PREV                _BIT(2)

    /* event status codes */
typedef enum {
    PTH_STATUS_PENDING,
    PTH_STATUS_OCCURRED,
    PTH_STATUS_FAILED
} pth_status_t;

    /* the key type and init value */
typedef int pth_key_t;
#define PTH_KEY_INIT (-1)

    /* the once structure and init value */
typedef int pth_once_t;
#define PTH_ONCE_INIT FALSE

    /* general ring structure */
typedef struct pth_ringnode_st pth_ringnode_t;
struct pth_ringnode_st {
    pth_ringnode_t *rn_next;
    pth_ringnode_t *rn_prev;
};
typedef struct pth_ring_st pth_ring_t;
struct pth_ring_st {
    pth_ringnode_t *r_hook;
    unsigned int    r_nodes;
};
#define PTH_RING_INIT { NULL }

    /* cancellation values */
#define PTH_CANCEL_ENABLE            _BIT(0)
#define PTH_CANCEL_DISABLE           _BIT(1)
#define PTH_CANCEL_ASYNCHRONOUS      _BIT(2)
#define PTH_CANCEL_DEFERRED          _BIT(3)
#define PTH_CANCEL_DEFAULT           (PTH_CANCEL_ENABLE|PTH_CANCEL_DEFERRED)
#define PTH_CANCELED                 ((void *)-1)

   /* mutex values */
#define PTH_MUTEX_INITIALIZED        _BIT(0)
#define PTH_MUTEX_LOCKED             _BIT(1)
#define PTH_MUTEX_INIT               { {NULL, NULL}, PTH_MUTEX_INITIALIZED, NULL, 0 }

   /* read-write lock values */
enum { PTH_RWLOCK_RD, PTH_RWLOCK_RW };
#define PTH_RWLOCK_INITIALIZED       _BIT(0)
#define PTH_RWLOCK_INIT              { PTH_RWLOCK_INITIALIZED, PTH_RWLOCK_RD, 0, \
                                       PTH_MUTEX_INIT, PTH_MUTEX_INIT }

   /* condition variable values */
#define PTH_COND_INITIALIZED         _BIT(0)
#define PTH_COND_SIGNALED            _BIT(1)
#define PTH_COND_BROADCAST           _BIT(2)
#define PTH_COND_HANDLED             _BIT(3)
#define PTH_COND_INIT                { PTH_COND_INITIALIZED, 0 }

   /* barrier variable values */
#define PTH_BARRIER_INITIALIZED      _BIT(0)
#define PTH_BARRIER_INIT(threshold)  { PTH_BARRIER_INITIALIZED, \
                                       (threshold), (threshold), FALSE, \
                                       PTH_COND_INIT, PTH_MUTEX_INIT }
#define PTH_BARRIER_HEADLIGHT        (-1)
#define PTH_BARRIER_TAILLIGHT        (-2)

    /* the message port structure */
typedef struct pth_msgport_st *pth_msgport_t;
struct pth_msgport_st;

    /* the message structure */
typedef struct pth_message_st pth_message_t;
struct pth_message_st { /* not hidden to allow inclusion */
    pth_ringnode_t m_node;
    pth_msgport_t  m_replyport;
    unsigned int   m_size;
    void          *m_data;
};

    /* the mutex structure */
typedef struct pth_mutex_st pth_mutex_t;
struct pth_mutex_st { /* not hidden to avoid destructor */
    pth_ringnode_t mx_node;
    int            mx_state;
    pth_t          mx_owner;
    unsigned long  mx_count;
};

    /* the read-write lock structure */
typedef struct pth_rwlock_st pth_rwlock_t;
struct pth_rwlock_st { /* not hidden to avoid destructor */
    int            rw_state;
    unsigned int   rw_mode;
    unsigned long  rw_readers;
    pth_mutex_t    rw_mutex_rd;
    pth_mutex_t    rw_mutex_rw;
};

    /* the condition variable structure */
typedef struct pth_cond_st pth_cond_t;
struct pth_cond_st { /* not hidden to avoid destructor */
    unsigned long cn_state;
    unsigned int  cn_waiters;
};

    /* the barrier variable structure */
typedef struct pth_barrier_st pth_barrier_t;
struct pth_barrier_st { /* not hidden to avoid destructor */
    unsigned long br_state;
    int           br_threshold;
    int           br_count;
    int           br_cycle;
    pth_cond_t    br_cond;
    pth_mutex_t   br_mutex;
};

    /* the user-space context structure */
typedef struct pth_uctx_st *pth_uctx_t;
struct pth_uctx_st;

    /* filedescriptor blocking modes */
enum {
    PTH_FDMODE_ERROR = -1,
    PTH_FDMODE_POLL  =  0,
    PTH_FDMODE_BLOCK,
    PTH_FDMODE_NONBLOCK
};

    /* optionally fake poll(2) data structure and options */
#ifndef _PTHREAD_PRIVATE
#define PTH_FAKE_POLL PTH_FAKE_POLL
#if !(PTH_FAKE_POLL)
/* use vendor poll(2) environment */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_set
#endif
#include <poll.h>
#ifdef _XOPEN_SOURCE_set
#undef _XOPEN_SOURCE_set
#undef _XOPEN_SOURCE
#endif
#ifndef POLLRDNORM
#define POLLRDNORM POLLIN
#endif
#ifndef POLLRDBAND
#define POLLRDBAND POLLIN
#endif
#ifndef POLLWRNORM
#define POLLWRNORM POLLOUT
#endif
#ifndef POLLWRBAND
#define POLLWRBAND POLLOUT
#endif
#ifndef INFTIM
#define INFTIM (-1)
#endif
#else
/* fake a poll(2) environment */
#define POLLIN      0x0001      /* any readable data available   */
#define POLLPRI     0x0002      /* OOB/Urgent readable data      */
#define POLLOUT     0x0004      /* file descriptor is writeable  */
#define POLLERR     0x0008      /* some poll error occurred      */
#define POLLHUP     0x0010      /* file descriptor was "hung up" */
#define POLLNVAL    0x0020      /* requested events "invalid"    */
#define POLLRDNORM  POLLIN
#define POLLRDBAND  POLLIN
#define POLLWRNORM  POLLOUT
#define POLLWRBAND  POLLOUT
#ifndef INFTIM
#define INFTIM      (-1)        /* poll infinite */
#endif
struct pollfd {
    int fd;                     /* which file descriptor to poll */
    short events;               /* events we are interested in   */
    short revents;              /* events found on return        */
};
#endif
#endif /* !_PTHREAD_PRIVATE */

    /* optionally fake readv(2)/writev(2) data structure and options */
#ifndef _PTHREAD_PRIVATE
#define PTH_FAKE_RWV @PTH_FAKE_RWV@
#if !(PTH_FAKE_RWV)
/* use vendor readv(2)/writev(2) environment */
#include <sys/uio.h>
#ifndef UIO_MAXIOV
#define UIO_MAXIOV 1024
#endif
#else
/* fake a readv(2)/writev(2) environment */
struct iovec {
    void  *iov_base;  /* memory base address */
    size_t iov_len;   /* memory chunk length */
};
#ifndef UIO_MAXIOV
#define UIO_MAXIOV 1024
#endif
#endif
#endif /* !_PTHREAD_PRIVATE */

    /* extension support */
#define PTH_EXT_SFIO 0

    /* Sfio extension support */
#if 0
#include <sfio.h>
#else
typedef void *Sfdisc_t;
#endif

    /* global functions */
extern int            pth_init(void);
extern int            pth_kill(void);
extern long           pth_ctrl(unsigned long, ...);
extern long           pth_version(void);

    /* thread attribute functions */
extern pth_attr_t     pth_attr_of(pth_t);
extern pth_attr_t     pth_attr_new(void);
extern int            pth_attr_init(pth_attr_t);
extern int            pth_attr_set(pth_attr_t, int, ...);
extern int            pth_attr_get(pth_attr_t, int, ...);
extern int            pth_attr_destroy(pth_attr_t);

    /* thread functions */
extern pth_t          pth_spawn(pth_attr_t, void *(*)(void *), void *);
extern int            pth_once(pth_once_t *, void (*)(void *), void *);
extern pth_t          pth_self(void);
extern int            pth_suspend(pth_t);
extern int            pth_resume(pth_t);
extern int            pth_yield(pth_t);
extern int            pth_nap(pth_time_t);
extern int            pth_wait(pth_event_t);
extern int            pth_cancel(pth_t);
extern int            pth_abort(pth_t);
extern int            pth_raise(pth_t, int);
extern int            pth_join(pth_t, void **);
extern void           pth_exit(void *);

    /* utility functions */
extern int            pth_fdmode(int, int);
extern pth_time_t     pth_time(long, long);
extern pth_time_t     pth_timeout(long, long);

    /* cancellation functions */
extern void           pth_cancel_state(int, int *);
extern void           pth_cancel_point(void);

    /* event functions */
extern pth_event_t    pth_event(unsigned long, ...);
extern unsigned long  pth_event_typeof(pth_event_t);
extern int            pth_event_extract(pth_event_t ev, ...);
extern pth_event_t    pth_event_concat(pth_event_t, ...);
extern pth_event_t    pth_event_isolate(pth_event_t);
extern pth_event_t    pth_event_walk(pth_event_t, unsigned int);
extern pth_status_t   pth_event_status(pth_event_t);
extern int            pth_event_free(pth_event_t, int);

    /* key-based storage functions */
extern int            pth_key_create(pth_key_t *, void (*)(void *));
extern int            pth_key_delete(pth_key_t);
extern int            pth_key_setdata(pth_key_t, const void *);
extern void          *pth_key_getdata(pth_key_t);

    /* message port functions */
extern pth_msgport_t  pth_msgport_create(const char *);
extern void           pth_msgport_destroy(pth_msgport_t);
extern pth_msgport_t  pth_msgport_find(const char *);
extern int            pth_msgport_pending(pth_msgport_t);
extern int            pth_msgport_put(pth_msgport_t, pth_message_t *);
extern pth_message_t *pth_msgport_get(pth_msgport_t);
extern int            pth_msgport_reply(pth_message_t *);

    /* cleanup handler functions */
extern int            pth_cleanup_push(void (*)(void *), void *);
extern int            pth_cleanup_pop(int);

    /* process forking functions */
extern int            pth_atfork_push(void (*)(void *), void (*)(void *), void (*)(void *), void *);
extern int            pth_atfork_pop(void);
extern pid_t          pth_fork(void);

    /* synchronization functions */
extern int            pth_mutex_init(pth_mutex_t *);
extern int            pth_mutex_acquire(pth_mutex_t *, int, pth_event_t);
extern int            pth_mutex_release(pth_mutex_t *);
extern int            pth_rwlock_init(pth_rwlock_t *);
extern int            pth_rwlock_acquire(pth_rwlock_t *, int, int, pth_event_t);
extern int            pth_rwlock_release(pth_rwlock_t *);
extern int            pth_cond_init(pth_cond_t *);
extern int            pth_cond_await(pth_cond_t *, pth_mutex_t *, pth_event_t);
extern int            pth_cond_notify(pth_cond_t *, int);
extern int            pth_barrier_init(pth_barrier_t *, int);
extern int            pth_barrier_reach(pth_barrier_t *);

    /* user-space context functions */
extern int            pth_uctx_create(pth_uctx_t *);
extern int            pth_uctx_make(pth_uctx_t, char *, size_t, const sigset_t *, void (*)(void *), void *, pth_uctx_t);
extern int            pth_uctx_switch(pth_uctx_t, pth_uctx_t);
extern int            pth_uctx_destroy(pth_uctx_t);

    /* extension functions */
extern Sfdisc_t      *pth_sfiodisc(void);


END_DECLARATION

    /* soft system call mapping support */
#if PTH_SYSCALL_SOFT && !defined(_PTH_PRIVATE)
#define fork          pth_fork
#define waitpid       pth_waitpid
#define system        pth_system
#define nanosleep     pth_nanosleep
#define usleep        pth_usleep
#define sleep         pth_sleep
#define sigprocmask   pth_sigmask
#define sigwait       pth_sigwait
#define select        pth_select
#define pselect       pth_pselect
#define poll          pth_poll
#define connect       pth_connect
#define accept        pth_accept
#define read          pth_read
#define write         pth_write
#define readv         pth_readv
#define writev        pth_writev
#define recv          pth_recv
#define send          pth_send
#define recvfrom      pth_recvfrom
#define sendto        pth_sendto
#define pread         pth_pread
#define pwrite        pth_pwrite
#endif

    /* backward compatibility (Pth < 1.5.0) */
#define pth_event_occurred(ev) \
    (   pth_event_status(ev) == PTH_STATUS_OCCURRED \
     || pth_event_status(ev) == PTH_STATUS_FAILED   )

typedef struct pth_cleanup_st pth_cleanup_t;
struct pth_cleanup_st {
    pth_cleanup_t *next;
    void (*func)(void *);
    void *arg;
};

typedef struct pth_mctx_st pth_mctx_t;
struct pth_mctx_st {
    unsigned int sp;
};

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

extern  int pth_errno_storage;
extern int pth_errno_flag    ;

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

extern pth_time_t pth_time_zero;
#define PTH_TIME_NOW  (pth_time_t *)(0)
#define PTH_TIME_ZERO &pth_time_zero
#define PTH_TIME(sec,usec) { sec, usec }
#define pth_time_equal(t1,t2) \
        (((t1).tv_sec == (t2).tv_sec) && ((t1).tv_usec == (t2).tv_usec))

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

#if defined(HAVE_GETTIMEOFDAY_ARGS1)
#define __gettimeofday(t) gettimeofday(t)
#else
#define __gettimeofday(t) gettimeofday(t, NULL)
#endif
#define pth_time_set(t1,t2) \
    do { \
        if ((t2) == PTH_TIME_NOW) \
            __gettimeofday((t1)); \
        else { \
            (t1)->tv_sec  = (t2)->tv_sec; \
            (t1)->tv_usec = (t2)->tv_usec; \
        } \
    } while (0)

intern char *pth_util_cpystrn(char *dst, const char *src, size_t dst_size);
intern int pth_attr_ctrl(int cmd, pth_attr_t a, int op, va_list ap);

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

intern int pth_pqueue_contains(pth_pqueue_t *q, pth_t t);
intern void pth_pqueue_delete(pth_pqueue_t *q, pth_t t);
intern void pth_thread_cleanup(pth_t thread);
intern void pth_tcb_free(pth_t t);
intern void pth_pqueue_insert(pth_pqueue_t *q, int prio, pth_t t);
intern int
pth_snprintf(
    char *str,
    size_t count,
    const char *fmt,
    ...);
intern int
pth_vsnprintf(
    char *str,
    size_t count,
    const char *fmt,
    va_list args);

#define pth_sc(func) func
intern void pth_dumpqueue(FILE *fp, const char *qn, pth_pqueue_t *q);

#define pth_pqueue_elements(q) \
    ((q) == NULL ? (-1) : (q)->q_num)

#define pth_pqueue_head(q) \
    ((q) == NULL ? NULL : (q)->q_head)

intern pth_t pth_pqueue_walk(pth_pqueue_t *q, pth_t t, int direction);

intern int pth_util_fd_valid(int fd);
intern void pth_scheduler_drop(void);

#define pth_pqueue_favorite_prio(q) \
    ((q)->q_head != NULL ? (q)->q_head->q_prio + 1 : PTH_PRIO_MAX)

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

intern void pth_ring_append(pth_ring_t *r, pth_ringnode_t *rn);

intern void pth_ring_delete(pth_ring_t *r, pth_ringnode_t *rn);

intern void pth_cleanup_popall(pth_t t, int execute);

#define pth_time_add(t1,t2) \
    (t1)->tv_sec  += (t2)->tv_sec; \
    (t1)->tv_usec += (t2)->tv_usec; \
    if ((t1)->tv_usec > 1000000) { \
        (t1)->tv_sec  += 1; \
        (t1)->tv_usec -= 1000000; \
    }

/* calculate: t1 = t1 - t2 */
#define pth_time_sub(t1,t2) \
    (t1)->tv_sec  -= (t2)->tv_sec; \
    (t1)->tv_usec -= (t2)->tv_usec; \
    if ((t1)->tv_usec < 0) { \
        (t1)->tv_sec  -= 1; \
        (t1)->tv_usec += 1000000; \
    }

/* message port structure */
struct pth_msgport_st {
    pth_ringnode_t mp_node;  /* maintainance node handle */
    const char    *mp_name;  /* optional name of message port */
    pth_t          mp_tid;   /* corresponding thread */
    pth_ring_t     mp_queue; /* queue of messages pending on port */
};

intern void pth_ring_init(pth_ring_t *r);

intern pth_ringnode_t *pth_ring_pop(pth_ring_t *r);

intern void *pth_scheduler(void *dummy);

#endif /* _PTH_H_ */

