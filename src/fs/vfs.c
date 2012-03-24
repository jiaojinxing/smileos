/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <JiaoJinXing1987@gmail.com>
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
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "vfs/vfs.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/mount.h"
#include "vfs/fs.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>

/*
 * �����ļ���Ϣ
 */
typedef struct {
    file_t              files[OPEN_MAX];                                /*  �ļ��ṹ��                  */
    char                cwd[PATH_MAX];                                  /*  ��ǰ����Ŀ¼                */
    kern_mutex_t        cwd_lock;                                       /*  ��ǰ����Ŀ¼��              */
} process_file_info_t;

process_file_info_t process_file_info[PROCESS_NR];

/*
 * newlib ��Ҫ���µ�׮����֧��:
 *
    extern int _close_r _PARAMS ((struct _reent *, int));
    extern int _execve_r _PARAMS ((struct _reent *, const char *, char *const *, char *const *));
    extern int _fcntl_r _PARAMS ((struct _reent *, int, int, int));
    extern int _fork_r _PARAMS ((struct _reent *));
    extern int _fstat_r _PARAMS ((struct _reent *, int, struct stat *));
    extern int _getpid_r _PARAMS ((struct _reent *));
    extern int _isatty_r _PARAMS ((struct _reent *, int));
    extern int _kill_r _PARAMS ((struct _reent *, int, int));
    extern int _link_r _PARAMS ((struct _reent *, const char *, const char *));
    extern _off_t _lseek_r _PARAMS ((struct _reent *, int, _off_t, int));
    extern int _mkdir_r _PARAMS ((struct _reent *, const char *, int));
    extern int _open_r _PARAMS ((struct _reent *, const char *, int, int));
    extern _ssize_t _read_r _PARAMS ((struct _reent *, int, void *, size_t));
    extern int _rename_r _PARAMS ((struct _reent *, const char *, const char *));
    extern void *_sbrk_r _PARAMS ((struct _reent *, ptrdiff_t));
    extern int _stat_r _PARAMS ((struct _reent *, const char *, struct stat *));
    extern _CLOCK_T_ _times_r _PARAMS ((struct _reent *, struct tms *));
    extern int _unlink_r _PARAMS ((struct _reent *, const char *));
    extern int _wait_r _PARAMS ((struct _reent *, int *));
    extern _ssize_t _write_r _PARAMS ((struct _reent *, int, const void *, size_t));
 */

/*
 * ���ҹ��ص�, PATH �����ǹ��ص�
 */
mount_point_t *vfs_mount_point_lookup(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t *point;
    char *tmp;

    if (path == NULL) {                                                 /*  PATH �Ϸ��Լ��             */
        return NULL;
    }

    if (path[0] == '/') {                                               /*  ����Ǿ���·��              */
        if (path[1] == '\0') {                                          /*  �����Ǹ�Ŀ¼                */
            return NULL;
        }
        strcpy(pathbuf, path);
    } else {                                                            /*  ��������·��              */
        /*
         * cwd Ҫ�� / �ſ�ͷ�ͽ�β
         */
        kern_mutex_lock(&process_file_info[current->pid].cwd_lock, 0);  /*  ��ǰ����뵱ǰ����Ŀ¼      */
        sprintf(pathbuf, "%s%s", process_file_info[current->pid].cwd, path);
        kern_mutex_unlock(&process_file_info[current->pid].cwd_lock);
    }

    /*
     * TODO: ����Ҫ��һ��ѹ��Ŀ¼����, ȥ�� .. �� .
     */

    tmp = strchr(pathbuf + 1, '/');                                     /*  ���ҹ��ص������ / ��       */
    if (tmp == NULL) {                                                  /*  û�е�                      */
        return NULL;
    }
    if (tmp[1] == '0') {                                                /*  �����ǹ��ص�                */
        return NULL;
    }

    *tmp = '\0';                                                        /*  ��ʱȥ�� / ��               */
    point = mount_point_lookup(pathbuf);                                /*  ���ҹ��ص�                  */
    *tmp = '/';                                                         /*  �ָ� / ��                   */

    *ppath = tmp;
    return point;
}

/*
 * ���ҹ��ص�, PATH �����ǹ��ص�
 */
