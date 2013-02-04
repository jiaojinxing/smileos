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
** File name:               mtdblock.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            MTD ���豸����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-27
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
#include <sys/stat.h>
#include <linux/mtd/mtd.h>
/*********************************************************************************************************
** ˽����Ϣ
*********************************************************************************************************/
typedef struct {
    VFS_DEVICE_MEMBERS;
    int                 start;                          /* Start block we're allowed to use             */
    int                 end;                            /* End block we're allowed to use               */
    int                 reserved;                       /* We want this tuneable so that we can reduce  */
    struct mtd_info    *mtd;
} privinfo_t;

static mutex_t          mtd_lock;
/*********************************************************************************************************
** Function name:           mtdblock_open
** Descriptions:            �� mtdblock
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
**                          oflag               �򿪱�־
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int mtdblock_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    atomic_inc(dev_ref(file));
    return 0;
}
/*********************************************************************************************************
** Function name:           mtdblock_close
** Descriptions:            �ر� mtdblock
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int mtdblock_close(void *ctx, file_t *file)
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
** Function name:           mtdblock_fstat
** Descriptions:            ��� mtdblock ״̬
** input parameters:        ctx                 ˽����Ϣ
**                          file                �ļ��ṹ
** output parameters:       buf                 ״̬�ṹ
** Returned value:          0 OR -1
*********************************************************************************************************/
static int mtdblock_fstat(void *ctx, file_t *file, struct stat *buf)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    buf->st_mode      = (buf->st_mode & (~S_IFMT)) | S_IFBLK;
    buf->st_blocks    = priv->end - priv->start + 1;
    buf->st_blksize   = priv->mtd->erasesize;
    buf->st_size      = buf->st_blocks * buf->st_blksize;

    buf->st_spare1    = (uint32_t)priv->mtd;
    buf->st_spare2    = priv->reserved;
    buf->st_spare4[0] = priv->start;
    buf->st_spare4[1] = priv->end;

    return 0;
}
/*********************************************************************************************************
** Function name:           mtdblock_unlink
** Descriptions:            ɾ�� mtdblock
** input parameters:        ctx                 ˽����Ϣ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int mtdblock_unlink(void *ctx)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    mutex_lock(&mtd_lock, 0);
    put_mtd_device(priv->mtd);
    mutex_unlock(&mtd_lock);

    kfree(priv);

    return 0;
}
/*********************************************************************************************************
** mtdblock ����
*********************************************************************************************************/
static driver_t mtdblock_drv = {
        .name     = "mtdblock",
        .open     = mtdblock_open,
        .close    = mtdblock_close,
        .fstat    = mtdblock_fstat,
        .unlink   = mtdblock_unlink,
};
/*********************************************************************************************************
** Function name:           mtdblock_init
** Descriptions:            ��ʼ�� MTD ������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mtdblock_init(void)
{
    mutex_new(&mtd_lock);

    return driver_install(&mtdblock_drv);
}
/*********************************************************************************************************
** Function name:           mtdblock_create
** Descriptions:            ���� MTD ���豸
** input parameters:        path                MTD ���豸·��
**                          mtd_no              MTD �豸��
**                          start               ��ʼ���
**                          end                 �������
**                          reserved            ��������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mtdblock_create(const char *path, uint32_t mtd_no, uint32_t start, uint32_t end, uint32_t reserved)
{
    privinfo_t *priv;

    if (path == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);

        priv->reserved = reserved;
        priv->start    = start;
        priv->end      = end;

        mutex_lock(&mtd_lock, 0);
        priv->mtd = get_mtd_device(NULL, mtd_no);
        mutex_unlock(&mtd_lock);

        if (priv->mtd == NULL) {

            kfree(priv);
            seterrno(EINVAL);
            return -1;
        }

        if (device_create(path, "mtdblock", priv) < 0) {
            mutex_lock(&mtd_lock, 0);
            put_mtd_device(priv->mtd);
            mutex_unlock(&mtd_lock);
            kfree(priv);
            return -1;
        }
        seterrno(0);
        return 0;
    } else {
        seterrno(ENOMEM);
        return -1;
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
