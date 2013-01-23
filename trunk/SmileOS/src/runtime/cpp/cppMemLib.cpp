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
** C++ 专用内存开辟函数
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           new
** Descriptions:            重载 new 运算
** input parameters:        size                内存大小
** output parameters:       NONE
** Returned value:          分配的内存指针
*********************************************************************************************************/
void *operator new(size_t size)
{
    return (void *)malloc(size);
}
/*********************************************************************************************************
** Function name:           new[]
** Descriptions:            重载 new[] 运算
** input parameters:        size                内存大小
** output parameters:       NONE
** Returned value:          分配的内存指针
*********************************************************************************************************/
void *operator new[](size_t size)
{
    return (void *)malloc(size);
}
/*********************************************************************************************************
** Function name:           delete
** Descriptions:            重载 delete 运算
** input parameters:        ptr                 需要释放的内存首指针
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void operator delete(void *ptr)
{
    if (ptr != NULL) {
        free(ptr);
    }
}
/*********************************************************************************************************
** Function name:           delete[]
** Descriptions:            重载 delete 运算
** input parameters:        ptr                 需要释放的内存首指针
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void operator delete[](void *ptr)
{
    if (ptr != NULL) {
        free(ptr);
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
