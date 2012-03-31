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
#include <fcntl.h>
#include <unistd.h>

/*
 * 任务文件信息
 */
typedef struct {
    /*
     * TODO: 一个 fd 对应一个 file_t 这样的设计很难实现 dup, dup2
     */
    file_t              files[OPEN_MAX];                                /*  文件结构表                  */
    char                cwd[PATH_MAX];                                  /*  当前工作目录                */
    mutex_t             cwd_lock;                                       /*  当前工作目录锁              */
} vfs_info_t;

static vfs_info_t infos[TASK_NR];

/*
 * 在 PATH 前加入挂载点名
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
 * 正常化 PATH
 */
static int vfs_path_normalization(char path[PATH_MAX], int sprit_end)
{
    /*
     * 文本状态机! 向李先静老师致敬!
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
static mount_point_t *vfs_mount_point_lookup(char pathbuf[PATH_MAX], char **ppath, const char *path)
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
        strlcpy(pathbuf, path, PATH_MAX);
    } else {                                                            /*  如果是相对路径              */
        /*
         * cwd 要以 / 号开头和结尾
         */
        int32_t tid = gettid();
        mutex_lock(&infos[tid].cwd_lock, 0);                   /*  在前面加入当前工作目录      */
        snprintf(pathbuf, PATH_MAX, "%s%s", infos[tid].cwd, path);
        mutex_unlock(&infos[tid].cwd_lock);
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
static mount_point_t *vfs_mount_point_lookup2(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t *point;
    char *tmp;
    static char rootdir[] = "/";

    if (path == NULL) {                                                 /*  PATH 合法性检查             */
        return NULL;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        strlcpy(pathbuf, path, PATH_MAX);
    } else {                                                            /*  如果是相对路径              */
        /*
         * cwd 要以 / 号开头和结尾
         */
        int32_t tid = gettid();
        mutex_lock(&infos[tid].cwd_lock, 0);                   /*  在前面加入当前工作目录      */
        snprintf(pathbuf, PATH_MAX, "%s%s", infos[tid].cwd, path);
        mutex_unlock(&infos[tid].cwd_lock);
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
 *                                          文件操作接口
 */

#define vfs_file_api_begin0                                                                               \
        mount_point_t *point;                                                                             \
        file_t *file;                                                                                     \
                                                                                                          \
        if (fd < 0 || fd >= OPEN_MAX) {                                 /*  文件描述符合法性判断        */\
            return -1;                                                                                    \
        }                                                                                                 \
        file = infos[gettid()].files + fd;                     /*  获得文件结构                */\
        mutex_lock(&file->lock, 0);                                     /*  锁住文件                    */

#define vfs_file_api_begin                                                                                \
        vfs_file_api_begin0                                                                               \
        if (!(file->flag & VFS_FILE_TYPE_FILE)) {                       /*  如果文件未打开或不是文件    */\
            mutex_unlock(&file->lock);                                                                    \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point;                                            /*  获得挂载点                  */

#define vfs_file_api_end                                                                                  \
        mutex_unlock(&file->lock);                                      /*  解锁文件                    */

/*
 * 打开文件
 */
int vfs_open(const char *path, int oflag, mode_t mode)
{
    mount_point_t *point;
    file_t *file;
    char pathbuf[PATH_MAX];
    char *filepath;
    int fd;
    int ret;

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }
                                                                        /*  查找一个空闲的文件结构      */
    for (fd = 0, file = infos[gettid()].files; fd < OPEN_MAX; fd++, file++) {
        mutex_lock(&file->lock, 0);
        if (!file->flag) {
            break;
        }
        mutex_unlock(&file->lock);
    }
    if (fd == OPEN_MAX) {                                               /*  没找到                      */
        return -1;
    }

    file->ctx   = NULL;
    file->point = point;                                                /*  记录挂载点                  */

    if (point->fs->open == NULL) {
        mutex_unlock(&file->lock);
        return -1;
    }

    ret = point->fs->open(point, file, filepath, oflag, mode);          /*  打开文件                    */
    if (ret < 0) {
        mutex_unlock(&file->lock);
        return -1;
    }
    file->flag  = VFS_FILE_TYPE_FILE;                                   /*  占用文件结构, 类型文件      */

    file->flag |= (oflag & O_ACCMODE) + 1;                              /*  记录访问模式                */

    file->flag |= oflag & O_APPEND;                                     /*  记录追加模式                */

    mutex_unlock(&file->lock);

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
        return 0;
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
    if (!(file->flag & FWRITE)) {
        vfs_file_api_end
        return 0;
    }
    if (point->fs->fsync == NULL) {
        vfs_file_api_end
        return 0;
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
    if (!(file->flag & FWRITE)) {
        vfs_file_api_end
        return 0;
    }
    if (point->fs->fdatasync == NULL) {
        vfs_file_api_end
        return 0;
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
    if (!(file->flag & FWRITE)) {
        vfs_file_api_end
        return -1;
    }
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
        if (!(file->flag & FREAD)) {
            vfs_file_api_end
            return -1;
        }
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
        if (!(file->flag & FWRITE)) {
            vfs_file_api_end
            return -1;
        }
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
    char pathbuf1[PATH_MAX];
    char *filepath1;
    mount_point_t *point2;
    char pathbuf2[PATH_MAX];
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
    char pathbuf1[PATH_MAX];
    char *filepath1;
    mount_point_t *point2;
    char pathbuf2[PATH_MAX];
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
    char pathbuf[PATH_MAX];
    char *filepath;
    int ret;

    if (buf == NULL) {
        return -1;
    }

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
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
    char pathbuf[PATH_MAX];
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
    char pathbuf[PATH_MAX];
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
    char pathbuf[PATH_MAX];
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
int vfs_access(const char *path, int amode)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->access == NULL) {
        return -1;
    }

    ret = point->fs->access(point, filepath, amode);
    return ret;
}

/*
 * 修改文件长度
 */
int vfs_truncate(const char *path, off_t len)
{
    mount_point_t *point;
    char pathbuf[PATH_MAX];
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
    char pathbuf[PATH_MAX];
    char *filepath;
    int ret;

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }

    if (point->fs->sync == NULL) {
        return 0;
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
        file = infos[gettid()].files + fd;                     /*  获得文件结构                */\
        mutex_lock(&file->lock, 0);                                     /*  锁住文件                    */

#define vfs_dir_api_begin                                                                                 \
        vfs_dir_api_begin0                                                                                \
        if (!(file->flag & VFS_FILE_TYPE_DIR)) {                        /*  如果文件未打开或不是目录    */\
            mutex_unlock(&file->lock);                                                                    \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point;                                            /*  获得挂载点                  */

#define vfs_dir_api_end                                                                                   \
        mutex_unlock(&file->lock);                                      /*  解锁文件                    */

/*
 * 打开目录
 */
DIR *vfs_opendir(const char *path)
{
    mount_point_t *point;
    file_t *file;
    char pathbuf[PATH_MAX];
    char *filepath;
    int fd;
    int ret;

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
    if (point == NULL) {
        return (DIR *)0;
    }
                                                                        /*  查找一个空闲的文件结构      */
    for (fd = 1 /* 不使用 0 */, file = infos[gettid()].files + 1; fd < OPEN_MAX; fd++, file++) {
        mutex_lock(&file->lock, 0);
        if (!file->flag) {
            break;
        }
        mutex_unlock(&file->lock);
    }
    if (fd == OPEN_MAX) {                                               /*  没找到                      */
        return (DIR *)0;
    }

    file->point = point;                                                /*  记录挂载点                  */

    if (point->fs->opendir == NULL) {
        mutex_unlock(&file->lock);
        return (DIR *)0;
    }

    ret = point->fs->opendir(point, file, filepath);                    /*  打开目录                    */
    if (ret < 0) {
        mutex_unlock(&file->lock);
        return (DIR *)0;
    }
    file->flag = VFS_FILE_TYPE_DIR;                                     /*  占用文件结构, 类型: 目录    */

    mutex_unlock(&file->lock);

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
    char pathbuf[PATH_MAX];
    int ret;
    int32_t tid;

    if (path == NULL) {
        return -1;
    }

    tid = gettid();

    mutex_lock(&infos[tid].cwd_lock, 0);

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        strlcpy(pathbuf, path, sizeof(pathbuf));
    } else {                                                            /*  如果是相对路径              */
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

    mutex_unlock(&infos[tid].cwd_lock);

    return ret;
}

/*
 * 获得当前工作目录
 */
char *vfs_getcwd(char *buf, size_t size)
{
    int32_t tid = gettid();

    if (buf != NULL) {
        mutex_lock(&infos[tid].cwd_lock, 0);
        strlcpy(buf, infos[tid].cwd, size);
        mutex_unlock(&infos[tid].cwd_lock);
        return buf;
    } else {
        /*
         * TODO: 应用程序不能释放它
         */
        return infos[tid].cwd;
    }
}

/*
 * 初始化虚拟文件系统
 */
int vfs_init(void)
{
    vfs_info_t *info;
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

    for (i = 0, info = infos; i < TASK_NR; i++, info++) {
        strcpy(info->cwd, "/");
        mutex_new(&info->cwd_lock);

        for (j = 0, file = info->files; j < OPEN_MAX; j++, file++) {
            mutex_new(&file->lock);
            file->ctx   = NULL;
            file->point = NULL;
            file->flag  = VFS_FILE_TYPE_FREE;
        }
    }

    return 0;
}

/*
 * 初始化任务的文件信息
 */
int vfs_task_init(pid_t tid)
{
    vfs_info_t *info;
    file_t *file;
    int i;

    if (tid < TASK_NR) {
        info = infos + tid;

        strcpy(info->cwd, "/");

        for (i = 0, file = info->files; i < OPEN_MAX; i++, file++) {
            file->ctx   = NULL;
            file->point = NULL;
            file->flag  = VFS_FILE_TYPE_FREE;
        }

        return 0;
    } else {
        return -1;
    }
}

/*
 * 清理任务的文件信息
 */
int vfs_task_cleanup(pid_t tid)
{
    vfs_info_t *info;
    file_t *file;
    int i;

    if (tid < TASK_NR) {
        info = infos + tid;

        strcpy(info->cwd, "/");

        for (i = 0, file = info->files; i < OPEN_MAX; i++, file++) {
            if (file->flag & VFS_FILE_TYPE_FILE) {
                close(i);
            } else if (file->flag & VFS_FILE_TYPE_DIR) {
                vfs_closedir((DIR *)i);
            }

            file->ctx   = NULL;
            file->point = NULL;
            file->flag  = VFS_FILE_TYPE_FREE;
        }
        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
