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
** File name:               netdb.h
** Last modified Date:      2012-6-14
** Last Version:            1.0.0
** Descriptions:            netdb 头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-6-14
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
#ifndef NETDB_H_
#define NETDB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/netdb.h"

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/gethostbyname.html
 */
struct hostent *gethostbyname(const char *name);

int gethostbyname_r(const char *name, struct hostent *ret, char *buf,
                size_t buflen, struct hostent **result, int *h_errnop);

/*
 * http://pubs.opengroup.org/onlinepubs/009604499/functions/getaddrinfo.html
 */
void freeaddrinfo(struct addrinfo *ai);

/*
 * http://pubs.opengroup.org/onlinepubs/009604499/functions/getaddrinfo.html
 */
int getaddrinfo(const char *nodename,
       const char *servname,
       const struct addrinfo *hints,
       struct addrinfo **res);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  NETDB_H_                    */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
