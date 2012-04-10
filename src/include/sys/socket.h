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
** File name:               socket.h
** Last modified Date:      2012-2-23
** Last Version:            1.0.0
** Descriptions:            socket ͷ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-23
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
#ifndef SOCKET_H_
#define SOCKET_H_

#include "lwip/sockets.h"

#ifdef SMILEOS_KERNEL
/*
 * ���� socket
 */
int socket_attach(int sock_fd, int isatty);

/*
 * ��� socket ��˽���ļ�������
 */
int socket_priv_fd(int fd);
#else
/*
 * socket
 */
int socket(int domain, int type, int protocol);

/*
 * bind
 */
int bind(int s, const struct sockaddr *name, socklen_t namelen);
/*
 * accept
 */
int accept(int s, struct sockaddr *addr, socklen_t *addrlen);

/*
 * connect
 */
int connect(int s, const struct sockaddr *name, socklen_t namelen);
/*
 * listen
 */
int listen(int s, int backlog);
#endif

#endif                                                                  /*  SOCKET_H_                   */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
