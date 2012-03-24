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
** Descriptions:            虚拟文件系统
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
#include "vfs/config.h"
#include "vfs/types.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include "vfs/mount.h"
#include "vfs/fs.h"
#include "vfs/vfs.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>

/*
 * 进程文件信息
 */
typedef struct {
    /*
     * TODO: 一个 fd 对应一个 file_t 这样的设计很难实现 dup, dup2
     */
    file_t              files[OPEN_MAX];                                /*  文件结构表                  */
    char                cwd[PATH_MAX + 1];                              /*  当前工作目录                */
    kern_mutex_t        cwd_lock;                                       /*  当前工作目录锁              */
} process_file_info_t;

static process_file_info_t process_file_info[PROCESS_NR];

/*
 * newlib 需要如下的桩函数支持:
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
 * 正常化 PATH
 */
static int vfs_path_normalization(char path[PATH_MAX + 1], int sprit_end)
{
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

    for (p = path; (ch = *p) != '\0'; p++) {                            /*  处理所有文本                */
        switch (state) {
        case BEGIN:                                                     /*  开始状态                    */
            if (ch != '/') {                                            /*  如果不是以 / 号开始, 出错   */
                return -1;
            }
            path[pos++] = ch;                                           /*  记录 / 号                   */
            state = WORKING;
            continue;
            break;

        case WORKING:
            switch (ch) {
            case '.':
                if (prev2_ch == '/' && prev1_ch == '.' && (p[1] == '\0' || p[1] == '/')) {
                    /*
                     *  /../ 或 /..
                     */
                    /*
                     * pos, prev1_ch, prev2_ch
                     */
                    pos -= 3;
                    while (pos >= 0 && path[pos] != '/') {
                        pos--;
                    }
                    if (pos < 0) {
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
                     *  /./ 或 /.
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
 * 查找挂载点, PATH 不能是挂载点
 */
static mount_point_t *vfs_mount_point_lookup(char pathbuf[PATH_MAX + 1], char **ppath, const char *path)
{
    mount_point_t *point;
    char *tmp;

    if (path == NULL) {                                                 /*  PATH 合法性检查             */
        return NULL;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        if (path[1] == '\0') {                                          /*  不能是根目录                */
            return NULL;
        }
        strcpy(pathbuf, path);
    } else {                                                            /*  如果是相对路径              */
        /*
         * cwd 要以 / 号开头和结尾
         */
        kern_mutex_lock(&process_file_info[current->pid].cwd_lock, 0);  /*  在前面加入当前工作目录      */
        sprintf(pathbuf, "%s%s", process_file_info[current->pid].cwd, path);
        kern_mutex_unlock(&process_file_info[current->pid].cwd_lock);
    }

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {
        return NULL;
    }

    tmp = strchr(pathbuf + 1, '/');                                     /*  查找挂载点名后的 / 号       */
    if (tmp == NULL) {                                                  /*  没有到                      */
        return NULL;
    }
    if (tmp[1] == '\0') {                                               /*  不能是挂载点                */
        return NULL;
    }

    *tmp = '\0';                                                        /*  暂时去掉 / 号               */
    point = mount_point_lookup(pathbuf);                                /*  查找挂载点                  */
    *tmp = '/';                                                         /*  恢复 / 号                   */

    *ppath = tmp;
    return point;
}

/*
 * 查找挂载点, PATH 可以是挂载点
 */
static mount_point_t *vfs_mount_point_lookup2(char pathbuf[PATH_MAX + 1], char **ppath, const char *path)
{
    mount_point_t *point;
    char *tmp;
    static char rootdir[] = "/";

    if (path == NULL) {                                                 /*  PATH 合法性检查             */
        return NULL;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        strcpy(pathbuf, path);
    } else {                                                            /*  如果是相对路径              */
        /*
         * cwd 要以 / 号开头和结尾
         */
        kern_mutex_lock(&process_file_info[current->pid].cwd_lock, 0);  /*  在前面加入当前工作目录      */
        sprintf(pathbuf, "%s%s", process_file_info[current->pid].cwd, path);
        kern_mutex_unlock(&process_file_info[current->pid].cwd_lock);
    }

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {
        return NULL;
    }

    if (pathbuf[1] == '\0') {                                           /*  如果是根目录                */
        point = mount_point_lookup(pathbuf);
        tmp = pathbuf;
    } else {
        tmp = strchr(pathbuf + 1, '/');                                 /*  查找挂载点名后的 / 号       */
        if (tmp == NULL) {                                              /*  如果是挂载点                */
            point = mount_point_lookup(pathbuf);
            tmp = rootdir;
        } else {
            *tmp = '\0';                                                /*  暂时去掉 / 号               */
            point = mount_point_lookup(pathbuf);                        /*  查找挂载点                  */
            *tmp = '/';                                                 /*  恢复 / 号                   */
        }
    }

    *ppath = tmp;
    return point;
}

/*
 * 文件类型
 */
#define VFS_FILE_TYPE_FILE      (1 << 1)
#define VFS_FILE_TYPE_DIR       (1 << 2)
#define VFS_FILE_TYPE_FREE      (0)

/*********************************************************************************************************
 *                                          文件操作接口
 */

#define vfs_file_api_begin0                                                                               \
        mount_point_t *point;                                                                             \
        file_t *file;                                                                                     \
                                                                                                          \
        if (fd < 0 || fd >= OPEN_MAX) {                                 /*  文件描述符合法性判断        */\
            return -1;                                                                                    \
        }                                                                                                 \
        file = process_file_info[current->pid].files + fd;              /*  获得文件结构                */\
        kern_mutex_lock(&file->lock, 0);                                /*  锁住文件                    */

#define vfs_file_api_begin                                                                                \
        vfs_file_api_begin0                                                                               \
        if (!(file->flag & VFS_FILE_TYPE_FILE)) {                       /*  如果文件未打开或不是文件    */\
            kern_mutex_unlock(&file->lock);                                                               \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point;                                            /*  获得挂载点                  */

#define vfs_file_api_end                                                                                  \
        kern_mutex_unlock(&file->lock);                                 /*  解锁文件                    */

/*
 * 打开文件
 */
int vfs_open(const char *path, int oflag, mode_t mode)
{
    mount_point_t *point;
    file_t *file;
    char pathbuf[PATH_MAX + 1];
    char *filepath;
    int fd;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }
                                                                        /*  查找一个空闲的文件结构      */
    for (fd = 0, file = process_file_info[current->pid].files; fd < OPEN_MAX; fd++, file++) {
        kern_mutex_lock(&file->lock, 0);
        if (!file->flag) {
            break;
        }
        kern_mutex_unlock(&file->lock);
    }
    if (fd == OPEN_MAX) {                                               /*  没找到                      */
        return -1;
    }

    file->point = point;                                                /*  记录挂载点                  */

    if (point->fs->open == NULL) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }

    ret = point->fs->open(point, file, filepath, oflag, mode);          /*  打开文件                    */
    if (ret < 0) {
        kern_mutex_unlock(&file->lock);
        return -1;
    }
    file->flag = VFS_FILE_TYPE_FILE;                                    /*  占用文件结构, 类型文件      */

    kern_mutex_unlock(&file->lock);

    return fd;                                                          /*  返回文件描述符              */
}

/*
 * 关闭文件
 */
int vfs_close(int fd)
{
    int ret;

    vfs_file_api_begin
    if (point->fs->close == NULL) {
        vfs_file_api_end
        return -1;
    }
    ret = point->fs->close(point, file);
    if (ret == 0) {
        file->flag = VFS_FILE_TYPE_FREE;                                /*  如果关闭成功, 释放文件结构  */
    }
    vfs_file_api_end
    return ret;
}

/*
 * 控制文件
 */
int vfs_fcntl(int fd, int cmd, void *arg)
{
    int ret;

    vfs_file_api_begin
    if (point->fs->fcntl == NULL) {
        vfs_file_api_end
        return -1;
    }
    ret = point->fs->fcntl(point, file, cmd, arg);
    vfs_file_api_end
    return ret;
}

/*
 * 获得文件状态
 */
int vfs_fstat(int fd, struct stat *buf)
{
    int ret;

    if (buf == NULL) {
        return -1;
    }

    {
        vfs_file_api_begin
        if (point->fs->fstat == NULL) {
            vfs_file_api_end
            return -1;
        }
        ret = point->fs->fstat(point, file, buf);
        vfs_file_api_end
        return ret;
    }
}

/*
 * 判断文件是不是终端
 */
int vfs_isatty(int fd)
{
    int ret;

    vfs_file_api_begin
    if (point->fs->isatty == NULL) {
        vfs_file_api_end
        return -1;
    }
    ret = point->fs->isatty(point, file);
    vfs_file_api_end
    return ret;
}

/*
 * 同步文件
 */
int vfs_fsync(int fd)
{
    int ret;

    vfs_file_api_begin
    if (point->fs->fsync == NULL) {
        vfs_file_api_end
        return -1;
    }
    ret = point->fs->fsync(point, file);
    vfs_file_api_end
    return ret;
}

/*
 * 同步文件
 */
int vfs_fdatasync(int fd)
{
    int ret;

    vfs_file_api_begin
    if (point->fs->fdatasync == NULL) {
        vfs_file_api_end
        return -1;
    }
    ret = point->fs->fdatasync(point, file);
    vfs_file_api_end
    return ret;
}

/*
 * 修改文件长度
 */
int vfs_ftruncate(int fd, off_t len)
{
    int ret;

    vfs_file_api_begin
    if (point->fs->ftruncate == NULL) {
        vfs_file_api_end
        return -1;
    }
    ret = point->fs->ftruncate(point, file, len);
    vfs_file_api_end
    return ret;
}

/*
 * 读文件
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
        if (point->fs->read == NULL) {
            vfs_file_api_end
            return -1;
        }
        slen = point->fs->read(point, file, buf, len);
        vfs_file_api_end
        return slen;
    }
}

/*
 * 写文件
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
        if (point->fs->write == NULL) {
            vfs_file_api_end
            return -1;
        }
        slen = point->fs->write(point, file, buf, len);
        vfs_file_api_end
        return slen;
    }
}

/*
 * 控制文件
 */
int vfs_ioctl(int fd, int cmd, void *arg)
{
    int ret;

    vfs_file_api_begin
    if (point->fs->ioctl == NULL) {
        vfs_file_api_end
        return -1;
    }
    ret = point->fs->ioctl(point, file, cmd, arg);
    vfs_file_api_end
    return ret;
}

/*
 * 调整文件读写位置
 */
off_t vfs_lseek(int fd, off_t offset, int whence)
{
    vfs_file_api_begin
    if (point->fs->lseek == NULL) {
        vfs_file_api_end
        return -1;
    }
    offset = point->fs->lseek(point, file, offset, whence);
    vfs_file_api_end
    return offset;
}

/*********************************************************************************************************
 *                                          文件系统操作接口
 */
/*
 * 给文件创建一个链接
 */
int vfs_link(const char *path1, const char *path2)
{
    mount_point_t *point1;
    char pathbuf1[PATH_MAX + 1];
    char *filepath1;
    mount_point_t *point2;
    char pathbuf2[PATH_MAX + 1];
    char *filepath2;
    int ret;

    point1 = vfs_mount_point_lookup(pathbuf1, &filepath1, path1);       /*  查找挂载点                  */
    if (point1 == NULL) {
        return -1;
    }

    point2 = vfs_mount_point_lookup(pathbuf2, &filepath2, path2);       /*  查找挂载点                  */
    if (point2 == NULL) {
        return -1;
    }

    if (point2 != point1) {                                             /*  两个挂载点必须要相同        */
        return -1;
    }

    if (point1->fs->link == NULL) {
        return -1;
    }

    ret = point1->fs->link(point1, filepath1, filepath2);
    return ret;
}

/*
 * 重命名(也可移动)文件
 */
int vfs_rename(const char *old, const char *new)
{
    mount_point_t *point1;
    char pathbuf1[PATH_MAX + 1];
    char *filepath1;
    mount_point_t *point2;
    char pathbuf2[PATH_MAX + 1];
    char *filepath2;
    int ret;

    point1 = vfs_mount_point_lookup(pathbuf1, &filepath1, old);         /*  查找挂载点                  */
    if (point1 == NULL) {
        return -1;
    }

    point2 = vfs_mount_point_lookup(pathbuf2, &filepath2, new);         /*  查找挂载点                  */
    if (point2 == NULL) {
        return -1;
    }

    if (point2 != point1) {                                             /*  两个挂载点必须要相同        */
        return -1;
    }

    if (point1->fs->rename == NULL) {
        return -1;
    }

    ret = point1->fs->rename(point1, filepath1, filepath2);
    return ret;
}

/*
 * 获得文件状态
 */
int vfs_stat(const char *path, struct stat *buf)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX + 1];
    char *filepath;
    int ret;

    if (buf == NULL) {
        return -1;
    }

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->stat == NULL) {
        return -1;
    }

    ret = point->fs->stat(point, filepath, buf);
    return ret;
}

/*
 * 删除文件
 */
int vfs_unlink(const char *path)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX + 1];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->unlink == NULL) {
        return -1;
    }

    ret = point->fs->unlink(point, filepath);
    return ret;
}

