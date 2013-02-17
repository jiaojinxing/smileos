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
** File name:               nand_config.h
** Last modified Date:      2013-2-3
** Last Version:            1.0.0
** Descriptions:            nand 配置
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-3
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
#ifndef NAND_CONFIG_H_
#define NAND_CONFIG_H_

/*
 * MTD 配置
 */
#define CONFIG_SYS_MAX_NAND_DEVICE      1
#define CONFIG_SYS_NAND_BASE            0
#define CONFIG_MTD_DEVICE               1
#define CONFIG_RELOC_FIXUP_WORKS        1
#define CONFIG_MTD_DEBUG                1
#define CONFIG_MTD_DEBUG_VERBOSE        0

#endif                                                                  /*  NAND_CONFIG_H_              */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
