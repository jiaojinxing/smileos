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
** Descriptions:            �豸����ģ��
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-6
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "s3c2440.h"
#include "s3c2440_int.h"
#include "drv/audio.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>

/*
 * DMA ����
 */
typedef struct dma_job {
    struct dma_job  *prev;                                              /*  ǰһ�� DMA ����             */
    struct dma_job  *next;                                              /*  ��һ�� DMA ����             */
    uint32_t         len;                                               /*  ���ݵĳ���                  */
    uint8_t          buf[1];                                            /*  ����                        */
} dma_job_t;

/*
 * ˽����Ϣ
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
 * L3 ���߽ӿ����Ŷ���
 */
#define L3C         (1 << 4)                                            /*  GPB4 = L3CLOCK              */
#define L3D         (1 << 3)                                            /*  GPB3 = L3DATA               */
#define L3M         (1 << 2)                                            /*  GPB2 = L3MODE               */

/*
 * ��ʼ�� L3 ���߽ӿ�
 */
static int l3bus_init(void)
{
    GPBCON = (GPBCON & (~(0x03ul << 4))) | (0x01ul << 4);               /*  GPB2 ����Ϊ���             */
    GPBCON = (GPBCON & (~(0x03ul << 6))) | (0x01ul << 6);               /*  GPB3 ����Ϊ���             */
    GPBCON = (GPBCON & (~(0x03ul << 8))) | (0x01ul << 8);               /*  GPB4 ����Ϊ���             */
    GPBUP |= (0x07ul << 2);                                             /*  �ر� GPB2-4 ����������      */

    GPBDAT = (GPBDAT & (~(L3D | L3M | L3C))) | (L3C | L3M);

    return 0;
}

/*
 * ͨ�� L3 ���߽ӿڷ�������
 */
static int l3bus_write(uint8_t data, uint8_t addr_mode)
{
    int i, j;

    if (addr_mode) {
        GPBDAT = (GPBDAT & (~(L3D | L3M | L3C))) | L3C;                 /*  L3D=L, L3M=L, L3C=H         */
    } else {
        GPBDAT = (GPBDAT & (~(L3D | L3M | L3C))) | (L3C | L3M);         /*  L3D=L, L3M=H, L3C=H         */
    }

    for (j = 0; j < 10; j++) {                                          /*  �ȴ�һ��ʱ��                */
        ;
    }

    /*
     * ���ݴ��������LSB ˳��
     */
    for (i = 0; i < 8; i++) {
        if (data & 0x1) {
            GPBDAT &= ~L3C;                                             /*  L3C=L                       */
            GPBDAT |=  L3D;                                             /*  L3D=H                       */
            for (j = 0; j < 5; j++) {                                   /*  �ȴ�һ��ʱ��                */
                ;
            }
            GPBDAT |=  L3C;                                             /*  L3C=H                       */
            GPBDAT |=  L3D;                                             /*  L3D=H                       */
            for (j = 0; j < 5; j++) {                                   /*  �ȴ�һ��ʱ��                */
                ;
            }
        } else {
            GPBDAT &= ~L3C;                                             /*  L3C=L                       */
            GPBDAT &= ~L3D;                                             /*  L3D=L                       */
            for (j = 0; j < 5; j++) {                                   /*  �ȴ�һ��ʱ��                */
                ;
            }
            GPBDAT |=  L3C;                                             /*  L3C=H                       */
            GPBDAT &= ~L3D;                                             /*  L3D=L                       */
            for (j = 0; j < 5; j++) {                                   /*  �ȴ�һ��ʱ��                */
                ;
            }
        }
        data >>= 1;
    }

    GPBDAT = (GPBDAT & (~(L3D | L3M | L3C))) | (L3C | L3M);

    return 0;
}

/*
 * ��ʼ�� UDA1341
 */
static int uda1341_init(void)
{
    /*
     * UDA1341TS.pdf PAGE 15
     */
    l3bus_write(0x14 + 2, 1);                                           /*  ״̬ģʽ(000101xx+10)       */
    l3bus_write(0x40, 0);                                               /*  0,1,00,000,0  : ״̬0, ��λ */

    l3bus_write(0x14 + 2, 1);                                           /*  ״̬ģʽ(000101xx+10)       */
    l3bus_write(0x10, 0);                                               /*  0,0,01,000,0  : ״̬0, 384fs, IIS-bus, no DC-filter */

    l3bus_write(0x14 + 2, 1);                                           /*  ״̬ģʽ(000101xx+10)       */
    l3bus_write(0xc1, 0);                                               /*  1,0,0,0,0,0,01: ״̬1, ADC off, DAC on */

    return 0;
}

