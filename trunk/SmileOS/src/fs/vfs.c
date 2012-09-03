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
** Modified by:             JiaoJinXing
** Modified date:           2012-4-12
** Version:                 1.2.0
** Descriptions:            ȥ������Ҫ���ļ���, ��Ϊ�ļ��ṹ�������˽������, �²���Ҫ��ע�ز����ͻ���!
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-7-11
** Version:                 1.3.0
** Descriptions:            �Ľ����ļ��ṹ��ʹ��, �߱�ʵ�� dup, dup2 �� fork ������.
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/ipc.h"
#include "vfs/config.h"
#include "vfs/types.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/mount.h"
#include "vfs/fs.h"
#include "vfs/file.h"
#include "vfs/vfs.h"
#include <sys/time.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/*
 * �����ļ���Ϣ
 */
typedef struct {
    file_t             *files[OPEN_MAX];                                /*  �ļ��ṹ��                  */
    char                cwd[PATH_MAX];                                  /*  ��ǰ����Ŀ¼                */
} vfs_info_t;

static vfs_info_t infos[TASK_NR];

/*
 * �� PATH ǰ������ص���
 */
const char *vfs_path_add_mount_point(const char *path)
{
    path--;
    while (*path != '/') {
        path--;
    }
    return path;
}

/*
 * ������ PATH
 */
