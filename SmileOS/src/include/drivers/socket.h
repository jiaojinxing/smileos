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
** Descriptions:            socket ͷ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-19
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

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SMILEOS_KERNEL
/*********************************************************************************************************
** Function name:           socket_attach
** Descriptions:            ���� socket
** input parameters:        sock_fd             socket ��˽���ļ�������
** output parameters:       NONE
** Returned value:          IO ϵͳ�ļ�������
*********************************************************************************************************/
int socket_attach(int sock_fd);
/*********************************************************************************************************
** Function name:           socket_priv_fd
** Descriptions:            ��� socket ��˽���ļ�������
** input parameters:        fd                  IO ϵͳ�ļ�������
** output parameters:       ctx                 ������
** Returned value:          socket ��˽���ļ�������
*********************************************************************************************************/
int socket_priv_fd(int fd, void **ctx);
/*********************************************************************************************************
** Function name:           socket_op_end
** Descriptions:            socket ��������
** input parameters:        ctx                 ������
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
