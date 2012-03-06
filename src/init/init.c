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

#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"

static void tcpip_init_done(void *arg)
{
    printf("tcpip_init_done\n");
}

static void thread1(void *arg)
{
    static struct netif ethernetif;
    ip_addr_t           ip, submask, gateway;

    tcpip_init(tcpip_init_done, NULL);

    IP4_ADDR(&ip,       192, 168,   2,  60);
    IP4_ADDR(&submask,  255, 255, 255,   0);
    IP4_ADDR(&gateway,  192, 168,   2,   1);

    extern err_t ethernetif_init(struct netif *netif);
    netif_add(&ethernetif, &ip, &submask, &gateway, NULL, ethernetif_init, tcpip_input);

    netif_set_default(&ethernetif);

    netif_set_up(&ethernetif);

    while (1) {
        printf("hello SmileOS, kernel thread %d\n", (int)arg);
        sleep(1);
    }
}

int main(void)
{
    uint8_t  *code;
    uint32_t  size;
    int i;

    mmu_init();

    kernel_init();

    code = sbin_lookup("/2440_P1.hex", &size);

    for (i = 0; i < 1; i++) {
        process_create(code, size, 15);
    }

    kthread_create(thread1, (void *)1, 32 * 1024, 15);

    kernel_start();

    while (1) {
        printk("main\n");
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