int vfs_path_normalization(char path[PATH_MAX], int sprit_end)
{
    /*
     * �ı�״̬��! �����Ⱦ���ʦ�¾�!
     * http://blog.csdn.net/absurd/article/details/4249574
     */
    char *p;
    char ch;
    char prev1_ch;
    char prev2_ch;
    enum {
        BEGIN,
        WORKING,
    } state;
    int pos;

    pos   = 0;
    state = BEGIN;

    prev2_ch = '/';
    prev1_ch = '/';

    for (p = path; (ch = *p) != '\0'; p++) {                            /*  ���������ı�                */
        switch (state) {
        case BEGIN:                                                     /*  ��ʼ״̬                    */
            if (ch != '/') {                                            /*  ��������� / �ſ�ʼ, ����   */
                seterrno(EINVAL);
                return -1;
            }
            path[pos++] = ch;                                           /*  ��¼ / ��                   */
            state = WORKING;
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

/*
 * ���ҹ��ص�, PATH �����ǹ��ص�
 */
static mount_point_t *vfs_mount_point_lookup(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t *point;
    char *tmp;

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

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {
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

    *tmp = '\0';                                                        /*  ��ʱȥ�� / ��               */
    point = mount_point_lookup(pathbuf);                                /*  ���ҹ��ص�                  */
    *tmp = '/';                                                         /*  �ָ� / ��                   */

    *ppath = tmp;
    if (point == NULL) {
        seterrno(ENOENT);
    }
    return point;
}

/*
 * ���ҹ��ص�, PATH �����ǹ��ص�
 */
static mount_point_t *vfs_mount_point_lookup2(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t *point;
    char *tmp;
    static char rootdir[] = "/";

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
            tmp = rootdir;
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

/*
 * �ļ�����
 */
#define VFS_FILE_TYPE_FREE      (0)

#if 0
#define _FOPEN      (-1)    /* from sys/file.h, kernel use only */
#define _FREAD      0x0001  /* read enabled */
#define _FWRITE     0x0002  /* write enabled */
#define _FAPPEND    0x0008  /* append (writes guaranteed at the end) */
#define _FMARK      0x0010  /* internal; mark during gc() */
#define _FDEFER     0x0020  /* internal; defer for next gc pass */
#define _FASYNC     0x0040  /* signal pgrp when data ready */
#define _FSHLOCK    0x0080  /* BSD flock() shared lock present */
#define _FEXLOCK    0x0100  /* BSD flock() exclusive lock present */
#define _FCREAT     0x0200  /* open with file create */
#define _FTRUNC     0x0400  /* open with truncation */
#define _FEXCL      0x0800  /* error on open if file exists */
#define _FNBIO      0x1000  /* non blocking I/O (sys5 style) */
#define _FSYNC      0x2000  /* do all writes synchronously */
#define _FNONBLOCK  0x4000  /* non blocking I/O (POSIX style) */
#define _FNDELAY    _FNONBLOCK  /* non blocking I/O (4.2 style) */
#define _FNOCTTY    0x8000  /* don't assign a ctty on this open */

#define O_RDONLY    0       /* +1 == FREAD */
#define O_WRONLY    1       /* +1 == FWRITE */
#define O_RDWR      2       /* +1 == FREAD|FWRITE */
#endif

#define VFS_FILE_TYPE_FILE      (1 << 16)
#define VFS_FILE_TYPE_DIR       (1 << 17)

/*********************************************************************************************************
 *                                          �ļ������ӿ�
 */

#define vfs_file_begin(tid)                                                                               \
        mount_point_t *point;                                                                             \
        file_t *file;                                                                                     \
        vfs_info_t *info;                                                                                 \
                                                                                                          \
        if (fd < 0 || fd >= OPEN_MAX) {                                 /*  �ļ��������Ϸ����ж�        */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        info = infos + tid;                                                                               \
        file = info->files[fd];                                         /*  ����ļ��ṹ                */\
        if (file == NULL || file->ref == 0) {                           /*  ����ļ�δ��              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        if (!(file->flags & VFS_FILE_TYPE_FILE)) {                      /*  ����ļ��ṹ�����ļ�        */\
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point                                             /*  ��ù��ص�                  */

#define vfs_file_end(tid)

/*
 * ���ļ�
 */
int vfs_open(const char *path, int oflag, mode_t mode)
{
    mount_point_t *point;
    vfs_info_t *info;
    file_t *file;
    char pathbuf[PATH_MAX];
    char *filepath;
    int fd;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }
                                                                        /*  ����һ�����е��ļ�������    */
    info = infos + gettid();
    for (fd = 0, file = info->files[0]; fd < OPEN_MAX; fd++, file = info->files[fd]) {
        if (file == NULL) {
            break;
        }
    }
    if (fd == OPEN_MAX) {                                               /*  û�ҵ�                      */
        seterrno(EMFILE);
        return -1;
    }

    file = file_alloc();                                                /*  ����һ�����е��ļ��ṹ      */
    if (file == NULL) {
        seterrno(EMFILE);
        return -1;
    }

    file->ctx    = NULL;                                                /*  ��ʼ���ļ��ṹ              */
    file->ctx1   = NULL;
    file->flags  = VFS_FILE_TYPE_FILE;                                  /*  �ļ��ṹ�����ļ�            */
    file->flags |= (oflag & O_ACCMODE) + 1;                             /*  ��¼����ģʽ                */
    file->flags |= oflag & O_APPEND;                                    /*  ��¼׷��ģʽ                */
    file->point  = point;                                               /*  ��¼���ص�                  */

    if (point->fs->open == NULL) {
        seterrno(ENOSYS);
        file_free(file);
        return -1;
    }

    seterrno(0);
    ret = point->fs->open(point, file, filepath, oflag, mode);          /*  ���ļ�                    */
    if (ret < 0) {
        file_free(file);
        return -1;
    }

    info->files[fd] = file;

    return fd;                                                          /*  �����ļ�������              */
}

/*
 * �ر��ļ�
 */
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
    if (file->ref == 1) {
        ret = point->fs->close(point, file);
    } else {
        ret = 0;
    }
    if (ret == 0) {
        info->files[fd] = NULL;
        file_free(file);                                                /*  ����رճɹ�, �ͷ��ļ��ṹ  */
    }
    vfs_file_end(gettid());
    return ret;
}

/*
 * �ر��ļ�
 */
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
    if (file->ref == 1) {
        ret = point->fs->close(point, file);
    } else {
        ret = 0;
    }
    if (ret == 0) {
        info->files[fd] = NULL;
        file_free(file);                                                /*  ����رճɹ�, �ͷ��ļ��ṹ  */
    }
    vfs_file_end(tid);
    return ret;
}

/*
 * �����ļ�
 */
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

/*
 * ����ļ�״̬
 */
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

/*
 * �ж��ļ��ǲ����ն�
 */
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

/*
 * ͬ���ļ�
 */
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

/*
 * ͬ���ļ�
 */
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

/*
 * �޸��ļ�����
 */
int vfs_ftruncate(int fd, off_t len)
{
    int ret;

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

/*
 * ���ļ�
 */
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

/*
 * д�ļ�
 */
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

/*
 * �����ļ�
 */
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

/*
 * �����ļ���дλ��
 */
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

/*
 * �����ļ�������
 */
int vfs_dup(int fd)
{
    file_t *temp;
    int i;

    vfs_file_begin(gettid());
                                                                        /*  ����һ�����е��ļ�������    */
    for (i = 0, temp = info->files[0]; i < OPEN_MAX; i++, temp = info->files[i]) {
        if (temp == NULL) {
            break;
        }
    }
    if (i == OPEN_MAX) {                                                /*  û�ҵ�                      */
        vfs_file_end(gettid());
        seterrno(EMFILE);
        return -1;
    }

    info->files[i] = file;

    file->ref++;

    seterrno(0);

    vfs_file_end(gettid());

    return i;
}

/*
 * �����ļ���������ָ�����ļ�������
 */
int vfs_dup2(int fd, int to)
{
    int ret;

    vfs_file_begin(gettid());

    ret = vfs_close(to);
    if (ret < 0) {
        vfs_file_end(gettid());
        return ret;
    }

    info->files[to] = file;

    file->ref++;

    seterrno(0);

    vfs_file_end(gettid());

    return 0;
}
/*********************************************************************************************************
 *                                          select ʵ��
 */
/*
 * ɨ���ļ�
 */
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

/*
 * ����ǰ������뵽�ļ��ĵȴ��б�
 */
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

/*
 * ����ǰ������ļ��ĵȴ��б����Ƴ�
 */
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

/*
 * ɨ���ļ���
 */
static int vfs_select_scan(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds)
{
    fd_set rfds, wfds, efds;
    int flags;
    int i;
    int nset;
    int ret;

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

/*
 * ����ǰ������ļ����ĵȴ��б����Ƴ�
 */
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

/*
 * ����ǰ������뵽�ļ����ĵȴ��б�
 */
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

/*
 * select
 */
int vfs_select(int nfds, fd_set *readfds, fd_set *writefds,
               fd_set *errorfds, struct timeval *timeout)
{
    int resume_type;
    int nset;
    int ret;
    uint32_t reg;

    if (nfds < 0) {
        seterrno(EINVAL);
        return -1;
    }

    reg = interrupt_disable();
    nset = vfs_select_scan(nfds, readfds, writefds, errorfds);
    if (nset > 0) {
        interrupt_resume(reg);
        seterrno(0);
        return nset;
    } else if (nset < 0) {
        interrupt_resume(reg);
        return -1;
    }

    if (timeout != NULL && (timeout->tv_sec == 0 && timeout->tv_usec == 0)) {
        interrupt_resume(reg);
        if (readfds) {
            FD_ZERO(&readfds);
        }
        if (writefds) {
            FD_ZERO(&writefds);
        }
        if (errorfds) {
            FD_ZERO(&errorfds);
        }
        seterrno(0);
        return 0;
    }

    ret = vfs_select_select(nfds, readfds, writefds, errorfds);
    if (ret < 0) {
        interrupt_resume(reg);
        if (readfds) {
            FD_ZERO(&readfds);
        }
        if (writefds) {
            FD_ZERO(&writefds);
        }
        if (errorfds) {
            FD_ZERO(&errorfds);
        }
        return -1;
    }

    if (timeout == NULL) {
        current->timer   = 0;
        current->state   = TASK_SUSPEND;
    } else {
        current->timer   = timeout->tv_sec  * TICK_PER_SECOND +
                           timeout->tv_usec * TICK_PER_SECOND / 1000000;
        current->state   = TASK_SLEEPING;
    }
    current->resume_type = TASK_RESUME_UNKNOW;

    yield();

    vfs_select_unselect(nfds, readfds, writefds, errorfds);

    current->timer       = 0;
    resume_type          = current->resume_type;
    current->resume_type = TASK_RESUME_UNKNOW;

    if (resume_type & TASK_RESUME_SELECT_EVENT) {
        nset = vfs_select_scan(nfds, readfds, writefds, errorfds);
        interrupt_resume(reg);
        if (nset > 0) {
            seterrno(0);
            return nset;
        }

        if (readfds) {
            FD_ZERO(&readfds);
        }
        if (writefds) {
            FD_ZERO(&writefds);
        }
        if (errorfds) {
            FD_ZERO(&errorfds);
        }

        if (nset == 0) {
            seterrno(0);
            return 0;
        } else {
            return -1;
        }
    } else {
        interrupt_resume(reg);
        if (readfds) {
            FD_ZERO(&readfds);
        }
        if (writefds) {
            FD_ZERO(&writefds);
        }
        if (errorfds) {
            FD_ZERO(&errorfds);
        }
        if (resume_type & TASK_RESUME_INTERRUPT) {
            seterrno(EINTR);
            return -1;
        } else {
            return 0;
        }
    }
}
/*********************************************************************************************************
 *                                          �ļ�ϵͳ�����ӿ�
 */
/*
 * ���ļ�����һ������
 */
int vfs_link(const char *path1, const char *path2)
{
    mount_point_t *point1;
    char pathbuf1[PATH_MAX];
    char *filepath1;
    mount_point_t *point2;
    char pathbuf2[PATH_MAX];
    char *filepath2;
    int ret;

    point1 = vfs_mount_point_lookup(pathbuf1, &filepath1, path1);       /*  ���ҹ��ص�                  */
    if (point1 == NULL) {
        return -1;
    }

    point2 = vfs_mount_point_lookup(pathbuf2, &filepath2, path2);       /*  ���ҹ��ص�                  */
    if (point2 == NULL) {
        return -1;
    }

    if (point2 != point1) {                                             /*  �������ص����Ҫ��ͬ        */
        seterrno(EXDEV);
        return -1;
    }

    if (point1->fs->link == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point1->fs->link(point1, filepath1, filepath2);
    return ret;
}

/*
 * ������(Ҳ���ƶ�)�ļ�
 */
int vfs_rename(const char *old, const char *new)
{
    mount_point_t *point1;
    char pathbuf1[PATH_MAX];
    char *filepath1;
    mount_point_t *point2;
    char pathbuf2[PATH_MAX];
    char *filepath2;
    int ret;

    point1 = vfs_mount_point_lookup(pathbuf1, &filepath1, old);         /*  ���ҹ��ص�                  */
    if (point1 == NULL) {
        return -1;
    }

    point2 = vfs_mount_point_lookup(pathbuf2, &filepath2, new);         /*  ���ҹ��ص�                  */
    if (point2 == NULL) {
        return -1;
    }

    if (point2 != point1) {                                             /*  �������ص����Ҫ��ͬ        */
        seterrno(EXDEV);
        return -1;
    }

    if (point1->fs->rename == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point1->fs->rename(point1, filepath1, filepath2);
    return ret;
}

/*
 * ����ļ�״̬
 */
int vfs_stat(const char *path, struct stat *buf)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
    char *filepath;
    int ret;

    if (buf == NULL) {
        return -1;
    }

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->stat == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->stat(point, filepath, buf);
    return ret;
}

/*
 * ɾ���ļ�
 */
int vfs_unlink(const char *path)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->unlink == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->unlink(point, filepath);
    return ret;
}

/*
 * ����Ŀ¼
 */
int vfs_mkdir(const char *path, mode_t mode)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->mkdir == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->mkdir(point, filepath, mode);
    return ret;
}

/*
 * ɾ��Ŀ¼
 */
int vfs_rmdir(const char *path)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->rmdir == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->rmdir(point, filepath);
    return ret;
}

/*
 * �ж��Ƿ�ɷ���
 */
int vfs_access(const char *path, int amode)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->access == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->access(point, filepath, amode);
    return ret;
}

/*
 * �޸��ļ�����
 */
int vfs_truncate(const char *path, off_t len)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->truncate == NULL) {
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->truncate(point, filepath, len);
    return ret;
}

/*
 * ͬ��
 */
int vfs_sync(const char *path)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->sync == NULL) {
        seterrno(ENOSYS);
        return 0;
    }

    seterrno(0);
    ret = point->fs->sync(point);
    return ret;
}
/*********************************************************************************************************
 *                                          Ŀ¼�����ӿ�
 */

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
        file = info->files[fd];                                         /*  ����ļ��ṹ                */\
        if (file == NULL || file->ref == 0) {                           /*  ���Ŀ¼δ��              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        if (!(file->flags & VFS_FILE_TYPE_DIR)) {                       /*  ����ļ��ṹ����Ŀ¼        */\
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point                                             /*  ��ù��ص�                  */

#define vfs_dir_end(tid)

/*
 * ��Ŀ¼
 */
DIR *vfs_opendir(const char *path)
{
    mount_point_t *point;
    vfs_info_t *info;
    file_t *file;
    char pathbuf[PATH_MAX];
    char *filepath;
    int fd;
    int ret;

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return (DIR *)0;
    }
                                                                        /*  ����һ�����е��ļ�������    */
    info = infos + gettid();
    for (fd = 1, file = info->files[1]; fd < OPEN_MAX; fd++, file = info->files[fd]) {
        if (file == NULL) {
            break;
        }
    }
    if (fd == OPEN_MAX) {                                               /*  û�ҵ�                      */
        seterrno(EMFILE);
        return (DIR *)0;
    }

    file = file_alloc();                                                /*  ����һ�����е��ļ��ṹ      */
    if (file == NULL) {
        seterrno(EMFILE);
        return (DIR *)0;
    }

    file->ctx    = NULL;                                                /*  ��ʼ���ļ��ṹ              */
    file->ctx1   = NULL;
    file->flags  = VFS_FILE_TYPE_DIR;                                   /*  �ļ��ṹ�����ļ�            */
    file->point  = point;                                               /*  ��¼���ص�                  */

    if (point->fs->opendir == NULL) {
        seterrno(ENOSYS);
        file_free(file);
        return (DIR *)0;
    }

    seterrno(0);
    ret = point->fs->opendir(point, file, filepath);                    /*  ���ļ�                    */
    if (ret < 0) {
        file_free(file);
        return (DIR *)0;
    }

    info->files[fd] = file;

    return (DIR *)fd;                                                   /*  �����ļ�������              */
}

