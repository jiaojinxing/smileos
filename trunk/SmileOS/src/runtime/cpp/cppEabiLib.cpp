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
** File name:               cppEabiLib.cpp
** Last modified Date:      2012-7-17
** Last Version:            1.0.0
** Descriptions:            操作系统平台 C++ run time eabi(ARM) lib.
**                          See http://www.codesourcery.com/public/cxx-abi/abi.html.
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
  Temporary hack: this variable should not be part of the C library
  itself, but placed in the .bss section of each executable or
  shared library instead.

  We keep it here temporarily until the build system has been
  modified properly to use crtbegin_so.S and crtend_so.S when
  generating shared libraries.

  It must be a 'weak' symbol to avoid conflicts with the definitions
  that have been moved to crtbegin_static.S and crtbegin_dynamic.S

  For the record, it is used for static C++ object construction
  and destruction. See http://www.codesourcery.com/public/cxx-abi/abi.html#dso-dtor
*********************************************************************************************************/
void *__attribute__((weak)) __dso_handle = &__dso_handle;
/*********************************************************************************************************
  C++ 对象分为三类: 全局对象, 静态局部对象, 动态对象

  全局对象:类似于 C 程序中的全局变量, 一般在 main 程序执行前, 需要初始化全局变量. 同样, C++ 程序要求在运行
  第一个 C++ 程序前, 必须构造所有的全局对象, 编译器会把所有 C++ 全局对象构造函数以函数指针表的方式放在以下
  节区: .ctors .ctors.* .preinit_array .init_array 中. SmileOS 系统在执行用户程序之前会自动调用
  __cppRtDoCtors() 函数来执行所有的全局对象构造函数(详情请参见相关代码).
  同样, 在应用程序退出时需要析构所有全局对象, 这里有两种可能性:

  第一: 编译器会把所有 C++ 全局对象析构函数以函数指针表的方式放在以下节区: .dtors .dtors.* .fini_array 中.
  与构造类似, 由于 SmileOS 本身就是一个进程, 所以 SmileOS 系统在停止时将自动调用 __cppRtDoDtors 函数来执行
  所有的全局对象析构函数.
  第二: 编译器会自动在构造函数后添加一部分代码, 功能是: 执行完构造函数, 如果对象存在析构函数, 将析构函数
  通过 __aeabi_atexit 压入 atexit() 栈区, 所以 SmileOS 系统在停止时将自动调用 __cxa_finalize() 函数, 执行
  所有对象的析构函数.

  静态局部对象:(函数请参考 cppSupLib.cpp 中事例)是在第一次调用容器函数时被构造, 构造完毕后会将改对象的析构
  函数通过 __aeabi_atexit 压入 atexit() 栈区, SmileOS 系统在停止时将自动调用 __cxa_finalize() 函数, 执行
  所有对象的析构函数.

  动态对象:是通过执行 new 和 delete 操作来隐形的执行构造和析构函数的, 操作系统并没有特殊的处理.
*********************************************************************************************************/
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
