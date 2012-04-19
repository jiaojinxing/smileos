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
** File name:               s3c2440_sd.c
** Last modified Date:      2012-4-4
** Last Version:            1.0.0
** Descriptions:            S3C2440 SD 卡驱动
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
#include <unistd.h>
#include <string.h>

#define INICLK      300000
#define SDCLK       24000000
#define MMCCLK      15000000

static uint32_t RCA;

static void sd_delay(uint32_t ms)
{
    usleep(1000 * ms);
}

static int sd_cmd_end(int cmd, int be_resp)
{
    uint32_t finish0;

    if (!be_resp) {
        finish0 = SDICSTA;

        while ((finish0 & 0x800) != 0x800) {
            finish0 = SDICSTA;
        }

        SDICSTA = finish0;

        return 0;
    } else {
        finish0 = SDICSTA;

        while (!(((finish0 & 0x200) == 0x200) | ((finish0 & 0x400) == 0x400))) {
            finish0 = SDICSTA;
        }

        if (cmd == 1 || cmd == 41) {

            if ((finish0 & 0xf00) != 0xa00) {
                SDICSTA = finish0;

                if (((finish0 & 0x400) == 0x400)) {
                    return -1;
                }
            }

            SDICSTA = finish0;
        } else {
            if ((finish0 & 0x1f00) != 0xa00) {

                SDICSTA = finish0;

                if (((finish0 & 0x400) == 0x400)) {
                    return -1;
                }
            }

            SDICSTA = finish0;
        }

        return 0;
    }
}

static int sd_data_end(void)
{
    uint32_t finish;

    finish = SDIDSTA;

    while (!(((finish & 0x10) == 0x10) | ((finish & 0x20) == 0x20))) {
        finish = SDIDSTA;
    }

    if ((finish & 0xfc) != 0x10) {
        SDIDSTA = 0xec;
        return -1;
    }

    return 0;
}

static void sd_cmd0(void)
{
    SDICARG = 0x0;
    SDICCON = (1 << 8) | 0x40;

    sd_cmd_end(0, 0);

    SDICSTA = 0x800;
}

static int sd_cmd55(void)
{
    SDICARG = RCA << 16;
    SDICCON = (0x1 << 9) | (0x1 << 8) | 0x77;

    if (sd_cmd_end(55, 1) == -1) {
        return -1;
    }

    SDICSTA = 0xa00;

    return 0;
}

static void sd_sel_desel(char sel_desel)
{
    if (sel_desel) {
        RECMDS7:

        SDICARG = RCA << 16;
        SDICCON = (0x1 << 9) | (0x1 << 8) | 0x47;

        if (sd_cmd_end(7, 1) == -1) {
            goto RECMDS7;
        }

        SDICSTA = 0xa00;

        if (SDIRSP0 & 0x1e00 != 0x800) {
            goto RECMDS7;
        }
    } else {
        RECMDD7:

        SDICARG = 0 << 16;
        SDICCON = (0x1 << 8) | 0x47;

        if (sd_cmd_end(7, 0) == -1) {
            goto RECMDD7;
        }

        SDICSTA = 0x800;
    }
}

static void sd_setbus(void)
{
    do {
        sd_cmd55();

        SDICARG = 1 << 1;
        SDICCON = (0x1 << 9) | (0x1 << 8) | 0x46;
    } while (sd_cmd_end(6, 1) == -1);

    SDICSTA = 0xa00;
}

static int sd_ocr(void)
{
    int i;

    for (i = 0; i < 50; i++) {
        sd_cmd55();

        SDICARG = 0xff8000;
        SDICCON = (0x1 << 9) | (0x1 << 8) | 0x69;

        if ((sd_cmd_end(41, 1) == 0) & SDIRSP0 == 0x80ff8000) {
            SDICSTA = 0xa00;
            return 0;
        }

        sd_delay(200);
    }

    SDICSTA = 0xa00;

    return -1;
}

