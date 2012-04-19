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
** File name:               ffconf.h
** Last modified Date:      2012-3-17
** Last Version:            1.0.0
** Descriptions:            FatFS ����ͷ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-17
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
#ifndef _FFCONF
#define _FFCONF         6502

#include "vfs/config.h"

#define	_FS_TINY		0                                               /*  ÿ���ļ�ʹ�ø��ԵĻ���      */

#define _FS_READONLY	0                                               /*  �ɶ�д                      */

#define _FS_MINIMIZE	0                                               /*  ȫ����                      */

#define	_USE_STRFUNC	0                                               /*  ��ʹ���ַ�������            */
                                                                        /*  >= 2 ʱ, ������ '\r'        */

#define	_USE_MKFS		1                                               /*  ʹ�ø�ʽ������              */

#define	_USE_FORWARD	0                                               /*  ??? _FS_TINY Ϊ 0 ʱ��Ч    */

#define	_USE_FASTSEEK	1                                               /*  ʹ�ÿ��ٵ����ļ�ָ��        */

#define _CODE_PAGE	    936                                             /*  936 ����ҳ                  */

#define	_USE_LFN	    2                                               /*  ��ʹ�ó��ļ���              */

#define	_MAX_LFN	    NAME_MAX                                        /*  ���ļ�������󳤶�          */

#define	_LFN_UNICODE	0                                               /*  ֧�� unicode                */

#define _FS_RPATH		0                                               /*  �Ƴ����·����غ���        */

#define _VOLUMES	    1                                               /*  ֧�ֵ��߼��豸��            */

#define	_MAX_SS		    512                                             /*  ������СΪ 512 ���ֽ�       */

#define	_MULTI_PARTITION	0                                           /*  ��֧�ֶ����                */

#define	_USE_ERASE	    0                                               /*  ��ʹ��������������          */

#define _WORD_ACCESS	0                                               /*  ��ʹ���ַ���                */

#define _FS_REENTRANT	1                                               /*  ������                      */

#define _FS_TIMEOUT		200                                             /*  ��ʱ TICK ��                */

struct mutex;
#define	_SYNC_t			struct mutex *                                  /*  ͬ����������                */

#define	_FS_SHARE	    (OPEN_MAX - 3)                                  /*  ��ͬʱ�򿪶��ٸ��ļ�        */

#endif                                                                  /*  _FFCONFIG                   */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
