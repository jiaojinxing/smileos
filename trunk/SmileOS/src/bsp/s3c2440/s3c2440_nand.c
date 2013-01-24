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
** File name:               s3c2440_k9f1208.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            K9F1208
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
#include "kern/kern.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/utils.h"
#include <errno.h>
#include <fcntl.h>
/*********************************************************************************************************
** NAND 芯片尺寸定义
*********************************************************************************************************/
#define NAND_BYTES_PER_SECT         512                                 /*  每扇区 512 字节             */
#define NAND_SPARE_PER_SECT         16                                  /*  每扇区 16 字节扩扇数据      */
#define NAND_SECTS_PER_BLOCK        32                                  /*  每块 32 个扇区           	*/
/*********************************************************************************************************
** 控制参数配置
*********************************************************************************************************/
#define NAND_TACLS                  2                                   /*  CLE & ALE 持续时间(Min:10ns)*/
                                                                        /*  CLE Hold Time tCLH          */
#define NAND_TWRPH0                 4                                   /*  nWE 脉冲宽度(Min:25ns)      */
                                                                        /*  nWE Pulse Width tWP         */
#define NAND_TWRPH1                 2                                   /*  nWE 高电平保持时间(Min:15ns)*/
                                                                        /*  nWE High Hold Time tWH      */
/*********************************************************************************************************
** 硬件操作控制
*********************************************************************************************************/
#define BIT_CLR(addr, bit)			addr &= ~(1 << bit)
#define BIT_SET(addr, bit)			addr |=  (1 << bit)

#define rNFCONF		                (*(volatile unsigned *)0x4E000000)  //  NAND Flash configuration
#define rNFCONT		                (*(volatile unsigned *)0x4E000004)  //  NAND Flash control
#define rNFCMD		                (*(volatile unsigned *)0x4E000008)  //  NAND Flash command
#define rNFADDR		                (*(volatile unsigned *)0x4E00000C)  //  NAND Flash address
#define rNFDATA		                (*(volatile unsigned *)0x4E000010)  //  NAND Flash data
#define rNFDATA8	                (*(volatile unsigned char *)0x4E000010) //  NAND Flash data
#undef  NFDATA
#define NFDATA		                (0x4E000010)      					//  NAND Flash data address
#define rNFMECCD0	                (*(volatile unsigned *)0x4E000014)  //  NAND Flash ECC for Main Area
#define rNFMECCD1	                (*(volatile unsigned *)0x4E000018)
#define rNFSECCD	                (*(volatile unsigned *)0x4E00001C)  //  NAND Flash ECC for Spare Area
#define rNFSTAT		                (*(volatile unsigned *)0x4E000020)  //  NAND Flash operation status
#define rNFESTAT0	                (*(volatile unsigned *)0x4E000024)
#define rNFESTAT1	                (*(volatile unsigned *)0x4E000028)
#define rNFMECC0	                (*(volatile unsigned *)0x4E00002C)
#define rNFMECC1	                (*(volatile unsigned *)0x4E000030)
#define rNFSECC		                (*(volatile unsigned *)0x4E000034)
#define rNFSBLK		                (*(volatile unsigned *)0x4E000038)  //  NAND Flash Start block address
#define rNFEBLK		                (*(volatile unsigned *)0x4E00003C)  //  NAND Flash End block address
/*********************************************************************************************************
** 硬件 NAND FLASH 操作宏
*********************************************************************************************************/
#define NF_CMD(cmd)                 { rNFCMD  = cmd; }
#define NF_ADDR(addr)               { rNFADDR = addr; }
#define NF_NFCE_L()                 BIT_CLR(rNFCONT, 1)                 /*  { rNFCONT &= ~(1 << 1); }   */
#define NF_NFCE_H()                 BIT_SET(rNFCONT, 1)                 /*  { rNFCONT |=  (1 << 1); }   */
/*********************************************************************************************************
** 硬件 NAND FLASH 读写宏
*********************************************************************************************************/
#define NF_RDDATA()                 (rNFDATA)
#define NF_RDDATA8()                ((*(volatile unsigned char*)0x4E000010))
#define NF_WRDATA(data)             { rNFDATA  = data; }
#define NF_WRDATA8(data)            { rNFDATA8 = data; }
/*********************************************************************************************************
** 硬件 NAND FLASH RnB 信号判断
*********************************************************************************************************/
#define NF_CLEAR_RB()               BIT_SET(rNFSTAT, 2)                 /*  写入 1 清除此位             */
#define NF_DETECT_RB()              do {                                        \
                                            while (!(rNFSTAT & (1 << 2))) {     \
                                            }                                   \
                                    } while (0)
