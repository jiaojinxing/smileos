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
** File name:               s3c2440_asm.h
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            s3c2440 寄存器定义
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
#ifndef S3C2440_ASM_H_
#define S3C2440_ASM_H_

/* NAND Flash Controller */
#define NAND_CTL_BASE        (0x4E000000)
#define oNFCONF              0x00
#define oNFCONT              0x04
#define oNFCMD               0x08
#define oNFADDR              0x0c
#define oNFDATA              0x10
#define oNFSTAT              0x20
#define oNFECC               0x2c

// Memory Controllers
#define BWSCON               (0x48000000)
#define BANKCON0             (0x48000004)
#define BANKCON1             (0x48000008)
#define BANKCON2             (0x4800000C)
#define BANKCON3             (0x48000010)
#define BANKCON4             (0x48000014)
#define BANKCON5             (0x48000018)
#define BANKCON6             (0x4800001C)
#define BANKCON7             (0x48000020)
#define REFRESH              (0x48000024)
#define BANKSIZE             (0x48000028)
#define MRSRB6               (0x4800002C)
#define MRSRB7               (0x48000030)

// USB Host Controller
#define HcRevision           (0x49000000)
#define HcControl            (0x49000004)
#define HcCommonStatus       (0x49000008)
#define HcInterruptStatus    (0x4900000C)
#define HcInterruptEnable    (0x49000010)
#define HcInterruptDisable   (0x49000014)
#define HcHCCA               (0x49000018)
#define HcPeriodCuttentED    (0x4900001C)
#define HcControlHeadED      (0x49000020)
#define HcControlCurrentED   (0x49000024)
#define HcBulkHeadED         (0x49000028)
#define HcBulkCurrentED      (0x4900002C)
#define HcDoneHead           (0x49000030)
#define HcRmInterval         (0x49000034)
#define HcFmRemaining        (0x49000038)
#define HcFmNumber           (0x4900003C)
#define HcPeriodicStart      (0x49000040)
#define HcLSTreshold         (0x49000044)
#define HcRhDescriptorA      (0x49000048)
#define HcRhDescriptorB      (0x4900004C)
#define HcRhStatus           (0x49000050)
#define HcRhPortStatus1      (0x49000054)
#define HcRhPortStatus2      (0x49000058)

// Interrupt Controller
#define SRCPND               (0x4A000000)
#define INTMOD               (0x4A000004)
#define INTMSK               (0x4A000008)
#define PRIORITY             (0x4A00000C)
#define INTPND               (0x4A000010)
#define INTOFFSET            (0x4A000014)
#define SUBSRCPND            (0x4A000018)
#define INTSUBMSK            (0x4A00001C)

// DMA
#define DISRC0               (0x4B000000)
#define DISRCC0              (0x4B000004)
#define DIDST0               (0x4B000008)
#define DIDSTC0              (0x4B00000C)
#define DCON0                (0x4B000010)
#define DSTAT0               (0x4B000014)
#define DCSRC0               (0x4B000018)
#define DCDST0               (0x4B00001C)
#define DMASKTRIG0           (0x4B000020)

#define DISRC1               (0x4B000040)
#define DISRCC1              (0x4B000044)
#define DIDST1               (0x4B000048)
#define DIDSTC1              (0x4B00004C)
#define DCON1                (0x4B000050)
#define DSTAT1               (0x4B000054)
#define DCSRC1               (0x4B000058)
#define DCDST1               (0x4B00005C)
#define DMASKTRIG1           (0x4B000060)

#define DISRC2               (0x4B000080)
#define DISRCC2              (0x4B000084)
#define DIDST2               (0x4B000088)
#define DIDSTC2              (0x4B00008C)
#define DCON2                (0x4B000090)
#define DSTAT2               (0x4B000094)
#define DCSRC2               (0x4B000098)
#define DCDST2               (0x4B00009C)
#define DMASKTRIG2           (0x4B0000a0)

#define DISRC3               (0x4B0000C0)
#define DISRCC3              (0x4B0000C4)
#define DIDST3               (0x4B0000C8)
#define DIDSTC3              (0x4B0000CC)
#define DCON3                (0x4B0000D0)
#define DSTAT3               (0x4B0000D4)
#define DCSRC3               (0x4B0000D8)
#define DCDST3               (0x4B0000DC)
#define DMASKTRIG3           (0x4B0000E0)


