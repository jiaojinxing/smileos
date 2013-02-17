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
** Descriptions:            �����ļ�ϵͳ�Ĺ�������
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-20
** Version:                 1.0.0
** Descriptions:            �����ļ�
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
    atomic_t            ref;                                            /*  ���ü���                    */
    int                 open_max;                                       /*  OPEN_MAX                    */
    file_t             *files[1];                                       /*  �ļ��ṹ��                  */
} vfs_info_t;

#ifndef FS_VAR_INC
#define FS_VAR          extern
#else
#define FS_VAR
#endif

FS_VAR vfs_info_t      *infos[PROCESS_NR];                              /*  �����ļ���Ϣ                */

FS_VAR mutex_t          info_lock[PROCESS_NR];                          /*  �����ļ���Ϣ��              */

FS_VAR char            *cwd[TASK_NR];                                   /*  ����ǰ����Ŀ¼            */
/*********************************************************************************************************
** Function name:           vfs_file_alloc
** Descriptions:            �ͷ��ļ��ṹ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          �ļ��ṹ OR NULL
*********************************************************************************************************/
file_t *vfs_file_alloc(void);
/*********************************************************************************************************
** Function name:           vfs_file_free
** Descriptions:            �ͷ��ļ��ṹ
** input parameters:        file                �ļ��ṹ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
void vfs_file_free(file_t *file);
/*********************************************************************************************************
** Function name:           __vfs_close
** Descriptions:            �ر��ļ�
** input parameters:        pid                 ���� ID
**                          fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int __vfs_close(pid_t pid, int fd);
/*********************************************************************************************************
** Function name:           __vfs_closedir
** Descriptions:            �ر�Ŀ¼
** input parameters:        pid                 ���� ID
**                          dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int __vfs_closedir(pid_t pid, DIR *dir);
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup
** Descriptions:            ���ҹ��ص�, PATH �����ǹ��ص�
** input parameters:        pathbuf             ·����ʱ������
**                          path                ·��
** output parameters:       ppath               ָ��ȥ�����ص���·��
** Returned value:          ���ص� OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path);
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup2
** Descriptions:            ���ҹ��ص�, PATH �����ǹ��ص�
** input parameters:        pathbuf             ·����ʱ������
**                          path                ·��
** output parameters:       ppath               ָ��ȥ�����ص���·��
** Returned value:          ���ص� OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup2(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path);
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup_ref
** Descriptions:            ���Ҳ����ù��ص�, PATH �����ǹ��ص�
** input parameters:        pathbuf             ·����ʱ������
**                          path                ·��
** output parameters:       ppath               ָ��ȥ�����ص���·��
** Returned value:          ���ص� OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup_ref(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path);
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup2_ref
** Descriptions:            ���Ҳ����ù��ص�, PATH �����ǹ��ص�
** input parameters:        pathbuf             ·����ʱ������
**                          path                ·��
** output parameters:       ppath               ָ��ȥ�����ص���·��
** Returned value:          ���ص� OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup2_ref(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path);
/*********************************************************************************************************
** Function name:           vfs_scan_file
** Descriptions:            ɨ���ļ�
** input parameters:        fd                  �ļ�������
**                          flags               ��־
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_scan_file(int fd, int flags);
/*********************************************************************************************************
** Function name:           vfs_select_file
** Descriptions:            ����ǰ������뵽�ļ��ĵȴ��б�
** input parameters:        fd                  �ļ�������
**                          flags               ��־
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_select_file(int fd, int flags);
/*********************************************************************************************************
** Function name:           vfs_unselect_file
** Descriptions:            ����ǰ������ļ��ĵȴ��б����Ƴ�
** input parameters:        fd                  �ļ�������
**                          flags               ��־
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
