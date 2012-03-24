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
#include "kern/sbin.h"
#include "vfs/vfs.h"
#include <dirent.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

#ifndef LINE_MAX
#define LINE_MAX    512
#endif

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

    default:
        state = "dead";
        break;
    }

    if (strlen(task->name) < 7) {
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

static int do_ts(int argc, char **argv, int fd, char buf[LINE_MAX])
{
    int len;
    int i;
    uint32_t reg;
    task_t *task;

    len = sprintf(buf, "type\t name\t\t pid\t state\t count\t timer\t\t prio\t cpu\t frame_nr\r\n");
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
    return 0;
}

static int do_ls(int argc, char **argv, int fd, char buf[LINE_MAX])
{
    int len;
    DIR *dir;
    struct dirent *entry;

    if (argc < 2) {
        dir = vfs_opendir(".");
    } else {
        dir = vfs_opendir(argv[1]);
    }
    if (dir == NULL) {
        return -1;
    }

    while ((entry = vfs_readdir(dir)) != NULL) {
        len = sprintf(buf, "%s ", entry->d_name);
        send(fd, buf, len, 0);
    }
    vfs_closedir(dir);
    len = sprintf(buf, "\r\n");
    send(fd, buf, len, 0);
    return 0;
}

static int do_cd(int argc, char **argv, int fd, char buf[LINE_MAX])
{
    if (argc == 2) {
        return vfs_chdir(argv[1]);
    } else {
        return 0;
    }
}

/*
 * 执行内建的 sbin 命令
 */
static int do_exec_buildin(int argc, char **argv, int fd, char buf[LINE_MAX])
{
    int len;
    uint8_t *code;
    uint32_t size;

    code = sbin_lookup(argv[0], &size);
    if (code != NULL) {
        return process_create(argv[0], code, size, 10);
    } else {
        len = sprintf(buf, "unknown cmd\r\n");
        send(fd, buf, len, 0);
        return -1;
    }
}

#define ARGMAX      32

static int exec_cmd(char *cmd, int fd, char buf[LINE_MAX])
{
    static char *argv[ARGMAX];
    char *p, *word = NULL;
    int argc = 0;

    if (cmd[0] != ' ' && cmd[0] != '\t') {
        word = cmd;
    }

    p = cmd;
    while (*p) {
        if (word == NULL) {
            if (*p != ' ' && *p != '\t') {
                word = p;
            }
        } else {
            if (*p == ' ' || *p == '\t') {
                *p = '\0';
                argv[argc++] = word;
                word = NULL;
                if (argc >= ARGMAX - 1) {
                    return -1;
                }
            }
        }
        p++;
    }

    if (argc == 0 && word == NULL) {
        return -1;
    }

    if (word) {
        argv[argc++] = word;
    }

    argv[argc] = NULL;

    if (strcmp(argv[0], "ts") == 0) {
        do_ts(argc, argv, fd, buf);
    } else if (strcmp(argv[0], "ls") == 0) {
        do_ls(argc, argv, fd, buf);
    } else if (strcmp(argv[0], "cd") == 0) {
        do_cd(argc, argv, fd, buf);
    } else {
        do_exec_buildin(argc, argv, fd, buf);
    }

    return 0;
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
    char buf[LINE_MAX];
    char cmd[LINE_MAX];
    char ch;

    len = sprintf(buf, "******************* SmileOS Shell *******************\r\n");
    send(fd, buf, len, 0);

    len = sprintf(buf, "[%s]:", vfs_getcwd(NULL, 0));
    send(fd, buf, len, 0);

    pos = 0;

    while (1) {
        ret = recv(fd, &ch, 1, 0);
        if (ret <= 0) {
            break;
        }

        if (ret > 0) {
            if (iscntrl(ch)) {
                if (ch == '\b') {
                    if (pos > 0) {
                        pos--;
                        len = sprintf(buf, " \b \b");
                        send(fd, buf, len, 0);
                    } else {
                        len = sprintf(buf, ":");
                        send(fd, buf, len, 0);
                    }
                } else if (ch == '\n') {
                    if (pos > 0) {
                        cmd[pos] = '\0';
                        pos = 0;
                        exec_cmd(cmd, fd, buf);
                    }

                    len = sprintf(buf, "[%s]:", vfs_getcwd(NULL, 0));
                    send(fd, buf, len, 0);
                }
            } else if (isprint(ch)){
                cmd[pos] = ch;
                pos++;
            }
        }
    }

    closesocket(fd);
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
        closesocket(fd);
        exit(-1);
    }

    listen(fd, 2);

    while (1) {
        addr_len = sizeof(remote_addr);

        client_fd = accept(fd, (struct sockaddr *)&remote_addr, &addr_len);
        if (client_fd > 0) {
            sprintf(name, "%s%d", __func__, client_fd);

            kthread_create(name, telnetd_thread, (void *)client_fd, 16 * KB, 10);
        } else {
            printf("%s: failed to accept connect\n", __func__);
        }
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
