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
** File name:               audiodrv.c
** Last modified Date:      2012-4-6
** Last Version:            1.0.0
** Descriptions:            设备驱动模板
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-6
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
#include "kern/ipc.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "s3c2440.h"
#include "s3c2440_int.h"
#include "drv/audio.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>

/*
 * DMA 工作
 */
typedef struct dma_job {
    struct dma_job  *prev;                                              /*  前一个 DMA 工作             */
    struct dma_job  *next;                                              /*  后一个 DMA 工作             */
    uint32_t         len;                                               /*  数据的长度                  */
    uint8_t          buf[1];                                            /*  数据                        */
} dma_job_t;

/*
 * 私有信息
 */
typedef struct {
    VFS_SELECT_MEMBERS;
    int             mode;
    int             ref;
    dma_job_t       queue;
    uint32_t        bps;
    uint32_t        channels;
    uint32_t        fs;
    sem_t           sem;
} privinfo_t;

#define AUDIO_OUT_QUEUE_SIZE    (100 * KB)

/*
 * L3 总线接口引脚定义
 */
#define L3C         (1 << 4)                                            /*  GPB4 = L3CLOCK              */
#define L3D         (1 << 3)                                            /*  GPB3 = L3DATA               */
#define L3M         (1 << 2)                                            /*  GPB2 = L3MODE               */

/*
 * 初始化 L3 总线接口
 */
static int l3bus_init(void)
{
    GPBCON = (GPBCON & (~(0x03ul << 4))) | (0x01ul << 4);               /*  GPB2 配置为输出             */
    GPBCON = (GPBCON & (~(0x03ul << 6))) | (0x01ul << 6);               /*  GPB3 配置为输出             */
    GPBCON = (GPBCON & (~(0x03ul << 8))) | (0x01ul << 8);               /*  GPB4 配置为输出             */
    GPBUP |= (0x07ul << 2);                                             /*  关闭 GPB2-4 的上拉电阻      */

    GPBDAT = (GPBDAT & (~(L3D | L3M | L3C))) | (L3C | L3M);

    return 0;
}

/*
 * 通过 L3 总线接口发送数据
 */
static int l3bus_write(uint8_t data, uint8_t addr_mode)
{
    int i, j;

    if (addr_mode) {
        GPBDAT = (GPBDAT & (~(L3D | L3M | L3C))) | L3C;                 /*  L3D=L, L3M=L, L3C=H         */
    } else {
        GPBDAT = (GPBDAT & (~(L3D | L3M | L3C))) | (L3C | L3M);         /*  L3D=L, L3M=H, L3C=H         */
    }

    for (j = 0; j < 10; j++) {                                          /*  等待一段时间                */
        ;
    }

    /*
     * 数据串行输出，LSB 顺序
     */
    for (i = 0; i < 8; i++) {
        if (data & 0x1) {
            GPBDAT &= ~L3C;                                             /*  L3C=L                       */
            GPBDAT |=  L3D;                                             /*  L3D=H                       */
            for (j = 0; j < 5; j++) {                                   /*  等待一段时间                */
                ;
            }
            GPBDAT |=  L3C;                                             /*  L3C=H                       */
            GPBDAT |=  L3D;                                             /*  L3D=H                       */
            for (j = 0; j < 5; j++) {                                   /*  等待一段时间                */
                ;
            }
        } else {
            GPBDAT &= ~L3C;                                             /*  L3C=L                       */
            GPBDAT &= ~L3D;                                             /*  L3D=L                       */
            for (j = 0; j < 5; j++) {                                   /*  等待一段时间                */
                ;
            }
            GPBDAT |=  L3C;                                             /*  L3C=H                       */
            GPBDAT &= ~L3D;                                             /*  L3D=L                       */
            for (j = 0; j < 5; j++) {                                   /*  等待一段时间                */
                ;
            }
        }
        data >>= 1;
    }

    GPBDAT = (GPBDAT & (~(L3D | L3M | L3C))) | (L3C | L3M);

    return 0;
}

