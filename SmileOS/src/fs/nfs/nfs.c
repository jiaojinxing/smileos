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
** File name:               nfs.c
** Last modified Date:      2013-2-21
** Last Version:            1.0.0
** Descriptions:            网络文件系统
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-21
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
#include "vfs/config.h"
#include "vfs/types.h"
#include "vfs/vfs.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "src/nfs.h"
#include "src/mount.h"
/*********************************************************************************************************
** 配置
*********************************************************************************************************/
#define NFS_HOST_NAME_LEN   NAME_MAX                                    /*  HOST_NAME 的长度            */
#define NFS_EXPORT_PATH_LEN PATH_MAX                                    /*  EXPORT_PATH 的长度          */
#define NFS_BLOCK_SIZE      8192                                        /*  块的大小                    */
/*********************************************************************************************************
** 移植
*********************************************************************************************************/
#define NFS_MALLOC(x)       kmalloc(x, GFP_KERNEL)
#define NFS_FREE(x)         kfree(x)
#define NFS_STRDUP          strdup
#define NFS_DEBUG           printk
/*********************************************************************************************************
** 网络文件系统
*********************************************************************************************************/
typedef struct {
    CLIENT                 *client;                                     /*  客户端                      */
    CLIENT                 *mount_client;                               /*  挂载客户端                  */
    nfs_fh3                *rootdir_filehandle;                         /*  根目录的文件句柄            */
    char                    host[NFS_HOST_NAME_LEN];                    /*  HOST_NAME 主机名            */
    char                    export[NFS_EXPORT_PATH_LEN];                /*  EXPORT_PATH 输出路径        */
} nfs_fs;
/*********************************************************************************************************
** 网络文件
*********************************************************************************************************/
typedef struct {
    nfs_fh3                *filehandle;                                 /*  文件句柄                    */
    size_t                  offset;                                     /*  文件的读写位置              */
    size_t                  size;                                       /*  文件的大小                  */
    int                     oflag;                                      /*  文件的打开方式              */
    bool_t                  eof;                                        /*  文件是否结束                */

    /*
     * 目录才会使用以下的成员变量
     */
    cookie3                 cookie;
    cookieverf3             cookieverf;
    entry3                 *entry;
    READDIR3res             res;
    struct dirent           dirent;
} nfs_file;
/*********************************************************************************************************
** Function name:           __nfs_dup_filehandle
** Descriptions:            复制文件句柄
** input parameters:        filehandle          要复制的文件句柄
** output parameters:       NONE
** Returned value:          新的文件句柄, NULL 表示内存不足
*********************************************************************************************************/
static nfs_fh3 *__nfs_dup_filehandle(const nfs_fh3 *filehandle)
{
    u_int    data_len = filehandle->data.data_len;                      /*  数据的长度                  */
    nfs_fh3 *new_fh = (nfs_fh3 *)NFS_MALLOC(sizeof(nfs_fh3) + data_len);/*  分配 nfs_fh3 与数据存放空间 */

    if (new_fh != NULL) {
        new_fh->data.data_val = (char *)new_fh + sizeof(nfs_fh3);       /*  记录数据存放空间的起始地址  */
        new_fh->data.data_len = data_len;                               /*  记录数据的长度              */

        memcpy(new_fh->data.data_val, filehandle->data.data_val,        /*  拷贝数据到数据存放空间      */
                data_len);
    }
    return  (new_fh);                                                   /*  返回文件句柄                */
}
/*********************************************************************************************************
** Function name:           __nfs_free_filehandle
** Descriptions:            释放文件句柄
** input parameters:        filehandle          要释放的文件句柄
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void __nfs_free_filehandle(nfs_fh3 *filehandle)
{
    NFS_FREE(filehandle);                                               /*  释放 nfs_fh3 与数据存放空间 */
}
/*********************************************************************************************************
** Function name:           __nfs_lookup_filehandle
** Descriptions:            查找文件/目录的文件句柄
** input parameters:        nfs                 网络文件系统
**                          full_path           文件/目录的全路径
** output parameters:       NONE
** Returned value:          终级目录项的文件句柄
*********************************************************************************************************/
static nfs_fh3 *__nfs_lookup_filehandle(nfs_fs *nfs, const char *full_path)
{
    nfs_fh3    *filehandle;
    char       *filename;
    char       *path;
    char       *pathbase;

    if (full_path[0] != '/') {                                          /*  不是全路径                  */
        NFS_DEBUG("__nfs_lookup_filehandle() error: \"%s\" is not full path!\n", full_path);
        seterrno(EINVAL);
        return  (NULL);
    }

    /*
     * 复制路径
     */
    pathbase = path = NFS_STRDUP(full_path);
    if (pathbase == NULL) {
        NFS_DEBUG("__nfs_lookup_filehandle() error: NFS_STRDUP() failed, system low memory!\n");
        seterrno(ENOMEM);
        return  (NULL);
    }

    /*
     * 复制根目录的文件句柄
     */
    filehandle = __nfs_dup_filehandle(nfs->rootdir_filehandle);
    if (filehandle == NULL) {
        NFS_DEBUG("__nfs_lookup_filehandle() error: __nfs_dup_filehandle() failed, system low memory!\n");
        NFS_FREE(pathbase);
        seterrno(ENOMEM);
        return  (NULL);
    }

    /*
     * 获得终级目录项的文件句柄
     */
    while ((filename = strtok_r(NULL, "/", &path)) != NULL) {           /*  获得各级目录项的名字        */
        LOOKUP3args     args;
        LOOKUP3res      res;

        /*
         * 设定参数
         */
        args.what.dir  = *filehandle;                                   /*  父目录的文件句柄            */
        args.what.name = filename;                                      /*  目录项的名字                */

        /*
         * 在父目录查找该目录项
         */
        bzero(&res, sizeof(res));

        nfsproc3_lookup_3(&args, &res, nfs->client);

        if (res.status != NFS3_OK) {
            if (res.status != NFS3ERR_NOENT) {                          /*  出错不是因为不存在该目录项  */
                NFS_DEBUG("__nfs_lookup_filehandle() error: lookup \"%s\" failed, status=%d\n",
                        filename, res.status);
                seterrno(EIO);
            } else {
                seterrno(ENOENT);
            }

            NFS_FREE(pathbase);
            __nfs_free_filehandle(filehandle);
            xdr_free((xdrproc_t)xdr_LOOKUP3res, (char *)&res);
            return  (NULL);
        }

        __nfs_free_filehandle(filehandle);                              /*  释放父目录的文件句柄        */

        /*
         * 复制该目录的文件句柄, 并作为下一次查找时的父目录文件句柄
         */
        filehandle = __nfs_dup_filehandle(&res.LOOKUP3res_u.resok.object);

        xdr_free((xdrproc_t)xdr_LOOKUP3res, (char *)&res);              /*  释放返回资源                */

        if (filehandle == NULL) {                                       /*  复制文件句柄出错            */
            NFS_DEBUG("__nfs_lookup_filehandle() error: "
                      "__nfs_dup_filehandle() failed, system low memory!\n");
            NFS_FREE(pathbase);
            seterrno(ENOMEM);
            return  (NULL);
        }
    }

    NFS_FREE(pathbase);                                                 /*  释放路径                    */

    return filehandle;                                                  /*  返回终级目录项的文件句柄    */
}
/*********************************************************************************************************
** Function name:           __nfs_lookup_parentdirhandle
** Descriptions:            获得文件/目录的父目录的文件句柄
** input parameters:        nfs                 网络文件系统
**                          full_path           文件/目录的全路径
** output parameters:       file_name           终级文件/目录名(要自行释放其占用的内存)
** Returned value:          父目录的文件句柄
*********************************************************************************************************/
static nfs_fh3 *__nfs_lookup_parentdirhandle(nfs_fs *nfs, const char *full_path, char **file_name)
{
    nfs_fh3    *filehandle;
    char       *filename;
    char       *path;
    char       *pathbase;
    char       *end;
    char       *tmp;

    if (full_path[0] != '/') {                                          /*  不是全路径                  */
        NFS_DEBUG("__nfs_lookup_parentdirhandle() error: \"%s\" is not full path!\n", full_path);
        seterrno(EINVAL);
        return  (NULL);
    }

    /*
     * 复制路径
     */
    pathbase = path = NFS_STRDUP(full_path);
    if (pathbase == NULL) {
        NFS_DEBUG("__nfs_lookup_parentdirhandle() error: NFS_STRDUP() failed, system low memory!\n");
        seterrno(ENOMEM);
        return  (NULL);
    }

    /*
     * 复制根目录的文件句柄
     */
    filehandle = __nfs_dup_filehandle(nfs->rootdir_filehandle);
    if (filehandle == NULL) {
        NFS_DEBUG("__nfs_lookup_parentdirhandle() error: __nfs_dup_filehandle() failed, system low memory!\n");
        NFS_FREE(pathbase);
        seterrno(ENOMEM);
        return  (NULL);
    }

    /*
     * 找出终级目录项名字前的 '/'
     */
    end = pathbase;

    while (end && (tmp = strchr(end, '/')) && tmp[1] != '\0') {
        end = tmp + 1;
    }

    /*
     * 获得终级目录项的父目录的文件句柄
     */
    while ((filename = strtok_r(NULL, "/", &path)) != NULL              /*  获得各级目录项的名字        */
            && filename < end) {                                        /*  但在终级目录项名字前跳出    */
        LOOKUP3args     args;
        LOOKUP3res      res;

        /*
         * 设定参数
         */
        args.what.dir  = *filehandle;                                   /*  父目录的文件句柄            */
        args.what.name = filename;                                      /*  目录项的名字                */

        /*
         * 在父目录查找该目录项
         */
        bzero(&res, sizeof(res));

        nfsproc3_lookup_3(&args, &res, nfs->client);

        if (res.status != NFS3_OK) {
            if (res.status != NFS3ERR_NOENT) {                          /*  出错不是因为不存在该目录项  */
                NFS_DEBUG("__nfs_lookup_parentdirhandle() error: lookup \"%s\" failed, status=%d\n",
                        filename, res.status);
                seterrno(EIO);
            } else {
                seterrno(ENOENT);
            }
            NFS_FREE(pathbase);
            __nfs_free_filehandle(filehandle);
            xdr_free((xdrproc_t)xdr_LOOKUP3res, (char *)&res);
            return  (NULL);
        }

        __nfs_free_filehandle(filehandle);                              /*  释放父目录的文件句柄        */

        /*
         * 复制该目录的文件句柄, 并作为下一次查找时的父目录文件句柄
         */
        filehandle = __nfs_dup_filehandle(&res.LOOKUP3res_u.resok.object);

        xdr_free((xdrproc_t)xdr_LOOKUP3res, (char *)&res);              /*  释放返回资源                */

        if (filehandle == NULL) {                                       /*  复制文件句柄出错            */
            NFS_DEBUG("__nfs_lookup_parentdirhandle() error: "
                      "__nfs_dup_filehandle() failed, system low memory!\n");
            NFS_FREE(pathbase);
            seterrno(ENOMEM);
            return  (NULL);
        }
    }

    /*
     * 提取终级文件/目录名
     */
    {
        *file_name = NFS_STRDUP(end);                                   /*  复制终级目录项的名字        */
        if (*file_name != NULL) {
            tmp = strchr(*file_name, '/');                              /*  去掉后面的 '/', 如果有      */
            if (tmp != NULL) {
                *tmp = '\0';
            }
        } else {
            NFS_DEBUG("__nfs_lookup_parentdirhandle() error: NFS_STRDUP() failed, system low memory!\n");
            __nfs_free_filehandle(filehandle);                          /*  释放父目录的文件句柄        */
            filehandle = NULL;
            seterrno(ENOMEM);
        }
    }

    NFS_FREE(pathbase);                                                 /*  释放路径                    */

    return filehandle;                                                  /*  返回父目录的文件句柄        */
}
/*********************************************************************************************************
** Function name:           __nfs_parse_host_export
** Descriptions:            提取 hostname 与 export_path
** input parameters:        host_export         hostname 与 export_path 组成的字符串,
**                                              形如 "192.168.9.17:/nfs"
**                          host_len            hostname 字符数组的容量
**                          export_len          export_path 字符数组的容量
** output parameters:       host                hostname
**                          export              export_path
** Returned value:          成功返回 0, 出错返回 -1
*********************************************************************************************************/
static int __nfs_parse_host_export(const char  *host_export,
                                   char        *host,
                                   size_t       host_len,
                                   char        *export,
                                   size_t       export_len)
{
    int i;

    /*
     * 记录 hostname
     */
    for (i = 0; i < host_len; i++) {
        if (host_export[i] == '\0') {                                   /*  如果提前结束, 出错          */
            return  (-1);
        }

        if (host_export[i] == ':') {                                    /*  如果遇 ':' 分隔符, 跳出     */
            break;
        } else {
            host[i] = host_export[i];                                   /*  记录 hostname               */
        }
    }

    if (i == host_len) {                                                /*  如果 hostname 太长, 出错    */
        return  (-1);
    }

    host_len = i;
    host[host_len] = '\0';                                              /*  hostname 加入结束符         */
    host_len++;

    /*
     * 记录 export_path
     */
    for (i = host_len; i < host_len + export_len; i++) {
        if (host_export[i] == '\0') {                                   /*  如果遇结束                  */
            export[i - host_len] = '\0';                                /*  加入结束符                  */
            return  (0);
        } else {
            export[i - host_len] = host_export[i];                      /*  记录 export_path            */
        }
    }
    return  (-1);                                                       /*  export_path 太长, 出错      */
}
/*********************************************************************************************************
** Function name:           nfs_mount
** Descriptions:            挂载 NFS
** input parameters:        point               挂载点
**                          dev                 设备
**                          dev_name            设备名
**                          param               挂载参数
**                          hostname 与 export_path 组成的字符串, 形如 "192.168.9.17:/nfs"
** output parameters:       NONE
** Returned value:          成功返回 0, 出错返回 -1
*********************************************************************************************************/
static int nfs_mount(mount_point_t *point, device_t *dev, const char *dev_name, const char *param)
{
    mountres3   res;
    int         ret;
    nfs_fs     *nfs;

    if (point == NULL || param == NULL) {
        NFS_DEBUG("nfs_mount() error: param == NULL!\n");
        seterrno(EINVAL);
        return -1;
    }

    nfs = NFS_MALLOC(sizeof(nfs_fs));
    if (nfs == NULL) {
        NFS_DEBUG("nfs_mount() error: alloc nfs_fs failed, system low memory!\n");
        seterrno(ENOMEM);
        return -1;
    }

    nfs->mount_client           = NULL;
    nfs->client                 = NULL;

    extern int is_network_up(int    sock);
    if (!is_network_up(0)) {                                            /*  如果网络服务未就绪          */
        NFS_DEBUG("nfs_mount() error: network service is not ready!\n");
        seterrno(EIO);
        goto __error_handle;
    }

    /*
     * 提取 host_name 与 export_path
     */
    ret = __nfs_parse_host_export(param,
                                  nfs->host,   NFS_HOST_NAME_LEN,
                                  nfs->export, NFS_EXPORT_PATH_LEN);
    if (ret < 0) {
        NFS_DEBUG("nfs_mount() error: host name or export path error!\n");
        seterrno(EINVAL);
        goto __error_handle;
    }

    /*
     * 创建挂载客户端
     */
    nfs->mount_client = clnt_create(nfs->host, MOUNT_PROGRAM, MOUNT_V3, "udp");
    if (nfs->mount_client == NULL) {
        NFS_DEBUG("nfs_mount() error: create mount client failed!\n");
        seterrno(ENOMEM);
        goto __error_handle;
    }

    /*
     * 挂载
     */
    bzero(&res, sizeof(res));

    mountproc3_mnt_3((dirpath *)&nfs->export, &res, nfs->mount_client);

    if (res.fhs_status != MNT3_OK) {
        NFS_DEBUG("nfs_mount() error: nfs mount \"%s\" failed, status=%d\n",
                nfs->export, res.fhs_status);
        seterrno(EIO);
        goto __error_handle;
    }

    /*
     * 创建 NFS 客户端
     */
    nfs->client = clnt_create(nfs->host, NFS_PROGRAM, NFS_V3, "udp");
    if (nfs->client == NULL) {
        NFS_DEBUG("nfs_mount() error: create nfs client failed!\n");
        seterrno(ENOMEM);
        goto __error_handle;
    }

    /*
     * 复制根目录的文件句柄
     */
    nfs->rootdir_filehandle = __nfs_dup_filehandle((nfs_fh3 *)&(res.mountres3_u.mountinfo.fhandle));
    if (nfs->rootdir_filehandle == NULL) {
        NFS_DEBUG("nfs_mount() error: __nfs_dup_filehandle() failed, system low memory!\n");
        seterrno(ENOMEM);
        goto __error_handle;
    }

    point->ctx = nfs;

    return  (0);

    /*
     * 以下是出错处理
     */
__error_handle:
    if (nfs->mount_client != NULL) {

        if (nfs->mount_client->cl_auth != NULL) {
            auth_destroy(nfs->mount_client->cl_auth);                   /*  销毁挂载客户端的 AUTH       */
            nfs->mount_client->cl_auth = NULL;
        }

        clnt_destroy(nfs->mount_client);                                /*  销毁挂载客户端              */
        nfs->mount_client = NULL;
    }

    if (nfs->client != NULL) {

        if (nfs->client->cl_auth != NULL) {
            auth_destroy(nfs->client->cl_auth);                         /*  销毁 NFS 客户端的 AUTH      */
            nfs->client->cl_auth = NULL;
        }

        clnt_destroy(nfs->client);                                      /*  销毁 NFS 客户端             */
        nfs->client = NULL;
    }

    NFS_FREE(nfs);

    return  (-1);
}
/*********************************************************************************************************
** Function name:           nfs_unmount
** Descriptions:            取消挂载 NFS
** input parameters:        point               挂载点
**                          param               挂载参数
** output parameters:       NONE
** Returned value:          成功返回 0, 出错返回 -1
*********************************************************************************************************/
static int nfs_unmount(mount_point_t *point, const char *param)
{
    enum clnt_stat  ret;
    nfs_fs         *nfs;

    nfs = point->ctx;
    if (nfs == NULL) {
        NFS_DEBUG("nfs_unmount() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    if (nfs->mount_client == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_unmount() error: nfs no mount, can not unmount!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    ret = mountproc3_umnt_3((dirpath *)&nfs->export, NULL,              /*  取消挂载                    */
                            nfs->mount_client);
    if (ret != RPC_SUCCESS) {
        NFS_DEBUG("nfs_unmount() error: nfs unmount failed!\n");
        seterrno(EBUSY);
        return  (-1);
    }

    if (nfs->client->cl_auth != NULL) {
        auth_destroy(nfs->client->cl_auth);                             /*  销毁 NFS 客户端的 AUTH      */
        nfs->client->cl_auth = NULL;
    }

    clnt_destroy(nfs->client);                                          /*  销毁 NFS 客户端             */
    nfs->client = NULL;

    if (nfs->mount_client->cl_auth != NULL) {
        auth_destroy(nfs->mount_client->cl_auth);                       /*  销毁挂载客户端的 AUTH       */
        nfs->mount_client->cl_auth = NULL;
    }

    clnt_destroy(nfs->mount_client);                                    /*  销毁挂载客户端              */
    nfs->mount_client = NULL;

    __nfs_free_filehandle(nfs->rootdir_filehandle);                     /*  释放根目录的文件句柄        */

    NFS_FREE(nfs);
    point->ctx = NULL;

    return  (0);
}
/*********************************************************************************************************
** Function name:           __nfsfile_create
** Descriptions:            创建文件
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int __nfsfile_create(nfs_fs *nfs, const char *full_path, mode_t mode)
{
    CREATE3args     args;
    CREATE3res      res;
    nfs_fh3        *dirhandle;
    int             ret;

    /*
     * 获得父目录的文件句柄
     */
    dirhandle = __nfs_lookup_parentdirhandle(nfs, full_path, &args.where.name);
    if (dirhandle == NULL) {
        return  (-1);
    }

    /*
     * 设定参数
     */
    args.where.dir                                    = *dirhandle;     /*  父目录的文件句柄            */
    args.how.mode                                     = GUARDED;
    args.how.createhow3_u.obj_attributes.mode.set_it  = TRUE;
    args.how.createhow3_u.obj_attributes.mode.set_mode3_u.mode = mode;  /*  设置文件的权限              */
    args.how.createhow3_u.obj_attributes.uid.set_it   = FALSE;
    args.how.createhow3_u.obj_attributes.gid.set_it   = FALSE;
    args.how.createhow3_u.obj_attributes.size.set_it  = FALSE;
    args.how.createhow3_u.obj_attributes.atime.set_it = SET_TO_SERVER_TIME; /*  设置为服务器的时间      */
    args.how.createhow3_u.obj_attributes.mtime.set_it = SET_TO_SERVER_TIME;

    /*
     * 创建文件
     */
    bzero(&res, sizeof(res));

    nfsproc3_create_3(&args, &res, nfs->client);

    if (   res.status != NFS3_OK
        && res.status != NFS3ERR_EXIST) {                               /*  如果因存在而出错, 不回报错误*/
        NFS_DEBUG("nfs_create() error: nfs create file \"%s\" failed, status=%d\n",
                full_path, res.status);
        ret = -1;
        seterrno(EIO);
    } else {
        ret = 0;
    }

    xdr_free((xdrproc_t)xdr_CREATE3res, (char *)&res);                  /*  释放返回资源                */

    __nfs_free_filehandle(dirhandle);                                   /*  释放父目录的文件句柄        */

    NFS_FREE(args.where.name);                                          /*  释放终级文件名              */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           __nfsfile_get_size
** Descriptions:            获得文件大小
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static ssize_t __nfsfile_get_size(nfs_fs *nfs, nfs_file *nfsfile)
{
    GETATTR3args    args;
    GETATTR3res     res;
    fattr3         *attr;
    ssize_t         size;

    /*
     * 设定参数
     */
    args.object = *nfsfile->filehandle;                                 /*  文件句柄                    */

    /*
     * 获得属性
     */
    bzero(&res, sizeof(res));

    nfsproc3_getattr_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("__nfs_get_filesize() error: nfs get file attr failed, status=%d\n", res.status);
        size = -1;
        seterrno(EIO);
    } else {
        attr = &res.GETATTR3res_u.resok.obj_attributes;                 /*  文件的属性                  */

        size = attr->size;                                              /*  文件的大小                  */
    }

    xdr_free((xdrproc_t)xdr_GETATTR3res, (char *)&res);                 /*  释放返回资源                */

    return  (size);                                                     /*  返回文件的大小              */
}
/*********************************************************************************************************
** Function name:           __nfsfile_is_dir
** Descriptions:            返回文件是否一个目录
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static bool_t __nfsfile_is_dir(nfs_fs *nfs, nfs_file *nfsfile)
{
    GETATTR3args    args;
    GETATTR3res     res;
    fattr3         *attr;
    bool_t          is_dir;

    /*
     * 设定参数
     */
    args.object = *nfsfile->filehandle;                                 /*  文件句柄                    */

    /*
     * 获得属性
     */
    bzero(&res, sizeof(res));

    nfsproc3_getattr_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("__nfsfile_is_dir() error: nfs get file attr failed, status=%d\n", res.status);
        is_dir = TRUE;
    } else {
        attr   = &res.GETATTR3res_u.resok.obj_attributes;               /*  对象的属性                  */

        is_dir = (attr->type == NFS3DIR) ? TRUE : FALSE;                /*  根据对象的类型判断是否为目录*/
    }

    xdr_free((xdrproc_t)xdr_GETATTR3res, (char *)&res);                 /*  释放返回资源                */

    return  (is_dir);                                                   /*  返回是否为目录              */
}
/*********************************************************************************************************
** Function name:           __nfsfile_set_attr
** Descriptions:            设置文件属性
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int __nfsfile_set_attr(nfs_fs *nfs, nfs_file *nfsfile, const sattr3 *attr)
{
    SETATTR3args    args;
    SETATTR3res     res;
    int             ret;

    /*
     * 设定参数
     */
    args.object         = *nfsfile->filehandle;                         /*  文件/目录的文件句柄         */
    args.guard.check    = TRUE;                                         /*  需要检查                    */
    args.new_attributes = *attr;                                        /*  属性                        */

    /*
     * 设置属性
     */
    bzero(&res, sizeof(res));

    nfsproc3_setattr_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("__nfs_set_fileattr() error: nfs set file attr failed, status=%d\n", res.status);
        ret = -1;
        seterrno(EIO);
    } else {
        ret = 0;
    }

    xdr_free((xdrproc_t)xdr_SETATTR3res, (char *)&res);                 /*  释放返回资源                */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           __nfspath_set_attr
