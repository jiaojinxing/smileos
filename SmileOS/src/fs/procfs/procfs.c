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
** File name:               procfs.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            PROC 文件系统
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-20
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
#include "kern/ipc.h"
#include "kern/atomic.h"
#include "vfs/config.h"
#include "vfs/types.h"
#include "vfs/vfs.h"
#include "vfs/procfs.h"
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
/*********************************************************************************************************
** 节点类型
*********************************************************************************************************/
typedef enum {
    PROCFS_NODE_TYPE_FILE,
    PROCFS_NODE_TYPE_DIR,
} procfs_node_type;
/*********************************************************************************************************
** 节点
*********************************************************************************************************/
struct _procfs_node {
    procfs_node_type            type;
    struct _procfs_node        *son;
    struct _procfs_node        *next;
    procfs_node_read_t          read;
    procfs_node_write_t         write;
    void                       *ctx;
    char                        name[NAME_MAX];
    atomic_t                    ref;
};
/*********************************************************************************************************
** 内部变量
*********************************************************************************************************/
static procfs_node_t            procfs_root_node;
static mutex_t                  procfs_node_mgr_lock;
static mutex_t                  procfs_node_access_lock;
/*********************************************************************************************************
** 私有信息
*********************************************************************************************************/
typedef struct {
    struct dirent               entry;
    long                        loc;
    procfs_node_t              *node;
} privinfo_t;

static int procfs_mount(mount_point_t *point, device_t *dev, const char *dev_name, const char *param)
{
    procfs_node_t *node = &procfs_root_node;

    node->type  = PROCFS_NODE_TYPE_DIR;
    node->read  = NULL;
    node->write = NULL;
    node->ctx   = NULL;
    atomic_set(&node->ref, 0);
    node->son   = NULL;
    node->next  = NULL;
    strcpy(node->name, "/");

    return mutex_create(&procfs_node_mgr_lock) || mutex_create(&procfs_node_access_lock);
}

static procfs_node_t *procfs_node_lookup(const char *path, procfs_node_type type)
{
    char  temp[PATH_MAX];
    char *buf;
    char *ptr;
    procfs_node_t *dir_node;
    procfs_node_t *node;

    if (VFS_PATH_IS_ROOT(path)) {
        if (type == PROCFS_NODE_TYPE_DIR) {
            return &procfs_root_node;
        }
    } else {
        strcpy(temp, path + 1);

        buf      = temp;

        dir_node = &procfs_root_node;

        node     = NULL;

        mutex_lock(&procfs_node_mgr_lock, 0);

        while ((buf = strtok_r(buf, "/", &ptr)) != NULL) {

            for (node = dir_node->son; node != NULL; node = node->next) {
                if (strcmp(node->name, buf) == 0 && node->type == type) {
                    break;
                }
            }

            if (node == NULL) {
                mutex_unlock(&procfs_node_mgr_lock);
                return NULL;
            }

            dir_node = node;
            buf      = NULL;
        }

        mutex_unlock(&procfs_node_mgr_lock);

        return node;
    }

    return NULL;
}

static procfs_node_t *procfs_node_lookup_ref(const char *path, procfs_node_type type)
{
    procfs_node_t *node;

    mutex_lock(&procfs_node_mgr_lock, 0);

    node = procfs_node_lookup(path, type);
    if (node != NULL) {
        atomic_inc(&node->ref);
    }

    mutex_unlock(&procfs_node_mgr_lock);

    return node;
}

static procfs_node_t *procfs_node_get(procfs_node_t *dir_node, long index)
{
    long i;
    procfs_node_t *node;

    mutex_lock(&procfs_node_mgr_lock, 0);

    for (i = 0, node = dir_node->son; i < index && node != NULL; i++, node = node->next) {
    }

    mutex_unlock(&procfs_node_mgr_lock);

    return node;
}

static int procfs_open(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode)
{
    procfs_node_t *node;
    privinfo_t *priv;

    node = procfs_node_lookup_ref(path, PROCFS_NODE_TYPE_FILE);
    if (node == NULL) {
        seterrno(ENOENT);
        return -1;
    }

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        file->ctx  = priv;
        priv->loc  = 0;
        priv->node = node;
        return 0;
    } else {
        atomic_dec(&node->ref);
        seterrno(ENOMEM);
        return -1;
    }
}

