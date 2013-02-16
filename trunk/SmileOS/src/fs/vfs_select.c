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
** File name:               vfs_select.c
** Last modified Date:      2012-3-20
** Last Version:            1.0.0
** Descriptions:            虚拟文件系统的 select 接口
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
** Function name:           vfs_select_scan
** Descriptions:            扫描文件集
** input parameters:        nfds                文件描述符范围
**                          readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** output parameters:       readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** Returned value:          文件集总共设置了多少个位
*********************************************************************************************************/
static int vfs_select_scan(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds)
{
    fd_set  rfds, wfds, efds;
    int     flags;
    int     i;
    int     nset;
    int     ret;

    nset = 0;

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    for (i = 0; i < nfds; i++) {

        flags = 0;

        if (readfds && FD_ISSET(i, readfds)) {
            flags |= VFS_FILE_READABLE;
        }

        if (writefds && FD_ISSET(i, writefds)) {
            flags |= VFS_FILE_WRITEABLE;
        }

        if (errorfds && FD_ISSET(i, errorfds)) {
            flags |= VFS_FILE_ERROR;
        }

        if (flags != 0) {

            ret = vfs_scan_file(i, flags);

            if (ret < 0) {

                if (readfds  != NULL) {
                    FD_ZERO(readfds);
                }

                if (writefds != NULL) {
                    FD_ZERO(writefds);
                }

                if (errorfds != NULL) {
                    FD_ZERO(errorfds);
                }

                return -1;

            } else if (ret > 0) {

                if (readfds && ret & VFS_FILE_READABLE) {
                    FD_SET(i, &rfds);
                    nset++;
                }

                if (writefds && ret & VFS_FILE_WRITEABLE) {
                    FD_SET(i, &wfds);
                    nset++;
                }

                if (errorfds && ret & VFS_FILE_ERROR) {
                    FD_SET(i, &efds);
                    nset++;
                }
            } else {
                continue;
            }
        }
    }

    if (nset > 0) {
        if (readfds) {
            *readfds  = rfds;
        }

        if (writefds) {
            *writefds = wfds;
        }

        if (errorfds) {
            *errorfds = efds;
        }

        return nset;
    } else {
        return 0;
    }
}
/*********************************************************************************************************
** Function name:           vfs_select_unselect
** Descriptions:            将当前任务从文件集的等待列表中移除
** input parameters:        nfds                文件描述符范围
**                          readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** output parameters:       NONE
** Returned value:          0
*********************************************************************************************************/
static int vfs_select_unselect(int nfds, const fd_set *readfds, const fd_set *writefds, const fd_set *errorfds)
{
    int flags;
    int i;

    for (i = 0; i < nfds; i++) {

        flags = 0;

        if (readfds && FD_ISSET(i, readfds)) {
            flags |= VFS_FILE_READABLE;
        }

        if (writefds && FD_ISSET(i, writefds)) {
            flags |= VFS_FILE_WRITEABLE;
        }

        if (errorfds && FD_ISSET(i, errorfds)) {
            flags |= VFS_FILE_ERROR;
        }

        if (flags != 0) {
            vfs_unselect_file(i, flags);
        }
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_select_select
** Descriptions:            将当前任务加入到文件集的等待列表
** input parameters:        nfds                文件描述符范围
**                          readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int vfs_select_select(int nfds, const fd_set *readfds, const fd_set *writefds, const fd_set *errorfds)
{
    int flags;
    int i;
    int ret;

    for (i = 0; i < nfds; i++) {

        flags = 0;

        if (readfds && FD_ISSET(i, readfds)) {
            flags |= VFS_FILE_READABLE;
        }

        if (writefds && FD_ISSET(i, writefds)) {
            flags |= VFS_FILE_WRITEABLE;
        }

        if (errorfds && FD_ISSET(i, errorfds)) {
            flags |= VFS_FILE_ERROR;
        }

        if (flags != 0) {

            ret = vfs_select_file(i, flags);

            if (ret < 0) {
                vfs_select_unselect(i, readfds, writefds, errorfds);
                return -1;
            }
        }
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_select
** Descriptions:            将当前任务加入到文件集的等待列表
** input parameters:        nfds                文件描述符范围
**                          readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** output parameters:       readfds             可读文件集
**                          writefds            可写文件集
**                          errorfds            出错文件集
** Returned value:          文件集总共设置了多少个位 OR -1
*********************************************************************************************************/
int vfs_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
{
    int         resume_type;
    int         nset;
    int         ret;
    reg_t       reg;

    if (nfds < 0 || nfds > FD_SETSIZE) {                                /*  检查 nfds                   */

        if (readfds  != NULL) {
            FD_ZERO(readfds);
        }

        if (writefds != NULL) {
            FD_ZERO(writefds);
        }

        if (errorfds != NULL) {
            FD_ZERO(errorfds);
        }

        seterrno(EINVAL);
        return -1;
    }

    if (timeout != NULL) {                                              /*  检查 timeout                */

        if (   timeout->tv_sec  < 0
            || timeout->tv_usec < 0
            || timeout->tv_usec >= 1000000 /* a full second */) {

            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }

            if (writefds != NULL) {
                FD_ZERO(writefds);
            }

            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }

            seterrno(EINVAL);
            return -1;
        }

        if (timeout->tv_sec > 31 * 24 * 60 * 60) {                      /*  修正 timeout                */
            timeout->tv_sec = 31 * 24 * 60 * 60;
        }
    }

    if (nfds == 0) {                                                    /*  nfds 为 0 的情况            */

        if (timeout != NULL) {                                          /*  timeout 不为 NULL, 即只延时 */

            if (timeout->tv_sec != 0) {
                sleep(timeout->tv_sec);                                 /*  延时                        */
            }

            if (timeout->tv_usec != 0) {
                usleep(timeout->tv_usec);                               /*  延时                        */
            }

            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }

            if (writefds != NULL) {
                FD_ZERO(writefds);
            }

            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }

            seterrno(0);
            return 0;
        } else {                                                        /*  参数错误                    */
            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }

            if (writefds != NULL) {
                FD_ZERO(writefds);
            }

            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }

            seterrno(EINVAL);
            return -1;
        }
    }

    reg  = interrupt_disable();

    nset = vfs_select_scan(nfds, readfds, writefds, errorfds);          /*  扫描文件集                  */

    if (nset > 0) {                                                     /*  有文件在文件集中置位了      */

        interrupt_resume(reg);

        seterrno(0);
        return nset;

    } else if (nset < 0) {                                              /*  出错了                      */

        interrupt_resume(reg);

        return -1;
    }

    /*
     * 无文件在文件集中置位
     */
                                                                        /*  上层不想休眠                */
    if (timeout != NULL && (timeout->tv_sec == 0 && timeout->tv_usec == 0)) {

        interrupt_resume(reg);

        if (readfds  != NULL) {
            FD_ZERO(readfds);
        }

        if (writefds != NULL) {
            FD_ZERO(writefds);
        }

        if (errorfds != NULL) {
            FD_ZERO(errorfds);
        }

        seterrno(0);
        return 0;
    }

    /*
     * 将当前任务加入到文件集的等待列表
     */
    ret = vfs_select_select(nfds, readfds, writefds, errorfds);

    if (ret < 0) {

        interrupt_resume(reg);

        if (readfds  != NULL) {
            FD_ZERO(readfds);
        }

        if (writefds != NULL) {
            FD_ZERO(writefds);
        }

        if (errorfds != NULL) {
            FD_ZERO(errorfds);
        }

        return -1;
    }

    if (timeout == NULL) {                                              /*  上层要求等待                */
        current->delay   = 0;
        current->status  = TASK_SUSPEND;
    } else {                                                            /*  上层要求休眠                */
        current->delay   = timeout->tv_sec  * TICK_PER_SECOND +
                           timeout->tv_usec * TICK_PER_SECOND / 1000000;
        current->status  = TASK_SLEEPING;
    }
    current->resume_type = TASK_RESUME_UNKNOW;

    schedule();                                                         /*  释放 CPU 使用               */

    /*
     * 将当前任务从文件集的等待列表中移除
     */
    vfs_select_unselect(nfds, readfds, writefds, errorfds);

    current->delay       = 0;
    resume_type          = current->resume_type;
    current->resume_type = TASK_RESUME_UNKNOW;

    if (resume_type & TASK_RESUME_SELECT_EVENT) {                       /*  因 select 事件恢复          */

        nset = vfs_select_scan(nfds, readfds, writefds, errorfds);      /*  扫描文件集                  */

        interrupt_resume(reg);

        if (nset > 0) {                                                 /*  有文件在文件集中置位了      */
            seterrno(0);
            return nset;
        }

        if (nset == 0) {                                                /*  无文件在文件集中置位了      */

            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }

            if (writefds != NULL) {
                FD_ZERO(writefds);
            }

            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }

            seterrno(0);
            return 0;
        } else {
            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }

            if (writefds != NULL) {
                FD_ZERO(writefds);
            }

            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }

            return -1;
        }
    } else {                                                            /*  不是因 select 事件恢复      */

        interrupt_resume(reg);

        if (resume_type & TASK_RESUME_INTERRUPT) {

            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }

            if (writefds != NULL) {
                FD_ZERO(writefds);
            }

            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }

            seterrno(EINTR);

            return -1;
        } else {
            if (readfds  != NULL) {
                FD_ZERO(readfds);
            }

            if (writefds != NULL) {
                FD_ZERO(writefds);
            }

            if (errorfds != NULL) {
                FD_ZERO(errorfds);
            }

            seterrno(EAGAIN);

            return 0;
        }
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
