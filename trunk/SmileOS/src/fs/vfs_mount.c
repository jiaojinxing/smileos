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
** Descriptions:            虚拟文件系统的挂载点接口
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
** 外部变量
*********************************************************************************************************/
extern mutex_t              mount_point_lock;                           /*  挂载点管理锁                */

extern mount_point_t       *rootfs_point;                               /*  根文件系统挂载点            */

extern mutex_t              device_lock;                                /*  设备管理锁                  */

extern mutex_t              file_system_lock;                           /*  文件系统管理锁              */
/*********************************************************************************************************
** Function name:           vfs_path_add_mount_point
** Descriptions:            在 PATH 前加入挂载点名
** input parameters:        path                路径
** output parameters:       NONE
** Returned value:          加入挂载点名的 PATH
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
** Descriptions:            正常化 PATH
** input parameters:        path                路径
**                          sprit_end           是否以 / 结尾
** output parameters:       path                正常化后的路径
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_path_normalization(char *path, int sprit_end)
{
    /*
     * 文本状态机! 向李先静老师致敬!
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

    for (p = path; (ch = *p) != '\0'; p++) {                            /*  处理所有文本                */
        switch (status) {
        case BEGIN:                                                     /*  开始状态                    */
            if (ch != '/') {                                            /*  如果不是以 / 号开始, 出错   */
                seterrno(EINVAL);
                return -1;
            }
            path[pos++] = ch;                                           /*  记录 / 号                   */
            status = WORKING;
            continue;
            break;

        case WORKING:
            switch (ch) {
            case '.':
                if (prev2_ch == '/' && prev1_ch == '.' && (p[1] == '\0' || p[1] == '/')) {
                    /*
                     *  /../ 或 /..
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
                     *  /./ 或 /.
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
** Function name:           vfs_path_format
** Descriptions:            格式化 PATH
** input parameters:        path                路径
** output parameters:       pathbuf             格式化后的路径
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_path_format(char *pathbuf, const char *path)
{
    char *pathbuf1;

    if (pathbuf == NULL || path == NULL || strlen(path) > PATH_MAX - 1) {/*  PATH 合法性检查            */
        seterrno(EINVAL);
        return -1;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        strlcpy(pathbuf, path, PATH_MAX);

        if (vfs_path_normalization(pathbuf, FALSE) < 0) {               /*  正常化 PATH                 */
            return -1;
        }
    } else {                                                            /*  如果是相对路径              */
        int tid = gettid();

        pathbuf1 = kmalloc(PATH_BUF_LEN, GFP_KERNEL);
        if (pathbuf1 == NULL) {
            seterrno(ENOMEM);
            return -1;
        }

        snprintf(pathbuf1, PATH_BUF_LEN, "%s%s", cwd[tid], path);       /*  在前面加入当前工作目录      */

        if (vfs_path_normalization(pathbuf1, FALSE) < 0) {              /*  正常化 PATH                 */
            kfree(pathbuf1);
            return -1;
        }

        kfree(pathbuf1);
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup
** Descriptions:            查找挂载点, PATH 不能是挂载点
** input parameters:        pathbuf             路径临时缓冲区
**                          path                路径
** output parameters:       ppath               指向去掉挂载点后的路径
** Returned value:          挂载点 OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path)
{
    mount_point_t *point;
    char          *tmp;

    if (path == NULL || strlen(path) > PATH_MAX - 1) {                  /*  PATH 合法性检查             */
        seterrno(EINVAL);
        return NULL;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        if (path[1] == '\0') {                                          /*  不能是根目录                */
            seterrno(EINVAL);
            return NULL;
        }
        strlcpy(pathbuf, path, PATH_MAX);
    } else {                                                            /*  如果是相对路径              */
        /*
         * cwd 要以 / 号开头和结尾
         */
        int tid = gettid();
        snprintf(pathbuf, PATH_BUF_LEN, "%s%s", cwd[tid], path);        /*  在前面加入当前工作目录      */
    }

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {                   /*  正常化 PATH                 */
        return NULL;
    }

    tmp = strchr(pathbuf + 1, '/');                                     /*  查找挂载点名后的 / 号       */
    if (tmp == NULL) {                                                  /*  没有到                      */
        seterrno(EINVAL);
        return NULL;
    }
    if (tmp[1] == '\0') {                                               /*  不能是挂载点                */
        seterrno(EINVAL);
        return NULL;
    }

    *tmp  = '\0';                                                       /*  暂时去掉 / 号               */
    point = mount_point_lookup(pathbuf);                                /*  查找挂载点                  */
    *tmp  = '/';                                                        /*  恢复 / 号                   */

    *ppath = tmp;
    if (point == NULL) {
        seterrno(ENOENT);
    }
    return point;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup2
** Descriptions:            查找挂载点, PATH 可以是挂载点
** input parameters:        pathbuf             路径临时缓冲区
**                          path                路径
** output parameters:       ppath               指向去掉挂载点后的路径
** Returned value:          挂载点 OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup2(char pathbuf[PATH_MAX], char **ppath, const char *path)
{
    mount_point_t  *point;
    char           *tmp;

    if (path == NULL || strlen(path) > PATH_MAX - 1) {                  /*  PATH 合法性检查             */
        seterrno(EINVAL);
        return NULL;
    }

    if (path[0] == '/') {                                               /*  如果是绝对路径              */
        strlcpy(pathbuf, path, PATH_MAX);
    } else {                                                            /*  如果是相对路径              */
        /*
         * cwd 要以 / 号开头和结尾
         */
        int tid = gettid();
        snprintf(pathbuf, PATH_BUF_LEN, "%s%s", cwd[tid], path);        /*  在前面加入当前工作目录      */
    }

    if (vfs_path_normalization(pathbuf, FALSE) < 0) {
        return NULL;
    }

    if (pathbuf[1] == '\0') {                                           /*  如果是根目录                */
        point = mount_point_lookup(pathbuf);
        tmp = pathbuf;
    } else {
        tmp = strchr(pathbuf + 1, '/');                                 /*  查找挂载点名后的 / 号       */
        if (tmp == NULL) {                                              /*  如果是挂载点                */
            point = mount_point_lookup(pathbuf);
            strcat(pathbuf + 1, "/");
            tmp = strchr(pathbuf + 1, '/');
        } else {
            *tmp = '\0';                                                /*  暂时去掉 / 号               */
            point = mount_point_lookup(pathbuf);                        /*  查找挂载点                  */
            *tmp = '/';                                                 /*  恢复 / 号                   */
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
** Descriptions:            查找并引用挂载点, PATH 不能是挂载点
** input parameters:        pathbuf             路径临时缓冲区
**                          path                路径
** output parameters:       ppath               指向去掉挂载点后的路径
** Returned value:          挂载点 OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup_ref(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path)
{
    mount_point_t *point;

    mutex_lock(&mount_point_lock, 0);

    point = vfs_mount_point_lookup(pathbuf, ppath, path);

    if (point != NULL) {
        atomic_inc(&point->ref);
    }

    mutex_unlock(&mount_point_lock);

    return point;
}
/*********************************************************************************************************
** Function name:           vfs_mount_point_lookup2_ref
** Descriptions:            查找并引用挂载点, PATH 可以是挂载点
** input parameters:        pathbuf             路径临时缓冲区
**                          path                路径
** output parameters:       ppath               指向去掉挂载点后的路径
** Returned value:          挂载点 OR NULL
*********************************************************************************************************/
mount_point_t *vfs_mount_point_lookup2_ref(char pathbuf[PATH_BUF_LEN], char **ppath, const char *path)
{
    mount_point_t *point;

    mutex_lock(&mount_point_lock, 0);

    point = vfs_mount_point_lookup2(pathbuf, ppath, path);

    if (point != NULL) {
        atomic_inc(&point->ref);
    }

    mutex_unlock(&mount_point_lock);

    return point;
}
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

    mutex_lock(&mount_point_lock, 0);
    point = mount_point_lookup(point_name);                             /*  查找挂载点                  */
    if (point != NULL) {                                                /*  没找到                      */
        mutex_unlock(&mount_point_lock);
        seterrno(EINVAL);
        return -1;
    }

    mutex_lock(&file_system_lock, 0);
    fs = file_system_lookup(fs_name);                                   /*  查找文件系统                */
    if (fs == NULL) {
        mutex_unlock(&file_system_lock);
        mutex_unlock(&mount_point_lock);
        seterrno(EINVAL);
        return -1;
    }
    atomic_inc(&fs->ref);
    mutex_unlock(&file_system_lock);

    if (dev_name != NULL) {
        mutex_lock(&device_lock, 0);
        dev = device_lookup(dev_name);                                  /*  查找设备                    */
        if (dev == NULL) {
            atomic_dec(&fs->ref);
            mutex_unlock(&device_lock);
            mutex_unlock(&mount_point_lock);
            seterrno(ENODEV);
            return -1;
        }

        if (atomic_inc_return(&dev->ref) != 1) {
            atomic_dec(&dev->ref);
            atomic_dec(&fs->ref);
            mutex_unlock(&device_lock);
            mutex_unlock(&mount_point_lock);
            seterrno(EBUSY);
            return -1;
        }
        mutex_unlock(&device_lock);
    }

    point = kmalloc(sizeof(mount_point_t), GFP_KERNEL);                 /*  分配挂载点                  */
    if (point == NULL) {
        if (dev != NULL) {
            atomic_dec(&dev->ref);
        }
        atomic_dec(&fs->ref);
        mutex_unlock(&mount_point_lock);
        seterrno(ENOMEM);
        return -1;
    }

    if (point_name[0] == '/') {                                         /*  保证挂载点以 / 号开始       */
        strlcpy(point->name, point_name, sizeof(point->name));
    } else {
        snprintf(point->name, sizeof(point->name), "/%s", point_name);
    }

    if (point->name[1] != '\0') {                                       /*  如果不是根文件系统          */
        if (strchr(point->name + 1, '/') != NULL) {                     /*  保证挂载点不能再出现 / 号   */
                                                                        /*  因为我不知道 / 号还有几个   */
            kfree(point);
            if (dev != NULL) {
                atomic_dec(&dev->ref);
            }
            atomic_dec(&fs->ref);
            mutex_unlock(&mount_point_lock);
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
    ret = fs->mount(point, dev, dev_name, param);                       /*  挂载                        */
    if (ret < 0) {
        if (rootfs_point == point) {
            rootfs_point =  NULL;
        }
        kfree(point);
        if (dev != NULL) {
            atomic_dec(&dev->ref);
        }
        atomic_dec(&fs->ref);
        mutex_unlock(&mount_point_lock);
        return -1;
    }

    mount_point_install(point);                                         /*  安装挂载点                  */
    mutex_unlock(&mount_point_lock);

    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_unmount
** Descriptions:            取消挂载文件系统
** input parameters:        path                目录 PATH
**                          param               参数
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

    mutex_lock(&mount_point_lock, 0);

    point = vfs_mount_point_lookup2(pathbuf, &filepath, path);          /*  查找挂载点                  */
    if (point == NULL) {
        mutex_unlock(&mount_point_lock);
        kfree(pathbuf);
        return -1;
    }

    if (point->fs->unmount == NULL) {
        mutex_unlock(&mount_point_lock);
        kfree(pathbuf);
        seterrno(ENOSYS);
        return -1;
    }

    if (atomic_read(&point->ref) != 0) {
        mutex_unlock(&mount_point_lock);
        kfree(pathbuf);
        seterrno(EBUSY);
        return -1;
    }

    vfs_sync(path);

    seterrno(0);
    ret = point->fs->unmount(point, param);                             /*  取消挂载文件系统            */
    if (ret == 0) {
        if (point->dev != NULL) {
            atomic_dec(&point->dev->ref);
        }
        atomic_dec(&point->fs->ref);
        mount_point_remove(point);
    }

    mutex_unlock(&mount_point_lock);

    kfree(pathbuf);

    return ret;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
