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
** Descriptions:            s3c2440 ����ͷ�ļ�
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
#ifndef S3C2440_CONFIG_H_
#define S3C2440_CONFIG_H_

#define PHY_MEM_BASE        (0x30000000)                                /*  �����ڴ��ַ                */
#define PHY_MEM_SIZE        (64 * MB)                                   /*  �����ڴ��С                */

#define KERN_MEM_SIZE       (3 * MB)                                    /*  �ں��ڴ��С                */
#define KERN_MEM_BASE       PHY_MEM_BASE                                /*  �ں��ڴ��ַ                */

#define INT_MEM_SIZE        (1 * MB)                                    /*  �ж��ڴ��С                */
#define INT_MEM_BASE        (KERN_MEM_BASE + KERN_MEM_SIZE)             /*  �ж��ڴ��ַ                */

#define PROCESS_SPACE_SIZE  (32 * MB)                                   /*  ���̿ռ��С                */
#define PROCESS_MEM_SIZE    (4 * MB)                                    /*  �����ڴ��С                */
#define PROCESS_MEM_BASE    (INT_MEM_BASE + INT_MEM_SIZE)               /*  �����ڴ��ַ                */
                                                                        /*  ������, �������� 0          */
#define PROCESS_NR          ((PHY_MEM_SIZE - KERN_MEM_SIZE - INT_MEM_SIZE) / PROCESS_MEM_SIZE)

#ifdef SMILEOS_KTHREAD
#define THREAD_NR           64                                          /*  �߳���                      */
#else
#define THREAD_NR           0                                           /*  �߳���                      */
#endif

#define TASK_NR             (PROCESS_NR + 1 + THREAD_NR)                /*  ������                      */

#define MMU_TBL_ALIGNED     (32 * KB)                                   /*  MMU ת�����ַ�����С      */
                                                                        /*  MMU ת�����ַ              */
#define MMU_TBL_BASE        (KERN_MEM_BASE + KERN_MEM_SIZE - MMU_TBL_ALIGNED)
#define MMU_TBL_SIZE        (16 * KB)                                   /*  MMU ת�����С              */

#define PROCESS0_STACK_BASE MMU_TBL_BASE

#define KERN_LOAD_ADDR      (KERN_MEM_BASE)                             /*  �ں˼��ص�ַ                */
#define KERN_STACK_SIZE     1024                                        /*  �ں˶�ջ��С                */

#define VECTOR_V_ADDR       (0xFFFFF0000)                               /*  ���������ַ                */
#define VECTOR_P_ADDR       (INT_MEM_BASE + 0xF0000)                    /*  ���������ַ                */

#define IRQ_STACK_P_BASE    VECTOR_P_ADDR                               /*  IRQ ��ջ�����ַ            */
#define IRQ_STACK_V_BASE    VECTOR_V_ADDR                               /*  IRQ ��ջ�����ַ            */

#define UART_BAUD_RATE      115200                                      /*  UART ������                 */

#define TICK_PER_SECOND     100                                         /*  ÿ�� tick ��                */

#define PROCESS_HEAP_SIZE   (PROCESS_MEM_SIZE - 1 * MB)                 /*  ���̶Ѵ�С                  */

#define KERN_HEAP_SIZE      (KERN_MEM_SIZE - 1 * MB)                    /*  �ں˶Ѵ�С                  */

#endif                                                                  /*  S3C2440_CONFIG_H_           */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
