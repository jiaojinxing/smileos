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
** File name:               s3c2440_sd.c
** Last modified Date:      2012-4-4
** Last Version:            1.0.0
** Descriptions:            S3C2440 SDIO 接口驱动
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-4
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "s3c2440.h"
#include "s3c2440_clock.h"
#include "drv/sdio.h"
#include <unistd.h>
#include <string.h>

#define INIT_CLK        100000
#define SD_CLK          24000000
#define MMC_CLK         15000000

/*
 * 等待命令结束
 */
static int sdio_wait_cmd_end(sdio_t *sdio, uint32_t cmd, int need_resp)
{
    uint32_t status;

    if (!need_resp) {
        do {
            status = SDICSTA;
        } while (!(status & (1 << 11)));

        SDICSTA = status;

        return 0;
    } else {
        do {
            status = SDICSTA;
        } while (!(status & (1 << 11)));

        do {
            status = SDICSTA;
        } while (!(status & (1 << 9)) && !(status & (1 << 10)));

        SDICSTA = status;

        if (status & (1 << 9)) {
            return 0;
        }

        if (status & (1 << 10)) {
            printk("%s: cmd%d timeout\n", __func__, cmd);
            return -1;
        }

        return 0;
    }
}

/*
 * 等待数据结束
 */
static int sdio_wait_data_end(sdio_t *sdio)
{
    uint32_t status;

    status = SDIDSTA;

    while (!(((status & 0x10) == 0x10) | ((status & 0x20) == 0x20))) {
        status = SDIDSTA;
    }

    if ((status & 0xfc) != 0x10) {
        SDIDSTA = 0xec;
        return -1;
    }

    return 0;
}

/*
 * 判断命令是否为终止命令
 */
static inline int sdio_abort_cmd(uint32_t cmd)
{
    return (cmd == 12 || cmd == 52) ? 1 : 0;
}

/*
 * 判断回应类型是否为长回应
 */
static inline int sdio_long_resp(sd_resp_type_t resp_type)
{
    return (resp_type == SD_R2) ? 1 : 0;
}

/*
 * 判断回应类型是否需要等待回应
 */
static inline int sdio_need_resp(sd_resp_type_t resp_type)
{
    return (resp_type != SD_R0) ? 1 : 0;
}

/*
 * 通过 SDIO 接口发送命令
 */
static int sdio_send_cmd(sdio_t *sdio,
                         uint32_t cmd,
                         int has_arg, uint32_t arg,
                         sd_resp_type_t resp_type, sd_resp_t *resp)
{
    /*
        Write command argument 32bit to SDICmdArg.
        Determine command types and start command transmit with setting SDICmdCon.
        Confirm the end of SDI CMD path operation when the specific flag of SDICmdSta is set
        The flag is CmdSent if command type is no response.
        The flag is RspFin if command type is with response.
        Clear the flags of SDICmdSta by writing '1' to the corresponding bit.
     */

    SDIDTIMER = 0x7FFFFF;                                               /*  Set Timeout count           */

    SDICARG = arg;                                                      /*  Command argument            */

    SDICCON = (sdio_abort_cmd(cmd) << 12) |                             /*  command type                */
              (has_arg ? (1 << 11) : (0 << 11)) |                       /*  With data ?                 */
              (sdio_long_resp(resp_type) << 10) |                       /*  Long response ?             */
              (sdio_need_resp(resp_type) <<  9) |                       /*  Wait response ?             */
              (1 << 8) |                                                /*  Command start               */
              (cmd | 1 << 6);                                           /*  Command index               */

    if (sdio_wait_cmd_end(sdio, cmd, resp_type) == -1) {                /*  Wait Command end            */
        return -1;
    }

    if (sdio_need_resp(resp_type)) {
        if (sdio_long_resp(resp_type)) {
            resp->resp32[0] = SDIRSP0;
            resp->resp32[1] = SDIRSP1;
            resp->resp32[2] = SDIRSP2;
            resp->resp32[3] = SDIRSP3;
        } else {
            resp->resp32[0] = SDIRSP0;
        }
    }

    return 0;
}

/*
 * 初始化 SDIO 接口
 */
static int sdio_init(sdio_t *sdio)
{
    CLKCON   |= 1 << 9;                                                 /*  PCLK into SDIO enable       */

    GPEUP     = (GPEUP & (~(0x3f << 5))) | (0x01 << 5);
    GPECON    = (GPECON & (~(0xfff << 10))) | (0xaaa << 10);

    SDIPRE    = PCLK / (INIT_CLK) - 1;                                  /*  400KHz                      */

    SDICON    = (0 << 8) |                                              /*  SDMMC reset                 */
                (0 << 5) |                                              /*  Clock type: SD type         */
                (1 << 4) |                                              /*  Byte order type: Type B     */
                (0 << 3) |                                              /*  Ignore SDIO Interrupt       */
                (1 << 2) |                                              /*  Read wait enable            */
                (1 << 0);                                               /*  SDCLK Out enable            */

    SDIFSTA  |= (1 << 16);                                              /*  FIFO reset                  */

    SDIBSIZE  = 512;                                                    /*  512Byte                     */

    SDIDTIMER = 0x7FFFFF;                                               /*  Set Timeout count           */

    return 0;
}

