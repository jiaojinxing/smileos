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
** File name:               pinfo.h
** Last modified Date:      2013-1-5
** Last Version:            1.0.0
** Descriptions:            ������Ϣ
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-1-5
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
#ifndef PINFO_H_
#define PINFO_H_

#include <dirent.h>
struct _reent;

typedef struct {
    struct _reent      *reent;                                          /*  ������ṹָ��            ��*/
    struct dirent       entry;                                          /*  Ŀ¼��ṹ����            ��*/
    char                cwd[PATH_MAX];                                  /*  ��ǰ����Ŀ¼��            ��*/
} pinfo_t;

#endif                                                                  /*  PINFO_H_                    */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
