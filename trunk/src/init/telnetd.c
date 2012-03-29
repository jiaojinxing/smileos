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
** Descriptions:            telnetd ������
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-6
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/sbin.h"
#include "vfs/vfs.h"
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

/*
 * ���������Ϣ
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
        return sprintf(buf, "%s\t %s\t\t %4u\t %s\t %4u\t %10u\t %4u\t %4u%%\t %4u%%\t %4u\t %4u\r\n",
                        task->type == TASK_TYPE_PROCESS ? "process" : "kthread",
                        task->name,
                        task->tid,
                        state,
                        task->counter,
                        task->timer,
                        task->priority,
                        task->cpu_rate,
                        task->stack_rate,
                        task->frame_nr,
                        task->dabt_cnt);
    } else {
        return sprintf(buf, "%s\t %s\t %4u\t %s\t %4u\t %10u\t %4u\t %4u%%\t %4u%%\t %4u\t %4u\r\n",
                        task->type == TASK_TYPE_PROCESS ? "process" : "kthread",
                        task->name,
                        task->tid,
                        state,
                        task->counter,
                        task->timer,
                        task->priority,
                        task->cpu_rate,
                        task->stack_rate,
                        task->frame_nr,
                        task->dabt_cnt);
    }
}

static int do_ts(int argc, char **argv, int fd, char buf[LINE_MAX])
{
    int len;
    int i;
    uint32_t reg;
    task_t *task;

    len = sprintf(buf, "type\t name\t\t pid\t state\t count\t timer\t\t prio\t cpu\t stack\t page\t dabt\r\n");
    write(fd, buf, len);

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
        reg = interrupt_disable();
        if (task->state != TASK_UNALLOCATE) {
            len = get_task_info(task, buf);
            interrupt_resume(reg);
            write(fd, buf, len);
        } else {
            interrupt_resume(reg);
        }
    }
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
 * ִ���ڽ��� sbin ����
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
        write(fd, buf, len);
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
        extern int ls_main(int argc, char *argv[]);
        ls_main(argc, argv);
    } else if (strcmp(argv[0], "cd") == 0) {
        do_cd(argc, argv, fd, buf);
    } else if (strcmp(argv[0], "mems") == 0) {
        kern_heap_show(fd);
    } else if (strcmp(argv[0], "exit") == 0) {
        close(fd);
        _exit(0);
    } else {
        do_exec_buildin(argc, argv, fd, buf);
    }

    return 0;
}

const char logo[] =
        "_________________________________________________\r\n"
        "      __                            __       __\r\n"
        "    /    )          ,   /         /    )   /    )\r\n"
        "    \\       _--_       /   ___   /    /    \\\r\n"
        "     \\     / /  ) /   /   /___) /    /      \\\r\n"
        "_(____/___/_/__/_/___/___(___ _(____/___(____/___\r\n";

/*
 * telnetd �߳�
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

    close(STDOUT_FILENO);

    fd = socket_attach(fd);

    write(fd, logo, strlen(logo));

    len = sprintf(buf, "%s]#", vfs_getcwd(NULL, 0));
    write(fd, buf, len);

    pos = 0;

    while (1) {
        ret = vfs_read(fd, &ch, 1);
        if (ret <= 0) {
            fprintf(stderr, "%s: failed to read socket\r\n", __func__);
            break;
        }

        if (ret > 0) {
            if (iscntrl(ch)) {
                if (ch == '\b') {
                    if (pos > 0) {
                        pos--;
                        len = sprintf(buf, " \b \b");
                        write(fd, buf, len);
                    } else {
                        len = sprintf(buf, "#");
                        write(fd, buf, len);
                    }
                } else if (ch == '\n') {
                    if (pos > 0) {
                        cmd[pos] = '\0';
                        pos = 0;
                        exec_cmd(cmd, fd, buf);
                    }

                    len = sprintf(buf, "%s]#", vfs_getcwd(NULL, 0));
                    write(fd, buf, len);
                }
            } else if (isprint(ch)){
                cmd[pos] = ch;
                pos++;
            }
        }
    }

    close(fd);
}

/*
 * telnet �������߳�
 */
void telnetd(void *arg)
{
    struct sockaddr_in local_addr, remote_addr;
    socklen_t addr_len;
    int fd, client_fd;
    char name[32];

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) {
        printf("%s: failed to create socket\r\n", __func__);
        _exit(-1);
    }

    local_addr.sin_family       = AF_INET;
    local_addr.sin_len          = sizeof(struct sockaddr_in);
    local_addr.sin_addr.s_addr  = INADDR_ANY;
    local_addr.sin_port         = htons(23);

    if (bind(fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        printf("%s: failed to bind port %d\r\n", __func__, ntohs(local_addr.sin_port));
        closesocket(fd);
        _exit(-1);
    }

    listen(fd, 2);

    while (1) {
        addr_len = sizeof(remote_addr);

        client_fd = accept(fd, (struct sockaddr *)&remote_addr, &addr_len);
        if (client_fd > 0) {
            sprintf(name, "%s%d", __func__, client_fd);

            kthread_create(name, telnetd_thread, (void *)client_fd, 4 * KB, 10);
        } else {
            printf("%s: failed to accept connect\r\n", __func__);
        }
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