/*
 * 创建目录
 */
int vfs_mkdir(const char *path, mode_t mode)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX + 1];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->mkdir == NULL) {
        return -1;
    }

    ret = point->fs->mkdir(point, filepath, mode);
    return ret;
}

/*
 * 删除目录
 */
int vfs_rmdir(const char *path)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX + 1];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->rmdir == NULL) {
        return -1;
    }

    ret = point->fs->rmdir(point, filepath);
    return ret;
}

/*
 * 判断是否可访问
 */
int vfs_access(const char *path, mode_t mode)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX + 1];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->access == NULL) {
        return -1;
    }

    ret = point->fs->access(point, filepath, mode);
    return ret;
}

/*
 * 修改文件长度
 */
int vfs_truncate(const char *path, off_t len)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX + 1];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->truncate == NULL) {
        return -1;
    }

    ret = point->fs->truncate(point, filepath, len);
    return ret;
}

/*
 * 同步
 */
int vfs_sync(const char *path)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX + 1];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->sync == NULL) {
        return -1;
    }

    ret = point->fs->sync(point);
    return ret;
}

/*********************************************************************************************************
 *                                          目录操作接口
 */

#define vfs_dir_api_begin0                                                                                \
        mount_point_t *point;                                                                             \
        file_t *file;                                                                                     \
                                                                                                          \
        if (fd < 1 || fd >= OPEN_MAX) {                                 /*  文件描述符合法性判断        */\
            return -1;                                                                                    \
        }                                                                                                 \
        file = process_file_info[current->pid].files + fd;              /*  获得文件结构                */\
        kern_mutex_lock(&file->lock, 0);                                /*  锁住文件                    */

