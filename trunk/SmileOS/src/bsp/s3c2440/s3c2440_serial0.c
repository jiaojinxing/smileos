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
** File name:               s3c2440_serial0.c
** Last modified Date:      2012-3-31
** Last Version:            1.0.0
** Descriptions:            S3C2440 串口 0 驱动和设备
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
#include "kern/kern.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/select.h"
#include "kern/kfifo.h"
#include "kern/ipc.h"

#include "s3c2440.h"
#include "s3c2440_clock.h"
/*********************************************************************************************************
** 私有信息
*********************************************************************************************************/
typedef struct {
    VFS_DEVICE_MEMBERS;
    kfifo_t                 iq;                                         /*  输入队列                    */
    kfifo_t                 oq;                                         /*  输出队列                    */
    mutex_t                 write_lock;
    mutex_t                 read_lock;
} privinfo_t;
/*********************************************************************************************************
** 定义
*********************************************************************************************************/
#define TX_FIFO_SIZE        64                                          /*  发送 FIFO 大小              */
#define RX_FIFO_SIZE        64                                          /*  接收 FIFO 大小              */

static int serial0_scan(void *ctx, file_t *file, int flags);
/*********************************************************************************************************
** Function name:           uart0_isr
** Descriptions:            UART0 中断服务程序
** input parameters:        interrupt           中断
**                          arg                 参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int uart0_isr(intno_t interrupt, void *arg)
{
    char buf[max(RX_FIFO_SIZE, RX_FIFO_SIZE)];
    privinfo_t *priv = arg;
    uint32_t reg = SUBSRCPND;
    uint32_t len;
    uint32_t i;

    if (reg & BIT_SUB_RXD0) {                                           /*  接收中断                    */
        while ((len = (UFSTAT0 & 0x3F)) > 0) {                          /*  接收数据                    */
            for (i = 0; i < len; i++) {
                buf[i] = URXH0;
            }
            kfifo_put(&priv->iq, buf, len);                             /*  放到输入队列              　*/
            vfs_event_report(&priv->select, VFS_FILE_READABLE);         /*  通知可读                    */
        }
    }

    if (reg & BIT_SUB_TXD0) {                                           /*  发送 FIFO 空中断            */
        len = kfifo_get(&priv->oq, buf, TX_FIFO_SIZE);                  /*  从输出队列提取数据          */
        if (len == 0) {                                                 /*  没有数据可发                */
            INTSUBMSK |= INTSUB_TXD0;                                   /*  禁能发送 FIFO 空中断     　 */
        } else {
            vfs_event_report(&priv->select, VFS_FILE_WRITEABLE);        /*  通知可写                    */
            for (i = 0; i < len; i++) {                                 /*  发送数据                    */
                UTXH0 = buf[i];
            }
        }
    }

    SUBSRCPND |= BIT_SUB_RXD0 | BIT_SUB_TXD0;                           /*  清除中断                    */

    return 0;
}
/*********************************************************************************************************
** Function name:           serial0_start
** Descriptions:            启动 serial0 输出
** input parameters:        priv                私有信息
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void serial0_start(privinfo_t *priv)
{
    char buf[max(RX_FIFO_SIZE, RX_FIFO_SIZE)];
    unsigned int len;
    unsigned int i;

    if ((((UFSTAT0) >> 8) & 0x3F) == 0) {                               /*  可以直接写入发送 FIFO       */
        len = kfifo_get(&priv->oq, buf, TX_FIFO_SIZE);                  /*  从输出队列提取数据          */
        if (len == 0) {                                                 /*  没有数据可发                */
            INTSUBMSK |= INTSUB_TXD0;                                   /*  禁能发送 FIFO 空中断     　 */
            return;
        } else {
            vfs_event_report(&priv->select, VFS_FILE_WRITEABLE);
            for (i = 0; i < len; i++) {                                 /*  发送数据                    */
                UTXH0 = buf[i];
            }
        }
    }

    INTSUBMSK &= ~INTSUB_TXD0;                                          /*  使能发送 FIFO 空中断     　 */
}
/*********************************************************************************************************
** Function name:           serial0_open
** Descriptions:            打开 serial0
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          oflag               打开标志
**                          mode                模式
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int serial0_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_inc_return(dev_ref(file)) == 1) {
        /*
         * 第一次打开时的初始化代码
         */
        if (kfifo_init(&priv->iq, 1 * KB) < 0) {
            atomic_dec(dev_ref(file));
            seterrno(ENOMEM);
            return -1;
        }

        if (kfifo_init(&priv->oq, 1 * KB) < 0) {
            kfifo_free(&priv->iq);
            atomic_dec(dev_ref(file));
            seterrno(ENOMEM);
            return -1;
        }

        GPHCON  = (GPHCON & (~(0x0F << 4))) | (0x0A << 4);

        GPHUP  |= 0x0F;

        CLKCON |= (1 << 10);                                            /*  时钟挂接                    */

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
                  0x01 << 8 |                                           /*  Rx Interrupt Type: Level    */
                  0x01 << 9 |                                           /*  Tx Interrupt Type: Level    */
                  0x00 << 10;                                           /*  PCLK                        */

        /*
         * UART FIFO CONTROL REGISTER
         */
        UFCON0  = 0x00 << 6 |                                           /*  Tx FIFO Trigger Level:  0B  */
                  0x03 << 4 |                                           /*  Rx FIFO Trigger Level: 32B  */
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

        INTSUBMSK &= ~INTSUB_RXD0;                                      /*  使能接收中断             　 */
        INTSUBMSK |=  INTSUB_TXD0;                                      /*  禁能发送 FIFO 空中断     　 */

        return 0;
    } else {
        return 0;
    }
}
/*********************************************************************************************************
** Function name:           serial0_close
** Descriptions:            关闭 serial0
** input parameters:        ctx                 私有信息
**                          file                文件结构
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int serial0_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_read(dev_ref(file)) == 1) {
        /*
         * TODO: 加上最后一次关闭时的清理代码
         */
        INTSUBMSK |= INTSUB_TXD0;
        INTSUBMSK |= INTSUB_RXD0;

        interrupt_mask(INTUART0);

        kfifo_free(&priv->iq);
        kfifo_free(&priv->oq);
    }
    atomic_dec(dev_ref(file));

    return 0;
}
/*********************************************************************************************************
** Function name:           serial0_ioctl
** Descriptions:            控制 serial0
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          cmd                 命令
**                          arg                 参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int serial0_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           serial0_isatty
** Descriptions:            serial0 是一个 tty
** input parameters:        ctx                 私有信息
**                          file                文件结构
** output parameters:       NONE
** Returned value:          1
*********************************************************************************************************/
static int serial0_isatty(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 1;
}
/*********************************************************************************************************
** Function name:           serial0_read
** Descriptions:            读 serial0
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          buf                 缓冲区
**                          len                 读取长度
** output parameters:       NONE
** Returned value:          成功读取的字节数
*********************************************************************************************************/
static ssize_t serial0_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    __again:
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    mutex_lock(&priv->read_lock, 0);

    if (kfifo_is_empty(&priv->iq)) {                                    /*  如果没有数据可读            */
        ret = vfs_block_helper(&priv->select, serial0_scan, ctx, file, VFS_FILE_READABLE);
        if (ret <= 0) {
            mutex_unlock(&priv->read_lock);
            return ret;
        } else {
            mutex_unlock(&priv->read_lock);
            goto __again;
        }
    }

    len = kfifo_get(&priv->iq, buf, len);

    mutex_unlock(&priv->read_lock);

    return len;
}
/*********************************************************************************************************
** Function name:           serial0_write
** Descriptions:            写 serial0
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          buf                 数据
**                          len                 数据长度
** output parameters:       NONE
** Returned value:          成功写入的字节数
*********************************************************************************************************/
static ssize_t serial0_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    __again:
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    mutex_lock(&priv->write_lock, 0);

    if (kfifo_is_full(&priv->oq)) {                                     /*  如果没有空间可写            */
        ret = vfs_block_helper(&priv->select, serial0_scan, ctx, file, VFS_FILE_WRITEABLE);
        if (ret <= 0) {
            mutex_unlock(&priv->write_lock);
            return ret;
        } else {
            mutex_unlock(&priv->write_lock);
            goto __again;
        }
    }

    len = kfifo_put(&priv->oq, buf, len);

    serial0_start(priv);                                                /*  启动写操作                  */

    mutex_unlock(&priv->write_lock);

    return len;
}
/*********************************************************************************************************
** Function name:           serial0_scan
** Descriptions:            扫描 serial0
** input parameters:        ctx                 私有信息
**                          file                文件结构
**                          flags               可读写标志
** output parameters:       NONE
** Returned value:          可读写标志
*********************************************************************************************************/
static int serial0_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if ((!kfifo_is_empty(&priv->iq)) && flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if ((!kfifo_is_full(&priv->oq)) && flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    return ret;
}
/*********************************************************************************************************
** serial0 驱动
*********************************************************************************************************/
static driver_t serial0_drv = {
        .name     = "serial0",
        .open     = serial0_open,
        .write    = serial0_write,
        .read     = serial0_read,
        .isatty   = serial0_isatty,
        .ioctl    = serial0_ioctl,
        .close    = serial0_close,
        .scan     = serial0_scan,
        .select   = select_select,
        .unselect = select_unselect,
};
/*********************************************************************************************************
** Function name:           serial0_init
** Descriptions:            初始化 serial0
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int serial0_init(void)
{
    privinfo_t *priv;

    driver_install(&serial0_drv);

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);

        mutex_create(&priv->write_lock);
        mutex_create(&priv->read_lock);

        if (device_create("/dev/serial0", "serial0", priv) < 0) {
            kfree(priv);
            return -1;
        }
        return 0;
    } else {
        seterrno(ENOMEM);
        return -1;
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
