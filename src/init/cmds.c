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
** File name:               cmds.c
** Last modified Date:      2012-6-7
** Last Version:            1.0.0
** Descriptions:            命令集
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-6-7
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
#include <termios.h>

/*
 * 执行命令
 */
int do_cmd(char *cmd)
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
        extern int ts_main(int argc, char **argv);
        ts_main(argc, argv);
    } else if (strcmp(argv[0], "cd") == 0) {
        extern int cd_main(int argc, char **argv);
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
        struct stat st;

        if (stat(argv[0], &st) < 0) {
            fprintf(stderr, "%s no found!\n", argv[0]);
            return -1;
        }

        process_create(argv[0], 5);
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
