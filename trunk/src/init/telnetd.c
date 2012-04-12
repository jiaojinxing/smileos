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
** Modified by:             JiaoJinXing
** Modified date:           2012-4-1
** Version:                 1.1.0
** Descriptions:            今天是愚人节, 也是老婆的生日, 祝愿老婆和 BB 身体健康!
**                          感谢您们一直陪伴着我!
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/sbin.h"
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

/*
 * 获得任务信息
 */
static int pstat(task_t *task, char *buf)
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
        return sprintf(buf, "%s\t %s\t\t %4u\t %s\t %4u\t %10u\t %4u\t %4u%%\t %4u%%\t %4u\t %4u\n",
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
        return sprintf(buf, "%s\t %s\t %4u\t %s\t %4u\t %10u\t %4u\t %4u%%\t %4u%%\t %4u\t %4u\n",
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

/*
 * ts 命令
 */
static int ts_main(int argc, char **argv)
{
    int i;
    uint32_t reg;
    task_t *task;
    char buf[LINE_MAX];

    printf("type\t name\t\t pid\t state\t count\t timer\t\t prio\t cpu\t stack\t page\t dabt\n");

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
        reg = interrupt_disable();
        if (task->state != TASK_UNALLOCATE) {
            pstat(task, buf);
            interrupt_resume(reg);
            printf(buf);
        } else {
            interrupt_resume(reg);
        }
    }
    return 0;
}

/*
 * cd 命令
 */
static int cd_main(int argc, char **argv)
{
    if (argc == 2) {
        return chdir(argv[1]);
    } else {
        return 0;
    }
}

/*
 * 执行内建的 sbin 命令
 */
static int exec_buildin(int argc, char **argv)
{
    uint8_t *code;
    uint32_t size;

    code = sbin_lookup(argv[0], &size);
    if (code != NULL) {
        return process_create(argv[0], code, size, 5);
    } else {
        printf("unknown cmd\n");
        return -1;
    }
}

/*
 * 执行命令
 */
static int exec_cmd(char *cmd)
{
    static char *argv[ARG_MAX];
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
                if (argc >= ARG_MAX - 1) {
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
        ts_main(argc, argv);
    } else if (strcmp(argv[0], "ls") == 0) {
        extern int ls_main(int argc, char *argv[]);
        ls_main(argc, argv);
    } else if (strcmp(argv[0], "cat") == 0) {
        extern int cat_main(int argc, char *argv[]);
        cat_main(argc, argv);
    } else if (strcmp(argv[0], "cd") == 0) {
        cd_main(argc, argv);
    } else if (strcmp(argv[0], "mems") == 0) {
        kheap_show(STDOUT_FILENO);
    } else if (strcmp(argv[0], "exit") == 0) {
        _exit(0);
    } else {
        exec_buildin(argc, argv);
    }

    return 0;
}

/*
 * logo
 */
const char logo[] =
        "_________________________________________________\n"
        "      __                            __       __\n"
        "    /    )          ,   /         /    )   /    )\n"
        "    \\       _--_       /   ___   /    /    \\\n"
        "     \\     / /  ) /   /   /___) /    /      \\\n"
        "_(____/___/_/__/_/___/___(___ _(____/___(____/___\n";


#define IAC             255         /*  选项协商的第一个字节                                            */
#define WILL            251         /*  发送方激活选项(接收方同意激活选项)                              */
#define WONT            252         /*  接收方不同意                                                    */
#define DO              253         /*  接收方同意(发送方想让接收方激活选项)                            */
#define DONT            254         /*  接受方回应 WONT                                                 */

#define SE              240         /*  子选项结束                                                      */
#define SB              250         /*  子选项开始                                                      */

#define NAWS            31
#define TTY_RATE        32
#define TTY_TYPE        24
#define ECHO_TYPE       1
#define CANCEL_GOAHEAD  3

/*
 * telnetd 线程
 */
static void telnetd_thread(void *arg)
{
    int  ret;
    int  pos;
    char cmd[LINE_MAX];
    u_char ch;

    fclose(stdin);
    stdin = fopen((const char *)arg, "r");

    fclose(stdout);
    stdout = fopen((const char *)arg, "w+");

    printf(logo);

    printf("%s]#", getcwd(NULL, 0));
    fflush(stdout);

    pos = 0;

    while (1) {

        ret = read(STDIN_FILENO, &ch, 1);
        if (ret <= 0) {
            fprintf(stderr, "%s: failed to read socket, errno=%d\n", __func__, errno);
            goto end;
        }

        if (ch == IAC) {
            ret = read(STDIN_FILENO, &ch, 1);
            if (ret <= 0) {
                fprintf(stderr, "%s: failed to read socket, errno=%d\n", __func__, errno);
                goto end;
            }

            if (ch == WILL) {
                ret = read(STDIN_FILENO, &ch, 1);
                if (ret <= 0) {
                    fprintf(stderr, "%s: failed to read socket, errno=%d\n", __func__, errno);
                    goto end;
                }

                switch (ch) {
                case NAWS:
                    break;

                case TTY_RATE:
                    break;

                case TTY_TYPE:
                    break;

                case ECHO_TYPE:
                    break;

                case CANCEL_GOAHEAD:
                    break;

                default:
                    /*
                     * TODO:
                     */
                    break;
                }
            } else if (ch == DO) {
                ret = read(STDIN_FILENO, &ch, 1);
                if (ret <= 0) {
                    fprintf(stderr, "%s: failed to read socket, errno=%d\n", __func__, errno);
                    goto end;
                }

                switch (ch) {
                case CANCEL_GOAHEAD:
                    break;

                default:
                    /*
                     * TODO:
                     */
                    break;
                }
                break;
            } else {
                /*
                 * TODO:
                 */
                break;
            }
        } else {
            if (iscntrl(ch)) {
                switch (ch) {
                case '\b':
                    if (pos > 0) {
                        pos--;
                        printf(" \b \b");
                    } else {
                        putchar('#');
                    }
                    break;

                case '\n':
                    if (pos > 0 && pos < LINE_MAX) {
                        cmd[pos] = '\0';
                        pos = 0;
                        exec_cmd(cmd);
                    }
                    printf("%s]#", getcwd(NULL, 0));
                    break;

                default:
                    break;
                }
            } else if (isprint(ch) && pos < LINE_MAX){
                cmd[pos] = ch;
                pos++;
            }
        }
        fflush(stdout);
    }

    end:
    fclose(stdout);
}

/*
 * telnet 服务器线程
 */
void telnetd(void *arg)
{
    struct sockaddr_in local_addr, remote_addr;
    socklen_t addr_len;
    int fd, client_fd;
    char pty_name[NAME_MAX];
    char thread_name[NAME_MAX];

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) {
        fprintf(stderr, "%s: failed to create socket\n", __func__);
        _exit(-1);
    }

    local_addr.sin_family       = AF_INET;
    local_addr.sin_len          = sizeof(struct sockaddr_in);
    local_addr.sin_addr.s_addr  = INADDR_ANY;
    local_addr.sin_port         = htons(23);

    if (bind(fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        fprintf(stderr, "%s: failed to bind port %d\n", __func__, ntohs(local_addr.sin_port));
        closesocket(fd);
        _exit(-1);
    }

    listen(fd, 2);

    while (1) {
        addr_len = sizeof(remote_addr);

        client_fd = accept(fd, (struct sockaddr *)&remote_addr, &addr_len);
        if (client_fd > 0) {

            sprintf(pty_name, "/dev/pty%d", client_fd);

            pty_create(pty_name, client_fd);

            sprintf(thread_name, "%s%d", __func__, client_fd);

            kthread_create(thread_name, telnetd_thread, pty_name, 8 * KB, 10);
        } else {
            fprintf(stderr, "%s: failed to accept connect\n", __func__);
        }
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
