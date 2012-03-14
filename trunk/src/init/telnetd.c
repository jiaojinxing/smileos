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
** File name:               telnetd.c
** Last modified Date:      2012-3-6
** Last Version:            1.0.0
** Descriptions:            telnetd 服务器
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-6
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

/*
 * 获得任务信息
 */
static int get_task_info(task_t *task, char *buf)
{
    const char *state;

    switch (task->state) {
    case TASK_RUNNING:
        state = "ready";
        break;

    case TASK_SLEEPING:
        state = "sleep";
        break;

    case TASK_SUSPEND:
        state = "wait";
        break;
    }

    if (strlen(task->name) < 5) {
        return sprintf(buf, "%s\t %s\t\t %4u\t %s\t %4u\t %10u\t %4u\t %4u%%\t %4u\r\n",
                        task->type == TASK_TYPE_PROCESS ? "process" : "kthread",
                        task->name,
                        task->tid,
                        state,
                        task->counter,
                        task->timer,
                        task->priority,
                        task->utilization,
                        task->frame_nr);
    } else {
        return sprintf(buf, "%s\t %s\t %4u\t %s\t %4u\t %10u\t %4u\t %4u%%\t %4u\r\n",
                        task->type == TASK_TYPE_PROCESS ? "process" : "kthread",
                        task->name,
                        task->tid,
                        state,
                        task->counter,
                        task->timer,
                        task->priority,
                        task->utilization,
                        task->frame_nr);
    }
}

/*
 * telnetd 线程
 */
static void telnetd_thread(void *arg)
{
    int  fd = (int)arg;
    int  len;
    int  ret;
    int  pos;
    char buf[128];
    char cmd[128];
    char ch;
    task_t *task;
    int  i;
    uint32_t reg;
    int on = 1;

    len = sprintf(buf, "******************* SmileOS Shell *******************\r\n");
    send(fd, buf, len, 0);

    len = sprintf(buf, ">:");
    send(fd, buf, len, 0);

    pos = 0;

    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on));

    while (1) {
        ret = recv(fd, &ch, 1, 0);
        if (ret < 0) {
            break;
        }

        if (ret > 0) {
            if (iscntrl(ch)) {
                if (ch == 0x08) {
                    if (pos > 0) {
                        pos--;
                        len = sprintf(buf, " \b \b");
                        send(fd, buf, len, 0);
                    } else {
                        len = sprintf(buf, ":");
                        send(fd, buf, len, 0);
                    }
                } else if (ch == 0x0D) {
                    if (pos > 0) {
                        cmd[pos] = '\0';
                        if (strcmp(cmd, "ts") == 0) {

                            len = sprintf(buf, "type\t name\t\t pid\t state\t count\t timer\t\t prio\t usage\t frame_nr\r\n");
                            send(fd, buf, len, 0);

                            for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
                                reg = interrupt_disable();
                                if (task->state != TASK_UNALLOCATE) {
                                    len = get_task_info(task, buf);
                                    interrupt_resume(reg);
                                    send(fd, buf, len, 0);
                                } else {
                                    interrupt_resume(reg);
                                }
                            }
                        } else if (strcmp(cmd, "time") == 0) {
                            /*
                             * TODO:
                             */
                        } else if (strcmp(cmd, "exit") == 0) {
                            break;
                        } else {
                            len = sprintf(buf, "unknown cmd\r\n");
                            send(fd, buf, len, 0);
                        }
                        pos = 0;
                        len = sprintf(buf, ">:");
                        send(fd, buf, len, 0);
                    } else {
                        len = sprintf(buf, ">:");
                        send(fd, buf, len, 0);
                    }
                }
            } else if (isprint(ch)){
                cmd[pos] = ch;
                pos++;
            }
        }
    }

    closesocket(fd);

    exit(0);
}

/*
 * telnet 服务器线程
 */
void telnetd(void *arg)
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
    local_addr.sin_port         = htons(23);

    if (bind(fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        printf("%s: failed to bind port %d\n", __func__, ntohs(local_addr.sin_port));
        exit(-1);
    }

    listen(fd, 2);

    while (1) {
        addr_len = sizeof(remote_addr);

        client_fd = accept(fd, (struct sockaddr *)&remote_addr, &addr_len);
        if (client_fd > 0) {
            sprintf(name, "%s%d", __func__, client_fd);

            kthread_create(name, telnetd_thread, (void *)client_fd, 16 * KB, 15);
        } else {
            printf("%s: failed to accept connect\n", __func__);
        }
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
