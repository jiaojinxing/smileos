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
** File name:               s3c2440_lcd.c
** Last modified Date:      2012-3-15
** Last Version:            1.0.0
** Descriptions:            S3C2440 LCD ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-15
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
#include "kern/config.h"
#include "kern/types.h"
#include "s3c2440.h"
/*
 * ��չ�Ķ�:
 * http://hi.baidu.com/760159/blog/item/b4c408894605c49ea4c272a3.html
 */
/*********************************************************************************************************
  LCD �ͺ�����
*********************************************************************************************************/
#define LCD_N35     (1)
#define LCD_PANEL   LCD_N35
/*********************************************************************************************************
  LCD_N35
*********************************************************************************************************/
#if LCD_PANEL == LCD_N35
#define CLKVAL      (4)                                                 /*  LCD Ƶ��                    */
#define PNRMODE     (3)                                                 /*  ��ʾģʽ: TFT LCD panel     */
#define BPPMODE     (12)                                                /*  BPP ģʽ: 16 bpp for TFT    */
#define ENVID       (1)                                                 /*  ������Ƶ���                */

#define VBPD        (1)                                                 /*  ��ֱͬ���źź��            */
#define LINEVAL     (320)                                               /*  ��ֱ�ߴ�                    */
#define VFPD        (5)                                                 /*  ��ֱͬ���ź�ǰ��            */
#define VSPW        (1)                                                 /*  ��ֱͬ���ź�����            */

#define HBPD        (39)                                                /*  ˮƽͬ���źź��            */
#define HOZVAL      (240)                                               /*  ˮƽ�ߴ�                    */
#define HFPD        (14)                                                /*  ˮƽͬ���ź�ǰ��            */
#define HSPW        (5)                                                 /*  ˮƽͬ���ź�����            */

#define OFFSIZE     (0)                                                 /*  ������Ļƫ�ƴ�С(��λ����)  */
#define PAGEWIDTH   (HOZVAL)                                            /*  ������Ļ���(��λ����)      */

#define FRM565      (1)                                                 /*  16 BPP ��Ƶ���ݸ�ʽ: RGB565 */
#define PWREN       (1)                                                 /*  ʹ�� LCD_PWREN ����ź�     */
#define ENLEND      (0)                                                 /*  ���� LEND ����ź�          */

#define BSWP        (0)                                                 /*  �ֽڲ�����                  */
#define HWSWP       (1)                                                 /*  ���ֽ���                    */
#define BPP24BL     (0)                                                 /*  24 BPP ��Ƶ�����ֽ���: LSB  */

#define INVVCLK     (0)                                                 /*  ����ת VCLK                 */
#define INVVLINE    (1)                                                 /*  ��ת VLINE                  */
#define INVVFRAME   (1)                                                 /*  ��ת VFRAME                 */
#define INVVD       (0)                                                 /*  ����ת VD                   */
#define INVVDEN     (0)                                                 /*  ����ת VDEN                 */
#define INVPWREN    (0)                                                 /*  ����ת PWREN                */
#define INVLEND     (0)                                                 /*  ����ת LEND                 */

#define LCD_WIDTH   (HOZVAL)
#define LCD_HEIGHT  (LINEVAL)
#define LCD_BPP     16

#endif

/*
 * ��Ƶ֡����
 */
static uint16_t framebuffer[LINEVAL][HOZVAL];

/*
 * ��ʼ�� LCD
 */
