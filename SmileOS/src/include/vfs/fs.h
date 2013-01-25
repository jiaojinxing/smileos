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
** File name:               fs.h
** Last modified Date:      2012-3-22
** Last Version:            1.0.0
** Descriptions:            �ļ�ϵͳ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-22
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
#ifndef FS_H_
#define FS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "vfs/types.h"

/*********************************************************************************************************
** Function name:           file_system_lookup
** Descriptions:            �����ļ�ϵͳ
** input parameters:        name                �ļ�ϵͳ��
** output parameters:       NONE
** Returned value:          �ļ�ϵͳ OR NULL
*********************************************************************************************************/
file_system_t *file_system_lookup(const char *name);
/*********************************************************************************************************
** Function name:           file_system_install
** Descriptions:            ��װ�ļ�ϵͳ
** input parameters:        fs                  �ļ�ϵͳ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int file_system_install(file_system_t *fs);
/*********************************************************************************************************
** Function name:           file_system_remove
** Descriptions:            ɾ���ļ�ϵͳ
** input parameters:        fs                  �ļ�ϵͳ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int file_system_remove(file_system_t *fs);
/*********************************************************************************************************
** Function name:           file_system_manager_init
** Descriptions:            ��ʼ���ļ�ϵͳ����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int file_system_manager_init(void);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  FS_H_                       */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
