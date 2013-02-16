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
** File name:               vfs.h
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
#ifndef VFS_H_
#define VFS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

struct stat;
struct dirent;
struct timeval;

#include "vfs/types.h"

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
const char *vfs_path_add_mount_point(const char *path);
/*********************************************************************************************************
** Function name:           vfs_path_add_mount_point
** Descriptions:            正常化 PATH
** input parameters:        path                路径
**                          sprit_end           是否以 / 结尾
** output parameters:       path                正常化后的路径
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_path_normalization(char path[PATH_BUF_LEN], int sprit_end);
/*********************************************************************************************************
** 文件操作接口
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_open
** Descriptions:            打开文件
** input parameters:        path                文件路径
**                          oflag               标志
**                          mode                模式
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_open(const char *path, int oflag, mode_t mode);
/*********************************************************************************************************
** Function name:           vfs_close
** Descriptions:            关闭文件
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_close(int fd);
/*********************************************************************************************************
** Function name:           vfs_fcntl
** Descriptions:            控制文件
** input parameters:        fd                  文件描述符
**                          cmd                 命令
**                          arg                 参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fcntl(int fd, int cmd, int arg);
/*********************************************************************************************************
** Function name:           vfs_fstat
** Descriptions:            获得文件状态
** input parameters:        fd                  文件描述符
**                          buf                 文件状态缓冲
** output parameters:       buf                 文件状态
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fstat(int fd, struct stat *buf);
/*********************************************************************************************************
** Function name:           vfs_fstat
** Descriptions:            判断文件是不是终端
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_isatty(int fd);
/*********************************************************************************************************
** Function name:           vfs_fsync
** Descriptions:            同步文件
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fsync(int fd);
/*********************************************************************************************************
** Function name:           vfs_fdatasync
** Descriptions:            同步文件
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fdatasync(int fd);
/*********************************************************************************************************
** Function name:           vfs_ftruncate
** Descriptions:            修改文件长度
** input parameters:        fd                  文件描述符
**                          len                 新长度
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_ftruncate(int fd, off_t len);
/*********************************************************************************************************
** Function name:           vfs_read
** Descriptions:            读文件
** input parameters:        fd                  文件描述符
**                          buf                 数据缓冲区
**                          len                 新长度
** output parameters:       buf                 数据
** Returned value:          成功读取的字节数 OR -1
*********************************************************************************************************/
ssize_t vfs_read(int fd, void *buf, size_t len);
/*********************************************************************************************************
** Function name:           vfs_write
** Descriptions:            写文件
** input parameters:        fd                  文件描述符
**                          buf                 数据缓冲区
**                          len                 新长度
** output parameters:       NONE
** Returned value:          成功写入的字节数 OR -1
*********************************************************************************************************/
ssize_t vfs_write(int fd, const void *buf, size_t len);
/*********************************************************************************************************
** Function name:           vfs_ioctl
** Descriptions:            控制文件
** input parameters:        fd                  文件描述符
**                          cmd                 命令
**                          arg                 参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_ioctl(int fd, int cmd, void *arg);
/*********************************************************************************************************
** Function name:           vfs_lseek
** Descriptions:            调整文件读写位置
** input parameters:        fd                  文件描述符
**                          offset              偏移
**                          whence              调整的位置
** output parameters:       NONE
** Returned value:          新的读写位置 OR -1
*********************************************************************************************************/
off_t vfs_lseek(int fd, off_t offset, int whence);
/*********************************************************************************************************
** Function name:           vfs_dup
** Descriptions:            复制文件描述符
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          新的文件描述符 OR -1
*********************************************************************************************************/
int vfs_dup(int fd);
/*********************************************************************************************************
** Function name:           vfs_dup2
** Descriptions:            复制文件描述符到指定的文件描述符
** input parameters:        fd                  文件描述符
**                          to                  指定的文件描述符
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_dup2(int fd, int to);
/*********************************************************************************************************
** select
*********************************************************************************************************/
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
int vfs_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout);
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
int vfs_link(const char *path1, const char *path2);
/*********************************************************************************************************
** Function name:           vfs_rename
** Descriptions:            重命名(也可移动)文件
** input parameters:        old                 源文件 PATH
**                          _new                新文件 PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rename(const char *old, const char *_new);
/*********************************************************************************************************
** Function name:           vfs_stat
** Descriptions:            获得文件状态
** input parameters:        path                文件 PATH
**                          buf                 文件状态缓冲
** output parameters:       buf                 文件状态
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_stat(const char *path, struct stat *buf);
/*********************************************************************************************************
** Function name:           vfs_unlink
** Descriptions:            删除文件
** input parameters:        path                文件 PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_unlink(const char *path);
/*********************************************************************************************************
** Function name:           vfs_mkdir
** Descriptions:            创建目录
** input parameters:        path                目录 PATH
**                          mode                模式
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mkdir(const char *path, mode_t mode);
/*********************************************************************************************************
** Function name:           vfs_rmdir
** Descriptions:            删除目录
** input parameters:        path                目录 PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rmdir(const char *path);
/*********************************************************************************************************
** Function name:           vfs_access
** Descriptions:            判断是否可访问
** input parameters:        path                文件 PATH
**                          amode               访问模式
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_access(const char *path, int amode);
/*********************************************************************************************************
** Function name:           vfs_truncate
** Descriptions:            修改文件长度
** input parameters:        path                文件 PATH
**                          amode               新的文件长度
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_truncate(const char *path, off_t len);
/*********************************************************************************************************
** Function name:           vfs_truncate
** Descriptions:            同步
** input parameters:        path                文件 PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_sync(const char *path);
/*********************************************************************************************************
** 目录操作接口
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_opendir
** Descriptions:            打开目录
** input parameters:        path                目录 PATH
** output parameters:       NONE
** Returned value:          目录指针
*********************************************************************************************************/
DIR *vfs_opendir(const char *path);
/*********************************************************************************************************
** Function name:           vfs_closedir
** Descriptions:            关闭目录
** input parameters:        dir                 目录指针
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_closedir(DIR *dir);
/*********************************************************************************************************
** Function name:           vfs_readdir
** Descriptions:            读目录项
** input parameters:        dir                 目录指针
** output parameters:       NONE
** Returned value:          目录项
*********************************************************************************************************/
struct dirent *vfs_readdir(DIR *dir);
/*********************************************************************************************************
** Function name:           vfs_rewinddir
** Descriptions:            重置目录读点
** input parameters:        dir                 目录指针
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rewinddir(DIR *dir);
/*********************************************************************************************************
** Function name:           vfs_rewinddir
** Descriptions:            调整目录读点
** input parameters:        dir                 目录指针
**                          loc                 新的读点
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_seekdir(DIR *dir, long loc);
/*********************************************************************************************************
** Function name:           vfs_telldir
** Descriptions:            获得目录读点
** input parameters:        dir                 目录指针
** output parameters:       NONE
** Returned value:          当前读点
*********************************************************************************************************/
long vfs_telldir(DIR *dir);
/*********************************************************************************************************
** Function name:           vfs_chdir
** Descriptions:            改变当前工作目录
** input parameters:        path                目录 PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_chdir(const char *path);
/*********************************************************************************************************
** Function name:           vfs_getcwd
** Descriptions:            获得当前工作目录
** input parameters:        size                buf 大小
**                          buf                 当前工作目录缓冲
** output parameters:       buf                 当前工作目录
** Returned value:          当前工作目录
*********************************************************************************************************/
char *vfs_getcwd(char *buf, size_t size);
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
int vfs_mount(const char *point_name, const char *dev_name, const char *fs_name, const char *param);
/*********************************************************************************************************
** Function name:           vfs_mount_point_create
** Descriptions:            创建挂载点
** input parameters:        point_name          挂载点名
**                          fs                  文件系统
**                          dev                 设备
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mount_point_create(const char *point_name, file_system_t *fs, device_t *dev);
/*********************************************************************************************************
** Function name:           vfs_mkfs
** Descriptions:            格式化文件系统
** input parameters:        path                目录 PATH
**                          param               参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mkfs(const char *path, const char *param);
/*********************************************************************************************************
** Function name:           vfs_unmount
** Descriptions:            取消挂载文件系统
** input parameters:        path                目录 PATH
**                          param               参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_unmount(const char *path, const char *param);
/*********************************************************************************************************
** 其它接口
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_init
** Descriptions:            初始化虚拟文件系统
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_init(void);
/*********************************************************************************************************
** Function name:           vfs_process_init
** Descriptions:            初始化进程的文件信息
** input parameters:        pid                 进程 ID
**                          tid                 任务 ID
**                          open_max            OPEN_MAX
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_process_init(pid_t pid, pid_t tid, int open_max);
/*********************************************************************************************************
** Function name:           vfs_process_cleanup
** Descriptions:            清理进程的文件信息
** input parameters:        pid                 进程 ID
**                          tid                 任务 ID
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_process_cleanup(pid_t pid, pid_t tid);
/*********************************************************************************************************
** Function name:           vfs_get_file
** Descriptions:            根据文件描述符获得文件结构
** input parameters:        fd                  文件描述符
** output parameters:       NONE
** Returned value:          文件结构
*********************************************************************************************************/
file_t *vfs_get_file(int fd);
/*********************************************************************************************************
** Function name:           vfs_put_file
** Descriptions:            归还文件结构
** input parameters:        file                文件结构
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void vfs_put_file(file_t *file);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  VFS_H_                      */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
