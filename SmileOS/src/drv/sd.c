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
** File name:               sd.c
** Last modified Date:      2012-6-29
** Last Version:            1.0.0
** Descriptions:            SD ������
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-6-29
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
#include "kern/kern.h"
#include "drv/sdio.h"
#include "drv/sd.h"
#include <unistd.h>
#include <string.h>
/********************************************************************************************************/
/*
 * SD ��
 */
typedef struct {
    sdio_t         *sdio;
    uint16_t        rca;
    uint8_t         sdhc;
    uint8_t         multi_blk_en;
} sdcard_t;
/********************************************************************************************************/
/*
 * SD ���� SDIO �ӿ���ز�������
 */
/*
 * ��ʼ�� SD ���� SDIO �ӿ�
 */
static int sdio_init(sdcard_t *sd, sdio_t *sdio)
{
    sd->sdio = sdio;

    sd->rca  = 0;

    return sd->sdio->init(sd->sdio);
}

/*
 * ͨ�� SD ���� SDIO �ӿڷ�������
 */
static int sdio_send_cmd(sdcard_t *sd,
                         uint32_t cmd,
                         int has_arg, uint32_t arg,
                         sd_resp_type_t resp_type, sd_resp_t *resp)
{
    return sd->sdio->send_cmd(sd->sdio, cmd, has_arg, arg, resp_type, resp);
}

/*
 * �� SD ���� SDIO �ӿڵ����ʱ������Ϊ���
 */
static int sdio_max_clk(sdcard_t *sd)
{
    return sd->sdio->max_clk(sd->sdio);
}

/*
 * ͨ�� SD ���� SDIO �ӿ�Ԥ����
 */
static int sdio_preread_blk(sdcard_t *sd, uint32_t blk_cnt)
{
    return sd->sdio->preread_blk(sd->sdio, blk_cnt);
}

/*
 * ͨ�� SD ���� SDIO �ӿڶ���
 */
static int sdio_read_blk(sdcard_t *sd, uint32_t len, uint8_t *buf)
{
    return sd->sdio->read_blk(sd->sdio, len, buf);
}

/*
 * ͨ�� SD ���� SDIO �ӿ�Ԥд��
 */
static int sdio_prewrite_blk(sdcard_t *sd, uint32_t blk_cnt)
{
    return sd->sdio->prewrite_blk(sd->sdio, blk_cnt);
}

/*
 * ͨ�� SD ���� SDIO �ӿ�д��
 */
static int sdio_write_blk(sdcard_t *sd, uint32_t len, const uint8_t *buf)
{
    return sd->sdio->write_blk(sd->sdio, len, buf);
}
/********************************************************************************************************/
/*
 * ��������
 */
/*
 * �������� 0
 *
 * GO_IDLE_STATE
 *
 * Resets all cards to idle state
 */
static int sdio_send_cmd0(sdcard_t *sd)
{
    return sdio_send_cmd(sd, 0, 0, 0, SD_R0, NULL);
}

/*
 * �������� 2
 *
 * ALL_SEND_CID
 *
 * Asks any card to send the CID numbers on the CMD line
 * (any card that is connected to the host will respond)
 */
static int sdio_send_cmd2(sdcard_t *sd, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 2, 0, 0, SD_R2, resp);
}

/*
 * �������� 3
 *
 * SEND_RELATIVE_ADDR
 *
 * Ask the card to publish a new relative address (RCA)
 */
static int sdio_send_cmd3(sdcard_t *sd, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 3, 0, 0, SD_R6, resp);
}

/*
 * �������� 4
 *
 * SET_DSR
 *
 * Programs the DSR of all cards
 */
static int sdio_send_cmd4(sdcard_t *sd, uint32_t dsr)
{
    return sdio_send_cmd(sd, 4, 1, dsr << 16, SD_R0, NULL);
}

/*
 * �������� 7
 *
 * SELECT/DESELECT_CARD
 *
 * Command toggles a card between the stand-by and transfer states or between the programming and
 * disconnect states. In both cases, the card is selected by its own relative address and
 * gets deselected by any other address; address 0 deselects all.
 */
static int sdio_send_cmd7(sdcard_t *sd, uint32_t rca, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 7, 1, rca << 16, SD_R1b, resp);
}

