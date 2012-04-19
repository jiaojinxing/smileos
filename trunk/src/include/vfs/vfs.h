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

#include <sys/types.h>

struct stat;
struct dirent;
struct timeval;

#include "vfs/types.h"

/*
 * 打开文件
 */
int vfs_open(const char *path, int oflag, mode_t mode);

/*
 * 关闭文件
 */
int vfs_close(int fd);

/*
 * 控制文件
 */
int vfs_fcntl(int fd, int cmd, int arg);

/*
 * 获得文件状态
 */
int vfs_fstat(int fd, struct stat *buf);

/*
 * 判断文件是不是终端
 */
int vfs_isatty(int fd);

/*
 * 同步文件
 */
int vfs_fsync(int fd);

/*
 * 同步文件
 */
int vfs_fdatasync(int fd);

/*
 * 修改文件长度
 */
int vfs_ftruncate(int fd, off_t len);

/*
 * 读文件
 */
ssize_t vfs_read(int fd, void *buf, size_t len);

/*
 * 写文件
 */
ssize_t vfs_write(int fd, const void *buf, size_t len);

/*
 * 控制文件
 */
int vfs_ioctl(int fd, int cmd, void *arg);

/*
 * 调整文件读写位置
 */
off_t vfs_lseek(int fd, off_t offset, int whence);

/*
 * select
 */
int vfs_select(int nfds, fd_set *readfds, fd_set *writefds,
               fd_set *errorfds, struct timeval *timeout);

/*********************************************************************************************************
 *                                          文件系统操作接口
 */
/*
 * 给文件创建一个链接
 */
int vfs_link(const char *path1, const char *path2);

/*
 * 重命名(也可移动)文件
 */
int vfs_rename(const char *old, const char *new);

/*
 * 获得文件状态
 */
int vfs_stat(const char *path, struct stat *buf);

/*
 * 删除文件
 */
int vfs_unlink(const char *path);

/*
 * 创建目录
 */
int vfs_mkdir(const char *path, mode_t mode);

/*
 * 删除目录
 */
int vfs_rmdir(const char *path);

/*
 * 判断是否可访问
 */
int vfs_access(const char *path, int amode);

/*
 * 修改文件长度
 */
int vfs_truncate(const char *path, off_t len);

/*
 * 同步
 */
int vfs_sync(const char *path);

/*********************************************************************************************************
 *                                          目录操作接口
 */
/*
 * 打开目录
 */
DIR *vfs_opendir(const char *path);

/*
 * 关闭目录
 */
int vfs_closedir(DIR *dir);

/*
 * 读目录项
 */
struct dirent *vfs_readdir(DIR *dir);

/*
 * 重置目录读点
 */
int vfs_rewinddir(DIR *dir);

/*
 * 调整目录读点
 */
int vfs_seekdir(DIR *dir, long loc);

/*
 * 获得目录读点
 */
long vfs_telldir(DIR *dir);

/*
 * 改变当前工作目录
 */
int vfs_chdir(const char *path);

/*
 * 获得当前工作目录
 */
char *vfs_getcwd(char *buf, size_t size);

/*
 * 初始化虚拟文件系统
 */
int vfs_init(void);

/*
 * 在 PATH 前加入挂载点名
 */
const char *vfs_path_add_mount_point(const char *path);

/*
 * 初始化任务的文件信息
 */
int vfs_task_init(pid_t tid);

/*
 * 清理任务的文件信息
 */
int vfs_task_cleanup(pid_t tid);

/*
 * 根据文件描述符获得文件结构
 */
file_t *vfs_get_file(int fd);

#endif                                                                  /*  VFS_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
