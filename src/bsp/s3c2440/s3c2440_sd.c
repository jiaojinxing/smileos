/*
 * s3c2440_sd.c
 *
 *  Created on: 2012-4-4
 *      Author: Administrator
 */

#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "s3c2440.h"
#include "s3c2440_clock.h"
#include <unistd.h>

#define INICLK  300000
#define SDCLK   24000000
#define MMCCLK  15000000

volatile uint32_t rd_cnt;
volatile uint32_t wt_cnt;
volatile int32_t RCA;

static void sd_delay(uint32_t ms)
{
    usleep(1000 * ms);
}

static int sd_cmd_end(int cmd, int be_resp)
{
    int finish0;

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
    int finish;

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

int sd_init(void)
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
        printk("In SD ready\n");
    } else {
        printk("Initialize fail\nNo Card assertion\n");
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

