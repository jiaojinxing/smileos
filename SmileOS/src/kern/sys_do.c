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
#include "kern/kern.h"
#include "kern/kvars.h"
#include "vfs/vfs.h"
#include <sys/socket.h>
#include <dirent.h>
#include <string.h>
#include "kern/pinfo.h"
#include <signal.h>
/*********************************************************************************************************
** Function name:           sys_do_enter
** Descriptions:            ����ϵͳ���ô���
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void sys_do_enter(void)
{
    /*
     * TODO: Ϊ�������Ӧ�ó���ʱ GDB �����ں�, ����Ӧ�ü���һЩ����
     */
}
/*********************************************************************************************************
** Function name:           sys_do_exit
** Descriptions:            �˳�ϵͳ���ô���
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void sys_do_exit(void)
{
}
/*********************************************************************************************************
** Function name:           do_exit
** Descriptions:            exit ϵͳ���ô�����
** input parameters:        args                ϵͳ���ô������
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void do_exit(syscall_args_t *args)
{
    sys_do_enter();

    kill(gettid(), SIGKILL);

    sys_do_exit();
}
/*********************************************************************************************************
** Function name:           do_msleep
** Descriptions:            usleep ϵͳ���ô�����
** input parameters:        args                ϵͳ���ô������
** output parameters:       NONE
** Returned value:          0
*********************************************************************************************************/
static int do_msleep(syscall_args_t *args)
{
    uint32_t timeout;
    int ret;

    sys_do_enter();

    timeout = (uint32_t)args->arg0;
    if (timeout != 0) {
        if (timeout < 1000 / TICK_PER_SECOND) {
            timeout = 1;
        } else {
            timeout = timeout * TICK_PER_SECOND / 1000;
        }
        extern int task_sleep(uint32_t ticks);
        ret = task_sleep(timeout);
    } else {
        ret = 0;
    }

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_pause
** Descriptions:            pause ϵͳ���ô�����
** input parameters:        args                ϵͳ���ô������
** output parameters:       NONE
** Returned value:          0
*********************************************************************************************************/
static int do_pause(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    extern int task_pause(void);
    ret = task_pause();

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_gettimeofday
** Descriptions:            gettimeofday ϵͳ���ô�����
** input parameters:        args                ϵͳ���ô������
** output parameters:       tv                  ʱ��ֵ
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_gettimeofday(syscall_args_t *args)
{
    struct timeval *tv = args->arg0;
    //void *tzp = args->arg1;
    uint64_t tick;

    sys_do_enter();

    tick = getticks();

    tv->tv_sec  = (tick / TICK_PER_SECOND);
    tv->tv_usec = (tick % TICK_PER_SECOND) * 1000000 / TICK_PER_SECOND;

    sys_do_exit();

    return 0;
}
/*********************************************************************************************************
** Function name:           do_getpid
** Descriptions:            getpid ϵͳ���ô�����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          PID
*********************************************************************************************************/
static int do_getpid(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    extern int32_t task_getpid(void);
    ret = task_getpid();

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_schedule
** Descriptions:            schedule ϵͳ���ô�����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int do_schedule(syscall_args_t *args)
{
    sys_do_enter();

    extern void task_schedule(void);
    task_schedule();

    sys_do_exit();

    return 0;
}
/*********************************************************************************************************
** Function name:           do_bad
** Descriptions:            ����ϵͳ���ô�����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int do_bad(void)
{
    sys_do_enter();

    kill(gettid(), SIGSYS);

    sys_do_exit();

    return -1;
}
/*********************************************************************************************************
** Function name:           do_signal
** Descriptions:            ����ϵͳ���ô�����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static sighandler_t do_signal(syscall_args_t *args)
{
    sighandler_t ret;

    sys_do_enter();

    extern sighandler_t signal_signal(int sig, sighandler_t func);
    ret = signal_signal((int)args->arg0, (sighandler_t)args->arg1);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_signal
** Descriptions:            ����ϵͳ���ô�����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int do_sigprocmask(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    extern int signal_procmask(int how, const sigset_t *set, sigset_t *oset);
    ret = signal_procmask((int)args->arg0,
            (const sigset_t *)va_to_mva(args->arg1),
            (sigset_t *)va_to_mva(args->arg2));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_signal
** Descriptions:            ����ϵͳ���ô�����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int do_sigsuspend(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    extern int signal_suspend(const sigset_t *set);
    ret = signal_suspend((const sigset_t *)va_to_mva(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_setpinfo
** Descriptions:            setpinfo ϵͳ���ô�����
** input parameters:        reent               ������ṹָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_setpinfo(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    extern int task_setpinfo(pinfo_t *pinfo);
    ret = task_setpinfo(va_to_mva(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_kill
** Descriptions:            kill ϵͳ���ô�����
** input parameters:        pid                 PID
**                          sig                 �ź�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_kill(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    extern int signal_queue(int32_t tid, int sig);
    ret = signal_queue((int32_t)args->arg0, (int)args->arg1);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_alarm
** Descriptions:            alarm ϵͳ���ô�����
** input parameters:        pid                 PID
**                          sig                 �ź�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_alarm(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    extern int task_alarm(uint32_t ms);
    ret = task_alarm((uint32_t)args->arg0);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_open
** Descriptions:            open ϵͳ���ô�����
** input parameters:        path                �ļ�·��
**                          oflag               ��־
**                          mode                ģʽ
** output parameters:       NULL
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_open(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_open(va_to_mva(args->arg0), (int)args->arg1, (mode_t)args->arg2);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_read
** Descriptions:            read ϵͳ���ô�����
** input parameters:        fd                  �ļ�������
**                          buf                 ���ݻ�����
**                          len                 �³���
** output parameters:       buf                 ����
** Returned value:          �ɹ���ȡ���ֽ��� OR -1
*********************************************************************************************************/
static ssize_t do_read(syscall_args_t *args)
{
    ssize_t ret;

    sys_do_enter();

    ret = vfs_read((int)args->arg0, va_to_mva(args->arg1), (size_t)args->arg2);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_write
** Descriptions:            write ϵͳ���ô�����
** input parameters:        fd                  �ļ�������
**                          buf                 ���ݻ�����
**                          len                 �³���
** output parameters:       NONE
** Returned value:          �ɹ�д����ֽ��� OR -1
*********************************************************************************************************/
static ssize_t do_write(syscall_args_t *args)
{
    ssize_t ret;

    sys_do_enter();

    ret = vfs_write((int)args->arg0, va_to_mva(args->arg1), (size_t)args->arg2);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_fcntl
** Descriptions:            fcntl ϵͳ���ô�����
** input parameters:        fd                  �ļ�������
**                          cmd                 ����
**                          arg                 ����
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
** Descriptions:            isatty ϵͳ���ô�����
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR 1
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
** Descriptions:            fstat ϵͳ���ô�����
** input parameters:        fd                  �ļ�������
** output parameters:       buf                 �ļ�״̬
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_fstat(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_fstat((int)args->arg0, va_to_mva(args->arg1));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_lseek
** Descriptions:            lseek ϵͳ���ô�����
** input parameters:        fd                  �ļ�������
**                          offset              ƫ��
**                          whence              ������λ��
** output parameters:       NONE
** Returned value:          �µĶ�дλ�� OR -1
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
** Descriptions:            close ϵͳ���ô�����
** input parameters:        fd                  �ļ�������
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
** Descriptions:            ioctl ϵͳ���ô�����
** input parameters:        fd                  �ļ�������
**                          cmd                 ����
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_ioctl(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    /*
     * ע�� args->arg2 ������һ��ָ�������, �����ָ��, ��㺯��Ӧ�õ��� va_to_mva ����ת��һ��
     */
    ret = vfs_ioctl((int)args->arg0, (int)args->arg1, args->arg2);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_opendir
** Descriptions:            opendir ϵͳ���ô�����
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          Ŀ¼ָ��
*********************************************************************************************************/
static DIR *do_opendir(syscall_args_t *args)
{
    DIR *ret;

    sys_do_enter();

    ret = vfs_opendir(va_to_mva(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_closedir
** Descriptions:            closedir ϵͳ���ô�����
** input parameters:        dir                 Ŀ¼ָ��
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
** Descriptions:            readdir ϵͳ���ô�����
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          Ŀ¼��
*********************************************************************************************************/
static struct dirent *do_readdir(syscall_args_t *args)
{
    struct dirent *ret;

    sys_do_enter();

    /*
     * ����������ص����ں�ָ��, �����ǹ����� USR ģʽ��, ���ܷ����ں˿ռ�,
     *
     * ���Կ����������ṩ��"������"��, Ȼ�󷵻�"������"�ĵ�ַ
     */
    ret = vfs_readdir(args->arg0);
    if (ret != NULL) {
        uint32_t reg;
        pinfo_t *info;

        reg  = interrupt_disable();
        info = current->pinfo;
        if (info != NULL) {
            memcpy(&info->entry, ret, sizeof(struct dirent));
            ret = mva_to_va(&info->entry);
        }
        interrupt_resume(reg);
    }

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_rewinddir
** Descriptions:            rewinddir ϵͳ���ô�����
** input parameters:        dir                 Ŀ¼ָ��
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
** Descriptions:            seekdir ϵͳ���ô�����
** input parameters:        dir                 Ŀ¼ָ��
**                          loc                 �µĶ���
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
** Descriptions:            telldir ϵͳ���ô�����
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          ��ǰ����
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
** Descriptions:            getcwd ϵͳ���ô�����
** input parameters:        size                buf ��С
**                          buf                 ��ǰ����Ŀ¼����
** output parameters:       buf                 ��ǰ����Ŀ¼
** Returned value:          ��ǰ����Ŀ¼
*********************************************************************************************************/
static char *do_getcwd(syscall_args_t *args)
{
    char *ret;

    sys_do_enter();

    /*
     * ����������ص����ں�ָ��, �����ǹ����� USR ģʽ��, ���ܷ����ں˿ռ�,
     *
     * ���Կ����������ṩ��"������"��, Ȼ�󷵻�"������"�ĵ�ַ
     */
    ret = vfs_getcwd(va_to_mva(args->arg0), (size_t)args->arg1);
    if (ret != NULL) {
        uint32_t reg;
        pinfo_t *info;

        reg  = interrupt_disable();
        info = current->pinfo;
        if (info != NULL) {
            strcpy(info->cwd, ret);
            ret = mva_to_va(info->cwd);
        }
        interrupt_resume(reg);
    }
    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_chdir
** Descriptions:            chdir ϵͳ���ô�����
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_chdir(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_chdir(va_to_mva(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_rename
** Descriptions:            rename ϵͳ���ô�����
** input parameters:        old                 Դ�ļ� PATH
**                          _new                ���ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_rename(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_rename(va_to_mva(args->arg0), va_to_mva(args->arg1));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_unlink
** Descriptions:            unlink ϵͳ���ô�����
** input parameters:        path                �ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_unlink(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_unlink(va_to_mva(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_link
** Descriptions:            link ϵͳ���ô�����
** input parameters:        path1               �ļ� PATH
**                          path2               ���ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_link(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_link(va_to_mva(args->arg0), va_to_mva(args->arg1));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_stat
** Descriptions:            stat ϵͳ���ô�����
** input parameters:        path                �ļ� PATH
**                          buf                 �ļ�״̬����
** output parameters:       buf                 �ļ�״̬
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_stat(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_stat(va_to_mva(args->arg0), va_to_mva(args->arg1));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_mkdir
** Descriptions:            mkdir ϵͳ���ô�����
** input parameters:        path                Ŀ¼ PATH
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_mkdir(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_mkdir(va_to_mva(args->arg0), (mode_t)args->arg1);

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_rmdir
** Descriptions:            rmdir ϵͳ���ô�����
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int do_rmdir(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_rmdir(va_to_mva(args->arg0));

    sys_do_exit();

    return ret;
}
/*********************************************************************************************************
** Function name:           do_dup
** Descriptions:            dup ϵͳ���ô�����
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          �µ��ļ������� OR -1
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
** Descriptions:            dup2 ϵͳ���ô�����
** input parameters:        fd                  �ļ�������
**                          to                  ָ�����ļ�������
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

    sys_do_enter();

    int sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        ret = lwip_bind(sock_fd, va_to_mva(args->arg1), (socklen_t)args->arg2);
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

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        int new_sock_fd = lwip_accept(sock_fd, va_to_mva(args->arg1), va_to_mva(args->arg2));
        if (new_sock_fd >= 0) {
            int fd = socket_attach(new_sock_fd);
            if (fd < 0) {
                lwip_close(new_sock_fd);
                sys_do_exit();
                return -1;
            }
            sys_do_exit();
            return fd;
        } else {
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

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        ret = lwip_connect(sock_fd, va_to_mva(args->arg1), (socklen_t)args->arg2);
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

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        ret = lwip_listen(sock_fd, (int)args->arg1);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}

/*
 * do_select
 */
static int do_select(syscall_args_t *args)
{
    int ret;

    sys_do_enter();

    ret = vfs_select(
            (int)args->arg0,
            va_to_mva(args->arg1),
            va_to_mva(args->arg2),
            va_to_mva(args->arg3),
            va_to_mva(args->arg4));

    sys_do_exit();
    return ret;
}

/*
 * do_recv
 */
static int do_recv(syscall_args_t *args)
{
    int ret;
    int sock_fd;

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        ret = lwip_recv(
                sock_fd,
                va_to_mva(args->arg1),
                (size_t)args->arg2,
                (int)args->arg3);
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

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        ret = lwip_recvfrom(
                sock_fd,
                va_to_mva(args->arg1),
                (size_t)args->arg2,
                (int)args->arg3,
                (struct sockaddr *)va_to_mva(args->arg4),
                (socklen_t *)va_to_mva(args->arg5));
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

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        ret = lwip_sendto(
                sock_fd,
                va_to_mva(args->arg1),
                (size_t)args->arg2,
                (int)args->arg3,
                (const struct sockaddr *)va_to_mva(args->arg4),
                (socklen_t)args->arg5);
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

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        ret = lwip_getsockopt(
                sock_fd,
                (int)args->arg1,
                (int)args->arg2,
                va_to_mva(args->arg3),
                (socklen_t *)va_to_mva(args->arg4));
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

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        ret = lwip_send(
                sock_fd,
                va_to_mva(args->arg1),
                (size_t)args->arg2,
                (int)args->arg3);
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

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        ret = lwip_setsockopt(
                sock_fd,
                (int)args->arg1,
                (int)args->arg2,
                va_to_mva(args->arg3),
                (socklen_t)args->arg4);
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

    sys_do_enter();

    sock_fd = socket_priv_fd((int)args->arg0);
    if (sock_fd >= 0) {
        ret = lwip_shutdown(sock_fd, (int)args->arg1);
        sys_do_exit();
        return ret;
    } else {
        sys_do_exit();
        return -1;
    }
}
/*********************************************************************************************************
  ϵͳ���ô����
*********************************************************************************************************/
sys_do_t sys_do_table[] = {
        /*
         * do_xxx ��λ�ñ���Ҫ��  SYSCALL_XXX ��ֵһ��
         */
#define  SYSCALL_EXIT       0
#define  SYSCALL_MSLEEP     1
#define  SYSCALL_SCHEDULE   2
#define  SYSCALL_PAUSE      3
        (sys_do_t)do_exit,
        (sys_do_t)do_msleep,
        (sys_do_t)do_schedule,
        (sys_do_t)do_pause,
#define  SYSCALL_BAD        4
        (sys_do_t)do_bad,
#define  SYSCALL_SIGNAL     5
#define  SYSCALL_SIGPROCMASK 6
#define  SYSCALL_SIGSUSPEND 7
        (sys_do_t)do_signal,
        (sys_do_t)do_sigprocmask,
        (sys_do_t)do_sigsuspend,
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
        (sys_do_t)do_alarm,
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
        (sys_do_t)do_rename,
        (sys_do_t)do_unlink,
        (sys_do_t)do_link,
        (sys_do_t)do_stat,
        (sys_do_t)do_mkdir,
        (sys_do_t)do_rmdir,
        (sys_do_t)do_dup,
        (sys_do_t)do_dup2,
        NULL,
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
** END FILE
*********************************************************************************************************/
