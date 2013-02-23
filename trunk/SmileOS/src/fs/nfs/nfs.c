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
** Descriptions:            �����ļ�ϵͳ
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-21
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
** ����
*********************************************************************************************************/
#define NFS_HOST_NAME_LEN   NAME_MAX                                    /*  HOST_NAME �ĳ���            */
#define NFS_EXPORT_PATH_LEN PATH_MAX                                    /*  EXPORT_PATH �ĳ���          */
#define NFS_BLOCK_SIZE      8192                                        /*  ��Ĵ�С                    */
/*********************************************************************************************************
** ��ֲ
*********************************************************************************************************/
#define NFS_MALLOC(x)       kmalloc(x, GFP_KERNEL)
#define NFS_FREE(x)         kfree(x)
#define NFS_STRDUP          strdup
#define NFS_DEBUG           printk
/*********************************************************************************************************
** �����ļ�ϵͳ
*********************************************************************************************************/
typedef struct {
    CLIENT                 *client;                                     /*  �ͻ���                      */
    CLIENT                 *mount_client;                               /*  ���ؿͻ���                  */
    nfs_fh3                *rootdir_filehandle;                         /*  ��Ŀ¼���ļ����            */
    char                    host[NFS_HOST_NAME_LEN];                    /*  HOST_NAME ������            */
    char                    export[NFS_EXPORT_PATH_LEN];                /*  EXPORT_PATH ���·��        */
} nfs_fs;
/*********************************************************************************************************
** �����ļ�
*********************************************************************************************************/
typedef struct {
    nfs_fh3                *filehandle;                                 /*  �ļ����                    */
    size_t                  offset;                                     /*  �ļ��Ķ�дλ��              */
    size_t                  size;                                       /*  �ļ��Ĵ�С                  */
    int                     oflag;                                      /*  �ļ��Ĵ򿪷�ʽ              */
    bool_t                  eof;                                        /*  �ļ��Ƿ����                */

    /*
     * Ŀ¼�Ż�ʹ�����µĳ�Ա����
     */
    cookie3                 cookie;
    cookieverf3             cookieverf;
    entry3                 *entry;
    READDIR3res             res;
    struct dirent           dirent;
} nfs_file;
/*********************************************************************************************************
** Function name:           __nfs_dup_filehandle
** Descriptions:            �����ļ����
** input parameters:        filehandle          Ҫ���Ƶ��ļ����
** output parameters:       NONE
** Returned value:          �µ��ļ����, NULL ��ʾ�ڴ治��
*********************************************************************************************************/
static nfs_fh3 *__nfs_dup_filehandle(const nfs_fh3 *filehandle)
{
    u_int    data_len = filehandle->data.data_len;                      /*  ���ݵĳ���                  */
    nfs_fh3 *new_fh = (nfs_fh3 *)NFS_MALLOC(sizeof(nfs_fh3) + data_len);/*  ���� nfs_fh3 �����ݴ�ſռ� */

    if (new_fh != NULL) {
        new_fh->data.data_val = (char *)new_fh + sizeof(nfs_fh3);       /*  ��¼���ݴ�ſռ����ʼ��ַ  */
        new_fh->data.data_len = data_len;                               /*  ��¼���ݵĳ���              */

        memcpy(new_fh->data.data_val, filehandle->data.data_val,        /*  �������ݵ����ݴ�ſռ�      */
                data_len);
    }
    return  (new_fh);                                                   /*  �����ļ����                */
}
/*********************************************************************************************************
** Function name:           __nfs_free_filehandle
** Descriptions:            �ͷ��ļ����
** input parameters:        filehandle          Ҫ�ͷŵ��ļ����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void __nfs_free_filehandle(nfs_fh3 *filehandle)
{
    NFS_FREE(filehandle);                                               /*  �ͷ� nfs_fh3 �����ݴ�ſռ� */
}
/*********************************************************************************************************
** Function name:           __nfs_lookup_filehandle
** Descriptions:            �����ļ�/Ŀ¼���ļ����
** input parameters:        nfs                 �����ļ�ϵͳ
**                          full_path           �ļ�/Ŀ¼��ȫ·��
** output parameters:       NONE
** Returned value:          �ռ�Ŀ¼����ļ����
*********************************************************************************************************/
static nfs_fh3 *__nfs_lookup_filehandle(nfs_fs *nfs, const char *full_path)
{
    nfs_fh3    *filehandle;
    char       *filename;
    char       *path;
    char       *pathbase;

    if (full_path[0] != '/') {                                          /*  ����ȫ·��                  */
        NFS_DEBUG("__nfs_lookup_filehandle() error: \"%s\" is not full path!\n", full_path);
        seterrno(EINVAL);
        return  (NULL);
    }

    /*
     * ����·��
     */
    pathbase = path = NFS_STRDUP(full_path);
    if (pathbase == NULL) {
        NFS_DEBUG("__nfs_lookup_filehandle() error: NFS_STRDUP() failed, system low memory!\n");
        seterrno(ENOMEM);
        return  (NULL);
    }

    /*
     * ���Ƹ�Ŀ¼���ļ����
     */
    filehandle = __nfs_dup_filehandle(nfs->rootdir_filehandle);
    if (filehandle == NULL) {
        NFS_DEBUG("__nfs_lookup_filehandle() error: __nfs_dup_filehandle() failed, system low memory!\n");
        NFS_FREE(pathbase);
        seterrno(ENOMEM);
        return  (NULL);
    }

    /*
     * ����ռ�Ŀ¼����ļ����
     */
    while ((filename = strtok_r(NULL, "/", &path)) != NULL) {           /*  ��ø���Ŀ¼�������        */
        LOOKUP3args     args;
        LOOKUP3res      res;

        /*
         * �趨����
         */
        args.what.dir  = *filehandle;                                   /*  ��Ŀ¼���ļ����            */
        args.what.name = filename;                                      /*  Ŀ¼�������                */

        /*
         * �ڸ�Ŀ¼���Ҹ�Ŀ¼��
         */
        bzero(&res, sizeof(res));

        nfsproc3_lookup_3(&args, &res, nfs->client);

        if (res.status != NFS3_OK) {
            if (res.status != NFS3ERR_NOENT) {                          /*  ��������Ϊ�����ڸ�Ŀ¼��  */
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

        __nfs_free_filehandle(filehandle);                              /*  �ͷŸ�Ŀ¼���ļ����        */

        /*
         * ���Ƹ�Ŀ¼���ļ����, ����Ϊ��һ�β���ʱ�ĸ�Ŀ¼�ļ����
         */
        filehandle = __nfs_dup_filehandle(&res.LOOKUP3res_u.resok.object);

        xdr_free((xdrproc_t)xdr_LOOKUP3res, (char *)&res);              /*  �ͷŷ�����Դ                */

        if (filehandle == NULL) {                                       /*  �����ļ��������            */
            NFS_DEBUG("__nfs_lookup_filehandle() error: "
                      "__nfs_dup_filehandle() failed, system low memory!\n");
            NFS_FREE(pathbase);
            seterrno(ENOMEM);
            return  (NULL);
        }
    }

    NFS_FREE(pathbase);                                                 /*  �ͷ�·��                    */

    return filehandle;                                                  /*  �����ռ�Ŀ¼����ļ����    */
}
/*********************************************************************************************************
** Function name:           __nfs_lookup_parentdirhandle
** Descriptions:            ����ļ�/Ŀ¼�ĸ�Ŀ¼���ļ����
** input parameters:        nfs                 �����ļ�ϵͳ
**                          full_path           �ļ�/Ŀ¼��ȫ·��
** output parameters:       file_name           �ռ��ļ�/Ŀ¼��(Ҫ�����ͷ���ռ�õ��ڴ�)
** Returned value:          ��Ŀ¼���ļ����
*********************************************************************************************************/
static nfs_fh3 *__nfs_lookup_parentdirhandle(nfs_fs *nfs, const char *full_path, char **file_name)
{
    nfs_fh3    *filehandle;
    char       *filename;
    char       *path;
    char       *pathbase;
    char       *end;
    char       *tmp;

    if (full_path[0] != '/') {                                          /*  ����ȫ·��                  */
        NFS_DEBUG("__nfs_lookup_parentdirhandle() error: \"%s\" is not full path!\n", full_path);
        seterrno(EINVAL);
        return  (NULL);
    }

    /*
     * ����·��
     */
    pathbase = path = NFS_STRDUP(full_path);
    if (pathbase == NULL) {
        NFS_DEBUG("__nfs_lookup_parentdirhandle() error: NFS_STRDUP() failed, system low memory!\n");
        seterrno(ENOMEM);
        return  (NULL);
    }

    /*
     * ���Ƹ�Ŀ¼���ļ����
     */
    filehandle = __nfs_dup_filehandle(nfs->rootdir_filehandle);
    if (filehandle == NULL) {
        NFS_DEBUG("__nfs_lookup_parentdirhandle() error: __nfs_dup_filehandle() failed, system low memory!\n");
        NFS_FREE(pathbase);
        seterrno(ENOMEM);
        return  (NULL);
    }

    /*
     * �ҳ��ռ�Ŀ¼������ǰ�� '/'
     */
    end = pathbase;

    while (end && (tmp = strchr(end, '/')) && tmp[1] != '\0') {
        end = tmp + 1;
    }

    /*
     * ����ռ�Ŀ¼��ĸ�Ŀ¼���ļ����
     */
    while ((filename = strtok_r(NULL, "/", &path)) != NULL              /*  ��ø���Ŀ¼�������        */
            && filename < end) {                                        /*  �����ռ�Ŀ¼������ǰ����    */
        LOOKUP3args     args;
        LOOKUP3res      res;

        /*
         * �趨����
         */
        args.what.dir  = *filehandle;                                   /*  ��Ŀ¼���ļ����            */
        args.what.name = filename;                                      /*  Ŀ¼�������                */

        /*
         * �ڸ�Ŀ¼���Ҹ�Ŀ¼��
         */
        bzero(&res, sizeof(res));

        nfsproc3_lookup_3(&args, &res, nfs->client);

        if (res.status != NFS3_OK) {
            if (res.status != NFS3ERR_NOENT) {                          /*  ��������Ϊ�����ڸ�Ŀ¼��  */
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

        __nfs_free_filehandle(filehandle);                              /*  �ͷŸ�Ŀ¼���ļ����        */

        /*
         * ���Ƹ�Ŀ¼���ļ����, ����Ϊ��һ�β���ʱ�ĸ�Ŀ¼�ļ����
         */
        filehandle = __nfs_dup_filehandle(&res.LOOKUP3res_u.resok.object);

        xdr_free((xdrproc_t)xdr_LOOKUP3res, (char *)&res);              /*  �ͷŷ�����Դ                */

        if (filehandle == NULL) {                                       /*  �����ļ��������            */
            NFS_DEBUG("__nfs_lookup_parentdirhandle() error: "
                      "__nfs_dup_filehandle() failed, system low memory!\n");
            NFS_FREE(pathbase);
            seterrno(ENOMEM);
            return  (NULL);
        }
    }

    /*
     * ��ȡ�ռ��ļ�/Ŀ¼��
     */
    {
        *file_name = NFS_STRDUP(end);                                   /*  �����ռ�Ŀ¼�������        */
        if (*file_name != NULL) {
            tmp = strchr(*file_name, '/');                              /*  ȥ������� '/', �����      */
            if (tmp != NULL) {
                *tmp = '\0';
            }
        } else {
            NFS_DEBUG("__nfs_lookup_parentdirhandle() error: NFS_STRDUP() failed, system low memory!\n");
            __nfs_free_filehandle(filehandle);                          /*  �ͷŸ�Ŀ¼���ļ����        */
            filehandle = NULL;
            seterrno(ENOMEM);
        }
    }

    NFS_FREE(pathbase);                                                 /*  �ͷ�·��                    */

    return filehandle;                                                  /*  ���ظ�Ŀ¼���ļ����        */
}
/*********************************************************************************************************
** Function name:           __nfs_parse_host_export
** Descriptions:            ��ȡ hostname �� export_path
** input parameters:        host_export         hostname �� export_path ��ɵ��ַ���,
**                                              ���� "192.168.9.17:/nfs"
**                          host_len            hostname �ַ����������
**                          export_len          export_path �ַ����������
** output parameters:       host                hostname
**                          export              export_path
** Returned value:          �ɹ����� 0, ������ -1
*********************************************************************************************************/
static int __nfs_parse_host_export(const char  *host_export,
                                   char        *host,
                                   size_t       host_len,
                                   char        *export,
                                   size_t       export_len)
{
    int i;

    /*
     * ��¼ hostname
     */
    for (i = 0; i < host_len; i++) {
        if (host_export[i] == '\0') {                                   /*  �����ǰ����, ����          */
            return  (-1);
        }

        if (host_export[i] == ':') {                                    /*  ����� ':' �ָ���, ����     */
            break;
        } else {
            host[i] = host_export[i];                                   /*  ��¼ hostname               */
        }
    }

    if (i == host_len) {                                                /*  ��� hostname ̫��, ����    */
        return  (-1);
    }

    host_len = i;
    host[host_len] = '\0';                                              /*  hostname ���������         */
    host_len++;

    /*
     * ��¼ export_path
     */
    for (i = host_len; i < host_len + export_len; i++) {
        if (host_export[i] == '\0') {                                   /*  ���������                  */
            export[i - host_len] = '\0';                                /*  ���������                  */
            return  (0);
        } else {
            export[i - host_len] = host_export[i];                      /*  ��¼ export_path            */
        }
    }
    return  (-1);                                                       /*  export_path ̫��, ����      */
}
/*********************************************************************************************************
** Function name:           nfs_mount
** Descriptions:            ���� NFS
** input parameters:        point               ���ص�
**                          dev                 �豸
**                          dev_name            �豸��
**                          param               ���ز���
**                          hostname �� export_path ��ɵ��ַ���, ���� "192.168.9.17:/nfs"
** output parameters:       NONE
** Returned value:          �ɹ����� 0, ������ -1
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
    if (!is_network_up(0)) {                                            /*  ����������δ����          */
        NFS_DEBUG("nfs_mount() error: network service is not ready!\n");
        seterrno(EIO);
        goto __error_handle;
    }

    /*
     * ��ȡ host_name �� export_path
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
     * �������ؿͻ���
     */
    nfs->mount_client = clnt_create(nfs->host, MOUNT_PROGRAM, MOUNT_V3, "udp");
    if (nfs->mount_client == NULL) {
        NFS_DEBUG("nfs_mount() error: create mount client failed!\n");
        seterrno(ENOMEM);
        goto __error_handle;
    }

    /*
     * ����
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
     * ���� NFS �ͻ���
     */
    nfs->client = clnt_create(nfs->host, NFS_PROGRAM, NFS_V3, "udp");
    if (nfs->client == NULL) {
        NFS_DEBUG("nfs_mount() error: create nfs client failed!\n");
        seterrno(ENOMEM);
        goto __error_handle;
    }

    /*
     * ���Ƹ�Ŀ¼���ļ����
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
     * �����ǳ�����
     */
__error_handle:
    if (nfs->mount_client != NULL) {

        if (nfs->mount_client->cl_auth != NULL) {
            auth_destroy(nfs->mount_client->cl_auth);                   /*  ���ٹ��ؿͻ��˵� AUTH       */
            nfs->mount_client->cl_auth = NULL;
        }

        clnt_destroy(nfs->mount_client);                                /*  ���ٹ��ؿͻ���              */
        nfs->mount_client = NULL;
    }

    if (nfs->client != NULL) {

        if (nfs->client->cl_auth != NULL) {
            auth_destroy(nfs->client->cl_auth);                         /*  ���� NFS �ͻ��˵� AUTH      */
            nfs->client->cl_auth = NULL;
        }

        clnt_destroy(nfs->client);                                      /*  ���� NFS �ͻ���             */
        nfs->client = NULL;
    }

    NFS_FREE(nfs);

    return  (-1);
}
/*********************************************************************************************************
** Function name:           nfs_unmount
** Descriptions:            ȡ������ NFS
** input parameters:        point               ���ص�
**                          param               ���ز���
** output parameters:       NONE
** Returned value:          �ɹ����� 0, ������ -1
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

    ret = mountproc3_umnt_3((dirpath *)&nfs->export, NULL,              /*  ȡ������                    */
                            nfs->mount_client);
    if (ret != RPC_SUCCESS) {
        NFS_DEBUG("nfs_unmount() error: nfs unmount failed!\n");
        seterrno(EBUSY);
        return  (-1);
    }

    if (nfs->client->cl_auth != NULL) {
        auth_destroy(nfs->client->cl_auth);                             /*  ���� NFS �ͻ��˵� AUTH      */
        nfs->client->cl_auth = NULL;
    }

    clnt_destroy(nfs->client);                                          /*  ���� NFS �ͻ���             */
    nfs->client = NULL;

    if (nfs->mount_client->cl_auth != NULL) {
        auth_destroy(nfs->mount_client->cl_auth);                       /*  ���ٹ��ؿͻ��˵� AUTH       */
        nfs->mount_client->cl_auth = NULL;
    }

    clnt_destroy(nfs->mount_client);                                    /*  ���ٹ��ؿͻ���              */
    nfs->mount_client = NULL;

    __nfs_free_filehandle(nfs->rootdir_filehandle);                     /*  �ͷŸ�Ŀ¼���ļ����        */

    NFS_FREE(nfs);
    point->ctx = NULL;

    return  (0);
}
/*********************************************************************************************************
** Function name:           __nfsfile_create
** Descriptions:            �����ļ�
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
     * ��ø�Ŀ¼���ļ����
     */
    dirhandle = __nfs_lookup_parentdirhandle(nfs, full_path, &args.where.name);
    if (dirhandle == NULL) {
        return  (-1);
    }

    /*
     * �趨����
     */
    args.where.dir                                    = *dirhandle;     /*  ��Ŀ¼���ļ����            */
    args.how.mode                                     = GUARDED;
    args.how.createhow3_u.obj_attributes.mode.set_it  = TRUE;
    args.how.createhow3_u.obj_attributes.mode.set_mode3_u.mode = mode;  /*  �����ļ���Ȩ��              */
    args.how.createhow3_u.obj_attributes.uid.set_it   = FALSE;
    args.how.createhow3_u.obj_attributes.gid.set_it   = FALSE;
    args.how.createhow3_u.obj_attributes.size.set_it  = FALSE;
    args.how.createhow3_u.obj_attributes.atime.set_it = SET_TO_SERVER_TIME; /*  ����Ϊ��������ʱ��      */
    args.how.createhow3_u.obj_attributes.mtime.set_it = SET_TO_SERVER_TIME;

    /*
     * �����ļ�
     */
    bzero(&res, sizeof(res));

    nfsproc3_create_3(&args, &res, nfs->client);

    if (   res.status != NFS3_OK
        && res.status != NFS3ERR_EXIST) {                               /*  �������ڶ�����, ���ر�����*/
        NFS_DEBUG("nfs_create() error: nfs create file \"%s\" failed, status=%d\n",
                full_path, res.status);
        ret = -1;
        seterrno(EIO);
    } else {
        ret = 0;
    }

    xdr_free((xdrproc_t)xdr_CREATE3res, (char *)&res);                  /*  �ͷŷ�����Դ                */

    __nfs_free_filehandle(dirhandle);                                   /*  �ͷŸ�Ŀ¼���ļ����        */

    NFS_FREE(args.where.name);                                          /*  �ͷ��ռ��ļ���              */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           __nfsfile_get_size