** Descriptions:            设置文件属性
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int __nfspath_set_attr(nfs_fs *nfs, const char *full_path, const sattr3 *attr)
{
    SETATTR3args    args;
    SETATTR3res     res;
    nfs_fh3        *filehandle;
    int             ret;

    /*
     * 获得文件/目录的文件句柄
     */
    filehandle = __nfs_lookup_filehandle(nfs, full_path);
    if (filehandle == NULL) {
        return  (-1);
    }

    /*
     * 设定参数
     */
    args.object         = *filehandle;                                  /*  文件/目录的文件句柄         */
    args.guard.check    = TRUE;                                         /*  需要检查                    */
    args.new_attributes = *attr;                                        /*  属性                        */

    /*
     * 设置属性
     */
    bzero(&res, sizeof(res));

    nfsproc3_setattr_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("__nfspath_set_attr() error: nfs set attr of \"%s\" failed, status=%d\n",
                full_path, res.status);
        ret = -1;
        seterrno(EIO);
    } else {
        ret = 0;
    }

    xdr_free((xdrproc_t)xdr_SETATTR3res, (char *)&res);                 /*  释放返回资源                */

    __nfs_free_filehandle(filehandle);                                  /*  释放文件/目录的文件句柄     */

    return  (ret);
}
static int nfs_ftruncate(mount_point_t *point, file_t *file, off_t len);
/*********************************************************************************************************
** Function name:           nfs_open
** Descriptions:            打开文件
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_open(mount_point_t *point, file_t *file, const char *path, int oflag, mode_t mode)
{
    nfs_file   *nfsfile;
    nfs_fh3    *filehandle;
    nfs_fs     *nfs;

    nfs = point->ctx;
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_open() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * O_EXCL only has meaning if O_CREAT is specified
     */
    if (!(oflag & O_CREAT)) {
        oflag &= ~(O_EXCL);
    }

    /*
     * O_TRUNC has no meaning if (O_CREAT | O_EXCL) is specified
     */
    if ((oflag & O_CREAT) & (oflag & O_EXCL)) {
        oflag &= ~(O_TRUNC);
    }

    /*
     * 查找文件/目录
     */
    filehandle = __nfs_lookup_filehandle(nfs, path);
    if (filehandle != NULL) {                                           /*  如果文件/目录已经存在       */
        /*
         * Open should fail if O_CREAT and O_EXCL are specified since
         * the file exists
         */
        if ((oflag & O_EXCL) && (oflag & O_CREAT)) {
            __nfs_free_filehandle(filehandle);                          /*  释放文件句柄                */
            seterrno(EINVAL);
            return  (-1);
        }
    } else if (oflag & O_CREAT) {
        seterrno(0);
        /*
         * Let's see if we can create this file
         */
        if (__nfsfile_create(nfs, path, mode) < 0) {                    /*  创建文件                    */
            return  (-1);
        }

        /*
         * 获得文件句柄
         */
        filehandle = __nfs_lookup_filehandle(nfs, path);
        if (filehandle == NULL) {
            return  (-1);
        }
    } else {
        return  (-1);
    }

    nfsfile = NFS_MALLOC(sizeof(nfs_file));
    if (nfsfile == NULL) {
        __nfs_free_filehandle(filehandle);                              /*  释放文件句柄                */
        NFS_DEBUG("nfs_mount() error: alloc nfsfile failed, system low memory!\n");
        seterrno(ENOMEM);
        return -1;
    }

    /*
     * 初始化文件结构
     */
    nfsfile->filehandle = filehandle;                                   /*  文件句柄                    */
    nfsfile->oflag      = oflag;                                        /*  文件的打开方式              */
    nfsfile->offset     = 0;                                            /*  文件的读写位置              */
    nfsfile->size       = 0;                                            /*  文件的大小                  */
    nfsfile->eof        = FALSE;                                        /*  EOF 标志                    */

    nfsfile->cookie     = 0;
    nfsfile->entry      = NULL;

    bzero(&nfsfile->cookieverf, sizeof(cookieverf3));

    bzero(&nfsfile->res, sizeof(READDIR3res));

    if (__nfsfile_is_dir(nfs, nfsfile)) {                               /*  如果是目录, 返回 1          */
        __nfs_free_filehandle(filehandle);
        NFS_FREE(nfsfile);
        seterrno(EFTYPE);
        return  (-1);
    } else {
        if (oflag & O_TRUNC) {                                          /*  如果要求截断文件            */
            if (nfs_ftruncate(point, file, 0) < 0) {                    /*  设置文件大小为 0            */
                __nfs_free_filehandle(filehandle);
                NFS_FREE(nfsfile);
                return  (-1);
            }
        }

        nfsfile->size = __nfsfile_get_size(nfs, nfsfile);               /*  获得文件大小                */
        if (nfsfile->size == (size_t)-1) {
            __nfs_free_filehandle(filehandle);
            NFS_FREE(nfsfile);
            return  (-1);
        }

        if (oflag & O_APPEND) {                                         /*  如果以追加的方式打开        */
            nfsfile->offset = nfsfile->size;                            /*  设置读写位置为文件尾        */
        }

        nfsfile->eof = (nfsfile->offset >= nfsfile->size) ?             /*  重设 EOF 标志               */
                        TRUE: FALSE;

        file->ctx = nfsfile;

        return  (0);
    }
}
/*********************************************************************************************************
** Function name:           nfs_close
** Descriptions:            关闭文件
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_close(mount_point_t *point, file_t *file)
{
    nfs_file *nfsfile;

    nfsfile = file->ctx;
    if (nfsfile == NULL) {
        NFS_DEBUG("nfs_close() error: nfsfile == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    __nfs_free_filehandle(nfsfile->filehandle);                         /*  释放文件句柄                */

    return 0;
}
/*********************************************************************************************************
** Function name:           __nfs_read
** Descriptions:            读文件
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static ssize_t __nfs_read(mount_point_t *point, file_t *file, char *buffer, size_t nbyte)
{
    nfs_fs         *nfs;
    READ3args       args;
    READ3res        res;
    ssize_t         nread;
    nfs_file       *nfsfile;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("__nfs_read() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    nfsfile = file->ctx;
    if (nfsfile == NULL) {
        NFS_DEBUG("__nfs_read() error: nfsfile == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    if (nfsfile->eof == TRUE) {                                         /*  如果已经到达文件尾, 则返回 0*/
        return  (0);
    }

    /*
     * 设定参数
     */
    args.file   = *nfsfile->filehandle;                                 /*  文件句柄                    */
    args.offset = nfsfile->offset;                                      /*  读取位置                    */
    args.count  = min(nbyte, NFS_BLOCK_SIZE);                           /*  能够读取的字节数            */
                                                                        /*  一次最多读取一个块          */
    /*
     * 读文件
     */
    bzero(&res, sizeof(res));

    nfsproc3_read_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("__nfs_read() error: nfs read file failed, status=%d\n", res.status);
        nread = -1;
        seterrno(EIO);
    } else {
        nread            = res.READ3res_u.resok.count;                  /*  实际读取的字节数            */

        nfsfile->offset += nread;                                       /*  调整读写位置                */

        nfsfile->eof     = (nfsfile->offset >= nfsfile->size) ?         /*  重设 EOF 标志               */
                            TRUE : FALSE;

        memcpy(buffer, res.READ3res_u.resok.data.data_val, nread);      /*  拷贝读到的数据到缓冲区      */
    }

    xdr_free((xdrproc_t)xdr_READ3res, (char *)&res);                    /*  释放返回资源                */

    return  (nread);                                                    /*  返回实际读取的字节数        */
}
/*********************************************************************************************************
** Function name:           nfs_read
** Descriptions:            读文件
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static ssize_t nfs_read(mount_point_t *point, file_t *file, void *buf, size_t len)
{
    ssize_t     nread;
    size_t      nleft = len;
    char       *buff  = (char *)buf;

    while (nleft > 0) {                                                 /*  剩余的字节数大于 0          */

        nread = __nfs_read(point, file, buff, nleft);                   /*  读取剩余的字节              */

        if (nread == -1) {                                              /*  如果读文件出错, 返回 -1     */
            return  (-1);
        } else if (nread == 0) {                                        /*  如果到达文件尾              */
            return  (len - nleft);                                      /*  返回实际读取的字节数        */
        } else {
            nleft -= nread;                                             /*  计算剩余的字节数            */
            buff  += nread;                                             /*  调整数据存放地址            */
        }
    }
    return  (len);                                                      /*  返回成功读取的字节数        */
}
/*********************************************************************************************************
** Function name:           __nfs_write
** Descriptions:            写文件
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static ssize_t __nfs_write(mount_point_t *point, file_t *file, const char *buffer, size_t nbyte)
{
    nfs_fs         *nfs;
    WRITE3args      args;
    WRITE3res       res;
    ssize_t         nwrite;
    nfs_file       *nfsfile;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("__nfs_write() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    nfsfile = file->ctx;
    if (nfsfile == NULL) {
        NFS_DEBUG("__nfs_write() error: nfsfile == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    if (nfsfile->oflag & O_APPEND) {                                    /*  如果以追加的方式打开        */
        nfsfile->offset = nfsfile->size;                                /*  写入位置设置为文件尾        */
    }

    nbyte = min(nbyte, NFS_BLOCK_SIZE);                                 /*  一次最多写入一个块          */

    /*
     * 设定参数
     */
    args.stable         = FILE_SYNC;                                    /*  总是同步的                  */
    args.file           = *nfsfile->filehandle;                         /*  文件句柄                    */
    args.offset         = nfsfile->offset;                              /*  写入位置                    */
    args.count          = nbyte;                                        /*  希望写入的字节数            */

    args.data.data_len  = nbyte;                                        /*  数据的长度                  */
    args.data.data_val  = (char *)buffer;                               /*  数据的起始地址              */

    /*
     * 写文件
     */
    bzero(&res, sizeof(res));

    nfsproc3_write_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("__nfs_write() error: nfs write file failed, status=%d\n", res.status);
        nwrite = -1;
        seterrno(EIO);
    } else {
        nwrite           = res.WRITE3res_u.resok.count;                 /*  实际写入的字节数            */

        nfsfile->offset += nwrite;                                      /*  调整读写位置                */

        nfsfile->size    = max(nfsfile->size, nfsfile->offset);         /*  调整文件的大小              */

        nfsfile->eof     = (nfsfile->offset >= nfsfile->size) ?         /*  重设 EOF 标志               */
                            TRUE : FALSE;
    }

    xdr_free((xdrproc_t)xdr_WRITE3res, (char *)&res);                   /*  释放返回资源                */

    return  (nwrite);                                                   /*  返回实际写入的字节数        */
}
/*********************************************************************************************************
** Function name:           nfs_write
** Descriptions:            写文件
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static ssize_t nfs_write(mount_point_t *point, file_t *file, const void *buf, size_t len)
{
    int          nwrite;
    size_t       nleft = len;
    const char  *buff  = (const char *)buf;

    while (nleft > 0) {                                                 /*  剩余的字节数大于 0          */

        nwrite = __nfs_write(point, file, buff, nleft);                 /*  写入剩余的字节              */

        if (nwrite == -1) {                                             /*  写文件出错                  */
            if (nleft == len) {                                         /*  未成功写入过                */
                return  (-1);                                           /*  返回 -1                     */
            } else {
                return  (len - nleft);                                  /*  返回实际写入的字节数        */
            }
        } else {
            nleft -= nwrite;                                            /*  计算剩余的字节数            */
            buff  += nwrite;                                            /*  调整数据存放地址            */
        }
    }
    return  (len);                                                      /*  返回成功写入的字节数        */
}
/*********************************************************************************************************
** Function name:           nfs_fcntl
** Descriptions:            控制文件
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_fcntl(mount_point_t *point, file_t *file, int cmd, int arg)
{
    switch (cmd) {
    case F_GETFL:
        return file->flags;

    case F_SETFL:
        if ((!(file->flags & FWRITE)) && (arg & FWRITE)) {
            seterrno(EINVAL);
            return -1;
        }
        file->flags = arg;
        return 0;

    default:
        seterrno(EINVAL);
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           nfs_fstat
** Descriptions:            控制文件
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_fstat(mount_point_t *point, file_t *file, struct stat *stat)
{
    GETATTR3args    args;
    GETATTR3res     res;
    fattr3         *attr;
    int             ret;
    nfs_fs         *nfs;
    nfs_file       *nfsfile;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_fstat() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    nfsfile = file->ctx;
    if (nfsfile == NULL) {
        NFS_DEBUG("nfs_fstat() error: nfsfile == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * 设定参数
     */
    args.object = *nfsfile->filehandle;                                 /*  文件/目录的文件句柄         */

    /*
     * 获得属性
     */
    bzero(&res, sizeof(res));

    nfsproc3_getattr_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("nfs_fstat() error: nfs get file attr failed, status=%d\n", res.status);
        ret = -1;
        seterrno(EIO);
    } else {
        attr = &res.GETATTR3res_u.resok.obj_attributes;

        /*
         * 设定 stat 状态结构
         */
        bzero(stat, sizeof(*stat));

        stat->st_dev       = attr->fsid;                                /*  device                      */
        stat->st_ino       = attr->fileid;                              /*  inode                       */
        stat->st_mode      = attr->mode;                                /*  protection                  */
        stat->st_nlink     = attr->nlink;                               /*  number of hard links        */
        stat->st_gid       = attr->gid;                                 /*  group ID of owner           */
        stat->st_uid       = attr->uid;                                 /*  user ID of owner            */
        stat->st_rdev      = attr->rdev.specdata1;                      /*  device type                 */
        stat->st_size      = attr->size;                                /*  total size, in bytes        */
        stat->st_blksize   = NFS_BLOCK_SIZE;                            /*  blocksize for filesystem I/O*/
                                                                        /*  number of blocks allocated  */
        stat->st_blocks    = (max(attr->used, attr->size) + NFS_BLOCK_SIZE - 1) / NFS_BLOCK_SIZE;
        stat->st_atime     = attr->atime.seconds;                       /*  time of last access         */
        stat->st_ctime     = attr->ctime.seconds;                       /*  time of last create         */
        stat->st_mtime     = attr->mtime.seconds;                       /*  time of last modification   */

        switch (attr->type) {
        case NFS3REG:
            stat->st_mode |= S_IFREG;
            break;

        case NFS3DIR:
            stat->st_mode |= S_IFDIR;
            break;

        case NFS3BLK:
            stat->st_mode |= S_IFBLK;
            break;

        case NFS3CHR:
            stat->st_mode |= S_IFCHR;
            break;

        case NFS3LNK:
            stat->st_mode |= S_IFLNK;
            break;

        case NFS3SOCK:
            stat->st_mode |= S_IFSOCK;
            break;

        case NFS3FIFO:
            stat->st_mode |= S_IFIFO;
            break;
        }

        ret = 0;
    }

    xdr_free((xdrproc_t)xdr_GETATTR3res, (char *)&res);                 /*  释放返回资源                */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_ftruncate
