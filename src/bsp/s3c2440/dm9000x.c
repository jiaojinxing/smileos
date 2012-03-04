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
#include "kern/kern.h"

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  struct eth_addr *ethaddr;
  void (*outblk)(const void *data, int len);
  void (*inblk)(void *data, int len);
  void (*rx_status)(uint16_t *RxStatus, uint16_t *RxLen);
};

#define CONFIG_DRIVER_DM9000    1
#include "dm9000x.h"

/*
 * 板级
 */
#define CONFIG_DM9000_BASE      0x20000300
#define DM9000_IO               (CONFIG_DM9000_BASE)
#define DM9000_DATA             (CONFIG_DM9000_BASE + 4)
#define CONFIG_DM9000_NO_SROM   1

/*
 * ARCH IO 级
 */
#define writeb(d, r)            (*(volatile uint8_t  *)r) = (d)
#define writew(d, r)            (*(volatile uint16_t *)r) = (d)
#define writel(d, r)            (*(volatile uint32_t *)r) = (d)
#define readb(r)                (*(volatile uint8_t  *)r)
#define readw(r)                (*(volatile uint16_t *)r)
#define readl(r)                (*(volatile uint32_t *)r)
#define __le16_to_cpu(x)        ((uint16_t)(x))

/*
 * 系统级
 */
#define CONFIG_SYS_HZ           TICK_PER_SECOND
#define get_timer(a)            get_tick()
#define printf                  printk
extern void usleep(unsigned int us);
#define udelay                  usleep

#define CONFIG_DM9000_DEBUG     0


static int dm9000_init(struct eth_device *dev);

#include "s3c2440.h"

void dm9000_isr(void)
{
    printk("dm9000_isr\n");
}

static int eint47_isr(uint32_t interrupt)
{
    uint32_t sub_interrupt;

    sub_interrupt = EINTPEND;

    if (sub_interrupt & (1 << 7)) {
       dm9000_isr();
    }

    EINTPEND = sub_interrupt;

    return 0;
}

#if CONFIG_DM9000_DEBUG  > 0
#define DM9000_DBG(fmt,args...) printf(fmt, ##args)
#else
#define DM9000_DBG(fmt,args...)
#define DM9000_ERR(fmt,args...)
#endif

#define DM9000_outb(d,r)        writeb(d, (volatile uint8_t  *)(r))
#define DM9000_outw(d,r)        writew(d, (volatile uint16_t *)(r))
#define DM9000_outl(d,r)        writel(d, (volatile uint32_t *)(r))
#define DM9000_inb(r)           readb((volatile uint8_t  *)(r))
#define DM9000_inw(r)           readw((volatile uint16_t *)(r))
#define DM9000_inl(r)           readl((volatile uint32_t *)(r))

/*
 * 输出块 8 位模式
 */
static void dm9000_outblk_8bit(const void *data, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        DM9000_outb(((const uint8_t *)data)[i], DM9000_DATA);
    }
}

/*
 * 输出块 16 位模式
 */
static void dm9000_outblk_16bit(const void *data, int len)
{
    int i;

    len = (len + 1) / 2;

    for (i = 0; i < len; i++) {
        DM9000_outw(((const uint16_t *)data)[i], DM9000_DATA);
    }
}

/*
 * 输出块 32 位模式
 */
static void dm9000_outblk_32bit(const void *data, int len)
{
    int i;

    len = (len + 3) / 4;

    for (i = 0; i < len; i++) {
        DM9000_outl(((const uint32_t *)data)[i], DM9000_DATA);
    }
}

/*
 * 输入块 8 位模式
 */
static void dm9000_inblk_8bit(void *data, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        ((uint8_t *)data)[i] = DM9000_inb(DM9000_DATA);
    }
}

/*
 * 输入块 16 位模式
 */
static void dm9000_inblk_16bit(void *data, int len)
{
    int i;

    len = (len + 1) / 2;

    for (i = 0; i < len; i++) {
        ((uint16_t *)data)[i] = DM9000_inw(DM9000_DATA);
    }
}

/*
 * 输入块 32 位模式
 */
