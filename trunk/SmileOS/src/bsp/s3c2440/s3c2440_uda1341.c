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
** File name:               s3c2440_uda1341.c
** Last modified Date:      2012-4-6
** Last Version:            1.0.0
** Descriptions:            S3C2440 UDA1341 ��ƵоƬ����
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
#include "vfs/select.h"
#include "drivers/audio.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "s3c2440.h"
/*********************************************************************************************************
** DMA ����
*********************************************************************************************************/
typedef struct dma_job {
    struct dma_job  *prev;                                              /*  ǰһ�� DMA ����             */
    struct dma_job  *next;                                              /*  ��һ�� DMA ����             */
    uint32_t         len;                                               /*  ���ݵĳ���                  */
    uint8_t          buf[1];                                            /*  ����                        */
} dma_job_t;
/*********************************************************************************************************
** ˽����Ϣ
*********************************************************************************************************/
typedef struct {
    VFS_DEVICE_MEMBERS;
    dma_job_t       queue;
    uint32_t        bps;
    uint32_t        channels;
    uint32_t        fs;
    sem_t           done;
} privinfo_t;
/*********************************************************************************************************
** ��Ƶ������д�С����
*********************************************************************************************************/
#define AUDIO_OUT_QUEUE_SIZE    (100 * KB)
#define AUDIO_DMA_INT           INTDMA2
/*********************************************************************************************************
** L3 ���߽ӿ����Ŷ���
*********************************************************************************************************/
#define L3C         (1 << 4)                                            /*  GPB4 = L3CLOCK              */
#define L3D         (1 << 3)                                            /*  GPB3 = L3DATA               */
#define L3M         (1 << 2)                                            /*  GPB2 = L3MODE               */
/*********************************************************************************************************
** Function name:           l3bus_init
** Descriptions:            ��ʼ�� L3 ���߽ӿ�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int l3bus_init(void)
{
    GPBCON = (GPBCON & (~(0x03ul << 4))) | (0x01ul << 4);               /*  GPB2 ����Ϊ���             */
    GPBCON = (GPBCON & (~(0x03ul << 6))) | (0x01ul << 6);               /*  GPB3 ����Ϊ���             */
    GPBCON = (GPBCON & (~(0x03ul << 8))) | (0x01ul << 8);               /*  GPB4 ����Ϊ���             */
    GPBUP |= (0x07ul << 2);                                             /*  �ر� GPB2-4 ����������      */

    GPBDAT = (GPBDAT & (~(L3D | L3M | L3C))) | (L3C | L3M);

    return 0;
}
/*********************************************************************************************************
** Function name:           l3bus_write
** Descriptions:            ͨ�� L3 ���߽ӿڷ�������
** input parameters:        data                ����
**                          addr_mode           ��ַģʽ?
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
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
/*********************************************************************************************************
** Function name:           uda1341_init
** Descriptions:            ��ʼ�� UDA1341
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
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
    l3bus_write(0xC1, 0);                                               /*  1,0,0,0,0,0,01: ״̬1, ADC off, DAC on */

    return 0;
}
/*********************************************************************************************************
** Function name:           iis_init
** Descriptions:            ��ʼ�� IIS �ӿ�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
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
/*********************************************************************************************************
** Function name:           iis_config
** Descriptions:            ���� IIS �ӿ�
** input parameters:        channels            ������
**                          bps                 ��������
**                          fs                  ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
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
/*********************************************************************************************************
** Function name:           audio_dma_isr
** Descriptions:            DMA2 ͨ���жϷ�����
** input parameters:        interrupt           �жϺ�
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int audio_dma_isr(intno_t interrupt, void *arg)
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

        select_report(&priv->select, VFS_FILE_WRITEABLE);               /*  ����д��                    */

        if (priv->queue.len == 0) {
            sem_sync(&priv->done);                                      /*  ������                      */
        }
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           audio_open
** Descriptions:            �� audio
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
**                          oflag               �򿪱�־
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int audio_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_inc_return(dev_ref(file)) == 1) {
        /*
         * ��һ�δ�ʱ�ĳ�ʼ������
         */
        l3bus_init();

        uda1341_init();

        iis_init();

        priv->queue.next = priv->queue.prev = &priv->queue;
        priv->queue.len  = 0;

        priv->channels = 2;
        priv->bps      = 16;
        priv->fs       = 44100;

        sem_new(&priv->done, 0);

        iis_config(priv->channels, priv->bps, priv->fs);

        interrupt_install(AUDIO_DMA_INT, audio_dma_isr, NULL, NULL);

        interrupt_unmask(AUDIO_DMA_INT);

        return 0;
    } else {
        /*
         * ����豸������ͬʱ�򿪶��, ��ʹ�����´���:
         */
        atomic_dec(dev_ref(file));
        seterrno(EBUSY);
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           audio_wait_done
** Descriptions:            �ȴ���Ƶ����
** input parameters:        priv                ˽����Ϣ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void audio_wait_done(privinfo_t *priv)
{
    interrupt_mask(AUDIO_DMA_INT);

    while (priv->queue.len != 0) {
        interrupt_unmask(AUDIO_DMA_INT);

        /*
         * ���������﷢�����ж�, ������ sem_sync, ���� sem_wait �ǳ�ʱ��, ���Բ���������
         */

        sem_wait(&priv->done, 10);

        interrupt_mask(AUDIO_DMA_INT);
    }

    interrupt_unmask(AUDIO_DMA_INT);
}
/*********************************************************************************************************
** Function name:           audio_ioctl
** Descriptions:            ���� audio
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
**                          cmd                 ����
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int audio_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;
    int         value;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    audio_wait_done(priv);

    switch (cmd) {
    case AUDIO_CHANNELS_NR:
        value = (int)arg;
        if (iis_config(value, priv->bps, priv->fs) == 0) {
            priv->channels = value;
            return 0;
        }
        break;

    case AUDIO_SAMPLE_BITS:
        value = (int)arg;
        if (iis_config(priv->channels, value, priv->fs) == 0) {
            priv->bps = value;
            return 0;
        }
        break;

    case AUDIO_SAMPLE_RATE:
        value = (int)arg;
        if (iis_config(priv->channels, priv->bps, value) == 0) {
            priv->fs = value;
            return 0;
        }
        break;
    }
    seterrno(EINVAL);
    return -1;
}
/*********************************************************************************************************
** Function name:           audio_close
** Descriptions:            �ر� audio
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int audio_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    audio_wait_done(priv);

    interrupt_mask(AUDIO_DMA_INT);

    sem_free(&priv->done);

    atomic_dec(dev_ref(file));

    return 0;
}

static int audio_scan(void *ctx, file_t *file, int flags);
/*********************************************************************************************************
** Function name:           audio_write
** Descriptions:            д audio
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
**                          buf                 ����
**                          len                 ���ݳ���
** output parameters:       NONE
** Returned value:          �ɹ�д����ֽ���
*********************************************************************************************************/
static ssize_t audio_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    dma_job_t  *job;
    int         ret;
    uint32_t    queue_len;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    __again:
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    /*
     * ���û�пռ��д
     */
    interrupt_mask(AUDIO_DMA_INT);
    queue_len = priv->queue.len;
    interrupt_unmask(AUDIO_DMA_INT);

    if (queue_len >= AUDIO_OUT_QUEUE_SIZE) {
        ret = select_helper(&priv->select, audio_scan, ctx, file, VFS_FILE_WRITEABLE);
        if (ret <= 0) {
            return ret;
        } else {
            goto __again;
        }
    }

    job = (dma_job_t *)kmalloc(sizeof(dma_job_t) + len - 1, GFP_DMA);   /*  ���䲢��ʼ�� DMA ����       */
    if (job == NULL) {
        seterrno(ENOMEM);
        return -1;
    }
    memcpy(job->buf, buf, len);
    job->len = len;

    interrupt_mask(AUDIO_DMA_INT);

    priv->queue.prev->next = job;                                       /*  �� DMA ��������β           */
    job->prev              = priv->queue.prev;
    job->next              = &priv->queue;
    priv->queue.prev       = job;
    priv->queue.len       += job->len;                                  /*  ���� DMA �������ݵ��ܳ���   */

    if (priv->queue.len == job->len) {

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

    interrupt_unmask(AUDIO_DMA_INT);

    return len;
}
/*********************************************************************************************************
** Function name:           audio_scan
** Descriptions:            ɨ�� audio
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
**                          flags               �ɶ�д��־
** output parameters:       NONE
** Returned value:          �ɶ�д��־
*********************************************************************************************************/
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
/*********************************************************************************************************
** audio ����
*********************************************************************************************************/
static driver_t audio_drv = {
        .name     = "audio",
        .open     = audio_open,
        .write    = audio_write,
        .ioctl    = audio_ioctl,
        .close    = audio_close,
        .scan     = audio_scan,
        .select   = select_select,
        .unselect = select_unselect,
};
/*********************************************************************************************************
** Function name:           audio_init
** Descriptions:            ��ʼ�� audio
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int audio_init(void)
{
    privinfo_t *priv;

    driver_install(&audio_drv);

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);
        if (device_create("/dev/audio", "audio", priv) < 0) {
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