** Descriptions:            ����ļ���С
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
     * �趨����
     */
    args.object = *nfsfile->filehandle;                                 /*  �ļ����                    */

    /*
     * �������
     */
    bzero(&res, sizeof(res));

    nfsproc3_getattr_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("__nfs_get_filesize() error: nfs get file attr failed, status=%d\n", res.status);
        size = -1;
        seterrno(EIO);
    } else {
        attr = &res.GETATTR3res_u.resok.obj_attributes;                 /*  �ļ�������                  */

        size = attr->size;                                              /*  �ļ��Ĵ�С                  */
    }

    xdr_free((xdrproc_t)xdr_GETATTR3res, (char *)&res);                 /*  �ͷŷ�����Դ                */

    return  (size);                                                     /*  �����ļ��Ĵ�С              */
}
/*********************************************************************************************************
** Function name:           __nfsfile_is_dir
** Descriptions:            �����ļ��Ƿ�һ��Ŀ¼
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
     * �趨����
     */
    args.object = *nfsfile->filehandle;                                 /*  �ļ����                    */

    /*
     * �������
     */
    bzero(&res, sizeof(res));

    nfsproc3_getattr_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("__nfsfile_is_dir() error: nfs get file attr failed, status=%d\n", res.status);
        is_dir = TRUE;
    } else {
        attr   = &res.GETATTR3res_u.resok.obj_attributes;               /*  ���������                  */

        is_dir = (attr->type == NFS3DIR) ? TRUE : FALSE;                /*  ���ݶ���������ж��Ƿ�ΪĿ¼*/
    }

    xdr_free((xdrproc_t)xdr_GETATTR3res, (char *)&res);                 /*  �ͷŷ�����Դ                */

    return  (is_dir);                                                   /*  �����Ƿ�ΪĿ¼              */
}
/*********************************************************************************************************
** Function name:           __nfsfile_set_attr
** Descriptions:            �����ļ�����
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
     * �趨����
     */
    args.object         = *nfsfile->filehandle;                         /*  �ļ�/Ŀ¼���ļ����         */
    args.guard.check    = TRUE;                                         /*  ��Ҫ���                    */
    args.new_attributes = *attr;                                        /*  ����                        */

    /*
     * ��������
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

    xdr_free((xdrproc_t)xdr_SETATTR3res, (char *)&res);                 /*  �ͷŷ�����Դ                */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           __nfspath_set_attr
