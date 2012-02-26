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
** File name:               s3c2440.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            s3c2440 ��ʼ��
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
#include "s3c2440_clock.h"
#include "s3c2440_uart.h"
#include "s3c2440_int.h"
#include "s3c2440_timer.h"

/*
 * ��ʼ�� CPU
 */
void cpu_init(void)
{
    WTCON = 0;                                                          /*  �رտ��Ź�                  */

    INTMSK = 0xffffffff;                                                /*  ���������ж�                */

    INTSUBMSK = 0x7fff;                                                 /*  �����������ж�              */
}

/*
 * BSP �ڴ�ӳ��
 */
void bsp_mem_map(void)
{
    /*
     * sfr
     */
    mmu_map_section(MMU_TBL_BASE,
            0x48000000,
            0x48000000,
           (0x60000000 - 0x48000000) / MB,
            SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));

    /*
     * dm9000
     */
}

/*
 * ��ʼ�� BSP
 */
void bsp_init(void)
{
    clock_init();

    uart_init();

    interrupt_init();

    timer_init();
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
