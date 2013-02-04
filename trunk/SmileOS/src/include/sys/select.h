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
** Last modified Date:      2012-4-11
** Last Version:            1.0.0
** Descriptions:            select 头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-11
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
#ifndef SYS_SELECT_H_
#define SYS_SELECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>
/*********************************************************************************************************
** Function name:           select
** Descriptions:            将当前任务加入到文件集的等待列表
** input parameters:        nfds                文件描述符范围
**                          readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** output parameters:       readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** Returned value:          文件集总共设置了多少个位 OR -1
*********************************************************************************************************/
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  SYS_SELECT_H_               */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
