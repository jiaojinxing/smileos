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
** File name:               vmm.h
** Last modified Date:      2012-3-1
** Last Version:            1.0.0
** Descriptions:            虚拟内存管理
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-1
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
#ifndef VMM_H_
#define VMM_H_

#include "kern/types.h"

/*
 * 通过一个虚拟地址映射进程虚拟地址空间中的一个页面
 */
int vmm_page_map(task_t *task, uint32_t mva);

/*
 * 初始化进程的虚拟内存信息
 */
int vmm_process_init(task_t *task, uint32_t file_size);

/*
 * 释放进程的虚拟地址空间
 */
void vmm_process_cleanup(task_t *task);

/*
 * 初始化虚拟内存管理
 */
void vmm_init(void);

/*
 * 打印 vmm 信息到文件
 */
int vmm_show(int fd);

#endif                                                                  /*  VMM_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
