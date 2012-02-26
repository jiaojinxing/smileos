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
#include <pthread.h>

#define HEAP_SIZE       (1 * MB)

static uint8_t heap[HEAP_SIZE];

static void *test_thread(void *arg)
{
    int i = 0;
    void *ptr;

    while (1) {
        printf("thread %d, i = %d\n", (int)arg, i++);

        ptr = malloc(i);

        printf("thread %d addr = %p\n", (int)arg, ptr);

        free(ptr);

        __pthread_usleep(10000);
    }

    return NULL;
}

int main(void)
{
    uint8_t  *code;
    uint32_t  size;
    pthread_t tid1;
    pthread_t tid2;

    mmu_init();

    memcpy((void *)VECTOR_P_ADDR, (void *)KERN_LOAD_ADDR, PAGE);

    sched_init();

    code = sbin_lookup("/2440_P1.hex", &size);
    process_create(code, size, 15);
    process_create(code, size, 15);
    process_create(code, size, 15);
    process_create(code, size, 15);
    process_create(code, size, 15);
    process_create(code, size, 15);

    sched_start();

    printk("process 0 inter\n");

    heap_init(heap, sizeof(heap));

    pthread_create(&tid1, NULL, test_thread, (void *)1);
    pthread_create(&tid2, NULL, test_thread, (void *)2);
    pthread_create(&tid2, NULL, test_thread, (void *)3);
    pthread_create(&tid2, NULL, test_thread, (void *)4);
    pthread_create(&tid2, NULL, test_thread, (void *)5);

    while (1) {
        pthread_yield_np();
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