static void dm9000_inblk_32bit(void *data, int len)
{
    int i;

    len = (len + 3) / 4;

    for (i = 0; i < len; i++) {
        ((uint32_t *)data)[i] = DM9000_inl(DM9000_DATA);
    }
}

/*
 * 获得接收状态 32 位模式
 */
static void dm9000_rx_status_32bit(uint16_t *status, uint16_t *len)
{
    uint32_t temp;

    DM9000_outb(DM9000_MRCMD, DM9000_IO);

    temp    = DM9000_inl(DM9000_DATA);
    *status = __le16_to_cpu(temp);
    *len    = __le16_to_cpu(temp >> 16);
}

/*
 * 获得接收状态 16 位模式
 */
static void dm9000_rx_status_16bit(uint16_t *status, uint16_t *len)
{
    DM9000_outb(DM9000_MRCMD, DM9000_IO);

    *status = __le16_to_cpu(DM9000_inw(DM9000_DATA));
    *len    = __le16_to_cpu(DM9000_inw(DM9000_DATA));
}

/*
 * 获得接收状态 8 位模式
 */
static void dm9000_rx_status_8bit(uint16_t *status, uint16_t *len)
{
    DM9000_outb(DM9000_MRCMD, DM9000_IO);

    *status = __le16_to_cpu(DM9000_inb(DM9000_DATA) + (DM9000_inb(DM9000_DATA) << 8));
    *len    = __le16_to_cpu(DM9000_inb(DM9000_DATA) + (DM9000_inb(DM9000_DATA) << 8));
}

/*
 * 从 io 端口读一个字节
 */
static uint8_t dm9000_io_read(uint8_t reg)
{
    DM9000_outb(reg, DM9000_IO);
    return DM9000_inb(DM9000_DATA);
}

/*
 * 从 io 端口写一个字节
 */
static void dm9000_io_write(uint8_t reg, uint8_t value)
{
    DM9000_outb(reg, DM9000_IO);
    DM9000_outb(value, DM9000_DATA);
}

/*
 * Read a word from phyxcer
 */
static uint16_t dm9000_phy_read(uint8_t reg)
{
    uint16_t value;

    /*
     * Fill the phyxcer register into REG_0C
     */
    dm9000_io_write(DM9000_EPAR, DM9000_PHY | reg);

    dm9000_io_write(DM9000_EPCR, 0x0c);                                 /*  Issue phyxcer read command  */
    udelay(100);                                                        /*  Wait read complete          */
    dm9000_io_write(DM9000_EPCR, 0x00);                                 /*  Clear phyxcer read command  */

    value = (dm9000_io_read(DM9000_EPDRH) << 8) | dm9000_io_read(DM9000_EPDRL);

    /*
     * The read data keeps on REG_0D & REG_0E
     */
    DM9000_DBG("dm9000_phy_read(reg:0x%x, value:0x%x)\n", reg, value);

    return value;
}

/*
   Write a word to phyxcer
*/
static void dm9000_phy_write(uint8_t reg, uint16_t value)
{
    /*
     * Fill the phyxcer register into REG_0C
     */
    dm9000_io_write(DM9000_EPAR, DM9000_PHY | reg);

    /*
     * Fill the written data into REG_0D & REG_0E
     */
    dm9000_io_write(DM9000_EPDRL, (value & 0xff));
    dm9000_io_write(DM9000_EPDRH, ((value >> 8) & 0xff));

    dm9000_io_write(DM9000_EPCR, 0x0a);                                 /*  Issue phyxcer write command */
    udelay(500);                                                        /*  Wait write complete         */
    dm9000_io_write(DM9000_EPCR, 0x00);                                 /*  Clear phyxcer write command */

    DM9000_DBG("dm9000_phy_write(reg:0x%x, value:0x%x)\n", reg, value);
}

/*
 * 探测 dm9000 芯片
 */
static int dm9000_probe(void)
{
    uint32_t id;

    id  = dm9000_io_read(DM9000_VIDL);
    id |= dm9000_io_read(DM9000_VIDH) << 8;
    id |= dm9000_io_read(DM9000_PIDL) << 16;
    id |= dm9000_io_read(DM9000_PIDH) << 24;

    if (id == DM9000_ID) {
        printf("DM9000 i/o: 0x%x, id: 0x%x \n", CONFIG_DM9000_BASE, id);
        return 0;
    } else {
        printf("DM9000 not found at 0x%x id: 0x%x\n", CONFIG_DM9000_BASE, id);
        return -1;
    }
}

