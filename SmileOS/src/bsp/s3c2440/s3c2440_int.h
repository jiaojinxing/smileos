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
** File name:               s3c2440_int.h
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            S3C2440 中断
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
#ifndef S3C2440_INT_H_
#define S3C2440_INT_H_

#include "kern/types.h"

/*
 * 中断服务程序类型
 */
typedef int (*isr_t)(uint32_t interrupt, void *arg);

/*
 * 无效中断服务程序
 */
int isr_invaild(uint32_t interrupt, void *arg);

/*
 * 初始化中断
 */
void interrupt_init(void);

/*
 * 屏蔽中断
 */
void interrupt_mask(uint32_t interrupt);

/*
 * 取消屏蔽中断
 */
void interrupt_unmask(uint32_t interrupt);

/*
 * 安装中断服务程序
 */
void interrupt_install(uint32_t interrupt, isr_t new_isr, isr_t *old_isr, void *arg);

#endif                                                                  /*  S3C2440_INT_H_              */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
