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
** File name:               dm9000x.c
** Last modified Date:      2012-3-3
** Last Version:            1.0.0
** Descriptions:            DM9000X 以太网芯片驱动
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-3
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

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

#undef  NETIF_DEBUG
#define NETIF_DEBUG             LWIP_DBG_ON

/*
 * Define those to better describe your network interface.
 */
#define IFNAME0                 'e'
#define IFNAME1                 'n'

/*
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
    struct eth_addr *ethaddr;
    u8_t             tx_packet_nr;
    u16_t            tx_packet_len;
    void           (*outblk)(const void *data, int len);
    void           (*inblk)(void *data, int len);
    void           (*rx_status)(u16_t *status, u16_t *len);
};

#define CONFIG_DRIVER_DM9000    1
#include "dm9000x.h"

/*
 * 板级
 */
#define CONFIG_DM9000_BASE      0x20000300
#define DM9000_IO               (CONFIG_DM9000_BASE)
#define DM9000_DATA             (CONFIG_DM9000_BASE + 4)

/*
 * 系统级
 */
extern void usleep(unsigned int us);
#define udelay                  usleep

/*
 * ARCH IO 级
 */
#define writeb(d, r)            (*(volatile u8_t  *)r) = (d)
#define writew(d, r)            (*(volatile u16_t *)r) = (d)
#define writel(d, r)            (*(volatile u32_t *)r) = (d)
#define readb(r)                (*(volatile u8_t  *)r)
#define readw(r)                (*(volatile u16_t *)r)
#define readl(r)                (*(volatile u32_t *)r)
#define __le16_to_cpu(x)        ((u16_t)(x))

#define DM9000_outb(d,r)        writeb(d, (volatile u8_t  *)(r))
#define DM9000_outw(d,r)        writew(d, (volatile u16_t *)(r))
#define DM9000_outl(d,r)        writel(d, (volatile u32_t *)(r))
#define DM9000_inb(r)           readb((volatile u8_t  *)(r))
#define DM9000_inw(r)           readw((volatile u16_t *)(r))
#define DM9000_inl(r)           readl((volatile u32_t *)(r))

/*
 * 输出块 8 位模式
 */
static void
dm9000_outblk_8bit(const void *data, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        DM9000_outb(((const u8_t *)data)[i], DM9000_DATA);
    }
}

/*
 * 输出块 16 位模式
 */
static void
dm9000_outblk_16bit(const void *data, int len)
{
    int i;

    len = (len + 1) / 2;

    for (i = 0; i < len; i++) {
        DM9000_outw(((const u16_t *)data)[i], DM9000_DATA);
    }
}

/*
 * 输出块 32 位模式
 */
static void
dm9000_outblk_32bit(const void *data, int len)
{
    int i;

    len = (len + 3) / 4;

    for (i = 0; i < len; i++) {
        DM9000_outl(((const u32_t *)data)[i], DM9000_DATA);
    }
}

/*
 * 输入块 8 位模式
 */
static void
dm9000_inblk_8bit(void *data, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        ((u8_t *)data)[i] = DM9000_inb(DM9000_DATA);
    }
}

/*
 * 输入块 16 位模式
 */
static void
dm9000_inblk_16bit(void *data, int len)
{
    int i;

    len = (len + 1) / 2;

    for (i = 0; i < len; i++) {
        ((u16_t *)data)[i] = DM9000_inw(DM9000_DATA);
    }
}

/*
 * 输入块 32 位模式
 */
static void
dm9000_inblk_32bit(void *data, int len)
{
    int i;

    len = (len + 3) / 4;

    for (i = 0; i < len; i++) {
        ((u32_t *)data)[i] = DM9000_inl(DM9000_DATA);
    }
}

/*
 * 获得接收状态 32 位模式
 */
static void
dm9000_rx_status_32bit(u16_t *status, u16_t *len)
{
    u32_t temp;

    DM9000_outb(DM9000_MRCMD, DM9000_IO);

    temp    = DM9000_inl(DM9000_DATA);
    *status = __le16_to_cpu(temp);
    *len    = __le16_to_cpu(temp >> 16);
}

/*
 * 获得接收状态 16 位模式
 */
