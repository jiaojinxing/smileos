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
** File name:               vfs_common.h
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            虚拟文件系统的公共部分
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
#ifndef VFS_COMMON_H_
#define VFS_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

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
**
*********************************************************************************************************/
typedef struct {
    atomic_t                ref;                                        /*  引用计数                    */
    int                     open_max;                                   /*  OPEN_MAX                    */
    file_t                 *files[1];                                   /*  文件结构表                  */
} vfs_info_t;

#ifndef FS_VAR_INC
#define FS_VAR              extern
#else
#define FS_VAR
#endif

FS_VAR vfs_info_t          *infos[PROCESS_NR];                          /*  进程文件信息                */

FS_VAR mutex_t              info_lock[PROCESS_NR];                      /*  进程文件信息锁              */

FS_VAR char                *cwd[TASK_NR];                               /*  任务当前工作目录            */
/*********************************************************************************************************
** Function name:           vfs_file_alloc
** Descriptions:            释放文件结构
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          文件结构 OR NULL
*********************************************************************************************************/
file_t *vfs_file_alloc(void);
/*********************************************************************************************************
** Function name:           vfs_file_free
** Descriptions:            释放文件结构
** input parameters:        file                文件结构
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
void vfs_file_free(file_t *file);
/*********************************************************************************************************
** Function name:           __vfs_close
** Descriptions:            关闭文件
** input parameters:        pid                 任务 ID
**                          fd                  文件描述符
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int __vfs_close(pid_t pid, int fd);
/*********************************************************************************************************
** Function name:           __vfs_closedir
** Descriptions:            关闭目录
** input parameters:        pid                 任务 ID
**                          dir                 目录指针
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int __vfs_closedir(pid_t pid, DIR *dir);
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup
** Descriptions:            查找挂载点, PATH 不能是挂载点
** input parameters:        pathbuf             路径临时缓冲区
**                          path                路径
** output parameters:       ppath               指向去掉挂载点后的路径
** Returned value:          挂载点 OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path);
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup2
** Descriptions:            查找挂载点, PATH 可以是挂载点
** input parameters:        pathbuf             路径临时缓冲区
**                          path                路径
** output parameters:       ppath               指向去掉挂载点后的路径
** Returned value:          挂载点 OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup2(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path);
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup_ref
** Descriptions:            查找并引用挂载点, PATH 不能是挂载点
** input parameters:        pathbuf             路径临时缓冲区
**                          path                路径
** output parameters:       ppath               指向去掉挂载点后的路径
** Returned value:          挂载点 OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup_ref(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path);
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup2_ref
** Descriptions:            查找并引用挂载点, PATH 可以是挂载点
** input parameters:        pathbuf             路径临时缓冲区
**                          path                路径
** output parameters:       ppath               指向去掉挂载点后的路径
** Returned value:          挂载点 OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup2_ref(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path);
/*********************************************************************************************************
** Function name:           vfs_scan_file
** Descriptions:            扫描文件
** input parameters:        fd                  文件描述符
**                          flags               标志
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_scan_file(int fd, int flags);
/*********************************************************************************************************
** Function name:           vfs_select_file
** Descriptions:            将当前任务加入到文件的等待列表
** input parameters:        fd                  文件描述符
**                          flags               标志
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_select_file(int fd, int flags);
/*********************************************************************************************************
** Function name:           vfs_unselect_file
** Descriptions:            将当前任务从文件的等待列表中移除
** input parameters:        fd                  文件描述符
**                          flags               标志
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_unselect_file(int fd, int flags);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  VFS_COMMON_H_               */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