** Descriptions:            �����ļ�����
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
     * ����ļ�/Ŀ¼���ļ����
     */
    filehandle = __nfs_lookup_filehandle(nfs, full_path);
    if (filehandle == NULL) {
        return  (-1);
    }

    /*
     * �趨����
     */
    args.object         = *filehandle;                                  /*  �ļ�/Ŀ¼���ļ����         */
    args.guard.check    = TRUE;                                         /*  ��Ҫ���                    */
    args.new_attributes = *attr;                                        /*  ����                        */

    /*
     * ��������
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

    xdr_free((xdrproc_t)xdr_SETATTR3res, (char *)&res);                 /*  �ͷŷ�����Դ                */

    __nfs_free_filehandle(filehandle);                                  /*  �ͷ��ļ�/Ŀ¼���ļ����     */

    return  (ret);
}
static int nfs_ftruncate(mount_point_t *point, file_t *file, off_t len);
/*********************************************************************************************************
** Function name:           nfs_open
** Descriptions:            ���ļ�
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
     * �����ļ�/Ŀ¼
     */
    filehandle = __nfs_lookup_filehandle(nfs, path);
    if (filehandle != NULL) {                                           /*  ����ļ�/Ŀ¼�Ѿ�����       */
        /*
         * Open should fail if O_CREAT and O_EXCL are specified since
         * the file exists
         */
        if ((oflag & O_EXCL) && (oflag & O_CREAT)) {
            __nfs_free_filehandle(filehandle);                          /*  �ͷ��ļ����                */
            seterrno(EINVAL);
            return  (-1);
        }
    } else if (oflag & O_CREAT) {
        seterrno(0);
        /*
         * Let's see if we can create this file
         */
        if (__nfsfile_create(nfs, path, mode) < 0) {                    /*  �����ļ�                    */
            return  (-1);
        }

        /*
         * ����ļ����
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
        __nfs_free_filehandle(filehandle);                              /*  �ͷ��ļ����                */
        NFS_DEBUG("nfs_mount() error: alloc nfsfile failed, system low memory!\n");
        seterrno(ENOMEM);
        return -1;
    }

    /*
     * ��ʼ���ļ��ṹ
     */
    nfsfile->filehandle = filehandle;                                   /*  �ļ����                    */
    nfsfile->oflag      = oflag;                                        /*  �ļ��Ĵ򿪷�ʽ              */
    nfsfile->offset     = 0;                                            /*  �ļ��Ķ�дλ��              */
    nfsfile->size       = 0;                                            /*  �ļ��Ĵ�С                  */
    nfsfile->eof        = FALSE;                                        /*  EOF ��־                    */

    nfsfile->cookie     = 0;
    nfsfile->entry      = NULL;

    bzero(&nfsfile->cookieverf, sizeof(cookieverf3));

    bzero(&nfsfile->res, sizeof(READDIR3res));

    if (__nfsfile_is_dir(nfs, nfsfile)) {                               /*  �����Ŀ¼, ���� 1          */
        __nfs_free_filehandle(filehandle);
        NFS_FREE(nfsfile);
        seterrno(EFTYPE);
        return  (-1);
    } else {
        if (oflag & O_TRUNC) {                                          /*  ���Ҫ��ض��ļ�            */
            if (nfs_ftruncate(point, file, 0) < 0) {                    /*  �����ļ���СΪ 0            */
                __nfs_free_filehandle(filehandle);
                NFS_FREE(nfsfile);
                return  (-1);
            }
        }

        nfsfile->size = __nfsfile_get_size(nfs, nfsfile);               /*  ����ļ���С                */
        if (nfsfile->size == (size_t)-1) {
            __nfs_free_filehandle(filehandle);
            NFS_FREE(nfsfile);
            return  (-1);
        }

        if (oflag & O_APPEND) {                                         /*  �����׷�ӵķ�ʽ��        */
            nfsfile->offset = nfsfile->size;                            /*  ���ö�дλ��Ϊ�ļ�β        */
        }

        nfsfile->eof = (nfsfile->offset >= nfsfile->size) ?             /*  ���� EOF ��־               */
                        TRUE: FALSE;

        file->ctx = nfsfile;

        return  (0);
    }
}
/*********************************************************************************************************
** Function name:           nfs_close
** Descriptions:            �ر��ļ�
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

    __nfs_free_filehandle(nfsfile->filehandle);                         /*  �ͷ��ļ����                */

    return 0;
}
/*********************************************************************************************************
** Function name:           __nfs_read
** Descriptions:            ���ļ�
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

    nfs = point->ctx;                                                   /*  ��� NFS                    */
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

    if (nfsfile->eof == TRUE) {                                         /*  ����Ѿ������ļ�β, �򷵻� 0*/
        return  (0);
    }

    /*
     * �趨����
     */
    args.file   = *nfsfile->filehandle;                                 /*  �ļ����                    */
    args.offset = nfsfile->offset;                                      /*  ��ȡλ��                    */
    args.count  = min(nbyte, NFS_BLOCK_SIZE);                           /*  �ܹ���ȡ���ֽ���            */
                                                                        /*  һ������ȡһ����          */
    /*
     * ���ļ�
     */
    bzero(&res, sizeof(res));

    nfsproc3_read_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("__nfs_read() error: nfs read file failed, status=%d\n", res.status);
        nread = -1;
        seterrno(EIO);
    } else {
        nread            = res.READ3res_u.resok.count;                  /*  ʵ�ʶ�ȡ���ֽ���            */

        nfsfile->offset += nread;                                       /*  ������дλ��                */

        nfsfile->eof     = (nfsfile->offset >= nfsfile->size) ?         /*  ���� EOF ��־               */
                            TRUE : FALSE;

        memcpy(buffer, res.READ3res_u.resok.data.data_val, nread);      /*  �������������ݵ�������      */
    }

    xdr_free((xdrproc_t)xdr_READ3res, (char *)&res);                    /*  �ͷŷ�����Դ                */

    return  (nread);                                                    /*  ����ʵ�ʶ�ȡ���ֽ���        */
}
/*********************************************************************************************************
** Function name:           nfs_read
** Descriptions:            ���ļ�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static ssize_t nfs_read(mount_point_t *point, file_t *file, void *buf, size_t len)
{
    ssize_t     nread;
    size_t      nleft = len;
    char       *buff  = (char *)buf;

    while (nleft > 0) {                                                 /*  ʣ����ֽ������� 0          */

        nread = __nfs_read(point, file, buff, nleft);                   /*  ��ȡʣ����ֽ�              */

        if (nread == -1) {                                              /*  ������ļ�����, ���� -1     */
            return  (-1);
        } else if (nread == 0) {                                        /*  ��������ļ�β              */
            return  (len - nleft);                                      /*  ����ʵ�ʶ�ȡ���ֽ���        */
        } else {
            nleft -= nread;                                             /*  ����ʣ����ֽ���            */
            buff  += nread;                                             /*  �������ݴ�ŵ�ַ            */
        }
    }
    return  (len);                                                      /*  ���سɹ���ȡ���ֽ���        */
}
/*********************************************************************************************************
** Function name:           __nfs_write
** Descriptions:            д�ļ�
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

    nfs = point->ctx;                                                   /*  ��� NFS                    */
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

    if (nfsfile->oflag & O_APPEND) {                                    /*  �����׷�ӵķ�ʽ��        */
        nfsfile->offset = nfsfile->size;                                /*  д��λ������Ϊ�ļ�β        */
    }

    nbyte = min(nbyte, NFS_BLOCK_SIZE);                                 /*  һ�����д��һ����          */

    /*
     * �趨����
     */
    args.stable         = FILE_SYNC;                                    /*  ����ͬ����                  */
    args.file           = *nfsfile->filehandle;                         /*  �ļ����                    */
    args.offset         = nfsfile->offset;                              /*  д��λ��                    */
    args.count          = nbyte;                                        /*  ϣ��д����ֽ���            */

    args.data.data_len  = nbyte;                                        /*  ���ݵĳ���                  */
    args.data.data_val  = (char *)buffer;                               /*  ���ݵ���ʼ��ַ              */

    /*
     * д�ļ�
     */
    bzero(&res, sizeof(res));

    nfsproc3_write_3(&args, &res, nfs->client);

    if (res.status != NFS3_OK) {
        NFS_DEBUG("__nfs_write() error: nfs write file failed, status=%d\n", res.status);
        nwrite = -1;
        seterrno(EIO);
    } else {
        nwrite           = res.WRITE3res_u.resok.count;                 /*  ʵ��д����ֽ���            */

        nfsfile->offset += nwrite;                                      /*  ������дλ��                */

        nfsfile->size    = max(nfsfile->size, nfsfile->offset);         /*  �����ļ��Ĵ�С              */

        nfsfile->eof     = (nfsfile->offset >= nfsfile->size) ?         /*  ���� EOF ��־               */
                            TRUE : FALSE;
    }

    xdr_free((xdrproc_t)xdr_WRITE3res, (char *)&res);                   /*  �ͷŷ�����Դ                */

    return  (nwrite);                                                   /*  ����ʵ��д����ֽ���        */
}
/*********************************************************************************************************
** Function name:           nfs_write
** Descriptions:            д�ļ�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static ssize_t nfs_write(mount_point_t *point, file_t *file, const void *buf, size_t len)
{
    int          nwrite;
    size_t       nleft = len;
    const char  *buff  = (const char *)buf;

    while (nleft > 0) {                                                 /*  ʣ����ֽ������� 0          */

        nwrite = __nfs_write(point, file, buff, nleft);                 /*  д��ʣ����ֽ�              */

        if (nwrite == -1) {                                             /*  д�ļ�����                  */
            if (nleft == len) {                                         /*  δ�ɹ�д���                */
                return  (-1);                                           /*  ���� -1                     */
            } else {
                return  (len - nleft);                                  /*  ����ʵ��д����ֽ���        */
            }
        } else {
            nleft -= nwrite;                                            /*  ����ʣ����ֽ���            */
            buff  += nwrite;                                            /*  �������ݴ�ŵ�ַ            */
        }
    }
    return  (len);                                                      /*  ���سɹ�д����ֽ���        */
}
/*********************************************************************************************************
** Function name:           nfs_fcntl
** Descriptions:            �����ļ�
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
** Descriptions:            �����ļ�
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

    nfs = point->ctx;                                                   /*  ��� NFS                    */
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
     * �趨����
     */
    args.object = *nfsfile->filehandle;                                 /*  �ļ�/Ŀ¼���ļ����         */

    /*
     * �������
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
         * �趨 stat ״̬�ṹ
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

    xdr_free((xdrproc_t)xdr_GETATTR3res, (char *)&res);                 /*  �ͷŷ�����Դ                */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_ftruncate
