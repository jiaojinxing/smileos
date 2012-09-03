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
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-8-29
** Version:                 1.3.0
** Descriptions:            ����ע��
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/mmu.h"
#include "kern/vmm.h"
#include "kern/arm.h"

extern void get_sys_lr(uint32_t *LR);
/*********************************************************************************************************
** Function name:           fiq_c_handler
** Descriptions:            FIQ �����жϴ������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
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
/*********************************************************************************************************
** Function name:           undf_c_handler
** Descriptions:            δ����ָ���쳣�������
** input parameters:        PC                  �Ĵ��� PC
**                          CPSR                �Ĵ��� CPSR
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void undf_c_handler(uint32_t PC, uint32_t CPSR)
{
    uint32_t LR;

    interrupt_enter();                                                  /*  �����ж�                    */

    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    printk("PC   = 0x%x\n", PC);
    printk("CPSR = 0x%x\n", CPSR);
    get_sys_lr(&LR);
    printk("LR = 0x%x\n", LR);
    if ((CPSR & ARM_MODE_MASK) == ARM_SVC_MODE && current->pid == 0) {  /*  ������� SVC ģʽ�ҵ�       */
        extern const char *last_syscall;
        printk("last system call = %s\n", last_syscall);
    }

    task_kill(current->tid, SIGILL);                                    /*  ɱ����ǰ����                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}
/*********************************************************************************************************
** Function name:           pabt_c_handler
** Descriptions:            Ԥȡָ��ֹ�쳣�������
** input parameters:        PC                  �Ĵ��� PC
**                          CPSR                �Ĵ��� CPSR
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void pabt_c_handler(uint32_t PC, uint32_t CPSR)
{
    uint32_t LR;

    interrupt_enter();                                                  /*  �����ж�                    */

    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());
    printk("PC   = 0x%x\n", PC);
    printk("CPSR = 0x%x\n", CPSR);
    get_sys_lr(&LR);
    printk("LR = 0x%x\n", LR);
    if ((CPSR & ARM_MODE_MASK) == ARM_SVC_MODE && current->pid == 0) {  /*  ������� SVC ģʽ�ҵ�       */
        extern const char *last_syscall;
        printk("last system call = %s\n", last_syscall);
    }

    task_kill(current->tid, SIGSEGV);                                   /*  ɱ����ǰ����                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}
/*********************************************************************************************************
** Function name:           dabt_oops
** Descriptions:            ���ݷ�����ֹ oops
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
#define dabt_oops()                                                                     \
    printk("%s, current tid = %d name=%s\n", __func__, current->tid, current->name);    \
    printk("fault address = 0x%x\n", mmu_get_fault_address());                          \
    printk("fault status  = 0x%x\n", mmu_get_data_fault_status());                      \
    printk("PC   = 0x%x\n", PC);                                                        \
    printk("CPSR = 0x%x\n", CPSR);                                                      \
    get_sys_lr(&LR);                                                                    \
    printk("LR   = 0x%x\n", LR);                                                        \
    if ((CPSR & ARM_MODE_MASK) == ARM_SVC_MODE && current->pid == 0) {      /*  ������� SVC ģʽ�ҵ�   */\
        extern const char *last_syscall;                                                \
        printk("last system call = %s\n", last_syscall);                                \
    }                                                                                   \
    task_kill(current->tid, SIGSEGV)                                        /*  ɱ����ǰ����            */
/*********************************************************************************************************
** Function name:           dabt_c_handler
** Descriptions:            ���ݷ�����ֹ�쳣�������
** input parameters:        PC                  �Ĵ��� PC
**                          CPSR                �Ĵ��� CPSR
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void dabt_c_handler(uint32_t PC, uint32_t CPSR)
{
    uint32_t MVA;
    uint32_t LR;

    interrupt_enter();                                                  /*  �����ж�                    */

    switch (mmu_get_data_fault_status() & 0x0F) {
    case 1:     /* Alignment */
    case 3:
        dabt_oops();
        break;

    case 5:     /* Translation */
    case 7:
        if (current->pid != 0) {                                        /*  ������ڽ����﷢������      */

            MVA = mmu_get_fault_address();
            /*
             * ��ʵӲ��ʱ, mmu_get_fault_address Ӧ�÷��ص���
             *
             * MVA = VA < 32 * MB ? VA + PROCESS_SPACE_SIZE * current->pid : VA
             *
             * �� qemu-system-arm.exe �� bug, ʼ�շ��ص��� VA
             *
             * ���Լ����������������
             */
            if (MVA < PROCESS_SPACE_SIZE) {
                MVA = MVA + PROCESS_SPACE_SIZE * current->pid;
            }

            if (    MVA >= PROCESS_SPACE_SIZE *  current->pid           /*  �жϳ����ַ�Ƿ��ڵ�ǰ����  */
                 && MVA <  PROCESS_SPACE_SIZE * (current->pid + 1)) {   /*  �������ַ�ռ䷶Χ��        */
                if (vmm_page_map(current, MVA) == 0) {                  /*  ҳ��ӳ��                    */
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

            MVA = mmu_get_fault_address();                              /*  ������� MVA                */

            pid = MVA / PROCESS_SPACE_SIZE;
            if (pid != 0 && pid < PROCESS_NR) {
                task_t *task = &tasks[pid];
                if (task->state != TASK_UNALLOCATE) {
                    if (vmm_page_map(task, MVA) == 0) {                 /*  ҳ��ӳ��                    */
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
** END FILE
*********************************************************************************************************/