// Clock & Power Management
#define LOCKTIME             (0x4C000000)
#define MPLLCON              (0x4C000004)
#define UPLLCON              (0x4C000008)
#define CLKCON               (0x4C00000C)
#define CLKSLOW              (0x4C000010)
#define CLKDIVN              (0x4C000014)
#define CAMDIVN              (0x4C000018)

// LCD Controller
#define LCDCON1              (0x4D000000)
#define LCDCON2              (0x4D000004)
#define LCDCON3              (0x4D000008)
#define LCDCON4              (0x4D00000C)
#define LCDCON5              (0x4D000010)
#define LCDSADDR1            (0x4D000014)
#define LCDSADDR2            (0x4D000018)
#define LCDSADDR3            (0x4D00001C)
#define REDLUT               (0x4D000020)
#define GREENLUT             (0x4D000024)
#define BLUELUT              (0x4D000028)
#define DITHMODE             (0x4D00004C)
#define TPAL                 (0x4D000050)
#define LCDINTPND            (0x4D000054)
#define LCDSRCPND            (0x4D000058)
#define LCDINTMSK            (0x4D00005C)
#define LPCSEL               (0x4D000060)
#define PALETTE               0x4D000400

// NAND flash
#define NFCONF               (0x4E000000)
#define NFCONT               (0x4E000004)
#define NFCMD                (0x4E000008)
#define NFADDR               (0x4E00000C)
#define NFDATA               (0x4E000010)
#define NFMECCD0             (0x4E000014)
#define NFMECCD1             (0x4E000018)
#define NFSECCD              (0x4E00001C)
#define NFSTAT               (0x4E000020)
#define NFESTAT0             (0x4E000024)
#define NFESTAT1             (0x4E000028)
#define NFMECC0              (0x4E00002C)
#define NFMECC1              (0x4E000030)
#define NFSECC               (0x4E000034)
#define NFSBLK               (0x4E000038)
#define NFEBLK               (0x4E00003C)

// Camera Interface
#define CISRCFMT             (0x4F000000)
#define CIWDOFST             (0x4F000004)
#define CIGCTRL              (0x4F000008)
#define CICOYSA1             (0x4F000018)
#define CICOYSA2             (0x4F00001C)
#define CICOYSA3             (0x4F000020)
#define CICOYSA4             (0x4F000024)
#define CICOCBSA1            (0x4F000028)
#define CICOCBSA2            (0x4F00002C)
#define CICOCBSA3            (0x4F000030)
#define CICOCBSA4            (0x4F000034)
#define CICORSA1             (0x4F000038)
#define CICORSA2             (0x4F00003C)
#define CICORSA3             (0x4F000040)
#define CICORSA4             (0x4F000044)
#define CICOTRGFMT           (0x4F000048)
#define CICOCTRL             (0x4F00004C)
#define CICOSCPRERATIO       (0x4F000050)
#define CICOSCPREDST         (0x4F000054)
#define CICOSCCTRL           (0x4F000058)
#define CICOTAREA            (0x4F00005C)
#define CICOSTATUS           (0x4F000064)
#define CIPRCLRSA1           (0x4F00006C)
#define CIPRCLRSA2           (0x4F000070)
#define CIPRCLRSA3           (0x4F000074)
#define CIPRCLRSA4           (0x4F000078)
#define CIPRTRGFMT           (0x4F00007C)
#define CIPRCTRL             (0x4F000080)
#define CIPRSCPRERATIO       (0x4F000084)
#define CIPRSCPREDST         (0x4F000088)
#define CIPRSCCTRL           (0x4F00008C)
#define CIPRTAREA            (0x4F000090)
#define CIPRSTATUS           (0x4F000098)
#define CIIMGCPT             (0x4F0000A0)


// UART
#define ULCON0               (0x50000000)
#define UCON0                (0x50000004)
#define UFCON0               (0x50000008)
#define UMCON0               (0x5000000C)
#define UTRSTAT0             (0x50000010)
#define UERSTAT0             (0x50000014)
#define UFSTAT0              (0x50000018)
#define UMSTAT0              (0x5000001C)
#define UTXH0                (0x50000020)
#define URXH0                (0x50000024)
#define UBRDIV0              (0x50000028)

#define ULCON1               (0x50004000)
#define UCON1                (0x50004004)
#define UFCON1               (0x50004008)
#define UMCON1               (0x5000400C)
#define UTRSTAT1             (0x50004010)
#define UERSTAT1             (0x50004014)
#define UFSTAT1              (0x50004018)
#define UMSTAT1              (0x5000401C)
#define UTXH1                (0x50004020)
#define URXH1                (0x50004024)
#define UBRDIV1              (0x50004028)