** Descriptions:            �����ļ��ĳ���
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

    nfs = point->ctx;                                                   /*  ��� NFS                    */
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
     * �趨����
     */
    attr.mode.set_it                      = FALSE;
    attr.uid.set_it                       = FALSE;
    attr.gid.set_it                       = FALSE;
    attr.size.set_it                      = TRUE;
    attr.size.set_size3_u.size            = len;                        /*  �ļ��ĳ���                  */
    attr.atime.set_it                     = DONT_CHANGE;
    attr.mtime.set_it                     = DONT_CHANGE;

    ret = __nfsfile_set_attr(nfs, nfsfile, &attr);                      /*  ��������                    */
    if (ret == 0) {
        nfsfile->size = len;                                            /*  �����ļ��Ĵ�С              */
        nfsfile->eof  = (nfsfile->offset >= nfsfile->size) ?            /*  ���� EOF ��־               */
                        TRUE : FALSE;
    }
    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_lseek
** Descriptions:            �����ļ���дλ��
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

    nfs = point->ctx;                                                   /*  ��� NFS                    */
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

    filesize = nfsfile->size;                                           /*  ����ļ��Ĵ�С              */

    if (whence == SEEK_SET) {                                           /*  ֱ������                    */
        if (offset >= 0) {
            newoffset = offset;
        }
    } else if (whence == SEEK_CUR) {                                    /*  ��Ե�ǰ�Ķ�дλ��          */
        if ((nfsfile->offset + offset) >= 0) {
            newoffset = (nfsfile->offset + offset);
        }
    } else if (whence == SEEK_END) {                                    /*  ����ļ�β                  */
        if (filesize >= 0 && (filesize + offset) >= 0) {
            newoffset = filesize + offset;
        }
    }

    if (newoffset >= 0) {
        if (newoffset > filesize) {                                     /*  ����ļ�Ҫ����            */
            if (nfs_ftruncate(point, file, newoffset) < 0) {            /*  �����ļ��Ĵ�С              */
                return  (-1);
            }
        }

        nfsfile->offset = newoffset;                                    /*  �����дλ��                */
        nfsfile->eof    = (nfsfile->offset >= nfsfile->size) ?          /*  ���� EOF ��־               */
                          TRUE : FALSE;
    } else {
        seterrno(EINVAL);
    }
    return  (newoffset);                                                /*  �����µĶ�дλ��            */
}
/*********************************************************************************************************
** Function name:           nfs_scan
** Descriptions:            ����ļ��ɶ�д״̬
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
** Descriptions:            ����ļ�״̬
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

    nfs = point->ctx;                                                   /*  ��� NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_stat() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * ����ļ�/Ŀ¼���ļ����
     */
    filehandle = __nfs_lookup_filehandle(nfs, path);
    if (filehandle == NULL) {
        return  (-1);
    }

    /*
     * �趨����
     */
    args.object = *filehandle;                                          /*  �ļ�/Ŀ¼���ļ����         */

    /*
     * �������
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
         * �趨 stat ״̬�ṹ
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

    xdr_free((xdrproc_t)xdr_GETATTR3res, (char *)&res);                 /*  �ͷŷ�����Դ                */

    __nfs_free_filehandle(filehandle);                                  /*  �ͷ��ļ�/Ŀ¼���ļ����     */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_access
