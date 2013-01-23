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
** File name:               poll.h
** Last modified Date:      2012-2-26
** Last Version:            1.0.0
** Descriptions:            poll 头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-26
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
#ifndef SYS_POLL_H_
#define SYS_POLL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int nfds_t;

/* fake a poll(2) environment */
#define POLLIN      0x0001      /* any readable data available   */
#define POLLPRI     0x0002      /* OOB/Urgent readable data      */
#define POLLOUT     0x0004      /* file descriptor is writeable  */
#define POLLERR     0x0008      /* some poll error occurred      */
#define POLLHUP     0x0010      /* file descriptor was "hung up" */
#define POLLNVAL    0x0020      /* requested events "invalid"    */
#define POLLRDNORM  POLLIN
#define POLLRDBAND  POLLIN
#define POLLWRNORM  POLLOUT
#define POLLWRBAND  POLLOUT
#define INFTIM      (-1)        /* poll infinite                 */

struct pollfd {
    int   fd;                   /* which file descriptor to poll */
    short events;               /* events we are interested in   */
    short revents;              /* events found on return        */
};
/*********************************************************************************************************
** Function name:           poll
** Descriptions:            使用 select 实现
** input parameters:        fds                 pollfd 结构数组指针
**                          nfds                pollfd 结构数组大小
**                          timeout             超时时间
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int poll(struct pollfd fds[], nfds_t nfds, int timeout);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  SYS_POLL_H_                 */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
