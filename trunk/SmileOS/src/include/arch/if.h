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
** Descriptions:            ����ϵ�ṹ��صĽӿ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-4
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
#ifndef ARCH_IF_H_
#define ARCH_IF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
** Function name:           cpu_init
** Descriptions:            �ں˳�ʼ�� CPU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void cpu_init(void);
/*********************************************************************************************************
** Function name:           cpu_timer_init
** Descriptions:            ��ʼ����ʱ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void cpu_timer_init(void);
/*********************************************************************************************************
** Function name:           interrupt_disable
** Descriptions:            �����ٽ�����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          �����ٽ�����ǰ�ĵ�ǰ����״̬�Ĵ���
*********************************************************************************************************/
reg_t interrupt_disable(void);
/*********************************************************************************************************
** Function name:           interrupt_resume
** Descriptions:            �˳��ٽ�����
** input parameters:        reg                 �����ٽ�����ǰ�ĵ�ǰ����״̬�Ĵ���
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_resume(register reg_t reg);
/*********************************************************************************************************
** Function name:           interrupt_mask
** Descriptions:            �����ж�
** input parameters:        interrupt           �жϺ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_mask(intno_t interrupt);
/*********************************************************************************************************
** Function name:           interrupt_unmask
** Descriptions:            ȡ�������ж�
** input parameters:        interrupt           �жϺ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_unmask(intno_t interrupt);
/*********************************************************************************************************
** Function name:           arch_mmu_init
** Descriptions:            ��ʼ�� MMU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void arch_mmu_init(void);
/*********************************************************************************************************
** Function name:           arch_in_kernel
** Descriptions:            �ж��Ƿ����ں���
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int arch_in_kernel(void);
/*********************************************************************************************************
** Function name:           arch_build_context
** Descriptions:            ��������Ļ���������
** input parameters:        task                ������ƿ�
**                          func                �������㺯��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int arch_build_context(task_t *task, void *func);
/*********************************************************************************************************
** Function name:           arch_switch_context
** Descriptions:            �л�����Ļ���������
** input parameters:        from                ��������ƿ�
**                          to                  ��������ƿ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void arch_switch_context(register task_t *from, register task_t *to);
/*********************************************************************************************************
** Function name:           arch_switch_context_to
** Descriptions:            �л�����Ļ���������
** input parameters:        from                ��������ƿ�
**                          to                  ��������ƿ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void arch_switch_context_to(register task_t *from, register task_t *to);
/*********************************************************************************************************
** Function name:           cpu_drivers_install
** Descriptions:            ��װ CPU ����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int cpu_drivers_install(void);
/*********************************************************************************************************
** Function name:           cpu_devices_create
** Descriptions:            ���� CPU �豸
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int cpu_devices_create(void);
/*********************************************************************************************************
** Function name:           board_drivers_install
** Descriptions:            ��װĿ�������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int board_drivers_install(void);
/*********************************************************************************************************
** Function name:           board_devices_create
** Descriptions:            ����Ŀ����豸
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int board_devices_create(void);
/*********************************************************************************************************
** Function name:           ua_to_ka
** Descriptions:            �����̿ռ�������ַת��Ϊ�ں˿��Դ���������ַ
** input parameters:        VA                  ���̿ռ�������ַ
** output parameters:       NONE
** Returned value:          �ں˿��Դ���������ַ
*********************************************************************************************************/
void *ua_to_ka(const void *uaddr);
/*********************************************************************************************************
** Function name:           ka_to_ua
** Descriptions:            ���ں˿��Դ���������ַת��Ϊ���̿ռ�������ַ
** input parameters:        kaddr               �ں˿��Դ���������ַ
** output parameters:       NONE
** Returned value:          ���̿ռ�������ַ
*********************************************************************************************************/
void *ka_to_ua(const void *kaddr);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  ARCH_IF_H_                  */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
