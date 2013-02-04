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
#include "kern/addr_config.h"

.extern libc_init
.extern _cppRtInit
.extern main
.extern exit

                            .text
                            .code 32
                            .align 2
/*********************************************************************************************************
** Function name:           _start
** Descriptions:            C 程序进入点
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
                            .global _start
_start:

    MOV     R0, #0                                                      /*  对 bss 段进行清零           */
    LDR     R1, =__bss_start
    LDR     R2, =__bss_end

bss_loop:
    CMP     R1, R2
    STRLO   R0, [R1], #4
    BLO     bss_loop

    BL      libc_init                                                   /*  初始化 C 库                 */

    BL      _cppRtInit                                                  /*  初始化 C++ 运行时支持       */

    LDR     R2, =(PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE)
    LDR     R0, [R2]

    LDR     R1, =(PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE + 4)

    BL      main                                                        /*  进入 main 函数              */

    BL      exit
/*********************************************************************************************************
** Function name:           arch_kernel_enter
** Descriptions:            进入内核
** input parameters:        R0                  系统调用参数指针
                            R1                  系统调用号
** output parameters:       NONE
** Returned value:          系统调用返回值
*********************************************************************************************************/
                            .global arch_kernel_enter
arch_kernel_enter:
    SWI     0
    MOV     PC, LR
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
