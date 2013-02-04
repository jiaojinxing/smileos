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
** File name:               context.c
** Last modified Date:      2013-2-3
** Last Version:            1.0.0
** Descriptions:            ��������Ļ���������
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-3
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
#include "arch/arm920t/arm.h"
/*********************************************************************************************************
** Function name:           arch_build_context
** Descriptions:            ��������Ļ���������
** input parameters:        task                ������ƿ�
**                          func                �������㺯��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int arch_build_context(task_t *task, void *func)
{
    if (task->type == TASK_TYPE_KTHREAD) {
        task->regs[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];     /*  SVC ģʽ�� SP (����ջ�ݼ�)  */
        task->regs[1]   = ARM_SYS_MODE | ARM_FIQ_EN | ARM_IRQ_EN;       /*  CPSR, SYS ģʽ              */
                                                                        /*  SYS ģʽ�� SP (����ջ�ݼ�)  */
        task->regs[2]   = (uint32_t)((uint8_t *)task->stack_base + task->stack_size);
        task->regs[3]   = ARM_SVC_MODE;                                 /*  SPSR, SVC ģʽ              */
        task->regs[4]   = (uint32_t)func;                               /*  LR                          */
        task->regs[5]   = (uint32_t)task;                               /*  R0 ~ R12                    */
        task->regs[6]   = 1;
        task->regs[7]   = 2;
        task->regs[8]   = 3;
        task->regs[9]   = 4;
        task->regs[10]  = 5;
        task->regs[11]  = 6;
        task->regs[12]  = 7;
        task->regs[13]  = 8;
        task->regs[14]  = 9;
        task->regs[15]  = 10;
        task->regs[16]  = 11;
        task->regs[17]  = 12;
        task->regs[18]  = (uint32_t)func;                               /*  PC                          */
    } else {

    }

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