/*
 * �ر�Ŀ¼
 */
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
    if (file->ref == 1) {
        ret = point->fs->closedir(point, file);
    } else {
        ret = 0;
    }
    if (ret == 0) {
        info->files[fd] = NULL;
        file_free(file);                                                /*  ����رճɹ�, �ͷ��ļ��ṹ  */
    }
    vfs_dir_end(gettid());
    return ret;
}

/*
 * �ر�Ŀ¼
 */
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
    if (file->ref == 1) {
        ret = point->fs->closedir(point, file);
    } else {
        ret = 0;
    }
    if (ret == 0) {
        info->files[fd] = NULL;
        file_free(file);                                                /*  ����رճɹ�, �ͷ��ļ��ṹ  */
    }
    vfs_dir_end(tid);
    return ret;
}

/*
 * ��Ŀ¼��
 */
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

struct dirent *vfs_readdir(DIR *dir)
{
    struct dirent *entry;

    if (__vfs_readdir(dir, &entry) < 0) {
        return NULL;
    } else {
        return entry;
    }
}

/*
 * ����Ŀ¼����
 */
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

/*
 * ����Ŀ¼����
 */
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

/*
 * ���Ŀ¼����
 */
long vfs_telldir(DIR *dir)
{
    int fd = (int)dir;
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

/*
 * �ı䵱ǰ����Ŀ¼
 */
int vfs_chdir(const char *path)
{
    char pathbuf[PATH_MAX];
    int ret;
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
        }
    }

    return ret;
}

