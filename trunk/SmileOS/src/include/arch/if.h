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
** File name:               if.h
** Last modified Date:      2013-2-4
** Last Version:            1.0.0
** Descriptions:            与体系结构相关的接口
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-4
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
#ifndef ARCH_IF_H_
#define ARCH_IF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
** Function name:           soc_init
** Descriptions:            内核初始化 CPU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void soc_init(void);
/*********************************************************************************************************
** Function name:           soc_timer_init
** Descriptions:            初始化定时器
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void soc_timer_init(void);
/*********************************************************************************************************
** Function name:           interrupt_disable
** Descriptions:            进入临界区域
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          进入临界区域前的当前程序状态寄存器
*********************************************************************************************************/
reg_t interrupt_disable(void);
/*********************************************************************************************************
** Function name:           interrupt_resume
** Descriptions:            退出临界区域
** input parameters:        reg                 进入临界区域前的当前程序状态寄存器
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_resume(register reg_t reg);
/*********************************************************************************************************
** Function name:           interrupt_mask
** Descriptions:            屏蔽中断
** input parameters:        interrupt           中断号
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_mask(intno_t interrupt);
/*********************************************************************************************************
** Function name:           interrupt_unmask
** Descriptions:            取消屏蔽中断
** input parameters:        interrupt           中断号
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_unmask(intno_t interrupt);
/*********************************************************************************************************
** Function name:           arch_mmu_init
** Descriptions:            初始化 MMU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void arch_mmu_init(void);
/*********************************************************************************************************
** Function name:           arch_in_kernel
** Descriptions:            判断是否在内核里
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int arch_in_kernel(void);
/*********************************************************************************************************
** Function name:           arch_build_context
** Descriptions:            构建任务的机器上下文
** input parameters:        task                任务控制块
**                          func                任务进入点函数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int arch_build_context(task_t *task, void *func);
/*********************************************************************************************************
** Function name:           arch_switch_context
** Descriptions:            切换任务的机器上下文
** input parameters:        from                旧任务控制块
**                          to                  新任务控制块
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void arch_switch_context(register task_t *from, register task_t *to);
/*********************************************************************************************************
** Function name:           arch_switch_context_to
** Descriptions:            切换任务的机器上下文
** input parameters:        from                旧任务控制块
**                          to                  新任务控制块
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void arch_switch_context_to(register task_t *from, register task_t *to);
/*********************************************************************************************************
** Function name:           soc_drivers_install
** Descriptions:            安装 SOC 驱动
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int soc_drivers_install(void);
/*********************************************************************************************************
** Function name:           soc_devices_create
** Descriptions:            创建 SOC 设备
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int soc_devices_create(void);
/*********************************************************************************************************
** Function name:           bsp_drivers_install
** Descriptions:            安装 BSP 驱动
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int bsp_drivers_install(void);
/*********************************************************************************************************
** Function name:           bsp_devices_create
** Descriptions:            创建 BSP 设备
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int bsp_devices_create(void);
/*********************************************************************************************************
** Function name:           ua_to_ka
** Descriptions:            将进程空间的虚拟地址转换为内核可以处理的虚拟地址
** input parameters:        VA                  进程空间的虚拟地址
** output parameters:       NONE
** Returned value:          内核可以处理的虚拟地址
*********************************************************************************************************/
void *ua_to_ka(const void *uaddr);
/*********************************************************************************************************
** Function name:           ka_to_ua
** Descriptions:            将内核可以处理的虚拟地址转换为进程空间的虚拟地址
** input parameters:        kaddr               内核可以处理的虚拟地址
** output parameters:       NONE
** Returned value:          进程空间的虚拟地址
*********************************************************************************************************/
void *ka_to_ua(const void *kaddr);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  ARCH_IF_H_                  */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
