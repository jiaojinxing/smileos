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
** File name:               cppMemLib.cpp
** Last modified Date:      2012-7-17
** Last Version:            1.0.0
** Descriptions:            操作系统平台 C++ 内存支持 (重载 new, delete 运算符).
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-17
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
#include <stddef.h>
#include <stdlib.h>
/*********************************************************************************************************
  C++ 专用内存开辟函数
*********************************************************************************************************/
/*********************************************************************************************************
** 函数名称: new
** 功能描述: 重载 new 运算.
** 输　入  : stSize        内存大小
** 输　出  : 分配的内存指针.
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void *operator new(size_t stSize)
{
    return (void *)malloc(stSize);
}
/*********************************************************************************************************
** 函数名称: new[]
** 功能描述: 重载 new 运算.
** 输　入  : stSize        内存大小
** 输　出  : 分配的内存指针.
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void *operator new[](size_t stSize)
{
    return (void *)malloc(stSize);
}
/*********************************************************************************************************
** 函数名称: delete
** 功能描述: 重载 delete 运算.
** 输　入  : pvMem         需要释放的内存首指针
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void operator delete(void *pvMem)
{
    if (pvMem) {
        free(pvMem);
    }
}
/*********************************************************************************************************
** 函数名称: delete
** 功能描述: 重载 delete 运算.
** 输　入  : pvMem         需要释放的内存首指针
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void operator delete[](void *pvMem)
{
    if (pvMem) {
        free(pvMem);
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
