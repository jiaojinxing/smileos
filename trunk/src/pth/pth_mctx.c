/*
 * pth_mctx.c
 *
 *  Created on: 2012-2-25
 *      Author: Administrator
 */

#include "pth_p.h"
#include "types.h"
#include "arm.h"
#include "mem.h"

int pth_mctx_set(pth_mctx_t *mctx, void (*func)(void), char *sk_addr_lo, char *sk_addr_hi)
{
    uint32_t *stk;

    stk    = MEM_ALIGN_LESS(sk_addr_hi - 4);
    *stk-- = (uint32_t)func;    /* pc   */
    *stk-- = (uint32_t)func;    /* lr   */
    *stk-- = 0;                 /* r12  */
    *stk-- = 0;                 /* r11  */
    *stk-- = 0;                 /* r10  */
    *stk-- = 0;                 /* r9   */
    *stk-- = 0;                 /* r8   */
    *stk-- = 0;                 /* r7   */
    *stk-- = 0;                 /* r6   */
    *stk-- = 0;                 /* r5   */
    *stk-- = 0;                 /* r4   */
    *stk-- = 0;                 /* r3   */
    *stk-- = 0;                 /* r2   */
    *stk-- = 0;                 /* r1   */
    *stk-- = 0;                 /* r0   */
    *stk   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;        /* cpsr: sys mode, irq enable, fiq dissable */

    mctx->sp = (uint32_t)stk;

    return 1;
}