#define ULCON2               (0x50008000)
#define UCON2                (0x50008004)
#define UFCON2               (0x50008008)
#define UMCON2               (0x5000800C)
#define UTRSTAT2             (0x50008010)
#define UERSTAT2             (0x50008014)
#define UFSTAT2              (0x50008018)
#define UTXH2                (0x50008020)
#define URXH2                (0x50008024)
#define UBRDIV2              (0x50008028)

#ifdef __BIG_ENDIAN
#undef  UTXH0
#define UTXH0                (0x50000023)
#undef  URXH0
#define URXH0                (0x50000027)
#undef  UTXH1
#define UTXH1                (0x50004023)
#undef  URXH1
#define URXH1                (0x50004027)
#undef  UTXH2
#define UTXH2                (0x50008023)
#undef  URXH2
#define URXH2                (0x50008027)
#endif

// PWM Timer
#define TCFG0                (0x51000000)
#define TCFG1                (0x51000004)
#define TCON                 (0x51000008)
#define TCNTB0               (0x5100000C)
#define TCMPB0               (0x51000010)
#define TCNTO0               (0x51000014)
#define TCNTB1               (0x51000018)
#define TCMPB1               (0x5100001C)
#define TCNTO1               (0x51000020)
#define TCNTB2               (0x51000024)
#define TCMPB2               (0x51000028)
#define TCNTO2               (0x5100002C)
#define TCNTB3               (0x51000030)
#define TCMPB3               (0x51000034)
#define TCNTO3               (0x51000038)
#define TCNTB4               (0x5100003C)
#define TCNTO4               (0x51000040)


// USB Device
#ifdef  __BIG_ENDIAN
#define FUNC_ADDR_REG        (0x52000143)
#define PWR_REG              (0x52000147)
#define EP_INT_REG           (0x5200014B)
#define USB_INT_REG          (0x5200015B)
#define EP_INT_EN_REG        (0x5200015F)
#define USB_INT_EN_REG       (0x5200016F)
#define FRAME_NUM1_REG       (0x52000173)
#define FRAME_NUM2_REG       (0x52000177)
#define INDEX_REG            (0x5200017B)
#define MAXP_REG             (0x52000183)
#define EP0_CSR              (0x52000187)
#define IN_CSR1_REG          (0x52000187)
#define IN_CSR2_REG          (0x5200018B)
#define OUT_CSR1_REG         (0x52000193)
#define OUT_CSR2_REG         (0x52000197)
#define OUT_FIFO_CNT1_REG    (0x5200019B)
#define OUT_FIFO_CNT2_REG    (0x5200019F)
#define EP0_FIFO             (0x520001C3)
#define EP1_FIFO             (0x520001C7)
#define EP2_FIFO             (0x520001CF)
#define EP3_FIFO             (0x520001CF)
#define EP4_FIFO             (0x520001D3)
#define EP1_DMA_CON          (0x52000203)
#define EP1_DMA_UNIT         (0x52000207)
#define EP1_DMA_FIFO         (0x5200020B)
#define EP1_DMA_TTC_L        (0x5200020F)
#define EP1_DMA_TTC_M        (0x52000213)
#define EP1_DMA_TTC_H        (0x52000217)
#define EP2_DMA_CON          (0x5200021B)
#define EP2_DMA_UNIT         (0x5200021F)
#define EP2_DMA_FIFO         (0x52000223)
#define EP2_DMA_TTC_L        (0x52000227)
#define EP2_DMA_TTC_M        (0x5200022B)
#define EP2_DMA_TTC_H        (0x5200022F)
#define EP3_DMA_CON          (0x52000243)
#define EP3_DMA_UNIT         (0x52000247)
#define EP3_DMA_FIFO         (0x5200024B)
#define EP3_DMA_TTC_L        (0x5200024F)
#define EP3_DMA_TTC_M        (0x52000253)
#define EP3_DMA_TTC_H        (0x52000257)
#define EP4_DMA_CON          (0x5200025B)
#define EP4_DMA_UNIT         (0x5200025F)
#define EP4_DMA_FIFO         (0x52000263)
#define EP4_DMA_TTC_L        (0x52000267)
#define EP4_DMA_TTC_M        (0x5200026B)
#define EP4_DMA_TTC_H        (0x5200026F)

