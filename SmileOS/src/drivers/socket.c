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
** File name:               socket.c
** Last modified Date:      2012-3-28
** Last Version:            1.0.0
** Descriptions:            socket 设备驱动
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-28
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
#include "vfs/vfs.h"
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

/*
 * 私有信息
 */
typedef struct {
    VFS_DEVICE_MEMBERS;
    int             sock_fd;
} privinfo_t;

extern int lwip_socket_set_ctx(int sock_fd, void *ctx);

/*
 * 打开 socket
 */
static int socket_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (atomic_inc_return(dev_ref(file)) == 1) {
        return 0;
    } else {
        /*
         * 如果设备不允许同时打开多次, 请使用如下代码:
         */
        atomic_dec(dev_ref(file));
        seterrno(EBUSY);
        return -1;
    }
}

/*
 * 关闭 socket
 */
static int socket_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;
    reg_t reg;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    reg = interrupt_disable();

    atomic_dec(dev_ref(file));

    device_remove(file->ctx);

    file->ctx = NULL;

    lwip_close(priv->sock_fd);

    kfree(priv);

    interrupt_resume(reg);

    return 0;
}

/*
 * 控制 socket
 */
static int socket_ioctl(void *ctx, file_t *file, int cmd, void *arg)
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

    return lwip_ioctl(priv->sock_fd, cmd, ua_to_ka(arg));
}

/*
 * 控制 socket
 */
static int socket_fcntl(void *ctx, file_t *file, int cmd, int arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    return lwip_fcntl(priv->sock_fd, cmd, arg);
}

/*
 * 读 socket
 */
static ssize_t socket_read(void *ctx, file_t *file, void *buf, size_t len)
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

    return lwip_recv(priv->sock_fd, buf, len, 0);
}

/*
 * 写 socket
 */
static ssize_t socket_write(void *ctx, file_t *file, const void *buf, size_t len)
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

    return lwip_send(priv->sock_fd, buf, len, 0);
}

/*
 * 获得 socket 状态
 */
static int socket_fstat(void *ctx, file_t *file, struct stat *buf)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    buf->st_mode = (buf->st_mode & (~S_IFMT)) | S_IFSOCK;

    return 0;
}

/*
 * 扫描 socket
 */
static int socket_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;
    int readable;
    int writeable;
    int error;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    extern int socket_stat(int sock_fd, int *readable, int *writeable, int *error);
    ret = socket_stat(priv->sock_fd, &readable, &writeable, &error);
    if (ret < 0) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if (readable && flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if (writeable && flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    if (error && flags & VFS_FILE_ERROR) {
        ret |= VFS_FILE_ERROR;
    }
    return ret;
}

/*
 * socket 驱动
 */
driver_t socket_drv = {
        .name     = "socket",
        .open     = socket_open,
        .write    = socket_write,
        .read     = socket_read,
        .ioctl    = socket_ioctl,
        .fcntl    = socket_fcntl,
        .close    = socket_close,
        .fstat    = socket_fstat,
        .scan     = socket_scan,
        .select   = select_select,
        .unselect = select_unselect,
};
/*********************************************************************************************************
** Function name:           smileos_socket_report
** Descriptions:            回报 socket 事件(供 lwIP 在安全环境下使用)
** input parameters:        sock_fd             socket 的私有文件描述符
**                          type                事件类型
**                          ctx                 上下文
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void smileos_socket_report(int sock_fd, int type, void *ctx)
{
    privinfo_t *priv = ctx;

    if (priv != NULL) {
        vfs_event_report(&priv->select, type);
    }
}
/*********************************************************************************************************
** Function name:           socket_init
** Descriptions:            初始化 socket 驱动
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int socket_init(void)
{
    return driver_install(&socket_drv);
}
/*********************************************************************************************************
** Function name:           socket_attach
** Descriptions:            联结 socket
** input parameters:        sock_fd             socket 的私有文件描述符
** output parameters:       NONE
** Returned value:          IO 系统文件描述符
*********************************************************************************************************/
int socket_attach(int sock_fd)
{
    char path[PATH_MAX];
    int fd;
    privinfo_t *priv;
    reg_t reg;
    file_t *file;
    int err;

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        priv->sock_fd = sock_fd;

        device_init(priv);

        sprintf(path, "/dev/socket%d", sock_fd);

        reg = interrupt_disable();

        if (device_create(path, "socket", priv) < 0) {
            interrupt_resume(reg);
            kfree(priv);
            return -1;
        }

        fd = vfs_open(path, O_RDWR, 0666);
        if (fd < 0) {
            geterrno(err);
            vfs_unlink(path);
            seterrno(err);
            interrupt_resume(reg);
            kfree(priv);
            return -1;
        }

        file = vfs_get_file(fd);
        if (file == NULL) {
            geterrno(err);
            vfs_close(fd);
            vfs_unlink(path);
            seterrno(err);
            interrupt_resume(reg);
            kfree(priv);
            return -1;
        }

        file->type = VFS_FILE_TYPE_SOCK;
        vfs_put_file(file);

        lwip_socket_set_ctx(sock_fd, priv);

        interrupt_resume(reg);
        seterrno(0);
        return fd;
    } else {
        seterrno(ENOMEM);
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           socket_priv_fd
** Descriptions:            获得 socket 的私有文件描述符
** input parameters:        fd                  IO 系统文件描述符
** output parameters:       ctx                 上下文
** Returned value:          socket 的私有文件描述符
*********************************************************************************************************/
int socket_priv_fd(int fd, void **ctx)
{
    file_t *file;
    device_t *dev;
    privinfo_t *priv;
    int sock_fd;

    file = vfs_get_file(fd);
    if (file != NULL) {
        if (file->type & VFS_FILE_TYPE_SOCK) {
            dev = file->ctx;
            if (dev != NULL) {
                priv = dev->ctx;
                if (priv != NULL) {
                    sock_fd = priv->sock_fd;
                    *ctx = file;
                    seterrno(0);
                    return sock_fd;
                }
            }
        }
        seterrno(EFTYPE);
    }
    return -1;
}
/*********************************************************************************************************
** Function name:           socket_op_end
** Descriptions:            socket 操作结束
** input parameters:        ctx                 上下文
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void socket_op_end(void *ctx)
{
    file_t *file = ctx;

    if (file != NULL && (file->type & VFS_FILE_TYPE_SOCK)) {
        vfs_put_file(file);
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
