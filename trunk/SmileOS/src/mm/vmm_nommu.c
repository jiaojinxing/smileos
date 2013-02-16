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
** File name:               vmm_nommu.c
** Last modified Date:      2013-2-3
** Last Version:            1.0.0
** Descriptions:            �� MMU �������ڴ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-3
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
#include "kern/config.h"
#if CONFIG_MMU_EN == 1
/*********************************************************************************************************
** Function name:           vmm_init
** Descriptions:            ��ʼ�������ڴ����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void vmm_init(void)
{

}
/*********************************************************************************************************
** Function name:           arch_mmu_init
** Descriptions:            ����ת����, ��ʼ�� MMU Cache ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
__attribute__ ((weak)) void arch_mmu_init(void);
void arch_mmu_init(void)
{

}
/*********************************************************************************************************
** Function name:           ua_to_ka
** Descriptions:            �����̿ռ�������ַת��Ϊ�ں˿��Դ���������ַ
** input parameters:        VA                  ���̿ռ�������ַ
** output parameters:       NONE
** Returned value:          �ں˿��Դ���������ַ
*********************************************************************************************************/
void *ua_to_ka(const void *uaddr)
{
    return (void *)uaddr;
}
/*********************************************************************************************************
** Function name:           ka_to_ua
** Descriptions:            ���ں˿��Դ���������ַת��Ϊ���̿ռ�������ַ
** input parameters:        kaddr               �ں˿��Դ���������ַ
** output parameters:       NONE
** Returned value:          ���̿ռ�������ַ
*********************************************************************************************************/
void *ka_to_ua(const void *kaddr)
{
    return (void *)kaddr;
}
#endif
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
