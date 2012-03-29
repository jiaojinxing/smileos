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
** Descriptions:            �����ļ�ϵͳ
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
#ifndef VFS_H_
#define VFS_H_

#include <sys/types.h>

struct stat;
struct dirent;

#include "vfs/types.h"

/*
 * ���ļ�
 */
int vfs_open(const char *path, int oflag, mode_t mode);

/*
 * �ر��ļ�
 */
int vfs_close(int fd);

/*
 * �����ļ�
 */
int vfs_fcntl(int fd, int cmd, void *arg);

/*
 * ����ļ�״̬
 */
int vfs_fstat(int fd, struct stat *buf);

/*
 * �ж��ļ��ǲ����ն�
 */
int vfs_isatty(int fd);

/*
 * ͬ���ļ�
 */
int vfs_fsync(int fd);

/*
 * ͬ���ļ�
 */
int vfs_fdatasync(int fd);

/*
 * �޸��ļ�����
 */
int vfs_ftruncate(int fd, off_t len);

/*
 * ���ļ�
 */
ssize_t vfs_read(int fd, void *buf, size_t len);

/*
 * д�ļ�
 */
ssize_t vfs_write(int fd, const void *buf, size_t len);

/*
 * �����ļ�
 */
int vfs_ioctl(int fd, int cmd, void *arg);

/*
 * �����ļ���дλ��
 */
off_t vfs_lseek(int fd, off_t offset, int whence);

/*********************************************************************************************************
 *                                          �ļ�ϵͳ�����ӿ�
 */
/*
 * ���ļ�����һ������
 */
int vfs_link(const char *path1, const char *path2);

/*
 * ������(Ҳ���ƶ�)�ļ�
 */
int vfs_rename(const char *old, const char *new);

/*
 * ����ļ�״̬
 */
int vfs_stat(const char *path, struct stat *buf);

/*
 * ɾ���ļ�
 */
int vfs_unlink(const char *path);

/*
 * ����Ŀ¼
 */
int vfs_mkdir(const char *path, mode_t mode);

/*
 * ɾ��Ŀ¼
 */
int vfs_rmdir(const char *path);

/*
 * �ж��Ƿ�ɷ���
 */
int vfs_access(const char *path, int amode);

/*
 * �޸��ļ�����
 */
int vfs_truncate(const char *path, off_t len);

/*
 * ͬ��
 */
int vfs_sync(const char *path);

/*********************************************************************************************************
 *                                          Ŀ¼�����ӿ�
 */
/*
 * ��Ŀ¼
 */
DIR *vfs_opendir(const char *path);

/*
 * �ر�Ŀ¼
 */
int vfs_closedir(DIR *dir);

/*
 * ��Ŀ¼��
 */
struct dirent *vfs_readdir(DIR *dir);

/*
 * ����Ŀ¼����
 */
int vfs_rewinddir(DIR *dir);

/*
 * ����Ŀ¼����
 */
int vfs_seekdir(DIR *dir, long loc);

/*
 * ���Ŀ¼����
 */
long vfs_telldir(DIR *dir);

/*
 * �ı䵱ǰ����Ŀ¼
 */
int vfs_chdir(const char *path);

/*
 * ��õ�ǰ����Ŀ¼
 */
char *vfs_getcwd(char *buf, size_t size);

/*
 * ��ʼ�������ļ�ϵͳ
 */
int vfs_init(void);

/*
 * �� PATH ǰ������ص���
 */
const char *vfs_path_add_mount_point(const char *path);

/*
 * ��ʼ��������ļ���Ϣ
 */
int vfs_task_file_info_init(pid_t tid);

/*
 * ����������ļ���Ϣ
 */
int vfs_task_file_info_cleanup(pid_t tid);

#endif                                                                  /*  VFS_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
