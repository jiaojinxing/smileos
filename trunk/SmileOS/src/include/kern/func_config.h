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
** File name:               func_config.h
** Last modified Date:      2013-2-3
** Last Version:            1.0.0
** Descriptions:            ��������ͷ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-3
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
#ifndef FUNC_CONFIG_H_
#define FUNC_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
** ��������
**
** ֻ�Ƕ��ں˵Ĺ��ܽ�������, �����ں˴������, Ҳ����Ӧ�ò�ӿڼ���
*********************************************************************************************************/
#define CONFIG_SIGNAL_EN        (0)
#define CONFIG_PROCESS_EN       (1)
#define CONFIG_VFS_EN           (1)
#define CONFIG_NET_EN           (1)
#define CONFIG_MODULE_EN        (1)
/*********************************************************************************************************
** ���ܼ��
*********************************************************************************************************/
#if CONFIG_PROCESS_EN > 0
#if CONFIG_VFS_EN == 0
#error "process depend on vfs"
#endif
#endif

#if CONFIG_NET_EN > 0
#if CONFIG_VFS_EN == 0
#error "net depend on vfs"
#endif
#endif

#if CONFIG_MODULE_EN > 0
#if CONFIG_VFS_EN == 0
#error "module depend on vfs"
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  FUNC_CONFIG_H_              */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
