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
** File name:               mini2440_lcd.c
** Last modified Date:      2012-3-15
** Last Version:            1.0.0
** Descriptions:            mini2440 LCD ��Ļ����
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
** ��չ�Ķ�:
** http://hi.baidu.com/760159/blog/item/b4c408894605c49ea4c272a3.html
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/addr_config.h"
#include "drivers/fb.h"
#include "../s3c2440.h"
/*********************************************************************************************************
** LCD �ͺ�����
*********************************************************************************************************/
#define LCD_N35     (1)
#define LCD_PANEL   LCD_N35
/*********************************************************************************************************
** LCD_N35
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

#define LCD_WIDTH   (240)
#define LCD_HEIGHT  (320)
#define LCD_BPP     (16)

#endif
/*********************************************************************************************************
** Function name:           lcd_config
** Descriptions:            ���� LCD
** input parameters:        var                 �ɱ���Ļ��Ϣ
**                          framebuffer         ��Ƶ֡������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int lcd_config(const struct fb_var_screeninfo *var, const struct fb_fix_screeninfo *fix, void *framebuffer)
{
    uint32_t lcdbaseu, offsize, pagewidth, lcdbasel;
    static int init = FALSE;

    if (!init) {
        init = TRUE;

        GPGUP   = GPGUP  | (1 << 4);                                        /*  GPG4 �ر���������           */
        GPGCON  = (GPGCON & ~(0x3 << 8)) | 3 << 8;                          /*  GPG4 -> LCD_PWREN           */
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

        LCDINTMSK = (LCDINTMSK & ~(0x3)) | 0;                               /*  �����ж�                    */
        LPCSEL    = (LPCSEL & ~(1)) | 0;                                    /*  ���� LPC3600/LCC3600 ģʽ   */
        TPAL      = 0x00;                                                   /*  ��ʹ�õ�ɫ��                */

        /* ��Ƶ֡�������ڴ��ַ��λ[30:22]  -> LCDSADDR1[29:21] */
        LCDSADDR1 = (LCDSADDR1 & ~(0x1FF << 21)) | (((uint32_t)framebuffer >> 22) & 0x1FF) << 21;

        lcdbaseu  = (((uint32_t)framebuffer & 0x3FFFFF) >> 1);

        /* ��Ƶ֡�������ڴ��ַ��λ[21:1]   -> LCDSADDR1[20:0] */
        LCDSADDR1 = (LCDSADDR1 & ~(0x1FFFFF)) | lcdbaseu;
    }

    printk("set xoffset = %d, yoffset = %d\r\n", var->xoffset, var->yoffset);

    lcdbaseu  = (((uint32_t)framebuffer & 0x3FFFFF) >> 1);

    offsize   = (var->xoffset + (var->yoffset * HOZVAL)) * var->bits_per_pixel / 8 / 2;

    pagewidth = HOZVAL * var->bits_per_pixel / 8 / 2;

    lcdbasel  = lcdbaseu + (pagewidth + offsize) * LINEVAL;

    /* ��Ƶ֡�������Ľ�����ַ��λ[21:1] -> LCDSADDR2[20:0] */
    LCDSADDR2 = (LCDSADDR2 & ~(0x1FFFFF)) | lcdbasel;

    LCDSADDR3 = (LCDSADDR3 & ~(0x7FF << 11)) | (offsize) << 11;         /*  ������Ļƫ�ƴ�С            */
    LCDSADDR3 = (LCDSADDR3 & ~(0x7FF)) | (pagewidth);                   /*  ������Ļ���                */

    return 0;
}
/*********************************************************************************************************
** Function name:           video_check
** Descriptions:            ���ɱ���Ļ��Ϣ�Ƿ�Ϸ�
** input parameters:        var                 �ɱ���Ļ��Ϣ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int video_check(struct fb_var_screeninfo *var)
{
    return 0;
}
/*********************************************************************************************************
** Function name:           video_onoff
** Descriptions:            ������Ƶ��Ƶ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void video_onoff(bool_t on)
{
    if (on) {
        LCDCON1 = (LCDCON1 & ~(1)) | ENVID;                             /*  ������Ƶ���                */
    } else {
        LCDCON1 = (LCDCON1 & ~(1)) | 0;                                 /*  �ر���Ƶ���                */
    }
}
/*********************************************************************************************************
** Function name:           lcd_init
** Descriptions:            ��ʼ�� LCD
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int lcd_init(void)
{
    struct fb_var_screeninfo var;

    var.xoffset        = 0;
    var.yoffset        = 0;
    var.xres           = LCD_WIDTH;
    var.yres           = LCD_HEIGHT;
    var.xres_virtual   = LCD_WIDTH;
    var.yres_virtual   = LCD_HEIGHT * 2;
    var.bits_per_pixel = LCD_BPP;
    var.red.offset     = 11;
    var.red.length     = 5;
    var.green.offset   = 5;
    var.green.length   = 6;
    var.blue.offset    = 5;
    var.blue.length    = 0;

    return fb_create("/dev/fb0", (void *)FB_MEM_BASE, &var, lcd_config, video_check, video_onoff);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
