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
** Descriptions:            s3c2440 中断
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
#include "s3c2440.h"
#include "s3c2440_int.h"

#define INTERRUPT_NR      INTGLOBAL

static isr_t isr_table[INTERRUPT_NR];

void irq_c_handler(void)
{
    uint32_t interrupt;
    isr_t    isr;

    interrupt = INTOFFSET;

    if (interrupt >= INTERRUPT_NR) {
        return;
    }

    isr = isr_table[interrupt];

    isr(interrupt);

    SRCPND = 1 << interrupt;

    INTPND = INTPND;
}

int isr_invaild(uint32_t interrupt)
{
    return -1;
}

void interrupt_init(void)
{
    int i;

    /*
     * clear all source pending
     */
    SRCPND = 0x00;

    /*
     * clear all sub source pending
     */
    SUBSRCPND = 0x00;

    /*
     * set all interrupt mode = IRQ mode
     */
    INTMOD = 0x00;

    /*
     * disable all interrupt
     */
    INTMSK = BIT_ALLMSK;

    /*
     * disable all sub interrupt
     */
    INTSUBMSK = BIT_SUB_ALLMSK;

    /*
     * clear all interrupt pending
     */
    INTPND = 0x00;

    /*
     * init isr table
     */
    for (i = 0; i < INTERRUPT_NR; i++) {
        isr_table[i] = (isr_t)isr_invaild;
    }
}

void interrupt_mask(uint32_t interrupt)
{
    if (interrupt < INTERRUPT_NR) {
        INTMSK |= 1 << interrupt;
    }
}

void interrupt_umask(uint32_t interrupt)
{
    if (interrupt < INTERRUPT_NR) {
        INTMSK &= ~(1 << interrupt);
    }
}

void interrupt_install(uint32_t interrupt, isr_t new_isr, isr_t *old_isr)
{
    if (interrupt < INTERRUPT_NR) {
        if (old_isr != NULL) {
            *old_isr = isr_table[interrupt];
        }

        if (new_isr != NULL) {
            isr_table[interrupt] = new_isr;
        } else {
            isr_table[interrupt] = (isr_t)isr_invaild;
        }
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
