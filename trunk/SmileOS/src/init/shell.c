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
#include "kern/kern.h"
#include "cmds/cmds.h"
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*********************************************************************************************************
** logo
*********************************************************************************************************/
static const char smileos_logo[] =
" [[[[                      [[[[   [[[[  \r\n"
"[    [                    [    [ [    [ \r\n"
"[             @ [    [[[  [    [ [      \r\n"
" [[[[   [  [    [   [   [ [    [  [[[[  \r\n"
"     [ [ [[ [ [ [   [[[[  [    [      [ \r\n"
"[    [ [    [ [ [   [     [    [ [    [ \r\n"
" [[[[  [    [ [ [[[  [[[[  [[[[   [[[[  \r\n";
/*********************************************************************************************************
** Function name:           run_cmd
** Descriptions:            执行命令
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int run_cmd(int argc, char **argv)
{
    cmd_t *cmd;
    int    ret = -1;

    extern cmd_t __smileos_cmd_start;
    extern cmd_t __smileos_cmd_end;
    for (cmd = &__smileos_cmd_start;
         cmd != &__smileos_cmd_end;
         cmd++) {
        if (strcmp(argv[0], cmd->name) == 0) {
            ret = cmd->cmd(argc, argv);
            break;
        }
    }

    if (cmd == &__smileos_cmd_end) {
        struct stat st;

        if (stat(argv[0], &st) < 0) {
            fprintf(stderr, "%s no found!\n", argv[0]);
            return -1;
        }

        ret = process_create(argv[0], 5, argc, argv);
    }

    return ret;
}
static char erase_seq[] = "\b \b";      /* erase sequence   */
static char   tab_seq[] = "        ";       /* used to expand TABs  */

