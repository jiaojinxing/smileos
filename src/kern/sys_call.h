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

#define SYS_CALL_EXIT       0
#define SYS_CALL_SLEEP      1
#define SYS_CALL_WRITE      2
#define SYS_CALL_MALLOC     3
#define SYS_CALL_FREE       4
#define SYS_CALL_HEAP_INIT  5
#define SYS_CALL_NR         6                                           /*  ϵͳ������                  */

#ifndef __ASSEMBLER__
/*
 * �����˳�
 */
int exit(int error_code);

/*
 * ��������
 */
int sleep(int time);

/*
 * д
 */
int write(char *str);

/*
 * printf
 */
int printf(const char *fmt, ...);

/*
 * malloc
 */
void *malloc(uint32_t size);

/*
 * free
 */
void *free(void *ptr);

/*
 * heap_init
 */
int heap_init(uint8_t *base, uint32_t size);
#endif

#endif                                                                  /*  SYS_CALL_H_                 */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
