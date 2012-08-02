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
** File name:               main.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            主函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
 * 网络服务器线程
 */
static void *webs_thread(void *arg)
{
    extern int webServerStart(char *pcAddr, char *pcWebPath, int iPort);

    webServerStart("192.168.0.30", "/sd0/webs", 80);

    return NULL;
}

/*
 * 视频播放器线程
 */
static void *player_thread(void *arg)
{
    while (1) {
        extern int ffplay(const char *filename);

        ffplay(arg);
    }

    return NULL;
}

/*
 * main 函数
 */
int main(int argc, char *argv[])
{
    pthread_t webs_tid;
    pthread_t player_tid;
    pid_t pid;

    if (argc < 2) {
        printf("Usage: %s file.avi\n", argv[0]);
        return -1;
    }

    pid = fork();
    if (pid == 0) {
        printf("in child process\n");

        pthread_create(&player_tid, NULL, player_thread, argv[1]);
    } else {
        printf("in parent process, child pid=%d\n", pid);

        /*
         * TODO: 为什么现在不行了呢?
         */
        //pthread_create(&webs_tid,   NULL, webs_thread,   NULL);

        pthread_create(&player_tid, NULL, player_thread, argv[1]);
    }

    while (1) {
        pthread_yield_np();
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