static void
dm9000_rx_status_16bit(u16_t *status, u16_t *len)
{
    DM9000_outb(DM9000_MRCMD, DM9000_IO);

    *status = __le16_to_cpu(DM9000_inw(DM9000_DATA));
    *len    = __le16_to_cpu(DM9000_inw(DM9000_DATA));
}

/*
 * 获得接收状态 8 位模式
 */
static void
dm9000_rx_status_8bit(u16_t *status, u16_t *len)
{
    DM9000_outb(DM9000_MRCMD, DM9000_IO);

    *status = __le16_to_cpu(DM9000_inb(DM9000_DATA) + (DM9000_inb(DM9000_DATA) << 8));
    *len    = __le16_to_cpu(DM9000_inb(DM9000_DATA) + (DM9000_inb(DM9000_DATA) << 8));
}

/*
 * 从 io 端口读一个字节
 */
static u8_t
dm9000_io_read(u8_t reg)
{
    DM9000_outb(reg, DM9000_IO);
    return DM9000_inb(DM9000_DATA);
}

/*
 * 从 io 端口写一个字节
 */
static void
dm9000_io_write(u8_t reg, u8_t value)
{
    DM9000_outb(reg, DM9000_IO);
    DM9000_outb(value, DM9000_DATA);
}

/*
 * Read a word from phyxcer
 */
static u16_t
dm9000_phy_read(u8_t reg)
{
    u16_t value;

    /*
     * Fill the phyxcer register into REG_0C
     */
    dm9000_io_write(DM9000_EPAR, DM9000_PHY | reg);

    dm9000_io_write(DM9000_EPCR, 0x0C);                                 /*  Issue phyxcer read command  */
    udelay(100);                                                        /*  Wait read complete          */
    dm9000_io_write(DM9000_EPCR, 0x00);                                 /*  Clear phyxcer read command  */

    value = (dm9000_io_read(DM9000_EPDRH) << 8) | dm9000_io_read(DM9000_EPDRL);

    return value;
}

#if 0
/*
 * Write a word to phyxcer
 */
static void
dm9000_phy_write(u8_t reg, u16_t value)
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
    udelay(500);                                                        /*  Wait write complete         */
    dm9000_io_write(DM9000_EPCR, 0x00);                                 /*  Clear phyxcer write command */
}
#endif

/*
 * 探测 dm9000 芯片
 */
static err_t
dm9000_probe(void)
{
    u32_t id;

    id  = dm9000_io_read(DM9000_VIDL);
    id |= dm9000_io_read(DM9000_VIDH) << 8;
    id |= dm9000_io_read(DM9000_PIDL) << 16;
    id |= dm9000_io_read(DM9000_PIDH) << 24;

    if (id == DM9000_ID) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_probe: found at i/o: 0x%x, id: 0x%x\n", CONFIG_DM9000_BASE, id));
        return 0;
    } else {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_probe: no found at i/o: 0x%x, id: 0x%x\n", CONFIG_DM9000_BASE, id));
        return -1;
    }
}

/*
 * 复位 dm9000 芯片
 */
