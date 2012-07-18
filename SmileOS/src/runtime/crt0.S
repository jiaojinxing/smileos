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
** File name:               crt0.S
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            C 运行时启动文件
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

.extern libc_init
.extern _cppRtInit
.extern main
.extern exit

/*
 * 处理器模式
 */
#define ARM_USR_MODE    0x10
#define ARM_FIQ_MODE    0x11
#define ARM_IRQ_MODE    0x12
#define ARM_SVC_MODE    0x13
#define ARM_ABT_MODE    0x17
#define ARM_UDF_MODE    0x1B
#define ARM_SYS_MODE    0x1F
#define ARM_MODE_MASK   0x1F

/*
 * 中断禁能位
 */
#define ARM_FIQ_NO      (1 << 6)
#define ARM_FIQ_EN      (0 << 6)

#define ARM_IRQ_NO      (1 << 7)
#define ARM_IRQ_EN      (0 << 7)
/*********************************************************************************************************
  函数: _start
*********************************************************************************************************/
.text
.code 32
.align 2

.global _start
_start:

    mov     r0, #0                                                      /*  对 bss 段进行清零           */
    ldr     r1, =__bss_start
    ldr     r2, =__bss_end

bss_loop:
    cmp     r1, r2
    strlo   r0, [r1], #4
    blo     bss_loop

    bl      libc_init                                                   /*  初始化 C 库                 */

    bl      _cppRtInit                                                  /*  初始化 C++ 运行时支持       */

    ldr     r2, =(PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE)
    ldr     r0, [r2]

    ldr     r1, =(PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE + 4)

    bl      main                                                        /*  进入 main 函数              */

    bl      exit
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
