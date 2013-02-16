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
** File name:               syscall_ex.c
** Last modified Date:      2012-5-3
** Last Version:            1.0.0
** Descriptions:            系统调用包裹函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-5-3
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
#include <fcntl.h>
#include <reent.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/select.h>
/*********************************************************************************************************
** Function name:           creat
** Descriptions:            创建文件
** input parameters:        path                文件路径
**                          mode                模式
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int creat(const char *path, mode_t mode)
{
    return open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
/*********************************************************************************************************
** Function name:           mkdir
** Descriptions:            创建目录
** input parameters:        path                目录路径
**                          mode                模式
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mkdir(const char *path, mode_t mode)
{
    return _mkdir_r(_REENT, path, mode);
}
/*********************************************************************************************************
** Function name:           rmdir
** Descriptions:            删除目录
** input parameters:        path                目录路径
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int rmdir(const char *path)
{
    extern int _rmdir_r(struct _reent *reent, const char *path);

    return _rmdir_r(_REENT, path);
}
/*********************************************************************************************************
** Function name:           isatty
** Descriptions:            判断文件是不是一个 TTY
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          1 OR 0
*********************************************************************************************************/
int isatty(int fd)
{
    return _isatty_r(_REENT, fd);
}
/*********************************************************************************************************
** Function name:           poll
** Descriptions:            使用 select 实现
** input parameters:        fds                 pollfd 结构数组指针
**                          nfds                pollfd 结构数组大小
**                          timeout             超时时间
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
    fd_set rfds;
    fd_set wfds;
    fd_set efds;
    nfds_t i;
    int    n;
    int    ret;
    struct timeval tv;

    if (fds == NULL && nfds != 0) {
        return -1;
    }

    if (fds != NULL && nfds == 0) {
        return -1;
    }

    if (nfds == 0 && fds == NULL) {
        if (timeout >= 0) {
            tv.tv_sec  = timeout / 1000;
            tv.tv_usec = 1000 * (timeout % 1000);
            return select(0, NULL, NULL, NULL, &tv);
        } else {
            return -1;
        }
    }

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    n = -1;
    for (i = 0; i < nfds; i++) {
        if (fds[i].fd < 0) {
            continue;
        }

        if (fds[i].events & POLLIN) {
            FD_SET(fds[i].fd, &rfds);
        }

        if (fds[i].events & POLLOUT) {
            FD_SET(fds[i].fd, &wfds);
        }

        if (fds[i].events & POLLERR) {
            FD_SET(fds[i].fd, &efds);
        }

        if (fds[i].fd > n) {
            n = fds[i].fd;
        }
    }

    if (n == -1) {
        return -1;
    }

    if (timeout < 0) {
        ret = select(n + 1, &rfds, &wfds, &efds, NULL);
    } else {
        tv.tv_sec  = timeout / 1000;
        tv.tv_usec = 1000 * (timeout % 1000);
        ret = select(n + 1, &rfds, &wfds, &efds, &tv);
    }

    if (ret < 0) {
        return ret;
    }

    for (i = 0; i < nfds; i++) {
        fds[i].revents = 0;

        if (FD_ISSET(fds[i].fd, &rfds)) {
            fds[i].revents |= POLLIN;
        }

        if (FD_ISSET(fds[i].fd, &wfds)) {
            fds[i].revents |= POLLOUT;
        }

        if (FD_ISSET(fds[i].fd, &efds)) {
            fds[i].revents |= POLLERR;
        }
    }

    return ret;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
