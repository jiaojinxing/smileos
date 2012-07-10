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
** File name:               pth_acdef.h
** Last modified Date:      2012-4-27
** Last Version:            1.0.0
** Descriptions:            GNU/pth 库配置头文件
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
#ifndef _PTH_ACDEF_H_
#define _PTH_ACDEF_H_

/* Define to 1 if you have the `dlclose' function. */
#undef HAVE_DLCLOSE

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Define to 1 if you have the `dlopen' function. */
#undef HAVE_DLOPEN

/* Define to 1 if you have the `dlsym' function. */
#undef HAVE_DLSYM

/* Define to 1 if you have the <dmalloc.h> header file. */
#undef HAVE_DMALLOC_H

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the <ex.h> header file. */
#undef HAVE_EX_H

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `getcontext' function. */
#undef HAVE_GETCONTEXT

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* define if gettimeofday(2) wants a single-argument only */
#undef HAVE_GETTIMEOFDAY_ARGS1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `dl' library (-ldl). */
#undef HAVE_LIBDL

/* Define to 1 if you have the `dmalloc' library (-ldmalloc). */
#undef HAVE_LIBDMALLOC

/* Define to 1 if you have the `ex' library (-lex). */
#undef HAVE_LIBEX

/* Define to 1 if you have the `nsl' library (-lnsl). */
#undef HAVE_LIBNSL

/* Define to 1 if you have the `sfio' library (-lsfio). */
#undef HAVE_LIBSFIO

/* Define to 1 if you have the `socket' library (-lsocket). */
#define HAVE_LIBSOCKET 1

/* define if "long double" type exists */
#define HAVE_LONGDOUBLE 1

/* Define to 1 if you have the `longjmp' function. */
#define HAVE_LONGJMP 1

/* define if "long long" type exists */
#define HAVE_LONGLONG 1

/* Define to 1 if you have the `makecontext' function. */
#undef HAVE_MAKECONTEXT

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <net/errno.h> header file. */
#undef HAVE_NET_ERRNO_H

/* define if typedef nfds_t exists in header poll.h */
#define HAVE_NFDS_T 1

/* define if typedef off_t exists in header sys/types.h */
#define HAVE_OFF_T 1

/* Define to 1 if you have the <paths.h> header file. */
#define HAVE_PATHS_H 1

/* define if typedef pid_t exists in header sys/types.h */
#define HAVE_PID_T 1

/* Define to 1 if you have the `poll' function. */
#define HAVE_POLL 1

/* define if pre-processor define POLLIN exists in header poll.h */
#define HAVE_POLLIN 1

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have the `readv' function. */
#define HAVE_READV 1

/* define if pre-processor define RTLD_NEXT exists in header dlfcn.h */
#undef HAVE_RTLD_NEXT

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `setcontext' function. */
#undef HAVE_SETCONTEXT

/* Define to 1 if you have the `setjmp' function. */
#define HAVE_SETJMP 1

/* Define to 1 if you have the <setjmp.h> header file. */
#define HAVE_SETJMP_H 1

/* Define to 1 if you have the <sfio.h> header file. */
#undef HAVE_SFIO_H

/* Define to 1 if you have the `sigaction' function. */
#undef HAVE_SIGACTION

/* Define to 1 if you have the `sigaltstack' function. */
#undef HAVE_SIGALTSTACK

/* Define to 1 if you have the `siglongjmp' function. */
#undef HAVE_SIGLONGJMP

/* Define to 1 if you have the <signal.h> header file. */
#undef HAVE_SIGNAL_H

/* Define to 1 if you have the `sigpending' function. */
#undef HAVE_SIGPENDING

/* Define to 1 if you have the `sigprocmask' function. */
#undef HAVE_SIGPROCMASK

/* Define to 1 if you have the `sigsetjmp' function. */
#undef HAVE_SIGSETJMP

/* Define to 1 if you have the `sigstack' function. */
#undef HAVE_SIGSTACK

/* Define to 1 if you have the `sigsuspend' function. */
#undef HAVE_SIGSUSPEND

/* define if typedef sig_atomic_t exists in header signal.h */
#undef HAVE_SIG_ATOMIC_T

/* define if typedef size_t exists in header stdlib.h */
#define HAVE_SIZE_T 1

/* define if typedef socklen_t exists in header sys/socket.h */
#define HAVE_SOCKLEN_T 1

/* define if typedef ssize_t exists in header sys/types.h */
#define HAVE_SSIZE_T 1

/* define if attribute ss_base exists in struct sigaltstack from header
   sys/signal.h */
#undef HAVE_SS_BASE

/* define if attribute ss_sp exists in struct sigaltstack from header
   sys/signal.h */
#undef HAVE_SS_SP

/* define if typedef stack_t exists in header signal.h */
#undef HAVE_STACK_T

/* Define to 1 if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* define if exists "struct timespec" */
#define HAVE_STRUCT_TIMESPEC 1

/* Define to 1 if you have the `swapcontext' function. */
#undef HAVE_SWAPCONTEXT

/* Define to 1 if you have the `syscall' function. */
#define HAVE_SYSCALL 1

/* define if pre-processor define SYS_read exists in header sys/syscall.h */
#define HAVE_SYS_READ 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socketcall.h> header file. */
#undef HAVE_SYS_SOCKETCALL_H

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/syscall.h> header file. */
#undef HAVE_SYS_SYSCALL_H

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#define HAVE_SYS_UIO_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
#undef HAVE_SYS_WAIT_H

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `usleep' function. */
#define HAVE_USLEEP 1

/* Define to 1 if you have the `writev' function. */
#define HAVE_WRITEV 1

/* Define to 1 if you have the `_longjmp' function. */
#undef HAVE__LONGJMP

/* Define to 1 if you have the `_setjmp' function. */
#undef HAVE__SETJMP

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* define to enable Pth debugging */
#undef PTH_DEBUG

/* define if using Dmalloc in GNU pth */
#undef PTH_DMALLOC

/* define if using OSSP ex in GNU pth */
#undef PTH_EX

/* define for machine context dispatching */
#undef PTH_MCTX_DSP_use

/* define for machine context method */
#undef PTH_MCTX_MTH_use

/* define for machine context stack */
#undef PTH_MCTX_STK_use

/* define for number of signals */
#undef PTH_NSIG

/* define for stack growth */
#define PTH_STACKGROWTH -1

/* define for the paths to syscall dynamic libraries */
#undef PTH_SYSCALL_LIBS

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

#define PTH_FAKE_RWV        1
#define PTH_EXT_SFIO        0
#define PTH_FAKE_POLL       0
#define PTH_SYSCALL_HARD    1
#define PTH_SYSCALL_SOFT    0

#define PTH_VERSION_STR     "pth-2.0.7"
#define PTH_VERSION_HEX     0xCF

#define PTH_FDSETSIZE       64

#define intern

#endif                                                                  /*  _PTH_ACDEF_H_               */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
