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
** File name:               s3c2440_nand.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            S3C2440 NAND 接口驱动
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include "s3c2440.h"
/*********************************************************************************************************
** 寄存器定义
*********************************************************************************************************/
#define NF_BASE             (0x4E000000)
#define rNFCONF             (*(volatile unsigned *)0x4E000000)          /*  NAND Flash configuration    */
#define rNFCONT             (*(volatile unsigned *)0x4E000004)          /*  NAND Flash control          */
#define rNFCMD              (*(volatile unsigned *)0x4E000008)          /*  NAND Flash command          */
#define rNFADDR             (*(volatile unsigned *)0x4E00000C)          /*  NAND Flash address          */
#define rNFDATA             (*(volatile unsigned *)0x4E000010)          /*  NAND Flash data             */
#define rNFDATA8            (*(volatile unsigned char *)0x4E000010)     /*  NAND Flash data             */
#undef  NFDATA
#define NFDATA              (0x4E000010)                                /*  NAND Flash data address     */
#define rNFMECCD0           (*(volatile unsigned *)0x4E000014)          /*  NAND Flash ECC for Main Area*/
#define rNFMECCD1           (*(volatile unsigned *)0x4E000018)
#define rNFSECCD            (*(volatile unsigned *)0x4E00001C)          /*  NAND Flash ECC for Spare    */
                                                                        /*  Area                        */
#define rNFSTAT             (*(volatile unsigned *)0x4E000020)          /*  NAND Flash operation status */
#define rNFESTAT0           (*(volatile unsigned *)0x4E000024)
#define rNFESTAT1           (*(volatile unsigned *)0x4E000028)
#define rNFMECC0            (*(volatile unsigned *)0x4E00002C)
#define rNFMECC1            (*(volatile unsigned *)0x4E000030)
#define rNFSECC             (*(volatile unsigned *)0x4E000034)
#define rNFSBLK             (*(volatile unsigned *)0x4E000038)          /*  NAND Flash Start block      */
                                                                        /*  address                     */
#define rNFEBLK             (*(volatile unsigned *)0x4E00003C)          /*  NAND Flash End block address*/
/*********************************************************************************************************
** NAND FLASH 芯片时间参数定义
*********************************************************************************************************/
#define NAND_TACLS          2
#define NAND_TWRPH0         4
#define NAND_TWRPH1         2
/*********************************************************************************************************
** Function name:           s3c2440_hwcontrol
** Descriptions:            硬件控制
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void s3c2440_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
    static ulong IO_ADDR_W = NF_BASE;

#define S3C2440_NFCONT_nCE  (1 << 1)                                    /*  chip select bit             */
#define S3C2440_ADDR_NALE   (0x0c)                                      /*  reg offset                  */
#define S3C2440_ADDR_NCLE   (0x08)                                      /*  reg offset                  */

    if (ctrl & NAND_CTRL_CHANGE) {

        IO_ADDR_W = NF_BASE;

        if (!(ctrl & NAND_CLE)){
            IO_ADDR_W |= S3C2440_ADDR_NALE;
        }

        if (!(ctrl & NAND_ALE)) {
            IO_ADDR_W |= S3C2440_ADDR_NCLE;
        }

        if (ctrl & NAND_NCE) {
            rNFCONT &= ~S3C2440_NFCONT_nCE;
        } else {
            rNFCONT |= S3C2440_NFCONT_nCE;
        }
    }
    
    if (cmd != NAND_CMD_NONE){
        writeb(cmd, (void *)IO_ADDR_W);
    }
}
/*********************************************************************************************************
** Function name:           s3c2440_dev_ready
** Descriptions:            询问设备是否就绪
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int s3c2440_dev_ready(struct mtd_info *mtd)
{
    return (rNFSTAT & 0x01);
}
/*********************************************************************************************************
** Function name:           board_nand_init
** Descriptions:            目标板 NAND FLASH 接口和芯片结构初始化
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
int board_nand_init(struct nand_chip *nand)
{
    GPACON = (GPACON & ~(0x3f << 17)) | (0x3f << 17);

    rNFCONF = (NAND_TACLS  << 12)                                       /*  HCLK x (TACLS)              */
            | (NAND_TWRPH0 <<  8)                                       /*  HCLK x (TWRPH0 + 1)         */
            | (NAND_TWRPH1 <<  4);                                      /*  HCLK x (TWRPH1 + 1)         */

    rNFCONT = (0 << 13)                                                 /*  Disable lock-tight          */
            | (0 << 12)                                                 /*  Disable Soft lock           */
            | (0 << 10)                                                 /*  Disable interrupt           */
            | (0 <<  9)                                                 /*  Disable RnB interrupt       */
            | (0 <<  8)                                                 /*  RnB Detect rising edge      */
            | (1 <<  6)                                                 /*  Lock spare ECC              */
            | (1 <<  5)                                                 /*  Lock main data area ECC     */
            | (1 <<  4)                                                 /*  Initialize ECC de/encoder   */
            | (1 <<  1)                                                 /*  Force nFCE to high          */
                                                                        /*  (Disable chip select)       */
            | (1 <<  0);                                                /*  NAND flash controller enable*/

    rNFSTAT = 0;

    /*
     * initialize nand_chip data structure
     */
    nand->IO_ADDR_R = nand->IO_ADDR_W = (void *)NFDATA;

    nand->ecc.mode  = NAND_ECC_SOFT;

    /*
     *  read_buf and write_buf are default
     *  read_byte and write_byte are default
     *  but cmd_ctrl and dev_ready always must be implemented
     */
    nand->cmd_ctrl  = s3c2440_hwcontrol;
    nand->dev_ready = s3c2440_dev_ready;
    
    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
