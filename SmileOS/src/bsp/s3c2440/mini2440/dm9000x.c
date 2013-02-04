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
** File name:               dm9000x.c
** Last modified Date:      2012-3-3
** Last Version:            1.0.0
** Descriptions:            DM9000X ��̫��оƬ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-3
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
#include "kern/ipc.h"
#include <unistd.h>

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

#include "dm9000x.h"

#include "../s3c2440.h"
/*********************************************************************************************************
** ����
*********************************************************************************************************/
/*
 * Define those to better describe your network interface.
 */
#define IFNAME0                 'e'
#define IFNAME1                 'n'

#define HOSTNAME                "lwIP"

/*
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
    struct eth_addr  *mac_addr;
    u8_t              tx_pkt_cnt;
    u16_t             queue_pkt_len;
    sys_mutex_t       lock;
    sys_sem_t         tx_sync;
    void (*outblk)(const void *data, int len);
    void (*inblk)(void *data, int len);
    void (*dummy_inblk)(int len);
    void (*rx_status)(u16_t *status, u16_t *len);
};

/*
 * �弶
 */
#define DM9000_BASE             0x20000300
#define DM9000_IO               (DM9000_BASE)
#define DM9000_DATA             (DM9000_BASE + 4)

#define __le16_to_cpu(x)        ((u16_t)(x))

