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
** Modified by:             JiaoJinXing
** Modified date:           2012-3-25
** Version:                 1.1.0
** Descriptions:            ���������쳣, �쳣��Ϊɱ����ǰ���������ֹ�ں�
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/mmu.h"
#include "kern/vmm.h"

/*
 * FIQ �����жϴ������
 */
void fiq_c_handler(void)
{
    /*
     * FIQ �����жϴ�����򲢲����ɵ�ǰ����������쳣, ����ֻ������ SmileOS ��:-)
     */
    kcomplain("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    kcomplain("reboot SmileOS...\n");

    extern void _start(void);
    _start();
}

/*
 * δ����ָ���쳣�������
 */
void undf_c_handler(uint32_t lr, uint32_t spsr)
{
    interrupt_enter();                                                  /*  �����ж�                    */

    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    printk("lr   = 0x%x\n", lr);
    printk("spsr = 0x%x\n", spsr);

    task_kill(current->tid);                                            /*  ɱ����ǰ����                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}

/*
 * Ԥȡָ��ֹ�쳣�������
 */
void pabt_c_handler(uint32_t lr, uint32_t spsr)
{
    interrupt_enter();                                                  /*  �����ж�                    */

    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());
    printk("lr   = 0x%x\n", lr);
    printk("spsr = 0x%x\n", spsr);

    task_kill(current->tid);                                            /*  ɱ����ǰ����                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}

/*
 * ���ݷ�����ֹ�쳣�������
 */
void dabt_c_handler(uint32_t lr, uint32_t spsr)
{
    uint32_t mva;

    interrupt_enter();                                                  /*  �����ж�                    */

    switch (mmu_get_data_fault_status() & 0x0F) {
    case 1:     /* Alignment */
    case 3:
        printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        task_kill(current->tid);                                        /*  ɱ����ǰ����                */
        break;

    case 5:     /* Translation */
    case 7:
        /*
         * qemu-system-arm.exe �� bug..., ��ʵӲ��ʹ�� #if 1, qemu ʹ�� #if 0
         */
#if 1
        mva = mmu_get_fault_address();
#warning "you can't use qemu-system-arm.exe"
#else
        mva = mmu_get_fault_address() + PROCESS_SPACE_SIZE * current->pid;
#warning "you must use qemu-system-arm.exe"
#endif

        if (    mva >= PROCESS_SPACE_SIZE *  current->pid               /*  �жϳ����ַ�Ƿ��ڵ�ǰ����  */
             && mva <  PROCESS_SPACE_SIZE * (current->pid + 1)) {       /*  �������ַ�ռ䷶Χ��        */
            if (vmm_map_process_page(current, mva) == 0) {              /*  ҳ��ӳ��                    */
                current->dabt_cnt++;                                    /*  ������ֹ������һ            */
                interrupt_exit_no_schedule();                           /*  �˳��ж�, ����Ҫ����        */
                return;
            }
        }
        printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        task_kill(current->tid);                                        /*  ɱ����ǰ����                */
        break;

    case 9:     /* Domain */
    case 11:
        printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        task_kill(current->tid);                                        /*  ɱ����ǰ����                */
        break;

    case 13:    /* Permission */
    case 15:
        printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        task_kill(current->tid);                                        /*  ɱ����ǰ����                */
        break;

                /*
                 * External abort on noncachable
                 * nonbufferable access or noncachable
                 * bufferable read
                 */
    case 8:
    case 10:
        printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
        printk("fault address = 0x%x\n", mmu_get_fault_address());
        printk("fault status  = 0x%x\n", mmu_get_data_fault_status());
        printk("lr   = 0x%x\n", lr);
        printk("spsr = 0x%x\n", spsr);
        task_kill(current->tid);                                        /*  ɱ����ǰ����                */
        break;

    default:
        break;
    }

    interrupt_exit();                                                   /*  �˳��ж�                    */
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
