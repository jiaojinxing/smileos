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
#include "kern/kern.h"
#include "kern/kvars.h"
#include "kern/func_config.h"
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

extern int          task_cleanup(int32_t tid);
extern int          task_sleep(tick_t ticks);
extern int32_t      task_getpid(void);
extern void         task_schedule(void);

#if CONFIG_SIGNAL_EN > 0
#include <signal.h>
extern int          signal_procmask(int how, const sigset_t *set, sigset_t *oset);
extern int          signal_suspend(const sigset_t *set);

extern int          signal_queue(int32_t tid, int sig);
extern sighandler_t signal_signal(int sig, sighandler_t func);

extern int          task_alarm(uint32_t secs);
extern int          task_pause(void);
#endif

#if CONFIG_VFS_EN > 0
extern int          pipe_create(int fds[2]);
#endif

/*********************************************************************************************************
** Function name:           sys_do_enter
** Descriptions:            进入系统调用处理
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void sys_do_enter(void)
{
    if (!os_started) {
        while (1);
    }

    /*
     * TODO: 为避免调试应用程序时 GDB 进入内核, 这里应该加上一些处理
     */
}
/*********************************************************************************************************
** Function name:           sys_do_exit
** Descriptions:            退出系统调用处理
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void sys_do_exit(void)
{
}
/*********************************************************************************************************
** Function name:           do_exit
** Descriptions:            exit 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void do_exit(syscall_args_t *args)
{
    sys_do_enter();

#if CONFIG_SIGNAL_EN > 0
    kill(gettid(), SIGKILL);
#else
    task_cleanup(gettid());
#endif

    sys_do_exit();
}
/*********************************************************************************************************
** Function name:           do_msleep
** Descriptions:            usleep 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_msleep(syscall_args_t *args)
{
    mseconds_t timeout;
    int ret;

    sys_do_enter();

    timeout = *(mseconds_t *)args->arg0;
    ret = task_sleep(ms_to_tick(timeout));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_gettimeofday
** Descriptions:            gettimeofday 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_gettimeofday(syscall_args_t *args)
{
    struct timeval *tv = args->arg0;
    //void *tzp = args->arg1;
    tick_t tick;

    sys_do_enter();

    tick = getticks();

    tv->tv_sec  = (tick / TICK_PER_SECOND);
    tv->tv_usec = (tick % TICK_PER_SECOND) * 1000000 / TICK_PER_SECOND;

    sys_do_exit();

    return 0;
}
/*********************************************************************************************************
** Function name:           do_schedule
** Descriptions:            schedule 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_schedule(syscall_args_t *args)
{
    sys_do_enter();

    task_schedule();

    sys_do_exit();

    return 0;
}
/*********************************************************************************************************
** Function name:           do_bad
** Descriptions:            错误系统调用处理函数
** input parameters:        bad_syscall         错误系统调用号
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_bad(int bad_syscall)
{
    int ret;

    sys_do_enter();

    printk(KERN_ERR"%s: bad system call %d\n", __func__, bad_syscall);

#if CONFIG_SIGNAL_EN > 0
    ret = kill(gettid(), SIGSYS);
#else
    ret = task_cleanup(gettid());
#endif

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_getpid
** Descriptions:            getpid 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          PID
*********************************************************************************************************/
static int do_getpid(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = task_getpid();

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** 信号相关系统调用处理
*********************************************************************************************************/
#if CONFIG_SIGNAL_EN > 0
/*********************************************************************************************************
** Function name:           do_signal
** Descriptions:            signal 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          原信号处理函数
*********************************************************************************************************/
static sighandler_t do_signal(syscall_args_t *args)
{
    sighandler_t ret;

    sys_do_enter();

    ret = signal_signal((int)args->arg0, (sighandler_t)args->arg1);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_sigprocmask
** Descriptions:            sigprocmask 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_sigprocmask(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = signal_procmask((int)args->arg0,
            (const sigset_t *)ua_to_ka(args->arg1),
            (sigset_t *)ua_to_ka(args->arg2));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_sigsuspend
** Descriptions:            sigsuspend 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_sigsuspend(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = signal_suspend((const sigset_t *)ua_to_ka(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_alarm
** Descriptions:            alarm 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_alarm(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = task_alarm((uint32_t)args->arg0);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_pause
** Descriptions:            pause 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_pause(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = task_pause();

    sys_do_exit();

    return ret;
}
#endif
/*********************************************************************************************************
** Function name:           do_kill
** Descriptions:            kill 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_kill(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

#if CONFIG_SIGNAL_EN > 0
    ret = signal_queue((int32_t)args->arg0, (int)args->arg1);
#else
    ret = task_cleanup((int32_t)args->arg0);
#endif

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** 进程相关系统调用处理
*********************************************************************************************************/
#if CONFIG_PROCESS_EN > 0

#include "kern/pinfo.h"

/*********************************************************************************************************
** Function name:           do_setpinfo
** Descriptions:            setpinfo 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_setpinfo(syscall_args_t *args)
{
    int ret = -1;

    sys_do_enter();

//    extern int task_setpinfo(pinfo_t *pinfo);
//    ret = task_setpinfo(ua_to_ka(args->arg0));

    sys_do_exit();

    return ret;
}
#endif
/*********************************************************************************************************
** 虚拟文件系统相关系统调用处理
*********************************************************************************************************/
#if CONFIG_VFS_EN > 0

#include "vfs/vfs.h"
#include <dirent.h>

/*********************************************************************************************************
** Function name:           do_open
** Descriptions:            open 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_open(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_open(ua_to_ka(args->arg0), (int)args->arg1, (mode_t)args->arg2);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_read
** Descriptions:            read 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          成功读取的字节数 OR -1
*********************************************************************************************************/
static ssize_t do_read(syscall_args_t *args)
{
    ssize_t ret;

    sys_do_enter();

    ret = vfs_read((int)args->arg0, ua_to_ka(args->arg1), (size_t)args->arg2);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_write
** Descriptions:            write 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          成功写入的字节数 OR -1
*********************************************************************************************************/
static ssize_t do_write(syscall_args_t *args)
{
    ssize_t ret;

    sys_do_enter();

    ret = vfs_write((int)args->arg0, ua_to_ka(args->arg1), (size_t)args->arg2);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_fcntl
** Descriptions:            fcntl 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_fcntl(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_fcntl((int)args->arg0, (int)args->arg1, (int)args->arg2);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_isatty
** Descriptions:            isatty 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR 1 OR -1
*********************************************************************************************************/
static int do_isatty(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_isatty((int)args->arg0);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_fstat
** Descriptions:            fstat 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_fstat(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_fstat((int)args->arg0, ua_to_ka(args->arg1));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_lseek
** Descriptions:            lseek 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          新的读写位置 OR -1
*********************************************************************************************************/
static off_t do_lseek(syscall_args_t *args)
{
    off_t ret;

    sys_do_enter();

    ret = vfs_lseek((int)args->arg0, (off_t)args->arg1, (int)args->arg2);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_close
** Descriptions:            close 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_close(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_close((int)args->arg0);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_ioctl
** Descriptions:            ioctl 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_ioctl(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    /*
     * 注意 args->arg2 可能是一个指针或整型, 如果是指针, 里层函数应该调用 ua_to_ka 将其转换一次
     */
    ret = vfs_ioctl((int)args->arg0, (int)args->arg1, args->arg2);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_opendir
** Descriptions:            opendir 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          目录指针
*********************************************************************************************************/
static DIR *do_opendir(syscall_args_t *args)
{
    DIR *ret;

    sys_do_enter();

    ret = vfs_opendir(ua_to_ka(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_closedir
** Descriptions:            closedir 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_closedir(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_closedir(args->arg0);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_readdir
** Descriptions:            readdir 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          目录项
*********************************************************************************************************/
static struct dirent *do_readdir(syscall_args_t *args)
{
    struct dirent *ret;

    sys_do_enter();

    /*
     * 这个函数返回的是内核指针, 进程是工作在 USR 模式的, 不能访问内核空间,
     *
     * 所以拷贝到进程提供的"缓冲区"里, 然后返回"缓冲区"的地址
     */
    ret = vfs_readdir(args->arg0);
#if CONFIG_PROCESS_EN > 0
    if (ret != NULL) {
        pinfo_t *info;
        reg_t reg;

        reg  = interrupt_disable();
        info = current->pinfo;
        interrupt_resume(reg);

        if (info != NULL) {
            memcpy(&info->entry, ret, sizeof(struct dirent));
            ret = ka_to_ua(&info->entry);
        }
    }
#endif

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_rewinddir
** Descriptions:            rewinddir 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_rewinddir(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_rewinddir(args->arg0);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_seekdir
** Descriptions:            seekdir 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_seekdir(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_seekdir(args->arg0, (long)args->arg1);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_telldir
** Descriptions:            telldir 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          当前读点
*********************************************************************************************************/
static long do_telldir(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_telldir(args->arg0);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_getcwd
** Descriptions:            getcwd 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          当前工作目录
*********************************************************************************************************/
static char *do_getcwd(syscall_args_t *args)
{
    char *ret;

    sys_do_enter();

    /*
     * 这个函数返回的是内核指针, 进程是工作在 USR 模式的, 不能访问内核空间,
     *
     * 所以拷贝到进程提供的"缓冲区"里, 然后返回"缓冲区"的地址
     */
    ret = vfs_getcwd(ua_to_ka(args->arg0), (size_t)args->arg1);
#if CONFIG_PROCESS_EN > 0
    if (ret != NULL && args->arg0 == NULL) {
        reg_t reg;
        pinfo_t *info;

        reg  = interrupt_disable();
        info = current->pinfo;
        interrupt_resume(reg);

        if (info != NULL) {
            strcpy(info->cwd, ret);
            free(ret);
            ret = ka_to_ua(info->cwd);
        }
    }
#endif
    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_chdir
** Descriptions:            chdir 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_chdir(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_chdir(ua_to_ka(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_rename
** Descriptions:            rename 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_rename(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_rename(ua_to_ka(args->arg0), ua_to_ka(args->arg1));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_unlink
** Descriptions:            unlink 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_unlink(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_unlink(ua_to_ka(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_link
** Descriptions:            link 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_link(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_link(ua_to_ka(args->arg0), ua_to_ka(args->arg1));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_stat
** Descriptions:            stat 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_stat(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_stat(ua_to_ka(args->arg0), ua_to_ka(args->arg1));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_mkdir
** Descriptions:            mkdir 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_mkdir(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_mkdir(ua_to_ka(args->arg0), (mode_t)args->arg1);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_rmdir
** Descriptions:            rmdir 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_rmdir(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_rmdir(ua_to_ka(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_dup
** Descriptions:            dup 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          新的文件描述符 OR -1
*********************************************************************************************************/
static int do_dup(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_dup((int)args->arg0);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_dup2
** Descriptions:            dup2 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_dup2(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_dup2((int)args->arg0, (int)args->arg1);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_select
** Descriptions:            select 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_select(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_select(
            (int)args->arg0,
            ua_to_ka(args->arg1),
            ua_to_ka(args->arg2),
            ua_to_ka(args->arg3),
            ua_to_ka(args->arg4));

    sys_do_exit();
    return ret;
}
/*********************************************************************************************************
** Function name:           do_pipe
** Descriptions:            pipe 系统调用处理函数
** input parameters:        args                系统调用处理参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_pipe(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = pipe_create(ua_to_ka(args->arg0));

    sys_do_exit();
    return ret;
}
#endif
/*********************************************************************************************************
** 网络相关系统调用处理
*********************************************************************************************************/
#if CONFIG_NET_EN > 0

#include <sys/socket.h>
#include "drivers/socket.h"

/*
 * do_socket
 */
static int do_socket(syscall_args_t *args)
{
    int sock_fd;

    sys_do_enter();

    sock_fd = lwip_socket((int)args->arg0, (int)args->arg1, (int)args->arg2);
    if (sock_fd >= 0) {
        int fd = socket_attach(sock_fd);
        if (fd < 0) {
            lwip_close(sock_fd);
            sys_do_exit();
            return -1;
        }
        sys_do_exit();
        return fd;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_bind
 */
static int do_bind(syscall_args_t *args)
{
    int ret;
    void *ctx;

    sys_do_enter();

    int sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_bind(sock_fd, ua_to_ka(args->arg1), (socklen_t)args->arg2);
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_accept
 */
static int do_accept(syscall_args_t *args)
{
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        int new_sock_fd = lwip_accept(sock_fd, ua_to_ka(args->arg1), ua_to_ka(args->arg2));
        if (new_sock_fd >= 0) {
            int fd = socket_attach(new_sock_fd);
            if (fd < 0) {
                lwip_close(new_sock_fd);
                socket_op_end(ctx);
                sys_do_exit();
                return -1;
            }
            socket_op_end(ctx);
            sys_do_exit();
            return fd;
        } else {
            socket_op_end(ctx);
            sys_do_exit();
            return -1;
        }
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_connect
 */
static int do_connect(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_connect(sock_fd, ua_to_ka(args->arg1), (socklen_t)args->arg2);
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_listen
 */
static int do_listen(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_listen(sock_fd, (int)args->arg1);
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}


/*
 * do_recv
 */
static int do_recv(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_recv(
                sock_fd,
                ua_to_ka(args->arg1),
                (size_t)args->arg2,
                (int)args->arg3);
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_recvfrom
 */
static int do_recvfrom(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_recvfrom(
                sock_fd,
                ua_to_ka(args->arg1),
                (size_t)args->arg2,
                (int)args->arg3,
                (struct sockaddr *)ua_to_ka(args->arg4),
                (socklen_t *)ua_to_ka(args->arg5));
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_sendto
 */
static int do_sendto(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_sendto(
                sock_fd,
                ua_to_ka(args->arg1),
                (size_t)args->arg2,
                (int)args->arg3,
                (const struct sockaddr *)ua_to_ka(args->arg4),
                (socklen_t)args->arg5);
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_getsockopt
 */
static int do_getsockopt(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_getsockopt(
                sock_fd,
                (int)args->arg1,
                (int)args->arg2,
                ua_to_ka(args->arg3),
                (socklen_t *)ua_to_ka(args->arg4));
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_send
 */
static int do_send(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_send(
                sock_fd,
                ua_to_ka(args->arg1),
                (size_t)args->arg2,
                (int)args->arg3);
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_setsockopt
 */
static int do_setsockopt(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_setsockopt(
                sock_fd,
                (int)args->arg1,
                (int)args->arg2,
                ua_to_ka(args->arg3),
                (socklen_t)args->arg4);
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_getsockname
 */
static int do_getsockname(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_getsockname(
                sock_fd,
                ua_to_ka(args->arg1),
                ua_to_ka(args->arg2));
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_getpeername
 */
static int do_getpeername(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_getpeername(
                sock_fd,
                ua_to_ka(args->arg1),
                ua_to_ka(args->arg2));
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_shutdown
 */
static int do_shutdown(syscall_args_t *args)
{
    int ret;
    int sock_fd;
    void *ctx;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0, &ctx);
    if (sock_fd >= 0) {
        ret = lwip_shutdown(sock_fd, (int)args->arg1);
        socket_op_end(ctx);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

#include "lwip/netdb.h"

/*
 * do_gethostbyname
 */
static struct hostent *do_gethostbyname(syscall_args_t *args)
{
    struct hostent *ret;

    sys_do_enter();

    ret = lwip_gethostbyname(ua_to_ka(args->arg0));

    sys_do_exit();

    return ret;
}

/*
 * do_gethostbyname_r
 */
static int do_gethostbyname_r(syscall_args_t *args)
{
    const char *name;
    struct hostent *ret;
    char *buf;
    size_t buflen;
    struct hostent **result;
    int *h_errnop;
    int _ret;

    sys_do_enter();

    name     = ua_to_ka(args->arg0);
    ret      = ua_to_ka(args->arg1);
    buf      = ua_to_ka(args->arg2);
    buflen   = (size_t)args->arg3;
    result   = ua_to_ka(args->arg4);
    h_errnop = ua_to_ka(args->arg5);

    _ret = lwip_gethostbyname_r(name, ret, buf, buflen, result, h_errnop);

    sys_do_exit();

    return _ret;
}

/*
 * do_freeaddrinfo
 */
static void do_freeaddrinfo(syscall_args_t *args)
{
    sys_do_enter();

    lwip_freeaddrinfo(ua_to_ka(args->arg0));

    sys_do_exit();
}

/*
 * do_getaddrinfo
 */
static int do_getaddrinfo(syscall_args_t *args)
{
    int ret;
    const char *nodename;
    const char *servname;
    const struct addrinfo *hints;
    struct addrinfo **res;

    sys_do_enter();

    nodename = ua_to_ka(args->arg0);
    servname = ua_to_ka(args->arg1);
    hints    = ua_to_ka(args->arg2);
    res      = ua_to_ka(args->arg3);

    ret = lwip_getaddrinfo(nodename, servname, hints, res);

    sys_do_exit();

    return ret;
}
#endif

static void  *do_dlopen(syscall_args_t *args)
{
    const char *path;
    int mode;
    void *ret;

    sys_do_enter();

    path = ua_to_ka(args->arg0);
    mode = (int)args->arg1;

    ret = module_open(path, mode);

    sys_do_exit();

    return ret;
}

static void  *do_dlsym(syscall_args_t *args)
{
    void *mod;
    const char *name;
    void *ret;

    sys_do_enter();

    mod = args->arg0;
    name = ua_to_ka(args->arg1);

    ret = module_symbol(mod, name);

    sys_do_exit();

    return ret;
}

static int    do_dlclose(syscall_args_t *args)
{
    void *mod;
    int ret;

    sys_do_enter();

    mod = args->arg0;

    ret = module_close(mod);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
  系统调用处理表
*********************************************************************************************************/
sys_do_t sys_do_table[] = {
        /*
         * do_xxx 的位置必须要和  SYSCALL_XXX 的值一致
         */
#define  SYSCALL_EXIT       0
#define  SYSCALL_MSLEEP     1
#define  SYSCALL_SCHEDULE   2
#define  SYSCALL_PAUSE      3
        (sys_do_t)do_exit,
        (sys_do_t)do_msleep,
        (sys_do_t)do_schedule,
#if CONFIG_SIGNAL_EN > 0
        (sys_do_t)do_pause,
#else
        NULL,
#endif
#define  SYSCALL_BAD        4
        (sys_do_t)do_bad,
#define  SYSCALL_SIGNAL     5
#define  SYSCALL_SIGPROCMASK 6
#define  SYSCALL_SIGSUSPEND 7
#if CONFIG_SIGNAL_EN > 0
        (sys_do_t)do_signal,
        (sys_do_t)do_sigprocmask,
        (sys_do_t)do_sigsuspend,
#else
        NULL,
        NULL,
        NULL,
#endif
        NULL,
        NULL,
#define  SYSCALL_GETTIME    10
#define  SYSCALL_GETPID     11
#define  SYSCALL_SETPINFO   12
#define  SYSCALL_KILL       13
#define  SYSCALL_ALARM      14
        (sys_do_t)do_gettimeofday,
        (sys_do_t)do_getpid,
        (sys_do_t)do_setpinfo,
        (sys_do_t)do_kill,
#if CONFIG_SIGNAL_EN > 0
        (sys_do_t)do_alarm,
#else
        NULL,
#endif
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#define  SYSCALL_OPEN       20
#define  SYSCALL_READ       21
#define  SYSCALL_WRITE      22
#define  SYSCALL_FCNTL      23
#define  SYSCALL_ISATTY     24
#define  SYSCALL_FSTAT      25
#define  SYSCALL_LSEEK      26
#define  SYSCALL_CLOSE      27
#define  SYSCALL_IOCTL      28
#define  SYSCALL_SELECT     29
        (sys_do_t)do_open,
        (sys_do_t)do_read,
        (sys_do_t)do_write,
        (sys_do_t)do_fcntl,
        (sys_do_t)do_isatty,
        (sys_do_t)do_fstat,
        (sys_do_t)do_lseek,
        (sys_do_t)do_close,
        (sys_do_t)do_ioctl,
        (sys_do_t)do_select,
#define  SYSCALL_RENAME     30
#define  SYSCALL_UNLINK     31
#define  SYSCALL_LINK       32
#define  SYSCALL_STAT       33
#define  SYSCALL_MKDIR      34
#define  SYSCALL_RMDIR      35
#define  SYSCALL_DUP        36
#define  SYSCALL_DUP2       37
#define  SYSCALL_PIPE       38
        (sys_do_t)do_rename,
        (sys_do_t)do_unlink,
        (sys_do_t)do_link,
        (sys_do_t)do_stat,
        (sys_do_t)do_mkdir,
        (sys_do_t)do_rmdir,
        (sys_do_t)do_dup,
        (sys_do_t)do_dup2,
        (sys_do_t)do_pipe,
        NULL,
#define  SYSCALL_OPENDIR    40
#define  SYSCALL_READDIR    41
#define  SYSCALL_SEEKDIR    42
#define  SYSCALL_REWINDDIR  43
#define  SYSCALL_TELLDIR    44
#define  SYSCALL_CLOSEDIR   45
        (sys_do_t)do_opendir,
        (sys_do_t)do_readdir,
        (sys_do_t)do_seekdir,
        (sys_do_t)do_rewinddir,
        (sys_do_t)do_telldir,
        (sys_do_t)do_closedir,
        NULL,
        NULL,
        NULL,
        NULL,
#define  SYSCALL_GETCWD     50
#define  SYSCALL_CHDIR      51
        (sys_do_t)do_getcwd,
        (sys_do_t)do_chdir,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#define  SYSCALL_SOCKET     60
#define  SYSCALL_BIND       61
#define  SYSCALL_ACCEPT     62
#define  SYSCALL_CONNECT    63
#define  SYSCALL_LISTEN     64
#define  SYSCALL_RECV       65
#define  SYSCALL_RECVFROM   66
#define  SYSCALL_SENDTO     67
#define  SYSCALL_GETSOCKOPT 68
#define  SYSCALL_SEND       69
#define  SYSCALL_SHUTDOWN   70
#define  SYSCALL_SETSOCKOPT 71
#define  SYSCALL_GETSOCKNAME 72
#define  SYSCALL_GETPEERNAME 73
#define  SYSCALL_GETHOSTBYNAME   74
#define  SYSCALL_GETHOSTBYNAME_R 75
#define  SYSCALL_FREEADDRINFO    76
#define  SYSCALL_GETADDRINFO     77
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
        (sys_do_t)do_getsockname,
        (sys_do_t)do_getpeername,
        (sys_do_t)do_gethostbyname,
        (sys_do_t)do_gethostbyname_r,
        (sys_do_t)do_freeaddrinfo,
        (sys_do_t)do_getaddrinfo,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (sys_do_t)do_dlopen,
        (sys_do_t)do_dlsym,
        (sys_do_t)do_dlclose,
};
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
