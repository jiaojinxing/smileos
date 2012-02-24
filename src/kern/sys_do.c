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
#include "config.h"
#include "types.h"
#include "kern.h"
#include "mem.h"
/*********************************************************************************************************
  系统调用处理
*********************************************************************************************************/
/*
 * 进程退出
 */
static int do_exit(int error_code)
{
    current->state = TASK_UNALLOCATE;

    schedule();

    return 0;
}

/*
 * 进程休眠
 */
static int do_sleep(uint32_t time)
{
    current->timer = time;

    current->state = TASK_SLEEPING;

    schedule();

    return 0;
}

/*
 * 进程写
 */
static int do_write(char *str)
{
    printk(str);

    return 0;
}

static void *do_malloc(uint32_t size)
{
    return mem_heap_alloc(&task[current->pid].heap, size);
}

static void *do_free(void *ptr)
{
    return mem_heap_free(&task[current->pid].heap, ptr);
}

static int do_heap_init(uint8_t *base, uint32_t size)
{
    return mem_heap_init(&task[current->pid].heap, base, size);
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
        (sys_do_t)do_malloc,
        (sys_do_t)do_free,
        (sys_do_t)do_heap_init,
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
