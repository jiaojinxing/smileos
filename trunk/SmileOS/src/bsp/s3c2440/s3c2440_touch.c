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
** File name:               s3c2440_touch.c
** Last modified Date:      2012-7-25
** Last Version:            1.0.0
** Descriptions:            S3C2440 触摸屏驱动
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-25
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
#include "kern/kern.h"
#include <linux/input.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "s3c2440.h"
#include "s3c2440_int.h"

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

/*
 * 私有信息
 */
typedef struct {
    VFS_SELECT_MEMBERS;
    uint8_t     ref;
    int         mode;
    uint8_t     own_adc;
    uint8_t     presc;
    uint16_t    delay;
    uint32_t    count;
    uint32_t    xp;
    uint32_t    yp;
    uint32_t    x;
    uint32_t    y;
    uint8_t     down_msg_ok;
    uint8_t     up_msg_ok;
} privinfo_t;

#include "drv/selectdrv.h"

/*
 * 回报触摸屏事件
 */
static void touch_report_event(privinfo_t *priv, int is_down)
{
    if (is_down) {
        priv->x           = priv->xp;
        priv->y           = priv->yp;
        priv->down_msg_ok = TRUE;
    } else {
        priv->up_msg_ok   = TRUE;
    }

    select_report(priv, VFS_FILE_READABLE);
}

/*
 * 触摸屏按抬中断服务程序
 */
static int touch_isr(void *arg)
{
    privinfo_t *priv = arg;
    uint32_t    data0;
    uint32_t    data1;
    int         is_down;

    /*
     * 读取 ADC 转换结果
     */
    data0 = ADCDAT0;
    data1 = ADCDAT1;

    /*
     * 是按下还是抬起
     */
    is_down = (!(data0 & S3C2410_ADCDAT0_UPDOWN)) && (!(data1 & S3C2410_ADCDAT1_UPDOWN));

    if (is_down) {                                                      /*  如果是按下                  */

        priv->own_adc = TRUE;                                           /*  拥有 ADC 资源               */

        /*
         * 将 ADC 触摸屏控制寄存器设置成自动转换模式
         */
        ADCTSC  = S3C2410_ADCTSC_PULL_UP_DISABLE | AUTOPST;

        /*
         * 启动 ADC 转换
         */
        ADCCON |= S3C2410_ADCCON_ENABLE_START;

    } else {                                                            /*  如果是抬起                  */

        priv->own_adc = FALSE;                                          /*  释放 ADC 资源               */

        touch_report_event(priv, is_down);                              /*  回报抬起事件                */

        /*
         * 将之前的转换结果清掉
         */
        priv->xp    = 0;
        priv->yp    = 0;
        priv->count = 0;

        /*
         * 将 ADC 触摸屏控制寄存器设置成等待按下中断模式
         */
        ADCTSC = WAIT4INT(0);
    }
    return 0;
}

/*
 * ADC 转换结束中断服务程序
 */
static int adc_isr(void *arg)
{
    privinfo_t *priv = arg;
    uint32_t    data0;
    uint32_t    data1;

    if (priv->own_adc) {                                                /*  如果拥有 ADC 资源           */

        if (priv->count < 4) {
            /*
             * 读取 ADC 转换结果
             */
            data0 = ADCDAT0;
            data1 = ADCDAT1;

            priv->xp += data0 & S3C2410_ADCDAT0_XPDATA_MASK;
            priv->yp += data1 & S3C2410_ADCDAT1_YPDATA_MASK;

            priv->count++;

            if (priv->count < 4) {
                /*
                 * 将 ADC 触摸屏控制寄存器设置成自动转换模式
                 */
                ADCTSC  = S3C2410_ADCTSC_PULL_UP_DISABLE | AUTOPST;

                /*
                 * 启动 ADC 转换
                 */
                ADCCON |= S3C2410_ADCCON_ENABLE_START;
            } else {
                touch_report_event(priv, TRUE);                         /*  回报触摸屏事件              */

                /*
                 * 将之前的转换结果清掉
                 */
                priv->xp    = 0;
                priv->yp    = 0;
                priv->count = 0;

                /*
                 * 将 ADC 触摸屏控制寄存器设置成等待抬起中断模式
                 */
                ADCTSC = WAIT4INT(1);
            }
        }
    }
    return 0;
}

/*
 * ADC 转换结束和触摸屏按抬中断服务程序
 */
static int adc_touch_isr(uint32_t interrupt, void *arg)
{
    if (SUBSRCPND & BIT_SUB_ADC) {                                      /*  ADC 转换结束中断            */
        adc_isr(arg);
        SUBSRCPND |= BIT_SUB_ADC;
    }

    if (SUBSRCPND & BIT_SUB_TC) {                                       /*  触摸屏按抬中断              */
        touch_isr(arg);
        SUBSRCPND |= BIT_SUB_TC;
    }
    return 0;
}