/*********************************************************************************************************
** 命令定义
*********************************************************************************************************/
#define NAND_PAGE_READ_1            0x00                                /*  页读命令序列1               */
#define NAND_READ_ID                0x90                                /*  读芯片ID命令                */
#define NAND_RESET                  0xff                                /*  芯片复位命令                */
#define NAND_READ_STATUS            0x70                                /*  读芯片状态命令              */
#define NAND_PAGE_PROGRAM_1         0x80                                /*  页编程命令序列1             */
#define NAND_PAGE_PROGRAM_2         0x10                                /*  页编程命令序列2             */
#define NAND_BLOCK_ERASE_1          0x60                                /*  块擦除命令序列1             */
#define NAND_BLOCK_ERASE_2          0xd0                                /*  块擦除命令序列2             */
/*********************************************************************************************************
** 状态定义
*********************************************************************************************************/
#define NAND_FALG_BUSY              (1 << 6)                            /*  忙标志                      */
#define NAND_FALG_PE_OK             (1 << 0)                            /*  结果标志                    */

#define ERROR_NONE 				    0
#define PX_ERROR   					-1
/*********************************************************************************************************
** Function name:           nand_status
** Descriptions:            获得 NAND FLASH 芯片状态
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NAND FLASH 芯片状态
*********************************************************************************************************/
static unsigned char nand_status (void)
{
    unsigned char  status = 0xFF;

    NF_NFCE_L();                                                        /*  使能芯片片选              	*/
    NF_CMD(NAND_READ_STATUS);                                           /*  发送读取状态命令            */
    status = NF_RDDATA8();                                              /*  读取状态                    */
    NF_NFCE_H();                                                        /*  释放新片片选                */

    return  (status);
}
/*********************************************************************************************************
** Function name:           nand_reset
** Descriptions:            复位 NAND FLASH 芯片
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void  nand_reset (void)
{
    NF_NFCE_L();                                                        /*  使能芯片片选             	*/
    NF_CLEAR_RB();                                                      /*  清除忙闲信号                */
    NF_CMD(NAND_RESET);                                                 /*  发送复位命令                */
    NF_NFCE_H();                                                        /*  释放新片片选                */
}
/*********************************************************************************************************
** Function name:           nand_is_ok
** Descriptions:            判断芯片操作是否完成 chip Erase or Program OK?
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0                   完成   OK
**                          -1                  未完成 Not OK
*********************************************************************************************************/
static int  nand_is_ok (void)
{
    unsigned char  status;

    status = nand_status();
    if ((status & NAND_FALG_BUSY) != 0) {
        if ((status & NAND_FALG_PE_OK) == 0) {
            return  (ERROR_NONE);
        }
    }
    return  (PX_ERROR);
}
/*********************************************************************************************************
** Function name:           nand_read_id
** Descriptions:            读芯片 ID
** input parameters:        NONE
** output parameters:       id                  保存读到的ID
** Returned value:          NONE
*********************************************************************************************************/
static void nand_read_id (uint16_t *id)
{
    NF_NFCE_L();                                                        /*  片选               			*/

    NF_CMD(NAND_READ_ID);
    NF_ADDR(0);

    *id  = NF_RDDATA8();
    *id += (uint16_t)NF_RDDATA8() << 8;

    NF_NFCE_H();
}
/*********************************************************************************************************
** Function name:           nand_port_init
** Descriptions:            初始化 NAND FLASH 端口
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void  nand_port_init (void)
{
    static int  iIsInit = 0;

    if (iIsInit) {                                                      /*  避免重复初始化    			*/
        return;
    }

    iIsInit = 1;

    rNFCONF = (NAND_TACLS << 12)                                        /*  HCLK x (TACLS)              */
            | (NAND_TWRPH0 << 8)                                        /*  HCLK x (TWRPH0 + 1)         */
            | (NAND_TWRPH1 << 4);                                       /*  HCLK x (TWRPH1 + 1)         */

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
}
/*********************************************************************************************************
** Function name:           nand_read
** Descriptions:            读一个扇区的数据, 包含扩展区域
** input parameters:        pvSecBuf            保存扇区数据
**                          pvSeccDataBuf       保存扇区附加数据
**                          ulSecIndex          扇区索引
** output parameters:       NONE
** Returned value:          0                   成功
**                          -1                  未成功
*********************************************************************************************************/
static int  nand_read (void                  *pvSecBuf,
                       void                  *pvSeccDataBuf,
                       uint64_t               ulSecIndex)
{
             int    i;
    unsigned char  *pucBuffer      = (unsigned char *)pvSecBuf;
    unsigned char  *pucBufferSpare = (unsigned char *)pvSeccDataBuf;

    NF_NFCE_L();                                                        /*  片选                        */
    NF_CLEAR_RB();
    NF_CMD(NAND_PAGE_READ_1);                                           /*  准备读                      */

    /*
     * 写入地址
     */
    NF_ADDR((uint8_t)(0));
    NF_ADDR((uint8_t)(ulSecIndex));
    NF_ADDR((uint8_t)(ulSecIndex >> 8));
    NF_ADDR((uint8_t)(ulSecIndex >> 16));

    NF_DETECT_RB();                                                     /*  等待空闲                  	*/

    for (i = 0; i < NAND_BYTES_PER_SECT; i++) {
        *pucBuffer++ = NF_RDDATA8();                                    /*  读取数据                    */
    }

    for (i = 0; i < NAND_SPARE_PER_SECT; i++) {
        *pucBufferSpare++ = NF_RDDATA8();                               /*  读取数据                    */
    }
    NF_NFCE_H();                                                        /*  释放新片片选               	*/

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           nand_write
** Descriptions:            写入一个扇区的数据, 包含扩展区域
** input parameters:        pvSecBuf            保存扇区数据
**                          pvSeccDataBuf       保存扇区附加数据
**                          ulSecIndex          扇区索引
** output parameters:       NONE
** Returned value:          0                   完成
**                          -1                  未完成
*********************************************************************************************************/
static int  nand_write (const void            *pvSecBuf,
                        const void            *pvSeccDataBuf,
                        uint64_t               ulSecIndex)
{
             int    i;
    unsigned char  *pucBuffer      = (unsigned char *)pvSecBuf;
    unsigned char  *pucBufferSpare = (unsigned char *)pvSeccDataBuf;

    NF_NFCE_L();                                                        /*  片选                     	*/

    NF_CMD(0);                                    					    /*  准备编程                    */
    NF_CMD(NAND_PAGE_PROGRAM_1);                                        /*  准备编程                    */

    NF_ADDR((uint8_t)(0));
    NF_ADDR((uint8_t)(ulSecIndex));
    NF_ADDR((uint8_t)(ulSecIndex >> 8));
    NF_ADDR((uint8_t)(ulSecIndex >> 16));

    for (i = 0; i < NAND_BYTES_PER_SECT; i++) {                         /*  写入数据                    */
        NF_WRDATA8(*pucBuffer);
        pucBuffer++;
    }

    for (i = 0; i < NAND_SPARE_PER_SECT; i++) {                         /*  写入扩展数据                */
        NF_WRDATA8(*pucBufferSpare);
        pucBufferSpare++;
    }

    NF_CLEAR_RB();

    NF_CMD(NAND_PAGE_PROGRAM_2);                                        /*  准备编程                    */

    NF_DETECT_RB();

    return  (nand_is_ok());
}
/*********************************************************************************************************
** Function name:           nand_erase
** Descriptions:            擦除芯片指定的块
** input parameters:        ulBlockIndex        块索引
** output parameters:       NONE
** Returned value:          0                   完成
**                          -1                  未完成
*********************************************************************************************************/
static int  nand_erase (uint64_t  ulBlockIndex)
{
    uint64_t ulSecIndex = ulBlockIndex * 32;

    NF_NFCE_L();                                                        /*  片选               			*/

    NF_CMD(NAND_BLOCK_ERASE_1);                                         /*  准备擦除                 	*/

    NF_ADDR((uint8_t)(ulSecIndex));
    NF_ADDR((uint8_t)(ulSecIndex >> 8));
    NF_ADDR((uint8_t)(ulSecIndex >> 16));

    NF_CLEAR_RB();

    NF_CMD(NAND_BLOCK_ERASE_2);                                         /*  开始编程                    */

    NF_DETECT_RB();

    return  (nand_is_ok());
}
/*********************************************************************************************************
** Function name:           nand_chip_init
** Descriptions:            初始化 NAND FLASH 芯片
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0                   成功
**                          -1                  未成功
*********************************************************************************************************/
static int  nand_chip_init (void)
{
    uint16_t id;

    nand_port_init();                                                   /*  初始化端口             	    */
    nand_reset();                                                       /*  复位 NAND FLASH 芯片        */
    nand_read_id(&id);                                           		/*  读取芯片 ID 号              */

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 私有信息
*********************************************************************************************************/
typedef struct {
    VFS_DEVICE_MEMBERS;
    int totalBytesPerChunk;         /* Should be >= 512, does not need to be a power of 2 */
    int nChunksPerBlock;            /* does not need to be a power of 2 */
    int spareBytesPerChunk;         /* spare area size */
    int startBlock;                 /* Start block we're allowed to use */
    int endBlock;                   /* End block we're allowed to use */
    int nReservedBlocks;            /* We want this tuneable so that we can reduce */
                                    /* reserved blocks on NOR and RAM. */
    int nShortOpCaches;             /* If <= 0, then short op caching is disabled, else
                                     * the number of short op caches (don't use too many).
                                     * 10 to 20 is a good bet.
                                     */
    int useNANDECC;                 /* Flag to decide whether or not to use NANDECC on data (yaffs1) */
    int noTagsECC;                  /* Flag to decide whether or not to do ECC on packed tags (yaffs2) */
    const char *name;
} privinfo_t;
/*********************************************************************************************************
** 设备分区信息
*********************************************************************************************************/
typedef struct {
    int totalBytesPerChunk;         /* Should be >= 512, does not need to be a power of 2 */
    int nChunksPerBlock;            /* does not need to be a power of 2 */
    int spareBytesPerChunk;         /* spare area size */
    int startBlock;                 /* Start block we're allowed to use */
    int endBlock;                   /* End block we're allowed to use */
    int nReservedBlocks;            /* We want this tuneable so that we can reduce */
                                    /* reserved blocks on NOR and RAM. */
    int nShortOpCaches;             /* If <= 0, then short op caching is disabled, else
                                     * the number of short op caches (don't use too many).
                                     * 10 to 20 is a good bet.
                                     */
    int useNANDECC;                 /* Flag to decide whether or not to use NANDECC on data (yaffs1) */
    int noTagsECC;                  /* Flag to decide whether or not to do ECC on packed tags (yaffs2) */
    const char *name;
} yaffs_Info;
/*********************************************************************************************************
** 读写参数
*********************************************************************************************************/
typedef struct {
    void       *data;
    void       *spare;
} yaffs_RwParam;

/*
 * 打开 mtdblock
 */
static int mtdblock_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_inc_return(&(((device_t *)file->ctx)->ref)) == 1) {
        /*
         * 第一次打开时的初始化代码
         */
        if (nand_chip_init() != 0) {
            seterrno(EIO);
            return -1;
        } else {
            return 0;
        }
    } else {
        /*
         * 如果设备不允许同时打开多次, 请使用如下代码:
         */
        atomic_dec(&(((device_t *)file->ctx)->ref));
        seterrno(EBUSY);
        return -1;
    }
}

