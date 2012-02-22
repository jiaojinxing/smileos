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
** File name:               config.h
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            配置头文件
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
#ifndef CONFIG_H_
#define CONFIG_H_

#define KB                  (1024)
#define MB                  (1024 * KB)
#define PAGE                (4 * KB)

#define PHY_MEM_BASE        (0x30000000)                                /*  物理内存基址                */
#define PHY_MEM_SIZE        (64 * MB)                                   /*  物理内存大小                */

#define KERN_MEM_SIZE       (3 * MB)                                    /*  内核内存大小                */
#define KERN_MEM_BASE       PHY_MEM_BASE                                /*  内核内存基址                */

#define INT_MEM_SIZE        (1 * MB)                                    /*  中断内存大小                */
#define INT_MEM_BASE        (KERN_MEM_BASE + KERN_MEM_SIZE)             /*  中断内存基址                */

#define PROCESS_SPACE_SIZE  (32 * MB)                                   /*  进程空间大小                */
#define PROCESS_MEM_SIZE    (4 * MB)                                    /*  进程内存大小                */
#define PROCESS_MEM_BASE    (INT_MEM_BASE + INT_MEM_SIZE)               /*  进程内存基址                */
                                                                        /*  进程数, 不含进程 0          */
#define PROCESS_NR          ((PHY_MEM_SIZE - KERN_MEM_SIZE - INT_MEM_SIZE) / PROCESS_MEM_SIZE)

#define THREAD_NR           64                                          /*  线程数                      */

#define TASK_NR             (PROCESS_NR + 1 + THREAD_NR)                /*  任务数                      */

#define MMU_TBL_BASE        (KERN_MEM_BASE + KERN_MEM_SIZE - 32 * KB)   /*  MMU 转换表基址              */
#define MMU_TBL_SIZE        (16 * KB)                                   /*  MMU 转换表大小              */

#define KERN_LOAD_ADDR      (KERN_MEM_BASE)                             /*  内核加载地址                */
#define KERN_STACK_SIZE     1024                                        /*  内核堆栈大小                */

#define VECTOR_V_ADDR       (0xFFFFF0000)                               /*  向量虚拟地址                */
#define VECTOR_P_ADDR       (INT_MEM_BASE + 0xF0000)                    /*  向量物理地址                */

#define IRQ_STACK_P_BASE    VECTOR_P_ADDR                               /*  IRQ 堆栈物理基址            */
#define IRQ_STACK_V_BASE    VECTOR_V_ADDR                               /*  IRQ 堆栈虚拟基址            */

#define TICK_PER_SECOND     100                                         /*  每秒 tick 数                */

#define UART_BAUD_RATE      115200                                      /*  UART 波特率                 */

#endif                                                                  /*  CONFIG_H_                   */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