/*
 * 打开 touch
 */
static int touch_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    /*
     * 只能打开一次
     */
    if (priv->ref != 0) {
        seterrno(EBUSY);
        return -1;
    }
    priv->ref++;

    /*
     * 设置 AD 转换预定标器值, 使能 AD 转换预定标器
     */
    ADCCON = S3C2410_ADCCON_PRSCEN | S3C2410_ADCCON_PRSCVL(priv->presc);

    /*
     * 设置 ADC 开始延时寄存器
     */
    ADCDLY = priv->delay;

    /*
     * 将 ADC 触摸屏控制寄存器设置成等待按下中断模式
     */
    ADCTSC = WAIT4INT(0);

    /*
     * 安装 ADC 转换结束和触摸屏按抬中断服务程序
     */
    interrupt_install(INTADC, adc_touch_isr, NULL, ctx);

    interrupt_unmask(INTADC);

    INTSUBMSK &= ~BIT_SUB_ADC;
    INTSUBMSK &= ~BIT_SUB_TC;

    return 0;
}

/*
 * 控制 touch
 */
static int touch_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }
    return 0;
}

/*
 * 关闭 touch
 */
static int touch_close(void *ctx, file_t *file)
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
 * 扫描
 */
static int touch_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if (priv->flags & flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if (priv->flags & flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    if (priv->flags & flags & VFS_FILE_ERROR) {
        ret |= VFS_FILE_ERROR;
    }
    return ret;
}

/*
 * 读 touch
 */
static ssize_t touch_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    struct input_event event;
    int ret;
    int resume_type;
    int len_backup;

    if ((len == 0) || ((len % sizeof(struct input_event)) != 0)) {
        seterrno(EINVAL);
        return -1;
    }
    len_backup = len;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    __again:
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }
    if (priv->flags & VFS_FILE_READABLE) {
        if (priv->down_msg_ok) {
            event.x     = priv->x;
            event.y     = priv->y;
            event.press = TRUE;
            memcpy(buf, &event, sizeof(event));
            priv->down_msg_ok = FALSE;
            len -= sizeof(event);
            buf  = (char *)buf + sizeof(event);

            if (len == 0) {
                if (!priv->up_msg_ok) {
                    priv->flags &= ~VFS_FILE_READABLE;
                }
                return len_backup;
            }
        }

        if (priv->up_msg_ok) {
            event.x     = priv->x;
            event.y     = priv->y;
            event.press = FALSE;
            memcpy(buf, &event, sizeof(event));
            priv->up_msg_ok = FALSE;
            len -= sizeof(event);
            buf  = (char *)buf + sizeof(event);
            if (!priv->down_msg_ok) {
                priv->flags &= ~VFS_FILE_READABLE;
            }
            if (len != 0) {
                goto __again;
            }
            return len_backup;
        } else {
            priv->flags &= ~VFS_FILE_READABLE;
            goto __select;
        }
    } else {
        __select:
        if (priv->mode & O_NONBLOCK) {
            seterrno(EAGAIN);
            return 0;
        } else {
            ret = select_select(priv, file, VFS_FILE_READABLE);
            if (ret < 0) {
                seterrno(ENOMEM);
                return -1;
            }

            current->timer   = 0;
            current->state   = TASK_SUSPEND;

            current->resume_type = TASK_RESUME_UNKNOW;

            yield();

            select_unselect(priv, file, VFS_FILE_READABLE);

            current->timer       = 0;
            resume_type          = current->resume_type;
            current->resume_type = TASK_RESUME_UNKNOW;

            if (resume_type & TASK_RESUME_SELECT_EVENT) {
                ret = touch_scan(priv, file, VFS_FILE_READABLE);
                if (ret & VFS_FILE_READABLE) {
                    goto __again;
                } else {
                    seterrno(ENODATA);
                    return 0;
                }
            } else {
                seterrno(EINTR);
                return -1;
            }
        }
    }
}

/*
 * touch 驱动
 */
static driver_t touch_drv = {
        .name     = "touch",
        .open     = touch_open,
        .read     = touch_read,
        .ioctl    = touch_ioctl,
        .close    = touch_close,
        .scan     = touch_scan,
        .select   = select_select,
        .unselect = select_unselect,
};

/*
 * 初始化 touch
 */
int touch_init(void)
{
    privinfo_t *priv;

    driver_install(&touch_drv);

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        select_init(priv);

        priv->ref         = 0;
        priv->mode        = 0;
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
        return -1;
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
