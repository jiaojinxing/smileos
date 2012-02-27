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
** Descriptions:            ����ͷ�ļ�
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
#ifndef CONFIG_H_
#define CONFIG_H_

#define KB                  (1024)
#define MB                  (1024 * KB)
#define PAGE                (4 * KB)

#ifdef S3C2440_BSP
#include "../../bsp/s3c2440/s3c2440_config.h"
#endif

#ifndef TICK_PER_SECOND
#define TICK_PER_SECOND     100                                         /*  ÿ�� tick ��                */
#endif

#ifndef PROCESS_HEAP_SIZE
#define PROCESS_HEAP_SIZE   (3 * MB)                                    /*  ���̶Ѵ�С                  */
#endif

#ifndef KERN_HEAP_SIZE
#define KERN_HEAP_SIZE      (2 * MB)                                    /*  �ں˶Ѵ�С                  */
#endif

#ifdef SMILEOS_KTHREAD
#define THREAD_NR           64                                          /*  �߳���                      */
#else
#define THREAD_NR           0                                           /*  �߳���                      */
#endif

#define TASK_NR             (PROCESS_NR + 1 + THREAD_NR)                /*  ������                      */

#endif                                                                  /*  CONFIG_H_                   */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
