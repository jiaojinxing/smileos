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
** File name:               rpc_smileos_fix.c
** Last modified Date:      2010-8-24
** Last Version:            1.0.0
** Descriptions:            ��ֲ sunrpc (���� glibc2.9) �� SylixOS.
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2010-8-24
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
#include <stdio.h>
#include <stdarg.h>

int __fxprintf(FILE *fp, const char *fmt, ...)
{
    int         ret;
    va_list     va;

    if (fp == NULL) {                                                   /*  Ĭ���������׼�����ļ�      */
        fp = stderr;
    }

    va_start(va, fmt);
    ret = vfprintf(fp, fmt, va);
    va_end(va);

    return  (ret);
}
/*********************************************************************************************************
**  END
*********************************************************************************************************/
