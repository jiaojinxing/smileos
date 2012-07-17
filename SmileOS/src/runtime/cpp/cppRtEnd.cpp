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
** Descriptions:            ����ϵͳƽ̨ C++ run time ȫ�ֶ��󹹽�������������.
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
  ˵��:

  ������(gcc)���� C++ ����ʱ, �������ȫ�ֶ���, ��ôȫ�ֶ���Ĺ�������ָ�����ڿ�ִ�� elf �ļ��� .ctors
  ����(section), ������������ڿ�ִ�� elf �ļ��� .dtors ����, һ���׼ gcc ��������ĸ�����:

  __CTOR_LIST__
  __CTOR_END__
  __DTOR_LIST__
  __DTOR_END__

  ���� __CTOR_LIST__ ��ʾ���е�ȫ�ֶ����캯��ָ��������׵�ַ, ��ʼָ��Ϊ 0xFFFFFFFF, ֮���ÿһ��Ϊ
  һ�����캯�������, ֱ�� __CTOR_END__ Ϊֹ, __CTOR_END__ ָ��ĺ���ָ��Ϊ 0x00000000

  ���� __DTOR_LIST__ ��ʾ���е�ȫ�ֶ�����������ָ��������׵�ַ, ��ʼָ��Ϊ 0xFFFFFFFF, ֮���ÿһ��Ϊ
  һ���������������, ֱ�� __DTOR_END__ Ϊֹ, __DTOR_END__ ָ��ĺ���ָ��Ϊ 0x00000000

  һ�´����ʵ������� 4 ���������ƵĶ���. ����ϵͳ�Ϳ����������û�����֮ǰ, ��ʼ�� C++ ����, ����ȫ��
  ����Ĺ��캯��, ��ϵͳ reboot ʱ, ����ϵͳ����������.

  ���Ҫ����Щ���Ŵ��ڶ�Ӧ .ctors �� .dtors ����ָ����λ��, ����Ҫ�������ļ�����һ�´���:

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

  �������ӽű�, ����Ҫ�ķ��Ŷ��嵽�� .ctors .dtors ������Ӧ��λ�� (�ֱ��嵽����������������β)
  (���� .init_array �� .fini_array �ֱ��ǹ������������о�̬�洢ʱ�޵Ķ���)

  ע��:

  ���ڲ���ϵͳ���ڵ����û�֮ǰ, ��������ȫ�ֶ����캯��, ��ʱ��û�н�������񻷾�, ���Զ���Ĺ��캯��һ��
  Ҫ�㹻�ļ�, һ�����������ʼ����������Ժ�һЩ�������ݽṹ, ����Ĳ������������м���ר�ŵĳ�ʼ������
  ��ʵ��.
*********************************************************************************************************/
/*********************************************************************************************************
  C++ ȫ�ֶ��󹹽����������������ָ�� (Ϊ�˺�һЩ��������������ͻ, ����ʹ�� SylixOS �Դ��ķ���)
*********************************************************************************************************/
typedef void (*VOIDFUNCPTR)(void);

static VOIDFUNCPTR __LW_CTOR_END__[1] __attribute__((section(".ctors"))) = { (VOIDFUNCPTR)NULL };
static VOIDFUNCPTR __LW_DTOR_END__[1] __attribute__((section(".dtors"))) = { (VOIDFUNCPTR)NULL };
/*********************************************************************************************************
** ��������: __cppRtDummy
** ��������: Ϊ��ʹ���������Ż����������������, �������Ҫ��һ������ʹ�ô˷���
** �䡡��  : NONE
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
void __cppRtDummy(void)
{
    __LW_CTOR_END__[0] = (VOIDFUNCPTR)NULL;
    __LW_DTOR_END__[0] = (VOIDFUNCPTR)NULL;
}
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
