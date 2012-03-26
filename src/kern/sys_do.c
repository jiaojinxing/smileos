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
** File name:               sys_do.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            系统调用处理
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/vmm.h"
#include <sys/types.h>
/*********************************************************************************************************
  系统调用处理
*********************************************************************************************************/
/*
 * 进程退出
 */
static void do_exit(int status)
{
    task_kill(current->tid);                                            /*  杀死当前任务                */
}

/*
 * 进程休眠
 */
static int do_sleep(unsigned int tick)
{
    current->timer = tick != 0 ? tick : 1;                              /*  休睡 TICK 数                */

    current->state = TASK_SLEEPING;                                     /*  当前任务进入休睡态          */

    schedule();                                                         /*  任务调度                    */

    return 0;
}

/*
 * 写
 */
static int do_write(int fd, const void *data, size_t len)
{
    printk((const char *)data);

    return (int)len;
}

#include <sys/time.h>
/*
 * 获得时间
 */
static int do_gettimeofday(struct timeval *tv, void *tzp)
{
    uint64_t tick;

    tick = get_tick();

    tv->tv_sec  = (tick / TICK_PER_SECOND);
    tv->tv_usec = (tick % TICK_PER_SECOND) * 1000000 / TICK_PER_SECOND;

    return 0;
}

/*
 * 获得 PID
 */
static int do_getpid(void)
{
    return (int)current->pid;
}

/*
 * 获得 errno 指针
 *
 * 进程最好记录该指针, 以免频繁切换用户态->内核态->用户态
 */
static int *do_errno(void)
{
    return &current->errno;
}

/*
 * 释放 CPU 使用权
 */
static void do_yeild(void)
{
    schedule();
}
/*********************************************************************************************************
  系统调用处理表
*********************************************************************************************************/
sys_do_t sys_do_table[] = {
        /*
         * do_xxx 的位置必须要和 SYS_CALL_XXX 的值一致
         */
        (sys_do_t)do_exit,
        (sys_do_t)do_sleep,
        (sys_do_t)do_write,
        (sys_do_t)do_gettimeofday,
        (sys_do_t)do_getpid,
        (sys_do_t)do_errno,
        (sys_do_t)do_yeild,
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
