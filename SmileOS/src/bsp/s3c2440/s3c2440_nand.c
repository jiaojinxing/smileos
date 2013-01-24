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
** Descriptions:            NAND FLASH ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#include "kern/kern.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/utils.h"
#include <errno.h>
#include <fcntl.h>
/*********************************************************************************************************
** NAND оƬ�ߴ綨��
*********************************************************************************************************/
#define NAND_BYTES_PER_SECT         512                                 /*  ÿ���� 512 �ֽ�             */
#define NAND_SPARE_PER_SECT         16                                  /*  ÿ���� 16 �ֽ���������      */
#define NAND_SECTS_PER_BLOCK        32                                  /*  ÿ�� 32 ������           	*/
/*********************************************************************************************************
** ���Ʋ�������
*********************************************************************************************************/
#define NAND_TACLS                  2                                   /*  CLE & ALE ����ʱ��(Min:10ns)*/
                                                                        /*  CLE Hold Time tCLH          */
#define NAND_TWRPH0                 4                                   /*  nWE ������(Min:25ns)      */
                                                                        /*  nWE Pulse Width tWP         */
#define NAND_TWRPH1                 2                                   /*  nWE �ߵ�ƽ����ʱ��(Min:15ns)*/
                                                                        /*  nWE High Hold Time tWH      */
/*********************************************************************************************************
** Ӳ����������
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
** Ӳ�� NAND FLASH ������
*********************************************************************************************************/
#define NF_CMD(cmd)                 { rNFCMD  = cmd; }
#define NF_ADDR(addr)               { rNFADDR = addr; }
#define NF_NFCE_L()                 BIT_CLR(rNFCONT, 1)
#define NF_NFCE_H()                 BIT_SET(rNFCONT, 1)
/*********************************************************************************************************
** Ӳ�� NAND FLASH ��д��
*********************************************************************************************************/
#define NF_RDDATA()                 (rNFDATA)
#define NF_RDDATA8()                (rNFDATA8)
#define NF_WRDATA(data)             { rNFDATA  = data; }
#define NF_WRDATA8(data)            { rNFDATA8 = data; }
/*********************************************************************************************************
** Ӳ�� NAND FLASH RnB �ź��ж�
*********************************************************************************************************/
#define NF_CLEAR_RB()               BIT_SET(rNFSTAT, 2)                 /*  д�� 1 �����λ             */
#define NF_DETECT_RB()              do {                                        \
                                            while (!(rNFSTAT & (1 << 2))) {     \
                                            }                                   \
                                    } while (0)
/*********************************************************************************************************
** �����
*********************************************************************************************************/
#define NAND_PAGE_READ_1            0x00                                /*  ҳ����������1               */
#define NAND_READ_ID                0x90                                /*  ��оƬID����                */
#define NAND_RESET                  0xff                                /*  оƬ��λ����                */
#define NAND_READ_STATUS            0x70                                /*  ��оƬ״̬����              */
#define NAND_PAGE_PROGRAM_1         0x80                                /*  ҳ�����������1             */
#define NAND_PAGE_PROGRAM_2         0x10                                /*  ҳ�����������2             */
#define NAND_BLOCK_ERASE_1          0x60                                /*  �������������1             */
#define NAND_BLOCK_ERASE_2          0xd0                                /*  �������������2             */
/*********************************************************************************************************
** ״̬����
*********************************************************************************************************/
#define NAND_FALG_BUSY              (1 << 6)                            /*  æ��־                      */
#define NAND_FALG_PE_OK             (1 << 0)                            /*  �����־                    */

