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
** File name:               vfs.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            �����ļ�ϵͳ
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-20
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-4-6
** Version:                 1.1.0
** Descriptions:
**
** ����֧�� select ��, ֧�� select ��Ҫ����Ϊ�û��ռ��е� GNU/pth ���Ҫ��!
**
** ��Ϊ��ĳ pthread �߳���� write �Ⱥ���, �п��ܻ�����������̱�����,
**
** �Ӷ����±�� pthread �ò������еĻ���.
**
** Ϊ�˽���������, GNU/pth ������������, ���߳� pthread_abc ���� write ����Ϊ��:
**
** 1) ���ļ�����Ϊ������ģʽ;
**
** 2) ����ļ�֮ǰ���ǹ����ڷ�����ģʽ, ��ô�͵��� write, �����Ϊ�յ� INT �źŶ�д��ʧ��,
**    ������������Ե��� write, ֱ��д��ɹ������.
**
** 3) ����ļ�֮ǰ����������ģʽ, ��ô select �ļ���д 0 ��;
**
** 4) ��� select �ļ���д, ��ô�͵��� write, �����Ϊ�յ� INT �źŶ�д��ʧ��,
**    ������������Ե��� write, ֱ��д��ɹ������.
**
** 5) ��� select �ļ�����д, ��ô�ȴ�һ�� PTH_EVENT_FD|PTH_UNTIL_FD_WRITEABLE|PTH_MODE_STATIC �¼�,
**    ���л��� pth_scheduler �߳�;
**
** 6) pth_scheduler �̻߳���û������ pthread �߳̿�����ʱ���� pth_sched_eventmanager ����;
**
** 7) pth_sched_eventmanager �����������еĵȴ��¼�, ���ջ� select ������Ҫ�ȴ����ļ��ɶ���д����
**    һ����С�ĵȴ�ʱ��;
**
** 8) pth_sched_eventmanager ������� select �Ľ�������ж�,
**    �Ծ����Ƿ��ѵȴ� PTH_EVENT_FD|PTH_UNTIL_FD_WRITEABLE|PTH_MODE_STATIC �¼����Ǹ� pthread_abc �߳�;
**
** 9) ����߳� pthread_abc ����¼�, ��ô�͵��� write, �����Ϊ�յ� INT �źŶ�д��ʧ��,
**    ������������Ե��� write, ֱ��д��ɹ������.
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
#include "kern/kvars.h"
#include "vfs/config.h"
#include "vfs/types.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/mount.h"
#include "vfs/fs.h"
#include "vfs/vfs.h"
#include <sys/time.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
/*********************************************************************************************************
** �ⲿ����
*********************************************************************************************************/
/*
 * ���ص������
 */
extern mutex_t          point_mgr_lock;

/*
 * ���ļ�ϵͳ���ص�
 */
extern mount_point_t   *rootfs_point;

/*
 * �豸������
 */
extern mutex_t          dev_mgr_lock;

/*
 * �ļ�ϵͳ������
 */
extern mutex_t          fs_mgr_lock;
/*********************************************************************************************************
** �����ļ���Ϣ
*********************************************************************************************************/
typedef struct {
    file_t              files[OPEN_MAX];                                /*  �ļ��ṹ��                  */
    char                cwd[PATH_MAX];                                  /*  ��ǰ����Ŀ¼                */
} vfs_info_t;