static ssize_t procfs_read(mount_point_t *point, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = file->ctx;
    procfs_node_t *node;
    ssize_t ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = priv->node;
    if (node == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (node->read == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    mutex_lock(&procfs_node_access_lock, 0);

    ret = node->read(node->ctx, buf, len);

    mutex_unlock(&procfs_node_access_lock);

    return ret;
}

static ssize_t procfs_write(mount_point_t *point, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = file->ctx;
    procfs_node_t *node;
    ssize_t ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = priv->node;
    if (node == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (node->write == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    mutex_lock(&procfs_node_access_lock, 0);

    ret = node->write(node->ctx, buf, len);

    mutex_unlock(&procfs_node_access_lock);

    return ret;
}

static int procfs_close(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;
    procfs_node_t *node;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = priv->node;
    if (node == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    atomic_dec(&node->ref);

    kfree(priv);
    file->ctx = NULL;

    return 0;
}

static int procfs_fcntl(mount_point_t *point, file_t *file, int cmd, int arg)
{
    privinfo_t *priv = file->ctx;
    procfs_node_t *node;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = priv->node;
    if (node == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    switch (cmd) {
    case F_GETFL:
        return file->flags;

    case F_SETFL:
        file->flags = arg;
        return 0;

    default:
        seterrno(EINVAL);
        return -1;
    }
}

static int procfs_nstat(mount_point_t *point, procfs_node_t *node, struct stat *buf)
{
    buf->st_dev     = (dev_t)0;
    buf->st_ino     = 0;
    if (node->type == PROCFS_NODE_TYPE_FILE) {
        buf->st_mode= S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IFREG;
    } else {
        buf->st_mode= S_IRWXU | S_IRWXG | S_IRWXO | S_IFDIR;
    }
    buf->st_nlink   = 0;
    buf->st_uid     = 0;
    buf->st_gid     = 0;
    buf->st_rdev    = (dev_t)0;
    buf->st_size    = 0;
    buf->st_atime   = 0;
    buf->st_spare1  = 0;
    buf->st_mtime   = 0;
    buf->st_spare2  = 0;
    buf->st_ctime   = 0;
    buf->st_spare3  = 0;
    buf->st_blksize = 0;
    buf->st_blocks  = 0;
    buf->st_spare4[0] = 0;
    buf->st_spare4[1] = 0;

    return 0;
}

static int procfs_fstat(mount_point_t *point, file_t *file, struct stat *buf)
{
    privinfo_t *priv = file->ctx;
    procfs_node_t *node;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = priv->node;
    if (node == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    return procfs_nstat(point, node, buf);
}

static int procfs_scan(mount_point_t *point, file_t *file, int flags)
{
    privinfo_t *priv = file->ctx;
    procfs_node_t *node;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = priv->node;
    if (node == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if (flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if (node->write && flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    return ret;
}

static int procfs_stat(mount_point_t *point, const char *path, struct stat *buf)
{
    if (VFS_PATH_IS_ROOT(path)) {
        return procfs_nstat(point, &procfs_root_node, buf);
    } else {
        procfs_node_t *node;

        mutex_lock(&procfs_node_mgr_lock, 0);

        node = procfs_node_lookup(path, PROCFS_NODE_TYPE_FILE);
        if (node == NULL) {
            node = procfs_node_lookup(path, PROCFS_NODE_TYPE_DIR);
        }

        if (node != NULL) {
            int ret;

            ret = procfs_nstat(point, node, buf);

            mutex_unlock(&procfs_node_mgr_lock);

            return ret;
        } else {
            mutex_unlock(&procfs_node_mgr_lock);
            seterrno(ENOENT);
            return -1;
        }
    }
}

static int procfs_access(mount_point_t *point, const char *path, int amode)
{
    struct stat buf;
    int         ret;

    ret = procfs_stat(point, path, &buf);
    if (ret == 0) {
        int access_ok = 1;

        if ((amode & R_OK) && !(buf.st_mode & S_IRUSR)) {
            access_ok = 0;
        }
        if ((amode & W_OK) && !(buf.st_mode & S_IWUSR)) {
            access_ok = 0;
        }
        if ((amode & X_OK) && !(buf.st_mode & S_IXUSR)) {
            access_ok = 0;
        }

        if (!access_ok) {
            seterrno(EACCES);
            return -1;
        } else {
            return 0;
        }
    } else {
        return ret;
    }
}

static int procfs_opendir(mount_point_t *point, file_t *file, const char *path)
{
    procfs_node_t *node;
    privinfo_t *priv;

    node = procfs_node_lookup_ref(path, PROCFS_NODE_TYPE_DIR);
    if (node == NULL) {
        seterrno(ENOENT);
        return -1;
    }

    priv = kmalloc(sizeof(privinfo_t), GFP_KERNEL);
    if (priv != NULL) {
        file->ctx  = priv;
        priv->loc  = 0;
        priv->node = node;
        return 0;
    } else {
        atomic_dec(&node->ref);
        seterrno(ENOMEM);
        return -1;
    }
}

static struct dirent *procfs_readdir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;
    procfs_node_t *node;

    if (priv == NULL) {
        seterrno(EINVAL);
        return NULL;
    }

    if (priv->node == NULL) {
        seterrno(EINVAL);
        return NULL;
    }

    mutex_lock(&procfs_node_mgr_lock, 0);

    node = procfs_node_get(priv->node, priv->loc);

    if (node != NULL) {

        strlcpy(priv->entry.d_name, node->name, sizeof(priv->entry.d_name));

        mutex_unlock(&procfs_node_mgr_lock);

        priv->entry.d_ino = priv->loc++;

        return &priv->entry;
    } else {
        mutex_unlock(&procfs_node_mgr_lock);

        return NULL;
    }
}

static int procfs_rewinddir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (priv->node == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    priv->loc = 0;

    return 0;
}

static int procfs_seekdir(mount_point_t *point, file_t *file, long loc)
{
    privinfo_t *priv = file->ctx;
    procfs_node_t *node;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (priv->node == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = procfs_node_get(priv->node, loc);

    if (node != NULL) {

        priv->loc = loc;

        return 0;
    } else {
        seterrno(EINVAL);
        return -1;
    }
}

static long procfs_telldir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    if (priv->node == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    return priv->loc;
}

static int procfs_closedir(mount_point_t *point, file_t *file)
{
    privinfo_t *priv = file->ctx;
    procfs_node_t *node;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = priv->node;
    if (node == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    atomic_dec(&node->ref);

    kfree(priv);
    file->ctx = NULL;

    return 0;
}

file_system_t procfs = {
        .name       = "procfs",
        .mount      = procfs_mount,
        .stat       = procfs_stat,
        .access     = procfs_access,

        .open       = procfs_open,
        .read       = procfs_read,
        .write      = procfs_write,
        .close      = procfs_close,
        .fcntl      = procfs_fcntl,
        .fstat      = procfs_fstat,

        .opendir    = procfs_opendir,
        .readdir    = procfs_readdir,
        .rewinddir  = procfs_rewinddir,
        .seekdir    = procfs_seekdir,
        .telldir    = procfs_telldir,
        .closedir   = procfs_closedir,

        .scan       = procfs_scan,
        .select     = NULL,
        .unselect   = NULL,
};
/*********************************************************************************************************
** Function name:           procfs_creat
** Descriptions:            创建 procfs 文件系统文件
** input parameters:        dir_node            目录节点
**                          read                读节点函数
**                          write               写节点函数
**                          ctx                 上下文
**                          name                名字
** output parameters:       NONE
** Returned value:          procfs 文件系统文件节点
*********************************************************************************************************/
procfs_node_t *procfs_creat(procfs_node_t *dir_node,
                            procfs_node_read_t read,
                            procfs_node_write_t write,
                            void *ctx,
                            const char *name)
{
    procfs_node_t *node;
    procfs_node_t *prev;

    if (dir_node == NULL ||
        read == NULL ||
        name == NULL ||
        strlen(name) > NAME_MAX - 1) {
        return NULL;
    }

    mutex_lock(&procfs_node_mgr_lock, 0);

    for (prev = node = dir_node->son; node != NULL; node = node->next) {
        if (strcmp(node->name, name) == 0) {
            mutex_unlock(&procfs_node_mgr_lock);
            return NULL;
        }
        if (prev->next != NULL) {
            prev = prev->next;
        }
    }

    node = kmalloc(sizeof(procfs_node_t), GFP_KERNEL);
    if (node == NULL) {
        mutex_unlock(&procfs_node_mgr_lock);
        return NULL;
    }

    if (dir_node->son == NULL) {
        dir_node->son = node;
    } else {
        prev->next    = node;
    }

    node->type  = PROCFS_NODE_TYPE_FILE;
    node->read  = read;
    node->write = write;
    node->ctx   = ctx;

    node->son   = NULL;
    node->next  = NULL;
    strcpy(node->name, name);

    mutex_unlock(&procfs_node_mgr_lock);

    return node;
}
/*********************************************************************************************************
** Function name:           procfs_mkdir
** Descriptions:            创建 procfs 文件系统目录
** input parameters:        dir_node            目录节点
**                          name                名字
** output parameters:       NONE
** Returned value:          procfs 文件系统目录节点
*********************************************************************************************************/
procfs_node_t *procfs_mkdir(procfs_node_t *dir_node, const char *name)
{
    procfs_node_t *node;
    procfs_node_t *prev;

    if (name == NULL ||
        strlen(name) > NAME_MAX - 1) {
        return NULL;
    }

    if (dir_node == NULL) {
        dir_node = &procfs_root_node;
    }

    mutex_lock(&procfs_node_mgr_lock, 0);

    for (prev = node = dir_node->son; node != NULL; node = node->next) {
        if (strcmp(node->name, name) == 0) {
            mutex_unlock(&procfs_node_mgr_lock);
            return NULL;
        }
        if (prev->next != NULL) {
            prev = prev->next;
        }
    }

    node = kmalloc(sizeof(procfs_node_t), GFP_KERNEL);
    if (node == NULL) {
        mutex_unlock(&procfs_node_mgr_lock);
        return NULL;
    }

    if (dir_node->son == NULL) {
        dir_node->son = node;
    } else {
        prev->next    = node;
    }

    node->type  = PROCFS_NODE_TYPE_DIR;
    node->read  = NULL;
    node->write = NULL;
    node->ctx   = NULL;
    atomic_set(&node->ref, 0);
    node->son   = NULL;
    node->next  = NULL;
    strcpy(node->name, name);

    mutex_unlock(&procfs_node_mgr_lock);

    return node;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