** Descriptions:            设置文件的长度
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_ftruncate(mount_point_t *point, file_t *file, off_t len)
{
    sattr3          attr;
    int             ret;
    nfs_fs         *nfs;
    nfs_file       *nfsfile;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_ftruncate() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    nfsfile = file->ctx;
    if (nfsfile == NULL) {
        NFS_DEBUG("nfs_ftruncate() error: nfsfile == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * 设定属性
     */
    attr.mode.set_it                      = FALSE;
    attr.uid.set_it                       = FALSE;
    attr.gid.set_it                       = FALSE;
    attr.size.set_it                      = TRUE;
    attr.size.set_size3_u.size            = len;                        /*  文件的长度                  */
    attr.atime.set_it                     = DONT_CHANGE;
    attr.mtime.set_it                     = DONT_CHANGE;

    ret = __nfsfile_set_attr(nfs, nfsfile, &attr);                      /*  设置属性                    */
    if (ret == 0) {
        nfsfile->size = len;                                            /*  重设文件的大小              */
        nfsfile->eof  = (nfsfile->offset >= nfsfile->size) ?            /*  重设 EOF 标志               */
                        TRUE : FALSE;
    }
    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_lseek
** Descriptions:            调整文件读写位置
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static off_t nfs_lseek(mount_point_t *point, file_t *file, off_t offset, int whence)
{
    off_t           newoffset = -1;
    size_t          filesize;
    nfs_fs         *nfs;
    nfs_file       *nfsfile;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_ftruncate() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    nfsfile = file->ctx;
    if (nfsfile == NULL) {
        NFS_DEBUG("nfs_ftruncate() error: nfsfile == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    filesize = nfsfile->size;                                           /*  获得文件的大小              */

    if (whence == SEEK_SET) {                                           /*  直接设置                    */
        if (offset >= 0) {
            newoffset = offset;
        }
    } else if (whence == SEEK_CUR) {                                    /*  相对当前的读写位置          */
        if ((nfsfile->offset + offset) >= 0) {
            newoffset = (nfsfile->offset + offset);
        }
    } else if (whence == SEEK_END) {                                    /*  相对文件尾                  */
        if (filesize >= 0 && (filesize + offset) >= 0) {
            newoffset = filesize + offset;
        }
    }

    if (newoffset >= 0) {
        if (newoffset > filesize) {                                     /*  如果文件要求变大            */
            if (nfs_ftruncate(point, file, newoffset) < 0) {            /*  调整文件的大小              */
                return  (-1);
            }
        }

        nfsfile->offset = newoffset;                                    /*  重设读写位置                */
        nfsfile->eof    = (nfsfile->offset >= nfsfile->size) ?          /*  重设 EOF 标志               */
                          TRUE : FALSE;
    } else {
        seterrno(EINVAL);
    }
    return  (newoffset);                                                /*  返回新的读写位置            */
}
/*********************************************************************************************************
** Function name:           nfs_scan
** Descriptions:            获得文件可读写状态
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_scan(mount_point_t *point, file_t *file, int flags)
{
    int ret;

    ret = 0;
    if (flags & VFS_FILE_READABLE) {
        ret |= VFS_FILE_READABLE;
    }
    if (flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    return ret;
}
/*********************************************************************************************************
** Function name:           nfs_stat
** Descriptions:            获得文件状态
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_stat(mount_point_t *point, const char *path, struct stat *stat)
{
    GETATTR3args    args;
    GETATTR3res     res;
    fattr3         *attr;
    nfs_fh3        *filehandle;
    int             ret;
    nfs_fs         *nfs;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_stat() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * 获得文件/目录的文件句柄
     */
    filehandle = __nfs_lookup_filehandle(nfs, path);
    if (filehandle == NULL) {
        return  (-1);
    }

    /*
     * 设定参数
     */
    args.object = *filehandle;                                          /*  文件/目录的文件句柄         */

    /*
     * 获得属性
     */
    bzero(&res, sizeof(res));

    nfsproc3_getattr_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("nfs_stat() error: nfs get attr of \"%s\" failed, status=%d\n",
                path, res.status);
        ret = -1;
        seterrno(EIO);
    } else {
        attr = &res.GETATTR3res_u.resok.obj_attributes;

        /*
         * 设定 stat 状态结构
         */
        bzero(stat, sizeof(*stat));

        stat->st_dev       = attr->fsid;                                /*  device                      */
        stat->st_ino       = attr->fileid;                              /*  inode                       */
        stat->st_mode      = attr->mode;                                /*  protection                  */
        stat->st_nlink     = attr->nlink;                               /*  number of hard links        */
        stat->st_gid       = attr->gid;                                 /*  group ID of owner           */
        stat->st_uid       = attr->uid;                                 /*  user ID of owner            */
        stat->st_rdev      = attr->rdev.specdata1;                      /*  device type                 */
        stat->st_size      = attr->size;                                /*  total size, in bytes        */
        stat->st_blksize   = NFS_BLOCK_SIZE;                            /*  blocksize for filesystem I/O*/
                                                                        /*  number of blocks allocated  */
        stat->st_blocks    = (max(attr->used, attr->size) + NFS_BLOCK_SIZE - 1) / NFS_BLOCK_SIZE;
        stat->st_atime     = attr->atime.seconds;                       /*  time of last access         */
        stat->st_ctime     = attr->ctime.seconds;                       /*  time of last create         */
        stat->st_mtime     = attr->mtime.seconds;                       /*  time of last modification   */

        switch (attr->type) {
        case NFS3REG:
            stat->st_mode |= S_IFREG;
            break;

        case NFS3DIR:
            stat->st_mode |= S_IFDIR;
            break;

        case NFS3BLK:
            stat->st_mode |= S_IFBLK;
            break;

        case NFS3CHR:
            stat->st_mode |= S_IFCHR;
            break;

        case NFS3LNK:
            stat->st_mode |= S_IFLNK;
            break;

        case NFS3SOCK:
            stat->st_mode |= S_IFSOCK;
            break;

        case NFS3FIFO:
            stat->st_mode |= S_IFIFO;
            break;
        }

        ret = 0;
    }

    xdr_free((xdrproc_t)xdr_GETATTR3res, (char *)&res);                 /*  释放返回资源                */

    __nfs_free_filehandle(filehandle);                                  /*  释放文件/目录的文件句柄     */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_access
** Descriptions:            获得文件状态
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_access(mount_point_t *point, const char *path, int amode)
{
    struct stat buf;
    int ret;

    ret = nfs_stat(point, path, &buf);
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
/*********************************************************************************************************
** Function name:           nfs_opendir
** Descriptions:            打开目录
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_opendir(mount_point_t *point, file_t *file, const char *path)
{
    nfs_file *nfsfile;
    nfs_fh3  *dirhandle;
    nfs_fs   *nfs;

    nfs = point->ctx;
    if (nfs == NULL) {
        NFS_DEBUG("nfs_opendir() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * 获得目录的文件句柄
     */
    dirhandle = __nfs_lookup_filehandle(nfs, path);
    if (dirhandle == NULL) {
        return  (-1);
    }

    nfsfile = NFS_MALLOC(sizeof(nfs_file));
    if (nfsfile == NULL) {
        __nfs_free_filehandle(dirhandle);                               /*  释放文件句柄                */
        NFS_DEBUG("nfs_opendir() error: alloc nfsfile failed, system low memory!\n");
        seterrno(ENOMEM);
        return -1;
    }

    /*
     * 初始化文件结构
     */
    nfsfile->filehandle = dirhandle;                                    /*  目录的文件句柄              */
    nfsfile->oflag      = 0;
    nfsfile->offset     = 0;
    nfsfile->size       = 0;
    nfsfile->cookie     = 0;
    nfsfile->entry      = NULL;
    nfsfile->eof        = FALSE;

    bzero(&nfsfile->cookieverf, sizeof(cookieverf3));

    bzero(&nfsfile->res, sizeof(READDIR3res));

    file->ctx = nfsfile;

    return  (0);
}
/*********************************************************************************************************
** Function name:           nfs_readdir
** Descriptions:            读目录项
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static struct dirent *nfs_readdir(mount_point_t *point, file_t *file)
{
    nfs_file *nfsfile;
    nfs_fs   *nfs;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_readdir() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (NULL);
    }

    nfsfile = file->ctx;
    if (nfsfile == NULL) {
        NFS_DEBUG("nfs_readdir() error: nfsfile == NULL!\n");
        seterrno(EINVAL);
        return  (NULL);
    }

    if (nfsfile->entry == NULL) {                                       /*  如果未曾读过                */
        READDIR3args args;

        /*
         * 设定参数
         */
        args.dir    = *nfsfile->filehandle;                             /*  目录句柄                    */
        args.cookie = nfsfile->cookie;                                  /*  cookie                      */
        args.count  = 1024;

        memcpy(&args.cookieverf, &nfsfile->cookieverf, sizeof(cookieverf3));

        xdr_free((xdrproc_t)xdr_READDIR3res, (char *)&nfsfile->res);    /*  释放返回资源                */

        /*
         * 读目录
         */
        bzero(&nfsfile->res, sizeof(READDIR3res));

        nfsproc3_readdir_3(&args, &nfsfile->res, nfs->client);

        if (nfsfile->res.status != NFS3_OK) {
            NFS_DEBUG("nfs_readdir() error: nfs read dir failed, status=%d\n", nfsfile->res.status);
            xdr_free((xdrproc_t)xdr_READDIR3res, (char *)&nfsfile->res);
            seterrno(EIO);
            return  (NULL);
        }

        memcpy(&nfsfile->cookieverf,
               &nfsfile->res.READDIR3res_u.resok.cookieverf,
                sizeof(cookieverf3));

        nfsfile->eof   = nfsfile->res.READDIR3res_u.resok.reply.eof;

        nfsfile->entry = nfsfile->res.READDIR3res_u.resok.reply.entries;
    }

    if (nfsfile->eof == TRUE && nfsfile->entry == NULL) {               /*  如果目录结束或目录为空      */
        return  (NULL);
    }

    nfsfile->cookie = nfsfile->entry->cookie;                           /*  cookie                      */

    strlcpy(nfsfile->dirent.d_name, nfsfile->entry->name, NAME_MAX);    /*  拷贝目录项的名字            */

    nfsfile->entry = nfsfile->entry->nextentry;                         /*  指向下一个目录项            */

    return  (&nfsfile->dirent);                                         /*  返回目录项指针              */
}
/*********************************************************************************************************
** Function name:           nfs_rewinddir
** Descriptions:            重设读目录点
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_rewinddir(mount_point_t *point, file_t *file)
{
    nfs_file *nfsfile;

    nfsfile = file->ctx;
    if (nfsfile == NULL) {
        NFS_DEBUG("nfs_rewinddir() error: nfsfile == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    xdr_free((xdrproc_t)xdr_READDIR3res, (char *)&nfsfile->res);        /*  释放返回资源                */

    nfsfile->cookie     = 0;
    nfsfile->entry      = NULL;
    nfsfile->eof        = FALSE;

    bzero(&nfsfile->cookieverf, sizeof(cookieverf3));

    bzero(&nfsfile->res, sizeof(READDIR3res));

    return 0;
}
/*********************************************************************************************************
** Function name:           nfs_closedir
** Descriptions:            关闭目录
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_closedir(mount_point_t *point, file_t *file)
{
    nfs_file *nfsfile;

    nfsfile = file->ctx;
    if (nfsfile == NULL) {
        NFS_DEBUG("nfs_rewinddir() error: nfsfile == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    xdr_free((xdrproc_t)xdr_READDIR3res, (char *)&nfsfile->res);        /*  释放返回资源                */

    __nfs_free_filehandle(nfsfile->filehandle);                         /*  释放目录的文件句柄          */

    return  (0);
}
/*********************************************************************************************************
** Function name:           nfs_unlink
** Descriptions:            删除文件
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_unlink(mount_point_t *point, const char *path)
{
    REMOVE3args     args;
    REMOVE3res      res;
    nfs_fh3        *dirhandle;
    int             ret;
    nfs_fs         *nfs;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_unlink() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * 获得父目录的文件句柄
     */
    dirhandle = __nfs_lookup_parentdirhandle(nfs, path, &args.object.name);
    if (dirhandle == NULL) {
        return  (-1);
    }

    /*
     * 设定参数
     */
    args.object.dir = *dirhandle;                                       /*  父目录的文件句柄            */

    /*
     * 删除文件
     */
    bzero(&res, sizeof(res));

    nfsproc3_remove_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("nfs_unlink() error: nfs remove file \"%s\" failed, status=%d\n",
                path, res.status);
        ret = -1;
        seterrno(EIO);
    } else {
        ret = 0;
    }

    xdr_free((xdrproc_t)xdr_REMOVE3res, (char *)&res);                  /*  释放返回资源                */

    __nfs_free_filehandle(dirhandle);                                   /*  释放父目录的文件句柄        */

    NFS_FREE(args.object.name);                                         /*  释放终级文件名              */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_mkdir
