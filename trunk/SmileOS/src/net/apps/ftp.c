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
** File name:               ftp.c
** Last modified Date:      2013-2-7
** Last Version:            1.0.0
** Descriptions:            小型 FTP 服务器
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-7
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

/*
 * Copyright (c) 2002 Florian Schulze.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the authors nor the names of the contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * ftpd.c - This file is part of the FTP daemon for lwIP
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "lwip/sys.h"
/*********************************************************************************************************
** 配置
*********************************************************************************************************/
#define FTPD_PORT               21
#define FTPD_DATA_PORT          20
#define FTPD_MAX_CONNECTION     10

#define FTPD_STACK_SIZE         (16 * KB)
#define FTPD_CTRL_PRIO          (33)
#define FTPD_SRV_PRIO           (32)

#define dbg_printf              printf
/*********************************************************************************************************
** 回应消息定义
*********************************************************************************************************/
#define msg110 "110 MARK %s = %s."
/*
         110 Restart marker reply.
             In this case, the text is exact and not left to the
             particular implementation; it must read:
                  MARK yyyy = mmmm
             Where yyyy is User-process data stream marker, and mmmm
             server's equivalent marker (note the spaces between markers
             and "=").
*/
#define msg120 "120 Service ready in nnn minutes."
#define msg125 "125 Data connection already open; transfer starting."
#define msg150 "150 File status okay; about to open data connection."
#define msg150recv "150 Opening BINARY mode data connection for %s (%i bytes)."
#define msg150stor "150 Opening BINARY mode data connection for %s."
#define msg200 "200 Command okay."
#define msg202 "202 Command not implemented, superfluous at this site."
#define msg211 "211 System status, or system help reply."
#define msg212 "212 Directory status."
#define msg213 "213 File status."
#define msg214 "214 %s."
/*
             214 Help message.
             On how to use the server or the meaning of a particular
             non-standard command.  This reply is useful only to the
             human user.
*/
#define msg214SYST "214 %s system type."
/*
         215 NAME system type.
             Where NAME is an official system name from the list in the
             Assigned Numbers document.
*/
#define msg220 "220 lwIP FTP Server ready."
/*
         220 Service ready for new user.
*/
#define msg221 "221 Goodbye."
/*
         221 Service closing control connection.
             Logged out if appropriate.
*/
#define msg225 "225 Data connection open; no transfer in progress."
#define msg226 "226 Closing data connection."
/*
             Requested file action successful (for example, file
             transfer or file abort).
*/
#define msg227 "227 Entering Passive Mode (%i,%i,%i,%i,%i,%i)."
/*
         227 Entering Passive Mode (h1,h2,h3,h4,p1,p2).
*/
#define msg230 "230 User logged in, proceed."
#define msg250 "250 Requested file action okay, completed."
#define msg257PWD "257 \"%s\" is current directory."
#define msg257 "257 \"%s\" created."
/*
         257 "PATHNAME" created.
*/
#define msg331 "331 User name okay, need password."
#define msg332 "332 Need account for login."
#define msg350 "350 Requested file action pending further information."
#define msg421 "421 Service not available, closing control connection."
/*
             This may be a reply to any command if the service knows it
             must shut down.
*/
#define msg425 "425 Can't open data connection."
#define msg426 "426 Connection closed; transfer aborted."
#define msg450 "450 Requested file action not taken."
/*
             File unavailable (e.g., file busy).
*/
#define msg451 "451 Requested action aborted: local error in processing."
#define msg452 "452 Requested action not taken."
/*
             Insufficient storage space in system.
*/
#define msg500 "500 Syntax error, command unrecognized."
/*
             This may include errors such as command line too long.
*/
#define msg501 "501 Syntax error in parameters or arguments."
#define msg502 "502 Command not implemented."
#define msg503 "503 Bad sequence of commands."
#define msg504 "504 Command not implemented for that parameter."
#define msg530 "530 Not logged in."
#define msg532 "532 Need account for storing files."
#define msg550 "550 Requested action not taken."
/*
             File unavailable (e.g., file not found, no access).
*/
#define msg551 "551 Requested action aborted: page type unknown."
#define msg552 "552 Requested file action aborted."
/*
             Exceeded storage allocation (for current directory or
             dataset).
*/
#define msg553 "553 Requested action not taken."
/*
             File name not allowed.
*/
/*********************************************************************************************************
** 月分英文表
*********************************************************************************************************/
static const char *month_table[12] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dez"
};
/*********************************************************************************************************
** 控制连接状态
*********************************************************************************************************/
enum ftpd_state_e {
    FTPD_USER,
    FTPD_PASS,
    FTPD_IDLE,
    FTPD_NLST,
    FTPD_LIST,
    FTPD_RETR,
    FTPD_RNFR,
    FTPD_STOR,
    FTPD_QUIT
};
/*********************************************************************************************************
** 数据连接状态
*********************************************************************************************************/
struct ftpd_msgstate;
struct ftpd_datastate {
    enum ftpd_state_e state;
    int fd;
    struct ftpd_msgstate *fsm;
    struct sockaddr_in remote;
    char buffer[4 * KB];
};
/*********************************************************************************************************
** 控制连接状态
*********************************************************************************************************/
struct ftpd_msgstate {
    enum ftpd_state_e state;
    int fd;
    char rx_buffer[256];
    char tx_buffer[256];
    char tmp_buffer[256];
    char *renamefrom;
    struct ftpd_datastate fsd;
};
/*********************************************************************************************************
** 发送消息
*********************************************************************************************************/
static void send_msg(struct ftpd_msgstate *fsm, char *msg, ...)
{
    va_list arg;
    int len;

    va_start(arg, msg);

    vsprintf(fsm->tx_buffer, msg, arg);

    va_end(arg);

    strcat(fsm->tx_buffer, "\r\n");

    len = strlen(fsm->tx_buffer);

    dbg_printf("response: %s", fsm->tx_buffer);

    write(fsm->fd, fsm->tx_buffer, len);
}
/*********************************************************************************************************
** USER 命令
*********************************************************************************************************/
static void cmd_user(const char *arg, struct ftpd_msgstate *fsm)
{
    send_msg(fsm, msg331);

    fsm->state = FTPD_PASS;
}
/*********************************************************************************************************
** PASS 命令
*********************************************************************************************************/
static void cmd_pass(const char *arg, struct ftpd_msgstate *fsm)
{
    send_msg(fsm, msg230);

    fsm->state = FTPD_IDLE;
}
/*********************************************************************************************************
** QUIT 命令
*********************************************************************************************************/
static void cmd_quit(const char *arg, struct ftpd_msgstate *fsm)
{
    send_msg(fsm, msg221);

    fsm->state = FTPD_QUIT;
}
/*********************************************************************************************************
** 发送目录
*********************************************************************************************************/
static void send_directory(struct ftpd_datastate *fsd, DIR *dir, int shortlist)
{
    struct dirent *dirent;
    int len;

    while (1) {
        dirent = readdir(dir);
        if (dirent != NULL) {
            if (shortlist) {
                len = sprintf(fsd->buffer, "%s\r\n", dirent->d_name);
                write(fsd->fd, fsd->buffer, len);
            } else {
                struct stat st;
                time_t current_time;
                int current_year;
                struct tm *s_time;

                time(&current_time);
                s_time = gmtime(&current_time);
                current_year = s_time->tm_year;

                stat(dirent->d_name, &st);

                s_time = gmtime(&st.st_mtime);
                if (s_time->tm_year == current_year) {
                    len = sprintf(fsd->buffer, "-rw-rw-rw-   1 user     ftp  %11ld %s %02i %02i:%02i %s\r\n", st.st_size, month_table[s_time->tm_mon], s_time->tm_mday, s_time->tm_hour, s_time->tm_min, dirent->d_name);
                } else {
                    len = sprintf(fsd->buffer, "-rw-rw-rw-   1 user     ftp  %11ld %s %02i %5i %s\r\n", st.st_size, month_table[s_time->tm_mon], s_time->tm_mday, s_time->tm_year + 1900, dirent->d_name);
                }

                if (S_ISDIR(st.st_mode)) {
                    fsd->buffer[0] = 'd';
                }

                write(fsd->fd, fsd->buffer, len);
            }
        } else {
            break;
        }
    }

    closedir(dir);
    close(fsd->fd);
    send_msg(fsd->fsm, msg226);
}
/*********************************************************************************************************
** 发送文件
*********************************************************************************************************/
static void send_file(struct ftpd_datastate *fsd, int fd)
{
    struct stat st;
    int len;
    int remain;

    fstat(fd, &st);

    remain = st.st_size;

    while (remain > 0) {
        len = min(remain, sizeof(fsd->buffer));
        len = read(fd, fsd->buffer, len);
        printk("read %d byte\n", len);
        if (len > 0) {
            write(fsd->fd, fsd->buffer, len);
            remain -= len;
        } else {
            break;
        }
    }

    close(fd);
    close(fsd->fd);
    send_msg(fsd->fsm, msg226);
}
/*********************************************************************************************************
** 接收文件
*********************************************************************************************************/
static void recv_file(struct ftpd_datastate *fsd, int fd)
{
    int len;
    fd_set readfds;
    fd_set errorfds;
    struct timeval tv;
    int ret;

    while (1) {

        FD_ZERO(&readfds);
        FD_ZERO(&errorfds);

        FD_SET(fsd->fd, &readfds);
        FD_SET(fsd->fd, &errorfds);

        tv.tv_sec  = 1;
        tv.tv_usec = 0;

        ret = select(fsd->fd + 1, &readfds, NULL, &errorfds, &tv);
        if (ret <= 0) {
            break;
        }

        if (FD_ISSET(fsd->fd, &errorfds)) {
            break;
        }

        if (!FD_ISSET(fsd->fd, &readfds)) {
            break;
        }

        len = read(fsd->fd, fsd->buffer, sizeof(fsd->buffer));
        if (len > 0) {
            write(fd, fsd->buffer, len);
        } else {
            break;
        }
    }

    close(fd);
    close(fsd->fd);
    send_msg(fsd->fsm, msg226);
}
/*********************************************************************************************************
** PORT 命令
*********************************************************************************************************/
static void cmd_port(const char *arg, struct ftpd_msgstate *fsm)
{
    int nr;
    unsigned pHi, pLo;
    unsigned ip[4];
    struct ip_addr dataip;
    u16_t dataport;
    struct ftpd_datastate *fsd = &fsm->fsd;

    nr = sscanf(arg, "%u,%u,%u,%u,%u,%u", &(ip[0]), &(ip[1]), &(ip[2]), &(ip[3]), &pHi, &pLo);
    if (nr != 6) {
        send_msg(fsm, msg501);
        return;
    }

    IP4_ADDR(&dataip, (u8_t)ip[0], (u8_t)ip[1], (u8_t)ip[2], (u8_t)ip[3]);
    dataport = ((u16_t)pHi << 8) | (u16_t)pLo;

    fsd->remote.sin_port        = htons(dataport);
    fsd->remote.sin_family      = AF_INET;
    fsd->remote.sin_addr.s_addr = dataip.addr;

    send_msg(fsm, msg200);
}
/*********************************************************************************************************
** CWD 命令
*********************************************************************************************************/
static void cmd_cwd(const char *arg, struct ftpd_msgstate *fsm)
{
    if (chdir(arg) == 0) {
        send_msg(fsm, msg250);
    } else {
        send_msg(fsm, msg550);
    }
}
/*********************************************************************************************************
** CDUP 命令
*********************************************************************************************************/
static void cmd_cdup(const char *arg, struct ftpd_msgstate *fsm)
{
    if (chdir("..") == 0) {
        send_msg(fsm, msg250);
    } else {
        send_msg(fsm, msg550);
    }
}
/*********************************************************************************************************
** PWD 命令
*********************************************************************************************************/
static void cmd_pwd(const char *arg, struct ftpd_msgstate *fsm)
{
    getcwd(fsm->tmp_buffer, sizeof(fsm->tmp_buffer));

    send_msg(fsm, msg257PWD, fsm->tmp_buffer);
}
/*********************************************************************************************************
** PWD 命令
*********************************************************************************************************/
static int open_dataconnection(struct ftpd_msgstate *fsm)
{
    int ret;
    int optval;
    struct sockaddr_in local;
    struct ftpd_datastate *fsd = &fsm->fsd;

    fsd->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fsd->fd < 0) {
        return -1;
    }

    optval = 1;
    ret  = setsockopt(fsd->fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    ret |= setsockopt(fsd->fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    if (ret < 0) {
        close(fsd->fd);
        return -1;
    }

    local.sin_port        = htons(FTPD_DATA_PORT);
    local.sin_family      = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;

    ret = bind(fsd->fd, (struct sockaddr *)&local, sizeof(local));
    if (ret < 0) {
        close(fsd->fd);
        return -1;
    }

    ret = connect(fsd->fd, (const struct sockaddr *)&fsd->remote, sizeof(fsd->remote));
    if (ret < 0) {
        close(fsd->fd);
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** LIST 命令
*********************************************************************************************************/
static void cmd_list_common(const char *arg, struct ftpd_msgstate *fsm, int shortlist)
{
    DIR *dir;

    getcwd(fsm->tmp_buffer, sizeof(fsm->tmp_buffer));

    dir = opendir(fsm->tmp_buffer);
    if (dir == NULL) {
        send_msg(fsm, msg451);
        return;
    }

    send_msg(fsm, msg150);

    if (open_dataconnection(fsm) < 0) {
        send_msg(fsm, msg451);
        closedir(dir);
        return;
    }

    if (shortlist != 0) {
        fsm->fsd.state = FTPD_NLST;
        send_directory(&fsm->fsd, dir, 1);
    } else {
        fsm->fsd.state = FTPD_LIST;
        send_directory(&fsm->fsd, dir, 0);
    }
}
/*********************************************************************************************************
** RETR 命令
*********************************************************************************************************/
static void cmd_retr(const char *arg, struct ftpd_msgstate *fsm)
{
    int fd;
    struct stat st;

    stat(arg, &st);

    if (!S_ISREG(st.st_mode)) {
        send_msg(fsm, msg550);
        return;
    }

    fd = open(arg, O_RDONLY);
    if (fd < 0) {
        send_msg(fsm, msg550);
        return;
    }

    send_msg(fsm, msg150recv, arg, st.st_size);

    if (open_dataconnection(fsm) < 0) {
        send_msg(fsm, msg451);
        close(fd);
        return;
    }

    fsm->fsd.state = FTPD_RETR;
    send_file(&fsm->fsd, fd);
}
/*********************************************************************************************************
** RETR 命令
*********************************************************************************************************/
static void cmd_stor(const char *arg, struct ftpd_msgstate *fsm)
{
    int fd;

    fd = creat(arg, 0666);
    if (fd < 0) {
        send_msg(fsm, msg550);
        return;
    }

    send_msg(fsm, msg150stor, arg);

    if (open_dataconnection(fsm) < 0) {
        send_msg(fsm, msg451);
        close(fd);
        return;
    }

    fsm->fsd.state = FTPD_STOR;
    recv_file(&fsm->fsd, fd);
}
/*********************************************************************************************************
** NLST 命令
*********************************************************************************************************/
static void cmd_nlst(const char *arg, struct ftpd_msgstate *fsm)
{
    cmd_list_common(arg, fsm, 1);
}
/*********************************************************************************************************
** LIST 命令
*********************************************************************************************************/
static void cmd_list(const char *arg, struct ftpd_msgstate *fsm)
{
    cmd_list_common(arg, fsm, 0);
}
/*********************************************************************************************************
** NOOP 命令
*********************************************************************************************************/
static void cmd_noop(const char *arg, struct ftpd_msgstate *fsm)
{
    send_msg(fsm, msg200);
}
/*********************************************************************************************************
** SYST 命令
*********************************************************************************************************/
static void cmd_syst(const char *arg, struct ftpd_msgstate *fsm)
{
    send_msg(fsm, msg214SYST, "UNIX");
}
/*********************************************************************************************************
** ABRT 命令
*********************************************************************************************************/
static void cmd_abrt(const char *arg, struct ftpd_msgstate *fsm)
{

}
/*********************************************************************************************************
** TYPE 命令
*********************************************************************************************************/
static void cmd_type(const char *arg, struct ftpd_msgstate *fsm)
{
    send_msg(fsm, msg200);
}
/*********************************************************************************************************
** MODE 命令
*********************************************************************************************************/
static void cmd_mode(const char *arg, struct ftpd_msgstate *fsm)
{
    send_msg(fsm, msg502);
}
/*********************************************************************************************************
** RNFR 命令
*********************************************************************************************************/
static void cmd_rnfr(const char *arg, struct ftpd_msgstate *fsm)
{
    if (arg == NULL) {
        send_msg(fsm, msg501);
        return;
    }

    if (*arg == '\0') {
        send_msg(fsm, msg501);
        return;
    }

    if (fsm->renamefrom) {
        free(fsm->renamefrom);
        fsm->renamefrom = NULL;
    }

    fsm->renamefrom = strdup(arg);
    if (fsm->renamefrom == NULL) {
        send_msg(fsm, msg451);
        return;
    }

    fsm->state = FTPD_RNFR;
    send_msg(fsm, msg350);
}
/*********************************************************************************************************
** RNTO 命令
*********************************************************************************************************/
static void cmd_rnto(const char *arg, struct ftpd_msgstate *fsm)
{
    if (fsm->state != FTPD_RNFR) {
        send_msg(fsm, msg503);
        return;
    }

    fsm->state = FTPD_IDLE;
    if (arg == NULL) {
        send_msg(fsm, msg501);
        return;
    }

    if (*arg == '\0') {
        send_msg(fsm, msg501);
        return;
    }

    if (rename(fsm->renamefrom, arg)) {
        send_msg(fsm, msg450);
    } else {
        send_msg(fsm, msg250);
    }

    free(fsm->renamefrom);
    fsm->renamefrom = NULL;
}
/*********************************************************************************************************
** MKD 命令
*********************************************************************************************************/
static void cmd_mkd(const char *arg, struct ftpd_msgstate *fsm)
{
    if (arg == NULL) {
        send_msg(fsm, msg501);
        return;
    }

    if (*arg == '\0') {
        send_msg(fsm, msg501);
        return;
    }

    if (mkdir(arg, 0777) != 0) {
        send_msg(fsm, msg550);
    } else {
        send_msg(fsm, msg257, arg);
    }
}
/*********************************************************************************************************
** RMD 命令
*********************************************************************************************************/
static void cmd_rmd(const char *arg, struct ftpd_msgstate *fsm)
{
    struct stat st;

    if (arg == NULL) {
        send_msg(fsm, msg501);
        return;
    }

    if (*arg == '\0') {
        send_msg(fsm, msg501);
        return;
    }

    if (stat(arg, &st) != 0) {
        send_msg(fsm, msg550);
        return;
    }

    if (!S_ISDIR(st.st_mode)) {
        send_msg(fsm, msg550);
        return;
    }

    if (rmdir(arg) != 0) {
        send_msg(fsm, msg550);
    } else {
        send_msg(fsm, msg250);
    }
}
/*********************************************************************************************************
** DELE 命令
*********************************************************************************************************/
static void cmd_dele(const char *arg, struct ftpd_msgstate *fsm)
{
    struct stat st;

    if (arg == NULL) {
        send_msg(fsm, msg501);
        return;
    }

    if (*arg == '\0') {
        send_msg(fsm, msg501);
        return;
    }

    if (stat(arg, &st) != 0) {
        send_msg(fsm, msg550);
        return;
    }

    if (!S_ISREG(st.st_mode)) {
        send_msg(fsm, msg550);
        return;
    }

    if (remove(arg) != 0) {
        send_msg(fsm, msg550);
    } else {
        send_msg(fsm, msg250);
    }
}
/*********************************************************************************************************
** FTP 命令
*********************************************************************************************************/
struct ftpd_command {
    const char *cmd;
    void (*func)(const char *arg, struct ftpd_msgstate *fsm);
};

static struct ftpd_command ftpd_commands[] = {
    "USER", cmd_user,
    "PASS", cmd_pass,
    "PORT", cmd_port,
    "QUIT", cmd_quit,
    "CWD",  cmd_cwd,
    "CDUP", cmd_cdup,
    "PWD",  cmd_pwd,
    "XPWD", cmd_pwd,
    "NLST", cmd_nlst,
    "LIST", cmd_list,
    "RETR", cmd_retr,
    "STOR", cmd_stor,
    "NOOP", cmd_noop,
    "SYST", cmd_syst,
    "ABOR", cmd_abrt,
    "TYPE", cmd_type,
    "MODE", cmd_mode,
    "RNFR", cmd_rnfr,
    "RNTO", cmd_rnto,
    "MKD",  cmd_mkd,
    "XMKD", cmd_mkd,
    "RMD",  cmd_rmd,
    "XRMD", cmd_rmd,
    "DELE", cmd_dele,
    NULL, NULL
};
/*********************************************************************************************************
** FTPD 控制连接线程
*********************************************************************************************************/
static void ftpd_ctrlconnection(void *arg)
{
    int fd = (int)arg;
    int len;
    char *text;
    char *pt;
    char cmd[5];
    struct ftpd_command *ftpd_cmd;
    struct ftpd_msgstate *fsm;

    fsm = malloc(sizeof(struct ftpd_msgstate));
    if (fsm == NULL) {
        goto error0;
    }
    memset(fsm, 0, sizeof(struct ftpd_msgstate));

    fsm->state   = FTPD_IDLE;
    fsm->fd      = fd;
    fsm->fsd.fsm = fsm;

    text = fsm->rx_buffer;

    send_msg(fsm, msg220);

    while (1) {
        len = read(fd, text, sizeof(fsm->rx_buffer) - 1);
        if (len < 0) {
            break;
        }

        if (len == 0 && errno == ENOTCONN) {
            break;
        }

        if (len == 0) {
            continue;
        }

        pt = text;

        pt[len] = '\0';

        pt = &text[len - 1];
        while (((*pt == '\r') || (*pt == '\n')) && pt >= text) {
            *pt-- = '\0';
        }

        dbg_printf("query: %s\n", text);

        strncpy(cmd, text, 4);
        for (pt = cmd; isalpha(*pt) && pt < &cmd[4]; pt++) {
            *pt = toupper(*pt);
        }
        *pt = '\0';

        for (ftpd_cmd = ftpd_commands; ftpd_cmd->cmd != NULL; ftpd_cmd++) {
            if (!strcmp(ftpd_cmd->cmd, cmd)) {
                break;
            }
        }

        if (len < (strlen(cmd) + 1)) {
            pt = "";
        } else {
            pt = &text[strlen(cmd) + 1];
        }

        if (ftpd_cmd->func) {
            ftpd_cmd->func(pt, fsm);
        } else {
            send_msg(fsm, msg502);
        }
    }

    dbg_printf("ftp control connection exit\n");

    if (fsm->renamefrom) {
        free(fsm->renamefrom);
        fsm->renamefrom = NULL;
    }

    free(fsm);

    error0:
    close(fd);
}
/*********************************************************************************************************
** FTPD 服务器线程
*********************************************************************************************************/
static void ftpd(void *arg)
{
    int fd;
    int new_fd;
    int ret;
    struct sockaddr_in local;
    struct sockaddr_in remote;
    socklen_t addr_len;
    char name[NAME_MAX];

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        return;
    }

    local.sin_port        = htons(FTPD_PORT);
    local.sin_family      = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;

    ret = bind(fd, (struct sockaddr *)&local, sizeof(local));
    if (ret < 0) {
        close(fd);
        return;
    }

    listen(fd, FTPD_MAX_CONNECTION);

    while (1) {
        addr_len = sizeof(struct sockaddr_in);

        new_fd = accept(fd, (struct sockaddr *)&remote, &addr_len);
        if (new_fd < 0) {
            continue;
        }

        sprintf(name, "ftpd_ctrl%d", new_fd);

        if (sys_thread_new(name, ftpd_ctrlconnection, (void *)new_fd, FTPD_STACK_SIZE, FTPD_CTRL_PRIO) < 0) {
            close(new_fd);
        }
    }

    close(fd);

    dbg_printf("ftp server exit\n");
}
/*********************************************************************************************************
** 初始化 FTPD 服务器
*********************************************************************************************************/
void ftpd_init(void)
{
    sys_thread_new("ftpd", ftpd, NULL, FTPD_STACK_SIZE, FTPD_SRV_PRIO);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
