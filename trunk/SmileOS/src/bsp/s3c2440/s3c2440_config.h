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

#define PHY_MEM_SIZE        (64 * MB)                                   /*  �����ڴ��С                */
#define PHY_MEM_BASE        (0x30000000)                                /*  �����ڴ��ַ                */

#define KERN_MEM_SIZE       (20 * MB)                                   /*  �ں��ڴ��С                */
#define KERN_MEM_BASE       (PHY_MEM_BASE)                              /*  �ں��ڴ��ַ                */

#define PAGE_TBL_SIZE       (1 * KB)                                    /*  ҳ���С                    */
#define PAGE_TBL_NR         (1024 - 16)                                 /*  ҳ����                      */
                                                                        /*  ҳ�������ַ                */
#define PAGE_TBL_BASE       (KERN_MEM_BASE + KERN_MEM_SIZE - PAGE_TBL_NR * PAGE_TBL_SIZE)

#define MMU_TBL_SIZE        (16 * KB)                                   /*  MMU ת�����С              */
#define MMU_TBL_BASE        (PAGE_TBL_BASE - MMU_TBL_SIZE)              /*  MMU ת�����ַ              */

#define KERN_MEM_TOP        (MMU_TBL_BASE)

#define FB_MEM_SIZE         (4 * MB)                                    /*  �ж��ڴ��С                */
#define FB_MEM_BASE         (KERN_MEM_BASE + KERN_MEM_SIZE)             /*  �ж��ڴ��ַ                */

#define INT_MEM_SIZE        (1 * MB)                                    /*  �ж��ڴ��С                */
#define INT_MEM_BASE        (FB_MEM_BASE + FB_MEM_SIZE)                 /*  �ж��ڴ��ַ                */

#define DMA_MEM_SIZE        (1 * MB)                                    /*  DMA �ڴ��С                */
#define DMA_MEM_BASE        (INT_MEM_BASE + INT_MEM_SIZE)               /*  DMA �ڴ��ַ                */

#define HW_SHARE_MEM_SIZE   (1 * MB)                                    /*  Ӳ�������ڴ��С            */
#define HW_SHARE_MEM_BASE   (DMA_MEM_BASE + DMA_MEM_SIZE)               /*  Ӳ�������ڴ��ַ            */

#define SW_SHARE_MEM_SIZE   (1 * MB)                                    /*  ��������ڴ��С            */
#define SW_SHARE_MEM_BASE   (HW_SHARE_MEM_BASE + HW_SHARE_MEM_SIZE)     /*  ��������ڴ��ַ            */

                                                                        /*  VMM �ڴ��С                */
#define VMM_MEM_SIZE        (PHY_MEM_SIZE - KERN_MEM_SIZE - FB_MEM_SIZE - \
                             INT_MEM_SIZE - DMA_MEM_SIZE - HW_SHARE_MEM_SIZE - SW_SHARE_MEM_SIZE)
#define VMM_MEM_BASE        (SW_SHARE_MEM_BASE + SW_SHARE_MEM_SIZE)     /*  VMM �ڴ��ַ                */
#define VMM_PHY_PAGE_SIZE   (4096)                                      /*  ����ҳ���С                */
#define VMM_PHY_PAGE_NR     (VMM_MEM_SIZE / VMM_PHY_PAGE_SIZE)             /*  ����ҳ����                  */

#define VMM_PAGE_SIZE       (VMM_PHY_PAGE_SIZE)
#define VMM_SECTION_NR      (4096)
#define VMM_SECTION_SIZE    (1 * MB)
#define VMM_SECTION_OFFSET  (20)

#define VECTOR_V_ADDR       (0xFFFF0000)                                /*  ���������ַ                */
#define VECTOR_P_ADDR       (INT_MEM_BASE + 0xF0000)                    /*  ���������ַ                */

#define IRQ_STACK_P_BASE    (VECTOR_P_ADDR)                             /*  IRQ ��ջ�����ַ            */
#define IRQ_STACK_V_BASE    (VECTOR_V_ADDR)                             /*  IRQ ��ջ�����ַ            */

#define UART_BAUD_RATE      (115200)                                    /*  UART ������                 */

#define SYS_CLK_FREQ        (12000000)                                  /*  Fin = 12.00MHz              */

#define PROCESS_SPACE_SIZE  (32 * MB)
#define PROCESS_PARAM_SIZE  (VMM_PAGE_SIZE)
#define PROCESS_STACK_SIZE  (8 * VMM_PAGE_SIZE)

#endif                                                                  /*  S3C2440_CONFIG_H_           */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
