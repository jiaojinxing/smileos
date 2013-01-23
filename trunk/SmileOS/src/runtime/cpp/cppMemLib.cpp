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
** C++ ר���ڴ濪�ٺ���
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           new
** Descriptions:            ���� new ����
** input parameters:        size                �ڴ��С
** output parameters:       NONE
** Returned value:          ������ڴ�ָ��
*********************************************************************************************************/
void *operator new(size_t size)
{
    return (void *)malloc(size);
}
/*********************************************************************************************************
** Function name:           new[]
** Descriptions:            ���� new[] ����
** input parameters:        size                �ڴ��С
** output parameters:       NONE
** Returned value:          ������ڴ�ָ��
*********************************************************************************************************/
void *operator new[](size_t size)
{
    return (void *)malloc(size);
}
/*********************************************************************************************************
** Function name:           delete
** Descriptions:            ���� delete ����
** input parameters:        ptr                 ��Ҫ�ͷŵ��ڴ���ָ��
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
** Descriptions:            ���� delete ����
** input parameters:        ptr                 ��Ҫ�ͷŵ��ڴ���ָ��
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
