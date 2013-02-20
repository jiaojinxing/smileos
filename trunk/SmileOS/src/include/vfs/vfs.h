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

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

struct stat;
struct dirent;
struct timeval;

#include "vfs/types.h"

/*********************************************************************************************************
** ���ߺ���
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_path_add_mount_point
** Descriptions:            �� PATH ǰ������ص���
** input parameters:        path                ·��
** output parameters:       NONE
** Returned value:          ������ص����� PATH
*********************************************************************************************************/
const char *vfs_path_add_mount_point(const char *path);
/*********************************************************************************************************
** Function name:           vfs_path_add_mount_point
** Descriptions:            ������ PATH
** input parameters:        path                ·��
**                          sprit_end           �Ƿ��� / ��β
** output parameters:       path                ���������·��
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_path_normalization(char *path, int sprit_end);
/*********************************************************************************************************
** Function name:           vfs_path_format
** Descriptions:            ��ʽ�� PATH
** input parameters:        path                ·��
** output parameters:       pathbuf             ��ʽ�����·��
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_path_format(char *pathbuf, const char *path);
/*********************************************************************************************************
** �ļ������ӿ�
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_open
** Descriptions:            ���ļ�
** input parameters:        path                �ļ�·��
**                          oflag               ��־
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_open(const char *path, int oflag, mode_t mode);
/*********************************************************************************************************
** Function name:           vfs_close
** Descriptions:            �ر��ļ�
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_close(int fd);
/*********************************************************************************************************
** Function name:           vfs_fcntl
** Descriptions:            �����ļ�
** input parameters:        fd                  �ļ�������
**                          cmd                 ����
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fcntl(int fd, int cmd, int arg);
/*********************************************************************************************************
** Function name:           vfs_fstat
** Descriptions:            ����ļ�״̬
** input parameters:        fd                  �ļ�������
**                          buf                 �ļ�״̬����
** output parameters:       buf                 �ļ�״̬
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fstat(int fd, struct stat *buf);
/*********************************************************************************************************
** Function name:           vfs_fstat
** Descriptions:            �ж��ļ��ǲ����ն�
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_isatty(int fd);
/*********************************************************************************************************
** Function name:           vfs_fsync
** Descriptions:            ͬ���ļ�
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fsync(int fd);
/*********************************************************************************************************
** Function name:           vfs_fdatasync
** Descriptions:            ͬ���ļ�
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_fdatasync(int fd);
/*********************************************************************************************************
** Function name:           vfs_ftruncate
** Descriptions:            �޸��ļ�����
** input parameters:        fd                  �ļ�������
**                          len                 �³���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_ftruncate(int fd, off_t len);
/*********************************************************************************************************
** Function name:           vfs_read
** Descriptions:            ���ļ�
** input parameters:        fd                  �ļ�������
**                          buf                 ���ݻ�����
**                          len                 �³���
** output parameters:       buf                 ����
** Returned value:          �ɹ���ȡ���ֽ��� OR -1
*********************************************************************************************************/
ssize_t vfs_read(int fd, void *buf, size_t len);
/*********************************************************************************************************
** Function name:           vfs_write
** Descriptions:            д�ļ�
** input parameters:        fd                  �ļ�������
**                          buf                 ���ݻ�����
**                          len                 �³���
** output parameters:       NONE
** Returned value:          �ɹ�д����ֽ��� OR -1
*********************************************************************************************************/
ssize_t vfs_write(int fd, const void *buf, size_t len);
/*********************************************************************************************************
** Function name:           vfs_ioctl
** Descriptions:            �����ļ�
** input parameters:        fd                  �ļ�������
**                          cmd                 ����
**                          arg                 ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_ioctl(int fd, int cmd, void *arg);
/*********************************************************************************************************
** Function name:           vfs_lseek
** Descriptions:            �����ļ���дλ��
** input parameters:        fd                  �ļ�������
**                          offset              ƫ��
**                          whence              ������λ��
** output parameters:       NONE
** Returned value:          �µĶ�дλ�� OR -1
*********************************************************************************************************/
off_t vfs_lseek(int fd, off_t offset, int whence);
/*********************************************************************************************************
** Function name:           vfs_dup
** Descriptions:            �����ļ�������
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          �µ��ļ������� OR -1
*********************************************************************************************************/
int vfs_dup(int fd);
/*********************************************************************************************************
** Function name:           vfs_dup2
** Descriptions:            �����ļ���������ָ�����ļ�������
** input parameters:        fd                  �ļ�������
**                          to                  ָ�����ļ�������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_dup2(int fd, int to);
/*********************************************************************************************************
** select
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_select
** Descriptions:            ����ǰ������뵽�ļ����ĵȴ��б�
** input parameters:        nfds                �ļ���������Χ
**                          readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** output parameters:       readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** Returned value:          �ļ����ܹ������˶��ٸ�λ OR -1
*********************************************************************************************************/
int vfs_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout);
/*********************************************************************************************************
** �ļ�ϵͳ�����ӿ�
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_link
** Descriptions:            ���ļ�����һ������
** input parameters:        path1               �ļ� PATH
**                          path2               ���ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_link(const char *path1, const char *path2);
/*********************************************************************************************************
** Function name:           vfs_rename
** Descriptions:            ������(Ҳ���ƶ�)�ļ�
** input parameters:        old                 Դ�ļ� PATH
**                          _new                ���ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rename(const char *old, const char *_new);
/*********************************************************************************************************
** Function name:           vfs_stat
** Descriptions:            ����ļ�״̬
** input parameters:        path                �ļ� PATH
**                          buf                 �ļ�״̬����
** output parameters:       buf                 �ļ�״̬
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_stat(const char *path, struct stat *buf);
/*********************************************************************************************************
** Function name:           vfs_unlink
** Descriptions:            ɾ���ļ�
** input parameters:        path                �ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_unlink(const char *path);
/*********************************************************************************************************
** Function name:           vfs_mkdir
** Descriptions:            ����Ŀ¼
** input parameters:        path                Ŀ¼ PATH
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mkdir(const char *path, mode_t mode);
/*********************************************************************************************************
** Function name:           vfs_rmdir
** Descriptions:            ɾ��Ŀ¼
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rmdir(const char *path);
/*********************************************************************************************************
** Function name:           vfs_access
** Descriptions:            �ж��Ƿ�ɷ���
** input parameters:        path                �ļ� PATH
**                          amode               ����ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_access(const char *path, int amode);
/*********************************************************************************************************
** Function name:           vfs_truncate
** Descriptions:            �޸��ļ�����
** input parameters:        path                �ļ� PATH
**                          amode               �µ��ļ�����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_truncate(const char *path, off_t len);
/*********************************************************************************************************
** Function name:           vfs_truncate
** Descriptions:            ͬ��
** input parameters:        path                �ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_sync(const char *path);
/*********************************************************************************************************
** Ŀ¼�����ӿ�
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_opendir
** Descriptions:            ��Ŀ¼
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          Ŀ¼ָ��
*********************************************************************************************************/
DIR *vfs_opendir(const char *path);
/*********************************************************************************************************
** Function name:           vfs_closedir
** Descriptions:            �ر�Ŀ¼
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_closedir(DIR *dir);
/*********************************************************************************************************
** Function name:           vfs_readdir
** Descriptions:            ��Ŀ¼��
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          Ŀ¼��
*********************************************************************************************************/
struct dirent *vfs_readdir(DIR *dir);
/*********************************************************************************************************
** Function name:           vfs_rewinddir
** Descriptions:            ����Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rewinddir(DIR *dir);
/*********************************************************************************************************
** Function name:           vfs_rewinddir
** Descriptions:            ����Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
**                          loc                 �µĶ���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_seekdir(DIR *dir, long loc);
/*********************************************************************************************************
** Function name:           vfs_telldir
** Descriptions:            ���Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          ��ǰ����
*********************************************************************************************************/
long vfs_telldir(DIR *dir);
/*********************************************************************************************************
** Function name:           vfs_chdir
** Descriptions:            �ı䵱ǰ����Ŀ¼
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_chdir(const char *path);
/*********************************************************************************************************
** Function name:           vfs_getcwd
** Descriptions:            ��õ�ǰ����Ŀ¼
** input parameters:        size                buf ��С
**                          buf                 ��ǰ����Ŀ¼����
** output parameters:       buf                 ��ǰ����Ŀ¼
** Returned value:          ��ǰ����Ŀ¼
*********************************************************************************************************/
char *vfs_getcwd(char *buf, size_t size);
/*********************************************************************************************************
** ���ص�ӿ�
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_mount
** Descriptions:            �����ļ�ϵͳ
** input parameters:        point_name          ���ص���
**                          dev_name            �豸��
**                          fs_name             �ļ�ϵͳ��
**                          param               ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mount(const char *point_name, const char *dev_name, const char *fs_name, const char *param);
/*********************************************************************************************************
** Function name:           vfs_mount_point_create
** Descriptions:            �������ص�
** input parameters:        point_name          ���ص���
**                          fs                  �ļ�ϵͳ
**                          dev                 �豸
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mount_point_create(const char *point_name, file_system_t *fs, device_t *dev);
/*********************************************************************************************************
** Function name:           vfs_mkfs
** Descriptions:            ��ʽ���ļ�ϵͳ
** input parameters:        path                Ŀ¼ PATH
**                          param               ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mkfs(const char *path, const char *param);
/*********************************************************************************************************
** Function name:           vfs_unmount
** Descriptions:            ȡ�������ļ�ϵͳ
** input parameters:        path                Ŀ¼ PATH
**                          param               ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_unmount(const char *path, const char *param);
/*********************************************************************************************************
** �����ӿ�
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           vfs_init
** Descriptions:            ��ʼ�������ļ�ϵͳ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_init(void);
/*********************************************************************************************************
** Function name:           vfs_process_init
** Descriptions:            ��ʼ�����̵��ļ���Ϣ
** input parameters:        pid                 ���� ID
**                          tid                 ���� ID
**                          open_max            OPEN_MAX
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_process_init(pid_t pid, pid_t tid, int open_max);
/*********************************************************************************************************
** Function name:           vfs_process_cleanup
** Descriptions:            ������̵��ļ���Ϣ
** input parameters:        pid                 ���� ID
**                          tid                 ���� ID
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_process_cleanup(pid_t pid, pid_t tid);
/*********************************************************************************************************
** Function name:           vfs_get_file
** Descriptions:            �����ļ�����������ļ��ṹ
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          �ļ��ṹ
*********************************************************************************************************/
file_t *vfs_get_file(int fd);
/*********************************************************************************************************
** Function name:           vfs_put_file
** Descriptions:            �黹�ļ��ṹ
** input parameters:        file                �ļ��ṹ
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
