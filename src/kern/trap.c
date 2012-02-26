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
** File name:               trap.c
** Last modified Date:      2012-2-25
** Last Version:            1.0.0
** Descriptions:            �쳣�������
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-25
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
#include "config.h"
#include "types.h"
#include "kern.h"
#include "mmu.h"

/*
 * δ����ָ��
 */
void undf_c_handler(uint32_t lr, uint32_t spsr)
{
    printk("%s\n", __func__);

    while (1);
}

/*
 * Ԥȡָ��ֹ
 */
void pabt_c_handler(uint32_t lr, uint32_t spsr)
{
    printk("%s, current pid = %d\n", __func__, current->pid);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());
    printk("lr   = 0x%x\n", lr);
    printk("spsr = 0x%x\n", spsr);

    while (1);
}

/*
 * ������ֹ
 */
void dabt_c_handler(uint32_t lr, uint32_t spsr)
{
    printk("%s, current pid = %d\n", __func__, current->pid);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
    printk("lr   = 0x%x\n", lr);
    printk("spsr = 0x%x\n", spsr);

    while (1);
}

/*
 * �����ж�
 */
void fiq_c_handler(void)
{
    printk("%s\n", __func__);

    while (1);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