/*
 * 复位 dm9000 芯片
 */
static void dm9000_reset(void)
{
    DM9000_DBG("resetting DM9000\n");

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
        DM9000_DBG("resetting the DM9000, 1st reset\n");
        udelay(25);                                                     /*  Wait at least 20 us         */
    } while (dm9000_io_read(DM9000_NCR) & 1);

    dm9000_io_write(DM9000_NCR, 0x00);
    dm9000_io_write(DM9000_NCR, (NCR_LBK_INT_MAC | NCR_RST));           /*  Issue a second reset        */

    do {
        DM9000_DBG("resetting the DM9000, 2nd reset\n");
        udelay(25);                                                     /*  Wait at least 20 us         */
    } while (dm9000_io_read(DM9000_NCR) & 1);

    /*
     * Check whether the ethernet controller is present
     */
    if ((dm9000_io_read(DM9000_PIDL) != 0x00) ||
        (dm9000_io_read(DM9000_PIDH) != 0x90)) {
        printf("ERROR: resetting DM9000 -> not responding\n");
    }
}

/*
 * 初始化 dm9000 芯片
 */
static int dm9000_init(struct netif *netif)
{
    struct ethernetif *ethernetif = netif->state;
    int i;
    uint8_t io_mode, oft;
    uint16_t lnk;

    /*
     * 复位 dm9000 芯片
     */
    dm9000_reset();

    /*
     * 探测 dm9000 芯片
     */
    if (dm9000_probe() < 0) {
        return -1;
    }

    /*
     * Auto-detect 8/16/32 bit mode, ISR Bit 6+7 indicate bus width
     */
    io_mode = dm9000_io_read(DM9000_ISR) >> 6;
    switch (io_mode) {
    case 0x00:  /* 16-bit mode */
        printf("DM9000: running in 16 bit mode\n");
        ethernetif->outblk    = dm9000_outblk_16bit;
        ethernetif->inblk     = dm9000_inblk_16bit;
        ethernetif->rx_status = dm9000_rx_status_16bit;
        break;

    case 0x01:  /* 32-bit mode */
        printf("DM9000: running in 32 bit mode\n");
        ethernetif->outblk    = dm9000_outblk_32bit;
        ethernetif->inblk     = dm9000_inblk_32bit;
        ethernetif->rx_status = dm9000_rx_status_32bit;
        break;

    case 0x02:  /* 8 bit mode */
        printf("DM9000: running in 8 bit mode\n");
        ethernetif->outblk    = dm9000_outblk_8bit;
        ethernetif->inblk     = dm9000_inblk_8bit;
        ethernetif->rx_status = dm9000_rx_status_8bit;
        break;

    default:    /* Assume 8 bit mode, will probably not work anyway */
        printf("DM9000: Undefined IO-mode:0x%x\n", io_mode);
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
        dm9000_io_write(oft, ethernetif->ethaddr[i]);
    }

    for (i = 0, oft = DM9000_MAR; i < 8; i++, oft++) {
        dm9000_io_write(oft, 0xff);
    }

    /*
     * read back mac, just to be sure
     */
    printf("DM9000: MAC=");
    for (i = 0, oft = DM9000_PAR; i < 6; i++, oft++) {
        printf("%x:", dm9000_io_read(oft));
    }
    printf("\n");

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
            printf("DM9000: could not establish link\n");
            return 0;
        }
    }

    lnk = dm9000_phy_read(17) >> 12;                                    /*  see what we've got          */
    printf("DM9000: operating at ");
    switch (lnk) {
    case 1:
        printf("10M half duplex ");
        break;

    case 2:
        printf("10M full duplex ");
        break;

    case 4:
        printf("100M half duplex ");
        break;

    case 8:
        printf("100M full duplex ");
        break;

    default:
        printf("unknown: %d ", lnk);
        break;
    }
    printf("mode\n");

    /*
     * Enable TX/RX interrupt mask
     */
    dm9000_io_write(DM9000_IMR, IMR_PAR | IMR_PTM | IMR_PRM);

    return 0;
}