static err_t
dm9000_reset(void)
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
        udelay(25);                                                     /*  Wait at least 20 us         */
    } while (dm9000_io_read(DM9000_NCR) & 1);

    dm9000_io_write(DM9000_NCR, 0x00);
    dm9000_io_write(DM9000_NCR, (NCR_LBK_INT_MAC | NCR_RST));           /*  Issue a second reset        */

    do {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_reset: 2st reset\n"));
        udelay(25);                                                     /*  Wait at least 20 us         */
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

/*
 * 初始化 dm9000 芯片
 */
static err_t
dm9000_init(struct netif *netif)
{
    struct ethernetif *ethernetif = netif->state;
    u8_t mode, oft;
    u16_t link;
    err_t ret;
    int i;

    /*
     * 复位 dm9000 芯片
     */
    ret = dm9000_reset();
    if (ret < 0) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: reset error\n"));
        return -1;
    }

    /*
     * 探测 dm9000 芯片
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
        ethernetif->outblk    = dm9000_outblk_16bit;
        ethernetif->inblk     = dm9000_inblk_16bit;
        ethernetif->rx_status = dm9000_rx_status_16bit;
        break;

    case 0x01:  /* 32-bit mode */
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: running in 32 bit mode\n"));
        ethernetif->outblk    = dm9000_outblk_32bit;
        ethernetif->inblk     = dm9000_inblk_32bit;
        ethernetif->rx_status = dm9000_rx_status_32bit;
        break;

    case 0x02:  /* 8 bit mode */
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: running in 8 bit mode\n"));
        ethernetif->outblk    = dm9000_outblk_8bit;
        ethernetif->inblk     = dm9000_inblk_8bit;
        ethernetif->rx_status = dm9000_rx_status_8bit;
        break;

    default:    /* Assume 8 bit mode, will probably not work anyway */
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: running in 0x%x bit mode\n", mode));
        ethernetif->outblk    = dm9000_outblk_8bit;
        ethernetif->inblk     = dm9000_inblk_8bit;
        ethernetif->rx_status = dm9000_rx_status_8bit;
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
     * Flow Control : High/Low Water
     */
    dm9000_io_write(DM9000_FCTR, FCTR_HWOT(3) | FCTR_LWOT(8));

    /*
     * SH FIXME: This looks strange! Flow Control
     */
    dm9000_io_write(DM9000_FCR, 0x00);

    /*
     * Special Mode
     */
    dm9000_io_write(DM9000_SMCR, 0x00);

    /*
     * clear TX status
     */
    dm9000_io_write(DM9000_NSR, NSR_WAKEST | NSR_TX2END | NSR_TX1END);

    /*
     * Clear interrupt status
     */
    dm9000_io_write(DM9000_ISR, ISR_ROOS | ISR_ROS | ISR_PTS | ISR_PRS);

    /*
     * fill device MAC address registers
     */
    for (i = 0, oft = DM9000_PAR; i < 6; i++, oft++) {
        dm9000_io_write(oft, ethernetif->ethaddr->addr[i]);
    }

    for (i = 0, oft = DM9000_MAR; i < 8; i++, oft++) {
        dm9000_io_write(oft, 0xFF);
    }

    /*
     * read back mac, just to be sure
     */
    LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: MAC="));
    for (i = 0, oft = DM9000_PAR; i < 6; i++, oft++) {
        LWIP_DEBUGF(NETIF_DEBUG, ("%x:", dm9000_io_read(oft)));
    }
    LWIP_DEBUGF(NETIF_DEBUG, ("\n"));

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
        udelay(1000);
        i++;
        if (i == 10000) {
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
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_init: operating at unknown 0x%x mode\n", link));
        break;
    }

    /*
     * Enable TX/RX interrupt mask
     */
    dm9000_io_write(DM9000_IMR, IMR_PAR | IMR_PTM | IMR_PRM);

    return 0;
}

static sys_sem_t rx_sem, tx_sem;
static sys_thread_t rx_thread;
static sys_mutex_t dm9000_lock;

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *q;
    u16_t len = 0;

    sys_mutex_lock(&dm9000_lock);

    dm9000_io_write(DM9000_IMR, IMR_PAR);

    DM9000_outb(DM9000_MWCMD, DM9000_IO);

    for (q = p; q != NULL; q = q->next) {
        (ethernetif->outblk)(q->payload, q->len);
        len += q->len;
    }

    if (ethernetif->tx_packet_nr == 0) {
        ethernetif->tx_packet_nr++;

        dm9000_io_write(DM9000_TXPLL, (len & 0xFF));
        dm9000_io_write(DM9000_TXPLH, (len >> 8) & 0xFF);

        dm9000_io_write(DM9000_TCR, TCR_TXREQ);
    } else {
        ethernetif->tx_packet_nr++;
        ethernetif->tx_packet_len = len;
    }

    dm9000_io_write(DM9000_IMR, IMR_PAR | IMR_PTM | IMR_PRM);

    LINK_STATS_INC(link.xmit);

    sys_mutex_unlock(&dm9000_lock);

    sys_arch_sem_wait(&tx_sem, 0);

    return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
