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
** File name:               sys_call.c
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
#include "config.h"
#include "types.h"
#include "sys_call.h"
#include <stdarg.h>
#include <stdio.h>

/*
 * �����˳�
 */
int exit(int error_code)
{
    int ret;

    __asm__(
        "mov    r0, %1\n"                                               /*  r0 �ǲ��� 1                 */
        "mov    r7, %2\n"                                               /*  r7 ��ϵͳ���ú�             */
        "stmdb  sp!, {lr}\n"                                            /*  ���� LR ����ջ              */
        "swi    0\n"                                                    /*  ����ж�                    */
        "ldmia  sp!, {lr}\n"                                            /*  �Ӷ�ջ�ָ� LR               */
        "mov    %0, r0\n"                                               /*  r0 �Ƿ���ֵ                 */
        :"=r"(ret)
        :"r"(error_code), "M"(SYS_CALL_EXIT)
        :"r0"
        );
    return ret;
}

/*
 * ��������
 */
int sleep(int time)
{
    int ret;

    __asm__(
        "mov    r0, %1\n"                                               /*  r0 �ǲ��� 1                 */
        "mov    r7, %2\n"                                               /*  r7 ��ϵͳ���ú�             */
        "stmdb  sp!, {lr}\n"                                            /*  ���� LR ����ջ              */
        "swi    0\n"                                                    /*  ����ж�                    */
        "ldmia  sp!, {lr}\n"                                            /*  �Ӷ�ջ�ָ� LR               */
        "mov    %0, r0\n"                                               /*  r0 �Ƿ���ֵ                 */
        :"=r"(ret)
        :"r"(time), "M"(SYS_CALL_SLEEP)
        :"r0"
        );
    return ret;
}

/*
 * д
 */
int write(char *str)
{
    int ret;

    __asm__(
        "mov    r0, %1\n"
        "mov    r7, %2\n"
        "stmdb  sp!, {lr}\n"
        "swi    0\n"
        "ldmia  sp!, {lr}\n"
        "mov    %0, r0\n"
        :"=r"(ret)
        :"r"(str), "M"(SYS_CALL_WRITE)
        :"r0"
        );
    return ret;
}

/*
 * printf
 */
int printf(const char *fmt, ...)
{
    va_list va;
    char    buf[128];

    va_start(va, fmt);

    vsprintf(buf, fmt, va);

    write(buf);

    va_end(va);

    return 0;
}

/*
 * malloc
 */
void *malloc(uint32_t size)
{
    void *ret;

    __asm__(
        "mov    r0, %1\n"
        "mov    r7, %2\n"
        "stmdb  sp!, {lr}\n"
        "swi    0\n"
        "ldmia  sp!, {lr}\n"
        "mov    %0, r0\n"
        :"=r"(ret)
        :"r"(size), "M"(SYS_CALL_MALLOC)
        :"r0"
        );
    return ret;
}

/*
 * free
 */
void *free(void *ptr)
{
    void *ret;

    __asm__(
        "mov    r0, %1\n"
        "mov    r7, %2\n"
        "stmdb  sp!, {lr}\n"
        "swi    0\n"
        "ldmia  sp!, {lr}\n"
        "mov    %0, r0\n"
        :"=r"(ret)
        :"r"(ptr), "M"(SYS_CALL_FREE)
        :"r0"
        );
    return ret;
}

/*
 * heap_init
 */
int heap_init(uint8_t *base, uint32_t size)
{
    void *ret;

    __asm__(
        "mov    r0, %1\n"
        "mov    r1, %2\n"
        "mov    r7, %3\n"
        "stmdb  sp!, {lr}\n"
        "swi    0\n"
        "ldmia  sp!, {lr}\n"
        "mov    %0, r0\n"
        :"=r"(ret)
        :"r"(base), "r"(size), "M"(SYS_CALL_HEAP_INIT)
        :"r0"
        );
    return ret;
}

void _sbrk(void)
{

}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
