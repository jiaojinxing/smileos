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
** File name:               config.h
** Last modified Date:      2013-2-4
** Last Version:            1.0.0
** Descriptions:            与体系结构相关的配置
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-4
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
#ifndef ARCH_CONFIG_H_
#define ARCH_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ARCH_CPU                    ARCH_ARM920T

#define CPU_REGS_SIZE               (32)                                /*  CPU 寄存器组大小            */
#define KERN_STACK_SIZE             (2 * KB)                            /*  内核栈空间大小              */
#define TICK_PER_SECOND             (100)                               /*  每秒内核定时器中断次数      */
#define CONFIG_MMU_EN               (1)                                 /*  带 MMU                      */
#define PROCESS_NR                  (20)                                /*  进程数, 含进程 0            */

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  ARCH_CONFIG_H_              */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