/*
 * 初始化 UDA1341
 */
static int uda1341_init(void)
{
    /*
     * UDA1341TS.pdf PAGE 15
     */
    l3bus_write(0x14 + 2, 1);                                           /*  状态模式(000101xx+10)       */
    l3bus_write(0x40, 0);                                               /*  0,1,00,000,0  : 状态0, 复位 */

    l3bus_write(0x14 + 2, 1);                                           /*  状态模式(000101xx+10)       */
    l3bus_write(0x10, 0);                                               /*  0,0,01,000,0  : 状态0, 384fs, IIS-bus, no DC-filter */

    l3bus_write(0x14 + 2, 1);                                           /*  状态模式(000101xx+10)       */
    l3bus_write(0xc1, 0);                                               /*  1,0,0,0,0,0,01: 状态1, ADC off, DAC on */

    return 0;
}

/*
 * 初始化 IIS 接口
 */
static int iis_init(void)
{
    GPECON = (GPECON & (~(0x03ul << 0))) | (0x02ul << 0);               /*  GPE0 配置为 I2SLRCK         */
    GPECON = (GPECON & (~(0x03ul << 2))) | (0x02ul << 2);               /*  GPE1 配置为 I2SSCLK         */
    GPECON = (GPECON & (~(0x03ul << 4))) | (0x02ul << 4);               /*  GPE2 配置为 CDCLK           */
    GPECON = (GPECON & (~(0x03ul << 6))) | (0x02ul << 6);               /*  GPE3 配置为 I2SDI           */
    GPECON = (GPECON & (~(0x03ul << 8))) | (0x02ul << 8);               /*  GPE4 配置为 I2SDO           */

    GPEUP |= (0x1Ful << 0);                                             /*  关闭 GPE0-4 的上拉电阻      */

    return 0;
}

/*
 * 配置 IIS 接口
 */
static int iis_config(uint32_t channels, uint32_t bps, uint32_t fs)
{
    uint32_t tmp;

    /*
     * PCLK=50MHz
     *
     * Prescaler control A=PCLK/CODCLK-1
     */
    switch (fs) {
    case 8000:
        tmp = 15;
        break;

    case 11025:
        tmp = 11;
        break;

    case 16000:
        tmp = 7;
        break;

    case 22050:
        tmp = 5;
        break;

    case 32000:
        tmp = 3;
        break;

    case 44100:
        tmp = 2;
        break;

    case 48000:
        tmp = 2;
        break;

    default:
        return -1;
    }

    IISPSR  = (tmp << 5) |
              (tmp << 0);

    IISCON  = (1 << 5) |                                                /*  使能发送 DMA 服务请求       */
              (0 << 4) |                                                /*  禁能接收 DMA 服务请求       */
              (0 << 3) |                                                /*  发送 DMA 通道不空闲         */
              (1 << 2) |                                                /*  接收 DMA 通道空闲           */
              (1 << 1) |                                                /*  IIS 预分频器有效            */
              (0 << 0);                                                 /*  暂停 IIS 接口               */

    IISMOD  = (0 << 9) |                                                /*  选择 PCLK 作为主时钟        */
              (0 << 8) |                                                /*  主模式                      */
              (2 << 6) |                                                /*  发送和接收双工模式          */
              (0 << 5) |                                                /*  发送右通道时, I2SLRCK 低电平*/
              (0 << 4) |                                                /*　IIS 兼容格式                */
              (1 << 3) |                                                /*  16 位采样                   */
              (1 << 2) |                                                /*  主时钟为 384 fs             */
              (1 << 0);                                                 /*  串行时钟为 32 fs            */

    IISFCON = (1 << 15) |                                               /*  发送 FIFO 模式选择 DMA      */
              (1 << 13);                                                /*  使能发送 FIFO               */

    IISCON |= 0x1;                                                      /*  启动 IIS                    */

    return 0;
}

/*
 * DMA2 通道中断服务函数
 */
static int select_report(void *ctx, int type);

