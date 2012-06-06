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
** File name:               sys_do.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            ϵͳ���ô���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "vfs/vfs.h"
#include <sys/time.h>
#include <sys/reent.h>
#include <sys/socket.h>

static inline void *va_to_mva(void *addr)
{
    if ((uint32_t)addr >= PROCESS_SPACE_SIZE) {
        return addr;
    } else {
        return (uint8_t *)addr + current->pid * PROCESS_SPACE_SIZE;
    }
}
/*********************************************************************************************************
  ϵͳ���ô���
*********************************************************************************************************/
/*
 * �����˳�
 */
static void do_exit(int status)
{
    task_kill(current->tid);                                            /*  ɱ����ǰ����                */
}

/*
 * ��������
 */
static int do_sleep(unsigned int ticks)
{
    current->timer = ticks != 0 ? ticks : 1;                            /*  ��˯ TICK ��                */

    current->state = TASK_SLEEPING;                                     /*  ��ǰ���������˯̬          */

    current->resume_type = TASK_RESUME_UNKNOW;                          /*  ���ûָ�����Ϊδ֪          */

    schedule();                                                         /*  �������                    */

    return 0;
}

/*
 * ���ʱ��
 */
static int do_gettimeofday(struct timeval *tv, void *tzp)
{
    uint64_t tick;

    tick = gettick();

    tv->tv_sec  = (tick / TICK_PER_SECOND);
    tv->tv_usec = (tick % TICK_PER_SECOND) * 1000000 / TICK_PER_SECOND;

    return 0;
}

/*
 * ��� PID
 */
static int do_getpid(void)
{
    return (int)current->pid;
}

/*
 * �ͷ� CPU ʹ��Ȩ
 */
static void do_yeild(void)
{
    schedule();
}

/*
 * ��� reent �ṹָ��
 */
static struct _reent *do_getreent(void)
{
    return _impure_ptr;
}

/*
 * do_socket
 */
static int do_socket(int domain, int type, int protocol)
{
    int sock_fd = lwip_socket(domain, type, protocol);
    if (sock_fd >= 0) {
        int fd = socket_attach(sock_fd);
        if (fd < 0) {
            lwip_close(sock_fd);
            return -1;
        }
        return fd;
    } else {
        return -1;
    }
}

/*
 * do_bind
 */
static int do_bind(int s, const struct sockaddr *name, socklen_t namelen)
{
    int sock_fd = socket_priv_fd(s);
    if (sock_fd >= 0) {
        return lwip_bind(sock_fd, va_to_mva(name), namelen);
    } else {
        return -1;
    }
}

/*
 * do_accept
 */
static int do_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    int sock_fd = socket_priv_fd(s);
    if (sock_fd >= 0) {
        int new_sock_fd = lwip_accept(sock_fd, va_to_mva(addr), va_to_mva(addrlen));
        if (new_sock_fd >= 0) {
            int fd = socket_attach(new_sock_fd);
            if (fd < 0) {
                lwip_close(new_sock_fd);
                return -1;
            }
            return fd;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

/*
 * do_connect
 */
static int do_connect(int s, const struct sockaddr *name, socklen_t namelen)
{
    int sock_fd = socket_priv_fd(s);
    if (sock_fd >= 0) {
        return lwip_connect(sock_fd, va_to_mva(name), namelen);
    } else {
        return -1;
    }
}

/*
 * do_listen
 */
static int do_listen(int s, int backlog)
{
    int sock_fd = socket_priv_fd(s);
    if (sock_fd >= 0) {
        return lwip_listen(sock_fd, backlog);
    } else {
        return -1;
    }
}

/*
 * do_select
 */
static int do_select(sys_do_args_t *args)
{
    return vfs_select(
            (int)args->arg0,
            va_to_mva(args->arg1),
            va_to_mva(args->arg2),
            va_to_mva(args->arg3),
            va_to_mva(args->arg4));
}

/*
 * do_recv
 */
static int do_recv(int s, void *mem, size_t len, int flags)
{
    int sock_fd = socket_priv_fd(s);
    if (sock_fd >= 0) {
        return lwip_recv(sock_fd, va_to_mva(mem), len, flags);
    } else {
        return -1;
    }
}

/*
 * do_recvfrom
 */
static int do_recvfrom(sys_do_args_t *args)
{
    int sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        return lwip_recvfrom(
                sock_fd,
                va_to_mva(args->arg1),
                (size_t)args->arg2,
                (int)args->arg3,
                (struct sockaddr *)va_to_mva(args->arg4),
                (socklen_t *)va_to_mva(args->arg5));
    } else {
        return -1;
    }
}

/*
 * do_sendto
 */
static int do_sendto(sys_do_args_t *args)
{
    int sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        return lwip_sendto(
                sock_fd,
                va_to_mva(args->arg1),
                (size_t)args->arg2,
                (int)args->arg3,
                (const struct sockaddr *)va_to_mva(args->arg4),
                (socklen_t)args->arg5);
    } else {
        return -1;
    }
}

/*
 * do_getsockopt
 */
