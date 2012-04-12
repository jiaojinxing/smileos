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
** File name:               ttyS0.c
** Last modified Date:      2012-3-31
** Last Version:            1.0.0
** Descriptions:            TTYS0 �������豸
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-31
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "kern/kern.h"
#include <errno.h>
#include "tty.h"

typedef struct tty privinfo_t;

/*
 * �� ttyS0
 */
static int ttyS0_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * ���� ttyS0
 */
static int ttyS0_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = tty_ioctl(priv, cmd, arg);
    if (ret != 0) {
        seterrno(ret);
        return -1;
    }
    return 0;
}

/*
 * �ر� ttyS0
 */
static int ttyS0_close(void *ctx, file_t *file)
{
    return 0;
}

/*
 * ttyS0 �ǲ���һ�� tty
 */
static int ttyS0_isatty(void *ctx, file_t *file)
{
    return 1;
}

/*
 * �� ttyS0
 */
static ssize_t ttyS0_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = tty_read(priv, buf, &len);
    if (ret != 0) {
        seterrno(ret);
        return -1;
    } else {
        return len;
    }
}

/*
 * д ttyS0
 */
static ssize_t ttyS0_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = tty_write(priv, buf, &len);
    if (ret != 0) {
        seterrno(ret);
        return -1;
    } else {
        return len;
    }
}

/*
 * �������
 */
static void ttyS0_start(struct tty *tp)
{
    int c;

    while ((c = tty_getc(&tp->t_outq)) >= 0) {
        kputc(c);
    }
}

/*
 * ttyS0 ����
 */
static driver_t ttyS0_drv = {
        .name   = "ttyS0",
        .open   = ttyS0_open,
        .write  = ttyS0_write,
        .read   = ttyS0_read,
        .isatty = ttyS0_isatty,
        .ioctl  = ttyS0_ioctl,
        .close  = ttyS0_close,
};

/*
 * ��ʼ�� ttyS0
 */
int ttyS0_init(void)
{
    static privinfo_t ttyS0;

    driver_install(&ttyS0_drv);

    device_create("/dev/ttyS0", "ttyS0", &ttyS0);

    tty_attach(&ttyS0);

    ttyS0.t_oproc = ttyS0_start;

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/