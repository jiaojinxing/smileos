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
** File name:               s3c2440_touch.c
** Last modified Date:      2012-7-25
** Last Version:            1.0.0
** Descriptions:            S3C2440 ����������
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-25
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/select.h"
#include "drivers/input.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "s3c2440.h"
/*********************************************************************************************************
** Ӳ������
*********************************************************************************************************/
/*
 * ADCCON Register Bits
 */
#define S3C2410_ADCCON_ECFLG                (1 << 15)
#define S3C2410_ADCCON_PRSCEN               (1 << 14)
#define S3C2410_ADCCON_PRSCVL(x)            (((x) & 0xFF) << 6)
#define S3C2410_ADCCON_PRSCVLMASK           (0xFF << 6)
#define S3C2410_ADCCON_SELMUX(x)            (((x) & 0x7) << 3)
#define S3C2410_ADCCON_MUXMASK              (0x7 << 3)
#define S3C2410_ADCCON_STDBM                (1 << 2)
#define S3C2410_ADCCON_READ_START           (1 << 1)
#define S3C2410_ADCCON_ENABLE_START         (1 << 0)
#define S3C2410_ADCCON_STARTMASK            (0x3 << 0)

/*
 * ADCTSC Register Bits
 */
#define S3C2410_ADCTSC_UD_SEN               (1 << 8)                    /*  ghcstop add for s3c2440a    */
#define S3C2410_ADCTSC_YM_SEN               (1 << 7)
#define S3C2410_ADCTSC_YP_SEN               (1 << 6)
#define S3C2410_ADCTSC_XM_SEN               (1 << 5)
#define S3C2410_ADCTSC_XP_SEN               (1 << 4)
#define S3C2410_ADCTSC_PULL_UP_DISABLE      (1 << 3)
#define S3C2410_ADCTSC_AUTO_PST             (1 << 2)
#define S3C2410_ADCTSC_XY_PST(x)            (((x) & 0x3) << 0)

/*
 * ADCDAT0 Bits
 */
#define S3C2410_ADCDAT0_UPDOWN              (1 << 15)
#define S3C2410_ADCDAT0_AUTO_PST            (1 << 14)
#define S3C2410_ADCDAT0_XY_PST              (0x3 << 12)
#define S3C2410_ADCDAT0_XPDATA_MASK         (0x03FF)

/*
 * ADCDAT1 Bits
 */
#define S3C2410_ADCDAT1_UPDOWN              (1 << 15)
#define S3C2410_ADCDAT1_AUTO_PST            (1 << 14)
#define S3C2410_ADCDAT1_XY_PST              (0x3 << 12)
#define S3C2410_ADCDAT1_YPDATA_MASK         (0x03FF)

#define WAIT4INT(x)                         (((x) << 8) |   \
             S3C2410_ADCTSC_YM_SEN | S3C2410_ADCTSC_YP_SEN | S3C2410_ADCTSC_XP_SEN | \
             S3C2410_ADCTSC_XY_PST(3))

#define AUTOPST                                             \
            (S3C2410_ADCTSC_YM_SEN | S3C2410_ADCTSC_YP_SEN | S3C2410_ADCTSC_XP_SEN | \
             S3C2410_ADCTSC_AUTO_PST | S3C2410_ADCTSC_XY_PST(0))