** Descriptions:            创建目录
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_mkdir(mount_point_t *point, const char *path, mode_t mode)
{
    MKDIR3args      args;
    MKDIR3res       res;
    nfs_fh3        *dirhandle;
    int             ret;
    nfs_fs         *nfs;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_mkdir() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * 获得父目录的文件句柄
     */
    dirhandle = __nfs_lookup_parentdirhandle(nfs, path, &args.where.name);
    if (dirhandle == NULL) {
        return  (-1);
    }

    /*
     * 设定参数
     */
    args.where.dir                        = *dirhandle;                 /*  父目录的文件句柄            */
    args.attributes.mode.set_it           = TRUE;
    args.attributes.mode.set_mode3_u.mode = mode;                       /*  设置目录的权限              */
    args.attributes.uid.set_it            = FALSE;
    args.attributes.gid.set_it            = FALSE;
    args.attributes.size.set_it           = FALSE;
    args.attributes.atime.set_it          = SET_TO_SERVER_TIME;         /*  设置为服务器的时间          */
    args.attributes.mtime.set_it          = SET_TO_SERVER_TIME;

    /*
     * 创建目录
     */
    bzero(&res, sizeof(res));

    nfsproc3_mkdir_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("nfs_mkdir() error: nfs create dir \"%s\" failed, status=%d\n", path, res.status);
        ret = -1;
        seterrno(EIO);
    } else {
        ret = 0;
    }

    xdr_free((xdrproc_t)xdr_MKDIR3res, (char *)&res);                   /*  释放返回资源                */

    __nfs_free_filehandle(dirhandle);                                   /*  释放父目录的文件句柄        */

    NFS_FREE(args.where.name);                                          /*  释放终级目录名              */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_rmdir
