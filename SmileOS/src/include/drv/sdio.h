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
** File name:               sd.h
** Last modified Date:      2012-6-29
** Last Version:            1.0.0
** Descriptions:            SDIO 接口
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-6-29
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
#ifndef SDIO_H_
#define SDIO_H_

#include "kern/types.h"

/*
 * 回应
 */
typedef union {
    uint8_t         resp8[16];
    uint32_t        resp32[4];
    uint16_t        resp16[8];
} sd_resp_t;

/*
 * 回应类型
 */
typedef enum {
    SD_R0 = 0,
    SD_R1,                                                              /*  normal response command     */
    SD_R1b,
    SD_R2,                                                              /*  CID, CSD register           */
    SD_R3,                                                              /*  OCR register                */
    SD_R6,                                                              /*  Published RCA response      */
    SD_R7,                                                              /*  Card interface condition    */
} sd_resp_type_t;

/*
 * sdio 接口
 */
struct _sdio_t;
typedef struct _sdio_t sdio_t;

struct _sdio_t {
    int (*send_cmd)(sdio_t *sdio,
                    uint32_t cmd,
                    int has_arg, uint32_t arg,
                    sd_resp_type_t resp_type, sd_resp_t *resp);
    int (*init)(sdio_t *sdio);
    int (*max_clk)(sdio_t *sdio);
    int (*preread_blk)(sdio_t *sdio, uint32_t blk_cnt);
    int (*read_blk)(sdio_t *sdio, uint32_t len, uint8_t *buf);
    int (*prewrite_blk)(sdio_t *sdio, uint32_t blk_cnt);
    int (*write_blk)(sdio_t *sdio, uint32_t len, const uint8_t *buf);
};

#endif                                                                  /*  SDIO_H_                     */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
