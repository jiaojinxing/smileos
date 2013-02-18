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
** File name:               vfs_fs.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            �����ļ�ϵͳ���ļ�ϵͳ�����ӿ�
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
** �ⲿ����
*********************************************************************************************************/
extern mutex_t              point_mgr_lock;                             /*  ���ص������                */
/*********************************************************************************************************
** Function name:           vfs_link
** Descriptions:            ���ļ�����һ������
** input parameters:        path1               �ļ� PATH
**                          path2               ���ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_link(const char *path1, const char *path2)
{
    mount_point_t  *point1;
    char           *pathbuf1;
    char           *filepath1;
    mount_point_t  *point2;
    char           *pathbuf2;
    char           *filepath2;
    int             ret;

    pathbuf1 = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf1 == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    pathbuf2 = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf2 == NULL) {
        kfree(pathbuf1);
        seterrno(ENOMEM);
        return -1;
    }

    point1 = vfs_mount_point_lookup_ref(pathbuf1, &filepath1, path1);   /*  ���ҹ��ص�                  */
    if (point1 == NULL) {
        kfree(pathbuf1);
        kfree(pathbuf2);
        return -1;
    }

    point2 = vfs_mount_point_lookup_ref(pathbuf2, &filepath2, path2);   /*  ���ҹ��ص�                  */
    if (point2 == NULL) {
        atomic_dec(&point1->ref);
        kfree(pathbuf1);
        kfree(pathbuf2);
        return -1;
    }

    if (point2 != point1) {                                             /*  �������ص����Ҫ��ͬ        */
        atomic_dec(&point1->ref);
        atomic_dec(&point2->ref);
        kfree(pathbuf1);
        kfree(pathbuf2);
        seterrno(EXDEV);
        return -1;
    }

    if (point1->fs->link == NULL) {
        atomic_dec(&point1->ref);
        atomic_dec(&point2->ref);
        kfree(pathbuf1);
        kfree(pathbuf2);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point1->fs->link(point1, filepath1, filepath2);

    atomic_dec(&point1->ref);
    atomic_dec(&point2->ref);

    kfree(pathbuf1);
    kfree(pathbuf2);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_rename
** Descriptions:            ������(Ҳ���ƶ�)�ļ�
** input parameters:        old                 Դ�ļ� PATH
**                          _new                ���ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rename(const char *old, const char *_new)
{
    mount_point_t  *point1;
    char           *pathbuf1;
    char           *filepath1;
    mount_point_t  *point2;
    char           *pathbuf2;
    char           *filepath2;
    int             ret;

    pathbuf1 = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf1 == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    pathbuf2 = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf2 == NULL) {
        kfree(pathbuf1);
        seterrno(ENOMEM);
        return -1;
    }

    point1 = vfs_mount_point_lookup_ref(pathbuf1, &filepath1, old);     /*  ���ҹ��ص�                  */
    if (point1 == NULL) {
        kfree(pathbuf1);
        kfree(pathbuf2);
        return -1;
    }

    point2 = vfs_mount_point_lookup_ref(pathbuf2, &filepath2, _new);    /*  ���ҹ��ص�                  */
    if (point2 == NULL) {
        atomic_dec(&point1->ref);
        kfree(pathbuf1);
        kfree(pathbuf2);
        return -1;
    }

    if (point2 != point1) {                                             /*  �������ص����Ҫ��ͬ        */
        atomic_dec(&point1->ref);
        atomic_dec(&point2->ref);
        kfree(pathbuf1);
        kfree(pathbuf2);
        seterrno(EXDEV);
        return -1;
    }

    if (point1->fs->rename == NULL) {
        atomic_dec(&point1->ref);
        atomic_dec(&point2->ref);
        kfree(pathbuf1);
        kfree(pathbuf2);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point1->fs->rename(point1, filepath1, filepath2);

    atomic_dec(&point1->ref);
    atomic_dec(&point2->ref);

    kfree(pathbuf1);
    kfree(pathbuf2);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_stat
** Descriptions:            ����ļ�״̬
** input parameters:        path                �ļ� PATH
**                          buf                 �ļ�״̬����
** output parameters:       buf                 �ļ�״̬
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_stat(const char *path, struct stat *buf)
{
    mount_point_t  *point;
    char           *pathbuf;
    char           *filepath;
    int             ret;

    if (buf == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    pathbuf = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    point = vfs_mount_point_lookup2_ref(pathbuf, &filepath, path);      /*  ���ҹ��ص�                  */
    if (point == NULL) {
        kfree(pathbuf);
        return -1;
    }

    if (point->fs->stat == NULL) {
        atomic_dec(&point->ref);
        kfree(pathbuf);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->stat(point, filepath, buf);

    atomic_dec(&point->ref);

    kfree(pathbuf);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_unlink
** Descriptions:            ɾ���ļ�
** input parameters:        path                �ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_unlink(const char *path)
{
    mount_point_t  *point;
    char           *pathbuf;
    char           *filepath;
    int             ret;

    pathbuf = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    point = vfs_mount_point_lookup_ref(pathbuf, &filepath, path);       /*  ���ҹ��ص�                  */
    if (point == NULL) {
        kfree(pathbuf);
        return -1;
    }

    if (point->fs->unlink == NULL) {
        atomic_dec(&point->ref);
        kfree(pathbuf);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->unlink(point, filepath);

    atomic_dec(&point->ref);

    kfree(pathbuf);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_mkdir
** Descriptions:            ����Ŀ¼
** input parameters:        path                Ŀ¼ PATH
**                          mode                ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mkdir(const char *path, mode_t mode)
{
    mount_point_t  *point;
    char           *pathbuf;
    char           *filepath;
    int             ret;

    pathbuf = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    point = vfs_mount_point_lookup_ref(pathbuf, &filepath, path);       /*  ���ҹ��ص�                  */
    if (point == NULL) {
        kfree(pathbuf);
        return -1;
    }

    if (point->fs->mkdir == NULL) {
        atomic_dec(&point->ref);
        kfree(pathbuf);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->mkdir(point, filepath, mode);

    atomic_dec(&point->ref);

    kfree(pathbuf);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_rmdir
** Descriptions:            ɾ��Ŀ¼
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_rmdir(const char *path)
{
    mount_point_t  *point;
    char           *pathbuf;
    char           *filepath;
    int             ret;

    pathbuf = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    point = vfs_mount_point_lookup_ref(pathbuf, &filepath, path);       /*  ���ҹ��ص�                  */
    if (point == NULL) {
        kfree(pathbuf);
        return -1;
    }

    if (point->fs->rmdir == NULL) {
        atomic_dec(&point->ref);
        kfree(pathbuf);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->rmdir(point, filepath);

    atomic_dec(&point->ref);

    kfree(pathbuf);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_access
** Descriptions:            �ж��Ƿ�ɷ���
** input parameters:        path                �ļ� PATH
**                          amode               ����ģʽ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_access(const char *path, int amode)
{
    mount_point_t  *point;
    char           *pathbuf;
    char           *filepath;
    int             ret;

    pathbuf = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    point = vfs_mount_point_lookup2_ref(pathbuf, &filepath, path);      /*  ���ҹ��ص�                  */
    if (point == NULL) {
        kfree(pathbuf);
        return -1;
    }

    if (point->fs->access == NULL) {
        atomic_dec(&point->ref);
        kfree(pathbuf);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->access(point, filepath, amode);

    atomic_dec(&point->ref);

    kfree(pathbuf);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_truncate
** Descriptions:            �޸��ļ�����
** input parameters:        path                �ļ� PATH
**                          amode               �µ��ļ�����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_truncate(const char *path, off_t len)
{
    mount_point_t  *point;
    char           *pathbuf;
    char           *filepath;
    int             ret;

    pathbuf = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    point = vfs_mount_point_lookup_ref(pathbuf, &filepath, path);       /*  ���ҹ��ص�                  */
    if (point == NULL) {
        kfree(pathbuf);
        return -1;
    }

    if (point->fs->truncate == NULL) {
        atomic_dec(&point->ref);
        kfree(pathbuf);
        seterrno(ENOSYS);
        return -1;
    }

    seterrno(0);
    ret = point->fs->truncate(point, filepath, len);

    atomic_dec(&point->ref);

    kfree(pathbuf);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_truncate
** Descriptions:            ͬ��
** input parameters:        path                �ļ� PATH
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_sync(const char *path)
{
    mount_point_t  *point;
    char           *pathbuf;
    char           *filepath;
    int             ret;

    pathbuf = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    point = vfs_mount_point_lookup2_ref(pathbuf, &filepath, path);      /*  ���ҹ��ص�                  */
    if (point == NULL) {
        kfree(pathbuf);
        return -1;
    }

    if (point->fs->sync == NULL) {
        atomic_dec(&point->ref);
        kfree(pathbuf);
        seterrno(ENOSYS);
        return 0;
    }

    seterrno(0);
    ret = point->fs->sync(point);

    atomic_dec(&point->ref);

    kfree(pathbuf);

    return ret;
}
/*********************************************************************************************************
** Function name:           vfs_mkfs
** Descriptions:            ��ʽ���ļ�ϵͳ
** input parameters:        path                Ŀ¼ PATH
**                          param               ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_mkfs(const char *path, const char *param)
{
    mount_point_t  *point;
    char           *pathbuf;
    char           *filepath;
    int             ret;

    pathbuf = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
    if (pathbuf == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  ���ҹ��ص�                  */
    if (point == NULL) {
        mutex_unlock(&point_mgr_lock);
        kfree(pathbuf);
        return -1;
    }

    if (point->fs->mkfs == NULL) {
        mutex_unlock(&point_mgr_lock);
        kfree(pathbuf);
        seterrno(ENOSYS);
        return -1;
    }

    if (atomic_read(&point->ref) != 0) {
        mutex_unlock(&point_mgr_lock);
        kfree(pathbuf);
        seterrno(EBUSY);
        return -1;
    }

    seterrno(0);
    ret = point->fs->mkfs(point, param);                                /*  ��ʽ���ļ�ϵͳ              */

    mutex_unlock(&point_mgr_lock);

    kfree(pathbuf);

    return ret;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
