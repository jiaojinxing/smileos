/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <jiaojinxing1987@gmail.com>
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
#include "kern/heap.h"
#include "vfs/vfs.h"
#include "lwip/tcpip.h"

#include <unistd.h>
#include <fcntl.h>

/*
 * LwIP 初始化完成处理函数
 */
static void tcpip_init_done(void *arg)
{
    static struct netif ethernetif;
    ip_addr_t           ip, submask, gateway;

    /*
     * 如果使用 qemu, 请把 #if 1 改为 #if 0
     */
#if 0
    IP4_ADDR(&ip,       192, 168,   27, 201);
    IP4_ADDR(&submask,  255, 255, 255,    0);
    IP4_ADDR(&gateway,  192, 168,   27,   1);
#else
    IP4_ADDR(&ip,       192, 168,   0,  30);
    IP4_ADDR(&submask,  255, 255, 255,   0);
    IP4_ADDR(&gateway,  192, 168,   0,   1);
#endif

    extern err_t ethernetif_init(struct netif *netif);
    netif_add(&ethernetif, &ip, &submask, &gateway, NULL, ethernetif_init, tcpip_input);

    netif_set_default(&ethernetif);

    netif_set_up(&ethernetif);

    extern void telnetd(void *arg);
    kthread_create("telnetd", telnetd, NULL, 4 * KB, 10);

    extern void ftpd(void *arg);
    kthread_create("ftpd", ftpd, NULL, 12 * KB, 10);

    extern void netio_init(void);
    netio_init();
}

#include <stdlib.h>
#include <stdio.h>
#include <linux/input.h>

/*
 * 初始化线程
 */
static void init(void *arg)
{
    struct input_event event;
    int fd;
    int len;

    extern int bsp_drivers_install(void);
    bsp_drivers_install();

    extern int bsp_devices_create(void);
    bsp_devices_create();

    tcpip_init(tcpip_init_done, NULL);


    extern int symbol_init(void);
    symbol_init();                                                      /*  增加系统的符号              */

    fd = open("/dev/event0", O_RDONLY, 0666);

    while (1) {
        len = read(fd, &event, sizeof(event));
        if (len == sizeof(event)) {
            printf("/dev/event0: x=%d, y=%d, %s\n", event.x, event.y, event.press ? "press" : "release");
        }
    }

    close(fd);
}

/*
 * 安装系统驱动
 */
static int sys_drivers_install(void)
{
    extern int null_init(void);
    null_init();

    extern int socket_init(void);
    socket_init();

    extern int ttyS0_init(void);
    ttyS0_init();

    extern int pty_init(void);
    pty_init();

    return 0;
}

/*
 * 主函数
 */
int main(void)
{
    kernel_init();

    vfs_init();

    sys_drivers_install();

    kernel_start();

    /*
     * 现在已经进入了 idle 进程
     */

    kthread_create("init", init, NULL, 4 * KB, 10);

    while (1) {
    }

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
