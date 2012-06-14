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
** File name:               posix_fix.c
** Last modified Date:      2012-5-3
** Last Version:            1.0.0
** Descriptions:            posix ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-5-3
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
#include <fcntl.h>
#include <reent.h>
#include <unistd.h>

/*
 * �����ļ�
 */
int creat(const char *path, mode_t mode)
{
    return open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
}

/*
 * ����Ŀ¼
 */
int mkdir(const char *path, mode_t mode)
{
    return _mkdir_r(_REENT, path, mode);
}

/*
 * ɾ��Ŀ¼
 */
int rmdir(const char *path)
{
    extern int _rmdir_r(struct _reent *reent, const char *path);

    return _rmdir_r(_REENT, path);
}

/*
 * �ж��ļ��ǲ���һ�� TTY
 */
int isatty(int fd)
{
    return _isatty_r(_REENT, fd);
}

/*
 * poll, ʹ�� select ʵ��
 */
#include <poll.h>
#include <sys/select.h>

int poll(struct pollfd *fds, nfds_t numfds, int timeout)
{
    fd_set read_set;
    fd_set write_set;
    fd_set exception_set;
    nfds_t i;
    int n;
    int rc;

    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&exception_set);

    n = -1;
    for (i = 0; i < numfds; i++) {
        if (fds[i].fd < 0) {
            continue;
        }

        if (fds[i].events & POLLIN)  FD_SET(fds[i].fd, &read_set);
        if (fds[i].events & POLLOUT) FD_SET(fds[i].fd, &write_set);
        if (fds[i].events & POLLERR) FD_SET(fds[i].fd, &exception_set);

        if (fds[i].fd > n) {
            n = fds[i].fd;
        }
    }

    if (n == -1) {
        return 0;
    }

    if (timeout < 0) {
        rc = select(n + 1, &read_set, &write_set, &exception_set, NULL);
    } else {
        struct timeval tv;

        tv.tv_sec  = timeout / 1000;
        tv.tv_usec = 1000 * (timeout % 1000);
        rc = select(n + 1, &read_set, &write_set, &exception_set, &tv);
    };

    if (rc < 0) {
        return rc;
    }

    for (i = 0; i < numfds; i++) {
        fds[i].revents = 0;

        if (FD_ISSET(fds[i].fd, &read_set))      fds[i].revents |= POLLIN;
        if (FD_ISSET(fds[i].fd, &write_set))     fds[i].revents |= POLLOUT;
        if (FD_ISSET(fds[i].fd, &exception_set)) fds[i].revents |= POLLERR;
    }

    return rc;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
