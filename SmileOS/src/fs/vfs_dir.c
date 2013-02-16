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
** Descriptions:            虚拟文件系统的目录操作接口
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
#include "vfs_common.h"
/*********************************************************************************************************
**
** 目录操作接口
**
** 这里的宏挺恶心的, 但如果没这些宏, 这个文件要大三倍吧, 并且不利于统一更改
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
        if (fd < 1 || fd >= info->open_max) {                           /*  文件描述符合法性判断        */\
            mutex_unlock(&info_lock[pid]);                                                                \
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        file = info->files[fd];                                         /*  获得文件结构                */\
                                                                                                          \
        mutex_unlock(&info_lock[pid]);                                                                    \
                                                                                                          \
        if (file == NULL) {                                             /*  如果文件未打开              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        if (mutex_lock(&file->lock, 0) < 0) {                                                             \
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        if (!(file->type & VFS_FILE_TYPE_DIR)) {                        /*  如果类型不是目录 　　       */\
            mutex_unlock(&file->lock);                                                                    \
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        point = file->point                                             /*  获得挂载点                  */

#define vfs_dir_end(pid)                                                                                  \
        mutex_unlock(&file->lock)
/*********************************************************************************************************
** Function name:           vfs_opendir
** Descriptions:            打开目录
** input parameters:        path                目录 PATH
** output parameters:       NONE
** Returned value:          目录指针
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

    point = vfs_mount_point_lookup2_ref(pathbuf, &filepath, path);      /*  查找挂载点                  */
    if (point == NULL) {
        return (DIR *)0;
    }

    mutex_lock(&info_lock[pid], 0);

    info = infos[pid];

    for (fd = 0; fd < info->open_max; fd++) {                           /*  查找一个空闲的文件描述符    */
        file = info->files[fd];
        if (file == NULL) {
            break;
        }
    }

    if (fd == info->open_max) {                                         /*  没找到                      */
        mutex_unlock(&info_lock[pid]);
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return (DIR *)0;
    }

    file = vfs_file_alloc();                                            /*  分配一个文件结构            */
    if (file == NULL) {
        mutex_unlock(&info_lock[pid]);
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return (DIR *)0;
    }

    info->files[fd] = file;                                             /*  记录下来                    */

    mutex_lock(&file->lock, 0);                                         /*  锁上文件结构                */

    mutex_unlock(&info_lock[pid]);

    file->type   = VFS_FILE_TYPE_DIR;                                   /*  类型: 文件                  */
    file->flags  = (0);                                                 /*  打开标志                    */
    file->ctx    = NULL;                                                /*  上下文     　               */
    file->point  = point;                                               /*  挂载点                      */

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
    ret = point->fs->opendir(point, file, filepath);                    /*  打开文件                    */
    if (ret < 0) {
        mutex_lock(&info_lock[pid], 0);
        info->files[fd] = NULL;
        mutex_unlock(&info_lock[pid]);
        vfs_file_free(file);
        atomic_dec(&point->ref);
        return (DIR *)0;
    }

    mutex_unlock(&file->lock);

    return (DIR *)fd;                                                   /*  返回文件描述符              */
}
/*********************************************************************************************************
** Function name:           vfs_closedir
** Descriptions:            关闭目录
** input parameters:        dir                 目录指针
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
** Descriptions:            关闭目录
** input parameters:        pid                 任务 ID
**                          dir                 目录指针
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
** Descriptions:            读目录项
** input parameters:        dir                 目录指针
** output parameters:       entry               目录项
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
** Descriptions:            读目录项
** input parameters:        dir                 目录指针
** output parameters:       NONE
** Returned value:          目录项
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
** Descriptions:            重置目录读点
** input parameters:        dir                 目录指针
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
** Descriptions:            调整目录读点
** input parameters:        dir                 目录指针
**                          loc                 新的读点
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
** Descriptions:            获得目录读点
** input parameters:        dir                 目录指针
** output parameters:       NONE
** Returned value:          当前读点
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
