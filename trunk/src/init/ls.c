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
** File name:               ls
** Last modified Date:      2012-3-29
** Last Version:            1.0.0
** Descriptions:            ls 命令
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-29
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
#include "vfs/vfs.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>

static void printentry(char *name, struct stat *st);
static int do_ls(char *path);

/* Flags */
#define LSF_DOT         0x01    /* List files begining with . */
#define LSF_LONG        0x02    /* Long format */
#define LSF_SINGLE      0x04    /* Single column */
#define LSF_TYPE        0x08    /* Add /(dir) and @(symlink) with file name */
#define LSF_ALL         0x10    /* List hidden files */

#define LSF_RECURSIVE   0x20    /* List Subdirectory */
#define LSF_TIMESORT    0x40    /* Sort by time */

#define DEFAULT_WIDTH   80

static unsigned int ls_flags;
static int termwidth;
static int cols;

int
ls_main(int argc, char *argv[])
{
    int ch, ret;

    ls_flags = 0;

    optind = 0;

    while ((ch = getopt(argc, argv, "1ClFaA?")) != -1) {
        switch (ch) {
        case '1':
            ls_flags |= LSF_SINGLE;
            ls_flags &= ~LSF_LONG;
            break;

        case 'C':
            ls_flags &= ~LSF_SINGLE;
            ls_flags &= ~LSF_LONG;
            break;

        case 'l':
            ls_flags |= LSF_LONG;
            ls_flags &= ~LSF_SINGLE;
            break;

        case 'F':
            ls_flags |= LSF_TYPE;
            break;

        case 'a':
            ls_flags |= LSF_DOT;
            /* FALLTHROUGH */

        case 'A':
            ls_flags |= LSF_ALL;
            break;

        default:
        case '?':
            fprintf(stderr, "usage: ls [-1CFAal] [file ...]\r\n");
            return 0;
        }
    }

    argc -= optind;
    argv += optind;

    termwidth = DEFAULT_WIDTH;

    if (argc == 0) {
        ret = do_ls(".");
    } else {
        do {
            ret = do_ls(*argv);
            ++argv;
        } while (*argv);
    }

    return ret;
}

static void
printtype(u_int mode)
{
    char type;

    switch (mode & S_IFMT) {
    case S_IFIFO:
        type = 'p';
        break;

    case S_IFCHR:
        type = 'c';
        break;

    case S_IFDIR:
        type = 'd';
        break;

    case S_IFBLK:
        type = 'b';
        break;

    case S_IFLNK:
        type = 'l';
        break;

    case S_IFSOCK:
        type = 's';
        break;

    case S_IFREG:
    default:
        type = '-';
        break;
    }

    putchar(type);
}

static void
printmode(u_int mode)
{
    if (mode & S_IRUSR) {
        putchar('r');
    } else {
        putchar('-');
    }

    if (mode & S_IWUSR) {
        putchar('w');
    } else {
        putchar('-');
    }

    if (mode & S_IXUSR) {
        putchar('x');
    } else {
        putchar('-');
    }
}

static void
printentry(char *name, struct stat *st)
{
    int color;
    int dot = 0;
    int len;

    if (name[0] == '.') {
        if ((ls_flags & LSF_DOT) == 0) {
            return;
        }
        dot = 1;
    }

    if (ls_flags & LSF_LONG) {
        printtype(st->st_mode);

        printmode(st->st_mode);
        printf("------");

        /* print link */
        printf("  1 ");

        /* print owner */
        printf("admin  ");

        /* print date/time */
        printf("%s 12:00 ", __DATE__);

        /* print size */
        printf("%7d ", (int)st->st_size);
    }

    /* set color */
    color = 0;
    if (S_ISCHR(st->st_mode) || S_ISBLK(st->st_mode)) {
        color = 35;  /* magenta */
    } else if (S_ISDIR(st->st_mode)) {
        color = 36;  /* cyan */
    } else if (S_ISFIFO(st->st_mode) || S_ISSOCK(st->st_mode)) {
        color = 34;
    } else if (S_ISLNK(st->st_mode)) {
        color = 33;  /* yellow */
    }
    printf("\033[%dm", color);

    /* print name */
    printf("%s", name);

    /* print type */
    if (!dot && (ls_flags & LSF_TYPE)) {
        printtype(st->st_mode);
        if (st->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
            putchar('*');
        }
    }
    printf("\033[0m");

    if (ls_flags & LSF_LONG || ls_flags & LSF_SINGLE) {
        printf("\r\n");
    } else {
        len = strlen(name);
        cols += len;
        if (cols > termwidth / 2 + 8) {
            printf("\r\n");
            cols = 0;
        } else {
            if (len > 8) {
                putchar(' ');
                cols++;
            } else {
                for (; len <= 9; len++) {
                    putchar(' ');
                    cols++;
                }
            }
        }
    }
}

static int
do_ls(char *path)
{
    struct stat st;
    int nr_file = 0;
    char buf[PATH_MAX];
    DIR *dir;
    struct dirent *entry;

    if (stat(path, &st) == -1) {
        return -ENOTDIR;
    }

    if (S_ISDIR(st.st_mode)) {

        dir = vfs_opendir(path);
        if (dir == NULL) {
            return -ENOTDIR;
        }

        cols = 0;
        for (;;) {

            entry = vfs_readdir(dir);
            if (entry == NULL) {
                break;
            }

            buf[0] = 0;
            strlcpy(buf, path, sizeof(buf));
            buf[sizeof(buf) - 1] = '\0';

            if (!strcmp(entry->d_name, ".")) {
                ;   /* Do nothing */
            } else if (!strcmp(entry->d_name, "..")) {
                ;   /* Do nothing */
            } else {
                strlcat(buf, "/", sizeof(buf));
                strlcat(buf, entry->d_name, sizeof(buf));
            }

            if (stat(buf, &st) == -1 && errno != EACCES) {
                continue;
            }

            printentry(entry->d_name, &st);
            nr_file++;
        }

        vfs_closedir(dir);

        if (ls_flags & LSF_LONG) {
            printf("total %d\r\n", nr_file);
        } else {
            printf("\r\n");
        }
    } else {
        printentry(path, &st);
        printf("\r\n");
    }

    fflush(stdout);

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
