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
** Modified by:             JiaoJinXing
** Modified date:           2012-4-6
** Version:                 1.1.0
** Descriptions:
**
** 现在支持 select 了, 支持 select 主要是因为用户空间中的 GNU/pth 库的要求!
** 因为在 pthread 里调 write 等函数, 会造成整个进程被阻塞, 从而导致别的 pthread 得不到运行的机会,
** 为了解决这个问题, GNU/pth 库是这样做的, 以调用 write 函数为例:
** 1) 强制将文件设置为非阻塞模式
** 2) 如果 1 步 OK, 那么就调用 write
** 3) 如果 2 步不 OK, 那么 select 文件可写 0 秒
** 4) 如果 select OK, 那么就调用 write
** 5) 如果 select 不 OK, 那么等待 PTH_EVENT_FD|PTH_UNTIL_FD_WRITEABLE|PTH_MODE_STATIC 事件, 会进行线程调度
** 6) pth_scheduler 线程会在没有其它线程可运行时调用 pth_sched_eventmanager 函数
** 7) pth_sched_eventmanager 函数会 select 所有文件可读可写出错一个最小的等待时间
** 8) pth_sched_eventmanager 函数会对 select 的结果进行判定,
**    以唤醒等待 PTH_EVENT_FD|PTH_UNTIL_FD_WRITEABLE|PTH_MODE_STATIC 事件的线程
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-4-12
** Version:                 1.2.0
** Descriptions:            去掉不必要的文件锁, 因为文件结构是任务的私有数据, 下层需要更注重并发和互斥!
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-7-11
** Version:                 1.3.0
** Descriptions:            改进了文件结构的使用, 具备实现 dup, dup2 和 fork 的条件.
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
 * 任务文件信息
 */
typedef struct {
    file_t             *files[OPEN_MAX];                                /*  文件结构表                  */
    char                cwd[PATH_MAX];                                  /*  当前工作目录                */
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
                seterrno(EINVAL);
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
        seterrno(EINVAL);
        return NULL;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        if (path[1] == '\0') {                                          /*  不能是根目录                */
            seterrno(EINVAL);
            return NULL;
        }
        strlcpy(pathbuf, path, PATH_MAX);
    } else {                                                            /*  如果是相对路径              */
        /*
         * cwd 要以 / 号开头和结尾
         */
        int32_t tid = gettid();
        snprintf(pathbuf, PATH_MAX, "%s%s", infos[tid].cwd, path);      /*  在前面加入当前工作目录      */
    }

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {
        return NULL;
    }

    tmp = strchr(pathbuf + 1, '/');                                     /*  查找挂载点名后的 / 号       */
    if (tmp == NULL) {                                                  /*  没有到                      */
        seterrno(EINVAL);
        return NULL;
    }
    if (tmp[1] == '\0') {                                               /*  不能是挂载点                */
        seterrno(EINVAL);
        return NULL;
    }

    *tmp = '\0';                                                        /*  暂时去掉 / 号               */
    point = mount_point_lookup(pathbuf);                                /*  查找挂载点                  */
    *tmp = '/';                                                         /*  恢复 / 号                   */

    *ppath = tmp;
    if (point == NULL) {
        seterrno(ENOENT);
    }
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
        seterrno(EINVAL);
        return NULL;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        strlcpy(pathbuf, path, PATH_MAX);
    } else {                                                            /*  如果是相对路径              */
        /*
         * cwd 要以 / 号开头和结尾
         */
        int32_t tid = gettid();
        snprintf(pathbuf, PATH_MAX, "%s%s", infos[tid].cwd, path);      /*  在前面加入当前工作目录      */
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
    if (point == NULL) {
        seterrno(ENOENT);
    }
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

#define vfs_file_begin(tid)                                                                               \
        mount_point_t *point;                                                                             \
        file_t *file;                                                                                     \
        vfs_info_t *info;                                                                                 \
                                                                                                          \
        if (fd < 0 || fd >= OPEN_MAX) {                                 /*  文件描述符合法性判断        */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        info = infos + tid;                                                                               \
        file = info->files[fd];                                         /*  获得文件结构                */\
        if (file == NULL || file->ref == 0) {                           /*  如果文件未打开              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        if (!(file->flags & VFS_FILE_TYPE_FILE)) {                      /*  如果文件结构不是文件        */\
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point                                             /*  获得挂载点                  */

#define vfs_file_end(tid)

/*
 * 打开文件
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

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
    }
                                                                        /*  查找一个空闲的文件描述符    */
    info = infos + gettid();
    for (fd = 0, file = info->files[0]; fd < OPEN_MAX; fd++, file = info->files[fd]) {
        if (file == NULL) {
            break;
        }
    }
    if (fd == OPEN_MAX) {                                               /*  没找到                      */
        seterrno(EMFILE);
        return -1;
    }

    file = file_alloc();                                                /*  分配一个空闲的文件结构      */
    if (file == NULL) {
        seterrno(EMFILE);
        return -1;
    }

    file->ctx    = NULL;                                                /*  初始化文件结构              */
    file->ctx1   = NULL;
    file->flags  = VFS_FILE_TYPE_FILE;                                  /*  文件结构类型文件            */
    file->flags |= (oflag & O_ACCMODE) + 1;                             /*  记录访问模式                */
    file->flags |= oflag & O_APPEND;                                    /*  记录追加模式                */
    file->point  = point;                                               /*  记录挂载点                  */

    if (point->fs->open == NULL) {
        seterrno(ENOSYS);
        file_free(file);
        return -1;
    }

    seterrno(0);
    ret = point->fs->open(point, file, filepath, oflag, mode);          /*  打开文件                    */
    if (ret < 0) {
        file_free(file);
        return -1;
    }

    info->files[fd] = file;

    return fd;                                                          /*  返回文件描述符              */
}

/*
 * 关闭文件
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
    ret = point->fs->close(point, file);
    if (ret == 0) {
        info->files[fd] = NULL;
        file_free(file);                                                /*  如果关闭成功, 释放文件结构  */
    }
    vfs_file_end(gettid());
    return ret;
}

