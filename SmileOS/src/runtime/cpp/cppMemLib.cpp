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
** Descriptions:            ����ϵͳƽ̨ C++ �ڴ�֧�� (���� new, delete �����).
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
#include <stdlib.h>
/*********************************************************************************************************
  C++ ר���ڴ濪�ٺ���
*********************************************************************************************************/
/*********************************************************************************************************
** ��������: new
** ��������: ���� new ����.
** �䡡��  : stSize        �ڴ��С
** �䡡��  : ������ڴ�ָ��.
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
void *operator new(size_t stSize)
{
    return (void *)malloc(stSize);
}
/*********************************************************************************************************
** ��������: new[]
** ��������: ���� new ����.
** �䡡��  : stSize        �ڴ��С
** �䡡��  : ������ڴ�ָ��.
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
void *operator new[](size_t stSize)
{
    return (void *)malloc(stSize);
}
/*********************************************************************************************************
** ��������: delete
** ��������: ���� delete ����.
** �䡡��  : pvMem         ��Ҫ�ͷŵ��ڴ���ָ��
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
void operator delete(void *pvMem)
{
    if (pvMem) {
        free(pvMem);
    }
}
/*********************************************************************************************************
** ��������: delete
** ��������: ���� delete ����.
** �䡡��  : pvMem         ��Ҫ�ͷŵ��ڴ���ָ��
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
void operator delete[](void *pvMem)
{
    if (pvMem) {
        free(pvMem);
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
