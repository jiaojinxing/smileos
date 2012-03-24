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
** File name:               ftpd.c
** Last modified Date:      2012-3-9
** Last Version:            1.0.0
** Descriptions:            ftpd 服务器
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-9
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/sys_call.h"
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

#ifndef LINE_MAX
#define LINE_MAX    512
#endif

#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"

static void ftpd_list_thread(void *arg)
{
    struct sockaddr_in local_addr, remote_addr;
    socklen_t addr_len;
    int fd, client_fd;
    char buf[LINE_MAX];
    int len;
    int port = (int)arg;
    int on = 1;

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) {
        printf("%s: failed to create socket\n", __func__);
        exit(-1);
    }

    local_addr.sin_family       = AF_INET;
    local_addr.sin_len          = sizeof(struct sockaddr_in);
    local_addr.sin_addr.s_addr  = INADDR_ANY;
    local_addr.sin_port         = htons(port);

    setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    if (bind(fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        closesocket(fd);
        exit(-1);
    }

    listen(fd, 2);

    addr_len = sizeof(remote_addr);

    client_fd = accept(fd, (struct sockaddr *)&remote_addr, &addr_len);
    if (client_fd > 0) {
        len = sprintf(buf, "drw-r--r-- 1 admin admin %d Jan 1 2000 %s\r\n", 0, "..");
        send(client_fd, buf, len, 0);
        len = sprintf(buf, "drw-r--r-- 1 admin admin %d Jan 1 2000 %s\r\n", 0, "test");
        send(client_fd, buf, len, 0);
        len = sprintf(buf, "drw-r--r-- 1 admin admin %d Jan 1 2000 %s\r\n", 0, "aa");
        send(client_fd, buf, len, 0);
        len = sprintf(buf, "drw-r--r-- 1 admin admin %d Jan 1 2000 %s\r\n", 0, "bb");
        send(client_fd, buf, len, 0);
        closesocket(client_fd);
    } else {
        printf("%s: failed to accept connect\n", __func__);
    }

    closesocket(fd);
}

/*
 * ftpd 线程
 */
static void ftpd_thread(void *arg)
{
    int  fd = (int)arg;
    char buf[LINE_MAX];
    char cmd[LINE_MAX];
    int  len;

    len = sprintf(buf, "220 SmileOS FTP Server Ready\r\n");
    send(fd, buf, len, 0);

    while (1) {
        len = recv(fd, cmd, sizeof(cmd) - 1, 0);
        if (len <= 0) {
            break;
        } else if (len > 0) {
            cmd[len] = '\0';
            if (strncmp(cmd, "USER", 4) == 0) {
                len = sprintf(buf, "331 Guest login OK, send your complete e-mail address as password\r\n");
                send(fd, buf, len, 0);
            } else if (strncmp(cmd, "PASS", 4) == 0) {
                len = sprintf(buf, "230 Guest login OK, access restrictions apply\r\n");
                send(fd, buf, len, 0);
            } else if (strncmp(cmd, "SYST", 4) == 0) {
                len = sprintf(buf, "200 SmileOS\r\n");
                send(fd, buf, len, 0);
            } else if (strncmp(cmd, "FEAT", 4) == 0) {
                len = sprintf(buf, "500 Failure\r\n");
                send(fd, buf, len, 0);
            } else if (strncmp(cmd, "PWD", 3) == 0) {
                len = sprintf(buf, "200 /\r\n");
                send(fd, buf, len, 0);
            } else if (strncmp(cmd, "TYPE", 4) == 0) {
                len = sprintf(buf, "200 ASCII\r\n");
                send(fd, buf, len, 0);
            } else if (strncmp(cmd, "PASV", 4) == 0) {
                extern struct netif *netif_default;
                len = sprintf(buf, "200 %d,%d,%d,%d,%s,%s\r\n",
                        (netif_default->ip_addr.addr >> 0 ) & 0xFF,
                        (netif_default->ip_addr.addr >> 8 ) & 0xFF,
                        (netif_default->ip_addr.addr >> 16) & 0xFF,
                        (netif_default->ip_addr.addr >> 24) & 0xFF,
                        /*
                         * 这里端口的表示方式有点怪异
                         */
                        "9",
                        "13");
                send(fd, buf, len, 0);
            } else if (strncmp(cmd, "LIST", 4) == 0) {

                len = sprintf(buf, "150 Opening ASCII mode data connection for /\r\n");
                send(fd, buf, len, 0);

                ftpd_list_thread((void *)2317);

                len = sprintf(buf, "226 Transfer complete\r\n");
                send(fd, buf, len, 0);
            } else if (strncmp(cmd, "CWD", 3) == 0) {
                len = sprintf(buf, "200 /\r\n");
                send(fd, buf, len, 0);
            } else {
                printf("%s: unknown cmd %s\n", __func__, cmd);
            }
        }
    }

    closesocket(fd);
}

/*
 * ftpd 服务器线程
 */
void ftpd(void *arg)
{
    struct sockaddr_in local_addr, remote_addr;
    socklen_t addr_len;
    int fd, client_fd;
    char name[32];

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) {
        printf("%s: failed to create socket\n", __func__);
        exit(-1);
    }

    local_addr.sin_family       = AF_INET;
    local_addr.sin_len          = sizeof(struct sockaddr_in);
    local_addr.sin_addr.s_addr  = INADDR_ANY;
    local_addr.sin_port         = htons(21);

    if (bind(fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        printf("%s: failed to bind port %d\n", __func__, ntohs(local_addr.sin_port));
        closesocket(fd);
        exit(-1);
    }

    listen(fd, 2);

    while (1) {
        addr_len = sizeof(remote_addr);

        client_fd = accept(fd, (struct sockaddr *)&remote_addr, &addr_len);
        if (client_fd > 0) {
            sprintf(name, "%s%d", __func__, client_fd);

            kthread_create(name, ftpd_thread, (void *)client_fd, 16 * KB, 10);
        } else {
            printf("%s: failed to accept connect\n", __func__);
        }
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
