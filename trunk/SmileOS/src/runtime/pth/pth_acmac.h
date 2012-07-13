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
** File name:               pth_acmac.h
** Last modified Date:      2012-4-27
** Last Version:            1.0.0
** Descriptions:            GNU/pth 库机器相关头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-27
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
#ifndef _PTH_ACMAC_H_
#define _PTH_ACMAC_H_

/* stack setup macros */
#define pth_skaddr(func,skaddr,sksize) pth_skaddr_##func(skaddr,sksize)
#define pth_sksize(func,skaddr,sksize) pth_sksize_##func(skaddr,sksize)

/* mctx compile defines */
#define PTH_MCTX_MTH(which)  (PTH_MCTX_MTH_use == (PTH_MCTX_MTH_##which))
#define PTH_MCTX_DSP(which)  (PTH_MCTX_DSP_use == (PTH_MCTX_DSP_##which))
#define PTH_MCTX_STK(which)  (PTH_MCTX_STK_use == (PTH_MCTX_STK_##which))
#define PTH_MCTX_MTH_mcsc    1
#define PTH_MCTX_MTH_sjlj    2
#define PTH_MCTX_DSP_sc      1
#define PTH_MCTX_DSP_ssjlj   2
#define PTH_MCTX_DSP_sjlj    3
#define PTH_MCTX_DSP_usjlj   4
#define PTH_MCTX_DSP_sjlje   5
#define PTH_MCTX_DSP_sjljlx  6
#define PTH_MCTX_DSP_sjljisc 7
#define PTH_MCTX_DSP_sjljw32 8
#define PTH_MCTX_STK_mc      1
#define PTH_MCTX_STK_ss      2
#define PTH_MCTX_STK_sas     3
#define PTH_MCTX_STK_none    4

#endif                                                                  /*  _PTH_ACMAC_H_               */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
