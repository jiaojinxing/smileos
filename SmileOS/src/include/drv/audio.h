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
** File name:               audio.h
** Last modified Date:      2012-8-28
** Last Version:            1.0.0
** Descriptions:            audio 头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-8-28
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
#ifndef AUDIO_H_
#define AUDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/ioctl.h>

#define AUDIO_SAMPLE_RATE           _IO('a', 'r')
#define AUDIO_SAMPLE_BITS           _IO('a', 'b')
#define AUDIO_CHANNELS_NR           _IO('a', 'c')

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  AUDIO_H_                    */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
