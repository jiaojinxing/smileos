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
** File name:               fifo.c
** Last modified Date:      2012-4-6
** Last Version:            1.0.0
** Descriptions:            FIFO 设备驱动
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
#include "kern/kfifo.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/select.h"
#include <sys/stat.h>

/*
 * 私有信息
 */
typedef struct {
    VFS_DEVICE_MEMBERS;
    kfifo_t     fifo;
} privinfo_t;

static int fifo_scan(void *ctx, file_t *file, int flags);
static int fifo_unlink(void *ctx);

/*
 * 打开 fifo
 */
static int fifo_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;
    int val;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    val = atomic_inc_return(dev_ref(file));
    if (val > 2) {
        atomic_dec(dev_ref(file));
        seterrno(EBUSY);
        return -1;
    }
    return 0;
}

/*
 * 关闭 fifo
 */
static int fifo_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;
    reg_t reg;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    reg = interrupt_disable();

    if (atomic_dec_and_test(dev_ref(file))) {

        device_remove(file->ctx);

        file->ctx = NULL;

        fifo_unlink(ctx);
    }

    interrupt_resume(reg);

    return 0;
}

/*
 * 读 fifo
 */
static ssize_t fifo_read(void *ctx, file_t *file, void *buf, size_t len)
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

    if (kfifo_is_empty(&priv->fifo)) {                                  /*  如果没有数据可读            */
        ret = vfs_block_helper(&priv->select, fifo_scan, ctx, file, VFS_FILE_READABLE);
        if (ret <= 0) {
            return ret;
        } else {
            goto __again;
        }
    }

    len = kfifo_get(&priv->fifo, buf, len);

    if (len > 0) {
        vfs_event_report(&priv->select, VFS_FILE_WRITEABLE);
    }

    return len;
}

/*
 * 写 fifo
 */
static ssize_t fifo_write(void *ctx, file_t *file, const void *buf, size_t len)
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

    if (kfifo_is_full(&priv->fifo)) {                                   /*  如果没有空间可写            */
        ret = vfs_block_helper(&priv->select, fifo_scan, ctx, file, VFS_FILE_WRITEABLE);
        if (ret <= 0) {
            return ret;
        } else {
            goto __again;
        }
    }

    len = kfifo_put(&priv->fifo, buf, len);

    if (len > 0) {
        vfs_event_report(&priv->select, VFS_FILE_READABLE);
    }

    return len;
}

/*
 * 扫描 fifo
 */
static int fifo_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if ((!kfifo_is_empty(&priv->fifo)) && flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if ((!kfifo_is_full(&priv->fifo)) && flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    if (atomic_read(&priv->select.flags) & flags & VFS_FILE_ERROR) {
        ret |= VFS_FILE_ERROR;
    }
    return ret;
}

/*
 * 删除 fifo
 */
static int fifo_unlink(void *ctx)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    kfifo_free(&priv->fifo);

    kfree(priv);

    return 0;
}

/*
 * 获得 fifo 状态
 */
static int fifo_fstat(void *ctx, file_t *file, struct stat *buf)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    buf->st_mode = (buf->st_mode & (~S_IFMT)) | S_IFIFO;
    buf->st_size = priv->fifo.size;

    return 0;
}

/*
 * fifo 驱动
 */
static driver_t fifo_drv = {
        .name     = "fifo",
        .open     = fifo_open,
        .write    = fifo_write,
        .read     = fifo_read,
        .close    = fifo_close,
        .scan     = fifo_scan,
        .unlink   = fifo_unlink,
        .fstat    = fifo_fstat,
        .select   = select_select,
        .unselect = select_unselect,
};
/*********************************************************************************************************
** Function name:           fifo_init
** Descriptions:            初始化 fifo 驱动
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int fifo_init(void)
{
    return driver_install(&fifo_drv);
}
/*********************************************************************************************************
** Function name:           fifo_create
** Descriptions:            创建 fifo 设备
** input parameters:        path                fifo 设备路径
**                          size                fifo 大小
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int fifo_create(const char *path, size_t size)
{
    privinfo_t *priv;

    if (path == NULL || size == 0) {
        seterrno(EINVAL);
        return -1;
    }

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        device_init(priv);

        if (kfifo_init(&priv->fifo, size) < 0) {
            kfree(priv);
            seterrno(ENOMEM);
            return -1;
        }

        if (device_create(path, "fifo", priv) < 0) {
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

#include <fcntl.h>
#include <stdio.h>
#include "vfs/vfs.h"
/*********************************************************************************************************
** Function name:           pipe_create
** Descriptions:            创建 PIPE
** input parameters:        NONE
** output parameters:       fds                 文件描述符
** Returned value:          0 OR -1
*********************************************************************************************************/
int pipe_create(int fds[2])
{
    static char path[NAME_MAX];
    static int key = 0;
    int err;
    int _key;
    reg_t reg;

    if (fds == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    again:

    reg = interrupt_disable();
    _key = key++;
    snprintf(path, sizeof(NAME_MAX), "/dev/pipe%d", _key);

    if (fifo_create(path, 4 * KB) < 0) {
        interrupt_resume(reg);
        if (errno == EEXIST) {
            goto again;
        }
        return -1;
    }

    fds[0] = vfs_open(path, O_RDONLY, 0666);
    if (fds[0] < 0) {
        geterrno(err);
        vfs_unlink(path);
        seterrno(err);
        interrupt_resume(reg);
        return -1;
    }

    fds[1] = vfs_open(path, O_WRONLY, 0666);
    if (fds[1] < 0) {
        geterrno(err);
        vfs_close(fds[0]);
        vfs_unlink(path);
        seterrno(err);
        interrupt_resume(reg);
        return -1;
    }

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