** Descriptions:            ����ļ�״̬
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
** Descriptions:            ��Ŀ¼
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
     * ���Ŀ¼���ļ����
     */
    dirhandle = __nfs_lookup_filehandle(nfs, path);
    if (dirhandle == NULL) {
        return  (-1);
    }

    nfsfile = NFS_MALLOC(sizeof(nfs_file));
    if (nfsfile == NULL) {
        __nfs_free_filehandle(dirhandle);                               /*  �ͷ��ļ����                */
        NFS_DEBUG("nfs_opendir() error: alloc nfsfile failed, system low memory!\n");
        seterrno(ENOMEM);
        return -1;
    }

    /*
     * ��ʼ���ļ��ṹ
     */
    nfsfile->filehandle = dirhandle;                                    /*  Ŀ¼���ļ����              */
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
** Descriptions:            ��Ŀ¼��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static struct dirent *nfs_readdir(mount_point_t *point, file_t *file)
{
    nfs_file *nfsfile;
    nfs_fs   *nfs;

    nfs = point->ctx;                                                   /*  ��� NFS                    */
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

    if (nfsfile->entry == NULL) {                                       /*  ���δ������                */
        READDIR3args args;

        /*
         * �趨����
         */
        args.dir    = *nfsfile->filehandle;                             /*  Ŀ¼���                    */
        args.cookie = nfsfile->cookie;                                  /*  cookie                      */
        args.count  = 1024;

        memcpy(&args.cookieverf, &nfsfile->cookieverf, sizeof(cookieverf3));

        xdr_free((xdrproc_t)xdr_READDIR3res, (char *)&nfsfile->res);    /*  �ͷŷ�����Դ                */

        /*
         * ��Ŀ¼
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

    if (nfsfile->eof == TRUE && nfsfile->entry == NULL) {               /*  ���Ŀ¼������Ŀ¼Ϊ��      */
        return  (NULL);
    }

    nfsfile->cookie = nfsfile->entry->cookie;                           /*  cookie                      */

    strlcpy(nfsfile->dirent.d_name, nfsfile->entry->name, NAME_MAX);    /*  ����Ŀ¼�������            */

    nfsfile->entry = nfsfile->entry->nextentry;                         /*  ָ����һ��Ŀ¼��            */

    return  (&nfsfile->dirent);                                         /*  ����Ŀ¼��ָ��              */
}
/*********************************************************************************************************
** Function name:           nfs_rewinddir
** Descriptions:            �����Ŀ¼��
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

    xdr_free((xdrproc_t)xdr_READDIR3res, (char *)&nfsfile->res);        /*  �ͷŷ�����Դ                */

    nfsfile->cookie     = 0;
    nfsfile->entry      = NULL;
    nfsfile->eof        = FALSE;

    bzero(&nfsfile->cookieverf, sizeof(cookieverf3));

    bzero(&nfsfile->res, sizeof(READDIR3res));

    return 0;
}
/*********************************************************************************************************
** Function name:           nfs_closedir
** Descriptions:            �ر�Ŀ¼
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

    xdr_free((xdrproc_t)xdr_READDIR3res, (char *)&nfsfile->res);        /*  �ͷŷ�����Դ                */

    __nfs_free_filehandle(nfsfile->filehandle);                         /*  �ͷ�Ŀ¼���ļ����          */

    return  (0);
}
/*********************************************************************************************************
** Function name:           nfs_unlink
** Descriptions:            ɾ���ļ�
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

    nfs = point->ctx;                                                   /*  ��� NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_unlink() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * ��ø�Ŀ¼���ļ����
     */
    dirhandle = __nfs_lookup_parentdirhandle(nfs, path, &args.object.name);
    if (dirhandle == NULL) {
        return  (-1);
    }

    /*
     * �趨����
     */
    args.object.dir = *dirhandle;                                       /*  ��Ŀ¼���ļ����            */

    /*
     * ɾ���ļ�
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

    xdr_free((xdrproc_t)xdr_REMOVE3res, (char *)&res);                  /*  �ͷŷ�����Դ                */

    __nfs_free_filehandle(dirhandle);                                   /*  �ͷŸ�Ŀ¼���ļ����        */

    NFS_FREE(args.object.name);                                         /*  �ͷ��ռ��ļ���              */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_mkdir
