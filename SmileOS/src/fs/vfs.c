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
**
** 因为在某 pthread 线程里调 write 等函数, 有可能会造成整个进程被挂起,
**
** 从而导致别的 pthread 得不到运行的机会.
**
** 为了解决这个问题, GNU/pth 库是这样做的, 以线程 pthread_abc 调用 write 函数为例:
**
** 1) 将文件设置为非阻塞模式;
**
** 2) 如果文件之前就是工作在非阻塞模式, 那么就调用 write, 如果因为收到 INT 信号而写入失败,
**    则它会继续尝试调用 write, 直至写入成功或出错.
**
** 3) 如果文件之前工作在阻塞模式, 那么 select 文件可写 0 秒;
**
** 4) 如果 select 文件可写, 那么就调用 write, 如果因为收到 INT 信号而写入失败,
**    则它会继续尝试调用 write, 直至写入成功或出错.
**
** 5) 如果 select 文件不可写, 那么等待一个 PTH_EVENT_FD|PTH_UNTIL_FD_WRITEABLE|PTH_MODE_STATIC 事件,
**    会切换回 pth_scheduler 线程;
**
** 6) pth_scheduler 线程会在没有其它 pthread 线程可运行时调用 pth_sched_eventmanager 函数;
**
** 7) pth_sched_eventmanager 函数处理所有的等待事件, 最终会 select 所有需要等待的文件可读可写出错
**    一个最小的等待时间;
**
** 8) pth_sched_eventmanager 函数会对 select 的结果进行判定,
**    以决定是否唤醒等待 PTH_EVENT_FD|PTH_UNTIL_FD_WRITEABLE|PTH_MODE_STATIC 事件的那个 pthread_abc 线程;
**
** 9) 如果线程 pthread_abc 获得事件, 那么就调用 write, 如果因为收到 INT 信号而写入失败,
**    则它会继续尝试调用 write, 直至写入成功或出错.
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
** 外部变量
*********************************************************************************************************/
/*
 * 挂载点管理锁
 */
extern mutex_t          point_mgr_lock;

/*
 * 根文件系统挂载点
 */
extern mount_point_t   *rootfs_point;

/*
 * 设备管理锁
 */
extern mutex_t          dev_mgr_lock;

/*
 * 文件系统管理锁
 */
extern mutex_t          fs_mgr_lock;
/*********************************************************************************************************
** 任务文件信息
*********************************************************************************************************/
typedef struct {
    file_t              files[OPEN_MAX];                                /*  文件结构表                  */
    char                cwd[PATH_MAX];                                  /*  当前工作目录                */
} vfs_info_t;

