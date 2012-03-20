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
#include "s3c2440_int.h"

#define INTERRUPT_NR      INTGLOBAL

/*
 * 中断服务程序及其参数表
 */
static isr_t  isr_table[INTERRUPT_NR];
static void  *isr_arg_table[INTERRUPT_NR];

/*
 * IRQ 中断处理程序
 */
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
        return;
    }

    isr = isr_table[interrupt];                                         /*  调用中断服务程序            */
    isr(interrupt, isr_arg_table[interrupt]);

    SRCPND  = 1 << interrupt;                                           /*  清除中断源等待              */

    INTPND  = INTPND;                                                   /*  清除中断等待                */

    interrupt_exit();                                                   /*  退出中断                    */
}

/*
 * 无效中断服务程序
 */
int isr_invaild(uint32_t interrupt, void *arg)
{
    printk("invaild interrupt %d!\n", interrupt);

    return -1;
}

/*
 * 初始化中断
 */
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

/*
 * 屏蔽中断
 */
void interrupt_mask(uint32_t interrupt)
{
    if (interrupt < INTERRUPT_NR) {
        INTMSK |= 1 << interrupt;
    }
}

/*
 * 取消屏蔽中断
 */
void interrupt_unmask(uint32_t interrupt)
{
    if (interrupt < INTERRUPT_NR) {
        INTMSK &= ~(1 << interrupt);
    }
}

/*
 * 安装中断服务程序
 */
void interrupt_install(uint32_t interrupt, isr_t new_isr, isr_t *old_isr, void *arg)
{
    if (interrupt < INTERRUPT_NR) {
        if (old_isr != NULL) {
            *old_isr = isr_table[interrupt];
        }

        if (new_isr != NULL) {
            isr_table[interrupt]     = new_isr;
            isr_arg_table[interrupt] = arg;
        } else {
            isr_table[interrupt]     = (isr_t)isr_invaild;
        }
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