void lcd_init(void)
{
    GPGUP   = GPGUP  | (1 << 4);                                        /*  GPG4 �ر���������           */
    GPGCON  = (GPGCON & ~(3 << 8)) | 3 << 8;                            /*  GPG4 -> LCD_PWREN           */
    GPGDAT  = GPGDAT | (1 << 4);                                        /*  �� LCD ��Դ               */

    GPCCON  = 0xAAAAAAAA;                                               /*  LCD ����                    */
    GPDCON  = 0xAAAAAAAA;                                               /*  LCD ����                    */

    LCDCON1 = (LCDCON1 & ~(0x3FF << 8)) | CLKVAL  << 8;                 /*  LCD Ƶ��                    */
    LCDCON1 = (LCDCON1 & ~(0x3   << 5)) | PNRMODE << 5;                 /*  ��ʾģʽ                    */
    LCDCON1 = (LCDCON1 & ~(0xF   << 1)) | BPPMODE << 1;                 /*  BPP ģʽ(ÿ���ص��ֽ���)    */
    LCDCON1 = (LCDCON1 & ~(1)) | 0;                                     /*  �ر���Ƶ���                */

    LCDCON2 = (LCDCON2 & ~(0xFF  << 24)) | VBPD << 24;                  /*  ��ֱͬ���źź��            */
    LCDCON2 = (LCDCON2 & ~(0x3FF << 14)) | (LINEVAL - 1) << 14;         /*  ��ֱ�ߴ�                    */
    LCDCON2 = (LCDCON2 & ~(0xFF  <<  6)) | VFPD << 6;                   /*  ��ֱͬ���ź�ǰ��            */
    LCDCON2 = (LCDCON2 & ~(0x3F)) | VSPW;                               /*  ��ֱͬ���ź�����            */

    LCDCON3 = (LCDCON3 & ~(0x7F  << 19)) | HBPD << 19;                  /*  ˮƽͬ���źź��            */
    LCDCON3 = (LCDCON3 & ~(0x7FF <<  8)) | (HOZVAL - 1) << 8;           /*  ˮƽ�ߴ�                    */
    LCDCON3 = (LCDCON3 & ~(0xFF)) | HFPD;                               /*  ˮƽͬ���ź�ǰ��            */

    LCDCON4 = (LCDCON4 & ~(0xFF)) | HSPW;                               /*  ˮƽͬ���ź�����            */

    LCDCON5 = (LCDCON5 & ~(1 << 12)) | BPP24BL   << 12;                 /*  24 BPP ��Ƶ�����ֽ���       */
    LCDCON5 = (LCDCON5 & ~(1 << 11)) | FRM565    << 11;                 /*  16 BPP ��Ƶ���ݸ�ʽ         */
    LCDCON5 = (LCDCON5 & ~(1 << 10)) | INVVCLK   << 10;                 /*  VCLK ��Ч����               */
    LCDCON5 = (LCDCON5 & ~(1 <<  9)) | INVVLINE  <<  9;                 /*  �Ƿ�ת VLINE              */
    LCDCON5 = (LCDCON5 & ~(1 <<  8)) | INVVFRAME <<  8;                 /*  �Ƿ�ת VFRAME             */
    LCDCON5 = (LCDCON5 & ~(1 <<  7)) | INVVD     <<  7;                 /*  �Ƿ�ת VD                 */
    LCDCON5 = (LCDCON5 & ~(1 <<  6)) | INVVDEN   <<  6;                 /*  �Ƿ�ת VDEN               */
    LCDCON5 = (LCDCON5 & ~(1 <<  5)) | INVPWREN  <<  5;                 /*  �Ƿ�ת PWREN              */
    LCDCON5 = (LCDCON5 & ~(1 <<  4)) | INVLEND   <<  4;                 /*  �Ƿ�ת LEND               */
    LCDCON5 = (LCDCON5 & ~(1 <<  3)) | PWREN     <<  3;                 /*  �Ƿ�ʹ�� LCD_PWREN ����ź� */
    LCDCON5 = (LCDCON5 & ~(1 <<  2)) | ENLEND    <<  2;                 /*  �Ƿ�ʹ�� LEND ����ź�      */
    LCDCON5 = (LCDCON5 & ~(1 <<  1)) | BSWP      <<  1;                 /*  �ֽ��Ƿ񽻻�                */
    LCDCON5 = (LCDCON5 & ~(1 <<  0)) | HWSWP     <<  0;                 /*  �����Ƿ񽻻�                */

    /* ��Ƶ֡�������ڴ��ַ��λ[30:22]  -> LCDSADDR1[29:21] */
    LCDSADDR1 = (LCDSADDR1 & ~(0x1FF << 21)) | (((uint32_t)framebuffer >> 22) & 0x1FF) << 21;

    /* ��Ƶ֡�������ڴ��ַ��λ[21:1]   -> LCDSADDR1[20:0] */
    LCDSADDR1 = (LCDSADDR1 & ~(0x1FFFFF)) | (((uint32_t)framebuffer >> 1) & 0x1FFFFF);

    /* ��Ƶ֡�������Ľ�����ַ��λ[21:1] -> LCDSADDR2[20:0] */
    LCDSADDR2 = (LCDSADDR2 & ~(0x1FFFFF)) | ((((uint32_t)framebuffer + LINEVAL * HOZVAL * 2) >> 1) & 0x1FFFFF);

    LCDSADDR3 = (LCDSADDR3 & ~(0x7FF << 11)) | OFFSIZE << 11;           /*  ������Ļƫ�ƴ�С            */
    LCDSADDR3 = (LCDSADDR3 & ~(0x7FF)) | PAGEWIDTH;                     /*  ������Ļ���                */

    LCDINTMSK = (LCDINTMSK & ~(0x3)) | 0;                               /*  �����ж�                    */
    LPCSEL    = (LPCSEL & ~(1)) | 0;                                    /*  ���� LPC3600/LCC3600 ģʽ   */
    TPAL      = 0;                                                      /*  ��ʹ�õ�ɫ��                */

    LCDCON1   = (LCDCON1 & ~(1)) | ENVID;                               /*  ������Ƶ���                */
}

//void lcd_putpixel(int x, int y, uint32_t col)
//{
//    uint16_t rgb565 = (((col >> 19) & 0x1F) << 11) |
//                      (((col >> 10) & 0x3F) << 5)  |
//                      (((col >>  3) & 0x1F) << 0);
//
//    framebuffer[y][x] = rgb565;
//}
//
//void lcd_clear(uint32_t col)
//{
//    int y, x;
//
//    for (y = 0; y < LCD_HEIGHT; y++) {
//        for (x = 0; x < LCD_WIDTH; x++) {
//            lcd_putpixel(x, y, col);
//        }
//    }
//}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
