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
** File name:               rpc_smileos_fix.h
** Last modified Date:      2010-8-24
** Last Version:            1.0.0
** Descriptions:            ��ֲ sunrpc (���� glibc2.9) �� SylixOS.
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2010-8-24
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
#ifndef RPC_SMILEOS_FIX_H_
#define RPC_SMILEOS_FIX_H_
/*********************************************************************************************************
**  ������׼��ͷ�ļ�
*********************************************************************************************************/
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <poll.h>

#include <arpa/inet.h>
/*********************************************************************************************************
**  �����
*********************************************************************************************************/
#define __THROW                                                         /*  ��Ҫ�׳��쳣                */

#define INTDEF(func)                                                    /*  ���庯��                    */
#define INTUSE(func)                  func                              /*  ���ú���                    */

#define _(str)                        str                               /*  �ַ���                      */

#define __libc_once_define(attr, var) attr bool_t var = FALSE           /*  ���� once ����              */
#define __libc_once(var, func)        \
        if (var == FALSE) {var = TRUE; func();}                         /*  once ���ú���               */

#define internal_function                                               /*  �ڲ�����                    */
#define libc_hidden_def(func)                                           /*  ���غ����Ķ���              */
#define strong_alias(func, alias)                                       /*  ǿ�Ʊ���                    */

#define __bzero                       bzero                             /*  ����                        */
#define __getpid                      gettid                            /*  ��ý��� ID ��              */
#define __set_errno                   seterrno                          /*  ���� errno                  */
#define __poll                        poll                              /*  poll ϵͳ����               */
#define __socket                      socket                            /*  BSD/Socket �ຯ��           */
#define __close                       close
#define __bind                        bind
#define __connect                     connect
#define __read                        read
#define __write                       write
#define __ioctl                       ioctl
#define __recvfrom                    recvfrom
#define __sendto                      sendto

extern struct rpc_createerr           rpc_createerr;
#define get_rpc_createerr()           rpc_createerr                     /*  ��� RPC �����������       */

#define IPPORT_RESERVED               4096                              /*  ������ IP �˿ں�            */

#define DEFAULT_NETIF_NAME            "en1"                             /*  ȱʡ������ӿ���            */

#define __u_char_defined                                                /*  u_char  �������Ѿ�����      */
#define __daddr_t_defined                                               /*  daddr_t �������Ѿ�����      */
/*********************************************************************************************************
**  ��������
*********************************************************************************************************/
typedef uid_t                         __uid_t;                          /*  �û� ID                     */
typedef gid_t                         __gid_t;                          /*  �� ID                       */

struct svc_req {
    int     dump;
};

typedef struct SVCXPRT SVCXPRT;
struct SVCXPRT {
    int     dump;
};
/*********************************************************************************************************
**  ��������
*********************************************************************************************************/
extern int bindresvport(int sd, struct sockaddr_in *sin);
extern int __fxprintf(FILE *fp, const char *fmt, ...);

#endif                                                                  /*  RPC_SMILEOS_FIX_H_          */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