/*********************************************************************************************************
** Function name:           parse_line
** Descriptions:            分析行
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int parse_line(char *line, char *argv[])
{
    int argc = 0;

    while (argc < ARG_MAX) {
        /*
         * skip any white space
         */
        while ((*line == ' ') || (*line == '\t')) {
            ++line;
        }

        if (*line == '\0') {                                            /*  end of line, no more args   */
            argv[argc] = NULL;
            return (argc);
        }

        argv[argc++] = line;                                            /*  begin of argument string    */

        /*
         * find end of string
         */
        while (*line && (*line != ' ') && (*line != '\t')) {
            ++line;
        }

        if (*line == '\0') {                                            /*  end of line, no more args   */
            argv[argc] = NULL;
            return (argc);
        }

        *line++ = '\0';                                                 /*  terminate current arg       */
    }

    printf("** Too many args (max. %d) **\n", ARG_MAX);

    return (argc);
}
/*********************************************************************************************************
** Function name:           process_macros
** Descriptions:            处理宏
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void process_macros(const char *input, char *output)
{
    char c, prev;
    const char *varname_start = NULL;
    int inputcnt  = strlen(input);
    int outputcnt = LINE_MAX;
    int state = 0;  /* 0 = waiting for '$'          */
                    /* 1 = waiting for '(' or '{'   */
                    /* 2 = waiting for ')' or '}'   */
                    /* 3 = waiting for '''          */

    prev = '\0';    /* previous character           */

    while (inputcnt && outputcnt) {
        c = *input++;
        inputcnt--;

        if (state != 3) {
            /* remove one level of escape characters */
            if ((c == '\\') && (prev != '\\')) {
                if (inputcnt-- == 0)
                    break;
                prev = c;
                c = *input++;
            }
        }

        switch (state) {
        case 0:         /* Waiting for (unescaped) $ */
            if ((c == '\'') && (prev != '\\')) {
                state = 3;
                break;
            }
            if ((c == '$') && (prev != '\\')) {
                state++;
            } else {
                *(output++) = c;
                outputcnt--;
            }
        break;

        case 1:         /* Waiting for ( */
            if (c == '(' || c == '{') {
                state++;
                varname_start = input;
            } else {
                state = 0;
                *(output++) = '$';
                outputcnt--;

                if (outputcnt) {
                    *(output++) = c;
                    outputcnt--;
                }
            }
        break;

        case 2:         /* Waiting for )    */
            if (c == ')' || c == '}') {
                int i;
                char envname[LINE_MAX], *envval;
                /* Varname # of chars */
                int envcnt = input - varname_start - 1;

                /* Get the varname */
                for (i = 0; i < envcnt; i++) {
                    envname[i] = varname_start[i];
                }
                envname[i] = 0;

                /* Get its value */
                envval = getenv(envname);

                /* Copy into the line if it exists */
                if (envval != NULL)
                    while ((*envval) && outputcnt) {
                        *(output++) = *(envval++);
                        outputcnt--;
                    }
                /* Look for another '$' */
                state = 0;
            }
        break;

        case 3:         /* Waiting for '    */
            if ((c == '\'') && (prev != '\\')) {
                state = 0;
            } else {
                *(output++) = c;
                outputcnt--;
            }
        break;

        default:
            break;
        }
        prev = c;
    }

    if (outputcnt)
        *output = 0;
}
/*********************************************************************************************************
** Function name:           run_command
** Descriptions:            分析命令
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int run_command(const char *cmd)
{
    char  cmdbuf[LINE_MAX];
    char *argv[ARG_MAX + 1];
    int   argc;

    if (!cmd || !*cmd) {                                                /*  空命令                      */
        return -1;
    }

    if (strlen(cmd) >= LINE_MAX) {                                      /*  命令太长                    */
        printf("## Command too long!\n");
        return -1;
    }

    process_macros(cmd, cmdbuf);                                        /*  处理宏                      */

    if ((argc = parse_line(cmdbuf, argv)) == 0) {                       /*  提取参数                    */
        return -1;
    }

    return run_cmd(argc, argv);                                         /*  运行命令                    */
}
/*********************************************************************************************************
** Function name:           delete_char
** Descriptions:            删除字符
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static char *delete_char(char *buf, char *p, int *colp, int *np, int plen)
{
    char *s;

    if (*np == 0) {
        putchar('\b');
        putchar(' ');
        return (p);
    }

    if (*(--p) == '\t') {                                               /*  will retype the whole line  */
        while (*colp > plen) {
            printf(erase_seq);
            (*colp)--;
        }
        for (s = buf; s < p; ++s) {
            if (*s == '\t') {
                printf(tab_seq + ((*colp) & 07));
                *colp += 8 - ((*colp) & 07);
            } else {
                ++(*colp);
                putchar(*s);
            }
        }
    } else {
        printf(erase_seq);
        (*colp)--;
    }
    (*np)--;
    return (p);
}
/*********************************************************************************************************
** Function name:           read_line
** Descriptions:            读一行输入
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int read_line(char *buf)
{
    char c;
    char *p = buf;
    char *p_buf = buf;
    int n = 0;
    int plen;
    int col;

    plen = sprintf(buf, "\r\n%s]# ", getcwd(NULL, 0));
    printf(buf);

    buf[0] = '\0';

    col = plen;

    for (;;) {
        fflush(stdout);

        c = getchar();                                                  /*  获得一个字符                */

        switch (c) {
        case '\r':                                                      /*  Enter                       */
        case '\n':
            *p = '\0';
            printf("\r\n");
            return (p - p_buf);

        case '\0':                                                      /*  nul                         */
            continue;

        case 0x03:                                                      /*  ^C - break                  */
            p_buf[0] = '\0';                                            /*  discard input               */
            return (-1);

        case 0x15:                                                      /*  ^U - erase line             */
            while (col > plen) {
                printf(erase_seq);
                --col;
            }
            p = p_buf;
            n = 0;
            continue;

        case 0x17:                                                      /*  ^W - erase word             */
            p = delete_char(p_buf, p, &col, &n, plen);
            while ((n > 0) && (*p != ' ')) {
                p = delete_char(p_buf, p, &col, &n, plen);
            }
            continue;

        case 0x08:                                                      /*  ^H  - backspace             */
        case 0x7F:                                                      /*  DEL - backspace             */
            p = delete_char(p_buf, p, &col, &n, plen);
            continue;

        default:
            /*
             * Must be a normal character then
             */
            if (n < LINE_MAX - 2) {
                if (c == '\t') {                                        /*  expand TABs                 */
                    printf(tab_seq + (col & 07));
                    col += 8 - (col & 07);
                } else {
                    ++col;                                              /*  echo input                  */
                    putchar(c);
                }
                *p++ = c;
                ++n;
            } else {                                                    /*  Buffer full                 */
                putchar('\a');
            }
        }
    }
}
/*********************************************************************************************************
** Function name:           shell
** Descriptions:            shell 函数
** input parameters:        arg                 参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void shell(void *arg)
{
    char buf[LINE_MAX];

    /*
     * 重定向标准输入输出到串口0
     */
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    open("/dev/serial0", O_RDWR);
    dup(STDIN_FILENO);
    dup(STDIN_FILENO);

    stdin  = fdopen(STDIN_FILENO,  "r");

    stdout = fdopen(STDOUT_FILENO, "w+");

    stderr = fdopen(STDERR_FILENO, "w+");

    /*
     * 打印 logo
     */
    printf(smileos_logo);

    while (1) {
        int len = read_line(buf);
        if (len > 0) {
             run_command(buf);
        }
    }

    fclose(stdout);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
