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
#ifndef SYS_SOCKET_H_
#define SYS_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/sockets.h"

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/accept.html
 */
int accept(int s, struct sockaddr *addr, socklen_t *addrlen);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/bind.html
 */
int bind(int s, const struct sockaddr *name, socklen_t namelen);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/shutdown.html
 */
int shutdown(int s, int how);

/*
 * http://pubs.opengroup.org/onlinepubs/009604499/functions/getpeername.html
 */
int getpeername(int s, struct sockaddr *name, socklen_t *namelen);

/*
 * http://pubs.opengroup.org/onlinepubs/009604499/functions/getsockname.html
 */
int getsockname(int s, struct sockaddr *name, socklen_t *namelen);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/getsockopt.html
 */
int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/setsockopt.html
 */
int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/connect.html
 */
int connect(int s, const struct sockaddr *name, socklen_t namelen);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/listen.html
 */
int listen(int s, int backlog);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/recv.html
 */
int recv(int s, void *mem, size_t len, int flags);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/recvfrom.html
 */
int recvfrom(int s, void *mem, size_t len, int flags,
             struct sockaddr *from, socklen_t *fromlen);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/send.html
 */
int send(int s, const void *dataptr, size_t size, int flags);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/sendto.html
 */
int sendto(int s, const void *dataptr, size_t size, int flags,
           const struct sockaddr *to, socklen_t tolen);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/socket.html
 */
int socket(int domain, int type, int protocol);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  SYS_SOCKET_H_               */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
