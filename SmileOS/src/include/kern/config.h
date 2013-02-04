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

#ifdef __cplusplus
extern "C" {
#endif

#include <syslimits.h>

/*********************************************************************************************************
** ARCH & CPU 配置
*********************************************************************************************************/
#define ARCH_ARM920T                (0)
#define ARCH_ARM926EJ_S             (1)
#define ARCH_ARM7TDMI               (2)

#include "arch/config.h"

#ifndef ARCH_CPU
#error "cpu arch no define"
#endif

#ifndef CONFIG_MMU_EN
#define CONFIG_MMU_EN               (0)
#endif
/*********************************************************************************************************
** 任务数配置
*********************************************************************************************************/
#ifndef PROCESS_NR
#define PROCESS_NR                  (1)                                 /*  进程数                      */
#endif

#define KTHREAD_NR                  (20)                                /*  内核线程数                  */
#define TASK_NR                     (PROCESS_NR + KTHREAD_NR)           /*  任务数                      */
/*********************************************************************************************************
** 中断配置
*********************************************************************************************************/
/*
 * 在 interrupt.c 可配置
 *
#define INTERRUPT_NR                (64)
 */
/*********************************************************************************************************
** IPC 配置
*********************************************************************************************************/
/*
 * 在 ipc.c 可配置
 *
#define MQUEUE_MIN_SIZE             (10)
#define MQUEUE_MAX_SIZE             (10000)
 */
/*********************************************************************************************************
** 内核日志配置
*********************************************************************************************************/
/*
 * 在 klogd.c 可配置
 *
#define KLOGD_QUEUE_SIZE            (100)
#define KLOGD_THREAD_STACKSIZE      (16 * KB)
#define KLOGD_THREAD_PRIO           (20)
#define KLOGD_LOG_LEVEL             (4)
#define KLOGD_LOG_FILE              "/dev/serial0"
 */
/*********************************************************************************************************
** 工作队列配置
*********************************************************************************************************/
/*
 * 在 klogd.c 可配置
 *
#define WQ_MIX_SIZE                 (100)
#define WQ_THREAD_STACKSIZE         (16 * KB)
#define WQ_THREAD_PRIO              (20)
 */

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  KERN_CONFIG_H_              */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