/*
 * ��ʼ�� IIS �ӿ�
 */
static int iis_init(void)
{
    GPECON = (GPECON & (~(0x03ul << 0))) | (0x02ul << 0);               /*  GPE0 ����Ϊ I2SLRCK         */
    GPECON = (GPECON & (~(0x03ul << 2))) | (0x02ul << 2);               /*  GPE1 ����Ϊ I2SSCLK         */
    GPECON = (GPECON & (~(0x03ul << 4))) | (0x02ul << 4);               /*  GPE2 ����Ϊ CDCLK           */
    GPECON = (GPECON & (~(0x03ul << 6))) | (0x02ul << 6);               /*  GPE3 ����Ϊ I2SDI           */
    GPECON = (GPECON & (~(0x03ul << 8))) | (0x02ul << 8);               /*  GPE4 ����Ϊ I2SDO           */

    GPEUP |= (0x1Ful << 0);                                             /*  �ر� GPE0-4 ����������      */

    return 0;
}

/*
 * ���� IIS �ӿ�
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

    IISCON  = (1 << 5) |                                                /*  ʹ�ܷ��� DMA ��������       */
              (0 << 4) |                                                /*  ���ܽ��� DMA ��������       */
              (0 << 3) |                                                /*  ���� DMA ͨ��������         */
              (1 << 2) |                                                /*  ���� DMA ͨ������           */
              (1 << 1) |                                                /*  IIS Ԥ��Ƶ����Ч            */
              (0 << 0);                                                 /*  ��ͣ IIS �ӿ�               */

    IISMOD  = (0 << 9) |                                                /*  ѡ�� PCLK ��Ϊ��ʱ��        */
              (0 << 8) |                                                /*  ��ģʽ                      */
              (2 << 6) |                                                /*  ���ͺͽ���˫��ģʽ          */
              (0 << 5) |                                                /*  ������ͨ��ʱ, I2SLRCK �͵�ƽ*/
              (0 << 4) |                                                /*��IIS ���ݸ�ʽ                */
              (1 << 3) |                                                /*  16 λ����                   */
              (1 << 2) |                                                /*  ��ʱ��Ϊ 384 fs             */
              (1 << 0);                                                 /*  ����ʱ��Ϊ 32 fs            */

    IISFCON = (1 << 15) |                                               /*  ���� FIFO ģʽѡ�� DMA      */
              (1 << 13);                                                /*  ʹ�ܷ��� FIFO               */

    IISCON |= 0x1;                                                      /*  ���� IIS                    */

    return 0;
}

/*
 * DMA2 ͨ���жϷ�����
 */
static int select_report(void *ctx, int type);

static int dma2_isr(uint32_t interrupt, void *arg)
{
    privinfo_t *priv = arg;
    dma_job_t  *head = priv->queue.next;                                /*  �Ƴ���ͷ DMA ����           */

    priv->queue.next = head->next;
    head->next->prev = &priv->queue;

    priv->queue.len -= head->len;                                       /*  ���� DMA �������ݵ��ܳ���   */

    kfree(head);                                                        /*  �ͷŶ�ͷ DMA ����           */

    if (priv->queue.next != &priv->queue) {
        dma_job_t *job = priv->queue.next;

        DISRC2     = (uint32_t)job->buf;                                /*  DMA ����Դ��ַ              */

        DISRCC2    = (0 << 1) |                                         /*  Դ��ַ��ϵͳ����(AHB)       */
                     (0 << 0);                                          /*  Դ��ַ����                  */

        DIDST2     = (uint32_t)&IISFIFO;                                /*  DMA ����Ŀ�ĵ�ַ            */

        DIDSTC2    = (0 << 2) |                                         /*  ���������ֵΪ 0 ʱ�ж�     */
                     (1 << 1) |                                         /*  Ŀ�ĵ�ַ����������(APB)     */
                     (1 << 0);                                          /*  Ŀ�ĵ�ַ����                */

        DCON2      = (1 << 31) |                                        /*  ʹ��Ӳ������ģʽ            */
                     (0 << 30) |                                        /*  DREQ �� DACK ͬ���� PCLK    */
                     (1 << 29) |                                        /*  �������ʱ�����ж�          */
                     (0 << 28) |                                        /*  A unit transfer is performed*/
                     (0 << 27) |
                     (0 << 24) |                                        /*  DMA request source: I2SSDO  */
                     (1 << 23) |                                        /*  I2SSDO triggers the DMA operation */
                     (1 << 22) |                                        /*  �������ʱ�ر� DMA ͨ��     */
                     (1 << 20) |                                        /*  ���䵥Ԫ�Ĵ�С: ����(16λ)  */
                     ((job->len >> 1) & 0xfffff);                       /*  ��ʼ�������                */

        DMASKTRIG2 = (0 << 2) |                                         /*  ��ֹͣ DMA ����             */
                     (1 << 1) |                                         /*  ���� DMA ͨ��               */
                     (0 << 0);                                          /*  ���������                  */
    }

    if (priv->queue.len < AUDIO_OUT_QUEUE_SIZE) {
        select_report(priv, VFS_FILE_WRITEABLE);
        sem_signal_if_has_wait(&priv->sem);
    }
    return 0;
}