#define DM9000_outb(d,r)        writeb(d, (volatile u8_t  *)(r))
#define DM9000_outw(d,r)        writew(d, (volatile u16_t *)(r))
#define DM9000_outl(d,r)        writel(d, (volatile u32_t *)(r))
#define DM9000_inb(r)           readb((volatile u8_t  *)(r))
#define DM9000_inw(r)           readw((volatile u16_t *)(r))
#define DM9000_inl(r)           readl((volatile u32_t *)(r))
/*********************************************************************************************************
** Function name:           dm9000_outblk_8bit
** Descriptions:            ����� 8 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_outblk_8bit(const u8_t *data, int len)
{
    int  i;

    for (i = 0; i < len; i++) {
        DM9000_outb(data[i], DM9000_DATA);
    }
}
/*********************************************************************************************************
** Function name:           dm9000_outblk_16bit
** Descriptions:            ����� 16 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_outblk_16bit(const u16_t *data, int len)
{
    int  i;

    len = (len + 1) / 2;

    for (i = 0; i < len; i++) {
        DM9000_outw(data[i], DM9000_DATA);
    }
}
/*********************************************************************************************************
** Function name:           dm9000_outblk_32bit
** Descriptions:            ����� 32 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_outblk_32bit(const u32_t *data, int len)
{
    int  i;

    len = (len + 3) / 4;

    for (i = 0; i < len; i++) {
        DM9000_outl(data[i], DM9000_DATA);
    }
}
/*********************************************************************************************************
** Function name:           dm9000_inblk_8bit
** Descriptions:            ����� 8 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_inblk_8bit(u8_t *data, int len)
{
    int  i;

    for (i = 0; i < len; i++) {
        data[i] = DM9000_inb(DM9000_DATA);
    }
}
/*********************************************************************************************************
** Function name:           dm9000_dummy_inblk_8bit
** Descriptions:            ����� 8 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_dummy_inblk_8bit(int len)
{
    int  i;

    for (i = 0; i < len; i++) {
        DM9000_inb(DM9000_DATA);
    }
}
/*********************************************************************************************************
** Function name:           dm9000_inblk_16bit
** Descriptions:            ����� 16 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_inblk_16bit(u16_t *data, int len)
{
    int  i;

    len = (len + 1) / 2;

    for (i = 0; i < len; i++) {
        data[i] = DM9000_inw(DM9000_DATA);
    }
}
/*********************************************************************************************************
** Function name:           dm9000_dummy_inblk_16bit
** Descriptions:            ����� 16 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_dummy_inblk_16bit(int len)
{
    int  i;

    len = (len + 1) / 2;

    for (i = 0; i < len; i++) {
        DM9000_inw(DM9000_DATA);
    }
}
/*********************************************************************************************************
** Function name:           dm9000_inblk_32bit
** Descriptions:            ����� 32 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_inblk_32bit(u32_t *data, int len)
{
    int  i;

    len = (len + 3) / 4;

    for (i = 0; i < len; i++) {
        data[i] = DM9000_inl(DM9000_DATA);
    }
}
/*********************************************************************************************************
** Function name:           dm9000_dummy_inblk_32bit
** Descriptions:            ����� 32 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_dummy_inblk_32bit(int len)
{
    int  i;

    len = (len + 3) / 4;

    for (i = 0; i < len; i++) {
        DM9000_inl(DM9000_DATA);
    }
}
/*********************************************************************************************************
** Function name:           dm9000_rx_status_32bit
** Descriptions:            ��ý���״̬ 32 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_rx_status_32bit(u16_t *status, u16_t *len)
{
    u32_t  temp;

    DM9000_outb(DM9000_MRCMD, DM9000_IO);

    temp    = DM9000_inl(DM9000_DATA);
    *status = __le16_to_cpu(temp);
    *len    = __le16_to_cpu(temp >> 16);
}
/*********************************************************************************************************
** Function name:           dm9000_rx_status_16bit
** Descriptions:            ��ý���״̬ 16 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_rx_status_16bit(u16_t *status, u16_t *len)
{
    DM9000_outb(DM9000_MRCMD, DM9000_IO);

    *status = __le16_to_cpu(DM9000_inw(DM9000_DATA));
    *len    = __le16_to_cpu(DM9000_inw(DM9000_DATA));
}
/*********************************************************************************************************
** Function name:           dm9000_rx_status_8bit
** Descriptions:            ��ý���״̬ 8 λģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_rx_status_8bit(u16_t *status, u16_t *len)
{
    DM9000_outb(DM9000_MRCMD, DM9000_IO);

    *status = __le16_to_cpu(DM9000_inb(DM9000_DATA) + (DM9000_inb(DM9000_DATA) << 8));
    *len    = __le16_to_cpu(DM9000_inb(DM9000_DATA) + (DM9000_inb(DM9000_DATA) << 8));
}
/*********************************************************************************************************
** Function name:           dm9000_io_read
** Descriptions:            �� IO �˿ڶ�һ���ֽ�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static u8_t dm9000_io_read(u8_t reg)
{
    DM9000_outb(reg, DM9000_IO);
    return DM9000_inb(DM9000_DATA);
}
/*********************************************************************************************************
** Function name:           dm9000_io_write
** Descriptions:            дһ���ֽڵ� IO �˿�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_io_write(u8_t reg, u8_t value)
{
    DM9000_outb(reg, DM9000_IO);
    DM9000_outb(value, DM9000_DATA);
}
/*********************************************************************************************************
** Function name:           dm9000_phy_read
** Descriptions:            Read a word from phyxcer
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static u16_t dm9000_phy_read(u8_t reg)
{
    u16_t  value;

    /*
     * Fill the phyxcer register into REG_0C
     */
    dm9000_io_write(DM9000_EPAR, DM9000_PHY | reg);

    dm9000_io_write(DM9000_EPCR, 0x0C);                                 /*  Issue phyxcer read command  */
    usleep(100);                                                        /*  Wait read complete          */
    dm9000_io_write(DM9000_EPCR, 0x00);                                 /*  Clear phyxcer read command  */

    value = (dm9000_io_read(DM9000_EPDRH) << 8) |
             dm9000_io_read(DM9000_EPDRL);

    return value;
}
/*********************************************************************************************************
** Function name:           dm9000_phy_write
** Descriptions:            Write a word to phyxcer
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if 0                                                                   /*  û�õ�, �������ʱ����      */
static void dm9000_phy_write(u8_t reg, u16_t value)
{
    /*
     * Fill the phyxcer register into REG_0C
     */
    dm9000_io_write(DM9000_EPAR, DM9000_PHY | reg);

    /*
     * Fill the written data into REG_0D & REG_0E
     */
    dm9000_io_write(DM9000_EPDRL, (value & 0xFF));
    dm9000_io_write(DM9000_EPDRH, ((value >> 8) & 0xFF));

    dm9000_io_write(DM9000_EPCR, 0x0A);                                 /*  Issue phyxcer write command */
    usleep(500);                                                        /*  Wait write complete         */
    dm9000_io_write(DM9000_EPCR, 0x00);                                 /*  Clear phyxcer write command */
}
#endif
/*********************************************************************************************************
** Function name:           dm9000_probe
** Descriptions:            ̽�� DM9000 оƬ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static err_t dm9000_probe(void)
{
    u32_t  id;

    id  = dm9000_io_read(DM9000_VIDL);
    id |= dm9000_io_read(DM9000_VIDH) << 8;
    id |= dm9000_io_read(DM9000_PIDL) << 16;
    id |= dm9000_io_read(DM9000_PIDH) << 24;

    if (id == DM9000_ID) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_probe: found at i/o: 0x%x, id: 0x%x\n", DM9000_BASE, id));
        return 0;
    } else {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_probe: no found at i/o: 0x%x, id: 0x%x\n", DM9000_BASE, id));
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           dm9000_reset
** Descriptions:            ��λ DM9000 оƬ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static err_t dm9000_reset(void)
{
    LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_reset: resetting\n"));

    /*
     * Reset DM9000,
     * see DM9000 Application Notes V1.22 Jun 11, 2004 page 29
     */

    /*
     * DEBUG: Make all GPIO0 outputs, all others inputs
     */
    dm9000_io_write(DM9000_GPCR, GPCR_GPIO0_OUT);

    /*
     * Step 1: Power internal PHY by writing 0 to GPIO0 pin
     */
    dm9000_io_write(DM9000_GPR, 0x00);

    /*
     * Step 2: Software reset
     */
    dm9000_io_write(DM9000_NCR, (NCR_LBK_INT_MAC | NCR_RST));

    do {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_reset: 1st reset\n"));
        usleep(25);                                                     /*  Wait at least 20 us         */
    } while (dm9000_io_read(DM9000_NCR) & 1);

    dm9000_io_write(DM9000_NCR, 0x00);
    dm9000_io_write(DM9000_NCR, (NCR_LBK_INT_MAC | NCR_RST));           /*  Issue a second reset        */

    do {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_reset: 2st reset\n"));
        usleep(25);                                                     /*  Wait at least 20 us         */
    } while (dm9000_io_read(DM9000_NCR) & 1);

    /*
     * Check whether the ethernet controller is present
     */
    if ((dm9000_io_read(DM9000_PIDL) != 0x00) ||
        (dm9000_io_read(DM9000_PIDH) != 0x90)) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_reset: no respond\n"));
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           dm9000_init
** Descriptions:            ��ʼ�� DM9000 оƬ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static err_t dm9000_init(struct netif *netif)
{
    struct ethernetif  *ethif = netif->state;
    u8_t                mode, oft, link;
    err_t               ret;
    int                 i;

    /*
     * ��λ DM9000 оƬ
     */
    ret = dm9000_reset();
    if (ret < 0) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: reset error\n"));
        return -1;
    }

    /*
     * ̽�� DM9000 оƬ
     */
    ret = dm9000_probe();
    if (ret < 0) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: probe error\n"));
        return -1;
    }

    /*
     * Auto-detect 8/16/32 bit mode, ISR Bit 6+7 indicate bus width
     */
    mode = dm9000_io_read(DM9000_ISR) >> 6;
    switch (mode) {
    case 0x00:  /* 16-bit mode */
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: running in 16 bit mode\n"));
        ethif->outblk      = (void (*)(const void *, int))dm9000_outblk_16bit;
        ethif->inblk       = (void (*)(void *, int))dm9000_inblk_16bit;
        ethif->dummy_inblk = dm9000_dummy_inblk_16bit;
        ethif->rx_status   = dm9000_rx_status_16bit;
        break;

    case 0x01:  /* 32-bit mode */
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: running in 32 bit mode\n"));
        ethif->outblk      = (void (*)(const void *, int))dm9000_outblk_32bit;
        ethif->inblk       = (void (*)(void *, int))dm9000_inblk_32bit;
        ethif->dummy_inblk = dm9000_dummy_inblk_32bit;
        ethif->rx_status   = dm9000_rx_status_32bit;
        break;

    case 0x02:  /* 8 bit mode */
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: running in 8 bit mode\n"));
        ethif->outblk      = (void (*)(const void *, int))dm9000_outblk_8bit;
        ethif->inblk       = (void (*)(void *, int))dm9000_inblk_8bit;
        ethif->dummy_inblk = dm9000_dummy_inblk_8bit;
        ethif->rx_status   = dm9000_rx_status_8bit;
        break;

    default:    /* Assume 8 bit mode, will probably not work anyway */
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: running in 0x%02x bit mode\n", mode));
        ethif->outblk      = (void (*)(const void *, int))dm9000_outblk_8bit;
        ethif->inblk       = (void (*)(void *, int))dm9000_inblk_8bit;
        ethif->dummy_inblk = dm9000_dummy_inblk_8bit;
        ethif->rx_status   = dm9000_rx_status_8bit;
        break;
    }

    /*
     * Program operating register, only internal phy supported
     */
    dm9000_io_write(DM9000_NCR, 0x00);

    /*
     * TX Polling clear
     */
    dm9000_io_write(DM9000_TCR, 0x00);

    /*
     * Less 3Kb, 200us
     */
    dm9000_io_write(DM9000_BPTR, BPTR_BPHW(3) | BPTR_JPT_600US);

    /*
     * Flow Control: High/Low Water
     */
    dm9000_io_write(DM9000_FCTR, FCTR_HWOT(3) | FCTR_LWOT(8));

    /*
     * TODO: This looks strange! Flow Control
     */
    dm9000_io_write(DM9000_FCR, 0x00);

    /*
     * Special Mode
     */
    dm9000_io_write(DM9000_SMCR, 0x00);

    /*
     * Clear TX status
     */
    dm9000_io_write(DM9000_NSR, NSR_WAKEST | NSR_TX2END | NSR_TX1END);

    /*
     * Clear interrupt status
     */
    dm9000_io_write(DM9000_ISR, ISR_ROOS | ISR_ROS | ISR_PTS | ISR_PRS);

    /*
     * Fill device MAC address registers
     */
    for (i = 0, oft = DM9000_PAR; i < 6; i++, oft++) {
        dm9000_io_write(oft, ethif->mac_addr->addr[i]);
    }

    for (i = 0, oft = DM9000_MAR; i < 8; i++, oft++) {
        dm9000_io_write(oft, 0xFF);
    }

    /*
     * Read back MAC address
     */
    LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: MAC="));
    LWIP_DEBUGF(NETIF_DEBUG, ("%02x:", dm9000_io_read(DM9000_PAR + 0)));
    LWIP_DEBUGF(NETIF_DEBUG, ("%02x:", dm9000_io_read(DM9000_PAR + 1)));
    LWIP_DEBUGF(NETIF_DEBUG, ("%02x:", dm9000_io_read(DM9000_PAR + 2)));
    LWIP_DEBUGF(NETIF_DEBUG, ("%02x:", dm9000_io_read(DM9000_PAR + 3)));
    LWIP_DEBUGF(NETIF_DEBUG, ("%02x:", dm9000_io_read(DM9000_PAR + 4)));
    LWIP_DEBUGF(NETIF_DEBUG, ("%02x\n",dm9000_io_read(DM9000_PAR + 5)));

    /*
     * Activate DM9000
     */
    /*
     * RX enable
     */
    dm9000_io_write(DM9000_RCR, RCR_DIS_LONG | RCR_DIS_CRC | RCR_RXEN);

    /*
     * Enable TX/RX interrupt mask
     */
    dm9000_io_write(DM9000_IMR, IMR_PAR);

    i = 0;
    while (!(dm9000_phy_read(1) & 0x20)) {                              /*  autonegation complete bit   */
        usleep(1000);
        i++;
        if (i == 100000) {
            LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: can not establish link\n"));
            return 0;
        }
    }

    link = dm9000_phy_read(17) >> 12;                                   /*  see what we've got          */
    switch (link) {
    case 1:
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: operating at 10M half duplex mode\n"));
        break;

    case 2:
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: operating at 10M full duplex mode\n"));
        break;

    case 4:
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: operating at 100M half duplex mode\n"));
        break;

    case 8:
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: operating at 100M full duplex mode\n"));
        break;

    default:
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: operating at unknown 0x%02x mode\n", link));
        break;
    }

    /*
     * Enable TX/RX interrupt mask
     */
    dm9000_io_write(DM9000_IMR, IMR_PAR | IMR_ROOM | IMR_ROM | IMR_PTM | IMR_PRM);

    return 0;
}
/*********************************************************************************************************
** Function name:           ethernetif_input
** Descriptions:            ��̫������ӿ���������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void ethernetif_input(struct netif *netif, struct pbuf *p)
{
    struct eth_hdr  *ethhdr;
    err_t            err;

    /*
     * No packet could be read, silently ignore this
     */
    if (p == NULL) {
        return;
    }

    /*
     * Points to packet payload, which starts with an Ethernet header
     */
    ethhdr = p->payload;
    if (ethhdr == NULL) {
        return;
    }

    switch (htons(ethhdr->type)) {
    /*
     * IP or ARP packet?
     */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
#if PPPOE_SUPPORT
    /*
     * PPPoE packet?
     */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
        /*
         * Full packet send to tcpip_thread to process
         */
        err = netif->input(p, netif);
        if (err != ERR_OK) {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error, err = %d\n", err));
            pbuf_free(p);
            p = NULL;
        }
        break;

    default:
        pbuf_free(p);
        p = NULL;
        break;
    }
}
/*********************************************************************************************************
** Function name:           dm9000_recv
** Descriptions:            DM9000 ��������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_recv(struct netif *netif)
{
    struct ethernetif  *ethif = netif->state;
    struct pbuf        *p, *q;
    u8_t                reg;
    u8_t                rx_byte;
    u16_t               rx_len;
    u16_t               rx_status;
    int                 good_pkt;

    sys_mutex_lock(&ethif->lock);

    do {
        dm9000_io_read(DM9000_MRCMDX);
        rx_byte = DM9000_inb(DM9000_DATA) & 0x03;

        if (rx_byte > DM9000_PKT_RDY) {
            LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx error\n"));
#if 0
            dm9000_io_write(DM9000_RCR, 0x00);                          /*  Stop Device                 */
            dm9000_io_write(DM9000_ISR, IMR_PAR);                       /*  Stop INT request            */
