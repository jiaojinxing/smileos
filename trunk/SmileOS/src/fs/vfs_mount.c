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
** File name:               vfs_mount.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            �����ļ�ϵͳ�Ĺ��ص�ӿ�
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

extern mount_point_t       *rootfs_point;                               /*  ���ļ�ϵͳ���ص�            */

extern mutex_t              dev_mgr_lock;                               /*  �豸������                  */

extern mutex_t              fs_mgr_lock;                                /*  �ļ�ϵͳ������              */
/*********************************************************************************************************
** Function name:           vfs_path_add_mount_point
** Descriptions:            �� PATH ǰ������ص���
** input parameters:        path                ·��
** output parameters:       NONE
** Returned value:          ������ص����� PATH
*********************************************************************************************************/
const char *vfs_path_add_mount_point(const char *path)
{
    path--;
    while (*path != '/') {
        path--;
    }
    return path;
}
/*********************************************************************************************************
** Function name:           vfs_path_add_mount_point
** Descriptions:            ������ PATH
** input parameters:        path                ·��
**                          sprit_end           �Ƿ��� / ��β
** output parameters:       path                ���������·��
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_path_normalization(char path[PATH_BUF_LEN], int sprit_end)
{
    /*
     * �ı�״̬��! �����Ⱦ���ʦ�¾�!
     * http://blog.csdn.net/absurd/article/details/4249574
     */
    char   *p;
    char    ch;
    char    prev1_ch;
    char    prev2_ch;
    enum {
            BEGIN,
            WORKING,
    }       status;
    int     pos;

    pos    = 0;
    status = BEGIN;

    prev2_ch = '/';
    prev1_ch = '/';

    for (p = path; (ch = *p) != '\0'; p++) {                            /*  ���������ı�                */
        switch (status) {
        case BEGIN:                                                     /*  ��ʼ״̬                    */
            if (ch != '/') {                                            /*  ��������� / �ſ�ʼ, ����   */
                seterrno(EINVAL);
                return -1;
            }
            path[pos++] = ch;                                           /*  ��¼ / ��                   */
            status = WORKING;
            continue;
            break;

        case WORKING:
            switch (ch) {
            case '.':
                if (prev2_ch == '/' && prev1_ch == '.' && (p[1] == '\0' || p[1] == '/')) {
                    /*
                     *  /../ �� /..
                     */
                    /*
                     * pos, prev1_ch, prev2_ch
                     */
                    pos -= 3;
                    while (pos >= 0 && path[pos] != '/') {
                        pos--;
                    }
                    if (pos < 0) {
                        seterrno(ENOENT);
                        return -1;
                    }
                    if (pos == 0) {
                        prev2_ch = '/';
                        prev1_ch = '/';
                    } else {
                        prev2_ch = path[pos - 1];
                        prev1_ch = '/';
                    }
                    pos++;
                    continue;
                } else if (prev1_ch == '/' && (p[1] == '\0' || p[1] == '/')) {
                    /*
                     *  /./ �� /.
                     *  skip it
                     */
                    continue;
                } else {
                    path[pos++] = ch;
                }
                break;

            case '/':
                if (prev1_ch == '/') {
                    /*
                     *  //
                     *  skip it
                     */
                    continue;
                } else {
                    path[pos++] = ch;
                }
                break;

            default:
                path[pos++] = ch;
                break;
            }
        }

        prev2_ch = prev1_ch;
        prev1_ch = ch;
    }

    if (sprit_end) {
        if (pos >= 2 && path[pos - 1] != '/') {
            path[pos] = '/';
            path[pos + 1] = '\0';
        } else {
            path[pos] = '\0';
        }
    } else {
        if (pos > 2 && path[pos - 1] == '/') {
            path[pos - 1] = '\0';
        } else {
            path[pos] = '\0';
        }
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup
** Descriptions:            ���ҹ��ص�, PATH �����ǹ��ص�
** input parameters:        pathbuf             ·����ʱ������
**                          path                ·��
** output parameters:       ppath               ָ��ȥ�����ص���·��
** Returned value:          ���ص� OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path)
{
    mount_point_t *point;
    char          *tmp;

    if (path == NULL || strlen(path) > PATH_MAX - 1) {                  /*  PATH �Ϸ��Լ��             */
        seterrno(EINVAL);
        return NULL;
    }

    if (path[0] == '/') {                                               /*  ����Ǿ���·��              */
        if (path[1] == '\0') {                                          /*  �����Ǹ�Ŀ¼                */
            seterrno(EINVAL);
            return NULL;
        }
        strlcpy(pathbuf, path, PATH_MAX);
    } else {                                                            /*  ��������·��              */
        /*
         * cwd Ҫ�� / �ſ�ͷ�ͽ�β
         */
        int tid = gettid();
        snprintf(pathbuf, PATH_BUF_LEN, "%s%s", cwd[tid], path);        /*  ��ǰ����뵱ǰ����Ŀ¼      */
    }

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {                   /*  ������ PATH                 */
        return NULL;
    }

    tmp = strchr(pathbuf + 1, '/');                                     /*  ���ҹ��ص������ / ��       */
    if (tmp == NULL) {                                                  /*  û�е�                      */
        seterrno(EINVAL);
        return NULL;
    }
    if (tmp[1] == '\0') {                                               /*  �����ǹ��ص�                */
        seterrno(EINVAL);
        return NULL;
    }

    *tmp  = '\0';                                                       /*  ��ʱȥ�� / ��               */
    point = mount_point_lookup(pathbuf);                                /*  ���ҹ��ص�                  */
    *tmp  = '/';                                                        /*  �ָ� / ��                   */

    *ppath = tmp;
    if (point == NULL) {
        seterrno(ENOENT);
    }
    return point;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup2
** Descriptions:            ���ҹ��ص�, PATH �����ǹ��ص�
** input parameters:        pathbuf             ·����ʱ������
**                          path                ·��
** output parameters:       ppath               ָ��ȥ�����ص���·��
** Returned value:          ���ص� OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup2(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t  *point;
    char           *tmp;

    if (path == NULL || strlen(path) > PATH_MAX - 1) {                  /*  PATH �Ϸ��Լ��             */
        seterrno(EINVAL);
        return NULL;
    }

    if (path[0] == '/') {                                               /*  ����Ǿ���·��              */
        strlcpy(pathbuf, path, PATH_MAX);
    } else {                                                            /*  ��������·��              */
        /*
         * cwd Ҫ�� / �ſ�ͷ�ͽ�β
         */
        int tid = gettid();
        snprintf(pathbuf, PATH_BUF_LEN, "%s%s", cwd[tid], path);        /*  ��ǰ����뵱ǰ����Ŀ¼      */
    }

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {
        return NULL;
    }

    if (pathbuf[1] == '\0') {                                           /*  ����Ǹ�Ŀ¼                */
        point = mount_point_lookup(pathbuf);
        tmp = pathbuf;
    } else {
        tmp = strchr(pathbuf + 1, '/');                                 /*  ���ҹ��ص������ / ��       */
        if (tmp == NULL) {                                              /*  ����ǹ��ص�                */
            point = mount_point_lookup(pathbuf);
            strcat(pathbuf + 1, "/");
            tmp = strchr(pathbuf + 1, '/');
        } else {
            *tmp = '\0';                                                /*  ��ʱȥ�� / ��               */
            point = mount_point_lookup(pathbuf);                        /*  ���ҹ��ص�                  */
            *tmp = '/';                                                 /*  �ָ� / ��                   */
        }
    }

    *ppath = tmp;
    if (point == NULL) {
        seterrno(ENOENT);
    }
    return point;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup_ref
** Descriptions:            ���Ҳ����ù��ص�, PATH �����ǹ��ص�
** input parameters:        pathbuf             ·����ʱ������
**                          path                ·��
** output parameters:       ppath               ָ��ȥ�����ص���·��
** Returned value:          ���ص� OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup_ref(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path)
{
    mount_point_t *point;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup(pathbuf, ppath, path);

    if (point != NULL) {
        atomic_inc(&point->ref);
    }

    mutex_unlock(&point_mgr_lock);

    return point;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup2_ref
** Descriptions:            ���Ҳ����ù��ص�, PATH �����ǹ��ص�
** input parameters:        pathbuf             ·����ʱ������
**                          path                ·��
** output parameters:       ppath               ָ��ȥ�����ص���·��
** Returned value:          ���ص� OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup2_ref(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path)
{
    mount_point_t *point;

    mutex_lock(&point_mgr_lock, 0);

    point = vfs_mount_point_lookup2(pathbuf, ppath, path);

    if (point != NULL) {
        atomic_inc(&point->ref);
    }

    mutex_unlock(&point_mgr_lock);

    return point;
}
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
int vfs_mount(const char *point_name, const char *dev_name, const char *fs_name, const char *param)
{
    mount_point_t  *point;
    file_system_t  *fs;
    device_t       *dev;
    int             ret;

    if (fs_name == NULL || point_name == NULL || strlen(point_name) > sizeof(point->name) - 1) {
        seterrno(EINVAL);
        return -1;
    }

    mutex_lock(&point_mgr_lock, 0);
    point = mount_point_lookup(point_name);                             /*  ���ҹ��ص�                  */
    if (point != NULL) {                                                /*  û�ҵ�                      */
        mutex_unlock(&point_mgr_lock);
        seterrno(EINVAL);
        return -1;
    }

    mutex_lock(&fs_mgr_lock, 0);
    fs = file_system_lookup(fs_name);                                   /*  �����ļ�ϵͳ                */
    if (fs == NULL) {
        mutex_unlock(&fs_mgr_lock);
        mutex_unlock(&point_mgr_lock);
        seterrno(EINVAL);
        return -1;
    }
    atomic_inc(&fs->ref);
    mutex_unlock(&fs_mgr_lock);

    if (dev_name != NULL) {
        mutex_lock(&dev_mgr_lock, 0);
        dev = device_lookup(dev_name);                                  /*  �����豸                    */
        if (dev == NULL) {
            atomic_dec(&fs->ref);
            mutex_unlock(&dev_mgr_lock);
            mutex_unlock(&point_mgr_lock);
            seterrno(ENODEV);
            return -1;
        }

        if (atomic_inc_return(&dev->ref) != 1) {
            atomic_dec(&dev->ref);
            atomic_dec(&fs->ref);
            mutex_unlock(&dev_mgr_lock);
            mutex_unlock(&point_mgr_lock);
            seterrno(EBUSY);
            return -1;
        }
        mutex_unlock(&dev_mgr_lock);
    }

    point = kmalloc(sizeof(mount_point_t), GFP_KERNEL);                 /*  ������ص�                  */
    if (point == NULL) {
        if (dev != NULL) {
            atomic_dec(&dev->ref);
        }
        atomic_dec(&fs->ref);
        mutex_unlock(&point_mgr_lock);
        seterrno(ENOMEM);
        return -1;
    }

    if (point_name[0] == '/') {                                         /*  ��֤���ص��� / �ſ�ʼ       */
        strlcpy(point->name, point_name, sizeof(point->name));
    } else {
        snprintf(point->name, sizeof(point->name), "/%s", point_name);
    }

    if (point->name[1] != '\0') {                                       /*  ������Ǹ��ļ�ϵͳ          */
        if (strchr(point->name + 1, '/') != NULL) {                     /*  ��֤���ص㲻���ٳ��� / ��   */
                                                                        /*  ��Ϊ�Ҳ�֪�� / �Ż��м���   */
            kfree(point);
            if (dev != NULL) {
                atomic_dec(&dev->ref);
            }
            atomic_dec(&fs->ref);
            mutex_unlock(&point_mgr_lock);
            seterrno(EINVAL);
            return -1;
        }
    } else {
        rootfs_point = point;
    }

    point->fs  = fs;
    point->dev = dev;
    point->ctx = NULL;
    atomic_set(&point->ref, 0);

    seterrno(0);
    ret = fs->mount(point, dev, dev_name, param);                       /*  ����                        */
    if (ret < 0) {
        if (rootfs_point == point) {
            rootfs_point =  NULL;
        }
        kfree(point);
        if (dev != NULL) {
            atomic_dec(&dev->ref);
        }
        atomic_dec(&fs->ref);
        mutex_unlock(&point_mgr_lock);
        return -1;
    }

    mount_point_install(point);                                         /*  ��װ���ص�                  */
    mutex_unlock(&point_mgr_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_unmount
** Descriptions:            ȡ�������ļ�ϵͳ
** input parameters:        path                Ŀ¼ PATH
**                          param               ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_unmount(const char *path, const char *param)
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

    if (point->fs->unmount == NULL) {
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

    vfs_sync(path);

    seterrno(0);
    ret = point->fs->unmount(point, param);                             /*  ȡ�������ļ�ϵͳ            */
    if (ret == 0) {
        if (point->dev != NULL) {
            atomic_dec(&point->dev->ref);
        }
        atomic_dec(&point->fs->ref);
        mount_point_remove(point);
    }

    mutex_unlock(&point_mgr_lock);

    kfree(pathbuf);

    return ret;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
