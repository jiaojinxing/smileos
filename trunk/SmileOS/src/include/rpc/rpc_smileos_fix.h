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
** Descriptions:            移植 sunrpc (来自 glibc2.9) 到 SylixOS.
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2010-8-24
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
#ifndef RPC_SMILEOS_FIX_H_
#define RPC_SMILEOS_FIX_H_
/*********************************************************************************************************
**  包含标准的头文件
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
**  定义宏
*********************************************************************************************************/
#define __THROW                                                         /*  不要抛出异常                */

#define INTDEF(func)                                                    /*  定义函数                    */
#define INTUSE(func)                  func                              /*  调用函数                    */

#define _(str)                        str                               /*  字符串                      */

#define __libc_once_define(attr, var) attr bool_t var = FALSE           /*  定义 once 变量              */
#define __libc_once(var, func)        \
        if (var == FALSE) {var = TRUE; func();}                         /*  once 调用函数               */

#define internal_function                                               /*  内部函数                    */
#define libc_hidden_def(func)                                           /*  隐藏函数的定义              */
#define strong_alias(func, alias)                                       /*  强制别名                    */

#define __bzero                       bzero                             /*  清零                        */
#define __getpid                      gettid                            /*  获得进程 ID 号              */
#define __set_errno                   seterrno                          /*  设置 errno                  */
#define __poll                        poll                              /*  poll 系统调用               */
#define __socket                      socket                            /*  BSD/Socket 类函数           */
#define __close                       close
#define __bind                        bind
#define __connect                     connect
#define __read                        read
#define __write                       write
#define __ioctl                       ioctl
#define __recvfrom                    recvfrom
#define __sendto                      sendto

extern struct rpc_createerr           rpc_createerr;
#define get_rpc_createerr()           rpc_createerr                     /*  获得 RPC 创建错误变量       */

#define IPPORT_RESERVED               4096                              /*  保留的 IP 端口号            */

#define DEFAULT_NETIF_NAME            "en1"                             /*  缺省的网络接口名            */

#define __u_char_defined                                                /*  u_char  等类型已经定义      */
#define __daddr_t_defined                                               /*  daddr_t 等类型已经定义      */
/*********************************************************************************************************
**  定义类型
*********************************************************************************************************/
typedef uid_t                         __uid_t;                          /*  用户 ID                     */
typedef gid_t                         __gid_t;                          /*  组 ID                       */

struct svc_req {
    int     dump;
};

typedef struct SVCXPRT SVCXPRT;
struct SVCXPRT {
    int     dump;
};
/*********************************************************************************************************
**  声明函数
*********************************************************************************************************/
extern int bindresvport(int sd, struct sockaddr_in *sin);
extern int __fxprintf(FILE *fp, const char *fmt, ...);

#endif                                                                  /*  RPC_SMILEOS_FIX_H_          */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