#endif
            goto __exit;
        }

        if (rx_byte != DM9000_PKT_RDY) {
            goto __exit;
        }

        good_pkt = TRUE;

        (ethif->rx_status)(&rx_status, &rx_len);

        if (rx_len < 0x40) {
            LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx len error, rx len < 0x40\n"));
            good_pkt = FALSE;
        }

        if (rx_len > DM9000_PKT_MAX) {
            LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx len error, rx len > %d\n", DM9000_PKT_MAX));
            good_pkt = FALSE;
        }

        rx_status >>= 8;

        if (rx_status & (RSR_FOE | RSR_CE | RSR_AE |
                         RSR_PLE | RSR_RWTO |
                         RSR_LCS | RSR_RF)) {

            if (rx_status & RSR_FOE) {
                LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx fifo error\n"));
            }

            if (rx_status & RSR_CE) {
                LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx crc error\n"));
            }

            if (rx_status & RSR_RF) {
                LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx len error\n"));
            }

            good_pkt = FALSE;
        }


        if (good_pkt) {
#if ETH_PAD_SIZE
            rx_len += ETH_PAD_SIZE;
#endif

            if ((p = pbuf_alloc(PBUF_RAW, rx_len, PBUF_POOL)) != NULL) {
#if ETH_PAD_SIZE
                pbuf_header(p, -ETH_PAD_SIZE);
#endif

                for (q = p; q != NULL; q = q->next) {
                    (ethif->inblk)(q->payload, q->len);
                }

#if ETH_PAD_SIZE
                pbuf_header(p, ETH_PAD_SIZE);
#endif

                LINK_STATS_INC(link.recv);

                ethernetif_input(netif, p);
            } else {
#if ETH_PAD_SIZE
                rx_len -= ETH_PAD_SIZE;
#endif
                ethif->dummy_inblk(rx_len);

                LINK_STATS_INC(link.drop);
            }
        } else {
            ethif->dummy_inblk(rx_len);

            LINK_STATS_INC(link.drop);
        }
    } while (rx_byte == DM9000_PKT_RDY);

    __exit:
    /*
     * �򿪽����ж�
     */
    reg  = dm9000_io_read(DM9000_IMR);
    reg |= IMR_PRM;
    dm9000_io_write(DM9000_IMR, reg);

    sys_mutex_unlock(&ethif->lock);
}
/*********************************************************************************************************
** Function name:           low_level_output
** Descriptions:            DM9000 ��������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    struct ethernetif  *ethif = netif->state;
    struct pbuf        *q;
    u16_t               tx_len;
    u8_t                reg;

    sys_mutex_lock(&ethif->lock);

    /*
     * �رշ����ж�
     */
    reg  = dm9000_io_read(DM9000_IMR);
    reg &= ~(IMR_PTM);
    dm9000_io_write(DM9000_IMR, reg);

    while (ethif->tx_pkt_cnt > 1) {
        /*
         * �򿪷����ж�
         */
        reg  = dm9000_io_read(DM9000_IMR);
        reg |= IMR_PTM;
        dm9000_io_write(DM9000_IMR, reg);

        sys_mutex_unlock(&ethif->lock);

        sys_arch_sem_wait(&ethif->tx_sync, 10);

        sys_mutex_lock(&ethif->lock);

        /*
         * �رշ����ж�
         */
        reg  = dm9000_io_read(DM9000_IMR);
        reg &= ~(IMR_PTM);
        dm9000_io_write(DM9000_IMR, reg);
    }

    DM9000_outb(DM9000_MWCMD, DM9000_IO);

    tx_len  = p->tot_len;