#define vfs_dir_api_begin                                                                                 \
        vfs_dir_api_begin0                                                                                \
        if (!(file->flag & VFS_FILE_TYPE_DIR)) {                        /*  如果文件未打开或不是目录    */\
            kern_mutex_unlock(&file->lock);                                                               \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point;                                            /*  获得挂载点                  */

#define vfs_dir_api_end                                                                                   \
        kern_mutex_unlock(&file->lock);                                 /*  解锁文件                    */

/*
 * 打开目录
 */
DIR *vfs_opendir(const char *path)
{
    mount_point_t *point;
    file_t *file;
    char pathbuf[PATH_MAX + 1];
    char *filepath;
    int fd;
    int ret;

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
    if (point == NULL) {
        return (DIR *)0;
    }
                                                                        /*  查找一个空闲的文件结构      */
    for (fd = 1 /* 不使用 0 */, file = process_file_info[current->pid].files; fd < OPEN_MAX; fd++, file++) {
        kern_mutex_lock(&file->lock, 0);
        if (!file->flag) {
            break;
        }
        kern_mutex_unlock(&file->lock);
    }
    if (fd == OPEN_MAX) {                                               /*  没找到                      */
        return (DIR *)0;
    }

    file->point = point;                                                /*  记录挂载点                  */

    if (point->fs->opendir == NULL) {
        kern_mutex_unlock(&file->lock);
        return (DIR *)0;
    }

    ret = point->fs->opendir(point, file, filepath);                    /*  打开目录                    */
    if (ret < 0) {
        kern_mutex_unlock(&file->lock);
        return (DIR *)0;
    }
    file->flag = VFS_FILE_TYPE_DIR;                                     /*  占用文件结构, 类型: 目录    */

    kern_mutex_unlock(&file->lock);

    return (DIR *)fd;                                                   /*  返回文件描述符              */
}

/*
 * 关闭目录
 */
int vfs_closedir(DIR *dir)
{
    int fd = (int)dir;
    int ret;

    vfs_dir_api_begin
    if (point->fs->closedir == NULL) {
        vfs_dir_api_end
        return -1;
    }
    ret = point->fs->closedir(point, file);
    if (ret == 0) {
        file->flag = VFS_FILE_TYPE_FREE;                                /*  如果关闭成功, 释放文件结构  */
    }
    vfs_dir_api_end
    return ret;
}

/*
 * 读目录项
 */
static int __vfs_readdir(DIR *dir, struct dirent **entry)
{
    int fd = (int)dir;
    int ret;

    vfs_dir_api_begin
    if (point->fs->readdir == NULL) {
        vfs_dir_api_end
        return -1;
    }
    *entry = point->fs->readdir(point, file);
    if (*entry != NULL) {
        ret = 0;
    } else {
        ret = -1;
    }
    vfs_dir_api_end
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
 * 重置目录读点
 */
int vfs_rewinddir(DIR *dir)
{
    int fd = (int)dir;
    int ret;

    vfs_dir_api_begin
    if (point->fs->rewinddir == NULL) {
        vfs_dir_api_end
        return -1;
    }
    ret = point->fs->rewinddir(point, file);
    vfs_dir_api_end
    return ret;
}

/*
 * 调整目录读点
 */
int vfs_seekdir(DIR *dir, long loc)
{
    int fd = (int)dir;
    int ret;

    vfs_dir_api_begin
    if (point->fs->seekdir == NULL) {
        vfs_dir_api_end
        return -1;
    }
    ret = point->fs->seekdir(point, file, loc);
    vfs_dir_api_end
    return ret;
}

/*
 * 获得目录读点
 */
long vfs_telldir(DIR *dir)
{
    int fd = (int)dir;
    long loc;

    vfs_dir_api_begin
    if (point->fs->telldir == NULL) {
        vfs_dir_api_end
        return -1;
    }
    loc = point->fs->telldir(point, file);
    vfs_dir_api_end
    return loc;
}

/*
 * 改变当前工作目录
 */
int vfs_chdir(const char *path)
{
    char pathbuf[PATH_MAX + 1];
    int ret;

    if (path == NULL) {
        return -1;
    }

    kern_mutex_lock(&process_file_info[current->pid].cwd_lock, 0);

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        strcpy(pathbuf, path);
    } else {                                                            /*  如果是相对路径              */
        sprintf(pathbuf, "%s%s", process_file_info[current->pid].cwd, path);
    }

    ret = vfs_path_normalization(pathbuf, TRUE);
    if (ret == 0) {
        strcpy(process_file_info[current->pid].cwd, pathbuf);
    }

    kern_mutex_unlock(&process_file_info[current->pid].cwd_lock);

    return ret;
}

/*
 * 获得当前工作目录
 */
char *vfs_getcwd(char *buf, size_t size)
{
    if (buf != NULL) {
        kern_mutex_lock(&process_file_info[current->pid].cwd_lock, 0);
        strcpy(buf, process_file_info[current->pid].cwd);
        kern_mutex_unlock(&process_file_info[current->pid].cwd_lock);
        return buf;
    } else {
        return process_file_info[current->pid].cwd;
    }
}

/*
 * 初始化虚拟文件系统
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
            file->flag  = VFS_FILE_TYPE_FREE;
        }
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