/*
 * �������� 8
 *
 * SEND_IF_COND
 *
 * Sends SD Memory Card interface condition,
 * which includes host supply voltage information and asks the card whether card supports voltage.
 * Reserved bits shall be set to '0'.
 */
static int sdio_send_cmd8(sdcard_t *sd, uint32_t vhs, uint32_t check_pattern, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 8, 1, vhs << 8 | check_pattern, SD_R7, resp);
}

/*
 * �������� 9
 *
 * SEND_CSD
 *
 * Addressed card sends its card-specific data (CSD) on the CMD line.
 */
static int sdio_send_cmd9(sdcard_t *sd, uint32_t rca, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 9, 1, rca << 16, SD_R2, resp);
}

/*
 * �������� 10
 *
 * SEND_CID
 *
 * Addressed card sends its card identification (CID) on CMD the line.
 */
static int sdio_send_cmd10(sdcard_t *sd, uint32_t rca, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 10, 1, rca << 16, SD_R2, resp);
}

/*
 * �������� 11
 *
 * VOLTAGE_SWITCH
 *
 * Switch to 1.8V bus signaling level.
 */
static int sdio_send_cmd11(sdcard_t *sd, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 11, 0, 0, SD_R1, resp);
}

/*
 * �������� 12
 *
 * STOP_TRANSMISSION
 *
 * Forces the card to stop transmission
 */
static int sdio_send_cmd12(sdcard_t *sd, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 12, 0, 0, SD_R1b, resp);
}

/*
 * �������� 13
 *
 * SEND_STATUS
 *
 * Addressed card sends its status register.
 */
static int sdio_send_cmd13(sdcard_t *sd, uint32_t rca, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 13, 1, rca << 16, SD_R1, resp);
}

/*
 * �������� 15
 *
 * GO_INACTIVE_STATE
 *
 * Sends an addressed card into the inactive State.
 */
static int sdio_send_cmd15(sdcard_t *sd, uint32_t rca)
{
    return sdio_send_cmd(sd, 15, 1, rca << 16, SD_R0, NULL);
}
/********************************************************************************************************/
/*
 * ��д������
 */
/*
 * �������� 16
 *
 * SET_BLOCKLEN
 */
static int sdio_send_cmd16(sdcard_t *sd, uint32_t blk_len, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 16, 1, blk_len, SD_R1, resp);
}

/*
 * �������� 17
 *
 * READ_SINGLE_BLOCK
 */
static int sdio_send_cmd17(sdcard_t *sd, uint32_t data_addr, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 17, 1, data_addr, SD_R1, resp);
}

/*
 * �������� 18
 *
 * READ_MULTIPLE_BLOCK
 */
static int sdio_send_cmd18(sdcard_t *sd, uint32_t data_addr, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 18, 1, data_addr, SD_R1, resp);
}

/*
 * �������� 19
 *
 * SEND_TUNING_BLOCK
 */
static int sdio_send_cmd19(sdcard_t *sd, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 19, 0, 0, SD_R1, resp);
}

/*
 * �������� 20
 *
 * SPEED_CLASS_CONTROL
 */
static int sdio_send_cmd20(sdcard_t *sd, uint32_t speed_class, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 20, 1, speed_class << 28, SD_R1b, resp);
}

/*
 * �������� 23
 *
 * SET_BLOCK_COUNT
 */
static int sdio_send_cmd23(sdcard_t *sd, uint32_t blk_cnt, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 23, 1, blk_cnt, SD_R1, resp);
}

/*
 * �������� 24
 *
 * WRITE_BLOCK
 */
static int sdio_send_cmd24(sdcard_t *sd, uint32_t data_addr, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 24, 1, data_addr, SD_R1, resp);
}

/*
 * �������� 25
 *
 * WRITE_MULTIPLE_BLOCK
 */
static int sdio_send_cmd25(sdcard_t *sd, uint32_t data_addr, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 25, 1, data_addr, SD_R1, resp);
}
/********************************************************************************************************/
/*
 * Ӧ������
 */
/*
 * ����Ӧ��������� 55
 *
 * APP_CMD
 *
 * Indicates to the card that the next command is an
 * application specific command rather than a standard command
 */
