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
** Descriptions:            S3C2440 中断
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
** Version:                 1.0.0
** Descriptions:            创建文件
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
 * 中断数目
 */
#define INTERRUPT_NR      INTGLOBAL

/*
 * 中断服务程序及其参数表
 */
static isr_t  isr_table[INTERRUPT_NR];
static void  *isr_arg_table[INTERRUPT_NR];
/*********************************************************************************************************
** Function name:           irq_c_handler
** Descriptions:            IRQ 中断处理程序
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void irq_c_handler(void)
{
    uint32_t interrupt;
    isr_t    isr;

    /*
     * 扩展阅读:
     * http://hi.baidu.com/ch314156/blog/item/027cdd62feb97f3caa184c29.html
     */

    interrupt_enter();                                                  /*  进入中断                    */

    interrupt = INTOFFSET;                                              /*  获得中断号                  */

    if (interrupt >= INTERRUPT_NR) {
        interrupt_exit();                                               /*  退出中断                    */
        return;
    }

    isr = isr_table[interrupt];                                         /*  调用中断服务程序            */
    if (isr != NULL) {
        isr(interrupt, isr_arg_table[interrupt]);
    }

    SRCPND  = 1 << interrupt;                                           /*  清除中断源等待              */

    INTPND  = INTPND;                                                   /*  清除中断等待                */

    interrupt_exit();                                                   /*  退出中断                    */
}
/*********************************************************************************************************
** Function name:           isr_invaild
** Descriptions:            无效中断服务程序
** input parameters:        interrupt           中断号
**                          arg                 参数
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
** Descriptions:            初始化中断
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_init(void)
{
    int i;

    SRCPND      = 0x00;                                                 /*  清除所有中断源等待          */

    SUBSRCPND   = 0x00;                                                 /*  清除所有子中断源等待        */

    INTMOD      = 0x00;                                                 /*  设置所有中断模式为 IRQ      */

    INTMSK      = BIT_ALLMSK;                                           /*  屏蔽所有中断                */

    INTSUBMSK   = BIT_SUB_ALLMSK;                                       /*  屏蔽所有子中断              */

    INTPND      = INTPND;                                               /*  清除所有中断等待            */

    for (i = 0; i < INTERRUPT_NR; i++) {                                /*  初始化中断服务程序及其参数表*/
        isr_table[i]     = (isr_t)isr_invaild;
        isr_arg_table[i] = NULL;
    }
}
/*********************************************************************************************************
** Function name:           interrupt_mask
** Descriptions:            屏蔽中断
** input parameters:        interrupt           中断号
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
** Descriptions:            取消屏蔽中断
** input parameters:        interrupt           中断号
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
** Descriptions:            安装中断服务程序
** input parameters:        interrupt           中断号
**                          new_isr             新的中断服务程序
**                          arg                 中断服务程序参数
** output parameters:       old_isr             旧的中断服务程序
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
