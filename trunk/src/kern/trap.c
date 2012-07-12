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
#include "kern/arm.h"

extern void get_sys_lr(uint32_t *lr);

/*
 * FIQ �����жϴ������
 */
void fiq_c_handler(void)
{
    /*
     * FIQ �����жϲ������ɵ�ǰ����������쳣, ����ֻ������ SmileOS ��:-(
     */
    kcomplain("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    kcomplain("reboot SmileOS...\n");

    extern void _start(void);
    _start();
}

/*
 * δ����ָ���쳣�������
 */
void undf_c_handler(uint32_t pc, uint32_t cpsr)
{
    uint32_t lr;

    interrupt_enter();                                                  /*  �����ж�                    */

    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    printk("pc   = 0x%x\n", pc);
    printk("cpsr = 0x%x\n", cpsr);
    get_sys_lr(&lr);
    printk("lr = 0x%x\n", lr);
    if ((cpsr & ARM_MODE_MASK) == ARM_SVC_MODE && current->pid == 0) {  /*  ������� SVC ģʽ�ҵ�       */
        extern const char *last_syscall;
        printk("last system call = %s\n", last_syscall);
    }

    task_kill(current->tid, SIGILL);                                    /*  ɱ����ǰ����                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}

/*
 * Ԥȡָ��ֹ�쳣�������
 */
void pabt_c_handler(uint32_t pc, uint32_t cpsr)
{
    uint32_t lr;

    interrupt_enter();                                                  /*  �����ж�                    */

    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());
    printk("pc   = 0x%x\n", pc);
    printk("cpsr = 0x%x\n", cpsr);
    get_sys_lr(&lr);
    printk("lr = 0x%x\n", lr);
    if ((cpsr & ARM_MODE_MASK) == ARM_SVC_MODE && current->pid == 0) {  /*  ������� SVC ģʽ�ҵ�       */
        extern const char *last_syscall;
        printk("last system call = %s\n", last_syscall);
    }

    task_kill(current->tid, SIGSEGV);                                   /*  ɱ����ǰ����                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}

/*
 * ���ݷ�����ֹ oops
 */
#define dabt_oops()                                                                     \
    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);    \
    printk("fault address = 0x%x\n", mmu_get_fault_address());                          \
    printk("fault status  = 0x%x\n", mmu_get_data_fault_status());                      \
    printk("pc   = 0x%x\n", pc);                                                        \
    printk("cpsr = 0x%x\n", cpsr);                                                      \
    get_sys_lr(&lr);                                                                    \
    printk("lr   = 0x%x\n", lr);                                                        \
    if ((cpsr & ARM_MODE_MASK) == ARM_SVC_MODE && current->pid == 0) {      /*  ������� SVC ģʽ�ҵ�   */\
        extern const char *last_syscall;                                                \
        printk("last system call = %s\n", last_syscall);                                \
    }                                                                                   \
    task_kill(current->tid, SIGSEGV)                                        /*  ɱ����ǰ����            */

/*
 * ���ݷ�����ֹ�쳣�������
 */
void dabt_c_handler(uint32_t pc, uint32_t cpsr)
{
    uint32_t mva;
    uint32_t lr;

    interrupt_enter();                                                  /*  �����ж�                    */

    switch (mmu_get_data_fault_status() & 0x0F) {
    case 1:     /* Alignment */
    case 3:
        dabt_oops();
        break;

    case 5:     /* Translation */
    case 7:
        if (current->pid != 0) {                                        /*  ������ڽ����﷢������      */

            mva = mmu_get_fault_address();
            /*
             * ��ʵӲ��ʱ, mmu_get_fault_address Ӧ�÷��ص���
             *
             * mva = va < 32 * MB ? va + PROCESS_SPACE_SIZE * current->pid : va
             *
             * �� qemu-system-arm.exe �� bug, ʼ�շ��ص��� va
             *
             * ���Լ����������������
             */
            if (mva < PROCESS_SPACE_SIZE) {
                mva = mva + PROCESS_SPACE_SIZE * current->pid;
            }

            if (    mva >= PROCESS_SPACE_SIZE *  current->pid           /*  �жϳ����ַ�Ƿ��ڵ�ǰ����  */
                 && mva <  PROCESS_SPACE_SIZE * (current->pid + 1)) {   /*  �������ַ�ռ䷶Χ��        */
                if (vmm_page_map(current, mva) == 0) {                  /*  ҳ��ӳ��                    */
                    current->dabt_cnt++;                                /*  ���ݷ�����ֹ������һ        */
                    interrupt_exit_no_sched();                          /*  �˳��ж�, ����Ҫ����        */
                    return;
                }
            }
        } else {
            int32_t pid;

            /*
             * ��������ں��﷢������
             *
             * ע����ϵͳ���ô�����, ���뽫ָ���Ͳ����� va_to_mva ת�����ɴ����ں˽ӿ�
             */

            mva = mmu_get_fault_address();                              /*  ������� MVA                */

            pid = mva / PROCESS_SPACE_SIZE;
            if (pid != 0 && pid < PROCESS_NR) {
                task_t *task = &tasks[pid];
                if (task->state != TASK_UNALLOCATE) {
                    if (vmm_page_map(task, mva) == 0) {                 /*  ҳ��ӳ��                    */
                        task->dabt_cnt++;                               /*  ���ݷ�����ֹ������һ        */
                        interrupt_exit_no_sched();                      /*  �˳��ж�, ����Ҫ����        */
                        return;
                    }
                }
            }
        }
        dabt_oops();
        break;

    case 9:     /* Domain */
    case 11:
        dabt_oops();
        break;

    case 13:    /* Permission */
    case 15:
        dabt_oops();
        break;

                /*
                 * External abort on noncachable
                 * nonbufferable access or noncachable
                 * bufferable read
                 */
    case 8:
    case 10:
        dabt_oops();
        break;

    default:
        break;
    }

    interrupt_exit();                                                   /*  �˳��ж�                    */
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