#define ERROR_NONE 				    0
#define PX_ERROR   					-1
/*********************************************************************************************************
** Function name:           nand_status
** Descriptions:            ��� NAND FLASH оƬ״̬
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NAND FLASH оƬ״̬
*********************************************************************************************************/
static unsigned char nand_status (void)
{
    unsigned char  status = 0xFF;

    NF_NFCE_L();                                                        /*  ʹ��оƬƬѡ              	*/
    NF_CMD(NAND_READ_STATUS);                                           /*  ���Ͷ�ȡ״̬����            */
    status = NF_RDDATA8();                                              /*  ��ȡ״̬                    */
    NF_NFCE_H();                                                        /*  �ͷ���ƬƬѡ                */

    return  (status);
}
/*********************************************************************************************************
** Function name:           nand_reset
** Descriptions:            ��λ NAND FLASH оƬ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void  nand_reset (void)
{
    NF_NFCE_L();                                                        /*  ʹ��оƬƬѡ             	*/
    NF_CLEAR_RB();                                                      /*  ���æ���ź�                */
    NF_CMD(NAND_RESET);                                                 /*  ���͸�λ����                */
    NF_NFCE_H();                                                        /*  �ͷ���ƬƬѡ                */
}
/*********************************************************************************************************
** Function name:           nand_is_ok
** Descriptions:            �ж�оƬ�����Ƿ���� chip Erase or Program OK?
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0                   ���   OK
**                          -1                  δ��� Not OK
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
** Descriptions:            ��оƬ ID
** input parameters:        NONE
** output parameters:       id                  ���������ID
** Returned value:          NONE
*********************************************************************************************************/
static void nand_read_id (uint16_t *id)
{
    NF_NFCE_L();                                                        /*  Ƭѡ               			*/

    NF_CMD(NAND_READ_ID);
    NF_ADDR(0);

    *id  = NF_RDDATA8();
    *id += (uint16_t)NF_RDDATA8() << 8;

    NF_NFCE_H();
}
/*********************************************************************************************************
** Function name:           nand_port_init
** Descriptions:            ��ʼ�� NAND FLASH �˿�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void  nand_port_init (void)
{
    static int  iIsInit = 0;

    if (iIsInit) {                                                      /*  �����ظ���ʼ��    			*/
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
** Descriptions:            ��һ������������, ������չ����
** input parameters:        pvSecBuf            ������������
**                          pvSeccDataBuf       ����������������
**                          ulSecIndex          ��������
** output parameters:       NONE
** Returned value:          0                   �ɹ�
**                          -1                  δ�ɹ�
*********************************************************************************************************/
static int  nand_read (void                  *pvSecBuf,
                       void                  *pvSeccDataBuf,
                       uint64_t               ulSecIndex)
{
             int    i;
    unsigned char  *pucBuffer      = (unsigned char *)pvSecBuf;
    unsigned char  *pucBufferSpare = (unsigned char *)pvSeccDataBuf;

    NF_NFCE_L();                                                        /*  Ƭѡ                        */
    NF_CLEAR_RB();
    NF_CMD(NAND_PAGE_READ_1);                                           /*  ׼����                      */

    /*
     * д���ַ
     */
    NF_ADDR((uint8_t)(0));
    NF_ADDR((uint8_t)(ulSecIndex));
    NF_ADDR((uint8_t)(ulSecIndex >> 8));
    NF_ADDR((uint8_t)(ulSecIndex >> 16));

    NF_DETECT_RB();                                                     /*  �ȴ�����                  	*/

    for (i = 0; i < NAND_BYTES_PER_SECT; i++) {
        *pucBuffer++ = NF_RDDATA8();                                    /*  ��ȡ����                    */
    }

    for (i = 0; i < NAND_SPARE_PER_SECT; i++) {
        *pucBufferSpare++ = NF_RDDATA8();                               /*  ��ȡ����                    */
    }
    NF_NFCE_H();                                                        /*  �ͷ���ƬƬѡ               	*/

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** Function name:           nand_write
** Descriptions:            д��һ������������, ������չ����
** input parameters:        pvSecBuf            ������������
**                          pvSeccDataBuf       ����������������
**                          ulSecIndex          ��������
** output parameters:       NONE
** Returned value:          0                   ���
**                          -1                  δ���
*********************************************************************************************************/
static int  nand_write (const void            *pvSecBuf,
                        const void            *pvSeccDataBuf,
                        uint64_t               ulSecIndex)
{
             int    i;
    unsigned char  *pucBuffer      = (unsigned char *)pvSecBuf;
    unsigned char  *pucBufferSpare = (unsigned char *)pvSeccDataBuf;

    NF_NFCE_L();                                                        /*  Ƭѡ                     	*/

    NF_CMD(0);                                    					    /*  ׼�����                    */
    NF_CMD(NAND_PAGE_PROGRAM_1);                                        /*  ׼�����                    */

    NF_ADDR((uint8_t)(0));
    NF_ADDR((uint8_t)(ulSecIndex));
    NF_ADDR((uint8_t)(ulSecIndex >> 8));
    NF_ADDR((uint8_t)(ulSecIndex >> 16));

    for (i = 0; i < NAND_BYTES_PER_SECT; i++) {                         /*  д������                    */
        NF_WRDATA8(*pucBuffer);
        pucBuffer++;
    }

    for (i = 0; i < NAND_SPARE_PER_SECT; i++) {                         /*  д����չ����                */
        NF_WRDATA8(*pucBufferSpare);
        pucBufferSpare++;
    }

    NF_CLEAR_RB();

    NF_CMD(NAND_PAGE_PROGRAM_2);                                        /*  ׼�����                    */

    NF_DETECT_RB();

    return  (nand_is_ok());
}
/*********************************************************************************************************
** Function name:           nand_erase
** Descriptions:            ����оƬָ���Ŀ�
** input parameters:        ulBlockIndex        ������
** output parameters:       NONE
** Returned value:          0                   ���
**                          -1                  δ���
*********************************************************************************************************/
static int  nand_erase (uint64_t  ulBlockIndex)
{
    uint64_t ulSecIndex = ulBlockIndex * 32;

    NF_NFCE_L();                                                        /*  Ƭѡ               			*/

    NF_CMD(NAND_BLOCK_ERASE_1);                                         /*  ׼������                 	*/

    NF_ADDR((uint8_t)(ulSecIndex));
    NF_ADDR((uint8_t)(ulSecIndex >> 8));
    NF_ADDR((uint8_t)(ulSecIndex >> 16));

    NF_CLEAR_RB();

    NF_CMD(NAND_BLOCK_ERASE_2);                                         /*  ��ʼ���                    */

    NF_DETECT_RB();

    return  (nand_is_ok());
}
/*********************************************************************************************************
** Function name:           nand_chip_init
** Descriptions:            ��ʼ�� NAND FLASH оƬ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0                   �ɹ�
**                          -1                  δ�ɹ�
*********************************************************************************************************/
static int  nand_chip_init (void)
{
    uint16_t id;

    nand_port_init();                                                   /*  ��ʼ���˿�             	    */
    nand_reset();                                                       /*  ��λ NAND FLASH оƬ        */
    nand_read_id(&id);                                           		/*  ��ȡоƬ ID ��              */

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ˽����Ϣ
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
** �豸������Ϣ
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
** ��д����
*********************************************************************************************************/
typedef struct {
    void       *data;
    void       *spare;
} yaffs_RwParam;

/*
 * �� mtdblock
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
         * ��һ�δ�ʱ�ĳ�ʼ������
         */
        if (nand_chip_init() != 0) {
            seterrno(EIO);
            return -1;
        } else {
            return 0;
        }
    } else {
        /*
         * ����豸������ͬʱ�򿪶��, ��ʹ�����´���:
         */
        atomic_dec(&(((device_t *)file->ctx)->ref));
        seterrno(EBUSY);
        return -1;
    }
}

/*
 * �ر� mtdblock
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
         * �������һ�ιر�ʱ���������
         */
    }
    atomic_dec(&(((device_t *)file->ctx)->ref));
    return 0;
}

/*
 * ���� mtdblock
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
 * �� mtdblock ��
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
 * д mtdblock ��
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
 * mtdblock ����
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
** Descriptions:            ��ʼ�� mtdblock
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