#else   // Little Endian
#define FUNC_ADDR_REG        (0x52000140)
#define PWR_REG              (0x52000144)
#define EP_INT_REG           (0x52000148)
#define USB_INT_REG          (0x52000158)
#define EP_INT_EN_REG        (0x5200015C)
#define USB_INT_EN_REG       (0x5200016C)
#define FRAME_NUM1_REG       (0x52000170)
#define FRAME_NUM2_REG       (0x52000174)
#define INDEX_REG            (0x52000178)
#define MAXP_REG             (0x52000180)
#define EP0_CSR              (0x52000184)
#define IN_CSR1_REG          (0x52000184)
#define IN_CSR2_REG          (0x52000188)
#define OUT_CSR1_REG         (0x52000190)
#define OUT_CSR2_REG         (0x52000194)
#define OUT_FIFO_CNT1_REG    (0x52000198)
#define OUT_FIFO_CNT2_REG    (0x5200019C)
#define EP0_FIFO             (0x520001C0)
#define EP1_FIFO             (0x520001C4)
#define EP2_FIFO             (0x520001C8)
#define EP3_FIFO             (0x520001CC)
#define EP4_FIFO             (0x520001D0)
#define EP1_DMA_CON          (0x52000200)
#define EP1_DMA_UNIT         (0x52000204)
#define EP1_DMA_FIFO         (0x52000208)
#define EP1_DMA_TTC_L        (0x5200020C)
#define EP1_DMA_TTC_M        (0x52000210)
#define EP1_DMA_TTC_H        (0x52000214)
#define EP2_DMA_CON          (0x52000218)
#define EP2_DMA_UNIT         (0x5200021C)
#define EP2_DMA_FIFO         (0x52000220)
#define EP2_DMA_TTC_L        (0x52000224)
#define EP2_DMA_TTC_M        (0x52000228)
#define EP2_DMA_TTC_H        (0x5200022C)
#define EP3_DMA_CON          (0x52000240)
#define EP3_DMA_UNIT         (0x52000244)
#define EP3_DMA_FIFO         (0x52000248)
#define EP3_DMA_TTC_L        (0x5200024C)
#define EP3_DMA_TTC_M        (0x52000250)
#define EP3_DMA_TTC_H        (0x52000254)
#define EP4_DMA_CON          (0x52000258)
#define EP4_DMA_UNIT         (0x5200025C)
#define EP4_DMA_FIFO         (0x52000260)
#define EP4_DMA_TTC_L        (0x52000264)
#define EP4_DMA_TTC_M        (0x52000268)
#define EP4_DMA_TTC_H        (0x5200026C)
#endif

// Watchdog Timer
#define WTCON                (0x53000000)
#define WTDAT                (0x53000004)
#define WTCNT                (0x53000008)

// IIC
#define IICCON               (0x54000000)
#define IICSTAT              (0x54000004)
#define IICADD               (0x54000008)
#define IICDS                (0x5400000C)
#define IICLC                (0x54000010)

// IIS
#define IISCON               (0x55000000)
#define IISMOD               (0x55000004)
#define IISPSR               (0x55000008)
#define IISFCON              (0x5500000C)
#define IISFIFO              (0x55000010)

#ifdef __BIG_ENDIAN
#undef  IISFIFO
#define IISFIFO              (0x55000012)
#endif

// I/O port
#define GPACON               (0x56000000)
#define GPADAT               (0x56000004)

#define GPBCON               (0x56000010)
#define GPBDAT               (0x56000014)
#define GPBUP                (0x56000018)

#define GPCCON               (0x56000020)
#define GPCDAT               (0x56000024)
#define GPCUP                (0x56000028)

#define GPDCON               (0x56000030)
#define GPDDAT               (0x56000034)
#define GPDUP                (0x56000038)

#define GPECON               (0x56000040)
#define GPEDAT               (0x56000044)
#define GPEUP                (0x56000048)

#define GPFCON               (0x56000050)
#define GPFDAT               (0x56000054)
#define GPFUP                (0x56000058)

#define GPGCON               (0x56000060)
#define GPGDAT               (0x56000064)
#define GPGUP                (0x56000068)

#define GPHCON               (0x56000070)
#define GPHDAT               (0x56000074)
#define GPHUP                (0x56000078)

#define GPJCON               (0x560000D0)
#define GPJDAT               (0x560000D4)
#define GPJUP                (0x560000D8)