static int sdio_send_cmd55(sdcard_t *sd, uint32_t rca, sd_resp_t *resp)
{
    return sdio_send_cmd(sd, 55, 1, rca << 16, SD_R1, resp);
}

/*
 * ����Ӧ������ 6
 *
 * SET_BUS_WIDTH
 *
 * Defines the data bus width ('00'=1bit or '10'=4 bits bus) to be used for data transfer.
 * The allowed data bus widths are given in SCR register.
 *
 */
static int sdio_send_acmd6(sdcard_t *sd, uint32_t bus_width, sd_resp_t *resp)
{
    if (sdio_send_cmd55(sd, sd->rca, resp) < 0) {
        printk("%s: cmd55 failed\n", __func__);
        return -1;
    }

    return sdio_send_cmd(sd, 6, 1, bus_width << 0, SD_R1, resp);
}

/*
 * ����Ӧ������ 41
 *
 */
static int sdio_send_acmd41(sdcard_t *sd, uint32_t hcs, uint32_t xpc, uint32_t s18r, uint32_t vvw, sd_resp_t *resp)
{
    if (sdio_send_cmd55(sd, sd->rca, resp) < 0) {
        printk("%s: cmd55 failed\n", __func__);
        return -1;
    }

    return sdio_send_cmd(sd, 41, 1, hcs << 30 | xpc << 28 | s18r << 24 | vvw << 0, SD_R3, resp);
}
/********************************************************************************************************/
/*
 * ��ʼ�� SD ��
 */