#if ETH_PAD_SIZE
    tx_len -= ETH_PAD_SIZE;
#endif

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE);
#endif

    for (q = p; q != NULL; q = q->next) {
        (ethif->outblk)(q->payload, q->len);
    }

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE);
#endif

    /*
     * �رշ����ж�
     */
    reg  = dm9000_io_read(DM9000_IMR);
    reg &= ~(IMR_PTM);
    dm9000_io_write(DM9000_IMR, reg);

    ethif->tx_pkt_cnt++;

    if (ethif->tx_pkt_cnt == 1) {
        dm9000_io_write(DM9000_TXPLL, (tx_len & 0xFF));
        dm9000_io_write(DM9000_TXPLH, (tx_len >> 8) & 0xFF);
        dm9000_io_write(DM9000_TCR, TCR_TXREQ);
    } else {
        ethif->queue_pkt_len = tx_len;
    }

    /*
     * �򿪷����ж�
     */
    reg  = dm9000_io_read(DM9000_IMR);
    reg |= IMR_PTM;
    dm9000_io_write(DM9000_IMR, reg);

    sys_mutex_unlock(&ethif->lock);

    return ERR_OK;
}
/*********************************************************************************************************
** Function name:           dm9000_send_done
** Descriptions:            DM9000 �������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_send_done(struct netif *netif)
{
    struct ethernetif  *ethif = netif->state;
    u8_t                tx_status;

    tx_status = dm9000_io_read(DM9000_NSR);                             /*  Get TX status               */
    if (tx_status & (NSR_TX2END | NSR_TX1END)) {
        /*
         * One packet sent complete
         */
        LINK_STATS_INC(link.xmit);

        ethif->tx_pkt_cnt--;

        /*
         * Queue packet check & send
         */
        if (ethif->tx_pkt_cnt > 0) {
            dm9000_io_write(DM9000_TXPLL, (ethif->queue_pkt_len & 0xFF));
            dm9000_io_write(DM9000_TXPLH, (ethif->queue_pkt_len >> 8) & 0xFF);
            dm9000_io_write(DM9000_TCR, TCR_TXREQ);
        }

        sem_sync(&ethif->tx_sync);
    }
}
/*********************************************************************************************************
** Function name:           dm9000_isr
** Descriptions:            DM9000 �ж�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void dm9000_isr(struct netif *netif)
{
    u8_t  int_status;
    u8_t  last_io;
    u8_t  reg;

    /*
     * Save previous io register
     */
    last_io = DM9000_inb(DM9000_IO);

    /*
     * Disable all interrupts
     */
    reg = dm9000_io_read(DM9000_IMR);
    dm9000_io_write(DM9000_IMR, IMR_PAR);

    /*
     * Get DM9000 interrupt status
     */
    int_status = dm9000_io_read(DM9000_ISR);                            /*  Get ISR status              */
    dm9000_io_write(DM9000_ISR, int_status);                            /*  Clear ISR status            */

    if (int_status & ISR_ROS) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_isr: rx overflow\n"));
    }

    if (int_status & ISR_ROOS) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_isr: rx overflow counter overflow\n"));
    }

    /*
     * Received the coming packet
     */
    if (int_status & ISR_PRS) {
        workqueue_add(NULL, (void (*)(void *))dm9000_recv, netif);
        /*
         * �رս����ж�
         */
        reg &= ~(IMR_PRM);
    }

    /*
     * Transmit Interrupt check
     */
    if (int_status & ISR_PTS) {
        dm9000_send_done(netif);
    }

    /*
     * Re-enable interrupt mask
     */
    dm9000_io_write(DM9000_IMR, reg);

    /*
     * Restore previous io register
     */
    DM9000_outb(last_io, DM9000_IO);
}
/*********************************************************************************************************
** Function name:           eint47_isr
** Descriptions:            EINT4-7 �жϷ�����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static int eint47_isr(intno_t interrupt, void *arg)
{
    uint32_t  sub_interrupt;

    sub_interrupt = EINTPEND;
    EINTPEND      = sub_interrupt;

    if (sub_interrupt & (1 << 7)) {
       dm9000_isr(arg);
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           low_level_init
** Descriptions:            ��ʼ����̫������ӿ�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static void low_level_init(struct netif *netif)
{
    struct ethernetif  *ethif = netif->state;
    err_t               ret;

    /*
     * Set MAC hardware address length
     */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /*
     * Set MAC hardware address
     */
    netif->hwaddr[0] = 0x08;
    netif->hwaddr[1] = 0x08;
    netif->hwaddr[2] = 0x3E;
    netif->hwaddr[3] = 0x26;
    netif->hwaddr[4] = 0x0A;
    netif->hwaddr[5] = 0x5B;

    /*
     * Set Maximum transfer unit
     */
    netif->mtu = 1500;

    /*
     * Set Device capabilities
     */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP | NETIF_FLAG_ETHERNET;

