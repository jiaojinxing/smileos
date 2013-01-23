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
** File name:               shell.c
** Last modified Date:      2012-3-6
** Last Version:            1.0.0
** Descriptions:            shell
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
#include "cmds/cmds.h"
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <termios.h>
/*********************************************************************************************************
** Function name:           do_cmd
** Descriptions:            执行命令
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int parse_line(char *line)
{
    char *argv[ARG_MAX];
    char *p, *word = NULL;
    int argc = 0;
    cmd_t *cmd;

    if (line[0] != ' ' && line[0] != '\t') {
        word = line;
    }

    p = line;
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

    extern cmd_t __smileos_cmd_start;
    extern cmd_t __smileos_cmd_end;
    for (cmd = &__smileos_cmd_start;
         cmd != &__smileos_cmd_end;
         cmd++) {
        if (strcmp(argv[0], cmd->name) == 0) {
            cmd->cmd(argc, argv);
            break;
        }
    }



//    } else if (strcmp(argv[0], "module") == 0) {
//        extern int module_load(const char *path, int argc, char **argv);
//        module_load(argv[1], argc - 1, &argv[1]);
//    } else {
//        struct stat st;
//
//        if (stat(argv[0], &st) < 0) {
//            fprintf(stderr, "%s no found!\n", argv[0]);
//            return -1;
//        }
//
//        process_create(argv[0], 5, argc, argv);
//    }

    return 0;
}
/*********************************************************************************************************
** logo
*********************************************************************************************************/
static const char smileos_logo[] =
" [[[[                      [[[[   [[[[  \n"
"[    [                    [    [ [    [ \n"
"[             @ [    [[[  [    [ [      \n"
" [[[[   [  [    [   [   [ [    [  [[[[  \n"
"     [ [ [[ [ [ [   [[[[  [    [      [ \n"
"[    [ [    [ [ [   [     [    [ [    [ \n"
" [[[[  [    [ [ [[[  [[[[  [[[[   [[[[  \n";
/*********************************************************************************************************
** Function name:           shell
** Descriptions:            shell 函数
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
void shell(void *arg)
{
    int  len;
    int  pos;
    char line[LINE_MAX];
    char buf[LINE_MAX];
    unsigned char ch;
    struct termios termbuf;
    int i;

    /*
     * 重定向标准输入输出到 PTY
     */
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    open("/dev/ttyS0", O_RDWR);
    dup(STDIN_FILENO);
    dup(STDIN_FILENO);

    stdin  = fdopen(STDIN_FILENO, "r");

    stdout = fdopen(STDOUT_FILENO, "w+");

    stderr = fdopen(STDERR_FILENO, "w+");

    /*
     * 设置终端属性
     */
    tcgetattr(0, &termbuf);
    termbuf.c_lflag  = ECHO | ICANON;
    termbuf.c_oflag |= ONLCR | OXTABS;
    termbuf.c_iflag &= ~IXOFF;
    tcsetattr(0, TCSANOW, &termbuf);

    /*
     * 发送 logo
     */
    printf(smileos_logo);

    /*
     * 发送提示符
     */
    printf("%s]#", getcwd(NULL, 0));

    fflush(stdout);

    pos = 0;

    while (1) {
        /*
         * 接收数据
         */
        len = read(STDIN_FILENO, buf, LINE_MAX);
        if (len < 0) {
            fprintf(stderr, "%s: failed to read stdin, errno=%d\n", __func__, errno);
            goto end;
        }

        i = 0;
        while (len-- > 0) {
            ch = buf[i++];

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
                        line[pos] = '\0';
                        pos = 0;
                        parse_line(line);
                    }
                    printf("%s]#", getcwd(NULL, 0));
                    fflush(stdout);
                    break;

                default:
                    break;
                }
            } else if (isprint(ch) && pos < LINE_MAX) {
                line[pos] = ch;
                pos++;
            } else {

            }
        }
    }

    end:
    fclose(stdout);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
