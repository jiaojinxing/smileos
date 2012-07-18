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
** Descriptions:            ����ϵͳƽ̨ C++ run time eabi(ARM) lib.
**                          See http://www.codesourcery.com/public/cxx-abi/abi.html.
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-17
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
#include <stddef.h>
/*********************************************************************************************************
  C ��������
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
  C++ �����Ϊ����: ȫ�ֶ���, ��̬�ֲ�����, ��̬����

  ȫ�ֶ���: ������ C �����е�ȫ�ֱ���, �� main ����ִ��ǰ, ��Ҫ��ʼ��ȫ�ֱ���.
  ͬ��, ������ C++ ����ǰ, ���빹�����е�ȫ�ֶ���, ������������� C++ ȫ�ֶ���Ĺ��캯���Ժ���ָ���ķ�ʽ
  �������½���: .ctors .ctors.* .preinit_array .init_array ��.
  SmileOS ����ִ���û�����֮ǰ����� __cppRtDoCtors() ������ִ������ȫ�ֶ���Ĺ��캯��.
  ͬ��, ��Ӧ�ó����˳�ʱ��Ҫ��������ȫ�ֶ���, ���������ֿ�����:

  ��һ: �������������ȫ�ֶ�������������Ժ���ָ���ķ�ʽ�������½���: .dtors .dtors.* .fini_array ��.
  �빹������, ��Ӧ�ó����˳�ʱ����� __cppRtDoDtors ����, ִ������ȫ�ֶ������������.

  �ڶ�: ���������Զ��ڹ��캯�������һ���ִ���, ������: ִ���깹�캯��, ������������������, ����������
  ͨ�� __aeabi_atexit ѹ�� atexit() ջ��, ��Ӧ�ó����˳�ʱ����� __cxa_finalize() ����, ִ�����ж����
  ��������.

  ��̬�ֲ�����: �ڵ�һ�ε�����������ʱ������, ������Ϻ�Ὣ�ö������������ͨ�� __aeabi_atexit
  ѹ�� atexit() ջ��, ��Ӧ�ó����˳�ʱ������ __cxa_finalize() ����, ִ�����ж������������.

  ��̬����: ��ͨ��ִ�� new �� delete ���������ε�ִ�й��������������, ����ϵͳ��û������Ĵ���.
*********************************************************************************************************/
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