static int sdcard_init(sdcard_t *sd, sdio_t *sdio)
{
    sd_resp_t  resp;
    int        ret;
    int        i;

    /*
     * ��ʼ�� SD ���� SDIO �ӿ�, SD ���� SDIO �ӿڵ����ʱ�Ӳ��ó��� 400KHz
     */
    ret = sdio_init(sd, sdio);
    if (ret < 0) {
        printk("%s: failed to init sdio\n", __func__);
        return -1;
    }

    /*
     * Wait 74 SDCLK clock cycle in order to initialize the card.
     */
    usleep(100 * 1000);

    /*
     * �������� 0, Resets all cards to idle state
     */
    ret = sdio_send_cmd0(sd);
    if (ret < 0) {
        printk("%s: cmd0 failed\n", __func__);
        return -1;
    }

    /*
     * �������� 8, Sends SD Memory Card interface condition
     */
    ret = sdio_send_cmd8(sd, 1, 0xAA, &resp);
    if (ret < 0) {
        printk("%s: cmd8 failed\n", __func__);
        return -1;
        /*
         * TODO:
         */
    }

    if (resp.resp8[1] != 1 /* 2.7-3.6V */ || resp.resp8[0] != 0xAA) {
        printk("%s: 2.7-3.6V Voltage not accepted or check pattern dismatch\n", __func__);

        printk("resp0=0x%x, resp1=0x%x, resp2=0x%x, resp3=0x%x\n",
                resp.resp8[0], resp.resp8[1], resp.resp8[2], resp.resp8[3]);
        return -1;
    }

    /*
     * ����Ӧ������ 41
     */
    for (i = 0; i < 50; i++) {
        ret = sdio_send_acmd41(sd, 1, 0, 0, 0xFF80 /* 2.7-3.6V */ , &resp);
        if (ret < 0) {
            printk("%s: acmd41 failed, try time = %d\n", __func__, i);
        } else {
            if (!(resp.resp8[1] & 0x80) || resp.resp8[2] != 0xFF /* 2.7-3.6V */) {
                printk("%s: 2.7-3.6V Voltage not accepted\n", __func__);

                printk("resp0=0x%x, resp1=0x%x, resp2=0x%x, resp3=0x%x\n",
                        resp.resp8[0], resp.resp8[1], resp.resp8[2], resp.resp8[3]);
                return -1;
            }

            if (resp.resp8[3] & (1 << 7)) {
                break;
            }
        }

        usleep(200 * 1000);
    }

    if (i == 50) {
        printk("%s: acmd41 timeout\n", __func__);
        return -1;
    }

    printk("%s: 2.7-3.6V Voltage accepted\n", __func__);

    if (resp.resp8[3] & (1 << 6)) {
        printk("%s: Ver2.00 or later High Capacity or Extended Capacity SD Memory Card\n", __func__);
        sd->sdhc = TRUE;
    } else {
        printk("%s: Ver2.00 or later Standard Capacity SD Memory Card\n", __func__);
        sd->sdhc = FALSE;
    }

    if (resp.resp8[3] & (1 << 0)) {
        printk("%s: Switching to 1.8V Accepted\n", __func__);

        /*
         * �������� 11
         */
        ret = sdio_send_cmd11(sd, &resp);
        if (ret < 0) {
            printk("%s: cmd11 failed\n", __func__);
            return -1;
        }

        printk("%s: Switching to 1.8V OK\n", __func__);
    }

    /*
     * �������� 2
     */
    ret = sdio_send_cmd2(sd, &resp);
    if (ret < 0) {
        printk("%s: cmd2 failed\n", __func__);
        return -1;
    }

    printk("%s: MID: 0x%x\n", __func__, resp.resp8[3]);

    printk("%s: OID: %c%c\n", __func__, resp.resp8[2], resp.resp8[1]);

    printk("%s: PND: %c%c%c%c%c\n", __func__,
            resp.resp8[0], resp.resp8[7], resp.resp8[6], resp.resp8[5], resp.resp8[4]);

    printk("%s: PRV: 0x%x\n", __func__, resp.resp8[11]);

    printk("%s: PSN: 0x%x,0x%x,0x%x,0x%x\n", __func__,
            resp.resp8[10], resp.resp8[9], resp.resp8[8], resp.resp8[15]);

    printk("%s: YM : year = %d, mon = %d\n", __func__,
            (resp.resp8[14] & 0x0F) * 10 + 2000 + ((resp.resp8[13] >> 4) & 0x0F), resp.resp8[13] & 0x0F);

    /*
     * �������� 3
     */
    ret = sdio_send_cmd3(sd, &resp);
    if (ret < 0) {
        printk("%s: cmd3 failed\n", __func__);
        return -1;
    }

    printk("%s: rca = 0x%x\n", __func__, resp.resp16[1]);

    printk("%s: card status = 0x%x\n", __func__, resp.resp16[0]);

    sd->rca = resp.resp16[1];

    /*
     * �� SD ���� SDIO �ӿڵ����ʱ������Ϊ���
     */
    sdio_max_clk(sd);

    /*
     * �������� 7
     */
    ret = sdio_send_cmd7(sd, sd->rca, &resp);
    if (ret < 0) {
        printk("%s: cmd7 failed\n", __func__);
        return -1;
    }

    usleep(200 * 1000);

    /*
     * ����Ӧ������ 6
     */
    ret = sdio_send_acmd6(sd, 2 /* 4 bits bus */, &resp);
    if (ret < 0) {
        printk("%s: acmd6 failed\n", __func__);
        return -1;
    }

    if (!sd->sdhc) {
        ret = sdio_send_cmd16(sd, 512 /* blk len */, &resp);
        if (ret < 0) {
            printk("%s: cmd16 failed\n", __func__);
            return -1;
        }
    }

    sd->multi_blk_en = TRUE;

    return 0;
}

/*
 * �� SD �����ж������
 */
static sdcard_t *master_sd;

int sd_read_block(uint32_t blk_nr, uint32_t blk_cnt, uint8_t *buf)
{
    sd_resp_t  resp;
    int        ret;
    int        i;
    uint32_t   data_addr;

    if (master_sd->sdhc) {
        data_addr = blk_nr;
    } else {
        data_addr = blk_nr * 512;
    }

    if (blk_cnt == 0) {
        return -1;
    }

    /*
     * ͨ�� SD ���� SDIO �ӿ�Ԥ����
     */
    ret = sdio_preread_blk(master_sd, blk_cnt);
    if (ret < 0) {
        return -1;
    }

    if (blk_cnt == 1) {
        /*
         * �������� 17
         */
        ret = sdio_send_cmd17(master_sd, data_addr, &resp);
        if (ret < 0) {
            printk("%s: cmd17 failed\n", __func__);
            return -1;
        }
    } else if (!master_sd->multi_blk_en) {
        for (i = 0; i < blk_cnt; i++) {
            ret = sd_read_block(blk_nr + i, 1, buf + 512 * i);
            if (ret < 0) {
                return -1;
            }
        }
        return 0;
    } else {
        /*
         * �������� 23
         */
        ret = sdio_send_cmd23(master_sd, blk_cnt, &resp);
        if (ret < 0) {
            master_sd->multi_blk_en = FALSE;
            for (i = 0; i < blk_cnt; i++) {
                ret = sd_read_block(blk_nr + i, 1, buf + 512 * i);
                if (ret < 0) {
                    return -1;
                }
            }
            return 0;
        }

        /*
         * �������� 18
         */
        ret = sdio_send_cmd18(master_sd, data_addr, &resp);
        if (ret < 0) {
            printk("%s: cmd18 failed\n", __func__);
            return -1;
        }
    }

    /*
     * ͨ�� SD ���� SDIO �ӿڶ���
     */
    return sdio_read_blk(master_sd, blk_cnt * 512, buf);
}

