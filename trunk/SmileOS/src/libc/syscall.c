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
** File name:               syscall.c
** Last modified Date:      2012-2-22
** Last Version:            1.0.0
** Descriptions:            ϵͳ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-22
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
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <reent.h>
#include "kern/pinfo.h"
/*********************************************************************************************************
** ����
*********************************************************************************************************/
struct stat;
struct tms;
struct timeval;
/*********************************************************************************************************
** �ں˶���
*********************************************************************************************************/
#ifdef SMILEOS_KERNEL

#include "kern/kern.h"
#include "arch/if.h"

extern sys_do_t             sys_do_table[];

#define debug(...)

/*
 * �ں���ʹ�� gettid
 */
#define getpid              gettid

#else
/*********************************************************************************************************
** �û��ռ������
*********************************************************************************************************/
typedef int (*sys_do_t)();

/*
 * ϵͳ���ô������
 */
typedef struct {
    void *arg0;
    void *arg1;
    void *arg2;
    void *arg3;
    void *arg4;
    void *arg5;
    void *arg6;
    void *arg7;
    void *arg8;
    void *arg9;
} syscall_args_t;

static sys_do_t             sys_do_table[1];

#define arch_in_kernel()    0

#define debug(...)

#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

#define fork            syscall_fork
#define waitpid         syscall_waitpid
#define system          syscall_system
#define nanosleep       syscall_nanosleep
#define usleep          syscall_usleep
#define sleep           syscall_sleep
#define sigprocmask     syscall_sigmask
#define sigwait         syscall_sigwait
#define select          syscall_select
#define pselect         syscall_pselect
//#define poll            syscall_poll
#define connect         syscall_connect
#define accept          syscall_accept
#define read            syscall_read
#define write           syscall_write
#define readv           syscall_readv
#define writev          syscall_writev
#define recv            syscall_recv
#define send            syscall_send
#define recvfrom        syscall_recvfrom
#define sendto          syscall_sendto
#define pread           syscall_pread
#define pwrite          syscall_pwrite

#endif
/*********************************************************************************************************
** newlib ��Ҫ���µ�׮����֧��
*********************************************************************************************************/
extern int _close_r _PARAMS ((struct _reent *, int));
extern int _execve_r _PARAMS ((struct _reent *, const char *, char *const *, char *const *));
extern int _fcntl_r _PARAMS ((struct _reent *, int, int, int));
extern int _fork_r _PARAMS ((struct _reent *));
extern int _fstat_r _PARAMS ((struct _reent *, int, struct stat *));
extern int _getpid_r _PARAMS ((struct _reent *));
extern int _isatty_r _PARAMS ((struct _reent *, int));
extern int _kill_r _PARAMS ((struct _reent *, int, int));
extern int _link_r _PARAMS ((struct _reent *, const char *, const char *));
extern _off_t _lseek_r _PARAMS ((struct _reent *, int, _off_t, int));
extern int _mkdir_r _PARAMS ((struct _reent *, const char *, int));
extern int _open_r _PARAMS ((struct _reent *, const char *, int, int));
extern _ssize_t _read_r _PARAMS ((struct _reent *, int, void *, size_t));
extern int _rename_r _PARAMS ((struct _reent *, const char *, const char *));
extern void *_sbrk_r _PARAMS ((struct _reent *, ptrdiff_t));
extern int _stat_r _PARAMS ((struct _reent *, const char *, struct stat *));
extern _CLOCK_T_ _times_r _PARAMS ((struct _reent *, struct tms *));
extern int _unlink_r _PARAMS ((struct _reent *, const char *));
extern int _wait_r _PARAMS ((struct _reent *, int *));
extern _ssize_t _write_r _PARAMS ((struct _reent *, int, const void *, size_t));
/*********************************************************************************************************
** ϵͳ���ú�
*********************************************************************************************************/
#define  SYSCALL_EXIT       0
#define  SYSCALL_MSLEEP     1
#define  SYSCALL_SCHEDULE   2
#define  SYSCALL_PAUSE      3
#define  SYSCALL_BAD        4
#define  SYSCALL_SIGNAL     5
#define  SYSCALL_SIGPROCMASK 6
#define  SYSCALL_SIGSUSPEND 7
#define  SYSCALL_GETTIME    10
#define  SYSCALL_GETPID     11
#define  SYSCALL_SETPINFO   12
#define  SYSCALL_KILL       13
#define  SYSCALL_ALARM      14
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
#define  SYSCALL_RENAME     30
#define  SYSCALL_UNLINK     31
#define  SYSCALL_LINK       32
#define  SYSCALL_STAT       33
#define  SYSCALL_MKDIR      34
#define  SYSCALL_RMDIR      35
#define  SYSCALL_DUP        36
#define  SYSCALL_DUP2       37
#define  SYSCALL_OPENDIR    40
#define  SYSCALL_READDIR    41
#define  SYSCALL_SEEKDIR    42
#define  SYSCALL_REWINDDIR  43
#define  SYSCALL_TELLDIR    44
#define  SYSCALL_CLOSEDIR   45
#define  SYSCALL_GETCWD     50
#define  SYSCALL_CHDIR      51
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
#define  SYSCALL_NR         100                                         /*  ϵͳ������                  */
/*********************************************************************************************************
** ϵͳ���ô���
*********************************************************************************************************/
#define syscall_code()                                  \
        if (arch_in_kernel()) {                         \
            return (sys_do_table[syscall])(&args);      \
        } else {                                        \
            return arch_kernel_enter(&args, syscall);   \
        }
