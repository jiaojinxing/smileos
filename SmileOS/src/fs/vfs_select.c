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
** Descriptions:            �����ļ�ϵͳ�� select �ӿ�
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
** Function name:           vfs_select_scan
** Descriptions:            ɨ���ļ���
** input parameters:        nfds                �ļ���������Χ
**                          readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** output parameters:       readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
** Returned value:          �ļ����ܹ������˶��ٸ�λ
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
** Descriptions:            ����ǰ������ļ����ĵȴ��б����Ƴ�
** input parameters:        nfds                �ļ���������Χ
**                          readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
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
** Descriptions:            ����ǰ������뵽�ļ����ĵȴ��б�
** input parameters:        nfds                �ļ���������Χ
**                          readfds             �ɶ��ļ���
**                          writefds            ��д�ļ���
**                          errorfds            �����ļ���
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
int vfs_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
{
    int         resume_type;
    int         nset;
    int         ret;
    reg_t       reg;

    if (nfds < 0 || nfds > FD_SETSIZE) {                                /*  ��� nfds                   */

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

    if (timeout != NULL) {                                              /*  ��� timeout                */

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

        if (timeout->tv_sec > 31 * 24 * 60 * 60) {                      /*  ���� timeout                */
            timeout->tv_sec = 31 * 24 * 60 * 60;
        }
    }

    if (nfds == 0) {                                                    /*  nfds Ϊ 0 �����            */

        if (timeout != NULL) {                                          /*  timeout ��Ϊ NULL, ��ֻ��ʱ */

            if (timeout->tv_sec != 0) {
                sleep(timeout->tv_sec);                                 /*  ��ʱ                        */
            }

            if (timeout->tv_usec != 0) {
                usleep(timeout->tv_usec);                               /*  ��ʱ                        */
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
        } else {                                                        /*  ��������                    */
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

    nset = vfs_select_scan(nfds, readfds, writefds, errorfds);          /*  ɨ���ļ���                  */

    if (nset > 0) {                                                     /*  ���ļ����ļ�������λ��      */

        interrupt_resume(reg);

        seterrno(0);
        return nset;

    } else if (nset < 0) {                                              /*  ������                      */

        interrupt_resume(reg);

        return -1;
    }

    /*
     * ���ļ����ļ�������λ
     */
                                                                        /*  �ϲ㲻������                */
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
     * ����ǰ������뵽�ļ����ĵȴ��б�
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

    if (timeout == NULL) {                                              /*  �ϲ�Ҫ��ȴ�                */
        current->delay   = 0;
        current->status  = TASK_SUSPEND;
    } else {                                                            /*  �ϲ�Ҫ������                */
        current->delay   = timeout->tv_sec  * TICK_PER_SECOND +
                           timeout->tv_usec * TICK_PER_SECOND / 1000000;
        current->status  = TASK_SLEEPING;
    }
    current->resume_type = TASK_RESUME_UNKNOW;

    schedule();                                                         /*  �ͷ� CPU ʹ��               */

    /*
     * ����ǰ������ļ����ĵȴ��б����Ƴ�
     */
    vfs_select_unselect(nfds, readfds, writefds, errorfds);

    current->delay       = 0;
    resume_type          = current->resume_type;
    current->resume_type = TASK_RESUME_UNKNOW;

    if (resume_type & TASK_RESUME_SELECT_EVENT) {                       /*  �� select �¼��ָ�          */

        nset = vfs_select_scan(nfds, readfds, writefds, errorfds);      /*  ɨ���ļ���                  */

        interrupt_resume(reg);

        if (nset > 0) {                                                 /*  ���ļ����ļ�������λ��      */
            seterrno(0);
            return nset;
        }

        if (nset == 0) {                                                /*  ���ļ����ļ�������λ��      */

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
    } else {                                                            /*  ������ select �¼��ָ�      */

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