** Descriptions:            删除目录
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_rmdir(mount_point_t *point, const char *path)
{
    RMDIR3args      args;
    RMDIR3res       res;
    nfs_fh3        *dirhandle;
    int             ret;
    nfs_fs         *nfs;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_rmdir() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * 获得父目录的文件句柄
     */
    dirhandle = __nfs_lookup_parentdirhandle(nfs, path, &args.object.name);
    if (dirhandle == NULL) {
        return  (-1);
    }

    /*
     * 设定参数
     */
    args.object.dir = *dirhandle;                                       /*  父目录的文件句柄            */

    /*
     * 删除目录
     */
    bzero(&res, sizeof(res));

    nfsproc3_rmdir_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("nfs_remove() error: nfs remove dir \"%s\" failed, status=%d\n",
                path, res.status);
        ret = -1;
        seterrno(EIO);
    } else {
        ret = 0;
    }

    xdr_free((xdrproc_t)xdr_RMDIR3res, (char *)&res);                   /*  释放返回资源                */

    __nfs_free_filehandle(dirhandle);                                   /*  释放父目录的文件句柄        */

    NFS_FREE(args.object.name);                                         /*  释放终级目录名              */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_rename
** Descriptions:            重命名
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_rename(mount_point_t *point, const char *old, const char *new)
{
    RENAME3args     args;
    RENAME3res      res;
    nfs_fh3        *srcdirhandle;
    nfs_fh3        *destdirhandle;
    int             ret;
    nfs_fs         *nfs;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_rename() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * 获得原路径的父目录句柄
     */
    srcdirhandle = __nfs_lookup_parentdirhandle(nfs, old, &args.from.name);
    if (srcdirhandle == NULL) {
        return  (-1);
    }

    /*
     * 获得目的路径的父目录句柄
     */
    destdirhandle = __nfs_lookup_parentdirhandle(nfs, new, &args.to.name);
    if (destdirhandle == NULL) {
        __nfs_free_filehandle(srcdirhandle);
        NFS_FREE(args.from.name);
        return  (-1);
    }

    /*
     * 设定参数
     */
    args.from.dir = *srcdirhandle;                                      /*  原路径的父目录句柄          */
    args.to.dir   = *destdirhandle;                                     /*  目的路径的父目录句柄        */

    /*
     * 重命名
     */
    bzero(&res, sizeof(res));

    nfsproc3_rename_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("nfs_rename() error: nfs rename \"%s\" to \"%s\" failed, status=%d\n",
                old, new, res.status);
        ret = -1;
        seterrno(EIO);
    } else {
        ret = 0;
    }

    xdr_free((xdrproc_t)xdr_RENAME3res, (char *)&res);                  /*  释放返回资源                */

    __nfs_free_filehandle(srcdirhandle);                                /*  释放原路径的父目录句柄      */

    __nfs_free_filehandle(destdirhandle);                               /*  释放目的路径的父目录句柄    */

    NFS_FREE(args.from.name);                                           /*  释放原路径的终级目录项名    */

    NFS_FREE(args.to.name);                                             /*  释放目的路径的终级目录项名  */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_truncate
