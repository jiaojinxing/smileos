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
** File name:               signal.h
** Last modified Date:      2013-1-6
** Last Version:            1.0.0
** Descriptions:            信号
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-1-6
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
#ifndef _SYS_SIGNAL_H_
#define _SYS_SIGNAL_H_

#include "_ansi.h"
#include <sys/types.h>

typedef int sig_atomic_t;                                               /*  Atomic entity type (ANSI)   */

typedef void (*_sig_func_ptr)();

typedef _sig_func_ptr sighandler_t;                                     /*  glibc naming                */

typedef unsigned long sigset_t;

union sigval {
    int                 sival_int;                                      /*  Integer signal value        */
    void               *sival_ptr;                                      /*  Pointer signal value        */
};

typedef struct {
    int                 si_signo;                                       /*  Signal number               */
    int                 si_code;                                        /*  Cause of the signal         */
    union sigval        si_value;                                       /*  Signal value                */
} siginfo_t;

struct sigaction {
    int                 sa_flags;                       /*  Special flags to affect behavior of signal  */
    sigset_t            sa_mask;                        /*  Additional set of signals to be blocked     */
                                                        /*  during execution of signal-catching         */
                                                        /*  function.                                   */
    union {
        _sig_func_ptr   _handler;                       /* SIG_DFL, SIG_IGN, or pointer to a function   */
        void          (*_sigaction)(int, siginfo_t *, void *);
    }                   _signal_handlers;
};

#define sa_handler      _signal_handlers._handler
#define sa_sigaction    _signal_handlers._sigaction

#define SIG_DFL         ((_sig_func_ptr)0)                              /*  Default action              */
#define SIG_IGN         ((_sig_func_ptr)1)                              /*  Ignore action               */
#define SIG_ERR         ((_sig_func_ptr)-1)                             /*  Error return                */

#define SIGHUP          1   /* hangup */
#define SIGINT          2   /* interrupt */
#define SIGQUIT         3   /* quit */
#define SIGILL          4   /* illegal instruction (not reset when caught) */
#define SIGTRAP         5   /* trace trap (not reset when caught) */
#define SIGIOT          6   /* IOT instruction */
#define SIGABRT         6   /* used by abort, replace SIGIOT in the future */
#define SIGEMT          7   /* EMT instruction */
#define SIGFPE          8   /* floating point exception */
#define SIGKILL         9   /* kill (cannot be caught or ignored) */
#define SIGBUS          10  /* bus error */
#define SIGSEGV         11  /* segmentation violation */
#define SIGSYS          12  /* bad argument to system call */
#define SIGPIPE         13  /* write on a pipe with no one to read it */
#define SIGALRM         14  /* alarm clock */
#define SIGTERM         15  /* software termination signal from kill */
#define SIGURG          16  /* urgent condition on IO channel */
#define SIGSTOP         17  /* sendable stop signal not from tty */
#define SIGTSTP         18  /* stop signal from tty */
#define SIGCONT         19  /* continue a stopped process */
#define SIGCHLD         20  /* to parent on child stop or exit */
#define SIGCLD          20  /* System V name for SIGCHLD */
#define SIGTTIN         21  /* to readers pgrp upon background tty read */
#define SIGTTOU         22  /* like TTIN for output if (tp->t_local&LTOSTOP) */
#define SIGIO           23  /* input/output possible signal */
#define SIGPOLL         23  /* System V name for SIGIO */
#define SIGXCPU         24  /* exceeded CPU time limit */
#define SIGXFSZ         25  /* exceeded file size limit */
#define SIGVTALRM       26  /* virtual time alarm */
#define SIGPROF         27  /* profiling time alarm */
#define SIGWINCH        28  /* window changed */
#define SIGLOST         29  /* resource lost (eg, record-lock lost) */
#define SIGUSR1         30  /* user defined signal 1 */
#define SIGUSR2         31  /* user defined signal 2 */
#define SIGRTMIN        32
#define SIGRTMAX        63
#define NSIG            64  /* signal 0 implied */

/* These depend upon the type of sigset_t, which right now
   is always a long.. They're in the POSIX namespace, but
   are not ANSI. */
#define sigaddset(what,sig)     (*(what) |= (1<<(sig)), 0)
#define sigdelset(what,sig)     (*(what) &= ~(1<<(sig)), 0)
#define sigemptyset(what)       (*(what) = 0, 0)
#define sigfillset(what)        (*(what) = ~(0), 0)
#define sigismember(what,sig)   (((*(what)) & (1<<(sig))) != 0)

#define SIG_SETMASK     0   /* set mask with sigprocmask() */
#define SIG_BLOCK       1   /* set of signals to block */
#define SIG_UNBLOCK     2   /* set of signals to, well, unblock */

int _EXFUN(kill, (pid_t, int));
int _EXFUN(killpg, (pid_t, int));
int _EXFUN(sigaction, (int, const struct sigaction *, struct sigaction *));
int _EXFUN(sigpending, (sigset_t *));
int _EXFUN(sigsuspend, (const sigset_t *));
int _EXFUN(sigpause, (int));
_sig_func_ptr _EXFUN(signal, (int, _sig_func_ptr));
int _EXFUN(raise, (int));
int _EXFUN(sigprocmask, (int how, const sigset_t *set, sigset_t *oset));

#endif                                                                  /*  _SYS_SIGNAL_H_              */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