/********************************************************************************************************/
    ethif->queue_pkt_len = 0;
    ethif->tx_pkt_cnt    = 0;

    /*
     * mini2440 �������ϵ� DM9000 оƬ���� BANK4
     */
    /*
     * ���� GPA15 Ϊ nGCS4
     */
    GPACON  |= 1 << 15;

    /*
     * ���� BANK4 ���߿��Ϊ 16 λ, ʹ�ܵȴ�
     */
    BWSCON   = (BWSCON & (~(0x07 << 16))) | (0x05 << 16);

    /*
     * ���� BANK4 �ķ���ʱ�����ʺ� DM9000 оƬ
     */
    BANKCON4 = (1 << 13) | (1 << 11) | (6 << 8) | (1 << 6) | (1 << 4) | (0 << 2) | (0 << 0);

    /*
     * ��ʼ�� DM9000 оƬ
     */
    ret = dm9000_init(netif);
    if (ret != 0) {
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_init: init dm9000 error\n"));
        return;
    }

    sys_mutex_new(&ethif->lock);

    sys_sem_new(&ethif->tx_sync, 0);

    /*
     * ��װ EINT4-7 �жϷ������, ��ʹ�� EINT4-7 �ж�
     */
    interrupt_install(INTEINT4_7, eint47_isr, NULL, (void *)netif);

    interrupt_unmask(INTEINT4_7);

    /*
     * ���� GPF7 Ϊ EINT7
     */
    GPFCON = (GPFCON & (~(0x03 << 14))) | (0x02 << 14);

    /*
     * �ر� GPF7 ����������
     */
    GPFUP = GPFUP | (1 << 7);

    /*
     * ���� EINT7 �ߵ�ƽ�ж�
     */
    EXTINT0 = (EXTINT0 & (~(0x07 << 28))) | (0x01 << 28);

    /*
     * ʹ�� EINT7
     */
    EINTMASK = EINTMASK & (~(1 << 7));
/********************************************************************************************************/
}
/*********************************************************************************************************
** Function name:           ethernetif_init
** Descriptions:            ��ʼ����̫������ӿ�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
** Created by:              JiaoJinXing
** Created Date:            2012/11/30
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
err_t ethernetif_init(struct netif *netif)
{
    struct ethernetif  *ethif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    ethif = (struct ethernetif *)mem_malloc(sizeof(struct ethernetif));
    if (ethif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
        return ERR_MEM;
    }

#if LWIP_NETIF_HOSTNAME
    /*
     * Initialize interface hostname
     */
    netif->hostname = HOSTNAME;
#endif /* LWIP_NETIF_HOSTNAME */

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state   = ethif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;

    /*
     * We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...)
     */
    netif->output     = etharp_output;
    netif->linkoutput = low_level_output;

    ethif->mac_addr = (struct eth_addr *)&(netif->hwaddr[0]);

    /*
     * Initialize the hardware
     */
    low_level_init(netif);

    return ERR_OK;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
