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
#ifndef KERN_CONFIG_H_
#define KERN_CONFIG_H_

#include "syslimits.h"

#ifndef KB
#define KB                          (1024)
#define MB                          (1024 * KB)
#define GB                          (1024 * MB)
#endif

#define SECTION_SIZE                (1 * MB)                            /*  段大小                      */
#define SECTION_OFFSET              (20)                                /*  段大小偏移                  */
#define SECTION_NR                  (4096)                              /*  段数                        */

#ifdef S3C2440_BSP
#include "../../bsp/s3c2440/s3c2440_config.h"
#endif

#ifndef TICK_PER_SECOND
#define TICK_PER_SECOND             (100)                               /*  每秒 TICK 数                */
#endif

#define KERN_HEAP_SIZE              (1 * MB)                            /*  内核动态内存堆大小          */
#define KERN_STACK_SIZE             (8192)                              /*  内核栈空间大小              */

#define PROCESS_NR                  (32)                                /*  进程数, 含进程 0            */
#define KTHREAD_NR                  (16)                                /*  内核线程数                  */
#define TASK_NR                     (PROCESS_NR + KTHREAD_NR)           /*  任务数                      */

#define PROCESS_SPACE_SIZE          (32 * MB)                           /*  进程空间大小                */
#define PROCESS_PARAM_SIZE          (PAGE_SIZE)                         /*  进程参数空间大小            */
#define PROCESS_STACK_SIZE          (128 * KB)                          /*  进程栈空间大小              */

#endif                                                                  /*  KERN_CONFIG_H_              */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
