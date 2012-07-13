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
** File name:               libbb.h
** Last modified Date:      2012-4-14
** Last Version:            1.0.0
** Descriptions:            vi �༭����ֲ��
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-14
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
#ifndef LIBBB_H_
#define LIBBB_H_
/*********************************************************************************************************
  vi ������ͷ�ļ�
*********************************************************************************************************/
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/select.h>
/*********************************************************************************************************
  vi ����
*********************************************************************************************************/
#ifndef  FAST_FUNC
#define  FAST_FUNC
#endif

#ifndef  ALIGN1
#define  ALIGN1
#endif

#ifndef  MAIN_EXTERNALLY_VISIBLE
#define  MAIN_EXTERNALLY_VISIBLE
#endif

typedef  int                                    smallint;
typedef  unsigned                               smalluint;

#define  ENABLE_LOCALE_SUPPORT                  0
#define  ENABLE_FEATURE_ALLOW_EXEC              0

#define  ENABLE_FEATURE_VI_8BIT                 1
#define  ENABLE_FEATURE_VI_OPTIMIZE_CURSOR      1
#define  ENABLE_FEATURE_VI_USE_SIGNALS          0
#define  ENABLE_FEATURE_VI_CRASHME              0
#define  ENABLE_FEATURE_VI_DOT_CMD              1
#define  ENABLE_FEATURE_VI_YANKMARK             1
#define  ENABLE_FEATURE_VI_READONLY             0
#define  ENABLE_FEATURE_VI_SEARCH               1
#define  ENABLE_FEATURE_VI_COLON                1
#define  ENABLE_FEATURE_VI_SETOPTS              1
#define  ENABLE_FEATURE_VI_WIN_RESIZE           1
#define  ENABLE_FEATURE_VI_SET                  1

#define  CONFIG_FEATURE_VI_MAX_LEN              BUFSIZ

#define  RESERVE_CONFIG_BUFFER(buffer, len)     char buffer[len]

#define  BB_VER                                 "busybox ver: 1.10.2"
#define  BB_BT                                  ""

#ifndef  __cplusplus
#define  inline                                 __inline__
#endif

#if      ENABLE_FEATURE_VI_COLON
#define  USE_FEATURE_VI_COLON(x)                x
#else
#define  USE_FEATURE_VI_COLON(x)
#endif

#if      ENABLE_FEATURE_VI_READONLY
#define  USE_FEATURE_VI_READONLY(...)           __VA_ARGS__
#else
#define  USE_FEATURE_VI_READONLY(...)
#endif

#if      ENABLE_FEATURE_VI_YANKMARK
#define  USE_FEATURE_VI_YANKMARK(x)             x
#else
#define  USE_FEATURE_VI_YANKMARK(x)
#endif

#if      ENABLE_FEATURE_VI_SEARCH
#define  USE_FEATURE_VI_SEARCH(x)               x
#else
#define  USE_FEATURE_VI_SEARCH(x)
#endif

#define  ARRAY_SIZE(x)                          (sizeof(x) / sizeof((x)[0]))

#define  SET_PTR_TO_GLOBALS(x)                              \
         do {                                               \
             (*(struct globals**)&ptr_to_globals) = (x);    \
         } while (0)

#define  bb_show_usage()

/*********************************************************************************************************
  ��������
*********************************************************************************************************/
/*
 * ��ȫд
 */
ssize_t FAST_FUNC safe_write(int fd, const void *buf, size_t len);

/*
 * ��ȫ��
 */
ssize_t FAST_FUNC safe_read(int fd, void *buf, size_t len);
/*
 * ��ȫд
 */
ssize_t FAST_FUNC full_write(int fd, const void *buf, size_t len);

/*
 * ����ն˿��
 */
void FAST_FUNC get_terminal_width_height(int fd, int *width, int *height);

/*
 * �ж����һ���ַ��Ƿ�ָ�����ַ�
 */
char* FAST_FUNC last_char_is(const char *s, int c);

/*
 * ���һ���ַ�
 */
int FAST_FUNC bb_putchar(int c);

/*
 * �ͷ� getopt ������ڴ�
 */
void getopt_free(void);

void *memrchr(const void *ptr, int ch, size_t len);

char *strchrnul(const char *str, int ch);

char *xstrndup(const char *str, size_t len);

char *xstrdup(const char *str);

void *xmalloc(size_t len);

void *xzalloc(size_t len);

#endif                                                                  /*  LIBBB_H_                    */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
