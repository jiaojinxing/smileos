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
** File name:               s3c2440_config.h
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            S3C2440 配置头文件
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
#ifndef S3C2440_CONFIG_H_
#define S3C2440_CONFIG_H_

#define SECTION_SIZE        (1 * MB)                                    /*  段大小                      */
#define SECTION_OFFSET      (20)                                        /*  段大小偏移                  */
#define SECTION_NR          (4096)                                      /*  段数                        */

#define PHY_MEM_SIZE        (64 * MB)                                   /*  物理内存大小                */
#define PHY_MEM_BASE        (0x30000000)                                /*  物理内存基址                */

#define KERN_MEM_SIZE       (8 * MB)                                    /*  内核内存大小                */
#define KERN_MEM_BASE       (PHY_MEM_BASE)                              /*  内核内存基址                */

#define INT_MEM_SIZE        (1 * MB)                                    /*  中断内存大小                */
#define INT_MEM_BASE        (KERN_MEM_BASE + KERN_MEM_SIZE)             /*  中断内存基址                */

#define VMM_MEM_SIZE        (PHY_MEM_SIZE - KERN_MEM_SIZE-INT_MEM_SIZE) /*  VMM 内存大小                */
#define VMM_MEM_BASE        (INT_MEM_BASE + INT_MEM_SIZE)               /*  VMM 内存基址                */
#define VMM_FRAME_SIZE      (PAGE_SIZE)                                 /*  页框大小                    */
#define VMM_FRAME_NR        (VMM_MEM_SIZE / VMM_FRAME_SIZE)             /*  页框数                      */

#define PAGE_TBL_SIZE       (1 * KB)                                    /*  页表大小                    */
#define PAGE_TBL_NR         (1024)                                      /*  页表数                      */
                                                                        /*  页表数组基址                */
#define PAGE_TBL_BASE       (KERN_MEM_BASE + KERN_MEM_SIZE - PAGE_TBL_NR * PAGE_TBL_SIZE)

#define MMU_TBL_SIZE        (16 * KB)                                   /*  MMU 转换表大小              */
#define MMU_TBL_BASE        (PAGE_TBL_BASE - MMU_TBL_SIZE)              /*  MMU 转换表基址              */

#define PROCESS0_STACK_BASE (MMU_TBL_BASE)                              /*  进程 0 栈空间基址           */

#define KERN_LOAD_ADDR      (KERN_MEM_BASE)                             /*  内核加载地址                */

#define VECTOR_V_ADDR       (0xFFFF0000)                                /*  向量虚拟地址                */
#define VECTOR_P_ADDR       (INT_MEM_BASE + 0xF0000)                    /*  向量物理地址                */

#define IRQ_STACK_P_BASE    (VECTOR_P_ADDR)                             /*  IRQ 堆栈物理基址            */
#define IRQ_STACK_V_BASE    (VECTOR_V_ADDR)                             /*  IRQ 堆栈虚拟基址            */

#define KERN_HEAP_SIZE      (1 * MB)                                    /*  内核动态内存堆大小          */
#define KERN_STACK_SIZE     (8192)                                      /*  内核栈空间大小              */

#define PROCESS_NR          (32)                                        /*  进程数, 含进程 0            */
#define KTHREAD_NR          (16)                                        /*  内核线程数                  */
#define TASK_NR             (PROCESS_NR + KTHREAD_NR)                   /*  任务数                      */

#define UART_BAUD_RATE      (115200)                                    /*  UART 波特率                 */

#define TICK_PER_SECOND     (100)                                       /*  每秒 TICK 数                */

#endif                                                                  /*  S3C2440_CONFIG_H_           */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
