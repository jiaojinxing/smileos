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
** File name:               sbin.c
** Last modified Date:      2012-2-21
** Last Version:            1.0.0
** Descriptions:            ϵͳ�����̳���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-21
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
#include "kern/config.h"
#include "kern/types.h"
#include "wsIntrn.h"
#include <string.h>

extern websRomPageIndexType websRomPageIndex[];

/*
 * ���� sbin
 */
uint8_t *sbin_lookup(char *name, uint32_t *size)
{
    int i;

    for (i = 0; websRomPageIndex[i].path; i++) {
        if (!strcmp(name, websRomPageIndex[i].path)) {
            *size = websRomPageIndex[i].size;
            return websRomPageIndex[i].page;
        }
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/