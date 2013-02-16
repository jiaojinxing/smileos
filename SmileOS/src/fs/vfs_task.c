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
** File name:               vfs_task.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            虚拟文件系统的任务接口
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
** 配置
*********************************************************************************************************/
#define VFS_ALLOW_OPEN_MAX      1000
/*********************************************************************************************************
** Function name:           vfs_chdir
** Descriptions:            改变当前工作目录
** input parameters:        path                目录 PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_chdir(const char *path)
{
    char    pathbuf[PATH_BUF_LEN];
    int     ret;
    int     tid;

    if (path == NULL || strlen(path) > PATH_MAX - 1) {
        seterrno(EINVAL);
        return -1;
    }

    tid = gettid();

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        strlcpy(pathbuf, path, sizeof(pathbuf));
    } else {                                                            /*  如果是相对路径              */
        snprintf(pathbuf, sizeof(pathbuf), "%s%s", cwd[tid], path);
    }

    ret = vfs_path_normalization(pathbuf, TRUE);
    if (ret == 0) {
        DIR *dir = vfs_opendir(pathbuf);
        if (dir != NULL) {
            strlcpy(cwd[tid], pathbuf, PATH_MAX);
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
        strlcpy(buf, cwd[tid], size);
        return buf;
    } else {
        return strdup(cwd[tid]);
    }
}
/*********************************************************************************************************
** Function name:           vfs_process_init
** Descriptions:            初始化进程的文件信息
** input parameters:        pid                 进程 ID
**                          tid                 任务 ID
**                          open_max            OPEN_MAX
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_process_init(pid_t pid, pid_t tid, int open_max)
{
    vfs_info_t *info;

    if (pid < 0 || pid >= PROCESS_NR) {
        return -1;
    }

    if (tid < 0 || tid >= TASK_NR) {
        return -1;
    }

    if (open_max <= 0 || open_max > VFS_ALLOW_OPEN_MAX) {
        return -1;
    }

    cwd[tid] = kmalloc(PATH_MAX, GFP_KERNEL);
    if (cwd[tid] == NULL) {
        return -1;
    }

    strcpy(cwd[tid], "/");

    mutex_lock(&info_lock[pid], 0);

    if (infos[pid] == NULL) {
        info = kzalloc(sizeof(vfs_info_t) + sizeof(file_t *) * (open_max - 1), GFP_KERNEL);
        if (info == NULL) {
            mutex_unlock(&info_lock[pid]);
            kfree(cwd[tid]);
            cwd[tid] = NULL;
            return -1;
        }

        infos[pid] = info;

        info->open_max = open_max;

        atomic_set(&info->ref, 1);
    } else {
        info = infos[pid];
        atomic_inc(&info->ref);
    }

    mutex_unlock(&info_lock[pid]);

    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_process_cleanup
** Descriptions:            清理进程的文件信息
** input parameters:        pid                 进程 ID
**                          tid                 任务 ID
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_process_cleanup(pid_t pid, pid_t tid)
{
    vfs_info_t *info;
    file_t     *file;
    int         fd;

    if (pid < 0 || pid >= PROCESS_NR) {
        return -1;
    }

    if (tid < 0 || tid >= TASK_NR) {
        return -1;
    }

    mutex_lock(&info_lock[pid], 0);

    info = infos[pid];

    if (atomic_dec_and_test(&info->ref)) {
        for (fd = 0; fd < info->open_max; fd++) {
            file = info->files[fd];
            if (file != NULL) {
                if (file->type & VFS_FILE_TYPE_FILE) {
                    __vfs_close(pid, fd);
                } else if (file->type & VFS_FILE_TYPE_DIR) {
                    __vfs_closedir(pid, (DIR *)fd);
                }
            }
        }
        kfree(info);
        infos[pid] = NULL;
    }

    mutex_unlock(&info_lock[pid]);

    kfree(cwd[tid]);
    cwd[tid] = NULL;

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
