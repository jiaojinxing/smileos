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
#include "config.h"
#include "types.h"
#include "kern.h"
#include "mmu.h"
#include "sys_call.h"
#include "sbin.h"
#include <string.h>

static uint32_t test_thread1_stack[1024];
static uint32_t test_thread2_stack[1024];

static uint8_t heap[2 * MB];

void test_thread1(void *arg)
{
    int i = 0;
    void *ptr;

    while (1) {
        printf("thread 1, i = %d\n", i++);

        ptr = malloc(i);

        printf("thread 1 addr = %p\n", ptr);

        sleep(TICK_PER_SECOND / 2);

        free(ptr);

        sleep(TICK_PER_SECOND / 2);
    }
}

void test_thread2(void *arg)
{
    int i = 0;

    while (1) {
        printf("thread 2, i = %d\n", i++);

        printf("thread 2 addr = %p\n", malloc(1024));

        sleep(TICK_PER_SECOND / 4);
    }
}

int main(void)
{
    uint8_t  *code;
    uint32_t  size;

    mmu_init();

    memcpy((void *)VECTOR_P_ADDR, (void *)KERN_LOAD_ADDR, PAGE);

    sched_init();

    create_thread((uint32_t)test_thread1, (uint32_t)&test_thread1_stack[1024], 15);

    create_thread((uint32_t)test_thread2, (uint32_t)&test_thread2_stack[1024], 15);

    code = sbin_lookup("/2440_P1.hex", &size);
    create_process(code, size, 15);

    __switch_to_process0(current->content[0]);

    heap_init(heap, 2 * MB);

    while (1) {
        //mmu_wait_for_interrupt();
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
