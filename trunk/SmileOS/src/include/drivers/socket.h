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
** Last modified Date:      2013-2-19
** Last Version:            1.0.0
** Descriptions:            socket 头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-19
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
** output parameters:       ctx                 上下文
** Returned value:          socket 的私有文件描述符
*********************************************************************************************************/
int socket_priv_fd(int fd, void **ctx);
/*********************************************************************************************************
** Function name:           socket_op_end
** Descriptions:            socket 操作结束
** input parameters:        ctx                 上下文
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void socket_op_end(void *ctx);
#endif

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  SOCKET_H_                   */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
