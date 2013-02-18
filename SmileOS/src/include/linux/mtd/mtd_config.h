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
** File name:               mtd_config.h
** Last modified Date:      2013-2-3
** Last Version:            1.0.0
** Descriptions:            MTD 配置
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
#ifndef MTD_CONFIG_H_
#define MTD_CONFIG_H_

#define CONFIG_SYS_MAX_NAND_DEVICE      1
#define CONFIG_SYS_NAND_BASE            0
#define CONFIG_MTD_DEVICE               1
#define CONFIG_MTD_DEBUG                1
#define CONFIG_SYS_NAND_RESET_CNT       200000
//#define PPCHAMELON_NAND_TIMER_HACK    1
//#define CONFIG_MTD_NAND_VERIFY_WRITE  1
//#define CONFIG_SYS_NAND_ONFI_DETECTION  1
//#define CONFIG_SYS_NAND_QUIET_TEST    1
//#define CONFIG_MTD_PARTITIONS         1
//#define CONFIG_NAND_NDFC              1
//#define CONFIG_MTD_NAND_ECC_SMC       1
//#define CONFIG_NAND_SPL               1
//#define CONFIG_SPL_NAND_SOFTECC       1
#define CONFIG_MTD_NAND_MUSEUM_IDS      1
#define CONFIG_SYS_NAND_BASE_LIST       {0}
//#define CONFIG_SYS_NAND_SELF_INIT     1
//#define CONFIG_SYS_NAND_SELECT_DEVICE 1
#define CONFIG_CMD_MTDPARTS_SPREAD      1
#define CONFIG_MTD_DEBUG_VERBOSE        0

#endif                                                                  /*  MTD_CONFIG_H_               */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
