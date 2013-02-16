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
** File name:               select.h
** Last modified Date:      2012-11-5
** Last Version:            1.0.0
** Descriptions:            select ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-11-5
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
#ifndef VFS_UTILS_H_
#define VFS_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "vfs/types.h"

/*********************************************************************************************************
** Function name:           select_select
** Descriptions:            ����ǰ������뵽�ļ��ĵȴ��б�
** input parameters:        select              select �ṹ
**                          file                �ļ�
**                          type                �ȴ�����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_select(select_struct_t *select, file_t *file, int type);
/*********************************************************************************************************
** Function name:           select_unselect
** Descriptions:            ����ǰ������ļ��ĵȴ��б����Ƴ�
** input parameters:        select              select �ṹ
**                          file                �ļ�
**                          type                �ȴ�����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_unselect(select_struct_t *select, file_t *file, int type);
/*********************************************************************************************************
** Function name:           select_report
** Descriptions:            �ر��¼�
** input parameters:        select              select �ṹ
**                          type                �ر�����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_report(select_struct_t *select, int type);
/*********************************************************************************************************
** Function name:           select_helper
** Descriptions:            select ��������
** input parameters:        select              select �ṹ
**                          scan                ɨ�躯��
**                          ctx                 ������
**                          file                �ļ��ṹ
**                          type                �ر�����
** output parameters:       NONE
** Returned value:          0 OR -1 OR 1
*********************************************************************************************************/
int select_helper(select_struct_t *select,
                  int (*scan)(void *, file_t *, int),
                  void *ctx, file_t *file, int type);
/*********************************************************************************************************
** Function name:           select_init
** Descriptions:            ��ʼ�� select
** input parameters:        select              select �ṹ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_init(select_struct_t *select);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  VFS_UTILS_H_                */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