static int dma2_isr(uint32_t interrupt, void *arg)
{
    privinfo_t *priv = arg;
    dma_job_t  *head = priv->queue.next;                                /*  移除队头 DMA 工作           */

    priv->queue.next = head->next;
    head->next->prev = &priv->queue;

    priv->queue.len -= head->len;                                       /*  所有 DMA 工作数据的总长度   */

    kfree(head);                                                        /*  释放队头 DMA 工作           */

    if (priv->queue.next != &priv->queue) {
        dma_job_t *job = priv->queue.next;

        DISRC2     = (uint32_t)job->buf;                                /*  DMA 传输源地址              */

        DISRCC2    = (0 << 1) |                                         /*  源地址在系统总线(AHB)       */
                     (0 << 0);                                          /*  源地址递增                  */

        DIDST2     = (uint32_t)&IISFIFO;                                /*  DMA 传输目的地址            */

        DIDSTC2    = (0 << 2) |                                         /*  当传输计数值为 0 时中断     */
                     (1 << 1) |                                         /*  目的地址在外设总线(APB)     */
                     (1 << 0);                                          /*  目的地址不变                */

        DCON2      = (1 << 31) |                                        /*  使用硬件握手模式            */
                     (0 << 30) |                                        /*  DREQ 和 DACK 同步到 PCLK    */
                     (1 << 29) |                                        /*  传输完成时产生中断          */
                     (0 << 28) |                                        /*  A unit transfer is performed*/
                     (0 << 27) |
                     (0 << 24) |                                        /*  DMA request source: I2SSDO  */
                     (1 << 23) |                                        /*  I2SSDO triggers the DMA operation */
                     (1 << 22) |                                        /*  传输完成时关闭 DMA 通道     */
                     (1 << 20) |                                        /*  传输单元的大小: 半字(16位)  */
                     ((job->len >> 1) & 0xfffff);                       /*  初始传输计数                */

        DMASKTRIG2 = (0 << 2) |                                         /*  不停止 DMA 操作             */
                     (1 << 1) |                                         /*  开启 DMA 通道               */
                     (0 << 0);                                          /*  非软件触发                  */
    }

    if (priv->queue.len < AUDIO_OUT_QUEUE_SIZE) {
        select_report(priv, VFS_FILE_WRITEABLE);
        sem_signal_if_has_wait(&priv->sem);
    }
    return 0;
}

/*
 * 打开 audio
 */
static int audio_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (priv->ref != 0) {
        seterrno(EBUSY);
        return -1;
    }
    priv->ref++;

    if (oflag & O_NONBLOCK) {
        priv->mode |= O_NONBLOCK;
    }
    return 0;
}

/*
 * 控制 audio
 */
static int audio_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;
    int         value;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    switch (cmd) {
    case AUDIO_CHANNELS_NR:
        value = (int)arg;
        if (iis_config(value, priv->bps, priv->fs) == 0) {
            priv->channels = value;
        }
        break;

    case AUDIO_SAMPLE_BITS:
        value = (int)arg;
        if (iis_config(priv->channels, value, priv->fs) == 0) {
            priv->bps = value;
        }
        break;

    case AUDIO_SAMPLE_RATE:
        value = (int)arg;
        if (iis_config(priv->channels, priv->bps, value) == 0) {
            priv->fs = value;
        }
        break;

    default:
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * 关闭 audio
 */
static int audio_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    priv->ref--;
    return 0;
}

/*
 * 写 audio
 */
