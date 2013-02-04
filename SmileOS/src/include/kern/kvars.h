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
** File name:               kvars.h
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            内核变量头文件
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
#ifndef KVARS_H_
#define KVARS_H_

#ifndef KVARS_INC
#define KVAR            extern
#else
#define KVAR
#endif

KVAR task_t             tasks[TASK_NR];                                 /*  任务控制块                  */
KVAR struct _reent      reents[1 + KTHREAD_NR];                         /*  可重入结构数组              */
KVAR task_t            *current;                                        /*  指向当前运行的任务          */
KVAR uint8_t            interrupt_nest;                                 /*  中断嵌套层次                */
KVAR bool_t             os_started;                                     /*  内核是否正在运行            */

#endif                                                                  /*  KVARS_H_                    */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