/*
 * ��õ�ǰ����Ŀ¼
 */
char *vfs_getcwd(char *buf, size_t size)
{
    int32_t tid = gettid();

    if (buf != NULL) {
        strlcpy(buf, infos[tid].cwd, size);
        return buf;
    } else {
        /*
         * TODO: Ӧ�ó������ͷ���
         */
        return infos[tid].cwd;
    }
}

/*
 * ��ʼ�������ļ�ϵͳ
 */
int vfs_init(void)
{
    file_manager_init();

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

    mount("/",    NULL, "rootfs");

    mount("/dev", NULL, "devfs");

    return 0;
}

/*
 * ��ʼ��������ļ���Ϣ
 */
int vfs_task_init(pid_t tid)
{
    vfs_info_t *info;

    if (tid < TASK_NR) {
        info = infos + tid;

        strcpy(info->cwd, "/");

        memset(info->files, 0, sizeof(info->files));

        return 0;
    } else {
        return -1;
    }
}

/*
 * ����������ļ���Ϣ
 */
int vfs_task_cleanup(pid_t tid)
{
    vfs_info_t *info;
    file_t *file;
    int fd;

    if (tid < TASK_NR) {
        info = infos + tid;

        for (fd = 0, file = info->files[0]; fd < OPEN_MAX; fd++, file = info->files[fd]) {
            if (file == NULL || file->ref == 0) {
                continue;
            }
            if (file->flags & VFS_FILE_TYPE_FILE) {
                __vfs_close(tid, fd);
            } else if (file->flags & VFS_FILE_TYPE_DIR) {
                __vfs_closedir(tid, (DIR *)fd);
            }
        }
        return 0;
    } else {
        return -1;
    }
}