/*
 * 设置 SDIO 接口的输出时钟为最大
 */
static int sdio_max_clk(sdio_t *sdio)
{
    SDIPRE = (PCLK / SD_CLK) - 1;

    return 0;
}

/*
 * 通过 SDIO 接口预读块
 */
static int sdio_preread_blk(sdio_t *sdio, uint32_t blk_cnt)
{
    SDIFSTA |= (1 << 16);                               /*  FIFO reset                                  */

    SDIDCON  = (0 << 24) |                              /*  Disable burst4 mode in DMA mode             */
               (2 << 22) |                              /*  The size of the transfer with FIFO: Word    */
               (0 << 21) |                              /*  SDIO Interrupt period is 2 cycle            */
               (0 << 20) |                              /*  Data transmit start after data mode set     */
               (1 << 19) |                              /*  Data receive start after command sent       */
               (0 << 18) |                              /*  Busy receive start after data mode set      */
               (1 << 17) |                              /*  Block data transfer mode                    */
               (1 << 16) |                              /*  Wide bus mode enable                        */
               (0 << 15) |                              /*  DMA mode disable                            */
               (1 << 14) |                              /*  Data transfer start                         */
               (2 << 12) |                              /*  Data receive mode                           */
               (blk_cnt << 0);                          /*  Block Number                                */

    return 0;
}

/*
 * 通过 SDIO 接口读块
 */
static int sdio_read_blk(sdio_t *sdio, uint32_t len, uint8_t *buf)
{
    uint32_t status;
    uint32_t temp;
    uint32_t read_cnt;

    read_cnt = 0;

    while (read_cnt < len / 4) {
        if ((SDIDSTA & 0x20) == 0x20) {
            SDIDSTA = (0x1 << 0x5);
            break;
        }

        status = SDIFSTA;
        if ((status & 0x1000) == 0x1000) {
            temp = SDIDAT;
            memcpy(buf, &temp, sizeof(uint32_t));
            read_cnt++;
            buf += 4;
        }
    }

    if (sdio_wait_data_end(sdio) < 0) {
        return -1;
    }

    SDIDCON = SDIDCON & ~(7 << 12);
    SDIFSTA = SDIFSTA & 0x200;
    SDIDSTA = 0x10;

    return 0;
}

/*
 * 通过 SDIO 接口预写块
 */
static int sdio_prewrite_blk(sdio_t *sdio, uint32_t blk_cnt)
{
    SDIFSTA |= (1 << 16);                               /*  FIFO reset                                  */

    SDIDCON  = (0 << 24) |                              /*  Disable burst4 mode in DMA mode             */
               (2 << 22) |                              /*  The size of the transfer with FIFO: Word    */
               (0 << 21) |                              /*  SDIO Interrupt period is 2 cycle            */
               (1 << 20) |                              /*  Data transmit start after data mode set     */
               (0 << 19) |                              /*  Data receive start after command sent       */
               (0 << 18) |                              /*  Busy receive start after data mode set      */
               (1 << 17) |                              /*  Block data transfer mode                    */
               (1 << 16) |                              /*  Wide bus mode enable                        */
               (0 << 15) |                              /*  DMA mode disable                            */
               (1 << 14) |                              /*  Data transfer start                         */
               (3 << 12) |                              /*  Data transmit mode                          */
               (blk_cnt << 0);                          /*  Block Number                                */

    return 0;
}

/*
 * 通过 SDIO 接口写块
 */
static int sdio_write_blk(sdio_t *sdio, uint32_t len, const uint8_t *buf)
{
    uint32_t status;
    uint32_t temp;
    uint32_t write_cnt;

    write_cnt = 0;

    while (write_cnt < len / 4) {
        status = SDIFSTA;
        if ((status & 0x2000) == 0x2000) {
            memcpy(&temp, buf, sizeof(uint32_t));
            SDIDAT = temp;
            write_cnt++;
            buf += 4;
        }
    }

    if (sdio_wait_data_end(sdio) < 0) {
        return -1;
    }

    SDIDCON = SDIDCON & ~(7 << 12);
    SDIDSTA = 0x10;

    return 0;
}

/*
 * S3C2440 SDIO 接口驱动
 */
sdio_t s3c2440_sdio = {
        .init           = sdio_init,
        .send_cmd       = sdio_send_cmd,
        .max_clk        = sdio_max_clk,
        .preread_blk    = sdio_preread_blk,
        .read_blk       = sdio_read_blk,
        .prewrite_blk   = sdio_prewrite_blk,
        .write_blk      = sdio_write_blk,
};
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
