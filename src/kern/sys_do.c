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
/*********************************************************************************************************
  ϵͳ���ô���
*********************************************************************************************************/
/*
 * �����˳�
 */
static void do_exit(int error_code)
{
    if (current->type == TASK_TYPE_PROCESS) {
        printk("process %d %s exit!\n", current->pid, current->name);
        extern void vmm_free_process_space(task_t *task);
        vmm_free_process_space(current);
    } else {
        printk("kthread %d %s exit!\n", current->tid, current->name);
    }

    current->state = TASK_UNALLOCATE;

    schedule();
}

/*
 * ��������
 */
static int do_sleep(uint32_t time)
{
    current->timer = time != 0 ? time : 1;

    current->state = TASK_SLEEPING;

    schedule();

    return 0;
}

/*
 * д
 */
static int do_write(int fd, void *data, uint32_t len)
{
    printk((char *)data);

    return (int)len;
}

#include <sys/time.h>
/*
 * ���ʱ��
 */
static int do_gettimeofday(struct timeval *tv, void *tzp)
{
    uint64_t tick = get_tick();

    tv->tv_sec  = (tick / TICK_PER_SECOND);
    tv->tv_usec = (tick % TICK_PER_SECOND) * 1000000 / TICK_PER_SECOND;

    return 0;
}

/*
 * ��� PID
 */
static int do_getpid(void)
{
    return (int)current->tid;
}

/*
 * ��� errno ָ��
 *
 * TODO: ��ð� errno �����û��ռ�, �����ύ�ں� errno �ĵ�ַ, �����û��ռ���� errno, ������ں�, ���������
 */
static int *do_errno(void)
{
    return &current->errno;
}

/*
 * �ͷ� CPU ʹ��Ȩ
 */
static void do_yeild(void)
{
    schedule();
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
        (sys_do_t)do_write,
        (sys_do_t)do_gettimeofday,
        (sys_do_t)do_getpid,
        (sys_do_t)do_errno,
        (sys_do_t)do_yeild,
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