static int sd_init(void)
{
    int i;

    CLKCON |= 1 << 9;

    GPEUP  = GPEUP & (~(0x3f << 5)) | (0x01 << 5);
    GPECON = GPECON & (~(0xfff << 10)) | (0xaaa << 10);

    RCA = 0;

    SDIPRE    = PCLK / (INICLK) - 1;
    SDICON    = (0 << 4) | 1;
    SDIFSTA   = SDIFSTA | (1 << 16);
    SDIBSIZE  = 0x200;
    SDIDTIMER = 0x7fffff;

    for (i = 0; i < 0x1000; i++) {
        ;
    }

    sd_cmd0();

    if (sd_ocr() == 0) {
        printk("%s: sd card found\n", __func__);
    } else {
        printk("%s: sd card no found\n", __func__);
        return -1;
    }

    RECMD2:

    SDICARG = 0x0;
    SDICCON = (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | 0x42;

    if (sd_cmd_end(2, 1) == -1) {
        goto RECMD2;
    }

    SDICSTA = 0xa00;

    RECMD3:

    SDICARG = 0 << 16;
    SDICCON = (0x1 << 9) | (0x1 << 8) | 0x43;

    if (sd_cmd_end(3, 1) == -1) {
        goto RECMD3;
    }

    SDICSTA = 0xa00;

    RCA = (SDIRSP0 & 0xffff0000) >> 16;

    SDIPRE = PCLK / (SDCLK) - 1;

    if (SDIRSP0 & 0x1e00 != 0x600) {
        goto RECMD3;
    }

    sd_sel_desel(1);

    sd_delay(200);

    sd_setbus();

    return 0;
}

int sd_readblock(uint32_t address, uint8_t *buf)
{
    uint32_t status;
    uint32_t temp;
    uint32_t read_cnt;


    SDIFSTA = SDIFSTA | (1 << 16);

    SDIDCON = (2 << 22) | (1 << 19) | (1 << 17) | (1 << 16) | (1 << 14) | (2 << 12) | (1 << 0);
    SDICARG = address;

    RERDCMD:

    SDICCON = (0x1 << 9) | (0x1 << 8) | 0x51;

    if (sd_cmd_end(17, 1) < 0) {
        goto RERDCMD;
    }

    SDICSTA = 0xa00;

    read_cnt = 0;

    while (read_cnt < 128) {
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

    if (sd_data_end() < 0) {
        return -1;
    }

    SDIDCON = SDIDCON & ~(7 << 12);
    SDIFSTA = SDIFSTA & 0x200;
    SDIDSTA = 0x10;

    return 0;
}

int sd_writeblock(uint32_t address, const uint8_t *buf)
{
    uint32_t status;
    uint32_t temp;
    uint32_t write_cnt;

    SDIFSTA = SDIFSTA | (1 << 16);
    SDIDCON = (2 << 22) | (1 << 20) | (1 << 17) | (1 << 16) | (1 << 14) | (3 << 12) | (1 << 0);
    SDICARG = address;

    REWTCMD:

    SDICCON = (0x1 << 9) | (0x1 << 8) | 0x58;

    if (sd_cmd_end(24, 1) < 0) {
        goto REWTCMD;
    }

    SDICSTA = 0xa00;

    write_cnt = 0;

    while (write_cnt < 128 * 1) {
        status = SDIFSTA;

        if ((status & 0x2000) == 0x2000) {
            memcpy(&temp, buf, sizeof(uint32_t));
            SDIDAT = temp;
            write_cnt++;
            buf += 4;
        }
    }

    if (sd_data_end() < 0) {
        return -1;
    }

    SDIDCON = SDIDCON & ~(7 << 12);
    SDIDSTA = 0x10;

    return 0;
}

#include "vfs/vfs.h"
#include "vfs/device.h"
#include <sys/stat.h>
#include <sys/mount.h>

/*
 * 打开 SD 卡
 */
static int sd_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    return 0;
}

/*
 * 控制 SD 卡
 */
static int sd_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    int ret = 0;

    switch (cmd) {
    case 0:
        break;

    default:
        ret = -1;
        break;
    }
    return ret;
}

/*
 * 关闭 SD 卡
 */
static int sd_close(void *ctx, file_t *file)
{
    return 0;
}

/*
 * SD 卡驱动
 */
driver_t sd_drv = {
        .name  = "sd",
        .open  = sd_open,
        .ioctl = sd_ioctl,
        .close = sd_close,
};

/*
 * 创建 SD 卡设备
 */
int sd_create(void)
{
    sd_init();

    device_create("/dev/sd0", "sd", NULL);

    mount("/sd0", "/dev/sd0", "fatfs");

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