/*
 * �� audio
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
 * ���� audio
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
 * �ر� audio
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
 * д audio
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
     * ���û�пռ��д
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

    job = (dma_job_t *)kmalloc(sizeof(dma_job_t) + len - 1);            /*  ���䲢��ʼ�� DMA ����       */
    if (job == NULL) {
        return -1;
    }
    memcpy(job->buf, buf, len);
    job->len = len;

    if (priv->queue.next != &priv->queue) {                             /*  ��� DMA ���ڴ���           */
        priv->queue.prev->next = job;                                   /*  �� DMA ��������β           */
        job->prev              = priv->queue.prev;
        job->next              = &priv->queue;
        priv->queue.prev       = job;
        priv->queue.len       += job->len;                              /*  ���� DMA �������ݵ��ܳ���   */
    } else {
        priv->queue.prev->next = job;                                   /*  �� DMA ��������β           */
        job->prev              = priv->queue.prev;
        job->next              = &priv->queue;
        priv->queue.prev       = job;
        priv->queue.len       += job->len;                              /*  ���� DMA �������ݵ��ܳ���   */

        DISRC2     = (uint32_t)job->buf;                                /*  DMA ����Դ��ַ              */

        DISRCC2    = (0 << 1) |                                         /*  Դ��ַ��ϵͳ����(AHB)       */
                     (0 << 0);                                          /*  Դ��ַ����                  */

        DIDST2     = (uint32_t)&IISFIFO;                                /*  DMA ����Ŀ�ĵ�ַ            */

        DIDSTC2    = (0 << 2) |                                         /*  ���������ֵΪ 0 ʱ�ж�     */
                     (1 << 1) |                                         /*  Ŀ�ĵ�ַ����������(APB)     */
                     (1 << 0);                                          /*  Ŀ�ĵ�ַ����                */

        DCON2      = (1 << 31) |                                        /*  ʹ��Ӳ������ģʽ            */
                     (0 << 30) |                                        /*  DREQ �� DACK ͬ���� PCLK    */
                     (1 << 29) |                                        /*  �������ʱ�����ж�          */
                     (0 << 28) |                                        /*  A unit transfer is performed*/
                     (0 << 27) |
                     (0 << 24) |                                        /*  DMA request source: I2SSDO  */
                     (1 << 23) |                                        /*  I2SSDO triggers the DMA operation */
                     (1 << 22) |                                        /*  �������ʱ�ر� DMA ͨ��     */
                     (1 << 20) |                                        /*  ���䵥Ԫ�Ĵ�С: ����(16λ)  */
                     ((job->len >> 1) & 0xfffff);                       /*  ��ʼ�������                */

        DMASKTRIG2 = (0 << 2) |                                         /*  ��ֹͣ DMA ����             */
                     (1 << 1) |                                         /*  ���� DMA ͨ��               */
                     (0 << 0);                                          /*  ���������                  */
    }

    return len;
}

/*
 * ɨ��
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
 * ����
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
 * audio ����
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
 * ��ʼ�� audio
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
