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
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "drv/pty.h"
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
    struct stat st;

    if (stat(argv[0], &st) < 0) {
        fprintf(stderr, "%s no found!\n", argv[0]);
        return -1;
    }

    return process_create(argv[0], 5);
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
    } else if (strcmp(argv[0], "cd") == 0) {
        cd_main(argc, argv);
    } else if (strcmp(argv[0], "cat") == 0) {
        extern int cat_main(int argc, char *argv[]);
        cat_main(argc, argv);
    } else if (strcmp(argv[0], "clear") == 0) {
        extern int clear_main(int argc, char *argv[]);
        clear_main(argc, argv);
    } else if (strcmp(argv[0], "cp") == 0) {
        extern int cp_main(int argc, char *argv[]);
        cp_main(argc, argv);
    } else if (strcmp(argv[0], "echo") == 0) {
        extern int echo_main(int argc, char *argv[]);
        echo_main(argc, argv);
    } else if (strcmp(argv[0], "head") == 0) {
        extern int head_main(int argc, char *argv[]);
        head_main(argc, argv);
    } else if (strcmp(argv[0], "ls") == 0) {
        extern int ls_main(int argc, char *argv[]);
        ls_main(argc, argv);
    } else if (strcmp(argv[0], "mkdir") == 0) {
        extern int mkdir_main(int argc, char *argv[]);
        mkdir_main(argc, argv);
    } else if (strcmp(argv[0], "more") == 0) {
        extern int more_main(int argc, char *argv[]);
        more_main(argc, argv);
    } else if (strcmp(argv[0], "mv") == 0) {
        extern int mv_main(int argc, char *argv[]);
        mv_main(argc, argv);
    } else if (strcmp(argv[0], "envs") == 0) {
        extern int printenv_main(int argc, char *argv[]);
        printenv_main(argc, argv);
    } else if (strcmp(argv[0], "pwd") == 0) {
        extern int pwd_main(int argc, char *argv[]);
        pwd_main(argc, argv);
    } else if (strcmp(argv[0], "rm") == 0) {
        extern int rm_main(int argc, char *argv[]);
        rm_main(argc, argv);
    } else if (strcmp(argv[0], "rmdir") == 0) {
        extern int rmdir_main(int argc, char *argv[]);
        rmdir_main(argc, argv);
    } else if (strcmp(argv[0], "touch") == 0) {
        extern int touch_main(int argc, char *argv[]);
        touch_main(argc, argv);
    } else if (strcmp(argv[0], "vi") == 0) {
        extern int vi_main(int argc, char *argv[]);
        vi_main(argc, argv);
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

#include <termios.h>

/*
 * Special telnet characters
 */
#define TELNET_SE    240   // End of subnegotiation parameters
#define TELNET_NOP   241   // No operation
#define TELNET_MARK  242   // Data mark
#define TELNET_BRK   243   // Break
#define TELNET_IP    244   // Interrupt process
#define TELNET_AO    245   // Abort output
#define TELNET_AYT   246   // Are you there
#define TELNET_EC    247   // Erase character
#define TELNET_EL    248   // Erase line
#define TELNET_GA    249   // Go ahead
#define TELNET_SB    250   // Start of subnegotiation parameters
#define TELNET_WILL  251   // Will option code
#define TELNET_WONT  252   // Won't option code
#define TELNET_DO    253   // Do option code
#define TELNET_DONT  254   // Don't option code
#define TELNET_IAC   255   // Interpret as command

/*
 * Telnet options
 */
#define TELOPT_TRANSMIT_BINARY      0  // Binary Transmission (RFC856)
#define TELOPT_ECHO                 1  // Echo (RFC857)
#define TELOPT_SUPPRESS_GO_AHEAD    3  // Suppress Go Ahead (RFC858)
#define TELOPT_STATUS               5  // Status (RFC859)
#define TELOPT_TIMING_MARK          6  // Timing Mark (RFC860)
#define TELOPT_NAOCRD              10  // Output Carriage-Return Disposition (RFC652)
#define TELOPT_NAOHTS              11  // Output Horizontal Tab Stops (RFC653)
#define TELOPT_NAOHTD              12  // Output Horizontal Tab Stop Disposition (RFC654)
#define TELOPT_NAOFFD              13  // Output Formfeed Disposition (RFC655)
#define TELOPT_NAOVTS              14  // Output Vertical Tabstops (RFC656)
#define TELOPT_NAOVTD              15  // Output Vertical Tab Disposition (RFC657)
#define TELOPT_NAOLFD              16  // Output Linefeed Disposition (RFC658)
#define TELOPT_EXTEND_ASCII        17  // Extended ASCII (RFC698)
#define TELOPT_TERMINAL_TYPE       24  // Terminal Type (RFC1091)
#define TELOPT_NAWS                31  // Negotiate About Window Size (RFC1073)
#define TELOPT_TERMINAL_SPEED      32  // Terminal Speed (RFC1079)
#define TELOPT_TOGGLE_FLOW_CONTROL 33  // Remote Flow Control (RFC1372)
#define TELOPT_LINEMODE            34  // Linemode (RFC1184)
#define TELOPT_AUTHENTICATION      37  // Authentication (RFC1416)

#define STATE_NORMAL 0
#define STATE_IAC    1
#define STATE_OPT    2
#define STATE_SB     3
#define STATE_OPTDAT 4
#define STATE_SE     5

static void sendopt(int code, int option)
{
    unsigned char buf[3];

    buf[0] = TELNET_IAC;
    buf[1] = (unsigned char)code;
    buf[2] = (unsigned char)option;
    write(STDOUT_FILENO, buf, 3);
}

static void parseopt(int code, int option)
{
    switch (option) {
    case TELOPT_ECHO:
    case TELOPT_SUPPRESS_GO_AHEAD:
    case TELOPT_NAWS:
        break;

    case TELOPT_TERMINAL_TYPE:
    case TELOPT_TERMINAL_SPEED:
        sendopt(TELNET_DO, option);
        break;

    default:
        if (code == TELNET_WILL || code == TELNET_WONT) {
            sendopt(TELNET_DONT, option);
        } else {
            sendopt(TELNET_WONT, option);
        }
    }
}

static void parseoptdat(int option, unsigned char *data, int len)
{
    switch (option) {
    case TELOPT_NAWS:
        if (len == 4) {
            int cols  = ntohs(*(unsigned short *)data);
            int lines = ntohs(*(unsigned short *)(data + 2));
        }
        break;

    case TELOPT_TERMINAL_SPEED:
        break;

    case TELOPT_TERMINAL_TYPE:
        break;
    }
}

/*
 * telnetd 线程
 */
static void telnetd_thread(void *arg)
{
    int  len;
    int  pos;
    char cmd[LINE_MAX];
    char buf[LINE_MAX];
    unsigned char ch;
    struct termios termbuf;
    int i;
    int state;
    int code;
    int optlen;
    unsigned char optdata[32];

    /*
     * 重定向标准输入输出到 PTY
     */
    fclose(stdin);
    stdin = fopen((const char *)arg, "r");

    fclose(stdout);
    stdout = fopen((const char *)arg, "w+");

    fclose(stderr);
    stderr = fopen((const char *)arg, "w+");

    /*
     * 设置终端属性
     */
    tcgetattr(0, &termbuf);
    termbuf.c_lflag  = ECHO | ICANON;
    termbuf.c_oflag |= ONLCR | OXTABS;
    termbuf.c_iflag |= IGNCR;
    termbuf.c_iflag &= ~IXOFF;
    tcsetattr(0, TCSANOW, &termbuf);

    /*
     * 发送选项
     */
    sendopt(TELNET_WILL, TELOPT_ECHO);
    sendopt(TELNET_WILL, TELOPT_SUPPRESS_GO_AHEAD);
    sendopt(TELNET_WONT, TELOPT_LINEMODE);
    sendopt(TELNET_WILL, TELOPT_NAWS);

    /*
     * 发送 logo
     */
    printf(logo);

    /*
     * 发送
     */
    printf("%s]#", getcwd(NULL, 0));

    pos = 0;

    state = STATE_NORMAL;

    fflush(stdout);

    while (1) {

        /*
         * 接收数据
         */
        len = read(STDIN_FILENO, &buf, LINE_MAX);
        if (len <= 0) {
            fprintf(stderr, "%s: failed to read socket, errno=%d\n", __func__, errno);
            goto end;
        }

        i = 0;
        while (len-- > 0) {
            ch = buf[i++];

            switch (state) {
              case STATE_NORMAL:
                if (ch == TELNET_IAC) {
                    state = STATE_IAC;
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
                            fflush(stdout);
                            break;

                        case '\n':
                            if (pos > 0 && pos < LINE_MAX) {
                                cmd[pos] = '\0';
                                pos = 0;
                                exec_cmd(cmd);
                            }
                            printf("%s]#", getcwd(NULL, 0));
                            fflush(stdout);
                            break;

                        default:
                            break;
                        }
                    } else if (isprint(ch) && pos < LINE_MAX){
                        cmd[pos] = ch;
                        pos++;
                    }
                }
                break;

              case STATE_IAC:
                  switch (ch) {
                  case TELNET_IAC:
                      if (pos < LINE_MAX) {
                          cmd[pos] = ch;
                          pos++;
                      }
                      state = STATE_NORMAL;
                      break;

                  case TELNET_WILL:
                  case TELNET_WONT:
                  case TELNET_DO:
                  case TELNET_DONT:
                      code = ch;
                      state = STATE_OPT;
                      break;

                  case TELNET_SB:
                      state = STATE_SB;
                      break;

                  default:
                      state = STATE_NORMAL;
                  }
                  break;

            case STATE_OPT:
                parseopt(code, ch);
                state = STATE_NORMAL;
                break;

            case STATE_SB:
                code = ch;
                optlen = 0;
                state = STATE_OPTDAT;
                break;

            case STATE_OPTDAT:
                if (ch == TELNET_IAC) {
                    state = STATE_SE;
                } else if (optlen < sizeof(optdata)) {
                    optdata[optlen++] = ch;
                }
                break;

            case STATE_SE:
                if (ch == TELNET_SE) {
                    parseoptdat(code, optdata, optlen);
                }
                state = STATE_NORMAL;
                break;
            }
        }
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
    char pty_name[PATH_MAX];
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

            pty_create(pty_name,
                      (int  (*)(void *))socket_attach,
                      (void (*)(void *))lwip_close,
                      (void  *)client_fd);

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
