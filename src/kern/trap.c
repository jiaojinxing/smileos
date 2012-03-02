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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/mmu.h"

/*
 * δ����ָ���쳣�������
 */
void undf_c_handler(uint32_t lr, uint32_t spsr)
{
    printk("%s, current tid = %d\n", __func__, current->tid);
    printk("lr   = 0x%x\n", lr);
    printk("spsr = 0x%x\n", spsr);

    while (1);
}

/*
 * Ԥȡָ��ֹ�쳣�������
 */
void pabt_c_handler(uint32_t lr, uint32_t spsr)
{
    printk("%s, current tid = %d\n", __func__, current->tid);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());
    printk("lr   = 0x%x\n", lr);
    printk("spsr = 0x%x\n", spsr);

    while (1);
}

/*
 * FIQ �����жϴ������
 */
void fiq_c_handler(uint32_t lr, uint32_t spsr)
{
    printk("%s, current tid = %d\n", __func__, current->tid);
    printk("lr   = 0x%x\n", lr);
    printk("spsr = 0x%x\n", spsr);

    while (1);
}

/*
 * ���ݷ�����ֹ�쳣�������
 */
void dabt_c_handler(uint32_t lr, uint32_t spsr)
{
    switch (mmu_get_data_fault_status() & 0x0F) {
    case 1: /* Alignment */
    case 3:
        printk("%s, current tid = %d\n", __func__, current->tid);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        while (1);
        break;

    case 5: /* Translation */
    case 7:
        if (translation_failed(mmu_get_fault_address()) < 0) {
            printk("%s, current tid = %d\n", __func__, current->tid);
            printk("fault address = 0x%x\n", mmu_get_fault_address());
            printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
            printk("lr   = 0x%x\n", lr);
            printk("spsr = 0x%x\n", spsr);
            while (1);
        }
        break;

    case 9:
    case 11:
        printk("%s, current tid = %d\n", __func__, current->tid);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        while (1);
        break;

    case 13:
    case 15:
        printk("%s, current tid = %d\n", __func__, current->tid);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        while (1);
        break;

    case 8:
    case 10:
        printk("%s, current tid = %d\n", __func__, current->tid);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        while (1);
        break;

    default:
        return;
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
