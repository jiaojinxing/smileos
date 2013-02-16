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
** Descriptions:            �����ļ�ϵͳ���ļ������ӿ�
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
#include "vfs_common.h"
/*********************************************************************************************************
**
** �ļ������ӿ�
**
** ����ĺ�ͦ���ĵ�, �����û��Щ��, ����ļ�Ҫ��������, ���Ҳ�����ͳһ����
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
        if (fd < 0 || fd >= info->open_max) {                           /*  �ļ��������Ϸ����ж�        */\
            mutex_unlock(&info_lock[pid]);                                                                \
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        file = info->files[fd];                                         /*  ����ļ��ṹ                */\
                                                                                                          \
        mutex_unlock(&info_lock[pid]);                                                                    \
                                                                                                          \
        if (file == NULL) {                                             /*  ����ļ�δ��              */\
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        if (mutex_lock(&file->lock, 0) < 0) {                                                             \
            seterrno(EBADFD);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        if (!(file->type & VFS_FILE_TYPE_FILE)) {                       /*  ������Ͳ����ļ� ����       */\
            mutex_unlock(&file->lock);                                                                    \
            seterrno(EFTYPE);                                                                             \
            return -1;                                                                                    \
        }                                                                                                 \
                                                                                                          \
        point = file->point                                             /*  ��ù��ص�                  */

#define vfs_file_end(pid)                                                                                 \
        mutex_unlock(&file->lock)
/*********************************************************************************************************
** Function name:           vfs_open
** Descriptions:            ���ļ�
** input parameters:        path                �ļ�·��
**                          oflag               ��־
**                          mode                ģʽ
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

    point = vfs_mount_point_lookup_ref(pathbuf, &filepath, path);       /*  ���ҹ��ص�                  */
    if (point == NULL) {
        return -1;
    }

    mutex_lock(&info_lock[pid], 0);

    info = infos[pid];

    for (fd = 0; fd < info->open_max; fd++) {                           /*  ����һ�����е��ļ�������    */
        file = info->files[fd];
        if (file == NULL) {
            break;
        }
    }

    if (fd == info->open_max) {                                         /*  û�ҵ�                      */
        mutex_unlock(&info_lock[pid]);
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return -1;
    }

    file = vfs_file_alloc();                                            /*  ����һ���ļ��ṹ            */
    if (file == NULL) {
        mutex_unlock(&info_lock[pid]);
        atomic_dec(&point->ref);
        seterrno(EMFILE);
        return -1;
    }

    info->files[fd] = file;                                             /*  ��¼����                    */

    mutex_lock(&file->lock, 0);                                         /*  �����ļ��ṹ                */

    mutex_unlock(&info_lock[pid]);

    file->type   = VFS_FILE_TYPE_FILE;                                  /*  ����: �ļ�                  */
    file->flags  = (oflag + 1);                                         /*  �򿪱�־                    */
    file->ctx    = NULL;                                                /*  ������     ��               */
    file->point  = point;                                               /*  ���ص�                      */

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
    ret = point->fs->open(point, file, filepath, oflag, mode);         /*  ���ļ�                    */
    if (ret < 0) {
        mutex_lock(&info_lock[pid], 0);
        info->files[fd] = NULL;
        mutex_unlock(&info_lock[pid]);
        vfs_file_free(file);
        atomic_dec(&point->ref);
        return -1;
    }

    mutex_unlock(&file->lock);

    return fd;                                                          /*  �����ļ�������              */
}
/*********************************************************************************************************
** Function name:           vfs_close
** Descriptions:            �ر��ļ�
** input parameters:        fd                  �ļ�������
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
** Descriptions:            �ر��ļ�
** input parameters:        pid                 ���� ID
**                          fd                  �ļ�������
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
** Descriptions:            �����ļ�
** input parameters:        fd                  �ļ�������
**                          cmd                 ����
**                          arg                 ����
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
** Descriptions:            ����ļ�״̬
** input parameters:        fd                  �ļ�������
**                          buf                 �ļ�״̬����
** output parameters:       buf                 �ļ�״̬
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
** Descriptions:            �ж��ļ��ǲ����ն�
** input parameters:        fd                  �ļ�������
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
** Descriptions:            ͬ���ļ�
** input parameters:        fd                  �ļ�������
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
** Descriptions:            ͬ���ļ�
** input parameters:        fd                  �ļ�������
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
** Descriptions:            �޸��ļ�����
** input parameters:        fd                  �ļ�������
**                          len                 �³���
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
** Descriptions:            ���ļ�
** input parameters:        fd                  �ļ�������
**                          buf                 ���ݻ�����
**                          len                 �³���
** output parameters:       buf                 ����
** Returned value:          �ɹ���ȡ���ֽ��� OR -1
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

        if (fd < 0 || fd >= info->open_max) {                           /*  �ļ��������Ϸ����ж�        */
            mutex_unlock(&info_lock[pid]);
            seterrno(EBADFD);
            return -1;
        }

        file = info->files[fd];                                         /*  ����ļ��ṹ                */

        mutex_unlock(&info_lock[pid]);

        if (file == NULL) {                                             /*  ����ļ�δ��              */
            seterrno(EBADFD);
            return -1;
        }

        if (mutex_lock(&file->lock, 0) < 0) {
            seterrno(EBADFD);
            return -1;
        }

        if (!(file->type & VFS_FILE_TYPE_FILE)) {                       /*  ������Ͳ����ļ� ����       */
            mutex_unlock(&file->lock);
            seterrno(EFTYPE);
            return -1;
        }

        point = file->point;                                             /*  ��ù��ص�                  */

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
** Descriptions:            д�ļ�
** input parameters:        fd                  �ļ�������
**                          buf                 ���ݻ�����
**                          len                 �³���
** output parameters:       NONE
** Returned value:          �ɹ�д����ֽ��� OR -1
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
** Descriptions:            �����ļ�
** input parameters:        fd                  �ļ�������
**                          cmd                 ����
**                          arg                 ����
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
** Descriptions:            �����ļ���дλ��
** input parameters:        fd                  �ļ�������
**                          offset              ƫ��
**                          whence              ������λ��
** output parameters:       NONE
** Returned value:          �µĶ�дλ�� OR -1
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
** Descriptions:            �����ļ�������
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          �µ��ļ������� OR -1
*********************************************************************************************************/
int vfs_dup(int fd)
{
    int     i;
    int     pid = getpid();
    file_t *temp;

    vfs_file_begin(pid);

    mutex_lock(&info_lock[pid], 0);

    for (i = 0; i < info->open_max; i++) {                              /*  ����һ�����е��ļ�������    */
        temp = info->files[i];
        if (temp == NULL) {
            break;
        }
    }
    if (i == info->open_max) {                                          /*  û�ҵ�                      */
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
** Descriptions:            �����ļ���������ָ�����ļ�������
** input parameters:        fd                  �ļ�������
**                          to                  ָ�����ļ�������
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
** Descriptions:            ɨ���ļ�
** input parameters:        fd                  �ļ�������
**                          flags               ��־
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
** Descriptions:            ����ǰ������뵽�ļ��ĵȴ��б�
** input parameters:        fd                  �ļ�������
**                          flags               ��־
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
** Descriptions:            ����ǰ������ļ��ĵȴ��б����Ƴ�
** input parameters:        fd                  �ļ�������
**                          flags               ��־
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
** Descriptions:            �����ļ�����������ļ��ṹ
** input parameters:        fd                  �ļ�������
** output parameters:       NONE
** Returned value:          �ļ��ṹ OR NULL
*********************************************************************************************************/
file_t *vfs_get_file(int fd)
{
    pid_t pid = getpid();

    vfs_file_begin(pid);

    return file;
}
/*********************************************************************************************************
** Function name:           vfs_put_file
** Descriptions:            �黹�ļ��ṹ
** input parameters:        file                �ļ��ṹ
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
