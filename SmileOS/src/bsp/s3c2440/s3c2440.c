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
** File name:               s3c2440.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            S3C2440 ��ʼ��
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
#include "kern/mmu.h"
#include "s3c2440.h"
/*********************************************************************************************************
** Function name:           cpu_reset_init
** Descriptions:            ��λ���ʼ�� CPU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void cpu_reset_init(void)
{
    WTCON       = 0x00;                                                 /*  �رտ��Ź�                  */

    INTMSK      = BIT_ALLMSK;                                           /*  ���������ж�                */

    INTSUBMSK   = BIT_SUB_ALLMSK;                                       /*  �����������ж�              */

    mmu_disable();

    mmu_disable_dcache();

    mmu_disable_icache();
}
/*********************************************************************************************************
** Function name:           cpu_mem_map
** Descriptions:            CPU �ڴ�ӳ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void cpu_mem_map(void)
{
    /*
     * ���⹦�ܼĴ���
     */
    mmu_map_region(0x48000000,
                   0x48000000,
                   0x60000000 - 0x48000000,
                   SECTION_ATTR(AP_USER_NO, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));
}
/*********************************************************************************************************
** CPU �����ռ�
*********************************************************************************************************/
const space_t cpu_resv_space[] = {
        {
            0x48000000,                                                 /*  ���⹦�ܼĴ���              */
            0x60000000 - 0x48000000
        },
        {
            0,
            0
        }
};
/*********************************************************************************************************
** Function name:           cpu_kernel_init
** Descriptions:            �ں˳�ʼ�� CPU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void cpu_kernel_init(void)
{
    extern void clock_init(void);
    clock_init();

    extern void interrupt_init(void);
    interrupt_init();

    extern void timer_init(void);
    timer_init();
}
/*********************************************************************************************************
** Function name:           soc_drivers_install
** Descriptions:            ��װ SOC ����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int soc_drivers_install(void)
{
    extern int serial0_init(void);
    serial0_init();

    extern void nand_init(void);
    nand_init();

    return 0;
}
/*********************************************************************************************************
** Function name:           soc_devices_create
** Descriptions:            ���� SOC �豸
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int soc_devices_create(void)
{
    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
