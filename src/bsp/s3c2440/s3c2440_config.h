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

#define PAGE_SIZE           (4 * KB)
#define PAGE_OFFSET         (12)
#define FRAME_SIZE          (PAGE_SIZE)

#define SECTION_SIZE        (1 * MB)
#define SECTION_OFFSET      (20)

#define PHY_MEM_SIZE        (64 * MB)                                   /*  �����ڴ��С                */
#define PHY_MEM_BASE        (0x30000000)                                /*  �����ڴ��ַ                */

#define KERN_MEM_SIZE       (7 * MB)                                    /*  �ں��ڴ��С                */
#define KERN_MEM_BASE       PHY_MEM_BASE                                /*  �ں��ڴ��ַ                */

#define INT_MEM_SIZE        (1 * MB)                                    /*  �ж��ڴ��С                */
#define INT_MEM_BASE        (KERN_MEM_BASE + KERN_MEM_SIZE)             /*  �ж��ڴ��ַ                */

#define VMM_MEM_SIZE        (PHY_MEM_SIZE - KERN_MEM_SIZE-INT_MEM_SIZE) /*  VMM �ڴ��С                */
#define VMM_MEM_BASE        (INT_MEM_BASE + INT_MEM_SIZE)               /*  VMM �ڴ��ַ                */

#define PROCESS_SPACE_SIZE  (32 * MB)                                   /*  ���̿ռ��С                */
#define PROCESS_NR          (15)                                        /*  ������, ������ 0            */

#define PAGE_TBL_SIZE       (1 * KB)
#define PAGE_TBL_NR         (1024)
#define PAGE_TBL_BASE       (KERN_MEM_BASE + KERN_MEM_SIZE - PAGE_TBL_NR * PAGE_TBL_SIZE)

#define MMU_TBL_SIZE        (16 * KB)                                   /*  MMU ת�����С              */
#define MMU_TBL_BASE        (PAGE_TBL_BASE - MMU_TBL_SIZE)              /*  MMU ת�����ַ              */

#define PROCESS0_STACK_BASE (MMU_TBL_BASE)

#define KERN_LOAD_ADDR      (KERN_MEM_BASE)                             /*  �ں˼��ص�ַ                */

#define VECTOR_V_ADDR       (0xFFFFF0000)                               /*  ���������ַ                */
#define VECTOR_P_ADDR       (INT_MEM_BASE + 0xF0000)                    /*  ���������ַ                */

#define IRQ_STACK_P_BASE    (VECTOR_P_ADDR)                             /*  IRQ ��ջ�����ַ            */
#define IRQ_STACK_V_BASE    (VECTOR_V_ADDR)                             /*  IRQ ��ջ�����ַ            */

#define UART_BAUD_RATE      115200                                      /*  UART ������                 */

#define TICK_PER_SECOND     100                                         /*  ÿ�� tick ��                */

/*
 * TODO
 */
#define PROCESS_HEAP_SIZE   (PROCESS_SPACE_SIZE - 1 * MB)               /*  ���̶Ѵ�С                  */

#define KERN_HEAP_SIZE      (2 * MB)                                    /*  �ں˶Ѵ�С                  */

#ifdef SMILEOS_KTHREAD
#define THREAD_NR           32                                          /*  �߳���                      */
#else
#define THREAD_NR           0                                           /*  �߳���                      */
#endif

#define TASK_NR             (PROCESS_NR + THREAD_NR)                    /*  ������                      */

#define KERN_STACK_SIZE     2048                                        /*  �ں˶�ջ��С                */

#define FRAME_NR            (VMM_MEM_SIZE / FRAME_SIZE)

#endif                                                                  /*  S3C2440_CONFIG_H_           */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
