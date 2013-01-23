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
** File name:               s3c2440_int.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            S3C2440 �ж�
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
#include "kern/kern.h"
#include "s3c2440.h"

/*
 * �ж���Ŀ
 */
#define INTERRUPT_NR      INTGLOBAL

/*
 * �жϷ�������������
 */
static isr_t  isr_table[INTERRUPT_NR];
static void  *isr_arg_table[INTERRUPT_NR];
/*********************************************************************************************************
** Function name:           irq_c_handler
** Descriptions:            IRQ �жϴ������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void irq_c_handler(void)
{
    uint32_t interrupt;
    isr_t    isr;

    /*
     * ��չ�Ķ�:
     * http://hi.baidu.com/ch314156/blog/item/027cdd62feb97f3caa184c29.html
     */

    interrupt_enter();                                                  /*  �����ж�                    */

    interrupt = INTOFFSET;                                              /*  ����жϺ�                  */

    if (interrupt >= INTERRUPT_NR) {
        interrupt_exit();                                               /*  �˳��ж�                    */
        return;
    }

    isr = isr_table[interrupt];                                         /*  �����жϷ������            */
    if (isr != NULL) {
        isr(interrupt, isr_arg_table[interrupt]);
    }

    SRCPND  = 1 << interrupt;                                           /*  ����ж�Դ�ȴ�              */

    INTPND  = INTPND;                                                   /*  ����жϵȴ�                */

    interrupt_exit();                                                   /*  �˳��ж�                    */
}
/*********************************************************************************************************
** Function name:           isr_invaild
** Descriptions:            ��Ч�жϷ������
** input parameters:        interrupt           �жϺ�
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int isr_invaild(uint32_t interrupt, void *arg)
{
    printk("invaild interrupt %d!\n", interrupt);

    return -1;
}
/*********************************************************************************************************
** Function name:           interrupt_init
** Descriptions:            ��ʼ���ж�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_init(void)
{
    int i;

    SRCPND      = 0x00;                                                 /*  ��������ж�Դ�ȴ�          */

    SUBSRCPND   = 0x00;                                                 /*  ����������ж�Դ�ȴ�        */

    INTMOD      = 0x00;                                                 /*  ���������ж�ģʽΪ IRQ      */

    INTMSK      = BIT_ALLMSK;                                           /*  ���������ж�                */

    INTSUBMSK   = BIT_SUB_ALLMSK;                                       /*  �����������ж�              */

    INTPND      = INTPND;                                               /*  ��������жϵȴ�            */

    for (i = 0; i < INTERRUPT_NR; i++) {                                /*  ��ʼ���жϷ�������������*/
        isr_table[i]     = (isr_t)isr_invaild;
        isr_arg_table[i] = NULL;
    }
}
/*********************************************************************************************************
** Function name:           interrupt_mask
** Descriptions:            �����ж�
** input parameters:        interrupt           �жϺ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_mask(uint32_t interrupt)
{
    uint32_t reg;

    if (interrupt < INTERRUPT_NR) {
        reg = interrupt_disable();
        INTMSK |= 1 << interrupt;
        interrupt_resume(reg);
    }
}
/*********************************************************************************************************
** Function name:           interrupt_unmask
** Descriptions:            ȡ�������ж�
** input parameters:        interrupt           �жϺ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_unmask(uint32_t interrupt)
{
    uint32_t reg;

    if (interrupt < INTERRUPT_NR) {
        reg = interrupt_disable();
        INTMSK &= ~(1 << interrupt);
        interrupt_resume(reg);
    }
}
/*********************************************************************************************************
** Function name:           interrupt_install
** Descriptions:            ��װ�жϷ������
** input parameters:        interrupt           �жϺ�
**                          new_isr             �µ��жϷ������
**                          arg                 �жϷ���������
** output parameters:       old_isr             �ɵ��жϷ������
** Returned value:          NONE
*********************************************************************************************************/
int interrupt_install(uint32_t interrupt, isr_t new_isr, isr_t *old_isr, void *arg)
{
    uint32_t reg;

    if (interrupt < INTERRUPT_NR) {
        reg = interrupt_disable();

        if (old_isr != NULL) {
            *old_isr = isr_table[interrupt];
        }

        if (new_isr != NULL) {
            isr_table[interrupt]     = new_isr;
            isr_arg_table[interrupt] = arg;
        } else {
            isr_table[interrupt]     = (isr_t)isr_invaild;
        }
        interrupt_resume(reg);
        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