/*
 * 关闭文件
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
    ret = point->fs->close(point, file);
    if (ret == 0) {
        info->files[fd] = NULL;
        file_free(file);                                                /*  如果关闭成功, 释放文件结构  */
    }
    vfs_file_end(tid);
    return ret;
}

/*
 * 控制文件
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
 * 获得文件状态
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
 * 判断文件是不是终端
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
 * 同步文件
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
 * 同步文件
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
 * 修改文件长度
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
 * 读文件
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
 * 写文件
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
 * 控制文件
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
 * 调整文件读写位置
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
 * 复制文件描述符
 */
int vfs_dup(int fd)
{
    file_t *temp;
    int i;

    vfs_file_begin(gettid());
                                                                        /*  查找一个空闲的文件描述符    */
    for (i = 0, temp = info->files[0]; i < OPEN_MAX; i++, temp = info->files[i]) {
        if (temp == NULL) {
            break;
        }
    }
    if (i == OPEN_MAX) {                                                /*  没找到                      */
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
 * 复制文件描述符到指定的文件描述符
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
 *                                          select 实现
 */
/*
 * 扫描文件
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
 * 将当前任务加入到文件的等待列表
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
 * 将当前任务从文件的等待列表中移除
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
 * 扫描文件集
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
 * 将当前任务从文件集的等待列表中移除
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
 * 将当前任务加入到文件集的等待列表
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
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
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
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
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
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
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
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
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
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
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
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
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
        seterrno(ENOSYS);
        return 0;
    }

    seterrno(0);
    ret = point->fs->sync(point);
    return ret;
}
/*********************************************************************************************************
 *                                          目录操作接口
 */

#define vfs_dir_begin(tid)                                                                                \
        mount_point_t *point;                                                                             \
        file_t *file;                                                                                     \
        vfs_info_t *info;                                                                                 \
                                                                                                          \
        if (fd < 1 || fd >= OPEN_MAX) {                                 /*  文件描述符合法性判断        */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        info = infos + tid;                                                                               \
        file = info->files[fd];                                         /*  获得文件结构                */\
        if (file == NULL || file->ref == 0) {                           /*  如果目录未打开              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        if (!(file->flags & VFS_FILE_TYPE_DIR)) {                       /*  如果文件结构不是目录        */\
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point                                             /*  获得挂载点                  */

#define vfs_dir_end(tid)

/*
 * 打开目录
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

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        return (DIR *)0;
    }
                                                                        /*  查找一个空闲的文件描述符    */
    info = infos + gettid();
    for (fd = 1, file = info->files[1]; fd < OPEN_MAX; fd++, file = info->files[fd]) {
        if (file == NULL) {
            break;
        }
    }
    if (fd == OPEN_MAX) {                                               /*  没找到                      */
        seterrno(EMFILE);
        return (DIR *)0;
    }

    file = file_alloc();                                                /*  分配一个空闲的文件结构      */
    if (file == NULL) {
        seterrno(EMFILE);
        return (DIR *)0;
    }

    file->ctx    = NULL;                                                /*  初始化文件结构              */
    file->ctx1   = NULL;
    file->flags  = VFS_FILE_TYPE_DIR;                                   /*  文件结构类型文件            */
    file->point  = point;                                               /*  记录挂载点                  */

    if (point->fs->opendir == NULL) {
        seterrno(ENOSYS);
        file_free(file);
        return (DIR *)0;
    }

    seterrno(0);
    ret = point->fs->opendir(point, file, filepath);                    /*  打开文件                    */
    if (ret < 0) {
        file_free(file);
        return (DIR *)0;
    }

    info->files[fd] = file;

    return (DIR *)fd;                                                   /*  返回文件描述符              */
}

/*
 * 关闭目录
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
    ret = point->fs->closedir(point, file);
    if (ret == 0) {
        info->files[fd] = NULL;
        file_free(file);                                                /*  如果关闭成功, 释放文件结构  */
    }
    vfs_dir_end(gettid());
    return ret;
}

/*
 * 关闭目录
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
    ret = point->fs->closedir(point, file);
    if (ret == 0) {
        info->files[fd] = NULL;
        file_free(file);                                                /*  如果关闭成功, 释放文件结构  */
    }
    vfs_dir_end(tid);
    return ret;
}

/*
 * 读目录项
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
 * 重置目录读点
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
 * 调整目录读点
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
 * 获得目录读点
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
 * 改变当前工作目录
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

    return ret;
}

/*
 * 获得当前工作目录
 */
char *vfs_getcwd(char *buf, size_t size)
{
    int32_t tid = gettid();

    if (buf != NULL) {
        strlcpy(buf, infos[tid].cwd, size);
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
 * 初始化任务的文件信息
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
 * 清理任务的文件信息
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
 * 根据文件描述符获得文件结构
 */
file_t *vfs_get_file(int fd)
{
    file_t *file;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */                                                              \
        return NULL;
    }

    file = infos[gettid()].files[fd];
    if (file == NULL || file->ref == 0) {                               /*  如果文件未打开              */
        return NULL;
    }

    if (!(file->flags & VFS_FILE_TYPE_FILE)) {                          /*  如果文件结构不是文件        */
        return NULL;
    }

    return file;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