/*********************************************************************************************************
** Function name:           _exit
** Descriptions:            exit ׮����
** input parameters:        status              �˳�״̬��
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void _exit(int status)
{
    int syscall = SYSCALL_EXIT;
    syscall_args_t args = {(void *)status};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           msleep
** Descriptions:            ����ָ���ĺ�����
** input parameters:        mseconds            ������
** output parameters:       NONE
** Returned value:          0 OR -1
**
** ��Ϊϵͳ����Ǻ��뼶, �����ṩ��ϵͳ����
**
*********************************************************************************************************/
int msleep(unsigned int mseconds)
{
    int syscall = SYSCALL_MSLEEP;
    syscall_args_t args = {(void *)mseconds};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           pause
** Descriptions:            ֹͣ����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int pause(void)
{
    int syscall = SYSCALL_PAUSE;
    syscall_args_t args;

    syscall_code();
}
/*********************************************************************************************************
** Function name:           alarm
** Descriptions:            ��ʱ
** input parameters:        secs                �������������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
unsigned alarm(unsigned secs)
{
    int syscall = SYSCALL_ALARM;
    syscall_args_t args = {(void *)secs};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           usleep
** Descriptions:            ����ָ����΢����
** input parameters:        useconds            ΢����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int usleep(useconds_t useconds)
{
    return msleep(useconds / 1000);
}
#ifndef SMILEOS_KERNEL
#undef usleep
int usleep(useconds_t useconds)
{
    return __pthread_usleep(useconds);
}
#endif
/*********************************************************************************************************
** Function name:           sleep
** Descriptions:            ����ָ��������
** input parameters:        seconds             ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
unsigned sleep(unsigned int seconds)
{
    return msleep(1000 * seconds);
}
#ifndef SMILEOS_KERNEL
#undef sleep
unsigned sleep(unsigned int seconds)
{
    return __pthread_sleep(seconds);
}
#endif
/*********************************************************************************************************
** Function name:           schedule
** Descriptions:            ���µ���
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int schedule(void)
{
    int syscall = SYSCALL_SCHEDULE;
    syscall_args_t args;

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _gettimeofday_r
** Descriptions:            gettimeofday ׮����
** input parameters:        reent               ������ṹ
** output parameters:       tv                  ʱ��ֵ
** Returned value:          0 OR -1
*********************************************************************************************************/
int _gettimeofday_r(struct _reent *reent, struct timeval *tv, void *tzp)
{
    int syscall = SYSCALL_GETTIME;
    syscall_args_t args = {(void *)tv, (void *)tzp};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _close_r
** Descriptions:            close ׮����
** input parameters:        reent               ������ṹ
**                          fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int _close_r(struct _reent *reent, int fd)
{
    int syscall = SYSCALL_CLOSE;
    syscall_args_t args = {(void *)fd};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           ioctl
** Descriptions:            �����ļ�
** input parameters:        fd                  �ļ�������
**                          cmd                 ����
**                          ...                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int ioctl(int fd, int cmd, ...)
{
    int syscall = SYSCALL_IOCTL;
    syscall_args_t args = {(void *)fd, (void *)cmd};
    int arg;
    va_list va;

    va_start(va, cmd);
    arg = va_arg(va, int);
    va_end(va);

    args.arg2 = (void *)arg;

    syscall_code();
}
/*********************************************************************************************************
** Function name:           fcntl
** Descriptions:            �����ļ�
** input parameters:        fd                  �ļ�������
**                          cmd                 ����
**                          ...                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
#include <fcntl.h>

int fcntl(int fd, int cmd, ...)
{
    int syscall = SYSCALL_FCNTL;
    syscall_args_t args = {(void *)fd, (void *)cmd};
    int arg;
    va_list va;

    if (cmd == F_SETFL) {
        va_start(va, cmd);
        arg = va_arg(va, int);
        va_end(va);
    } else {
        arg = 0;
    }

    args.arg2 = (void *)arg;

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _fstat_r
** Descriptions:            fstat ׮����
** input parameters:        reent               ������ṹ
**                          fd                  �ļ�������
** output parameters:       buf                 �ļ�״̬
** Returned value:          0 OR -1
*********************************************************************************************************/
int _fstat_r(struct _reent *reent, int fd, struct stat *buf)
{
    int syscall = SYSCALL_FSTAT;
    syscall_args_t args = {(void *)fd, (void *)buf};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _getpid_r
** Descriptions:            getpid ׮����
** input parameters:        reent               ������ṹ
** output parameters:       NONE
** Returned value:          PID
*********************************************************************************************************/
int _getpid_r(struct _reent *reent)
{
    int syscall = SYSCALL_GETPID;
    syscall_args_t args;

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _isatty_r
** Descriptions:            isatty ׮����
** input parameters:        reent               ������ṹ
**                          fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR 1
*********************************************************************************************************/
int _isatty_r(struct _reent *reent, int fd)
{
    int syscall = SYSCALL_ISATTY;
    syscall_args_t args = {(void *)fd};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _link_r
** Descriptions:            link ׮����
** input parameters:        reent               ������ṹ
**                          path1               �ļ� PATH
**                          path2               ���ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int _link_r(struct _reent *reent, const char *path1, const char *path2)
{
    int syscall = SYSCALL_LINK;
    syscall_args_t args = {(void *)path1, (void *)path2};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _lseek_r
** Descriptions:            lseek ׮����
** input parameters:        reent               ������ṹ
**                          fd                  �ļ�������
**                          offset              ƫ��
**                          whence              ������λ��
** output parameters:       NONE
** Returned value:          �µĶ�дλ�� OR -1
*********************************************************************************************************/
_off_t _lseek_r(struct _reent *reent, int fd, _off_t offset, int whence)
{
    int syscall = SYSCALL_LSEEK;
    syscall_args_t args = {(void *)fd, (void *)offset, (void *)whence};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _mkdir_r
** Descriptions:            mkdir ׮����
** input parameters:        reent               ������ṹ
**                          path                Ŀ¼ PATH
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int _mkdir_r(struct _reent *reent, const char *path, int mode)
{
    int syscall = SYSCALL_MKDIR;
    syscall_args_t args = {(void *)path, (void *)mode};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _rmdir_r
** Descriptions:            rmdir ׮����
** input parameters:        reent               ������ṹ
**                          path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int _rmdir_r(struct _reent *reent, const char *path)
{
    int syscall = SYSCALL_RMDIR;
    syscall_args_t args = {(void *)path};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _open_r
** Descriptions:            open ׮����
** input parameters:        reent               ������ṹ
**                          path                �ļ�·��
**                          oflag               ��־
**                          mode                ģʽ
** output parameters:       NULL
** Returned value:          0 OR -1
*********************************************************************************************************/
int _open_r(struct _reent *reent, const char *path, int oflag, int mode)
{
    int syscall = SYSCALL_OPEN;
    syscall_args_t args = {(void *)path, (void *)oflag, (void *)mode};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _read_r
** Descriptions:            read ׮����
** input parameters:        reent               ������ṹ
**                          fd                  �ļ�������
**                          buf                 ���ݻ�����
**                          len                 �³���
** output parameters:       buf                 ����
** Returned value:          �ɹ���ȡ���ֽ��� OR -1
*********************************************************************************************************/
_ssize_t _read_r(struct _reent *reent, int fd, void *buf, size_t nbytes)
#ifndef SMILEOS_KERNEL
{
    return __pthread_read(fd, buf, nbytes);
}

_ssize_t read(int fd, void *buf, size_t nbytes)
#endif
{
    int syscall = SYSCALL_READ;
    syscall_args_t args = {(void *)fd, (void *)buf, (void *)nbytes};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _write_r
** Descriptions:            write ׮����
** input parameters:        reent               ������ṹ
**                          fd                  �ļ�������
**                          buf                 ���ݻ�����
**                          len                 �³���
** output parameters:       NONE
** Returned value:          �ɹ�д����ֽ��� OR -1
*********************************************************************************************************/
_ssize_t _write_r(struct _reent *reent, int fd, const void *buf, size_t nbytes)
#ifndef SMILEOS_KERNEL
{
    if (STDERR_FILENO != fd) {
        return __pthread_write(fd, buf, nbytes);
    } else {
        _ssize_t syscall_write(int fd, const void *buf, size_t nbytes);
        return syscall_write(fd, buf, nbytes);
    }
}

_ssize_t write(int fd, const void *buf, size_t nbytes)
#endif
{
    int syscall = SYSCALL_WRITE;
    syscall_args_t args = {(void *)fd, (void *)buf, (void *)nbytes};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _rename_r
** Descriptions:            rename ׮����
** input parameters:        reent               ������ṹ
**                          old                 Դ�ļ� PATH
**                          _new                ���ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int _rename_r(struct _reent *reent, const char *old, const char *new)
{
    int syscall = SYSCALL_RENAME;
    syscall_args_t args = {(void *)old, (void *)new};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _stat_r
** Descriptions:            stat ׮����
** input parameters:        reent               ������ṹ
**                          path                �ļ� PATH
**                          buf                 �ļ�״̬����
** output parameters:       buf                 �ļ�״̬
** Returned value:          0 OR -1
*********************************************************************************************************/
int _stat_r(struct _reent *reent, const char *path, struct stat *buf)
{
    int syscall = SYSCALL_STAT;
    syscall_args_t args = {(void *)path, (void *)buf};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _unlink_r
** Descriptions:            unlink ׮����
** input parameters:        reent               ������ṹ
**                          path                �ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int _unlink_r(struct _reent *reent, const char *path)
{
    int syscall = SYSCALL_UNLINK;
    syscall_args_t args = {(void *)path};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           dup
** Descriptions:            �����ļ�������
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          �µ��ļ������� OR -1
*********************************************************************************************************/
int dup(int fd)
{
    int syscall = SYSCALL_DUP;
    syscall_args_t args = {(void *)fd};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           dup2
** Descriptions:            �����ļ���������ָ�����ļ�������
** input parameters:        fd                  �ļ�������
**                          to                  ָ�����ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int dup2(int fd, int to)
{
    int syscall = SYSCALL_DUP2;
    syscall_args_t args = {(void *)fd, (void *)to};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           setpinfo
** Descriptions:            ���ý�����Ϣ
** input parameters:        info                ������Ϣ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int setpinfo(pinfo_t *info)
{
    int syscall = SYSCALL_SETPINFO;
    syscall_args_t args = {(void *)info};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           _kill_r
** Descriptions:            kill ׮����
** input parameters:        reent               ������ṹָ��
**                          pid                 PID
**                          sig                 �ź�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int _kill_r(struct _reent *reent, int pid, int sig)
{
    int syscall = SYSCALL_KILL;
    syscall_args_t args = {(void *)pid, (void *)sig};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           opendir
** Descriptions:            ��Ŀ¼
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          Ŀ¼ָ��
*********************************************************************************************************/
DIR *opendir(const char *path)
{
    int syscall = SYSCALL_OPENDIR;
    syscall_args_t args = {(void *)path};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           closedir
** Descriptions:            �ر�Ŀ¼
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int closedir(DIR *dir)
{
    int syscall = SYSCALL_CLOSEDIR;
    syscall_args_t args = {(void *)dir};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           readdir
** Descriptions:            ��Ŀ¼��
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          Ŀ¼�� OR NULL
*********************************************************************************************************/
struct dirent *readdir(DIR *dir)
{
    int syscall = SYSCALL_READDIR;
    syscall_args_t args = {(void *)dir};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           rewinddir
** Descriptions:            ����Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int rewinddir(DIR *dir)
{
    int syscall = SYSCALL_REWINDDIR;
    syscall_args_t args = {(void *)dir};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           rewinddir
** Descriptions:            ����Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
**                          loc                 �µĶ���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int seekdir(DIR *dir, long loc)
{
    int syscall = SYSCALL_SEEKDIR;
    syscall_args_t args = {(void *)dir, (void *)loc};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           telldir
** Descriptions:            ���Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          ��ǰ����
*********************************************************************************************************/
long telldir(DIR *dir)
{
    int syscall = SYSCALL_TELLDIR;
    syscall_args_t args = {(void *)dir};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           chdir
** Descriptions:            �ı䵱ǰ����Ŀ¼
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int chdir(const char *path)
{
    int syscall = SYSCALL_CHDIR;
    syscall_args_t args = {(void *)path};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           getcwd
** Descriptions:            ��õ�ǰ����Ŀ¼
** input parameters:        size                buf ��С
**                          buf                 ��ǰ����Ŀ¼����
** output parameters:       buf                 ��ǰ����Ŀ¼
** Returned value:          ��ǰ����Ŀ¼
*********************************************************************************************************/
char *getcwd(char *buf, size_t size)
{
    int syscall = SYSCALL_GETCWD;
    syscall_args_t args = {(void *)buf, (void *)size};

    syscall_code();
}
/*********************************************************************************************************
** Function name:           select
** Descriptions:            ����ǰ������뵽�ļ����ĵȴ��б�
** input parameters:        nfds                �ļ���������Χ
**                          readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** output parameters:       readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** Returned value:          �ļ����ܹ������˶��ٸ�λ OR -1
*********************************************************************************************************/
int select(int nfds, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
    int syscall = SYSCALL_SELECT;
    syscall_args_t args = {(void *)nfds, (void *)readset, (void *)writeset, (void *)exceptset, (void *)timeout};

    syscall_code();
}
#ifndef SMILEOS_KERNEL
#undef select
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
    return __pthread_select(maxfdp1, readset, writeset, exceptset, timeout);
}
#endif
/*********************************************************************************************************
** Function name:           raise
** Descriptions:            ���Լ����ź�
** input parameters:        sig                 �ź�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int raise(int sig)
{
  return kill(getpid(), sig);
}
/*********************************************************************************************************
** Function name:           signal
** Descriptions:            ���źŴ�����
** input parameters:        sig                 �ź�
**                          func                �źŴ�����
** output parameters:       NONE
** Returned value:          ԭ�е��źŴ�����
*********************************************************************************************************/
_sig_func_ptr signal(int sig, _sig_func_ptr func)
{
    int syscall = SYSCALL_SIGNAL;
    syscall_args_t args = {(void *)sig, (void *)func};

    syscall_code();
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
    int syscall = SYSCALL_SIGPROCMASK;
    syscall_args_t args = {(void *)how, (void *)set, (void *)oset};

    syscall_code();
}

int sigsuspend(const sigset_t *set)
{
    int syscall = SYSCALL_SIGSUSPEND;
    syscall_args_t args = {(void *)set};

    syscall_code();
}

#include <sys/socket.h>

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/socket.html
 */
int socket(int domain, int type, int protocol)
{
    int syscall = SYSCALL_SOCKET;
    syscall_args_t args = {(void *)domain, (void *)type, (void *)protocol};

    syscall_code();
}

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/bind.html
 */
int bind(int s, const struct sockaddr *name, socklen_t namelen)
{
    int syscall = SYSCALL_BIND;
    syscall_args_t args = {(void *)s, (void *)name, (void *)namelen};

    syscall_code();
}

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/accept.html
 */
int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    int syscall = SYSCALL_ACCEPT;
    syscall_args_t args = {(void *)s, (void *)addr, (void *)addrlen};

    syscall_code();
}
#ifndef SMILEOS_KERNEL
#undef accept
int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    return __pthread_accept(s, addr, addrlen);
}
#endif

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/connect.html
 */
int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
    int syscall = SYSCALL_CONNECT;
    syscall_args_t args = {(void *)s, (void *)name, (void *)namelen};

    syscall_code();
}
#ifndef SMILEOS_KERNEL
#undef connect
int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
    return __pthread_connect(s, (struct sockaddr *)name, namelen);
}
#endif

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/listen.html
 */
int listen(int s, int backlog)
{
    int syscall = SYSCALL_LISTEN;
    syscall_args_t args = {(void *)s, (void *)backlog};

    syscall_code();
}

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/recv.html
 */
int recv(int s, void *mem, size_t len, int flags)
{
    int syscall = SYSCALL_RECV;
    syscall_args_t args = {(void *)s, (void *)mem, (void *)len, (void *)flags};

    syscall_code();
}
#ifndef SMILEOS_KERNEL
#undef recv
int recv(int s, void *mem, size_t len, int flags)
{
    return __pthread_recv(s, mem, len, flags);
}
#endif

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/recvfrom.html
 */
int recvfrom(int s, void *mem, size_t len, int flags,
      struct sockaddr *from, socklen_t *fromlen)
{
    int syscall = SYSCALL_RECVFROM;
    syscall_args_t args = {(void *)s, (void *)mem, (void *)len, (void *)flags, (void *)from, (void *)fromlen};

    syscall_code();
}
#ifndef SMILEOS_KERNEL
#undef recvfrom
int recvfrom(int s, void *mem, size_t len, int flags,
      struct sockaddr *from, socklen_t *fromlen)
{
    return __pthread_recvfrom(s, mem, len, flags, from, fromlen);
}
#endif

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/sendto.html
 */
int sendto(int s, const void *dataptr, size_t size, int flags,
    const struct sockaddr *to, socklen_t tolen)
{
    int syscall = SYSCALL_SENDTO;
    syscall_args_t args = {(void *)s, (void *)dataptr, (void *)size, (void *)flags, (void *)to, (void *)tolen};

    syscall_code();
}
#ifndef SMILEOS_KERNEL
#undef sendto
int sendto(int s, const void *dataptr, size_t size, int flags,
    const struct sockaddr *to, socklen_t tolen)
{
    return __pthread_sendto(s, dataptr, size, flags, to, tolen);
}
#endif

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/getsockopt.html
 */
int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
    int syscall = SYSCALL_GETSOCKOPT;
    syscall_args_t args = {(void *)s, (void *)level, (void *)optname, (void *)optval, (void *)optlen};

    syscall_code();
}

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/send.html
 */
int send(int s, const void *dataptr, size_t size, int flags)
{
    int syscall = SYSCALL_SEND;
    syscall_args_t args = {(void *)s, (void *)dataptr, (void *)size, (void *)flags};

    syscall_code();
}
#ifndef SMILEOS_KERNEL
#undef send
int send(int s, const void *dataptr, size_t size, int flags)
{
    return __pthread_send(s, dataptr, size, flags);
}
#endif

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/shutdown.html
 */
int shutdown(int s, int how)
{
    int syscall = SYSCALL_SHUTDOWN;
    syscall_args_t args = {(void *)s, (void *)how};

    syscall_code();
}

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/setsockopt.html
 */
int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
    int syscall = SYSCALL_SETSOCKOPT;
    syscall_args_t args = {(void *)s, (void *)level, (void *)optname, (void *)optval, (void *)optlen};

    syscall_code();
}
/*********************************************************************************************************
** ���¼���׮����δʵ��
*********************************************************************************************************/
/*
 * _sbrk_r
 */
void *_sbrk_r(struct _reent *reent, ptrdiff_t incr)
{
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
}

/*
 * _fork_r
 */
int _fork_r(struct _reent *reent)
{
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
}

/*
 * _times_r
 */
_CLOCK_T_ _times_r(struct _reent *reent, struct tms *buf)
{
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
}

/*
 * _wait_r
 */
int _wait_r(struct _reent *reent, int *status)
{
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
}

/*
 * _execve_r
 */
int _execve_r(struct _reent *reent, const char *path, char *const *argv, char *const *env)
{
    printf("can't call %s()!, kill process %d\n", __func__, getpid());

    abort();
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
