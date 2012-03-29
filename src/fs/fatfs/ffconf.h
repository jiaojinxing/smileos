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
** Descriptions:            FatFS 配置头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-17
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
#ifndef _FFCONF
#define _FFCONF         6502

#include "vfs/config.h"

#define	_FS_TINY		0                                               /*  每个文件使用各自的缓冲      */

#define _FS_READONLY	0                                               /*  可读写                      */

#define _FS_MINIMIZE	0                                               /*  全功能                      */

#define	_USE_STRFUNC	0                                               /*  不使用字符串函数            */
                                                                        /*  >= 2 时, 会跳过 '\r'        */

#define	_USE_MKFS		1                                               /*  使用格式化函数              */

#define	_USE_FORWARD	0                                               /*  ??? _FS_TINY 为 0 时无效    */

#define	_USE_FASTSEEK	1                                               /*  使用快速调整文件指针        */

#define _CODE_PAGE	    936                                             /*  936 代码页                  */

#define	_USE_LFN	    0                                               /*  不使用长文件名              */

#define	_MAX_LFN	    PATH_MAX                                        /*  长文件名的最大长度          */

#define	_LFN_UNICODE	0                                               /*  支持 unicode                */

#define _FS_RPATH		0                                               /*  移除相对路径相关函数        */

#define _VOLUMES	    1                                               /*  支持的逻辑设备数            */

#define	_MAX_SS		    512                                             /*  扇区大小为 512 个字节       */

#define	_MULTI_PARTITION	0                                           /*  不支持多分区                */

#define	_USE_ERASE	    1                                               /*  使用扇区擦除函数            */

#define _WORD_ACCESS	0                                               /*  不使用字访问                */

#define _FS_REENTRANT	1                                               /*  可重入                      */

#define _FS_TIMEOUT		1000                                            /*  超时 TICK 数                */

struct kern_mutex;
#define	_SYNC_t			struct kern_mutex *                             /*  同步对象类型                */

#define	_FS_SHARE	    (OPEN_MAX - 3)                                  /*  能同时打开多少个文件        */

#endif                                                                  /*  _FFCONFIG                   */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