** Descriptions:            设置文件大小
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_truncate(mount_point_t *point, const char *path, off_t len)
{
    sattr3          attr;
    nfs_fs         *nfs;

    nfs = point->ctx;                                                   /*  获得 NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_truncate() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * 设定属性
     */
    attr.mode.set_it                      = FALSE;
    attr.uid.set_it                       = FALSE;
    attr.gid.set_it                       = FALSE;
    attr.size.set_it                      = TRUE;
    attr.size.set_size3_u.size            = len;                        /*  文件的长度                  */
    attr.atime.set_it                     = DONT_CHANGE;
    attr.mtime.set_it                     = DONT_CHANGE;

    return  __nfspath_set_attr(nfs, path, &attr);                       /*  设置属性                    */
}

file_system_t nfs = {
        .name       = "nfs",
        .mount      = nfs_mount,
        .unmount    = nfs_unmount,
        .stat       = nfs_stat,
        .access     = nfs_access,

        .link       = nfs_rename,
        .unlink     = nfs_unlink,
        .mkdir      = nfs_mkdir,
        .rmdir      = nfs_rmdir,
        .rename     = nfs_rename,
        .truncate   = nfs_truncate,

        .open       = nfs_open,
        .read       = nfs_read,
        .write      = nfs_write,
        .close      = nfs_close,
        .fcntl      = nfs_fcntl,
        .fstat      = nfs_fstat,
        .ftruncate  = nfs_ftruncate,
        .lseek      = nfs_lseek,

        .opendir    = nfs_opendir,
        .readdir    = nfs_readdir,
        .rewinddir  = nfs_rewinddir,
        .closedir   = nfs_closedir,

        .scan       = nfs_scan,
};
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
