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
** File name:               fcntl.h
** Last modified Date:      2012-4-5
** Last Version:            1.0.0
** Descriptions:            文件控制头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-5
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
#ifndef FCNTL_H_
#define FCNTL_H_

#include <sys/fcntl.h>
#include <sys/ioctl.h>

/* We want to support O_BINARY for the open syscall.
   For example, the Demon debug monitor has a separate
   flag value for "rb" vs "r". */
#define _FBINARY        0x10000
#define O_BINARY        _FBINARY

#ifndef FIONREAD
#define FIONREAD        _IOR('f', 127, unsigned long)                   /*  get # bytes to read         */
#endif

#ifndef FIONBIO
#define FIONBIO         _IOW('f', 126, unsigned long)                   /*  set/clear non-blocking i/o  */
#endif

#ifndef SIOCSHIWAT
#define SIOCSHIWAT      _IOW('s', 0, unsigned long)                     /*  set high watermark          */
#define SIOCGHIWAT      _IOR('s', 1, unsigned long)                     /*  get high watermark          */
#define SIOCSLOWAT      _IOW('s', 2, unsigned long)                     /*  set low watermark           */
#define SIOCGLOWAT      _IOR('s', 3, unsigned long)                     /*  get low watermark           */
#define SIOCATMARK      _IOR('s', 7, unsigned long)                     /*  at oob mark?                */
#endif

#ifndef O_NDELAY
#define O_NDELAY        O_NONBLOCK                                      /*  same as O_NONBLOCK          */
#endif

#endif                                                                  /*  FCNTL_H_                    */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