/*
 * �� SD ������д�����
 */
int sd_write_block(uint32_t blk_nr, uint32_t blk_cnt, const uint8_t *buf)
{
    sd_resp_t  resp;
    int        ret;
    int        i;
    uint32_t   data_addr;

    if (master_sd->sdhc) {
        data_addr = blk_nr;
    } else {
        data_addr = blk_nr * 512;
    }

    if (blk_cnt == 0) {
        return -1;
    }

    /*
     * ͨ�� SD ���� SDIO �ӿ�Ԥд��
     */
    ret = sdio_prewrite_blk(master_sd, blk_cnt);
    if (ret < 0) {
        return -1;
    }

    if (blk_cnt == 1) {
        /*
         * �������� 24
         */
        ret = sdio_send_cmd24(master_sd, data_addr, &resp);
        if (ret < 0) {
            printk("%s: cmd24 failed\n", __func__);
            return -1;
        }
    } else if (!master_sd->multi_blk_en) {
        for (i = 0; i < blk_cnt; i++) {
            ret = sd_write_block(blk_nr + i, 1, buf + 512 * i);
            if (ret < 0) {
                return -1;
            }
        }
        return 0;
    } else {
        /*
         * �������� 23
         */
        ret = sdio_send_cmd23(master_sd, blk_cnt, &resp);
        if (ret < 0) {
            master_sd->multi_blk_en = FALSE;
            for (i = 0; i < blk_cnt; i++) {
                ret = sd_write_block(blk_nr + i, 1, buf + 512 * i);
                if (ret < 0) {
                    return -1;
                }
            }
            return 0;
        }

        /*
         * �������� 25
         */
        ret = sdio_send_cmd25(master_sd, data_addr, &resp);
        if (ret < 0) {
            printk("%s: cmd25 failed\n", __func__);
            return -1;
        }
    }

    /*
     * ͨ�� SD ���� SDIO �ӿ�д��
     */
    return sdio_write_blk(master_sd, blk_cnt * 512, buf);
}
/********************************************************************************************************/
/*
 * SD ���豸����
 */
#include "vfs/vfs.h"
#include "vfs/device.h"
#include <sys/stat.h>
#include <sys/mount.h>

/*
 * �� SD ���豸
 */
static int sdcard_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    return 0;
}

/*
 * ���� SD ���豸
 */
static int sdcard_ioctl(void *ctx, file_t *file, int cmd, void *arg)
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
 * �ر� SD ���豸
 */
static int sdcard_close(void *ctx, file_t *file)
{
    return 0;
}

/*
 * SD ���豸����
 */
driver_t sdcard_drv = {
        .name  = "sdcard",
        .open  = sdcard_open,
        .ioctl = sdcard_ioctl,
        .close = sdcard_close,
};

/*
 * ���� SD ���豸
 */
int sdcard_create(const char *dev_name, sdio_t *sdio)
{
    sdcard_t *sd;
    int       ret;

    if (dev_name   == NULL ||
        sdio       == NULL) {
        return -1;
    }

    master_sd = sd = kmalloc(sizeof(sdcard_t));
    if (sd == NULL) {
        return -1;
    }

    ret = sdcard_init(sd, sdio);
    if (ret < 0) {
        kfree(sd);
        master_sd = NULL;
        return ret;
    }

    ret = device_create(dev_name, "sdcard", sd);
    if (ret < 0) {
        kfree(sd);
        master_sd = NULL;
        return ret;
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
