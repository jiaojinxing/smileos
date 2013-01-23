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
** File name:               socket.h
** Last modified Date:      2012-2-23
** Last Version:            1.0.0
** Descriptions:            socket 头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-23
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
#ifndef SOCKET_H_
#define SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/sockets.h"

int accept(int s, struct sockaddr *addr, socklen_t *addrlen);

int bind(int s, const struct sockaddr *name, socklen_t namelen);

int shutdown(int s, int how);

int getpeername(int s, struct sockaddr *name, socklen_t *namelen);

int getsockname(int s, struct sockaddr *name, socklen_t *namelen);

int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen);

int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);

int connect(int s, const struct sockaddr *name, socklen_t namelen);

int listen(int s, int backlog);

int recv(int s, void *mem, size_t len, int flags);

int recvfrom(int s, void *mem, size_t len, int flags,
             struct sockaddr *from, socklen_t *fromlen);

int send(int s, const void *dataptr, size_t size, int flags);

int sendto(int s, const void *dataptr, size_t size, int flags,
           const struct sockaddr *to, socklen_t tolen);

int socket(int domain, int type, int protocol);

#ifdef SMILEOS_KERNEL
/*********************************************************************************************************
** Function name:           socket_attach
** Descriptions:            联结 socket
** input parameters:        sock_fd             socket 的私有文件描述符
** output parameters:       NONE
** Returned value:          IO 系统文件描述符
*********************************************************************************************************/
int socket_attach(int sock_fd);
/*********************************************************************************************************
** Function name:           socket_priv_fd
** Descriptions:            获得 socket 的私有文件描述符
** input parameters:        fd                  IO 系统文件描述符
** output parameters:       NONE
** Returned value:          socket 的私有文件描述符
*********************************************************************************************************/
int socket_priv_fd(int fd);
#endif

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  SOCKET_H_                   */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
