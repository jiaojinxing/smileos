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
** File name:               fb.c
** Last modified Date:      2012-3-15
** Last Version:            1.0.0
** Descriptions:            FrameBuffer �豸����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-15
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
#include "vfs/utils.h"
#include <fcntl.h>
#include <string.h>
#include "drv/fb.h"
/*********************************************************************************************************
** ˽����Ϣ
*********************************************************************************************************/
typedef struct {
    VFS_DEVICE_MEMBERS;
    struct fb_var_screeninfo var;
    struct fb_fix_screeninfo fix;
    void                    *framebuffer;                               /*  ��Ƶ֡����                  */
    int  (*video_config)(const struct fb_var_screeninfo *, const struct fb_fix_screeninfo *, void *);
    int  (*video_check) (struct fb_var_screeninfo *);
    void (*video_onoff)(int on);
} privinfo_t;
/*********************************************************************************************************
** Function name:           fb_open
** Descriptions:            �� FrameBuffer
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
**                          oflag               �򿪱�־
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int fb_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_inc_return(dev_ref(file)) == 1) {
        priv->video_config(&priv->var, &priv->fix, priv->framebuffer);
        priv->video_onoff(1);
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           fb_ioctl
** Descriptions:            ���� FrameBuffer
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
**                          cmd                 ����
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int fb_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;
    int ret = -1;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    switch (cmd) {
    case FBIOGET_VSCREENINFO:
        memcpy(va_to_mva(arg), &priv->var, sizeof(priv->var));
        ret = 0;
        break;

    case FBIOGET_FSCREENINFO:
        memcpy(va_to_mva(arg), &priv->fix, sizeof(priv->fix));
        ret = 0;
        break;

    case FBIOSET_VSCREENINFO:
        if (priv->video_check(va_to_mva(arg)) == 0) {
            memcpy(&priv->var, va_to_mva(arg), sizeof(priv->var));
            priv->video_config(&priv->var, &priv->fix, priv->framebuffer);
            priv->video_onoff(1);
            ret = 0;
        } else {
            seterrno(EINVAL);
        }
        break;

    default:
        seterrno(EINVAL);
        break;
    }
    return ret;
}
/*********************************************************************************************************
** Function name:           fb_close
** Descriptions:            �ر� FrameBuffer
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int fb_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_dec_return(dev_ref(file)) == 0) {
        priv->video_onoff(0);
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           fb_fstat
** Descriptions:            ��� FrameBuffer ״̬
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
** output parameters:       buf                 ״̬�ṹ
** Returned value:          0 OR -1
*********************************************************************************************************/
static int fb_fstat(void *ctx, file_t *file, struct stat *buf)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    buf->st_size = priv->fix.smem_len;

    return 0;
}
/*********************************************************************************************************
** Function name:           fb_unlink
** Descriptions:            ɾ�� FrameBuffer
** input parameters:        ctx                 ˽����Ϣ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int fb_unlink(void *ctx)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    kfree(priv->framebuffer);

    kfree(priv);

    return 0;
}
/*********************************************************************************************************
** FrameBuffer ����
*********************************************************************************************************/
driver_t fb_drv = {
        .name   = "fb",
        .open   = fb_open,
        .ioctl  = fb_ioctl,
        .close  = fb_close,
        .fstat  = fb_fstat,
        .unlink = fb_unlink,
};
/*********************************************************************************************************
** Function name:           fb_init
** Descriptions:            ��ʼ�� FrameBuffer ����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int fb_init(void)
{
    return driver_install(&fb_drv);
}
/*********************************************************************************************************
** Function name:           fb_create
** Descriptions:            ���� FrameBuffer �豸
** input parameters:        path                FrameBuffer �豸·��
**                          var                 �ɱ���Ļ��Ϣ
**                          video_config        ��Ƶģʽ���ú���
**                          video_check         ��Ƶģʽ�����������
**                          video_onoff         ��Ƶ���غ���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int fb_create(const char *path,
              const struct fb_var_screeninfo *var,
              int  (*video_config)(const struct fb_var_screeninfo *, const struct fb_fix_screeninfo *, void *),
              int  (*video_check)(struct fb_var_screeninfo *),
              void (*video_onoff)(int))
{
    struct fb_fix_screeninfo *fix;
    privinfo_t *priv;

    if (path == NULL ||
        var  == NULL ||
        video_config == NULL ||
        video_check  == NULL ||
        video_onoff  == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (var->xres_virtual   == 0 ||
        var->yres_virtual   == 0 ||
        var->bits_per_pixel == 0) {
        seterrno(EINVAL);
        return -1;
    }

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);

        priv->video_config  = video_config;
        priv->video_check   = video_check;
        priv->video_onoff   = video_onoff;

        memcpy(&priv->var, var, sizeof(priv->var));

        fix = &priv->fix;

        fix->smem_len       = ((var->bits_per_pixel + 7) / 8) * var->xres_virtual * var->yres_virtual;

        priv->framebuffer   = kmalloc(fix->smem_len, GFP_SHARE | GFP_DMA);
        if (priv->framebuffer == NULL) {
            kfree(priv);
            seterrno(ENOMEM);
            return -1;
        }

        fix->smem_start     = priv->framebuffer;
        fix->xpanstep       = 0;
        fix->ypanstep       = 0;
        fix->ywrapstep      = 0;

        if (device_create(path, "fb", priv) < 0) {
            kfree(priv->framebuffer);
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
