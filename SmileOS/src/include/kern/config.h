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
#ifndef KERN_CONFIG_H_
#define KERN_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <syslimits.h>

/*********************************************************************************************************
** ARCH & CPU ����
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
** ����������
*********************************************************************************************************/
#ifndef PROCESS_NR
#define PROCESS_NR                  (1)                                 /*  ������                      */
#endif

#define KTHREAD_NR                  (20)                                /*  �ں��߳���                  */
#define TASK_NR                     (PROCESS_NR + KTHREAD_NR)           /*  ������                      */
/*********************************************************************************************************
** �ж�����
*********************************************************************************************************/
/*
 * �� interrupt.c ������
 *
#define INTERRUPT_NR                (64)
 */
/*********************************************************************************************************
** IPC ����
*********************************************************************************************************/
/*
 * �� ipc.c ������
 *
#define MQUEUE_MIN_SIZE             (10)
#define MQUEUE_MAX_SIZE             (10000)
 */
/*********************************************************************************************************
** �ں���־����
*********************************************************************************************************/
/*
 * �� klogd.c ������
 *
#define KLOGD_QUEUE_SIZE            (100)
#define KLOGD_THREAD_STACKSIZE      (16 * KB)
#define KLOGD_THREAD_PRIO           (20)
#define KLOGD_LOG_LEVEL             (4)
#define KLOGD_LOG_FILE              "/dev/serial0"
 */
/*********************************************************************************************************
** ������������
*********************************************************************************************************/
/*
 * �� klogd.c ������
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