/*
 * dump dm9000 寄存器
 */
static void dm9000_dump_regs(void)
{
    printf("\n");
    printf("NCR   (0x00): 0x%x\n", dm9000_io_read(0));
    printf("NSR   (0x01): 0x%x\n", dm9000_io_read(1));
    printf("TCR   (0x02): 0x%x\n", dm9000_io_read(2));
    printf("TSRI  (0x03): 0x%x\n", dm9000_io_read(3));
    printf("TSRII (0x04): 0x%x\n", dm9000_io_read(4));
    printf("RCR   (0x05): 0x%x\n", dm9000_io_read(5));
    printf("RSR   (0x06): 0x%x\n", dm9000_io_read(6));
    printf("ISR   (0xFE): 0x%x\n", dm9000_io_read(DM9000_ISR));
    printf("\n");
}

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

  initiate transfer();

#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

  for(q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
    send data from(q->payload, q->len);
  }

  signal that packet should be sent();

#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

  LINK_STATS_INC(link.xmit);

  return ERR_OK;
}

sys_sem_t rx_sem;
int32_t rx_thread;

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

    /*
     * There is _at least_ 1 package in the fifo, read them all
     */
    for (;;) {
        /*
         * Dummy read
         */
        dm9000_io_read(DM9000_MRCMDX);

        /*
         * Get most updated data,
         * only look at bits 0:1, See application notes DM9000
         */
        byte = DM9000_inb(DM9000_DATA) & 0x03;

        /*
         * Status check: this byte must be 0 or 1
         */
        if (byte > DM9000_PKT_RDY) {
            dm9000_io_write(DM9000_RCR, 0x00);                          /*  Stop Device                 */
            dm9000_io_write(DM9000_ISR, 0x80);                          /*  Stop INT request            */
            DM9000_ERR("DM9000 error: status check fail: 0x%x\n", byte);
            break;
        }

        if (byte != DM9000_PKT_RDY) {
            break;                                                      /*  No packet received, ignore  */
        }

        DM9000_DBG("DM9000 debug: receiving packet\n");

        /*
         * A packet ready now & Get status/length
         */
        (ethernetif->rx_status)(&status, &len);

        DM9000_DBG("DM9000 debug: rx status: 0x%x rx len: %d\n", status, len);

        /*
         * We allocate a pbuf chain of pbufs from the pool.
         */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
        if (p != NULL) {
            for (q = p; q != NULL; q = q->next) {
                /*
                 * Move data from DM9000
                 * Read received packet from RX SRAM
                 */
                (ethernetif->inblk)(q->payload, q->len);
            }
        }

        if ((status & 0xbf00) || (len < 0x40) || (len > DM9000_PKT_MAX)) {
            if (status & 0x100) {
                printf("rx fifo error\n");
            }

            if (status & 0x200) {
                printf("rx crc error\n");
            }

            if (status & 0x8000) {
                printf("rx length error\n");
            }

            if (len > DM9000_PKT_MAX) {
                printf("rx length too big\n");
                dm9000_reset();
            }
        } else {
            DM9000_DBG("passing packet to upper layer\n");
            return p;
        }

        LINK_STATS_INC(link.recv);
    }

    return NULL;
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
    struct ethernetif *ethernetif = netif->state;

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
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

/********************************************************************************************************/
    /*
     * 创建接收信号量
     */
    sys_sem_new(&rx_sem, 0);

    /*
     * 创建接收线程
     */
    rx_thread = kthread_create(ethernetif_input, (void *)netif, 1024 * 8, 15);

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
    dm9000_init(netif);

    /*
     * 安装 EINT4-7 中断服务程序, 并使能 EINT4-7 中断
     */
    interrupt_install(INTEINT4_7, eint47_isr, NULL);

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

    netif->state = ethernetif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;

    /*
     * We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...)
     */
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;

    ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

    /*
     * initialize the hardware
     */
    low_level_init(netif);

    return ERR_OK;
}
