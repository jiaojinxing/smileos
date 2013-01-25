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
#include "vfs/vfs.h"
#include "lwip/tcpip.h"
/*********************************************************************************************************
** Function name:           tcpip_init_done
** Descriptions:            LwIP 初始化完成处理函数
** input parameters:        arg                 参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void tcpip_init_done(void *arg)
{
    static struct netif ethernetif;
    ip_addr_t           ip, submask, gateway;

#if 1
    IP4_ADDR(&ip,       192, 168,   7,  30);
    IP4_ADDR(&submask,  255, 255, 255,   0);
    IP4_ADDR(&gateway,  192, 168,   7,   1);
#else
    IP4_ADDR(&ip,       192, 168,   0,  30);
    IP4_ADDR(&submask,  255, 255, 255,   0);
    IP4_ADDR(&gateway,  192, 168,   0,   1);
#endif

    extern err_t ethernetif_init(struct netif *netif);
    netif_add(&ethernetif, &ip, &submask, &gateway, NULL, ethernetif_init, tcpip_input);

    netif_set_default(&ethernetif);

    netif_set_up(&ethernetif);
}
/*********************************************************************************************************
** Function name:           init
** Descriptions:            初始化线程
** input parameters:        arg                 参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void init(void *arg)
{
    extern int bsp_drivers_install(void);
    bsp_drivers_install();

    extern int bsp_devices_create(void);
    bsp_devices_create();

    tcpip_init(tcpip_init_done, NULL);

    vfs_mount("/tmp", "/dev/ramdisk", "fatfs", NULL);

    vfs_mkfs("/tmp", NULL);

    vfs_mount(NULL, NULL, "yaffs", NULL);

    extern void shell(void *arg);
    kthread_create("shell", shell, NULL, 16 * KB, 10);

    while (1) {
        sleep(1000);
    }
}
/*********************************************************************************************************
** Function name:           sys_drivers_install
** Descriptions:            安装系统驱动
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int sys_drivers_install(void)
{
    extern int null_init(void);
    null_init();

    extern int socket_init(void);
    socket_init();

    extern int ramdisk_init(void);
    ramdisk_init();

    return 0;
}
/*********************************************************************************************************
** Function name:           sys_devices_create
** Descriptions:            创建系统设备
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int sys_devices_create(void)
{
    return 0;
}
/*********************************************************************************************************
** Function name:           main
** Descriptions:            主函数
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int main(void)
{
    kernel_init();

    vfs_init();

    sys_drivers_install();

    sys_devices_create();

    kernel_start();

    /*
     * 现在已经进入了 idle 进程
     */

    kthread_create("init", init, NULL, 4 * KB, 100);

    while (1) {
    }

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