/*
 * 关闭 mtdblock
 */
static int mtdblock_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&(((device_t *)file->ctx)->ref)) == 1) {
        /*
         * 加上最后一次关闭时的清理代码
         */
    }
    atomic_dec(&(((device_t *)file->ctx)->ref));
    return 0;
}

/*
 * 控制 mtdblock
 */
static int mtdblock_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;
    yaffs_Info *param;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    arg = va_to_mva(arg);

    switch (cmd) {
    case BLKDEV_CMD_STATUS:
        *(uint32_t *)arg = 0;
        break;

    case BLKDEV_CMD_INIT:
        printk("mtdblock: init\n");
        if (nand_chip_init() != 0) {
            seterrno(EIO);
            return -1;
        }
        break;

    case BLKDEV_CMD_SYNC:
        printk("mtdblock: sync\n");
        break;

    case BLKDEV_CMD_SECT_NR:
        *(uint32_t *)arg = priv->startBlock - priv->endBlock + 1;
        break;

    case BLKDEV_CMD_SECT_SZ:
        *(uint32_t *)arg = priv->totalBytesPerChunk + priv->spareBytesPerChunk;
        break;

    case BLKDEV_CMD_BLK_SZ:
        *(uint32_t *)arg = (priv->totalBytesPerChunk + priv->spareBytesPerChunk) * priv->nChunksPerBlock;
        break;

    case BLKDEV_CMD_ERASE:
        if (nand_erase(*(uint32_t *)arg) != 0) {
            seterrno(EIO);
            return -1;
        }
        break;

    case BLKDEV_CMD_INFO:
        param                       = arg;
        param->endBlock             = priv->endBlock;
        param->nChunksPerBlock      = priv->nChunksPerBlock;
        param->nReservedBlocks      = priv->nReservedBlocks;
        param->nShortOpCaches       = priv->nShortOpCaches;
        param->noTagsECC            = priv->noTagsECC;
        param->spareBytesPerChunk   = priv->spareBytesPerChunk;
        param->startBlock           = priv->startBlock;
        param->totalBytesPerChunk   = priv->totalBytesPerChunk;
        param->useNANDECC           = priv->useNANDECC;
        param->name                 = priv->name;
        break;

    default:
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * 读 mtdblock 块
 */
static ssize_t mtdblock_readblk(void *ctx, file_t *file, size_t blk_no, size_t blk_cnt, void *buf)
{
    privinfo_t    *priv  = ctx;
    yaffs_RwParam *param = buf;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    if (nand_read(param->data, param->spare, blk_no) == 0) {
        return blk_cnt * (priv->totalBytesPerChunk + priv->spareBytesPerChunk);
    } else {
        seterrno(EIO);
        return -1;
    }
}

/*
 * 写 mtdblock 块
 */
static ssize_t mtdblock_writeblk(void *ctx, file_t *file, size_t blk_no, size_t blk_cnt, const void *buf)
{
    privinfo_t          *priv  = ctx;
    const yaffs_RwParam *param = buf;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    if (nand_write(param->data, param->spare, blk_no) == 0) {
        return blk_cnt * (priv->totalBytesPerChunk + priv->spareBytesPerChunk);
    } else {
        seterrno(EIO);
        return -1;
    }
}

/*
 * mtdblock 驱动
 */
static driver_t mtdblock_drv = {
        .name     = "mtdblock",
        .open     = mtdblock_open,
        .ioctl    = mtdblock_ioctl,
        .close    = mtdblock_close,
        .readblk  = mtdblock_readblk,
        .writeblk = mtdblock_writeblk,
};
/*********************************************************************************************************
** Function name:           mtdblock_init
** Descriptions:            初始化 mtdblock
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mtdblock_init(void)
{
    privinfo_t *priv;

    driver_install(&mtdblock_drv);

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        device_init(priv);

        priv->startBlock            = 0;
        priv->endBlock              = 63;
        priv->nReservedBlocks       = 0;
        priv->nShortOpCaches        = 0;
        priv->useNANDECC            = 0;
        priv->noTagsECC             = 0;
        priv->nChunksPerBlock       = NAND_SECTS_PER_BLOCK;
        priv->spareBytesPerChunk    = NAND_SPARE_PER_SECT;
        priv->totalBytesPerChunk    = NAND_BYTES_PER_SECT;
        priv->name                  = "boot";

        if (device_create("/dev/mtdblock0", "mtdblock", priv) < 0) {
            kfree(priv);
            return -1;
        }
    }

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        device_init(priv);

        priv->startBlock            = 64;
        priv->endBlock              = 255;
        priv->nReservedBlocks       = 0;
        priv->nShortOpCaches        = 0;
        priv->useNANDECC            = 0;
        priv->noTagsECC             = 0;
        priv->nChunksPerBlock       = NAND_SECTS_PER_BLOCK;
        priv->spareBytesPerChunk    = NAND_SPARE_PER_SECT;
        priv->totalBytesPerChunk    = NAND_BYTES_PER_SECT;
        priv->name                  = "kernel";

        if (device_create("/dev/mtdblock1", "mtdblock", priv) < 0) {
            kfree(priv);
            return -1;
        }
    }

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        device_init(priv);

        priv->startBlock            = 256;
        priv->endBlock              = 4095;
        priv->nReservedBlocks       = 0;
        priv->nShortOpCaches        = 10;
        priv->useNANDECC            = 0;
        priv->noTagsECC             = 0;
        priv->nChunksPerBlock       = NAND_SECTS_PER_BLOCK;
        priv->spareBytesPerChunk    = NAND_SPARE_PER_SECT;
        priv->totalBytesPerChunk    = NAND_BYTES_PER_SECT;
        priv->name                  = "rootfs";

        if (device_create("/dev/mtdblock2", "mtdblock", priv) < 0) {
            kfree(priv);
            return -1;
        }
    }

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
