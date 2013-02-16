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
** File name:               vfs_file.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            虚拟文件系统的文件操作接口
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
** 文件操作接口
**
** 这里的宏挺恶心的, 但如果没这些宏, 这个文件要大三倍吧, 并且不利于统一更改
**
*********************************************************************************************************/
#define vfs_file_begin(pid)                                                                               \
        mount_point_t  *point;                                                                            \
        file_t         *file;                                                                             \
        vfs_info_t     *info;                                                                             \
                                                                                                          \
        mutex_lock(&info_lock[pid], 0);                                                                   \
                                                                                                          \
        info = infos[pid];                                                                                \
                                                                                                          \
        if (fd < 0 || fd >= info->open_max) {                           /*  文件描述符合法性判断        */\
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
        if (!(file->type & VFS_FILE_TYPE_FILE)) {                       /*  如果类型不是文件 　　       */\
            mutex_unlock(&file->lock);                                                                    \
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        point = file->point                                             /*  获得挂载点                  */

#define vfs_file_end(pid)                                                                                 \
        mutex_unlock(&file->lock)
/*********************************************************************************************************
** Function name:           vfs_open
** Descriptions:            打开文件
** input parameters:        path                文件路径
**                          oflag               标志
**                          mode                模式
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_open(const char *path, int oflag, mode_t mode)
{
    mount_point_t  *point;
    vfs_info_t     *info;
    file_t         *file;
    char            pathbuf[PATH_BUF_LEN];
    char           *filepath;
    int             fd;
    int             ret;
    int             pid = getpid();

    point = vfs_mount_point_lookup_ref(pathbuf, &filepath, path);       /*  查找挂载点                  */
    if (point == NULL) {
        return -1;
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
        return -1;
    }

    file = vfs_file_alloc();                                            /*  分配一个文件结构            */
    if (file == NULL) {
        mutex_unlock(&info_lock[pid]);
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return -1;
    }

    info->files[fd] = file;                                             /*  记录下来                    */

    mutex_lock(&file->lock, 0);                                         /*  锁上文件结构                */

    mutex_unlock(&info_lock[pid]);

    file->type   = VFS_FILE_TYPE_FILE;                                  /*  类型: 文件                  */
    file->flags  = (oflag + 1);                                         /*  打开标志                    */
    file->ctx    = NULL;                                                /*  上下文     　               */
    file->point  = point;                                               /*  挂载点                      */

    if (point->fs->open == NULL) {
        mutex_lock(&info_lock[pid], 0);
        info->files[fd] = NULL;
        mutex_unlock(&info_lock[pid]);
        vfs_file_free(file);
        atomic_dec(&point->ref);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->open(point, file, filepath, oflag, mode);         /*  打开文件                    */
    if (ret < 0) {
        mutex_lock(&info_lock[pid], 0);
        info->files[fd] = NULL;
        mutex_unlock(&info_lock[pid]);
        vfs_file_free(file);
        atomic_dec(&point->ref);
        return -1;
    }

    mutex_unlock(&file->lock);

    return fd;                                                          /*  返回文件描述符              */
}
/*********************************************************************************************************
** Function name:           vfs_close
** Descriptions:            关闭文件
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_close(int fd)
{
    int ret;
    pid_t pid = getpid();

    vfs_file_begin(pid);
    if (point->fs->close == NULL) {
        vfs_file_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    if (atomic_dec_and_test(&file->ref)) {
        ret = point->fs->close(point, file);
        if (ret == 0) {
            mutex_lock(&info_lock[pid], 0);
            info->files[fd] = NULL;
            mutex_unlock(&info_lock[pid]);
            vfs_file_free(file);
            atomic_dec(&point->ref);
            return ret;
        } else {
            atomic_inc(&file->ref);
        }
    } else {
        ret = 0;
    }
    vfs_file_end(pid);
    return ret;
}
/*********************************************************************************************************
** Function name:           __vfs_close
** Descriptions:            关闭文件
** input parameters:        pid                 任务 ID
**                          fd                  文件描述符
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int __vfs_close(pid_t pid, int fd)
{
    int ret;

    vfs_file_begin(pid);
    if (point->fs->close == NULL) {
        vfs_file_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    if (atomic_dec_and_test(&file->ref)) {
        ret = point->fs->close(point, file);
        if (ret == 0) {
            mutex_lock(&info_lock[pid], 0);
            info->files[fd] = NULL;
            mutex_unlock(&info_lock[pid]);
            vfs_file_free(file);
            atomic_dec(&point->ref);
            return ret;
        } else {
            atomic_inc(&file->ref);
        }
    } else {
        ret = 0;
    }
    vfs_file_end(pid);
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_fcntl
** Descriptions:            控制文件
** input parameters:        fd                  文件描述符
**                          cmd                 命令
**                          arg                 参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fcntl(int fd, int cmd, int arg)
{
    int ret;
    pid_t pid = getpid();

    vfs_file_begin(pid);
    if (point->fs->fcntl == NULL) {
        vfs_file_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->fcntl(point, file, cmd, arg);
    vfs_file_end(pid);
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
    pid_t pid = getpid();

    if (buf == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    {
        vfs_file_begin(pid);
        if (point->fs->fstat == NULL) {
            vfs_file_end(pid);
            seterrno(ENOSYS);
            return -1;
        }
        seterrno(0);
        ret = point->fs->fstat(point, file, buf);
        vfs_file_end(pid);
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
    pid_t pid = getpid();

    vfs_file_begin(pid);
    if (point->fs->isatty == NULL) {
        vfs_file_end(pid);
        seterrno(0);
        return 0;
    }
    seterrno(0);
    ret = point->fs->isatty(point, file);
    vfs_file_end(pid);
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
    pid_t pid = getpid();

    vfs_file_begin(pid);
    if (!(file->flags & FWRITE)) {
        vfs_file_end(pid);
        seterrno(0);
        return 0;
    }
    if (point->fs->fsync == NULL) {
        vfs_file_end(pid);
        seterrno(0);
        return 0;
    }
    seterrno(0);
    ret = point->fs->fsync(point, file);
    vfs_file_end(pid);
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
    pid_t pid = getpid();

    vfs_file_begin(pid);
    if (!(file->flags & FWRITE)) {
        vfs_file_end(pid);
        seterrno(0);
        return 0;
    }
    if (point->fs->fdatasync == NULL) {
        vfs_file_end(pid);
        seterrno(0);
        return 0;
    }
    seterrno(0);
    ret = point->fs->fdatasync(point, file);
    vfs_file_end(pid);
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
    pid_t pid = getpid();

    if (len < 0) {
        seterrno(EINVAL);
        return -1;
    }

    vfs_file_begin(pid);
    if (!(file->flags & FWRITE)) {
        vfs_file_end(pid);
        seterrno(EIO);
        return -1;
    }
    if (point->fs->ftruncate == NULL) {
        vfs_file_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->ftruncate(point, file, len);
    vfs_file_end(pid);
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
    pid_t pid = getpid();

    if (buf == NULL || len < 0) {
        seterrno(EINVAL);
        return -1;
    }

    if (len == 0) {
        seterrno(0);
        return 0;
    }

    {
        mount_point_t  *point;
        file_t         *file;
        vfs_info_t     *info;

        mutex_lock(&info_lock[pid], 0);

        info = infos[pid];

        if (fd < 0 || fd >= info->open_max) {                           /*  文件描述符合法性判断        */
            mutex_unlock(&info_lock[pid]);
            seterrno(EBADFD);
            return -1;
        }

        file = info->files[fd];                                         /*  获得文件结构                */

        mutex_unlock(&info_lock[pid]);

        if (file == NULL) {                                             /*  如果文件未打开              */
            seterrno(EBADFD);
            return -1;
        }

        if (mutex_lock(&file->lock, 0) < 0) {
            seterrno(EBADFD);
            return -1;
        }

        if (!(file->type & VFS_FILE_TYPE_FILE)) {                       /*  如果类型不是文件 　　       */
            mutex_unlock(&file->lock);
            seterrno(EFTYPE);
            return -1;
        }

        point = file->point;                                             /*  获得挂载点                  */

        if (!(file->flags & FREAD)) {
            vfs_file_end(pid);
            seterrno(EIO);
            return -1;
        }
        if (point->fs->read == NULL) {
            vfs_file_end(pid);
            seterrno(ENOSYS);
            return -1;
        }
        seterrno(0);
        slen = point->fs->read(point, file, buf, len);
        vfs_file_end(pid);
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
    pid_t pid = getpid();

    if (buf == NULL || len < 0) {
        seterrno(EINVAL);
        return -1;
    }

    if (len == 0) {
        seterrno(0);
        return 0;
    }

    {
        vfs_file_begin(pid);
        if (!(file->flags & FWRITE)) {
            vfs_file_end(pid);
            seterrno(EIO);
            return -1;
        }
        if (point->fs->write == NULL) {
            vfs_file_end(pid);
            seterrno(ENOSYS);
            return -1;
        }
        seterrno(0);
        slen = point->fs->write(point, file, buf, len);
        vfs_file_end(pid);
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
    pid_t pid = getpid();

    vfs_file_begin(pid);
    if (point->fs->ioctl == NULL) {
        vfs_file_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->ioctl(point, file, cmd, arg);
    vfs_file_end(pid);
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
    pid_t pid = getpid();

    vfs_file_begin(pid);
    if (point->fs->lseek == NULL) {
        vfs_file_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    offset = point->fs->lseek(point, file, offset, whence);
    vfs_file_end(pid);
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
    int     i;
    int     pid = getpid();
    file_t *temp;

    vfs_file_begin(pid);

    mutex_lock(&info_lock[pid], 0);

    for (i = 0; i < info->open_max; i++) {                              /*  查找一个空闲的文件描述符    */
        temp = info->files[i];
        if (temp == NULL) {
            break;
        }
    }
    if (i == info->open_max) {                                          /*  没找到                      */
        mutex_unlock(&info_lock[pid]);
        vfs_file_end(pid);
        seterrno(EMFILE);
        return -1;
    }

    info->files[i] = file;

    mutex_unlock(&info_lock[pid]);

    atomic_inc(&file->ref);

    atomic_inc(&point->ref);

    vfs_file_end(pid);

    seterrno(0);

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
    pid_t pid = getpid();

    vfs_file_begin(pid);

    ret = vfs_close(to);
    if (ret < 0) {
        vfs_file_end(pid);
        return ret;
    }

    mutex_lock(&info_lock[pid], 0);

    info->files[to] = file;

    mutex_unlock(&info_lock[pid]);

    atomic_inc(&file->ref);

    atomic_inc(&point->ref);

    vfs_file_end(pid);

    seterrno(0);

    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_scan_file
** Descriptions:            扫描文件
** input parameters:        fd                  文件描述符
**                          flags               标志
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_scan_file(int fd, int flags)
{
    int ret;
    pid_t pid = getpid();

    vfs_file_begin(pid);
    if (point->fs->scan == NULL) {
        vfs_file_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->scan(point, file, flags);
    vfs_file_end(pid);
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
int vfs_select_file(int fd, int flags)
{
    int ret;
    pid_t pid = getpid();

    vfs_file_begin(pid);
    if (point->fs->select == NULL) {
        vfs_file_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->select(point, file, flags);
    vfs_file_end(pid);
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
int vfs_unselect_file(int fd, int flags)
{
    int ret;
    pid_t pid = getpid();

    vfs_file_begin(pid);
    if (point->fs->unselect == NULL) {
        vfs_file_end(pid);
        seterrno(ENOSYS);
        return -1;
    }
    seterrno(0);
    ret = point->fs->unselect(point, file, flags);
    vfs_file_end(pid);
    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_get_file
** Descriptions:            根据文件描述符获得文件结构
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          文件结构 OR NULL
*********************************************************************************************************/
file_t *vfs_get_file(int fd)
{
    pid_t pid = getpid();

    vfs_file_begin(pid);

    return file;
}
/*********************************************************************************************************
** Function name:           vfs_put_file
** Descriptions:            归还文件结构
** input parameters:        file                文件结构
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void vfs_put_file(file_t *file)
{
    if (file != NULL && (file->type & VFS_FILE_TYPE_FILE)) {
        vfs_file_end(0);
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