** Descriptions:            ����Ŀ¼
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

    nfs = point->ctx;                                                   /*  ��� NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_mkdir() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * ��ø�Ŀ¼���ļ����
     */
    dirhandle = __nfs_lookup_parentdirhandle(nfs, path, &args.where.name);
    if (dirhandle == NULL) {
        return  (-1);
    }

    /*
     * �趨����
     */
    args.where.dir                        = *dirhandle;                 /*  ��Ŀ¼���ļ����            */
    args.attributes.mode.set_it           = TRUE;
    args.attributes.mode.set_mode3_u.mode = mode;                       /*  ����Ŀ¼��Ȩ��              */
    args.attributes.uid.set_it            = FALSE;
    args.attributes.gid.set_it            = FALSE;
    args.attributes.size.set_it           = FALSE;
    args.attributes.atime.set_it          = SET_TO_SERVER_TIME;         /*  ����Ϊ��������ʱ��          */
    args.attributes.mtime.set_it          = SET_TO_SERVER_TIME;

    /*
     * ����Ŀ¼
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

    xdr_free((xdrproc_t)xdr_MKDIR3res, (char *)&res);                   /*  �ͷŷ�����Դ                */

    __nfs_free_filehandle(dirhandle);                                   /*  �ͷŸ�Ŀ¼���ļ����        */

    NFS_FREE(args.where.name);                                          /*  �ͷ��ռ�Ŀ¼��              */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_rmdir