static ssize_t audio_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    dma_job_t  *job;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    /*
     * 如果没有空间可写
     */
    if (priv->queue.len >= AUDIO_OUT_QUEUE_SIZE) {
        if (priv->mode & O_NONBLOCK) {
            seterrno(EAGAIN);
            return 0;
        } else {
            if (sem_wait(&priv->sem, 0) < 0) {
                seterrno(EINTR);
                return -1;
            }
        }
    }

    job = (dma_job_t *)kmalloc(sizeof(dma_job_t) + len - 1);            /*  分配并初始化 DMA 工作       */
    if (job == NULL) {
        return -1;
    }
    memcpy(job->buf, buf, len);
    job->len = len;

    if (priv->queue.next != &priv->queue) {                             /*  如果 DMA 正在传输           */
        priv->queue.prev->next = job;                                   /*  加 DMA 工作到队尾           */
        job->prev              = priv->queue.prev;
        job->next              = &priv->queue;
        priv->queue.prev       = job;
        priv->queue.len       += job->len;                              /*  所有 DMA 工作数据的总长度   */
    } else {
        priv->queue.prev->next = job;                                   /*  加 DMA 工作到队尾           */
        job->prev              = priv->queue.prev;
        job->next              = &priv->queue;
        priv->queue.prev       = job;
        priv->queue.len       += job->len;                              /*  所有 DMA 工作数据的总长度   */

        DISRC2     = (uint32_t)job->buf;                                /*  DMA 传输源地址              */

        DISRCC2    = (0 << 1) |                                         /*  源地址在系统总线(AHB)       */
                     (0 << 0);                                          /*  源地址递增                  */

        DIDST2     = (uint32_t)&IISFIFO;                                /*  DMA 传输目的地址            */

        DIDSTC2    = (0 << 2) |                                         /*  当传输计数值为 0 时中断     */
                     (1 << 1) |                                         /*  目的地址在外设总线(APB)     */
                     (1 << 0);                                          /*  目的地址不变                */

        DCON2      = (1 << 31) |                                        /*  使用硬件握手模式            */
                     (0 << 30) |                                        /*  DREQ 和 DACK 同步到 PCLK    */
                     (1 << 29) |                                        /*  传输完成时产生中断          */
                     (0 << 28) |                                        /*  A unit transfer is performed*/
                     (0 << 27) |
                     (0 << 24) |                                        /*  DMA request source: I2SSDO  */
                     (1 << 23) |                                        /*  I2SSDO triggers the DMA operation */
                     (1 << 22) |                                        /*  传输完成时关闭 DMA 通道     */
                     (1 << 20) |                                        /*  传输单元的大小: 半字(16位)  */
                     ((job->len >> 1) & 0xfffff);                       /*  初始传输计数                */

        DMASKTRIG2 = (0 << 2) |                                         /*  不停止 DMA 操作             */
                     (1 << 1) |                                         /*  开启 DMA 通道               */
                     (0 << 0);                                          /*  非软件触发                  */
    }

    return len;
}

/*
 * 扫描
 */
static int audio_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int         ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if ((priv->queue.len < AUDIO_OUT_QUEUE_SIZE) & flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    return ret;
}

/*
 * 控制
 */
static int audio_fcntl(void *ctx, file_t *file, int cmd, int arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    switch (cmd) {
    case F_GETFL:
        return priv->mode;

    case F_SETFL:
        priv->mode = arg;
        return 0;

    default:
        seterrno(EINVAL);
        return -1;
    }
}

#include "drv/selectdrv.h"

/*
 * audio 驱动
 */
driver_t audio_drv = {
        .name     = "audio",
        .open     = audio_open,
        .write    = audio_write,
        .ioctl    = audio_ioctl,
        .close    = audio_close,
        .fcntl    = audio_fcntl,
        .scan     = audio_scan,
        .select   = select_select,
        .unselect = select_unselect,
};

/*
 * 初始化 audio
 */
int audio_create(void)
{
    privinfo_t *priv;

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        priv->mode       = O_NONBLOCK;
        priv->ref        = 0;

        priv->queue.next = priv->queue.prev = &priv->queue;
        priv->queue.len  = 0;

        sem_new(&priv->sem, 0);

        l3bus_init();

        uda1341_init();

        iis_init();

        priv->channels  = 2;
        priv->bps       = 16;
        priv->fs        = 44100;

        iis_config(priv->channels, priv->bps, priv->fs);

        interrupt_install(INTDMA2, dma2_isr, NULL, NULL);

        interrupt_unmask(INTDMA2);

        select_init(priv);

        if (device_create("/dev/audio", "audio", priv) < 0) {
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
