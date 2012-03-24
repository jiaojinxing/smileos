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
** Descriptions:            S3C2440 ����ͷ�ļ�
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

#define PAGE_SIZE           (4 * KB)                                    /*  ҳ���С                    */
#define PAGE_OFFSET         (12)                                        /*  ҳ���Сƫ��                */

#define SECTION_SIZE        (1 * MB)                                    /*  �δ�С                      */
#define SECTION_OFFSET      (20)                                        /*  �δ�Сƫ��                  */
#define SECTION_NR          (4096)                                      /*  ����                        */

#define PHY_MEM_SIZE        (64 * MB)                                   /*  �����ڴ��С                */
#define PHY_MEM_BASE        (0x30000000)                                /*  �����ڴ��ַ                */

#define KERN_MEM_SIZE       (8 * MB)                                    /*  �ں��ڴ��С                */
#define KERN_MEM_BASE       (PHY_MEM_BASE)                              /*  �ں��ڴ��ַ                */

#define INT_MEM_SIZE        (1 * MB)                                    /*  �ж��ڴ��С                */
#define INT_MEM_BASE        (KERN_MEM_BASE + KERN_MEM_SIZE)             /*  �ж��ڴ��ַ                */

#define VMM_MEM_SIZE        (PHY_MEM_SIZE - KERN_MEM_SIZE-INT_MEM_SIZE) /*  VMM �ڴ��С                */
#define VMM_MEM_BASE        (INT_MEM_BASE + INT_MEM_SIZE)               /*  VMM �ڴ��ַ                */
#define VMM_FRAME_SIZE      (PAGE_SIZE)                                 /*  ҳ���С                    */
#define VMM_FRAME_NR        (VMM_MEM_SIZE / VMM_FRAME_SIZE)             /*  ҳ����                      */

#define PAGE_TBL_SIZE       (1 * KB)                                    /*  ҳ����С                    */
#define PAGE_TBL_NR         (1024)                                      /*  ҳ����                      */
                                                                        /*  ҳ�������ַ                */
#define PAGE_TBL_BASE       (KERN_MEM_BASE + KERN_MEM_SIZE - PAGE_TBL_NR * PAGE_TBL_SIZE)

#define MMU_TBL_SIZE        (16 * KB)                                   /*  MMU ת������С              */
#define MMU_TBL_BASE        (PAGE_TBL_BASE - MMU_TBL_SIZE)              /*  MMU ת������ַ              */

#define PROCESS0_STACK_BASE (MMU_TBL_BASE)                              /*  ���� 0 ջ�ռ��ַ           */

#define KERN_LOAD_ADDR      (KERN_MEM_BASE)                             /*  �ں˼��ص�ַ                */

#define VECTOR_V_ADDR       (0xFFFFF0000)                               /*  ���������ַ                */
#define VECTOR_P_ADDR       (INT_MEM_BASE + 0xF0000)                    /*  ����������ַ                */

#define IRQ_STACK_P_BASE    (VECTOR_P_ADDR)                             /*  IRQ ��ջ������ַ            */
#define IRQ_STACK_V_BASE    (VECTOR_V_ADDR)                             /*  IRQ ��ջ�����ַ            */

#define KERN_HEAP_SIZE      (2 * MB)                                    /*  �ں˶�̬�ڴ�Ѵ�С          */
#define KERN_STACK_SIZE     (2048)                                      /*  �ں�ջ�ռ��С              */

#define PROCESS_NR          (64)                                        /*  ������, ������ 0            */
#define KTHREAD_NR          (32)                                        /*  �ں��߳���                  */
#define TASK_NR             (PROCESS_NR + KTHREAD_NR)                   /*  ������                      */

#define UART_BAUD_RATE      (115200)                                    /*  UART ������                 */

#define TICK_PER_SECOND     (100)                                       /*  ÿ�� TICK ��                */

#endif                                                                  /*  S3C2440_CONFIG_H_           */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/