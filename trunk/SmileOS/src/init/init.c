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
** Descriptions:            ��ʼ��
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
#include "kern/kern.h"
#include "vfs/vfs.h"
#include "lwip/tcpip.h"
/*********************************************************************************************************
** Function name:           tcpip_init_done
** Descriptions:            lwIP ��ʼ����ɴ�����
** input parameters:        arg                 ����
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
** Descriptions:            ��ʼ���߳�
** input parameters:        arg                 ����
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

    vfs_mount("/kern",   "/dev/mtdblock0", "yaffs1", NULL);

    vfs_mount("/rootfs", "/dev/mtdblock1", "yaffs1", NULL);

    while (1) {
        sleep(10);
    }
}
/*********************************************************************************************************
** Function name:           sys_drivers_install
** Descriptions:            ��װϵͳ����
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

    extern int fifo_init(void);
    fifo_init();

    extern int zero_init(void);
    zero_init();

    return 0;
}
/*********************************************************************************************************
** Function name:           sys_devices_create
** Descriptions:            ����ϵͳ�豸
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
** Descriptions:            ������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int main(void)
{
    kernel_init();

    vfs_init();

    extern int module_init(void);
    module_init();

    sys_drivers_install();

    sys_devices_create();

    kernel_start();

    /*
     * �����Ѿ������� idle ����
     */

    kthread_create("init", init, NULL, 4 * KB, 100);

    while (1) {
    }

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