#define TOUCH_INT                           INTADC
/*********************************************************************************************************
** ˽����Ϣ
*********************************************************************************************************/
typedef struct {
    VFS_DEVICE_MEMBERS;
    uint32_t    presc;
    uint32_t    delay;
    uint32_t    xp;
    uint32_t    yp;
    uint32_t    x;
    uint32_t    y;
    bool_t      own_adc;
    bool_t      down_msg_ok;
    bool_t      up_msg_ok;
    uint8_t     count;
} privinfo_t;
/*********************************************************************************************************
** Function name:           touch_report_event
** Descriptions:            �ر��������¼�
** input parameters:        priv                ˽����Ϣ
**                          is_down             �Ƿ���
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void touch_report_event(privinfo_t *priv, bool_t is_down)
{
    if (is_down) {
        priv->x           = priv->xp;
        priv->y           = priv->yp;
        priv->down_msg_ok = TRUE;
    } else {
        priv->up_msg_ok   = TRUE;
    }

    vfs_event_report(&priv->select, VFS_FILE_READABLE);
}
/*********************************************************************************************************
** Function name:           touch_isr
** Descriptions:            ��������̧�жϷ������
** input parameters:        interrupt           �ж�
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int touch_isr(intno_t interrupt, void *arg)
{
    privinfo_t *priv = arg;
    uint32_t    data0;
    uint32_t    data1;
    bool_t      is_down;

    /*
     * ��ȡ ADC ת�����
     */
    data0 = ADCDAT0;
    data1 = ADCDAT1;

    /*
     * �ǰ��»���̧��
     */
    is_down = (!(data0 & S3C2410_ADCDAT0_UPDOWN)) && (!(data1 & S3C2410_ADCDAT1_UPDOWN));

    if (is_down) {                                                      /*  ����ǰ���                  */

        priv->own_adc = TRUE;                                           /*  ӵ�� ADC ��Դ               */

        /*
         * �� ADC ���������ƼĴ������ó��Զ�ת��ģʽ
         */
        ADCTSC  = S3C2410_ADCTSC_PULL_UP_DISABLE | AUTOPST;

        /*
         * ���� ADC ת��
         */
        ADCCON |= S3C2410_ADCCON_ENABLE_START;

    } else {                                                            /*  �����̧��                  */

        priv->own_adc = FALSE;                                          /*  �ͷ� ADC ��Դ               */

        touch_report_event(priv, is_down);                              /*  �ر�̧���¼�                */

        /*
         * ��֮ǰ��ת��������
         */
        priv->xp    = 0;
        priv->yp    = 0;
        priv->count = 0;

        /*
         * �� ADC ���������ƼĴ������óɵȴ������ж�ģʽ
         */
        ADCTSC = WAIT4INT(0);
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           adc_isr
** Descriptions:            ADC ת�������жϷ������
** input parameters:        interrupt           �ж�
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int adc_isr(intno_t interrupt, void *arg)
{
    privinfo_t *priv = arg;
    uint32_t    data0;
    uint32_t    data1;

    if (priv->own_adc) {                                                /*  ���ӵ�� ADC ��Դ           */

        if (priv->count < 4) {
            /*
             * ��ȡ ADC ת�����
             */
            data0 = ADCDAT0;
            data1 = ADCDAT1;

            priv->xp += data0 & S3C2410_ADCDAT0_XPDATA_MASK;
            priv->yp += data1 & S3C2410_ADCDAT1_YPDATA_MASK;

            priv->count++;

            if (priv->count < 4) {
                /*
                 * �� ADC ���������ƼĴ������ó��Զ�ת��ģʽ
                 */
                ADCTSC  = S3C2410_ADCTSC_PULL_UP_DISABLE | AUTOPST;

                /*
                 * ���� ADC ת��
                 */
                ADCCON |= S3C2410_ADCCON_ENABLE_START;
            } else {
                touch_report_event(priv, TRUE);                         /*  �ر��������¼�              */

                /*
                 * ��֮ǰ��ת��������
                 */
                priv->xp    = 0;
                priv->yp    = 0;
                priv->count = 0;

                /*
                 * �� ADC ���������ƼĴ������óɵȴ�̧���ж�ģʽ
                 */
                ADCTSC = WAIT4INT(1);
            }
        }
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           adc_touch_isr
** Descriptions:            ADC ת�������ʹ�������̧�жϷ������
** input parameters:        interrupt           �ж�
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int adc_touch_isr(intno_t interrupt, void *arg)
{
    uint32_t reg = SUBSRCPND;

    if (reg & BIT_SUB_ADC) {                                            /*  ADC ת�������ж�            */
        adc_isr(interrupt, arg);
    }

    if (reg & BIT_SUB_TC) {                                             /*  ��������̧�ж�              */
        touch_isr(interrupt, arg);
    }

    SUBSRCPND |= BIT_SUB_ADC | BIT_SUB_TC;

    return 0;
}
/*********************************************************************************************************
** Function name:           touch_open
** Descriptions:            �� touch
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
**                          oflag               �򿪱�־
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int touch_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_inc_return(dev_ref(file)) == 1) {
        /*
         * ���� AD ת��Ԥ������ֵ, ʹ�� AD ת��Ԥ������
         */
        ADCCON = S3C2410_ADCCON_PRSCEN | S3C2410_ADCCON_PRSCVL(priv->presc);

        /*
         * ���� ADC ��ʼ��ʱ�Ĵ���
         */
        ADCDLY = priv->delay;

        /*
         * �� ADC ���������ƼĴ������óɵȴ������ж�ģʽ
         */
        ADCTSC = WAIT4INT(0);

        /*
         * ��װ ADC ת�������ʹ�������̧�жϷ������
         */
        interrupt_install(TOUCH_INT, adc_touch_isr, NULL, ctx);

        interrupt_unmask(TOUCH_INT);

        INTSUBMSK &= ~BIT_SUB_ADC;
        INTSUBMSK &= ~BIT_SUB_TC;

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
** Function name:           touch_close
** Descriptions:            �ر� touch
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int touch_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    atomic_dec(dev_ref(file));
    return 0;
}
/*********************************************************************************************************
** Function name:           touch_scan
** Descriptions:            ɨ�� touch
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
**                          flags               �ɶ�д��־
** output parameters:       NONE
** Returned value:          �ɶ�д��־
*********************************************************************************************************/
static int touch_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if ((priv->up_msg_ok || priv->down_msg_ok) && flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    return ret;
}
/*********************************************************************************************************
** Function name:           touch_read
** Descriptions:            �� touch
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
**                          buf                 ������
**                          len                 ��ȡ����
** output parameters:       NONE
** Returned value:          �ɹ���ȡ���ֽ���
*********************************************************************************************************/
static ssize_t touch_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    struct input_event event;
    int ret;
    bool_t msg_ok;

    if (len != sizeof(struct input_event)) {
        seterrno(EINVAL);
        return -1;
    }

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    __again:
    if (atomic_read(&priv->select.flags) & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }

    interrupt_mask(TOUCH_INT);
    msg_ok = priv->down_msg_ok || priv->up_msg_ok;
    interrupt_unmask(TOUCH_INT);

    if (!msg_ok) {                                                      /*  ���û�����ݿɶ�            */
        ret = vfs_block_helper(&priv->select, touch_scan, ctx, file, VFS_FILE_READABLE);
        if (ret <= 0) {
            return ret;
        } else {
            goto __again;
        }
    }

    interrupt_mask(TOUCH_INT);

    if (priv->down_msg_ok) {
        event.x     = priv->x;
        event.y     = priv->y;
        event.press = TRUE;
        memcpy(buf, &event, sizeof(event));
        priv->down_msg_ok = FALSE;
    } else if (priv->up_msg_ok) {
        event.x     = priv->x;
        event.y     = priv->y;
        event.press = FALSE;
        memcpy(buf, &event, sizeof(event));
        priv->up_msg_ok = FALSE;
    }

    interrupt_unmask(TOUCH_INT);

    return len;
}
/*********************************************************************************************************
** touch ����
*********************************************************************************************************/
static driver_t touch_drv = {
        .name     = "touch",
        .open     = touch_open,
        .read     = touch_read,
        .close    = touch_close,
        .scan     = touch_scan,
        .select   = select_select,
        .unselect = select_unselect,
};
/*********************************************************************************************************
** Function name:           touch_init
** Descriptions:            ��ʼ�� touch
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int touch_init(void)
{
    privinfo_t *priv;

    driver_install(&touch_drv);

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);

        priv->own_adc     = FALSE;
        priv->presc       = 128;
        priv->delay       = 0xFFFF;
        priv->count       = 0;
        priv->xp          = 0;
        priv->yp          = 0;
        priv->x           = 0;
        priv->y           = 0;
        priv->down_msg_ok = FALSE;
        priv->up_msg_ok   = FALSE;

        if (device_create("/dev/event0", "touch", priv) < 0) {
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
