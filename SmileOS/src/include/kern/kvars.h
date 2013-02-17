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
** Descriptions:            �ں˱���ͷ�ļ�
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
#ifndef KVARS_H_
#define KVARS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KVARS_INC
#define KVAR            extern
#else
#define KVAR
#endif

KVAR task_t             tasks[TASK_NR];                                 /*  ������ƿ�                  */
KVAR struct _reent      reents[1 + KTHREAD_NR];                         /*  ������ṹ����              */
KVAR task_t            *current;                                        /*  ָ��ǰ���е�����          */
KVAR uint8_t            interrupt_nest;                                 /*  �ж�Ƕ�ײ��                */
KVAR bool_t             os_started;                                     /*  �ں��Ƿ���������            */

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  KVARS_H_                    */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