#define MISCCR               (0x56000080)
#define DCLKCON              (0x56000084)
#define EXTINT0              (0x56000088)
#define EXTINT1              (0x5600008C)
#define EXTINT2              (0x56000090)
#define EINTFLT0             (0x56000094)
#define EINTFLT1             (0x56000098)
#define EINTFLT2             (0x5600009C)
#define EINTFLT3             (0x560000A0)
#define EINTMASK             (0x560000A4)
#define EINTPEND             (0x560000A8)
#define GSTATUS0             (0x560000AC)
#define GSTATUS1             (0x560000B0)
#define GSTATUS2             (0x560000B4)
#define GSTATUS3             (0x560000B8)
#define GSTATUS4             (0x560000BC)
#define MSLCON               (0x560000CC)

// RTC
#ifdef  __BIG_ENDIAN
#define RTCCON               (0x57000043)
#define TICNT                (0x57000047)
#define RTCALM               (0x57000053)
#define ALMSEC               (0x57000057)
#define ALMMIN               (0x5700005B)
#define ALMHOUR              (0x5700005F)
#define ALMDATE              (0x57000063)
#define ALMMON               (0x57000067)
#define ALMYEAR              (0x5700006B)
#define RTCRST               (0x5700006F)
#define BCDSEC               (0x57000073)
#define BCDMIN               (0x57000077)
#define BCDHOUR              (0x5700007B)
#define BCDDATE              (0x5700007F)
#define BCDDAY               (0x57000083)
#define BCDMON               (0x57000087)
#define BCDYEAR              (0x5700008B)

#else   //Little Endian
#define RTCCON               (0x57000040)
#define TICNT                (0x57000044)
#define RTCALM               (0x57000050)
#define ALMSEC               (0x57000054)
#define ALMMIN               (0x57000058)
#define ALMHOUR              (0x5700005C)
#define ALMDATE              (0x57000060)
#define ALMMON               (0x57000064)
#define ALMYEAR              (0x57000068)
#define RTCRST               (0x5700006C)
#define BCDSEC               (0x57000070)
#define BCDMIN               (0x57000074)
#define BCDHOUR              (0x57000078)
#define BCDDATE              (0x5700007C)
#define BCDDAY               (0x57000080)
#define BCDMON               (0x57000084)
#define BCDYEAR              (0x57000088)
#endif

// A/D Converter
#define ADCCON               (0x58000000)
#define ADCTSC               (0x58000004)
#define ADCDLY               (0x58000008)
#define ADCDAT0              (0x5800000C)
#define ADCDAT1              (0x58000010)
#define ADCUPDN              (0x58000014)

// SPI
#define SPCON0               (0x59000000)
#define SPSTA0               (0x59000004)
#define SPPIN0               (0x59000008)
#define SPPRE0               (0x5900000C)
#define SPTDAT0              (0x59000010)
#define SPRDAT0              (0x59000014)

#define SPCON1               (0x59000020)
#define SPSTA1               (0x59000024)
#define SPPIN1               (0x59000028)
#define SPPRE1               (0x5900002C)
#define SPTDAT1              (0x59000030)
#define SPRDAT1              (0x59000034)

// SD Interface
#define SDICON               (0x5A000000)
#define SDIPRE               (0x5A000004)
#define SDICARG              (0x5A000008)
#define SDICCON              (0x5A00000C)
#define SDICSTA              (0x5A000010)
#define SDIRSP0              (0x5A000014)
#define SDIRSP1              (0x5A000018)
#define SDIRSP2              (0x5A00001C)
#define SDIRSP3              (0x5A000020)
#define SDIDTIMER            (0x5A000024)
#define SDIBSIZE             (0x5A000028)
#define SDIDCON              (0x5A00002C)
#define SDIDCNT              (0x5A000030)
#define SDIDSTA              (0x5A000034)
#define SDIFSTA              (0x5A000038)
#define SDIIMSK              (0x5A00003C)
#define SDIDAT               (0x5A000040)

#ifdef __BIG_ENDIAN
#undef  SDIDAT
#define SDIDAT               (0x5A000043)
#endif

// AC97 Audio-CODEC Interface
#define AC_GLBCTRL           (0x5B000000)
#define AC_GLBSTAT           (0x5B000004)
#define AC_CODEC_CMD         (0x5B000008)
#define AC_CODEC_STAT        (0x5B00000C)
#define AC_PCMADDR           (0x5B000010)
#define AC_MICADDR           (0x5B000014)
#define AC_PCMDATA           (0x5B000018)
#define AC_MICDATA           (0x5B00001C)

#endif                                                                  /*  S3C2440_ASM_H_              */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