static vfs_info_t       infos[TASK_NR];
/*********************************************************************************************************
** 工具函数
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_path_add_mount_point
** Descriptions:            在 PATH 前加入挂载点名
** input parameters:        path                路径
** output parameters:       NONE
** Returned value:          加入挂载点名的 PATH
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
** Descriptions:            正常化 PATH
** input parameters:        path                路径
**                          sprit_end           是否以 / 结尾
** output parameters:       path                正常化后的路径
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_path_normalization(char path[PATH_MAX], int sprit_end)
{
    /*
     * 文本状态机! 向李先静老师致敬!
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

    for (p = path; (ch = *p) != '\0'; p++) {                            /*  处理所有文本                */
        switch (status) {
        case BEGIN:                                                     /*  开始状态                    */
            if (ch != '/') {                                            /*  如果不是以 / 号开始, 出错   */
                seterrno(EINVAL);
                return -1;
            }
            path[pos++] = ch;                                           /*  记录 / 号                   */
            status = WORKING;
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
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup
** Descriptions:            查找挂载点, PATH 不能是挂载点
** input parameters:        pathbuf             路径临时缓冲区
**                          path                路径
** output parameters:       ppath               指向去掉挂载点后的路径
** Returned value:          挂载点 OR NULL
*********************************************************************************************************/
static mount_point_t *vfs_mount_point_lookup(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t *point;
    char          *tmp;

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
        int tid = gettid();
        snprintf(pathbuf, PATH_MAX, "%s%s", infos[tid].cwd, path);      /*  在前面加入当前工作目录      */
    }

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {                   /*  正常化 PATH                 */
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

    *tmp  = '\0';                                                       /*  暂时去掉 / 号               */
    point = mount_point_lookup(pathbuf);                                /*  查找挂载点                  */
    *tmp  = '/';                                                        /*  恢复 / 号                   */

    *ppath = tmp;
    if (point == NULL) {
        seterrno(ENOENT);
    }
    return point;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup2
** Descriptions:            查找挂载点, PATH 可以是挂载点
** input parameters:        pathbuf             路径临时缓冲区
**                          path                路径
** output parameters:       ppath               指向去掉挂载点后的路径
** Returned value:          挂载点 OR NULL
*********************************************************************************************************/
static mount_point_t *vfs_mount_point_lookup2(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t  *point;
    char           *tmp;

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
        int tid = gettid();
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
            strcat(pathbuf + 1, "/");
            tmp = strchr(pathbuf + 1, '/');
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

/*********************************************************************************************************
** 文件操作接口
*********************************************************************************************************/
#define vfs_file_begin(tid)                                                                               \
        mount_point_t  *point;                                                                            \
        file_t         *file;                                                                             \
        vfs_info_t     *info;                                                                             \
                                                                                                          \
        if (fd < 0 || fd >= OPEN_MAX) {                                 /*  文件描述符合法性判断        */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        info = infos + tid;                                                                               \
        file = &info->files[fd];                                        /*  获得文件结构                */\
        if (file->flags == VFS_FILE_TYPE_FREE) {                        /*  如果文件未打开              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        if (!(file->flags & VFS_FILE_TYPE_FILE)) {                      /*  如果文件结构不是文件        */\
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point                                             /*  获得挂载点                  */

#define vfs_file_end(tid)
/*********************************************************************************************************
** Function name:           vfs_open
** Descriptions:            打开文件
** input parameters:        path                文件路径
**                          oflag               标志
**                          mode                模式
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

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);
                                                                        /*  查找一个空闲的文件描述符    */
    info = infos + gettid();
    for (fd = 0, file = &info->files[0]; fd < OPEN_MAX; fd++, file++) {
        if (file->flags == VFS_FILE_TYPE_FREE) {
            break;
        }
    }
    if (fd == OPEN_MAX) {                                               /*  没找到                      */
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return -1;
    }

    file->ctx    = NULL;                                                /*  初始化文件结构              */
    file->ctx1   = NULL;
    file->flags  = VFS_FILE_TYPE_FILE | (oflag + 1);                    /*  文件结构类型文件            */
    file->point  = point;                                               /*  记录挂载点                  */

    if (point->fs->open == NULL) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);

    ret = point->fs->open(point, file, filepath, oflag, mode);         /*  打开文件                    */
    if (ret < 0) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
        return -1;
    }

    return fd;                                                          /*  返回文件描述符              */
}
/*********************************************************************************************************
** Function name:           vfs_close
** Descriptions:            关闭文件
** input parameters:        fd                  文件描述符
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
** Descriptions:            关闭文件
** input parameters:        tid                 任务 ID
**                          fd                  文件描述符
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
** Descriptions:            控制文件
** input parameters:        fd                  文件描述符
**                          cmd                 命令
**                          arg                 参数
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
** Descriptions:            获得文件状态
** input parameters:        fd                  文件描述符
**                          buf                 文件状态缓冲
** output parameters:       buf                 文件状态
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
** Descriptions:            判断文件是不是终端
** input parameters:        fd                  文件描述符
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
** Descriptions:            同步文件
** input parameters:        fd                  文件描述符
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
** Descriptions:            同步文件
** input parameters:        fd                  文件描述符
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
** Descriptions:            修改文件长度
** input parameters:        fd                  文件描述符
**                          len                 新长度
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
** Descriptions:            读文件
** input parameters:        fd                  文件描述符
**                          buf                 数据缓冲区
**                          len                 新长度
** output parameters:       buf                 数据
** Returned value:          成功读取的字节数 OR -1
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
** Descriptions:            写文件
** input parameters:        fd                  文件描述符
**                          buf                 数据缓冲区
**                          len                 新长度
** output parameters:       NONE
** Returned value:          成功写入的字节数 OR -1
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
** Descriptions:            控制文件
** input parameters:        fd                  文件描述符
**                          cmd                 命令
**                          arg                 参数
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
** Descriptions:            调整文件读写位置
** input parameters:        fd                  文件描述符
**                          offset              偏移
**                          whence              调整的位置
** output parameters:       NONE
** Returned value:          新的读写位置 OR -1
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
** Descriptions:            复制文件描述符
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          新的文件描述符 OR -1
*********************************************************************************************************/
int vfs_dup(int fd)
{
    file_t *temp;
    int     i;

    vfs_file_begin(gettid());

    atomic_inc(&point->ref);
                                                                        /*  查找一个空闲的文件描述符    */
    for (i = 0, temp = &info->files[0]; i < OPEN_MAX; i++, temp++) {
        if (temp->flags == VFS_FILE_TYPE_FREE) {
            break;
        }
    }
    if (i == OPEN_MAX) {                                                /*  没找到                      */
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
** Descriptions:            复制文件描述符到指定的文件描述符
** input parameters:        fd                  文件描述符
**                          to                  指定的文件描述符
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
** select 实现
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_scan_file
** Descriptions:            扫描文件
** input parameters:        fd                  文件描述符
**                          flags               标志
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
** Descriptions:            将当前任务加入到文件的等待列表
** input parameters:        fd                  文件描述符
**                          flags               标志
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
** Descriptions:            将当前任务从文件的等待列表中移除
** input parameters:        fd                  文件描述符
**                          flags               标志
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
** Descriptions:            扫描文件集
** input parameters:        nfds                文件描述符范围
**                          readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** output parameters:       readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** Returned value:          文件集总共设置了多少个位
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
** Descriptions:            将当前任务从文件集的等待列表中移除
** input parameters:        nfds                文件描述符范围
**                          readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
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
** Descriptions:            将当前任务加入到文件集的等待列表
** input parameters:        nfds                文件描述符范围
**                          readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
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
** Descriptions:            将当前任务加入到文件集的等待列表
** input parameters:        nfds                文件描述符范围
**                          readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** output parameters:       readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** Returned value:          文件集总共设置了多少个位 OR -1
*********************************************************************************************************/
int vfs_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
{
    int         resume_type;
    int         nset;
    int         ret;
    uint32_t    reg;

    if (nfds < 0 || nfds > FD_SETSIZE) {                                /*  检查 nfds                   */
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

    if (timeout != NULL) {                                              /*  检查 timeout                */
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

        if (timeout->tv_sec > 31 * 24 * 60 * 60) {                      /*  修正 timeout                */
            timeout->tv_sec = 31 * 24 * 60 * 60;
        }
    }

    if (nfds == 0) {                                                    /*  nfds 为 0 的情况            */
        if (timeout != NULL) {                                          /*  timeout 不为 0, 即只延时    */
            if (timeout->tv_sec != 0 || timeout->tv_usec != 0) {
                usleep(timeout->tv_sec * 1000000 + timeout->tv_usec);   /*  延时                        */
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
        } else {                                                        /*  参数错误                    */
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
    nset = vfs_select_scan(nfds, readfds, writefds, errorfds);          /*  扫描文件集                  */
    if (nset > 0) {                                                     /*  有文件在文件集中置位了      */
        interrupt_resume(reg);
        seterrno(0);
        return nset;
    } else if (nset < 0) {                                              /*  出错了                      */
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
     * 无文件在文件集中置位
     */
                                                                        /*  上层不想休眠                */
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
     * 将当前任务加入到文件集的等待列表
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

    if (timeout == NULL) {                                              /*  上层要求等待                */
        current->delay   = 0;
        current->status  = TASK_SUSPEND;
    } else {                                                            /*  上层要求休眠                */
        current->delay   = timeout->tv_sec  * TICK_PER_SECOND +
                           timeout->tv_usec * TICK_PER_SECOND / 1000000;
        current->status  = TASK_SLEEPING;
    }
    current->resume_type = TASK_RESUME_UNKNOW;

    schedule();                                                         /*  释放 CPU 使用               */

    /*
     * 将当前任务从文件集的等待列表中移除
     */
    vfs_select_unselect(nfds, readfds, writefds, errorfds);

    current->delay       = 0;
    resume_type          = current->resume_type;
    current->resume_type = TASK_RESUME_UNKNOW;

    if (resume_type & TASK_RESUME_SELECT_EVENT) {                       /*  因 select 事件恢复          */
        nset = vfs_select_scan(nfds, readfds, writefds, errorfds);      /*  扫描文件集                  */
        interrupt_resume(reg);
        if (nset > 0) {                                                 /*  有文件在文件集中置位了      */
            seterrno(0);
            return nset;
        }

        if (nset == 0) {                                                /*  无文件在文件集中置位了      */
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
    } else {                                                            /*  因 select 事件恢复          */
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
** 目录操作接口
*********************************************************************************************************/
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
        file = &info->files[fd];                                        /*  获得文件结构                */\
        if (file->flags == VFS_FILE_TYPE_FREE) {                        /*  如果目录未打开              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        if (!(file->flags & VFS_FILE_TYPE_DIR)) {                       /*  如果文件结构不是目录        */\
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
        point = file->point                                             /*  获得挂载点                  */

#define vfs_dir_end(tid)
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
    char            pathbuf[PATH_MAX];
    char           *filepath;
    int             fd;
    int             ret;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        return (DIR *)0;
    }

    atomic_inc(&point->ref);

    mutex_unlock(&point_mgr_lock);
                                                                        /*  查找一个空闲的文件描述符    */
    info = infos + gettid();
    for (fd = 1, file = &info->files[1]; fd < OPEN_MAX; fd++, file++) {
        if (file->flags == 0) {
            break;
        }
    }
    if (fd == OPEN_MAX) {                                               /*  没找到                      */
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return (DIR *)0;
    }

    file->ctx    = NULL;                                                /*  初始化文件结构              */
    file->ctx1   = NULL;
    file->flags  = VFS_FILE_TYPE_DIR;                                   /*  文件结构类型文件            */
    file->point  = point;                                               /*  记录挂载点                  */

    if (point->fs->opendir == NULL) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
        seterrno(ENOSYS);
        return (DIR *)0;
    }

    seterrno(0);
    ret = point->fs->opendir(point, file, filepath);                    /*  打开文件                    */
    if (ret < 0) {
        atomic_dec(&point->ref);
        file->flags = VFS_FILE_TYPE_FREE;
        return (DIR *)0;
    }

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
** Descriptions:            关闭目录
** input parameters:        tid                 任务 ID
**                          dir                 目录指针
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
** Descriptions:            读目录项
** input parameters:        dir                 目录指针
** output parameters:       entry               目录项
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
** Descriptions:            获得目录读点
** input parameters:        dir                 目录指针
** output parameters:       NONE
** Returned value:          当前读点
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
** 文件系统操作接口
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_link
** Descriptions:            给文件创建一个链接
** input parameters:        path1               文件 PATH
**                          path2               新文件 PATH
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

    point1 = vfs_mount_point_lookup(pathbuf1, &filepath1, path1);       /*  查找挂载点                  */
    if (point1 == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    point2 = vfs_mount_point_lookup(pathbuf2, &filepath2, path2);       /*  查找挂载点                  */
    if (point2 == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    if (point2 != point1) {                                             /*  两个挂载点必须要相同        */
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
** Descriptions:            重命名(也可移动)文件
** input parameters:        old                 源文件 PATH
**                          _new                新文件 PATH
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

    point1 = vfs_mount_point_lookup(pathbuf1, &filepath1, old);         /*  查找挂载点                  */
    if (point1 == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    point2 = vfs_mount_point_lookup(pathbuf2, &filepath2, _new);        /*  查找挂载点                  */
    if (point2 == NULL) {
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    if (point2 != point1) {                                             /*  两个挂载点必须要相同        */
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
** Descriptions:            获得文件状态
** input parameters:        path                文件 PATH
**                          buf                 文件状态缓冲
** output parameters:       buf                 文件状态
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

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
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
** Descriptions:            删除文件
** input parameters:        path                文件 PATH
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

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
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
** Descriptions:            创建目录
** input parameters:        path                目录 PATH
**                          mode                模式
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

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
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
** Descriptions:            删除目录
** input parameters:        path                目录 PATH
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

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
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
** Descriptions:            判断是否可访问
** input parameters:        path                文件 PATH
**                          amode               访问模式
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

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
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
** Descriptions:            修改文件长度
** input parameters:        path                文件 PATH
**                          amode               新的文件长度
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

    point = vfs_mount_point_lookup(pathbuf, &filepath, path);           /*  查找挂载点                  */
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
** Descriptions:            同步
** input parameters:        path                文件 PATH
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

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
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
** Descriptions:            格式化文件系统
** input parameters:        path                目录 PATH
**                          param               参数
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

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
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
    ret = point->fs->mkfs(point, param);                                /*  格式化文件系统              */

    mutex_unlock(&point_mgr_lock);

    return ret;
}
/*********************************************************************************************************
** 挂载点接口
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_mount
** Descriptions:            挂载文件系统
** input parameters:        point_name          挂载点名
**                          dev_name            设备名
**                          fs_name             文件系统名
**                          param               参数
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
    point = mount_point_lookup(point_name);                             /*  查找挂载点                  */
    if (point == NULL) {                                                /*  没找到                      */

        mutex_lock(&fs_mgr_lock, 0);

        fs = file_system_lookup(fs_name);                               /*  查找文件系统                */
        if (fs != NULL) {

            atomic_inc(&fs->ref);

            mutex_unlock(&fs_mgr_lock);

            mutex_lock(&dev_mgr_lock, 0);
            dev = device_lookup(dev_name);                              /*  查找设备                    */
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

            if (point_name != NULL) {                                   /*  有指定挂载点名              */
                point = kmalloc(sizeof(mount_point_t), GFP_KERNEL);     /*  分配挂载点                  */
                if (point != NULL) {
                    if (point_name[0] == '/') {                         /*  保证挂载点以 / 号开始       */
                        strlcpy(point->name, point_name, sizeof(point->name));
                    } else {
                        snprintf(point->name, sizeof(point->name), "/%s", point_name);
                    }

                    if (point->name[1] != '\0') {                       /*  如果不是根文件系统          */
                        if (strchr(point->name + 1, '/') != NULL) {     /*  保证挂载点不能再出现 / 号   */
                                                                        /*  因为我不知道 / 号还有几个   */
                            mutex_unlock(&point_mgr_lock);
                            if (dev != NULL) {
                                atomic_dec(&dev->ref);
                            }
                            atomic_dec(&fs->ref);
                            kfree(point);
                            return -1;                                  /*  所以当作出错来处理          */
                        }
                    } else {
                        rootfs_point = point;
                    }

                    point->fs  = fs;
                    point->dev = dev;
                    point->ctx = NULL;
                    atomic_set(&point->ref, 0);

                    ret = fs->mount(point, dev, dev_name, param);       /*  挂载                        */
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
                        mount_point_install(point);                     /*  安装挂载点                  */
                        mutex_unlock(&point_mgr_lock);
                        return 0;
                    }
                }
            } else {                                                    /*  无指定挂载点名, 挂载时      */
                                                                        /*  用分区名创建挂载点          */
                ret = fs->mount(NULL, dev, dev_name, param);            /*  挂载                        */
                if (ret < 0) {
                    mutex_unlock(&point_mgr_lock);
                    if (dev != NULL) {
                        atomic_dec(&dev->ref);
                    }
                    atomic_dec(&fs->ref);
                    return -1;
                } else {
                    mutex_unlock(&point_mgr_lock);
                    atomic_dec(&fs->ref);                               /*  创建挂载点会增加文件系统引用*/
                    return 0;                                           /*  这里会减少文件系统引用      */
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
** Descriptions:            创建挂载点
** input parameters:        point_name          挂载点名
**                          fs                  文件系统
**                          dev                 设备
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mount_point_create(const char *point_name, file_system_t *fs, device_t *dev)
{
    mount_point_t  *point;

    if (point_name == NULL || fs == NULL) {
        return -1;
    }

    point = kmalloc(sizeof(mount_point_t), GFP_KERNEL);                 /*  分配挂载点                  */
    if (point != NULL) {
        if (point_name[0] == '/') {                                     /*  保证挂载点以 / 号开始       */
            strlcpy(point->name, point_name, sizeof(point->name));
        } else {
            snprintf(point->name, sizeof(point->name), "/%s", point_name);
        }

        if (point->name[1] != '\0') {                                   /*  如果不是根文件系统          */
            if (strchr(point->name + 1, '/') != NULL) {                 /*  保证挂载点不能再出现 / 号   */
                kfree(point);
                return -1;                                              /*  所以当作出错来处理          */
            }
        } else {
            kfree(point);
            return -1;
        }

        atomic_inc(&fs->ref);                                           /*  增加文件系统引用            */
        if (dev != NULL) {
            atomic_inc(&dev->ref);                                      /*  增加设备引用                */
        }

        point->fs  = fs;
        point->dev = dev;
        point->ctx = NULL;
        atomic_set(&point->ref, 0);

        mount_point_install(point);                                     /*  安装挂载点                  */

        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           vfs_unmount
** Descriptions:            取消挂载文件系统
** input parameters:        path                目录 PATH
**                          param               参数
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

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
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
    ret = point->fs->unmount(point, param);                             /*  取消挂载文件系统            */
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
** 任务接口
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_task_init
** Descriptions:            初始化任务的文件信息
** input parameters:        tid                 任务 ID
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
** Descriptions:            清理任务的文件信息
** input parameters:        tid                 任务 ID
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
** 其它接口
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_chdir
** Descriptions:            改变当前工作目录
** input parameters:        path                目录 PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_chdir(const char *path)
{
    char    pathbuf[PATH_MAX];
    int     ret;
    int     tid;

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
        } else {
            ret = -1;
        }
    }

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_getcwd
** Descriptions:            获得当前工作目录
** input parameters:        size                buf 大小
**                          buf                 当前工作目录缓冲
** output parameters:       buf                 当前工作目录
** Returned value:          当前工作目录
*********************************************************************************************************/
char *vfs_getcwd(char *buf, size_t size)
{
    int tid = gettid();

    if (buf != NULL) {
        strlcpy(buf, infos[tid].cwd, size);
    }
    return infos[tid].cwd;
}
/*********************************************************************************************************
** Function name:           vfs_get_file
** Descriptions:            根据文件描述符获得文件结构
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          文件结构
*********************************************************************************************************/
file_t *vfs_get_file(int fd)
{
    file_t *file;

    if (fd < 0 || fd >= OPEN_MAX) {                                     /*  文件描述符合法性判断        */                                                              \
        return NULL;
    }

    file = &infos[gettid()].files[fd];
    if (file->flags == VFS_FILE_TYPE_FREE) {                            /*  如果文件未打开              */
        return NULL;
    }

    if (!(file->flags & VFS_FILE_TYPE_FILE)) {                          /*  如果文件结构不是文件        */
        return NULL;
    }

    return file;
}
/*********************************************************************************************************
** Function name:           vfs_init
** Descriptions:            初始化虚拟文件系统
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

    extern file_system_t yaffs1;
    file_system_install(&yaffs1);

    extern file_system_t yaffs2;
    file_system_install(&yaffs2);

    vfs_mount("/",    NULL, "rootfs", NULL);

    vfs_mount("/dev", NULL, "devfs", NULL);

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
