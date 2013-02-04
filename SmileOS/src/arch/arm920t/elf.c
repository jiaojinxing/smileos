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
** File name:               elf.c
** Last modified Date:      2013-2-3
** Last Version:            1.0.0
** Descriptions:            elf 重定位
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-3
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
#include "module/elf.h"
/*********************************************************************************************************
** Relocation type
*********************************************************************************************************/
#define R_ARM_NONE          0
#define R_ARM_PC24          1
#define R_ARM_ABS32         2
#define R_ARM_PLT32         27
#define R_ARM_CALL          28
#define R_ARM_JUMP24        29
/*********************************************************************************************************
** Function name:           arch_relocate_rel
** Descriptions:            对指定的 rel 条目进行重定位
** input parameters:        rel                 REL 条目
**                          addr                地址
**                          target              目标节区
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int arch_relocate_rel(Elf32_Rel *rel, Elf32_Addr addr, char *target)
{
    Elf32_Addr *where, tmp;
    Elf32_Sword addend;

    where = (Elf32_Addr *)(target + rel->r_offset);

    switch (ELF32_R_TYPE(rel->r_info)) {
    case R_ARM_NONE:
        break;

    case R_ARM_ABS32:
        *where += addr;
        break;

    case R_ARM_PC24:
    case R_ARM_PLT32:
    case R_ARM_CALL:
    case R_ARM_JUMP24:
        addend = *where & 0x00ffffff;
        if (addend & 0x00800000) {
            addend |= 0xff000000;
        }
        tmp    = addr - (Elf32_Addr)where + (addend << 2);
        tmp  >>= 2;
        *where = (*where & 0xff000000) | (tmp & 0x00ffffff);
        break;

    default:
        return -1;
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           arch_relocate_rela
** Descriptions:            对指定的 rela 条目进行重定位
** input parameters:        rela                RELA 条目
**                          addr                地址
**                          target              目标节区
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int arch_relocate_rela(Elf32_Rela *rela, Elf32_Addr addr, char *target)
{
    return -1;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
