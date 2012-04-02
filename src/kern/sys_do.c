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
** Descriptions:            系统调用处理
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "vfs/vfs.h"
#include <sys/types.h>
#include <sys/time.h>
#include <sys/reent.h>
#include <sys/socket.h>
/*********************************************************************************************************
  系统调用处理
*********************************************************************************************************/
/*
 * 任务退出
 */
static void do_exit(int status)
{
    task_kill(current->tid);                                            /*  杀死当前任务                */
}

/*
 * 任务休眠
 */
static int do_sleep(unsigned int ticks)
{
    current->timer = ticks != 0 ? ticks : 1;                            /*  休睡 TICK 数                */

    current->state = TASK_SLEEPING;                                     /*  当前任务进入休睡态          */

    schedule();                                                         /*  任务调度                    */

    return 0;
}

/*
 * 获得时间
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
 * 获得 PID
 */
static int do_getpid(void)
{
    return (int)current->pid;
}

/*
 * 释放 CPU 使用权
 */
static void do_yeild(void)
{
    schedule();
}

/*
 * 获得 reent 结构指针
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
        return lwip_bind(sock_fd, name, namelen);
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
        int new_sock_fd = lwip_accept(sock_fd, addr, addrlen);
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
        return lwip_connect(sock_fd, name, namelen);
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
/*********************************************************************************************************
  系统调用处理表
*********************************************************************************************************/
sys_do_t sys_do_table[] = {
        /*
         * do_xxx 的位置必须要和 SYS_CALL_XXX 的值一致
         */
        (sys_do_t)do_exit,
        (sys_do_t)do_sleep,
        (sys_do_t)do_yeild,
        (sys_do_t)do_gettimeofday,
        (sys_do_t)do_getpid,
        (sys_do_t)vfs_write,
        (sys_do_t)vfs_mkdir,
        (sys_do_t)vfs_open,
        (sys_do_t)vfs_read,
        (sys_do_t)vfs_rename,
        (sys_do_t)vfs_fstat,
        (sys_do_t)vfs_unlink,
        (sys_do_t)vfs_close,
        (sys_do_t)vfs_fcntl,
        (sys_do_t)vfs_isatty,
        (sys_do_t)vfs_link,
        (sys_do_t)vfs_lseek,
        (sys_do_t)vfs_stat,
        (sys_do_t)do_getreent,
        (sys_do_t)do_socket,
        (sys_do_t)do_bind,
        (sys_do_t)do_accept,
        (sys_do_t)do_connect,
        (sys_do_t)do_listen,
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
