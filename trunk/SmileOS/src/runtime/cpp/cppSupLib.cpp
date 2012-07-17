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
** File name:               cppRtEnd.cpp
** Last modified Date:      2012-7-17
** Last Version:            1.0.0
** Descriptions:            操作系统平台 C++ run time 全局对象构建与析构操作库.
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
/*********************************************************************************************************
  C 环境函数
*********************************************************************************************************/
extern "C" {
/*********************************************************************************************************
** 函数名称: _cppRtInit
** 功能描述: C++ 运行时支持初始化
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
int _cppRtInit(void)
{
    extern void __cppRtDummy(void);
    extern void __cppRtDoCtors(void);

    __cppRtDummy();
    __cppRtDoCtors();                                                   /*  运行全局对象构造函数        */

    return 0;
}
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
