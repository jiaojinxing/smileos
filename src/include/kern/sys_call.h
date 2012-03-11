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
** File name:               sys_call.h
** Last modified Date:      2012-2-22
** Last Version:            1.0.0
** Descriptions:            ϵͳ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-22
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
#ifndef SYS_CALL_H_
#define SYS_CALL_H_

/*
 * ϵͳ���ú�
 */
#define SYS_CALL_EXIT       0
#define SYS_CALL_SLEEP      1
#define SYS_CALL_WRITE      2
#define SYS_CALL_GETTIME    3
#define SYS_CALL_GETPID     4
#define SYS_CALL_ERRNO      5
#define SYS_CALL_YIELD      6
#define SYS_CALL_NR         7                                           /*  ϵͳ������                  */

#ifndef __ASSEMBLER__
#include <sys/time.h>

/*
 * exit
 */
void exit(int error_code);

/*
 * abort
 */
void abort(void);

/*
 * yield
 */
void yield(void);

/*
 * sleep
 */
void sleep(unsigned int s);

/*
 * usleep
 */
void usleep(unsigned int us);

/*
 * write
 */
int write(int fd, const char *data, unsigned int size);

/*
 * printf
 */
int printf(const char *fmt, ...);

/*
 * puts
 */
int puts(const char *str);

/*
 * select
 */
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);

/*
 * getpid
 */
int getpid(void);
#endif

#endif                                                                  /*  SYS_CALL_H_                 */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
