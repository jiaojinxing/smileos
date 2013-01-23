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
** File name:               syslimits.h
** Last modified Date:      2012-4-11
** Last Version:            1.0.0
** Descriptions:            ϵͳ����ͷ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-11
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
#ifndef SYSLIMITS_H_
#define SYSLIMITS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KB
#define KB                          (1024)
#define MB                          (1024 * KB)
#define GB                          (1024 * MB)
#endif

#define SECTION_SIZE                (1 * MB)                            /*  �δ�С                      */
#define SECTION_OFFSET              (20)                                /*  �δ�Сƫ��                  */
#define SECTION_NR                  (4096)                              /*  ����                        */

#define PAGE_SIZE                   (4096)                              /*  ҳ��Ĵ�С                  */
#define PAGE_OFFSET                 (12)                                /*  ҳ���С��ƫ��              */

#define MAX_INPUT                   (256)                               /*  �������󳤶�              */

#define LINE_MAX                    (256)                               /*  �е���󳤶�                */

#undef  ARG_MAX
#define ARG_MAX                     (32)                                /*  �������ĸ���              */

#define OPEN_MAX                    (20)                                /*  ����ܴ򿪵��ļ���          */

#define NAME_MAX                    (64)                                /*  ���ֵ���󳤶�              */

#undef  PATH_MAX
#define PATH_MAX                    (256)                               /*  ·������󳤶�              */

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  SYSLIMITS_H_                */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
