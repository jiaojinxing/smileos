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
** Descriptions:            无 MMU 的虚拟内存管理
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-3
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
#include "kern/config.h"
#if CONFIG_MMU_EN == 1
/*********************************************************************************************************
** Function name:           vmm_init
** Descriptions:            初始化虚拟内存管理
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void vmm_init(void)
{

}
/*********************************************************************************************************
** Function name:           arch_mmu_init
** Descriptions:            初始化 MMU Cache 等
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void arch_mmu_init(void) __attribute__ ((weak));
void arch_mmu_init(void)
{

}
/*********************************************************************************************************
** Function name:           ua_to_ka
** Descriptions:            将进程空间的虚拟地址转换为内核可以处理的虚拟地址
** input parameters:        VA                  进程空间的虚拟地址
** output parameters:       NONE
** Returned value:          内核可以处理的虚拟地址
*********************************************************************************************************/
void *ua_to_ka(const void *uaddr) __attribute__ ((weak));
void *ua_to_ka(const void *uaddr)
{
    return (void *)uaddr;
}
/*********************************************************************************************************
** Function name:           ka_to_ua
** Descriptions:            将内核可以处理的虚拟地址转换为进程空间的虚拟地址
** input parameters:        kaddr               内核可以处理的虚拟地址
** output parameters:       NONE
** Returned value:          进程空间的虚拟地址
*********************************************************************************************************/
void *ka_to_ua(const void *kaddr) __attribute__ ((weak));
void *ka_to_ua(const void *kaddr)
{
    return (void *)kaddr;
}
#endif
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