** Descriptions:            ɾ��Ŀ¼
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

    nfs = point->ctx;                                                   /*  ��� NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_rmdir() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * ��ø�Ŀ¼���ļ����
     */
    dirhandle = __nfs_lookup_parentdirhandle(nfs, path, &args.object.name);
    if (dirhandle == NULL) {
        return  (-1);
    }

    /*
     * �趨����
     */
    args.object.dir = *dirhandle;                                       /*  ��Ŀ¼���ļ����            */

    /*
     * ɾ��Ŀ¼
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

    xdr_free((xdrproc_t)xdr_RMDIR3res, (char *)&res);                   /*  �ͷŷ�����Դ                */

    __nfs_free_filehandle(dirhandle);                                   /*  �ͷŸ�Ŀ¼���ļ����        */

    NFS_FREE(args.object.name);                                         /*  �ͷ��ռ�Ŀ¼��              */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_rename
** Descriptions:            ������
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

    nfs = point->ctx;                                                   /*  ��� NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_rename() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * ���ԭ·���ĸ�Ŀ¼���
     */
    srcdirhandle = __nfs_lookup_parentdirhandle(nfs, old, &args.from.name);
    if (srcdirhandle == NULL) {
        return  (-1);
    }

    /*
     * ���Ŀ��·���ĸ�Ŀ¼���
     */
    destdirhandle = __nfs_lookup_parentdirhandle(nfs, new, &args.to.name);
    if (destdirhandle == NULL) {
        __nfs_free_filehandle(srcdirhandle);
        NFS_FREE(args.from.name);
        return  (-1);
    }

    /*
     * �趨����
     */
    args.from.dir = *srcdirhandle;                                      /*  ԭ·���ĸ�Ŀ¼���          */
    args.to.dir   = *destdirhandle;                                     /*  Ŀ��·���ĸ�Ŀ¼���        */

    /*
     * ������
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

    xdr_free((xdrproc_t)xdr_RENAME3res, (char *)&res);                  /*  �ͷŷ�����Դ                */

    __nfs_free_filehandle(srcdirhandle);                                /*  �ͷ�ԭ·���ĸ�Ŀ¼���      */

    __nfs_free_filehandle(destdirhandle);                               /*  �ͷ�Ŀ��·���ĸ�Ŀ¼���    */

    NFS_FREE(args.from.name);                                           /*  �ͷ�ԭ·�����ռ�Ŀ¼����    */

    NFS_FREE(args.to.name);                                             /*  �ͷ�Ŀ��·�����ռ�Ŀ¼����  */

    return  (ret);
}
/*********************************************************************************************************
** Function name:           nfs_truncate
** Descriptions:            �����ļ���С
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static int nfs_truncate(mount_point_t *point, const char *path, off_t len)
{
    sattr3          attr;
    nfs_fs         *nfs;

    nfs = point->ctx;                                                   /*  ��� NFS                    */
    if (nfs == NULL || nfs->client == NULL) {
        NFS_DEBUG("nfs_truncate() error: nfs == NULL!\n");
        seterrno(EINVAL);
        return  (-1);
    }

    /*
     * �趨����
     */
    attr.mode.set_it                      = FALSE;
    attr.uid.set_it                       = FALSE;
    attr.gid.set_it                       = FALSE;
    attr.size.set_it                      = TRUE;
    attr.size.set_size3_u.size            = len;                        /*  �ļ��ĳ���                  */
    attr.atime.set_it                     = DONT_CHANGE;
    attr.mtime.set_it                     = DONT_CHANGE;

    return  __nfspath_set_attr(nfs, path, &attr);                       /*  ��������                    */
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
