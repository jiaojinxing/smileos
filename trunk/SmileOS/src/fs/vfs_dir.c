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
** File name:               vfs_dir.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            �����ļ�ϵͳ��Ŀ¼�����ӿ�
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
#include "vfs_common.h"
/*********************************************************************************************************
**
** Ŀ¼�����ӿ�
**
** ����ĺ�ͦ���ĵ�, �����û��Щ��, ����ļ�Ҫ��������, ���Ҳ�����ͳһ����
**
*********************************************************************************************************/
#define vfs_dir_begin(pid)                                                                                \
        mount_point_t  *point;                                                                            \
        file_t         *file;                                                                             \
        vfs_info_t     *info;                                                                             \
                                                                                                          \
        mutex_lock(&info_lock[pid], 0);                                                                   \
                                                                                                          \
        info = infos[pid];                                                                                \
                                                                                                          \
        if (fd < 1 || fd >= info->open_max) {                           /*  �ļ��������Ϸ����ж�        */\
            mutex_unlock(&info_lock[pid]);                                                                \
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        file = info->files[fd];                                         /*  ����ļ��ṹ                */\
                                                                                                          \
        mutex_unlock(&info_lock[pid]);                                                                    \
                                                                                                          \
        if (file == NULL) {                                             /*  ����ļ�δ��              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        if (mutex_lock(&file->lock, 0) < 0) {                                                             \
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        if (!(file->type & VFS_FILE_TYPE_DIR)) {                        /*  ������Ͳ���Ŀ¼ ����       */\
            mutex_unlock(&file->lock);                                                                    \
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        point = file->point                                             /*  ��ù��ص�                  */

#define vfs_dir_end(pid)                                                                                  \
        mutex_unlock(&file->lock)
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
    char            pathbuf[PATH_BUF_LEN];
    char           *filepath;
    int             fd;
    int             ret;
    int             pid = getpid();

    point = vfs_mount_point_lookup2_ref(pathbuf, &filepath, path);      /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return (DIR *)0;
    }

    mutex_lock(&info_lock[pid], 0);

    info = infos[pid];

    for (fd = 0; fd < info->open_max; fd++) {                           /*  ����һ�����е��ļ�������    */
        file = info->files[fd];
        if (file == NULL) {
            break;
        }
    }

    if (fd == info->open_max) {                                         /*  û�ҵ�                      */
        mutex_unlock(&info_lock[pid]);
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return (DIR *)0;
    }

    file = vfs_file_alloc();                                            /*  ����һ���ļ��ṹ            */
    if (file == NULL) {
        mutex_unlock(&info_lock[pid]);
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return (DIR *)0;
    }

    info->files[fd] = file;                                             /*  ��¼����                    */

    mutex_lock(&file->lock, 0);                                         /*  �����ļ��ṹ                */

    mutex_unlock(&info_lock[pid]);

    file->type   = VFS_FILE_TYPE_DIR;                                   /*  ����: �ļ�                  */
    file->flags  = (0);                                                 /*  �򿪱�־                    */
    file->ctx    = NULL;                                                /*  ������     ��               */
    file->point  = point;                                               /*  ���ص�                      */

    if (point->fs->opendir == NULL) {
        mutex_lock(&info_lock[pid], 0);
        info->files[fd] = NULL;
        mutex_unlock(&info_lock[pid]);
        vfs_file_free(file);
        atomic_dec(&point->ref);
        seterrno(ENOSYS);
        return (DIR *)0;
    }

    seterrno(0);
    ret = point->fs->opendir(point, file, filepath);                    /*  ���ļ�                    */
    if (ret < 0) {
        mutex_lock(&info_lock[pid], 0);
        info->files[fd] = NULL;
        mutex_unlock(&info_lock[pid]);
        vfs_file_free(file);
        atomic_dec(&point->ref);
        return (DIR *)0;
    }

    mutex_unlock(&file->lock);

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
    pid_t pid = getpid();

    vfs_dir_begin(pid);
    if (point->fs->closedir == NULL) {
        vfs_dir_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->closedir(point, file);
    if (ret == 0) {
        mutex_lock(&info_lock[pid], 0);
        info->files[fd] = NULL;
        mutex_unlock(&info_lock[pid]);
        vfs_file_free(file);
        atomic_dec(&point->ref);
        return ret;
    }
    vfs_dir_end(pid);
    return ret;
}
/*********************************************************************************************************
** Function name:           __vfs_closedir
** Descriptions:            �ر�Ŀ¼
** input parameters:        pid                 ���� ID
**                          dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int __vfs_closedir(pid_t pid, DIR *dir)
{
    int fd = (int)dir;
    int ret;

    vfs_dir_begin(pid);
    if (point->fs->closedir == NULL) {
        vfs_dir_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->closedir(point, file);
    if (ret == 0) {
        mutex_lock(&info_lock[pid], 0);
        info->files[fd] = NULL;
        mutex_unlock(&info_lock[pid]);
        vfs_file_free(file);
        atomic_dec(&point->ref);
        return ret;
    }
    vfs_dir_end(pid);
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
    pid_t pid = getpid();

    vfs_dir_begin(pid);
    if (point->fs->readdir == NULL) {
        vfs_dir_end(pid);
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
    vfs_dir_end(pid);
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
    pid_t pid = getpid();

    vfs_dir_begin(pid);
    if (point->fs->rewinddir == NULL) {
        vfs_dir_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->rewinddir(point, file);
    vfs_dir_end(pid);
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
    pid_t pid = getpid();

    vfs_dir_begin(pid);
    if (point->fs->seekdir == NULL) {
        vfs_dir_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->seekdir(point, file, loc);
    vfs_dir_end(pid);
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
    int  pid = getpid();

    vfs_dir_begin(pid);
    if (point->fs->telldir == NULL) {
        vfs_dir_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    loc = point->fs->telldir(point, file);
    vfs_dir_end(pid);
    return loc;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