dm9000_recv(struct netif *netif)
{
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *p, *q;
    u8_t  byte;
    u16_t len;
    u16_t status;

    sys_mutex_lock(&dm9000_lock);

    again:

    dm9000_io_read(DM9000_MRCMDX);
    byte = DM9000_inb(DM9000_DATA) & 0x03;

    if (byte > DM9000_PKT_RDY) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx error, stop device\n"));
        dm9000_io_write(DM9000_RCR, 0x00);                              /*  Stop Device                 */
        dm9000_io_write(DM9000_ISR, 0x80);                              /*  Stop INT request            */
        sys_mutex_unlock(&dm9000_lock);
        return NULL;
    } else if (byte != DM9000_PKT_RDY) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx error, no data\n"));
        sys_mutex_unlock(&dm9000_lock);
        return NULL;
    }

    (ethernetif->rx_status)(&status, &len);

    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    if (p != NULL) {
        for (q = p; q != NULL; q = q->next) {
            (ethernetif->inblk)(q->payload, q->len);
        }
    } else {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: alloc pbuf error\n"));
    }

    if ((status & 0xBF00) || (len < 0x40) || (len > DM9000_PKT_MAX)) {
        if (status & 0x100) {
            LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx fifo error\n"));
        }

        if (status & 0x200) {
            LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx crc error\n"));
        }

        if (status & 0x8000) {
            LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx len error\n"));
        }

        if (len > DM9000_PKT_MAX) {
            LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_recv: rx len too long error\n"));
            dm9000_reset();
        }

        if (p != NULL) {
            pbuf_free(p);
            p = NULL;
        }
        goto again;
    } else {
        if (p != NULL) {
            LINK_STATS_INC(link.recv);
        }
        sys_mutex_unlock(&dm9000_lock);
        return p;
    }
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void
ethernetif_input(void *arg)
{
    struct netif *netif = (struct netif *)arg;
    struct ethernetif *ethernetif;
    struct eth_hdr *ethhdr;
    struct pbuf *p;

    ethernetif = netif->state;

    while (1) {
/********************************************************************************************************/
        /*
         * 等待接收信号量
         */
        if (sys_arch_sem_wait(&rx_sem, 0) == SYS_ARCH_TIMEOUT) {
            continue;
        }

        /*
         * 接收数据到 pbuf
         */
        p = dm9000_recv(netif);
/********************************************************************************************************/

        /*
         * no packet could be read, silently ignore this
         */
        if (p == NULL) {
            continue;
        }

        /*
         * points to packet payload, which starts with an Ethernet header
         */
        ethhdr = p->payload;

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
             * full packet send to tcpip_thread to process
             */
            if (netif->input(p, netif) != ERR_OK) {
                LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
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
}

/*
 * dm9000 中断
 */
static void
dm9000_isr(struct netif *netif)
{
    struct ethernetif *ethernetif = netif->state;
    u8_t int_status;
    u8_t last_io;

    last_io = DM9000_inb(DM9000_IO);

    dm9000_io_write(DM9000_IMR, IMR_PAR);

    int_status = dm9000_io_read(DM9000_ISR);

    dm9000_io_write(DM9000_ISR, int_status);

    if (int_status & ISR_ROS) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_isr: overflow\n"));
    }

    if (int_status & ISR_ROOS) {
        LWIP_DEBUGF(NETIF_DEBUG, ("dm9000_isr: overflow counter overflow\n"));
    }

    if (int_status & ISR_PRS) {
        sys_sem_signal(&rx_sem);
    }

    if (int_status & ISR_PTS) {
        int tx_status;

        tx_status = dm9000_io_read(DM9000_NSR);
        if (tx_status & (NSR_TX2END | NSR_TX1END)) {
            ethernetif->tx_packet_nr--;
            if (ethernetif->tx_packet_nr > 0) {
                dm9000_io_write(DM9000_TXPLL, (ethernetif->tx_packet_len & 0xFF));
                dm9000_io_write(DM9000_TXPLH, (ethernetif->tx_packet_len >> 8) & 0xFF);
                dm9000_io_write(DM9000_TCR, TCR_TXREQ);
            }
            sys_sem_signal(&tx_sem);
        }
    }

    dm9000_io_write(DM9000_IMR, IMR_PAR | IMR_PTM | IMR_PRM);

    DM9000_outb(last_io, DM9000_IO);
}

#include "s3c2440.h"
#include "s3c2440_int.h"

/*
 * EINT4-7 中断
 */
static int
eint47_isr(uint32_t interrupt, void *arg)
{
    uint32_t sub_interrupt;

    sub_interrupt = EINTPEND;

    if (sub_interrupt & (1 << 7)) {
       dm9000_isr(arg);
    }

    EINTPEND = sub_interrupt;

    return 0;
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
    err_t ret;

    /*
     * set MAC hardware address length
     */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /*
     * set MAC hardware address
     */
    netif->hwaddr[0] = 0x08;
    netif->hwaddr[1] = 0x08;
    netif->hwaddr[2] = 0x3E;
    netif->hwaddr[3] = 0x26;
    netif->hwaddr[4] = 0x0A;
    netif->hwaddr[5] = 0x5B;

    /*
     * maximum transfer unit
     */
    netif->mtu = 1500;

    /*
     * device capabilities
     */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP | NETIF_FLAG_ETHERNET;

/********************************************************************************************************/
    /*
     * 创建接收信号量
     */
    ret = sys_sem_new(&rx_sem, 0);
    if (ret != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_init: create rx sem error\n"));
        return;
    }

    /*
     * 创建发送信号量
     */
    ret = sys_sem_new(&tx_sem, 0);
    if (ret != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_init: create tx sem error\n"));
        return;
    }

    /*
     * 创建 dm9000 操作锁
     */
    ret = sys_mutex_new(&dm9000_lock);
    if (ret != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_init: create dm9000 lock error\n"));
        return;
    }

    /*
     * 创建接收线程
     */
    rx_thread = sys_thread_new("ethif_rx", ethernetif_input, (void *)netif, 1024 * 32, 10);
    if (rx_thread == -1) {
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_init: create rx thread error\n"));
        return;
    }

    /*
     * mini2440 开发板上的 DM9000 芯片挂在 BANK4
     */
    /*
     * 设置 GPA15 为 nGCS4
     */
    GPACON  |= 1 << 15;

    /*
     * 设置 BANK4 总线宽度为 16 位, 使能等待
     */
    BWSCON   = (BWSCON & (~(0x07 << 16))) | (0x05 << 16);

    /*
     * 设置 BANK4 的访问时序以适合 DM9000 芯片
     */
    BANKCON4 = (1 << 13) | (1 << 11) | (6 << 8) | (1 << 6) | (1 << 4) | (0 << 2) | (0 << 0);

    /*
     * 初始化 DM9000 芯片
     */
    ret = dm9000_init(netif);
    if (ret != 0) {
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_init: init dm9000 error\n"));
        return;
    }

    /*
     * 安装 EINT4-7 中断服务程序, 并使能 EINT4-7 中断
     */
    interrupt_install(INTEINT4_7, eint47_isr, NULL, (void *)netif);

    interrupt_umask(INTEINT4_7);

    /*
     * 设置 GPF7 为 EINT7
     */
    GPFCON = (GPFCON & (~(0x03 << 14))) | (0x02 << 14);

    /*
     * 关闭 GPF7 的上拉电阻
     */
    GPFUP = GPFUP | (1 << 7);

    /*
     * 设置 EINT7 高电平中断
     */
    EXTINT0 = (EXTINT0 & (~(0x07 << 28))) | (0x01 << 28);

    /*
     * 使能 EINT7
     */
    EINTMASK = EINTMASK & (~(1 << 7));
/********************************************************************************************************/
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
    struct ethernetif *ethernetif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    ethernetif = mem_malloc(sizeof(struct ethernetif));
    if (ethernetif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
        return ERR_MEM;
    }

#if LWIP_NETIF_HOSTNAME
    /*
     * Initialize interface hostname
     */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state   = ethernetif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;

    /*
     * We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...)
     */
    netif->output       = etharp_output;
    netif->linkoutput   = low_level_output;

    ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

    /*
     * initialize the hardware
     */
    low_level_init(netif);

    return ERR_OK;
}
