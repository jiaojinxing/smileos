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
** Descriptions:            socket �豸����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-28
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
#include "vfs/vfs.h"
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

/*
 * ˽����Ϣ
 */
typedef struct {
    VFS_DEVICE_MEMBERS;
    int             sock_fd;
} privinfo_t;

extern int lwip_socket_set_ctx(int sock_fd, void *ctx);

/*
 * �� socket
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
         * ����豸������ͬʱ�򿪶��, ��ʹ�����´���:
         */
        atomic_dec(dev_ref(file));
        seterrno(EBUSY);
        return -1;
    }
}

/*
 * �ر� socket
 */
static int socket_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;
    char name[PATH_MAX];
    uint32_t reg;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    sprintf(name, "/dev/socket%d", priv->sock_fd);

    reg = interrupt_disable();

    atomic_dec(dev_ref(file));

    vfs_unlink(name);

    lwip_socket_set_ctx(priv->sock_fd, NULL);

    interrupt_resume(reg);

    lwip_close(priv->sock_fd);

    kfree(priv);

    return 0;
}

/*
 * ���� socket
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

    return lwip_ioctl(priv->sock_fd, cmd, va_to_mva(arg));
}

/*
 * ���� socket
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
 * �� socket
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
 * д socket
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
 * ��� socket ״̬
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
 * ɨ�� socket
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
 * socket ����
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
** Descriptions:            �ر� socket �¼�(�� lwIP �ڰ�ȫ������ʹ��)
** input parameters:        sock_fd             socket ��˽���ļ�������
**                          type                �¼�����
**                          ctx                 ������
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void smileos_socket_report(int sock_fd, int type, void *ctx)
{
    privinfo_t *priv = ctx;

    if (priv != NULL) {
        select_report(&priv->select, type);
    }
}
/*********************************************************************************************************
** Function name:           socket_init
** Descriptions:            ��ʼ�� socket ����
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
** Descriptions:            ���� socket
** input parameters:        sock_fd             socket ��˽���ļ�������
** output parameters:       NONE
** Returned value:          IO ϵͳ�ļ�������
*********************************************************************************************************/
int socket_attach(int sock_fd)
{
    char name[PATH_MAX];
    int fd;
    privinfo_t *priv;
    uint32_t reg;

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        priv->sock_fd = sock_fd;

        device_init(priv);

        sprintf(name, "/dev/socket%d", sock_fd);

        reg = interrupt_disable();

        if (device_create(name, "socket", priv) < 0) {
            interrupt_resume(reg);
            kfree(priv);
            return -1;
        }

        fd = vfs_open(name, O_RDWR, 0666);
        if (fd < 0) {
            vfs_unlink(name);
            interrupt_resume(reg);
            kfree(priv);
            return -1;
        }

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
** Descriptions:            ��� socket ��˽���ļ�������
** input parameters:        fd                  IO ϵͳ�ļ�������
** output parameters:       NONE
** Returned value:          socket ��˽���ļ�������
*********************************************************************************************************/
int socket_priv_fd(int fd)
{
    file_t *file;
    device_t *dev;
    privinfo_t *priv;
    int sock_fd;

    file = vfs_get_file(fd);
    if (file != NULL) {
        dev = file->ctx;
        if (dev != NULL) {
            priv = dev->ctx;
            if (priv != NULL) {
                sock_fd = priv->sock_fd;
                seterrno(0);
                return sock_fd;
            }
        }
    }
    seterrno(EBADFD);
    return -1;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