static int do_getsockopt(sys_do_args_t *args)
{
    int sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        return lwip_getsockopt(
                sock_fd,
                (int)args->arg1,
                (int)args->arg2,
                va_to_mva(args->arg3),
                (socklen_t *)va_to_mva(args->arg4));
    } else {
        return -1;
    }
}

/*
 * do_send
 */
static do_send(int s, const void *data, size_t size, int flags)
{
    int sock_fd = socket_priv_fd(s);
    if (sock_fd >= 0) {
        return lwip_send(sock_fd, va_to_mva(data), size, flags);
    } else {
        return -1;
    }
}

/*
 * do_setsockopt
 */
static int do_setsockopt(sys_do_args_t *args)
{
    int sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        return lwip_setsockopt(
                sock_fd,
                (int)args->arg1,
                (int)args->arg2,
                va_to_mva(args->arg3),
                (socklen_t)args->arg4);
    } else {
        return -1;
    }
}

/*
 * do_shutdown
 */
static int do_shutdown(int s, int how)
{
    int sock_fd = socket_priv_fd(s);
    if (sock_fd >= 0) {
        return lwip_shutdown(sock_fd, how);
    } else {
        return -1;
    }
}
/*********************************************************************************************************
  ϵͳ���ô����
*********************************************************************************************************/
sys_do_t sys_do_table[] = {
        /*
         * do_xxx ��λ�ñ���Ҫ�� SYS_CALL_XXX ��ֵһ��
         */
#define SYS_CALL_EXIT       0
#define SYS_CALL_SLEEP      1
#define SYS_CALL_YIELD      2
        (sys_do_t)do_exit,
        (sys_do_t)do_sleep,
        (sys_do_t)do_yeild,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#define SYS_CALL_GETTIME    10
#define SYS_CALL_GETPID     11
#define SYS_CALL_GETREENT   12
        (sys_do_t)do_gettimeofday,
        (sys_do_t)do_getpid,
        (sys_do_t)do_getreent,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#define SYS_CALL_OPEN       20
#define SYS_CALL_READ       21
#define SYS_CALL_WRITE      22
#define SYS_CALL_FCNTL      23
#define SYS_CALL_ISATTY     24
#define SYS_CALL_FSTAT      25
#define SYS_CALL_LSEEK      26
#define SYS_CALL_CLOSE      27
#define SYS_CALL_IOCTL      28
#define SYS_CALL_SELECT     29
        (sys_do_t)vfs_open,
        (sys_do_t)vfs_read,
        (sys_do_t)vfs_write,
        (sys_do_t)vfs_fcntl,
        (sys_do_t)vfs_isatty,
        (sys_do_t)vfs_fstat,
        (sys_do_t)vfs_lseek,
        (sys_do_t)vfs_close,
        (sys_do_t)vfs_ioctl,
        (sys_do_t)do_select,
#define SYS_CALL_RENAME     30
#define SYS_CALL_UNLINK     31
#define SYS_CALL_LINK       32
#define SYS_CALL_STAT       33
#define SYS_CALL_MKDIR      34
#define SYS_CALL_RMDIR      35
        (sys_do_t)vfs_rename,
        (sys_do_t)vfs_unlink,
        (sys_do_t)vfs_link,
        (sys_do_t)vfs_stat,
        (sys_do_t)vfs_mkdir,
        (sys_do_t)vfs_rmdir,
        NULL,
        NULL,
        NULL,
        NULL,
#define SYS_CALL_OPENDIR    40
#define SYS_CALL_READDIR    41
#define SYS_CALL_SEEKDIR    42
#define SYS_CALL_REWINDDIR  43
#define SYS_CALL_TELLDIR    44
#define SYS_CALL_CLOSEDIR   45
        (sys_do_t)vfs_opendir,
        (sys_do_t)vfs_readdir,
        (sys_do_t)vfs_seekdir,
        (sys_do_t)vfs_rewinddir,
        (sys_do_t)vfs_telldir,
        (sys_do_t)vfs_closedir,
        NULL,
        NULL,
        NULL,
        NULL,
#define SYS_CALL_GETCWD     50
#define SYS_CALL_CHDIR      51
        (sys_do_t)vfs_getcwd,
        (sys_do_t)vfs_chdir,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#define SYS_CALL_SOCKET     60
#define SYS_CALL_BIND       61
#define SYS_CALL_ACCEPT     62
#define SYS_CALL_CONNECT    63
#define SYS_CALL_LISTEN     64
#define SYS_CALL_RECV       65
#define SYS_CALL_RECVFROM   66
#define SYS_CALL_SENDTO     67
#define SYS_CALL_GETSOCKOPT 68
#define SYS_CALL_SEND       69
#define SYS_CALL_SHUTDOWN   70
#define SYS_CALL_SETSOCKOPT 71
        (sys_do_t)do_socket,
        (sys_do_t)do_bind,
        (sys_do_t)do_accept,
        (sys_do_t)do_connect,
        (sys_do_t)do_listen,
        (sys_do_t)do_recv,
        (sys_do_t)do_recvfrom,
        (sys_do_t)do_sendto,
        (sys_do_t)do_getsockopt,
        (sys_do_t)do_send,
        (sys_do_t)do_shutdown,
        (sys_do_t)do_setsockopt,
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
