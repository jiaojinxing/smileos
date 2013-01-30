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
** File name:               fb.h
** Last modified Date:      2012-6-14
** Last Version:            1.0.0
** Descriptions:            FrameBuffer ͷ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-6-14
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
#ifndef FB_H_
#define FB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/ioctl.h>

/*
 * ioctl ����
 */
#define FBIOGET_VSCREENINFO      _IO('f', 'v')
#define FBIOGET_FSCREENINFO      _IO('f', 's')
#define FBIOPAN_DISPLAY          _IO('f', 'd')
#define FBIOSET_VSCREENINFO      _IO('f', 'm')

/*
 * �ɱ���Ļ��Ϣ
 */
struct fb_var_screeninfo {
    int     xoffset;
    int     yoffset;
    int     xres;
    int     yres;
    int     xres_virtual;
    int     yres_virtual;
    int     bits_per_pixel;
    struct {
        int offset;
        int length;
    }       red;
    struct {
        int offset;
        int length;
    }       green;
    struct {
        int offset;
        int length;
    }       blue;
};

/*
 * �̶���Ļ��Ϣ
 */
struct fb_fix_screeninfo {
    void   *smem_start;
    int     smem_len;
    int     xpanstep;
    int     ypanstep;
    int     ywrapstep;
};

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  FB_H_                       */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
