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
** Descriptions:            ϵͳ���ô���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#include "vfs/vfs.h"
#include <sys/types.h>
#include <sys/time.h>
#include <reent.h>
/*********************************************************************************************************
  ϵͳ���ô���
*********************************************************************************************************/
/*
 * �����˳�
 */
static void do_exit(int status)
{
    task_kill(current->tid);                                            /*  ɱ����ǰ����                */
}

/*
 * ��������
 */
static int do_sleep(unsigned int ticks)
{
    current->timer = ticks != 0 ? ticks : 1;                            /*  ��˯ TICK ��                */

    current->state = TASK_SLEEPING;                                     /*  ��ǰ���������˯̬          */

    schedule();                                                         /*  �������                    */

    return 0;
}

/*
 * ���ʱ��
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
 * ��� PID
 */
static int do_getpid(void)
{
    return (int)current->pid;
}

/*
 * �ͷ� CPU ʹ��Ȩ
 */
static void do_yeild(void)
{
    schedule();
}

/*
 * ��� reent �ṹָ��
 */
static struct _reent *do_get_reent(void)
{
    return _impure_ptr;
}
/*********************************************************************************************************
  ϵͳ���ô����
*********************************************************************************************************/
sys_do_t sys_do_table[] = {
        /*
         * do_xxx ��λ�ñ���Ҫ�� SYS_CALL_XXX ��ֵһ��
         */
        (sys_do_t)do_exit,
        (sys_do_t)do_sleep,
        (sys_do_t)do_yeild,
        (sys_do_t)do_gettimeofday,
        (sys_do_t)do_getpid,
        (sys_do_t)vfs_write,
        (sys_do_t)vfs_mkdir,
        (sys_do_t)vfs_open,
        (sys_do_t)vfs_read,
        (sys_do_t)vfs_rename,
        (sys_do_t)vfs_fstat,
        (sys_do_t)vfs_unlink,
        (sys_do_t)vfs_close,
        (sys_do_t)vfs_fcntl,
        (sys_do_t)vfs_isatty,
        (sys_do_t)vfs_link,
        (sys_do_t)vfs_lseek,
        (sys_do_t)vfs_stat,
        (sys_do_t)do_get_reent,
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