/*
 * fork ���̵��ļ���Ϣ
 */
int vfs_process_fork(pid_t pid, pid_t ppid)
{
    vfs_info_t *info;
    vfs_info_t *pinfo;
    file_t *file;
    int fd;

    if (pid < PROCESS_NR && ppid < PROCESS_NR) {
        pinfo = infos + ppid;
        info  = infos + pid;

        strcpy(info->cwd, pinfo->cwd);

        for (fd = 0, file = pinfo->files[0];
             fd < OPEN_MAX;
             fd++, file = pinfo->files[fd]) {

            if (file == NULL || file->ref == 0) {
                info->files[fd] = NULL;
            } else {
                info->files[fd] = file;
                file->ref++;
            }
        }
        return 0;
    } else {
        return -1;
    }
}

/*
 * �����ļ�����������ļ��ṹ
 */
file_t *vfs_get_file(int fd)
{
    file_t *file;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  �ļ��������Ϸ����ж�        */                                                              \
        return NULL;
    }

    file = infos[gettid()].files[fd];
    if (file == NULL || file->ref == 0) {                               /*  ����ļ�δ��              */
        return NULL;
    }

    if (!(file->flags & VFS_FILE_TYPE_FILE)) {                          /*  ����ļ��ṹ�����ļ�        */
        return NULL;
    }

    return file;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/