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
** File name:               s3c2440_ttyS0.c
** Last modified Date:      2012-3-31
** Last Version:            1.0.0
** Descriptions:            S3C2440 TTYS0 驱动和设备
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-31
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/utils.h"
#include "kern/kern.h"
#include <errno.h>
#include "drv/tty.h"
#include <fcntl.h>

#include "s3c2440.h"
#include "s3c2440_clock.h"
/*********************************************************************************************************
** 私有信息
*********************************************************************************************************/
typedef struct {
    VFS_DEVICE_MEMBERS;
    struct tty      tty;
} privinfo_t;

#define TX_FIFO_SIZE        64
#define RX_FIFO_SIZE        64

static void ttyS0_start(struct tty *tp);
/*********************************************************************************************************
** Function name:           ttyS0_rx_job
** Descriptions:            UART0 接收工作
** input parameters:        arg                 参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void ttyS0_rx_job(void *arg)
{
    privinfo_t *priv = arg;
    int i;
    int len;

    len = UFSTAT0 & 0x3F;
    if (len > 0) {
        for (i = 0; i < len; i++) {
            tty_input(URXH0, &priv->tty);
        }
        if (i > 0) {
            select_report(&priv->select, VFS_FILE_READABLE);
        }
    }
}
/*********************************************************************************************************
** Function name:           ttyS0_tx_job
** Descriptions:            UART0 发送工作
** input parameters:        arg                 参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void ttyS0_tx_job(void *arg)
{
    privinfo_t *priv = arg;
    int i;
    int len;
    int c;

    if (!(UFSTAT0 & (0x01 << 14))) {
        len = TX_FIFO_SIZE - (UFSTAT0 >> 8 & 0x3F);
        if (len > 0) {
            for (i = 0; i < len; i++) {
                if ((c = tty_getc(&priv->tty.t_outq)) >= 0) {
                    UTXH0 = c;
                }
            }

            if (i > 0) {
                select_report(&priv->select, VFS_FILE_WRITEABLE);
            }
        }
    }
}
/*********************************************************************************************************
** Function name:           uart0_isr
** Descriptions:            UART0 中断服务程序
** input parameters:        interrupt           中断
**                          arg                 参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int uart0_isr(uint32_t interrupt, void *arg)
{
    int reg = SUBSRCPND;

    if (reg & BIT_SUB_RXD0) {
        netjob_add(ttyS0_rx_job, arg);
    }

    if (reg & BIT_SUB_TXD0) {
        netjob_add(ttyS0_tx_job, arg);
    }

    SUBSRCPND = BIT_SUB_RXD0 | BIT_SUB_TXD0;

    return 0;
}
/*********************************************************************************************************
** Function name:           ttyS0_open
** Descriptions:            打开 ttyS0
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          oflag               打开标志
**                          mode                模式
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int ttyS0_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_inc_return(&(((device_t *)file->ctx)->ref)) == 1) {
        /*
         * 第一次打开时的初始化代码
         */
        GPHCON  = (GPHCON & (~(0x0F << 4))) | (0x0A << 4);

        GPHUP  |= 0x0F;

        /*
         * UART LINE CONTROL REGISTER
         */
        ULCON0  = 0x00 << 6 |                                           /*  Normal mode operation       */
                  0x00 << 3 |                                           /*  No parity                   */
                  0x00 << 2 |                                           /*  One stop bit per frame      */
                  0x03 << 0;                                            /*  8-bits                      */

        /*
         * UART CONTROL REGISTER
         */
        UCON0   = 0x01 << 0 |                                           /*  Receive  Mode: Interrupt request or polling mode */
                  0x01 << 2 |                                           /*  Transmit Mode: Interrupt request or polling mode */
                  0x00 << 4 |                                           /*  Normal transmit             */
                  0x00 << 5 |                                           /*  Normal operation            */
                  0x00 << 6 |                                           /*  Rx Error Interrupt Disable  */
                  0x01 << 7 |                                           /*  Rx Time Out Enable          */
                  0x00 << 8 |                                           /*  Rx Interrupt Type: Level    */
                  0x00 << 9 |                                           /*  Tx Interrupt Type: Level    */
                  0x02 << 10;                                           /*  PCLK                        */

        /*
         * UART FIFO CONTROL REGISTER
         */
        UFCON0  = 0x03 << 6 |                                           /*  Tx FIFO Trigger Level: 0B   */
                  0x02 << 4 |                                           /*  Rx FIFO Trigger Level: 1B   */
                  0x01 << 2 |                                           /*  Tx FIFO Reset               */
                  0x01 << 1 |                                           /*  Rx FIFO Reset               */
                  0x01 << 0;                                            /*  FIFO Enable                 */

        /*
         * UART MODEM CONTROL REGISTER
         */
        UMCON0  = 0x00;

        /*
         * (int)(UART clock / (baud rate x 16)) –1
         */
        UBRD0   = ((uint32_t)(PCLK / 16.0 / UART_BAUD_RATE + 0.5) - 1);


        interrupt_install(INTUART0, uart0_isr, NULL, priv);             /*  Install isr                 */

        interrupt_unmask(INTUART0);

        INTSUBMSK &= ~INTSUB_TXD0;
        INTSUBMSK &= ~INTSUB_RXD0;

        return 0;
    } else {
        /*
         * 如果设备不允许同时打开多次, 请使用如下代码:
         */
        atomic_dec(&(((device_t *)file->ctx)->ref));
        seterrno(EBUSY);
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           ttyS0_close
** Descriptions:            关闭 ttyS0
** input parameters:        ctx                 私有信息
**                          file                文件结构
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int ttyS0_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&(((device_t *)file->ctx)->ref)) == 1) {
        /*
         * 最后一次关闭时的清理代码
         */
        INTSUBMSK |= INTSUB_TXD0;
        INTSUBMSK |= INTSUB_RXD0;

        interrupt_mask(INTUART0);
    }
    atomic_dec(&(((device_t *)file->ctx)->ref));
    return 0;
}
/*********************************************************************************************************
** Function name:           ttyS0_ioctl
** Descriptions:            控制 ttyS0
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          cmd                 命令
**                          arg                 参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int ttyS0_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    arg = va_to_mva(arg);

    ret = tty_ioctl(&priv->tty, cmd, arg);
    if (ret != 0) {
        seterrno(ret);
        return -1;
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           ttyS0_isatty
** Descriptions:            ttyS0 是一个 tty
** input parameters:        ctx                 私有信息
**                          file                文件结构
** output parameters:       NONE
** Returned value:          1
*********************************************************************************************************/
static int ttyS0_isatty(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 1;
}
/*********************************************************************************************************
** Function name:           ttyS0_read
** Descriptions:            读 ttyS0
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          buf                 缓冲区
**                          len                 读取长度
** output parameters:       NONE
** Returned value:          成功读取的字节数
*********************************************************************************************************/
static ssize_t ttyS0_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    ret = tty_read(&priv->tty, buf, &len);
    if (ret != 0) {
        seterrno(ret);
        return -1;
    } else {
        return len;
    }
}
/*********************************************************************************************************
** Function name:           ttyS0_write
** Descriptions:            写 ttyS0
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          buf                 数据
**                          len                 数据长度
** output parameters:       NONE
** Returned value:          成功写入的字节数
*********************************************************************************************************/
static ssize_t ttyS0_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    ret = tty_write(&priv->tty, buf, &len);
    if (ret != 0) {
        seterrno(ret);
        return -1;
    } else {
        return len;
    }
}
/*********************************************************************************************************
** Function name:           ttyS0_start
** Descriptions:            启动 ttyS0 输出
** input parameters:        tp                  TTY 结构
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void ttyS0_start(struct tty *tp)
{
    netjob_add(ttyS0_tx_job, struct_addr(tp, privinfo_t, tty));
}
/*********************************************************************************************************
** Function name:           ttyS0_scan
** Descriptions:            扫描 ttyS0
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          flags               可读写标志
** output parameters:       NONE
** Returned value:          可读写标志
*********************************************************************************************************/
static int ttyS0_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if (tty_readable(&priv->tty) && (flags & VFS_FILE_READABLE)) {
        ret |= VFS_FILE_READABLE;
    }
    if (tty_writeable(&priv->tty) && (flags & VFS_FILE_WRITEABLE)) {
        ret |= VFS_FILE_WRITEABLE;
    }
    return ret;
}
/*********************************************************************************************************
** ttyS0 驱动
*********************************************************************************************************/
static driver_t ttyS0_drv = {
        .name     = "ttyS0",
        .open     = ttyS0_open,
        .write    = ttyS0_write,
        .read     = ttyS0_read,
        .isatty   = ttyS0_isatty,
        .ioctl    = ttyS0_ioctl,
        .close    = ttyS0_close,
        .scan     = ttyS0_scan,
        .select   = select_select,
        .unselect = select_unselect,
};
/*********************************************************************************************************
** Function name:           ttyS0_init
** Descriptions:            初始化 ttyS0
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int ttyS0_init(void)
{
    privinfo_t *priv;

    driver_install(&ttyS0_drv);

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        device_init(priv);
        tty_attach(&priv->tty);
        priv->tty.t_oproc = ttyS0_start;
        if (device_create("/dev/ttyS0", "ttyS0", priv) < 0) {
            kfree(priv);
            return -1;
        }
        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