static vfs_info_t       infos[TASK_NR];
/*********************************************************************************************************
** ���ߺ���
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_path_add_mount_point
** Descriptions:            �� PATH ǰ������ص���
** input parameters:        path                ·��
** output parameters:       NONE
** Returned value:          ������ص����� PATH
*********************************************************************************************************/
const char *vfs_path_add_mount_point(const char *path)
{
    path--;
    while (*path != '/') {
        path--;
    }
    return path;
}
/*********************************************************************************************************
** Function name:           vfs_path_add_mount_point
** Descriptions:            ������ PATH
** input parameters:        path                ·��
**                          sprit_end           �Ƿ��� / ��β
** output parameters:       path                ���������·��
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_path_normalization(char path[PATH_MAX], int sprit_end)
{
    /*
     * �ı�״̬��! �����Ⱦ���ʦ�¾�!
     * http://blog.csdn.net/absurd/article/details/4249574
     */
    char   *p;
    char    ch;
    char    prev1_ch;
    char    prev2_ch;
    enum {
            BEGIN,
            WORKING,
    }       status;
    int     pos;

    pos    = 0;
    status = BEGIN;

    prev2_ch = '/';
    prev1_ch = '/';

    for (p = path; (ch = *p) != '\0'; p++) {                            /*  ���������ı�                */
        switch (status) {
        case BEGIN:                                                     /*  ��ʼ״̬                    */
            if (ch != '/') {                                            /*  ��������� / �ſ�ʼ, ����   */
                seterrno(EINVAL);
                return -1;
            }
            path[pos++] = ch;                                           /*  ��¼ / ��                   */
            status = WORKING;
            continue;
            break;

        case WORKING:
            switch (ch) {
            case '.':
                if (prev2_ch == '/' && prev1_ch == '.' && (p[1] == '\0' || p[1] == '/')) {
                    /*
                     *  /../ �� /..
                     */
                    /*
                     * pos, prev1_ch, prev2_ch
                     */
                    pos -= 3;
                    while (pos >= 0 && path[pos] != '/') {
                        pos--;
                    }
                    if (pos < 0) {
                        seterrno(ENOENT);
                        return -1;
                    }
                    if (pos == 0) {
                        prev2_ch = '/';
                        prev1_ch = '/';
                    } else {
                        prev2_ch = path[pos - 1];
                        prev1_ch = '/';
                    }
                    pos++;
                    continue;
                } else if (prev1_ch == '/' && (p[1] == '\0' || p[1] == '/')) {
                    /*
                     *  /./ �� /.
                     *  skip it
                     */
                    continue;
                } else {
                    path[pos++] = ch;
                }
                break;

            case '/':
                if (prev1_ch == '/') {
                    /*
                     *  //
                     *  skip it
                     */
                    continue;
                } else {
                    path[pos++] = ch;
                }
                break;

            default:
                path[pos++] = ch;
                break;
            }
        }

        prev2_ch = prev1_ch;
        prev1_ch = ch;
    }

    if (sprit_end) {
        if (pos >= 2 && path[pos - 1] != '/') {
            path[pos] = '/';
            path[pos + 1] = '\0';
        } else {
            path[pos] = '\0';
        }
    } else {
        if (pos > 2 && path[pos - 1] == '/') {
            path[pos - 1] = '\0';
        } else {
            path[pos] = '\0';
        }
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup
** Descriptions:            ���ҹ��ص�, PATH �����ǹ��ص�
** input parameters:        pathbuf             ·����ʱ������
**                          path                ·��
** output parameters:       ppath               ָ��ȥ�����ص���·��
** Returned value:          ���ص� OR NULL
*********************************************************************************************************/
static mount_point_t *vfs_mount_point_lookup(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t *point;
    char          *tmp;

    if (path == NULL) {                                                 /*  PATH �Ϸ��Լ��             */
        seterrno(EINVAL);
        return NULL;
    }

    if (path[0] == '/') {                                               /*  ����Ǿ���·��              */
        if (path[1] == '\0') {                                          /*  �����Ǹ�Ŀ¼                */
            seterrno(EINVAL);
            return NULL;
        }
        strlcpy(pathbuf, path, PATH_MAX);
    } else {                                                            /*  ��������·��              */
        /*
         * cwd Ҫ�� / �ſ�ͷ�ͽ�β
         */
        int32_t tid = gettid();
        snprintf(pathbuf, PATH_MAX, "%s%s", infos[tid].cwd, path);      /*  ��ǰ����뵱ǰ����Ŀ¼      */
    }

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {                   /*  ������ PATH                 */
        return NULL;
    }

    tmp = strchr(pathbuf + 1, '/');                                     /*  ���ҹ��ص������ / ��       */
    if (tmp == NULL) {                                                  /*  û�е�                      */
        seterrno(EINVAL);
        return NULL;
    }
    if (tmp[1] == '\0') {                                               /*  �����ǹ��ص�                */
        seterrno(EINVAL);
        return NULL;
    }

    *tmp  = '\0';                                                       /*  ��ʱȥ�� / ��               */
    point = mount_point_lookup(pathbuf);                                /*  ���ҹ��ص�                  */
    *tmp  = '/';                                                        /*  �ָ� / ��                   */

    *ppath = tmp;
    if (point == NULL) {
        seterrno(ENOENT);
    }
    return point;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup2
** Descriptions:            ���ҹ��ص�, PATH �����ǹ��ص�
** input parameters:        pathbuf             ·����ʱ������
**                          path                ·��
** output parameters:       ppath               ָ��ȥ�����ص���·��
** Returned value:          ���ص� OR NULL
*********************************************************************************************************/
static mount_point_t *vfs_mount_point_lookup2(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t  *point;
    char           *tmp;

    if (path == NULL) {                                                 /*  PATH �Ϸ��Լ��             */
        seterrno(EINVAL);
        return NULL;
    }

    if (path[0] == '/') {                                               /*  ����Ǿ���·��              */
        strlcpy(pathbuf, path, PATH_MAX);
    } else {                                                            /*  ��������·��              */
        /*
         * cwd Ҫ�� / �ſ�ͷ�ͽ�β
         */
        int32_t tid = gettid();
        snprintf(pathbuf, PATH_MAX, "%s%s", infos[tid].cwd, path);      /*  ��ǰ����뵱ǰ����Ŀ¼      */
    }

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {
        return NULL;
    }

    if (pathbuf[1] == '\0') {                                           /*  ����Ǹ�Ŀ¼                */
        point = mount_point_lookup(pathbuf);
        tmp = pathbuf;
    } else {
        tmp = strchr(pathbuf + 1, '/');                                 /*  ���ҹ��ص������ / ��       */
        if (tmp == NULL) {                                              /*  ����ǹ��ص�                */
            point = mount_point_lookup(pathbuf);
            strcat(pathbuf + 1, "/");
            tmp = strchr(pathbuf + 1, '/');
        } else {
            *tmp = '\0';                                                /*  ��ʱȥ�� / ��               */
            point = mount_point_lookup(pathbuf);                        /*  ���ҹ��ص�                  */
            *tmp = '/';                                                 /*  �ָ� / ��                   */
        }
    }

    *ppath = tmp;
    if (point == NULL) {
        seterrno(ENOENT);
    }
    return point;
}

/*********************************************************************************************************
** �ļ������ӿ�
*********************************************************************************************************/
#define vfs_file_begin(tid)                                                                               \
        mount_point_t  *point;                                                                            \
        file_t         *file;                                                                             \
        vfs_info_t     *info;                                                                             \
                                                                                                          \
        if (fd < 0 || fd >= OPEN_MAX) {                                 /*  �ļ��������Ϸ����ж�        */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        info = infos + tid;                                                                               \
        file = &info->files[fd];                                        /*  ����ļ��ṹ                */\
        if (file->flags == VFS_FILE_TYPE_FREE) {                        /*  ����ļ�δ��              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        if (!(file->flags & VFS_FILE_TYPE_FILE)) {                      /*  ����ļ��ṹ�����ļ�        */\
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point                                             /*  ��ù��ص�                  */

#define vfs_file_end(tid)
/*********************************************************************************************************
** Function name:           vfs_open
** Descriptions:            ���ļ�
** input parameters:        path                �ļ�·��
**                          oflag               ��־
**                          mode                ģʽ
** output parameters:       NULL
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_open(const char *path, int oflag, mode_t mode)
{
    mount_point_t  *point;
    vfs_info_t     *info;
    file_t         *file;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             fd;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);
                                                                        /*  ����һ�����е��ļ�������    */
    info = infos + gettid();
    for (fd = 0, file = &info->files[0]; fd < OPEN_MAX; fd++, file++) {
        if (file->flags == VFS_FILE_TYPE_FREE) {
            break;
        }
    }
    if (fd == OPEN_MAX) {                                               /*  û�ҵ�                      */
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return -1;
    }

    file->ctx    = NULL;                                                /*  ��ʼ���ļ��ṹ              */
    file->ctx1   = NULL;
    file->flags  = VFS_FILE_TYPE_FILE | (oflag + 1);                    /*  �ļ��ṹ�����ļ�            */
    file->point  = point;                                               /*  ��¼���ص�                  */

    if (point->fs->open == NULL) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);

    ret = point->fs->open(point, file, filepath, oflag, mode);         /*  ���ļ�                    */
    if (ret < 0) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
        return -1;
    }

    return fd;                                                          /*  �����ļ�������              */
}
/*********************************************************************************************************
** Function name:           vfs_close
** Descriptions:            �ر��ļ�
** input parameters:        fd                  �ļ�������
** output parameters:       NULL
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_close(int fd)
{
    int ret;

    vfs_file_begin(gettid());
    if (point->fs->close == NULL) {
        vfs_file_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->close(point, file);
    if (ret == 0) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
    }
    vfs_file_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           __vfs_close
** Descriptions:            �ر��ļ�
** input parameters:        tid                 ���� ID
**                          fd                  �ļ�������
** output parameters:       NULL
** Returned value:          0 OR -1
*********************************************************************************************************/
static int __vfs_close(pid_t tid, int fd)
{
    int ret;

    vfs_file_begin(tid);
    if (point->fs->close == NULL) {
        vfs_file_end(tid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->close(point, file);
    if (ret == 0) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
    }
    vfs_file_end(tid);
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_fcntl
** Descriptions:            �����ļ�
** input parameters:        fd                  �ļ�������
**                          cmd                 ����
**                          arg                 ����
** output parameters:       NULL
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fcntl(int fd, int cmd, int arg)
{
    int ret;

    vfs_file_begin(gettid());
    if (point->fs->fcntl == NULL) {
        vfs_file_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->fcntl(point, file, cmd, arg);
    vfs_file_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_fstat
** Descriptions:            ����ļ�״̬
** input parameters:        fd                  �ļ�������
**                          buf                 �ļ�״̬����
** output parameters:       buf                 �ļ�״̬
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fstat(int fd, struct stat *buf)
{
    int ret;

    if (buf == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    {
        vfs_file_begin(gettid());
        if (point->fs->fstat == NULL) {
            vfs_file_end(gettid());
            seterrno(ENOSYS);
            return -1;
        }
        seterrno(0);
        ret = point->fs->fstat(point, file, buf);
        vfs_file_end(gettid());
        return ret;
    }
}
/*********************************************************************************************************
** Function name:           vfs_fstat
** Descriptions:            �ж��ļ��ǲ����ն�
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_isatty(int fd)
{
    int ret;

    vfs_file_begin(gettid());
    if (point->fs->isatty == NULL) {
        vfs_file_end(gettid());
        seterrno(0);
        return 0;
    }
    seterrno(0);
    ret = point->fs->isatty(point, file);
    vfs_file_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_fsync
** Descriptions:            ͬ���ļ�
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fsync(int fd)
{
    int ret;

    vfs_file_begin(gettid());
    if (!(file->flags & FWRITE)) {
        vfs_file_end(gettid());
        seterrno(0);
        return 0;
    }
    if (point->fs->fsync == NULL) {
        vfs_file_end(gettid());
        seterrno(0);
        return 0;
    }
    seterrno(0);
    ret = point->fs->fsync(point, file);
    vfs_file_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_fdatasync
** Descriptions:            ͬ���ļ�
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fdatasync(int fd)
{
    int ret;

    vfs_file_begin(gettid());
    if (!(file->flags & FWRITE)) {
        vfs_file_end(gettid());
        seterrno(0);
        return 0;
    }
    if (point->fs->fdatasync == NULL) {
        vfs_file_end(gettid());
        seterrno(0);
        return 0;
    }
    seterrno(0);
    ret = point->fs->fdatasync(point, file);
    vfs_file_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_ftruncate
** Descriptions:            �޸��ļ�����
** input parameters:        fd                  �ļ�������
**                          len                 �³���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_ftruncate(int fd, off_t len)
{
    int ret;

    if (len < 0) {
        seterrno(EINVAL);
        return -1;
    }

    vfs_file_begin(gettid());
    if (!(file->flags & FWRITE)) {
        vfs_file_end(gettid());
        seterrno(EIO);
        return -1;
    }
    if (point->fs->ftruncate == NULL) {
        vfs_file_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->ftruncate(point, file, len);
    vfs_file_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_read
** Descriptions:            ���ļ�
** input parameters:        fd                  �ļ�������
**                          buf                 ���ݻ�����
**                          len                 �³���
** output parameters:       buf                 ����
** Returned value:          �ɹ���ȡ���ֽ��� OR -1
*********************************************************************************************************/
ssize_t vfs_read(int fd, void *buf, size_t len)
{
    ssize_t slen;

    if (buf == NULL || len < 0) {
        seterrno(EINVAL);
        return -1;
    }

    if (len == 0) {
        seterrno(0);
        return 0;
    }

    {
        vfs_file_begin(gettid());
        if (!(file->flags & FREAD)) {
            vfs_file_end(gettid());
            seterrno(EIO);
            return -1;
        }
        if (point->fs->read == NULL) {
            vfs_file_end(gettid());
            seterrno(ENOSYS);
            return -1;
        }
        seterrno(0);
        slen = point->fs->read(point, file, buf, len);
        vfs_file_end(gettid());
        return slen;
    }
}
/*********************************************************************************************************
** Function name:           vfs_write
** Descriptions:            д�ļ�
** input parameters:        fd                  �ļ�������
**                          buf                 ���ݻ�����
**                          len                 �³���
** output parameters:       NONE
** Returned value:          �ɹ�д����ֽ��� OR -1
*********************************************************************************************************/
ssize_t vfs_write(int fd, const void *buf, size_t len)
{
    ssize_t slen;

    if (buf == NULL || len < 0) {
        seterrno(EINVAL);
        return -1;
    }

    if (len == 0) {
        seterrno(0);
        return 0;
    }

    {
        vfs_file_begin(gettid());
        if (!(file->flags & FWRITE)) {
            vfs_file_end(gettid());
            seterrno(EIO);
            return -1;
        }
        if (point->fs->write == NULL) {
            vfs_file_end(gettid());
            seterrno(ENOSYS);
            return -1;
        }
        seterrno(0);
        slen = point->fs->write(point, file, buf, len);
        vfs_file_end(gettid());
        return slen;
    }
}
/*********************************************************************************************************
** Function name:           vfs_ioctl
** Descriptions:            �����ļ�
** input parameters:        fd                  �ļ�������
**                          cmd                 ����
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_ioctl(int fd, int cmd, void *arg)
{
    int ret;

    vfs_file_begin(gettid());
    if (point->fs->ioctl == NULL) {
        vfs_file_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->ioctl(point, file, cmd, arg);
    vfs_file_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_lseek
** Descriptions:            �����ļ���дλ��
** input parameters:        fd                  �ļ�������
**                          offset              ƫ��
**                          whence              ������λ��
** output parameters:       NONE
** Returned value:          �µĶ�дλ�� OR -1
*********************************************************************************************************/
off_t vfs_lseek(int fd, off_t offset, int whence)
{
    vfs_file_begin(gettid());
    if (point->fs->lseek == NULL) {
        vfs_file_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    offset = point->fs->lseek(point, file, offset, whence);
    vfs_file_end(gettid());
    return offset;
}
/*********************************************************************************************************
** Function name:           vfs_dup
** Descriptions:            �����ļ�������
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          �µ��ļ������� OR -1
*********************************************************************************************************/
int vfs_dup(int fd)
{
    file_t *temp;
    int     i;

    vfs_file_begin(gettid());

    atomic_inc(&point->ref);
                                                                        /*  ����һ�����е��ļ�������    */
    for (i = 0, temp = &info->files[0]; i < OPEN_MAX; i++, temp++) {
        if (temp->flags == VFS_FILE_TYPE_FREE) {
            break;
        }
    }
    if (i == OPEN_MAX) {                                                /*  û�ҵ�                      */
        atomic_dec(&point->ref);
        vfs_file_end(gettid());
        seterrno(EMFILE);
        return -1;
    }

    memcpy(temp, file, sizeof(file_t));

    temp->ctx1 = NULL;

    if (point->fs->dup != NULL) {
        int ret = point->fs->dup(point, file, temp);
        if (ret < 0) {
            atomic_dec(&point->ref);
            temp->flags = VFS_FILE_TYPE_FREE;
            vfs_file_end(gettid());
            return ret;
        }
    }

    seterrno(0);

    vfs_file_end(gettid());

    return i;
}
/*********************************************************************************************************
** Function name:           vfs_dup2
** Descriptions:            �����ļ���������ָ�����ļ�������
** input parameters:        fd                  �ļ�������
**                          to                  ָ�����ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_dup2(int fd, int to)
{
    int ret;
    file_t *temp;

    vfs_file_begin(gettid());

    atomic_inc(&point->ref);

    ret = vfs_close(to);
    if (ret < 0) {
        atomic_dec(&point->ref);
        vfs_file_end(gettid());
        return ret;
    }

    temp = &info->files[to];

    memcpy(temp, file, sizeof(file_t));

    temp->ctx1 = NULL;

    if (point->fs->dup != NULL) {
        int ret = point->fs->dup(point, file, temp);
        if (ret < 0) {
            atomic_dec(&point->ref);
            temp->flags = VFS_FILE_TYPE_FREE;
            vfs_file_end(gettid());
            return ret;
        }
    }

    seterrno(0);

    vfs_file_end(gettid());

    return 0;
}
/*********************************************************************************************************
** select ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_scan_file
** Descriptions:            ɨ���ļ�
** input parameters:        fd                  �ļ�������
**                          flags               ��־
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int vfs_scan_file(int fd, int flags)
{
    int ret;

    vfs_file_begin(gettid());
    if (point->fs->scan == NULL) {
        vfs_file_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->scan(point, file, flags);
    vfs_file_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_select_file
** Descriptions:            ����ǰ������뵽�ļ��ĵȴ��б�
** input parameters:        fd                  �ļ�������
**                          flags               ��־
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int vfs_select_file(int fd, int flags)
{
    int ret;

    vfs_file_begin(gettid());
    if (point->fs->select == NULL) {
        vfs_file_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->select(point, file, flags);
    vfs_file_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_unselect_file
** Descriptions:            ����ǰ������ļ��ĵȴ��б����Ƴ�
** input parameters:        fd                  �ļ�������
**                          flags               ��־
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int vfs_unselect_file(int fd, int flags)
{
    int ret;

    vfs_file_begin(gettid());
    if (point->fs->unselect == NULL) {
        vfs_file_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->unselect(point, file, flags);
    vfs_file_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_select_scan
** Descriptions:            ɨ���ļ���
** input parameters:        nfds                �ļ���������Χ
**                          readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** output parameters:       readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** Returned value:          �ļ����ܹ������˶��ٸ�λ
*********************************************************************************************************/
static int vfs_select_scan(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds)
{
    fd_set  rfds, wfds, efds;
    int     flags;
    int     i;
    int     nset;
    int     ret;

    nset = 0;

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    for (i = 0; i < nfds; i++) {
        flags = 0;
        if (readfds && FD_ISSET(i, readfds)) {
            flags |= VFS_FILE_READABLE;
        }
        if (writefds && FD_ISSET(i, writefds)) {
            flags |= VFS_FILE_WRITEABLE;
        }
        if (errorfds && FD_ISSET(i, errorfds)) {
            flags |= VFS_FILE_ERROR;
        }
        if (flags != 0) {
            ret = vfs_scan_file(i, flags);
            if (ret < 0) {
                return -1;
            } else if (ret > 0) {
                if (readfds && ret & VFS_FILE_READABLE) {
                    FD_SET(i, &rfds);
                    nset++;
                } else if (writefds && ret & VFS_FILE_WRITEABLE) {
                    FD_SET(i, &wfds);
                    nset++;
                } else if (errorfds && ret & VFS_FILE_ERROR) {
                    FD_SET(i, &efds);
                    nset++;
                } else {
                    continue;
                }
            } else {
                continue;
            }
        }
    }

    if (nset > 0) {
        if (readfds) {
            *readfds  = rfds;
        }
        if (writefds) {
            *writefds = wfds;
        }
        if (errorfds) {
            *errorfds = efds;
        }
        return nset;
    } else {
        return 0;
    }
}
/*********************************************************************************************************
** Function name:           vfs_select_unselect
** Descriptions:            ����ǰ������ļ����ĵȴ��б����Ƴ�
** input parameters:        nfds                �ļ���������Χ
**                          readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** output parameters:       NONE
** Returned value:          0
*********************************************************************************************************/
static int vfs_select_unselect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds)
{
    int flags;
    int i;

    for (i = 0; i < nfds; i++) {
        flags = 0;
        if (readfds && FD_ISSET(i, readfds)) {
            flags |= VFS_FILE_READABLE;
        }
        if (writefds && FD_ISSET(i, writefds)) {
            flags |= VFS_FILE_WRITEABLE;
        }
        if (errorfds && FD_ISSET(i, errorfds)) {
            flags |= VFS_FILE_ERROR;
        }
        if (flags != 0) {
            vfs_unselect_file(i, flags);
        }
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_select_select
** Descriptions:            ����ǰ������뵽�ļ����ĵȴ��б�
** input parameters:        nfds                �ļ���������Χ
**                          readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int vfs_select_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds)
{
    int flags;
    int i;
    int ret;

    for (i = 0; i < nfds; i++) {
        flags = 0;
        if (readfds && FD_ISSET(i, readfds)) {
            flags |= VFS_FILE_READABLE;
        }
        if (writefds && FD_ISSET(i, writefds)) {
            flags |= VFS_FILE_WRITEABLE;
        }
        if (errorfds && FD_ISSET(i, errorfds)) {
            flags |= VFS_FILE_ERROR;
        }
        if (flags != 0) {
            ret = vfs_select_file(i, flags);
            if (ret < 0) {
                vfs_select_unselect(i, readfds, writefds, errorfds);
                return -1;
            }
        }
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_select
** Descriptions:            ����ǰ������뵽�ļ����ĵȴ��б�
** input parameters:        nfds                �ļ���������Χ
**                          readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** output parameters:       readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** Returned value:          �ļ����ܹ������˶��ٸ�λ OR -1
*********************************************************************************************************/
int vfs_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
{
    int         resume_type;
    int         nset;
    int         ret;
    uint32_t    reg;

    if (nfds < 0 || nfds > FD_SETSIZE) {                                /*  ��� nfds                   */
        seterrno(EINVAL);
        if (readfds  != NULL) {
            FD_ZERO(readfds);
        }
        if (writefds != NULL) {
            FD_ZERO(writefds);
        }
        if (errorfds != NULL) {
            FD_ZERO(errorfds);
        }
        return -1;
    }

    if (timeout != NULL) {                                              /*  ��� timeout                */
        if (   timeout->tv_sec  < 0
            || timeout->tv_usec < 0
            || timeout->tv_usec >= 1000000 /* a full second */) {
            seterrno(EINVAL);
            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }
            if (writefds != NULL) {
                FD_ZERO(writefds);
            }
            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }
            return -1;
        }

        if (timeout->tv_sec > 31 * 24 * 60 * 60) {                      /*  ���� timeout                */
            timeout->tv_sec = 31 * 24 * 60 * 60;
        }
    }

    if (nfds == 0) {                                                    /*  nfds Ϊ 0 �����            */
        if (timeout != NULL) {                                          /*  timeout ��Ϊ 0, ��ֻ��ʱ    */
            if (timeout->tv_sec != 0 || timeout->tv_usec != 0) {
                usleep(timeout->tv_sec * 1000000 + timeout->tv_usec);   /*  ��ʱ                        */
            }
            seterrno(0);
            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }
            if (writefds != NULL) {
                FD_ZERO(writefds);
            }
            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }
            return 0;
        } else {                                                        /*  ��������                    */
            seterrno(EINVAL);
            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }
            if (writefds != NULL) {
                FD_ZERO(writefds);
            }
            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }
            return -1;
        }
    }

    reg  = interrupt_disable();
    nset = vfs_select_scan(nfds, readfds, writefds, errorfds);          /*  ɨ���ļ���                  */
    if (nset > 0) {                                                     /*  ���ļ����ļ�������λ��      */
        interrupt_resume(reg);
        seterrno(0);
        return nset;
    } else if (nset < 0) {                                              /*  ������                      */
        interrupt_resume(reg);
        if (readfds  != NULL) {
            FD_ZERO(readfds);
        }
        if (writefds != NULL) {
            FD_ZERO(writefds);
        }
        if (errorfds != NULL) {
            FD_ZERO(errorfds);
        }
        return -1;
    }

    /*
     * ���ļ����ļ�������λ
     */
                                                                        /*  �ϲ㲻������                */
    if (timeout != NULL && (timeout->tv_sec == 0 && timeout->tv_usec == 0)) {
        interrupt_resume(reg);
        seterrno(0);
        if (readfds  != NULL) {
            FD_ZERO(readfds);
        }
        if (writefds != NULL) {
            FD_ZERO(writefds);
        }
        if (errorfds != NULL) {
            FD_ZERO(errorfds);
        }
        return 0;
    }

    /*
     * ����ǰ������뵽�ļ����ĵȴ��б�
     */
    ret = vfs_select_select(nfds, readfds, writefds, errorfds);
    if (ret < 0) {
        interrupt_resume(reg);
        if (readfds  != NULL) {
            FD_ZERO(readfds);
        }
        if (writefds != NULL) {
            FD_ZERO(writefds);
        }
        if (errorfds != NULL) {
            FD_ZERO(errorfds);
        }
        return -1;
    }

    if (timeout == NULL) {                                              /*  �ϲ�Ҫ��ȴ�                */
        current->delay   = 0;
        current->status  = TASK_SUSPEND;
    } else {                                                            /*  �ϲ�Ҫ������                */
        current->delay   = timeout->tv_sec  * TICK_PER_SECOND +
                           timeout->tv_usec * TICK_PER_SECOND / 1000000;
        current->status  = TASK_SLEEPING;
    }
    current->resume_type = TASK_RESUME_UNKNOW;

    schedule();                                                         /*  �ͷ� CPU ʹ��               */

    /*
     * ����ǰ������ļ����ĵȴ��б����Ƴ�
     */
    vfs_select_unselect(nfds, readfds, writefds, errorfds);

    current->delay       = 0;
    resume_type          = current->resume_type;
    current->resume_type = TASK_RESUME_UNKNOW;

    if (resume_type & TASK_RESUME_SELECT_EVENT) {                       /*  �� select �¼��ָ�          */
        nset = vfs_select_scan(nfds, readfds, writefds, errorfds);      /*  ɨ���ļ���                  */
        interrupt_resume(reg);
        if (nset > 0) {                                                 /*  ���ļ����ļ�������λ��      */
            seterrno(0);
            return nset;
        }

        if (nset == 0) {                                                /*  ���ļ����ļ�������λ��      */
            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }
            if (writefds != NULL) {
                FD_ZERO(writefds);
            }
            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }
            seterrno(0);
            return 0;
        } else {
            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }
            if (writefds != NULL) {
                FD_ZERO(writefds);
            }
            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }
            return -1;
        }
    } else {                                                            /*  �� select �¼��ָ�          */
        interrupt_resume(reg);
        if (resume_type & TASK_RESUME_INTERRUPT) {
            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }
            if (writefds != NULL) {
                FD_ZERO(writefds);
            }
            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }
            seterrno(EINTR);
            return -1;
        } else {
            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }
            if (writefds != NULL) {
                FD_ZERO(writefds);
            }
            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }
            seterrno(EAGAIN);
            return 0;
        }
    }
}
/*********************************************************************************************************
** Ŀ¼�����ӿ�
*********************************************************************************************************/
#define vfs_dir_begin(tid)                                                                                \
        mount_point_t *point;                                                                             \
        file_t *file;                                                                                     \
        vfs_info_t *info;                                                                                 \
                                                                                                          \
        if (fd < 1 || fd >= OPEN_MAX) {                                 /*  �ļ��������Ϸ����ж�        */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        info = infos + tid;                                                                               \
        file = &info->files[fd];                                        /*  ����ļ��ṹ                */\
        if (file->flags == VFS_FILE_TYPE_FREE) {                        /*  ���Ŀ¼δ��              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        if (!(file->flags & VFS_FILE_TYPE_DIR)) {                       /*  ����ļ��ṹ����Ŀ¼        */\
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point                                             /*  ��ù��ص�                  */

#define vfs_dir_end(tid)
/*********************************************************************************************************
** Function name:           vfs_opendir
** Descriptions:            ��Ŀ¼
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          Ŀ¼ָ��
*********************************************************************************************************/
DIR *vfs_opendir(const char *path)
{
    mount_point_t  *point;
    vfs_info_t     *info;
    file_t         *file;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             fd;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return (DIR *)0;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);
                                                                        /*  ����һ�����е��ļ�������    */
    info = infos + gettid();
    for (fd = 1, file = &info->files[1]; fd < OPEN_MAX; fd++, file++) {
        if (file->flags == 0) {
            break;
        }
    }
    if (fd == OPEN_MAX) {                                               /*  û�ҵ�                      */
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return (DIR *)0;
    }

    file->ctx    = NULL;                                                /*  ��ʼ���ļ��ṹ              */
    file->ctx1   = NULL;
    file->flags  = VFS_FILE_TYPE_DIR;                                   /*  �ļ��ṹ�����ļ�            */
    file->point  = point;                                               /*  ��¼���ص�                  */

    if (point->fs->opendir == NULL) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
        seterrno(ENOSYS);
        return (DIR *)0;
    }

    seterrno(0);
    ret = point->fs->opendir(point, file, filepath);                    /*  ���ļ�                    */
    if (ret < 0) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
        return (DIR *)0;
    }

    return (DIR *)fd;                                                   /*  �����ļ�������              */
}
/*********************************************************************************************************
** Function name:           vfs_closedir
** Descriptions:            �ر�Ŀ¼
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_closedir(DIR *dir)
{
    int fd = (int)dir;
    int ret;

    vfs_dir_begin(gettid());
    if (point->fs->closedir == NULL) {
        vfs_dir_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->closedir(point, file);
    if (ret == 0) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
    }
    vfs_dir_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           __vfs_closedir
** Descriptions:            �ر�Ŀ¼
** input parameters:        tid                 ���� ID
**                          dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int __vfs_closedir(pid_t tid, DIR *dir)
{
    int fd = (int)dir;
    int ret;

    vfs_dir_begin(tid);
    if (point->fs->closedir == NULL) {
        vfs_dir_end(tid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->closedir(point, file);
    if (ret == 0) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
    }
    vfs_dir_end(tid);
    return ret;
}
/*********************************************************************************************************
** Function name:           __vfs_readdir
** Descriptions:            ��Ŀ¼��
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       entry               Ŀ¼��
** Returned value:          0 OR -1
*********************************************************************************************************/
static int __vfs_readdir(DIR *dir, struct dirent **entry)
{
    int fd = (int)dir;
    int ret;

    vfs_dir_begin(gettid());
    if (point->fs->readdir == NULL) {
        vfs_dir_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    *entry = point->fs->readdir(point, file);
    if (*entry != NULL) {
        ret = 0;
    } else {
        ret = -1;
    }
    vfs_dir_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_readdir
** Descriptions:            ��Ŀ¼��
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          Ŀ¼��
*********************************************************************************************************/
struct dirent *vfs_readdir(DIR *dir)
{
    struct dirent *entry;

    if (__vfs_readdir(dir, &entry) < 0) {
        return NULL;
    } else {
        return entry;
    }
}
/*********************************************************************************************************
** Function name:           vfs_rewinddir
** Descriptions:            ����Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rewinddir(DIR *dir)
{
    int fd = (int)dir;
    int ret;

    vfs_dir_begin(gettid());
    if (point->fs->rewinddir == NULL) {
        vfs_dir_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->rewinddir(point, file);
    vfs_dir_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_rewinddir
** Descriptions:            ����Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
**                          loc                 �µĶ���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_seekdir(DIR *dir, long loc)
{
    int fd = (int)dir;
    int ret;

    vfs_dir_begin(gettid());
    if (point->fs->seekdir == NULL) {
        vfs_dir_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->seekdir(point, file, loc);
    vfs_dir_end(gettid());
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_telldir
** Descriptions:            ���Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          ��ǰ����
*********************************************************************************************************/
long vfs_telldir(DIR *dir)
{
    int  fd = (int)dir;
    long loc;

    vfs_dir_begin(gettid());
    if (point->fs->telldir == NULL) {
        vfs_dir_end(gettid());
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    loc = point->fs->telldir(point, file);
    vfs_dir_end(gettid());
    return loc;
}
/*********************************************************************************************************
** �ļ�ϵͳ�����ӿ�
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_link
** Descriptions:            ���ļ�����һ������
** input parameters:        path1               �ļ� PATH
**                          path2               ���ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_link(const char *path1, const char *path2)
{
    mount_point_t  *point1;
    char            pathbuf1[PATH_MAX];
    char           *filepath1;
    mount_point_t  *point2;
    char            pathbuf2[PATH_MAX];
    char           *filepath2;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point1 = vfs_mount_point_lookup(pathbuf1, &filepath1, path1);       /*  ���ҹ��ص�                  */
    if (point1 == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    point2 = vfs_mount_point_lookup(pathbuf2, &filepath2, path2);       /*  ���ҹ��ص�                  */
    if (point2 == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    if (point2 != point1) {                                             /*  �������ص����Ҫ��ͬ        */
        mutex_unlock(&point_mgr_lock);
        seterrno(EXDEV);
        return -1;
    }

    atomic_inc(&point1->ref);

    mutex_unlock(&point_mgr_lock);

    if (point1->fs->link == NULL) {
        atomic_dec(&point1->ref);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point1->fs->link(point1, filepath1, filepath2);

    atomic_dec(&point1->ref);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_rename
** Descriptions:            ������(Ҳ���ƶ�)�ļ�
** input parameters:        old                 Դ�ļ� PATH
**                          _new                ���ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rename(const char *old, const char *_new)
{
    mount_point_t  *point1;
    char            pathbuf1[PATH_MAX];
    char           *filepath1;
    mount_point_t  *point2;
    char            pathbuf2[PATH_MAX];
    char           *filepath2;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point1 = vfs_mount_point_lookup(pathbuf1, &filepath1, old);         /*  ���ҹ��ص�                  */
    if (point1 == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    point2 = vfs_mount_point_lookup(pathbuf2, &filepath2, _new);        /*  ���ҹ��ص�                  */
    if (point2 == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    if (point2 != point1) {                                             /*  �������ص����Ҫ��ͬ        */
        mutex_unlock(&point_mgr_lock);
        seterrno(EXDEV);
        return -1;
    }

    atomic_inc(&point1->ref);

    mutex_unlock(&point_mgr_lock);

    if (point1->fs->rename == NULL) {
        atomic_dec(&point1->ref);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point1->fs->rename(point1, filepath1, filepath2);

    atomic_dec(&point1->ref);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_stat
** Descriptions:            ����ļ�״̬
** input parameters:        path                �ļ� PATH
**                          buf                 �ļ�״̬����
** output parameters:       buf                 �ļ�״̬
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_stat(const char *path, struct stat *buf)
{
    mount_point_t  *point;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             ret;

    if (buf == NULL) {
        return -1;
    }

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);

    if (point->fs->stat == NULL) {
        atomic_dec(&point->ref);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->stat(point, filepath, buf);

    atomic_dec(&point->ref);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_unlink
** Descriptions:            ɾ���ļ�
** input parameters:        path                �ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_unlink(const char *path)
{
    mount_point_t  *point;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);

    if (point->fs->unlink == NULL) {
        atomic_dec(&point->ref);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->unlink(point, filepath);

    atomic_dec(&point->ref);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_mkdir
** Descriptions:            ����Ŀ¼
** input parameters:        path                Ŀ¼ PATH
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mkdir(const char *path, mode_t mode)
{
    mount_point_t  *point;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);

    if (point->fs->mkdir == NULL) {
        atomic_dec(&point->ref);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->mkdir(point, filepath, mode);

    atomic_dec(&point->ref);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_rmdir
** Descriptions:            ɾ��Ŀ¼
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rmdir(const char *path)
{
    mount_point_t  *point;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);

    if (point->fs->rmdir == NULL) {
        atomic_dec(&point->ref);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->rmdir(point, filepath);

    atomic_dec(&point->ref);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_access
** Descriptions:            �ж��Ƿ�ɷ���
** input parameters:        path                �ļ� PATH
**                          amode               ����ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_access(const char *path, int amode)
{
    mount_point_t  *point;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);

    if (point->fs->access == NULL) {
        atomic_dec(&point->ref);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->access(point, filepath, amode);

    atomic_dec(&point->ref);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_truncate
** Descriptions:            �޸��ļ�����
** input parameters:        path                �ļ� PATH
**                          amode               �µ��ļ�����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_truncate(const char *path, off_t len)
{
    mount_point_t  *point;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);

    if (point->fs->truncate == NULL) {
        atomic_dec(&point->ref);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->truncate(point, filepath, len);

    atomic_dec(&point->ref);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_truncate
** Descriptions:            ͬ��
** input parameters:        path                �ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_sync(const char *path)
{
    mount_point_t  *point;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);

    if (point->fs->sync == NULL) {
        atomic_dec(&point->ref);
        seterrno(ENOSYS);
        return 0;
    }

    seterrno(0);
    ret = point->fs->sync(point);

    atomic_dec(&point->ref);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_mkfs
** Descriptions:            ��ʽ���ļ�ϵͳ
** input parameters:        path                Ŀ¼ PATH
**                          param               ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mkfs(const char *path, const char *param)
{
    mount_point_t  *point;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    if (point->fs->mkfs == NULL) {
        mutex_unlock(&point_mgr_lock);
        seterrno(ENOSYS);
        return -1;
    }

    if (atomic_read(&point->ref) != 0) {
        mutex_unlock(&point_mgr_lock);
        seterrno(EBUSY);
        return -1;
    }

    seterrno(0);
    ret = point->fs->mkfs(point, param);                                /*  ��ʽ���ļ�ϵͳ              */

    mutex_unlock(&point_mgr_lock);

    return ret;
}
/*********************************************************************************************************
** ���ص�ӿ�
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_mount
** Descriptions:            �����ļ�ϵͳ
** input parameters:        point_name          ���ص���
**                          dev_name            �豸��
**                          fs_name             �ļ�ϵͳ��
**                          param               ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mount(const char *point_name, const char *dev_name, const char *fs_name, const char *param)
{
    mount_point_t  *point;
    file_system_t  *fs;
    device_t       *dev;
    int             ret;

    if (fs_name == NULL) {
        return -1;
    }

    mutex_lock(&point_mgr_lock, 0);
    point = mount_point_lookup(point_name);                             /*  ���ҹ��ص�                  */
    if (point == NULL) {                                                /*  û�ҵ�                      */

        mutex_lock(&fs_mgr_lock, 0);

        fs = file_system_lookup(fs_name);                               /*  �����ļ�ϵͳ                */
        if (fs != NULL) {

            atomic_inc(&fs->ref);

            mutex_unlock(&fs_mgr_lock);

            mutex_lock(&dev_mgr_lock, 0);
            dev = device_lookup(dev_name);                              /*  �����豸                    */
            if (dev != NULL) {
                if (atomic_inc_return(&dev->ref) != 1) {
                    atomic_dec(&dev->ref);
                    atomic_dec(&fs->ref);
                    mutex_unlock(&dev_mgr_lock);
                    mutex_unlock(&point_mgr_lock);
                    return -1;
                }
            }
            mutex_unlock(&dev_mgr_lock);

            if (point_name != NULL) {                                   /*  ��ָ�����ص���              */
                point = kmalloc(sizeof(mount_point_t), GFP_KERNEL);     /*  ������ص�                  */
                if (point != NULL) {
                    if (point_name[0] == '/') {                         /*  ��֤���ص��� / �ſ�ʼ       */
                        strlcpy(point->name, point_name, sizeof(point->name));
                    } else {
                        snprintf(point->name, sizeof(point->name), "/%s", point_name);
                    }

                    if (point->name[1] != '\0') {                       /*  ������Ǹ��ļ�ϵͳ          */
                        if (strchr(point->name + 1, '/') != NULL) {     /*  ��֤���ص㲻���ٳ��� / ��   */
                                                                        /*  ��Ϊ�Ҳ�֪�� / �Ż��м���   */
                            mutex_unlock(&point_mgr_lock);
                            if (dev != NULL) {
                                atomic_dec(&dev->ref);
                            }
                            atomic_dec(&fs->ref);
                            kfree(point);
                            return -1;                                  /*  ���Ե�������������          */
                        }
                    } else {
                        rootfs_point = point;
                    }

                    point->fs  = fs;
                    point->dev = dev;
                    point->ctx = NULL;
                    atomic_set(&point->ref, 0);

                    ret = fs->mount(point, dev, dev_name, param);       /*  ����                        */
                    if (ret < 0) {
                        if (rootfs_point == point) {
                            rootfs_point =  NULL;
                        }
                        mutex_unlock(&point_mgr_lock);
                        if (dev != NULL) {
                            atomic_dec(&dev->ref);
                        }
                        atomic_dec(&fs->ref);
                        kfree(point);
                        return -1;
                    } else {
                        mount_point_install(point);                     /*  ��װ���ص�                  */
                        mutex_unlock(&point_mgr_lock);
                        return 0;
                    }
                }
            } else {                                                    /*  ��ָ�����ص���, ����ʱ      */
                                                                        /*  �÷������������ص�          */
                ret = fs->mount(NULL, dev, dev_name, param);            /*  ����                        */
                if (ret < 0) {
                    mutex_unlock(&point_mgr_lock);
                    if (dev != NULL) {
                        atomic_dec(&dev->ref);
                    }
                    atomic_dec(&fs->ref);
                    return -1;
                } else {
                    mutex_unlock(&point_mgr_lock);
                    atomic_dec(&fs->ref);                               /*  �������ص�������ļ�ϵͳ����*/
                    return 0;                                           /*  ���������ļ�ϵͳ����      */
                }
            }
        }

        mutex_unlock(&fs_mgr_lock);
    }

    mutex_unlock(&point_mgr_lock);
    return -1;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_create
** Descriptions:            �������ص�
** input parameters:        point_name          ���ص���
**                          fs                  �ļ�ϵͳ
**                          dev                 �豸
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mount_point_create(const char *point_name, file_system_t *fs, device_t *dev)
{
    mount_point_t  *point;

    if (point_name == NULL || fs == NULL) {
        return -1;
    }

    point = kmalloc(sizeof(mount_point_t), GFP_KERNEL);                 /*  ������ص�                  */
    if (point != NULL) {
        if (point_name[0] == '/') {                                     /*  ��֤���ص��� / �ſ�ʼ       */
            strlcpy(point->name, point_name, sizeof(point->name));
        } else {
            snprintf(point->name, sizeof(point->name), "/%s", point_name);
        }

        if (point->name[1] != '\0') {                                   /*  ������Ǹ��ļ�ϵͳ          */
            if (strchr(point->name + 1, '/') != NULL) {                 /*  ��֤���ص㲻���ٳ��� / ��   */
                kfree(point);
                return -1;                                              /*  ���Ե�������������          */
            }
        } else {
            kfree(point);
            return -1;
        }

        atomic_inc(&fs->ref);                                           /*  �����ļ�ϵͳ����            */
        if (dev != NULL) {
            atomic_inc(&dev->ref);                                      /*  �����豸����                */
        }

        point->fs  = fs;
        point->dev = dev;
        point->ctx = NULL;
        atomic_set(&point->ref, 0);

        mount_point_install(point);                                     /*  ��װ���ص�                  */

        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           vfs_unmount
** Descriptions:            ȡ�������ļ�ϵͳ
** input parameters:        path                Ŀ¼ PATH
**                          param               ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_unmount(const char *path, const char *param)
{
    mount_point_t  *point;
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    vfs_sync(path);

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    if (point->fs->unmount == NULL) {
        mutex_unlock(&point_mgr_lock);
        seterrno(ENOSYS);
        return -1;
    }

    if (atomic_read(&point->ref) != 0) {
        mutex_unlock(&point_mgr_lock);
        seterrno(EBUSY);
        return -1;
    }

    seterrno(0);
    ret = point->fs->unmount(point, param);                             /*  ȡ�������ļ�ϵͳ            */
    if (ret == 0) {
        if (point->dev != NULL) {
            atomic_dec(&point->dev->ref);
        }
        atomic_dec(&point->fs->ref);
        mount_point_remove(point);
    }

    mutex_unlock(&point_mgr_lock);

    return ret;
}
/*********************************************************************************************************
** ����ӿ�
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_task_init
** Descriptions:            ��ʼ��������ļ���Ϣ
** input parameters:        tid                 ���� ID
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_task_init(pid_t tid)
{
    vfs_info_t *info;

    if (tid < 0 || tid >= TASK_NR) {
        return -1;
    }

    info = infos + tid;

    strcpy(info->cwd, "/");

    memset(info->files, 0, sizeof(info->files));

    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_task_cleanup
** Descriptions:            ����������ļ���Ϣ
** input parameters:        tid                 ���� ID
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_task_cleanup(pid_t tid)
{
    vfs_info_t *info;
    file_t     *file;
    int         fd;

    if (tid < 0 || tid >= TASK_NR) {
        return -1;
    }

    info = infos + tid;

    for (fd = 0, file = &info->files[0]; fd < OPEN_MAX; fd++, file++) {
        if (file->flags & VFS_FILE_TYPE_FILE) {
            __vfs_close(tid, fd);
        } else if (file->flags & VFS_FILE_TYPE_DIR) {
            __vfs_closedir(tid, (DIR *)fd);
        }
    }
    return 0;
}
/*********************************************************************************************************
** �����ӿ�
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_chdir
** Descriptions:            �ı䵱ǰ����Ŀ¼
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_chdir(const char *path)
{
    char    pathbuf[PATH_MAX];
    int     ret;
    int32_t tid;

    if (path == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    tid = gettid();

    if (path[0] == '/') {                                               /*  ����Ǿ���·��              */
        strlcpy(pathbuf, path, sizeof(pathbuf));
    } else {                                                            /*  ��������·��              */
        snprintf(pathbuf, sizeof(pathbuf), "%s%s", infos[tid].cwd, path);
    }

    ret = vfs_path_normalization(pathbuf, TRUE);
    if (ret == 0) {
        DIR *dir = vfs_opendir(pathbuf);
        if (dir != NULL) {
            strlcpy(infos[tid].cwd, pathbuf, sizeof(infos[tid].cwd));
            vfs_closedir(dir);
        } else {
            ret = -1;
        }
    }

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_getcwd
** Descriptions:            ��õ�ǰ����Ŀ¼
** input parameters:        size                buf ��С
**                          buf                 ��ǰ����Ŀ¼����
** output parameters:       buf                 ��ǰ����Ŀ¼
** Returned value:          ��ǰ����Ŀ¼
*********************************************************************************************************/
char *vfs_getcwd(char *buf, size_t size)
{
    int32_t tid = gettid();

    if (buf != NULL) {
        strlcpy(buf, infos[tid].cwd, size);
    }
    return infos[tid].cwd;
}
/*********************************************************************************************************
** Function name:           vfs_get_file
** Descriptions:            �����ļ�����������ļ��ṹ
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          �ļ��ṹ
*********************************************************************************************************/
file_t *vfs_get_file(int fd)
{
    file_t *file;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */                                                              \
        return NULL;
    }

    file = &infos[gettid()].files[fd];
    if (file->flags == VFS_FILE_TYPE_FREE) {                            /*  ����ļ�δ��              */
        return NULL;
    }

    if (!(file->flags & VFS_FILE_TYPE_FILE)) {                          /*  ����ļ��ṹ�����ļ�        */
        return NULL;
    }

    return file;
}
/*********************************************************************************************************
** Function name:           vfs_init
** Descriptions:            ��ʼ�������ļ�ϵͳ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_init(void)
{
    driver_manager_init();

    device_manager_init();

    file_system_manager_init();

    mount_point_manager_init();

    extern file_system_t rootfs;
    file_system_install(&rootfs);

    extern file_system_t devfs;
    file_system_install(&devfs);

    extern file_system_t fatfs;
    file_system_install(&fatfs);

    extern file_system_t yaffs;
    file_system_install(&yaffs);

    vfs_mount("/",    NULL, "rootfs", NULL);

    vfs_mount("/dev", NULL, "devfs", NULL);

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