mount_point_t *vfs_mount_point_lookup2(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t *point;
    char *tmp;
    static char rootdir[] = "/";

    if (path == NULL) {                                                 /*  PATH �Ϸ��Լ��             */
        return NULL;
    }

    if (path[0] == '/') {                                               /*  ����Ǿ���·��              */
        strcpy(pathbuf, path);
    } else {                                                            /*  ��������·��              */
        /*
         * cwd Ҫ�� / �ſ�ͷ�ͽ�β
         */
        kern_mutex_lock(&process_file_info[current->pid].cwd_lock, 0);  /*  ��ǰ����뵱ǰ����Ŀ¼      */
        sprintf(pathbuf, "%s%s", process_file_info[current->pid].cwd, path);
        kern_mutex_unlock(&process_file_info[current->pid].cwd_lock);
    }

    /*
     * TODO: ����Ҫ��һ��ѹ��Ŀ¼����, ȥ�� .. �� .
     */

    if (pathbuf[1] == '0') {                                            /*  ����Ǹ�Ŀ¼                */
        point = mount_point_lookup(pathbuf);
        tmp = pathbuf;
    } else {
        tmp = strchr(pathbuf + 1, '/');                                 /*  ���ҹ��ص������ / ��       */
        if (tmp == NULL) {                                              /*  ������ص�                  */
            point = mount_point_lookup(pathbuf);
            tmp = rootdir;
        } else {
            *tmp = '\0';                                                /*  ��ʱȥ�� / ��               */
            point = mount_point_lookup(pathbuf);                        /*  ���ҹ��ص�                  */
            *tmp = '/';                                                 /*  �ָ� / ��                   */
        }
    }

    *ppath = tmp;
    return point;
}

#define vfs_file_api_begin0                                                                               \
        mount_point_t *point;                                                                             \
        file_t *file;                                                                                     \
        int ret;                                                                                          \
                                                                                                          \
        if (fd < 0 || fd >= OPEN_MAX) {                                 /*  �ļ��������Ϸ����ж�        */\
            return -1;                                                                                    \
        }                                                                                                 \
        file = process_file_info[current->pid].files + fd;              /*  ����ļ��ṹ                */\
        kern_mutex_lock(&file->lock, 0);                                /*  ��ס�ļ�                    */

#define vfs_file_api_begin                                                                                \
        vfs_file_api_begin0                                                                               \
        if (!file->used) {                                              /*  ����ļ�δ��              */\
            kern_mutex_unlock(&file->lock);                                                               \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point;                                            /*  ��ù��ص�                  */

#define vfs_file_api_end                                                                                  \
        kern_mutex_unlock(&file->lock);                                 /*  �����ļ�                    */

/*
 * ���ļ�
 */
int vfs_open(const char *path, int oflag, mode_t mode)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
    char *filepath;
    int fd;
    int ret;
    file_t *file;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }
                                                                        /*  ����һ�����е��ļ��ṹ      */
    for (fd = 0, file = process_file_info[current->pid].files; fd < OPEN_MAX; fd++, file++) {
        kern_mutex_lock(&file->lock, 0);
        if (!file->used) {
            break;
        }
        kern_mutex_unlock(&file->lock);
    }
    if (fd == OPEN_MAX) {                                               /*  û�ҵ�                      */
        return -1;
    }

    file->point = point;                                                /*  ��¼���ص�                  */

    ret = point->fs->open(point, file, filepath, oflag, mode);          /*  ���ļ�                    */
    if (ret < 0) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }
    file->used = TRUE;                                                  /*  ռ���ļ��ṹ                */

    kern_mutex_unlock(&file->lock);

    return fd;                                                          /*  �����ļ�������              */
}

/*
 * �ر��ļ�
 */
int vfs_close(int fd)
{
    vfs_file_api_begin0
    if (!file->used) {                                                  /*  ����ļ�δ��              */
        vfs_file_api_end
        return -1;
    }
    point = file->point;                                                /*  ��ù��ص�                  */
    ret = point->fs->close(point, file);
    if (ret == 0) {
        file->used = FALSE;                                             /*  ����رճɹ�, �ͷ��ļ��ṹ  */
    }
    vfs_file_api_end
    return ret;
}

/*
 * �����ļ�
 */
int vfs_fcntl(int fd, int cmd, void *arg)
{
    vfs_file_api_begin
    ret = point->fs->fcntl(point, file, cmd, arg);
    vfs_file_api_end
    return ret;
}

/*
 * ����ļ�״̬
 */
int vfs_fstat(int fd, struct stat *buf)
{
    if (buf == NULL) {
        return -1;
    }

    {
        vfs_file_api_begin
        ret = point->fs->fstat(point, file, buf);
        vfs_file_api_end
        return ret;
    }
}

/*
 * �ж��ļ��ǲ����ն�
 */
int vfs_isatty(int fd)
{
    vfs_file_api_begin
    ret = point->fs->isatty(point, file);
    vfs_file_api_end
    return ret;
}

/*
 * ���ļ�
 */
ssize_t vfs_read(int fd, void *buf, size_t len)
{
    ssize_t slen;

    if (buf == NULL || len < 0) {
        return -1;
    }

    if (len == 0) {
        return 0;
    }

    {
        vfs_file_api_begin
        slen = point->fs->read(point, file, buf, len);
        vfs_file_api_end
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
        return -1;
    }

    if (len == 0) {
        return 0;
    }

    {
        vfs_file_api_begin
        slen = point->fs->write(point, file, buf, len);
        vfs_file_api_end
        return slen;
    }
}

