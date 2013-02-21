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
#include "kern/kern.h"
#include "kern/func_config.h"
#if CONFIG_VFS_EN > 0
#include "vfs/vfs.h"
#endif
#if CONFIG_NET_EN > 0
#include "lwip/tcpip.h"
#endif
#include <unistd.h>
#include <stdio.h>
/*********************************************************************************************************
** 配置
*********************************************************************************************************/
#define CONFIG_RAMDISK_EN   1
#define CONFIG_TMP_EN       1
#define CONFIG_MTD_EN       1
#define CONFIG_FB_EN        1
#define CONFIG_SHAREMEM_EN  1

#if CONFIG_RAMDISK_EN > 0
#include "drivers/ramdisk.h"
#endif
/*********************************************************************************************************
** Function name:           tcpip_init_done
** Descriptions:            lwIP 初始化完成处理函数
** input parameters:        arg                 参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
#if CONFIG_NET_EN > 0
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

    void ftpd_init(void);
    ftpd_init();

    void netio_init(void);
    netio_init();
}
#endif
/*********************************************************************************************************
** Function name:           init
** Descriptions:            初始化线程
** input parameters:        arg                 参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void init(void *arg)
{
#if CONFIG_VFS_EN > 0
    cpu_drivers_install();

    cpu_devices_create();

    {
        int fd;

        fclose(stdout);
        fd = open("/dev/serial0", O_WRONLY);
        stdout  = fdopen(fd,  "w");
    }

    board_drivers_install();

    board_devices_create();
#endif

#if CONFIG_VFS_EN > 0 && CONFIG_RAMDISK_EN > 0
    ramdisk_create("/dev/ramdisk", 1440 * KB);
#endif

#if CONFIG_VFS_EN > 0 && CONFIG_RAMDISK_EN > 0 && CONFIG_TMP_EN > 0
    vfs_mount("/tmp", "/dev/ramdisk", "fatfs", NULL);

    vfs_mkfs("/tmp", NULL);
#endif

#if CONFIG_NET_EN > 0
    tcpip_init(tcpip_init_done, NULL);
#endif

#if CONFIG_MODULE_EN > 0
    extern int module_init(void);
    module_init();                                                      /*  初始化内核模块子系统        */
#endif

    sleep(10);

    vfs_mount("/nfs", NULL, "nfs", "192.168.7.30:/nfs");

    while (1) {
        kheap_check();

        sleep(10);
    }
}
/*********************************************************************************************************
** Function name:           sys_drivers_install
** Descriptions:            安装系统驱动
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
#if CONFIG_VFS_EN > 0
static int sys_drivers_install(void)
{
    extern int null_init(void);
    null_init();

#if CONFIG_NET_EN > 0
    extern int socket_init(void);
    socket_init();
#endif

#if CONFIG_RAMDISK_EN > 0
    extern int ramdisk_init(void);
    ramdisk_init();
#endif

    extern int fifo_init(void);
    fifo_init();

    extern int zero_init(void);
    zero_init();

#if CONFIG_MTD_EN > 0
    extern int mtdblock_init(void);
    mtdblock_init();
#endif

#if CONFIG_FB_EN > 0
    extern int fb_init(void);
    fb_init();
#endif

#if CONFIG_SHAREMEM_EN > 0
    extern int sharemem_init(void);
    sharemem_init();
#endif

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
#endif
/*********************************************************************************************************
** Function name:           main
** Descriptions:            主函数
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int main(void)
{
    /*
     * 进入到这里 CPU 应该是关中断, 进入系统模式, MMU 被关闭,
     *
     * D-Cache 被关闭, I-Cache 可以打开
     *
     * 时钟可能未初始化好
     */
    extern void cpu_init(void);
    cpu_init();

    /*
     * 现在我认为时钟初始化好了, CPU 以全速跑
     */

    kernel_init();

#if CONFIG_VFS_EN > 0
    sys_drivers_install();

    sys_devices_create();
#endif

    kthread_create("init", init, NULL, 64 * KB, 50);

    kernel_start();

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
