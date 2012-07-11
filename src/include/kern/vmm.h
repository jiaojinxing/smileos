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
** Descriptions:            �����ڴ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-1
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
#ifndef VMM_H_
#define VMM_H_

#include "kern/types.h"

/*
 * ͨ��һ�������ַӳ����������ַ�ռ��е�һ��ҳ��
 */
int vmm_page_map(task_t *task, uint32_t mva);

/*
 * ��ʼ�����̵������ڴ���Ϣ
 */
int vmm_process_init(task_t *task, uint32_t file_size);

/*
 * fork ���̵������ڴ���Ϣ
 */
int vmm_process_fork(task_t *task, task_t *parent);

/*
 * �ͷŽ��̵������ַ�ռ�
 */
void vmm_process_cleanup(task_t *task);

/*
 * ��ʼ�������ڴ����
 */
void vmm_init(void);

/*
 * ��ӡ vmm ��Ϣ���ļ�
 */
int vmm_show(int fd);

#endif                                                                  /*  VMM_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
