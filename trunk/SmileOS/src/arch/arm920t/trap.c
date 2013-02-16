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
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/kvars.h"
#include "arch/arm920t/mmu.h"
#include <signal.h>
/*********************************************************************************************************
** Function name:           print_regs
** Descriptions:            ��ӡ�Ĵ�����
** input parameters:        regs                �Ĵ�����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void print_regs(reg_t *regs)
{
    int i;

    printk("\n-------- REGS --------\n");

    printk("CPSR\t= 0x%x\n", regs[0]);
    printk("SP\t= 0x%x\n",   regs[1]);
    printk("LR\t= 0x%x\n",   regs[2]);
    printk("PC\t= 0x%x\n",   regs[16]);

    for (i = 0; i < 13; i++) {
        printk("R%d\t= 0x%x\n", i, regs[3 + i]);
    }

    printk("\n-------- END  --------\n");
}
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
     * FIQ �����жϲ������ɵ�ǰ����������쳣, һ������, ˵��������, ����ֻ������ SmileOS ��:-(
     */
    extern void _start(void);
    _start();
}
/*********************************************************************************************************
** Function name:           undf_c_handler
** Descriptions:            δ����ָ���쳣�������
** input parameters:        regs                �Ĵ�����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void undf_c_handler(reg_t *regs)
{
    interrupt_enter();                                                  /*  �����ж�                    */

    printk("%s, current TID = %d name=%s\n", __func__, current->tid, current->name);

    print_regs(regs);

    kill(current->tid, SIGILL);                                         /*  ɱ����ǰ����                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}
/*********************************************************************************************************
** Function name:           pabt_c_handler
** Descriptions:            Ԥȡָ��ֹ�쳣�������
** input parameters:        regs                �Ĵ�����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void pabt_c_handler(reg_t *regs)
{
    interrupt_enter();                                                  /*  �����ж�                    */

    printk("%s, current TID = %d name=%s\n", __func__, current->tid, current->name);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_prefetch_fault_status());

    print_regs(regs);

    kill(current->tid, SIGSEGV);                                        /*  ɱ����ǰ����                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}
/*********************************************************************************************************
** Function name:           dabt_oops
** Descriptions:            ���ݷ�����ֹ oops
** input parameters:        regs                �Ĵ�����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void dabt_oops(reg_t *regs)
{
    printk("%s, current TID = %d name=%s\n", __func__, current->tid, current->name);
    printk("fault address = 0x%x\n", mmu_get_fault_address());
    printk("fault status  = 0x%x\n", mmu_get_data_fault_status());

    print_regs(regs);
}
/*********************************************************************************************************
** Function name:           dabt_c_handler
** Descriptions:            ���ݷ�����ֹ�쳣�������
** input parameters:        regs                �Ĵ�����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void dabt_c_handler(reg_t *regs)
{
    interrupt_enter();                                                  /*  �����ж�                    */

    dabt_oops(regs);

    kill(current->tid, SIGSEGV);                                        /*  ɱ����ǰ����                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