/*
 * �����ļ�
 */
int vfs_ioctl(int fd, int cmd, void *arg)
{
    vfs_file_api_begin
    ret = point->fs->ioctl(point, file, cmd, arg);
    vfs_file_api_end
    return ret;
}

/*
 * �����ļ���дλ��
 */
off_t vfs_lseek(int fd, off_t offset, int whence)
{
    vfs_file_api_begin
    offset = point->fs->lseek(point, file, offset, whence);
    vfs_file_api_end
    return offset;
}

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
        return -1;
    }

    ret = point1->fs->link(point1, filepath1, filepath2);
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

    ret = point->fs->mkdir(point, filepath, mode);
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
        return -1;
    }

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

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }

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

    ret = point->fs->unlink(point, filepath);
    return ret;
}

/*
 * ��Ŀ¼
 */
DIR *vfs_opendir(const char *path)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
    char *filepath;
    int fd;
    int ret;
    file_t *file;

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return (DIR *)-1;
    }

                                                                        /*  ����һ�����е��ļ��ṹ      */
    for (fd = 0, file = process_file_info[current->pid].files; fd < OPEN_MAX; fd++, file++) {
        kern_mutex_lock(&file->lock, 0);
        if (!file->used) {
            break;
        }
        kern_mutex_unlock(&file->lock);
    }
    if (fd == OPEN_MAX) {                                               /*  û�ҵ�                      */
        return (DIR *)-1;
    }

    file->point = point;                                                /*  ��¼���ص�                  */

    ret = point->fs->opendir(point, file, filepath);                    /*  ��Ŀ¼                    */
    if (ret < 0) {
        kern_mutex_unlock(&file->lock);
        return (DIR *)-1;
    }
    file->used = TRUE;                                                  /*  ռ���ļ��ṹ                */

    kern_mutex_unlock(&file->lock);

    return (DIR *)fd;                                                   /*  �����ļ�������              */
}

/*
 * �ر�Ŀ¼
 */
int vfs_closedir(DIR *dir)
{
    int fd = (int)dir;

    vfs_file_api_begin0
    if (!file->used) {                                                  /*  ���Ŀ¼δ��              */
        vfs_file_api_end
        return -1;
    }
    point = file->point;                                                /*  ��ù��ص�                  */
    ret = point->fs->closedir(point, file);
    if (ret == 0) {
        file->used = FALSE;                                             /*  ����رճɹ�, �ͷ��ļ��ṹ  */
    }
    vfs_file_api_end
    return ret;
}

/*
 * ��Ŀ¼��
 */
struct dirent *vfs_readdir(DIR *dir)
{
    int fd = (int)dir;
    struct dirent *entry;

    vfs_file_api_begin
    entry = point->fs->readdir(point, file);
    vfs_file_api_end
    return entry;
}

/*
 * ����Ŀ¼����
 */
int vfs_rewinddir(DIR *dir)
{
    int fd = (int)dir;

    vfs_file_api_begin
    ret = point->fs->rewinddir(point, file);
    vfs_file_api_end
    return ret;
}

/*
 * ����Ŀ¼����
 */
int vfs_seekdir(DIR *dir, long loc)
{
    int fd = (int)dir;

    vfs_file_api_begin
    ret = point->fs->seekdir(point, file, loc);
    vfs_file_api_end
    return ret;
}

/*
 * ���Ŀ¼����
 */
long vfs_telldir(DIR *dir)
{
    int fd = (int)dir;
    long loc;

    vfs_file_api_begin
    loc = point->fs->telldir(point, file);
    vfs_file_api_end
    return loc;
}

/*
 * ��ʼ�������ļ�ϵͳ
 */
int vfs_init(void)
{
    process_file_info_t *info;
    file_t *file;
    int i;
    int j;

    driver_manager_init();

    device_manager_init();

    file_system_manager_init();

    mount_point_manager_init();

    extern file_system_t rootfs;
    file_system_install(&rootfs);

    extern file_system_t devfs;
    file_system_install(&devfs);

    mount("/",    NULL, "rootfs");

    mount("/dev", NULL, "devfs");

    for (i = 0, info = process_file_info; i < PROCESS_NR; i++, info++) {
        strcpy(info->cwd, "/");
        kern_mutex_new(&info->cwd_lock);

        for (j = 0, file = info->files; j < OPEN_MAX; j++, file++) {
            kern_mutex_new(&file->lock);
            file->ctx   = NULL;
            file->point = NULL;
            file->used  = FALSE;
        }
    }

    return 0;
}

void vfs_test(void)
{
    DIR *dir;
    struct dirent *entry;

    dir = vfs_opendir("/dev");

    while ((entry = vfs_readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    vfs_closedir(dir);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
