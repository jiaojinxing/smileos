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
** File name:               init.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            初始化
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
#include "kern/mmu.h"
#include "kern/sys_call.h"
#include "kern/sbin.h"
#include <string.h>

void thread1(void *arg)
{
    while (1) {
        //printf("hello smileos, %d\n", (int)arg);
        sleep(1);
    }
}

int main(void)
{
    uint8_t  *code;
    uint32_t  size;

    mmu_init();

    memcpy((void *)VECTOR_P_ADDR, (void *)KERN_LOAD_ADDR, PAGE);

    sched_init();

    code = sbin_lookup("/2440_P1.hex", &size);

    //process_create(code, size, 15);
    //process_create(code, size, 15);

    kthread_create(thread1, (void *)1, 32 * 1024, 5);
    kthread_create(thread1, (void *)2, 32 * 1024, 5);
    kthread_create(thread1, (void *)3, 32 * 1024, 5);
    kthread_create(thread1, (void *)4, 32 * 1024, 5);
    kthread_create(thread1, (void *)5, 32 * 1024, 5);

    sched_start();

    while (1) {

    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
