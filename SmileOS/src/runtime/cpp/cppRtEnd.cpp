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
** C 环境函数
*********************************************************************************************************/
extern "C" {
/*********************************************************************************************************
  说明:

  编译器(gcc)编译 C++ 工程时, 如果存在全局对象, 那么全局对象的构建函数指针会放在可执行 elf 文件的 .ctors
  节区(section), 析构函数会放在可执行 elf 文件的 .dtors 节区, 一般标准 gcc 库会引出四个符号:

  __CTOR_LIST__
  __CTOR_END__
  __DTOR_LIST__
  __DTOR_END__

  其中 __CTOR_LIST__ 表示所有的全局对象构造函数指针数组的首地址, 起始指针为 0xFFFFFFFF, 之后的每一项为
  一个构造函数的入口, 直到 __CTOR_END__ 为止, __CTOR_END__ 指向的函数指针为 0x00000000

  其中 __DTOR_LIST__ 表示所有的全局对象析构函数指针数组的首地址, 起始指针为 0xFFFFFFFF, 之后的每一项为
  一个析构函数的入口, 直到 __DTOR_END__ 为止, __DTOR_END__ 指向的函数指针为 0x00000000

  一下代码就实现了这个 4 个符号类似的定义. 这样系统就可以在运行用户程序之前, 初始化 C++ 环境, 运行全局
  对象的构造函数.

  如果要让这些符号处于对应 .ctors 和 .dtors 节区指定的位置, 则需要在连接文件加入一下代码:

  .ctors :
  {
      KEEP (*cppRtBegin*.o(.ctors))
      KEEP (*(.preinit_array))
      KEEP (*(.init_array))
      KEEP (*(SORT(.ctors.*)))
      KEEP (*(.ctors))
      KEEP (*cppRtEnd*.o(.ctors))
  }

  .dtors :
  {
      KEEP (*cppRtBegin*.o(.dtors))
      KEEP (*(.fini_array))
      KEEP (*(SORT(.dtors.*)))
      KEEP (*(.dtors))
      KEEP (*cppRtEnd*.o(.dtors))
  }

  以上链接脚本, 将需要的符号定义到了 .ctors .dtors 节区对应的位置 (分别定义到了这两个节区的首尾)
  (其中 .init_array 和 .fini_array 分别是构建和析构具有静态存储时限的对象)

  注意:

  由于操作系统是在调用用户之前, 就运行了全局对象构造函数, 此时并没有进入多任务环境, 所以对象的构造函数一定
  要足够的简单, 一般仅是用来初始化对象的属性和一些基本数据结构, 更多的操作可以在类中加入专门的初始化方法
  来实现.
*********************************************************************************************************/
/*********************************************************************************************************
** C++ 全局对象构建与析构函数表结束指针 (为了和一些编译器不产生冲突, 这里使用 SmileOS 自带的符号)
*********************************************************************************************************/
typedef void (*VOIDFUNCPTR)(void);

static VOIDFUNCPTR __MY_CTOR_END__[1] __attribute__((section(".ctors"))) = { (VOIDFUNCPTR)NULL };
static VOIDFUNCPTR __MY_DTOR_END__[1] __attribute__((section(".dtors"))) = { (VOIDFUNCPTR)NULL };
/*********************************************************************************************************
** Function name:           __cppRtDummy
** Descriptions:            为了使编译器不优化掉上面的两个符号, 这里必须要有一个函数使用此符号
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void __cppRtDummy(void)
{
    __MY_CTOR_END__[0] = (VOIDFUNCPTR)NULL;
    __MY_DTOR_END__[0] = (VOIDFUNCPTR)NULL;
}
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
