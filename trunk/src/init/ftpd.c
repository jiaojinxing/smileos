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
#include "vfs/vfs.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <sys/stat.h>
#include <dirent.h>

#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"

static int ftpd_list(int pasv_fd)
{
    socklen_t addr_len;
    int data_fd;
    struct sockaddr_in remote_addr;

    addr_len = sizeof(remote_addr);

    data_fd = accept(pasv_fd, (struct sockaddr *)&remote_addr, &addr_len);
    if (data_fd > 0) {
        FILE *fp;
        struct stat st;
        DIR *dir;
        struct dirent *entry;

        data_fd = socket_attach(data_fd);
        fp      = fdopen(data_fd, "w+");

        dir = vfs_opendir(".");

        while (NULL != (entry = vfs_readdir(dir))) {

            stat(entry->d_name, &st);

            if (S_ISDIR(st.st_mode)) {
                fprintf(fp, "drw-r--r-- 1 admin admin %d Jan 1 2000 %s\r\n", 0, entry->d_name);
            } else {
                fprintf(fp, "-rw-r--r-- 1 admin admin %d Jan 1 2000 %s\r\n", 0, entry->d_name);
            }
            fflush(fp);
        }

        vfs_closedir(dir);

        fclose(fp);

        return 0;
    } else {
        return -1;
    }
}

int ftpd_pasv(int port)
{
    struct sockaddr_in local_addr;
    int pasv_fd;
    int on = 1;

    pasv_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (pasv_fd < 0) {
        fprintf(stderr, "%s: failed to create socket\r\n", __func__);
        return -1;
    }

    local_addr.sin_family       = AF_INET;
    local_addr.sin_len          = sizeof(struct sockaddr_in);
    local_addr.sin_addr.s_addr  = INADDR_ANY;
    local_addr.sin_port         = htons(port);

    setsockopt(pasv_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    setsockopt(pasv_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    if (bind(pasv_fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        fprintf(stderr, "%s: failed to bind port %d\r\n", __func__, ntohs(local_addr.sin_port));
        closesocket(pasv_fd);
        return -1;
    }

    listen(pasv_fd, 2);

    extern struct netif *netif_default;
    printf("200 %d,%d,%d,%d,%s,%s\r\n",
            (netif_default->ip_addr.addr >> 0 ) & 0xFF,
            (netif_default->ip_addr.addr >> 8 ) & 0xFF,
            (netif_default->ip_addr.addr >> 16) & 0xFF,
            (netif_default->ip_addr.addr >> 24) & 0xFF,
            /*
             * 这里端口的表示方式有点怪异
             */
            "9",
            "13");

    return pasv_fd;
}
/*
 * ftpd 线程
 */
static void ftpd_thread(void *arg)
{
    int  cmd_fd;
    char cmd[LINE_MAX];
    int  len;
    int  pasv_fd;

    fclose(stdout);
    cmd_fd = socket_attach((int)arg);
    stdout = fdopen(cmd_fd, "w+");

    printf("220 SmileOS FTP Server Ready\r\n");
    fflush(stdout);

    while (1) {
        len = read(cmd_fd, cmd, sizeof(cmd) - 1);
        if (len <= 0) {
            fprintf(stderr, "%s: failed to read socket\r\n", __func__);
            break;
        } else if (len > 0) {
            cmd[len] = '\0';
            if (strncmp(cmd, "USER", 4) == 0) {
                printf("331 Guest login OK, send your e-mail as password\r\n");
            } else if (strncmp(cmd, "PASS", 4) == 0) {
                printf("230 Guest login OK\r\n");
            } else if (strncmp(cmd, "SYST", 4) == 0) {
                printf("200 SmileOS\r\n");
            } else if (strncmp(cmd, "FEAT", 4) == 0) {
                printf("500 Failure\r\n");
            } else if (strncmp(cmd, "PWD", 3) == 0) {
                printf("200 %s\r\n", vfs_getcwd(NULL, 0));
            } else if (strncmp(cmd, "TYPE", 4) == 0) {
                printf("200 ASCII\r\n");
            } else if (strncmp(cmd, "PASV", 4) == 0) {
                pasv_fd = ftpd_pasv(2317);
            } else if (strncmp(cmd, "LIST", 4) == 0) {
                printf("150 Opening ASCII mode data connection for /\r\n");
                fflush(stdout);
                ftpd_list(pasv_fd);
                printf("226 Transfer complete\r\n");
                closesocket(pasv_fd);
            } else if (strncmp(cmd, "CWD", 3) == 0) {
                vfs_chdir(cmd + 4);
                printf("200 %s\r\n", cmd + 4);
            } else {
                fprintf(stderr, "%s: unknown cmd %s\n", __func__, cmd);
            }
            fflush(stdout);
        }
    }
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
        fprintf(stderr, "%s: failed to create socket\r\n", __func__);
        exit(-1);
    }

    local_addr.sin_family       = AF_INET;
    local_addr.sin_len          = sizeof(struct sockaddr_in);
    local_addr.sin_addr.s_addr  = INADDR_ANY;
    local_addr.sin_port         = htons(21);

    if (bind(fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        fprintf(stderr, "%s: failed to bind port %d\r\n", __func__, ntohs(local_addr.sin_port));
        closesocket(fd);
        exit(-1);
    }

    listen(fd, 2);

    while (1) {
        addr_len = sizeof(remote_addr);

        client_fd = accept(fd, (struct sockaddr *)&remote_addr, &addr_len);
        if (client_fd > 0) {
            sprintf(name, "%s%d", __func__, client_fd);

            kthread_create(name, ftpd_thread, (void *)client_fd, 8 * KB, 5);
        } else {
            fprintf(stderr, "%s: failed to accept connect\r\n", __func__);
        }
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
