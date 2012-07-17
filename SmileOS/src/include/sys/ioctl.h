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
** File name:               ioctl.h
** Last modified Date:      2012-4-10
** Last Version:            1.0.0
** Descriptions:            IO 控制头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-10
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
#ifndef IOCTL_H_
#define IOCTL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define IOCPARM_MASK    0x7fU                                           /*  parameters must be < 128 bytes*/
#define IOC_VOID        0x20000000UL                                    /*  no parameters               */
#define IOC_OUT         0x40000000UL                                    /*  copy out parameters         */
#define IOC_IN          0x80000000UL                                    /*  copy in parameters          */
#define IOC_INOUT       (IOC_IN | IOC_OUT)
                                                                        /*  0x20000000 distinguishes new &
                                                                            old ioctl's                 */
#define _IO(x, y)       (IOC_VOID | ((x) << 8) | (y))

#define _IOR(x, y, t)   (IOC_OUT | (((unsigned long)sizeof(t) & IOCPARM_MASK) << 16) | ((x) << 8) | (y))

#define _IOW(x, y, t)   (IOC_IN  | (((unsigned long)sizeof(t) & IOCPARM_MASK) << 16) | ((x) << 8) | (y))

int ioctl(int fd, int cmd, ...);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  IOCTL_H_                    */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
