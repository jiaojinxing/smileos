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
** File name:               mmu.h
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            ARM920T MMU �ײ�ӿں�����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#ifndef MMU_H_
#define MMU_H_

#include "types.h"

/*
 * domain
 */
#define DOMAIN_NO_ACCESS        0
#define DOMAIN_CHECK            1
#define DOMAIN_NO_CHECK         3

#define DOMAIN0_ATTR            DOMAIN_NO_ACCESS
#define DOMAIN1_ATTR            DOMAIN_CHECK
#define DOMAIN3_ATTR            DOMAIN_NO_CHECK

/*
 * ϵͳ�� ROM ����λ�ֱ�����Ϊ 0, 1
 */
#define SYS_PROTECT_BIT         0
#define ROM_PROTECT_BIT         1

/*
 * �쳣������ַ
 */
#define VECTOR_ADDR_00000000    0
#define VECTOR_ADDR_FFFF0000    1

/*
 * AP
 */
#define AP_ROM                  0
#define AP_USER_NO              1
#define AP_USER_R               2
#define AP_USER_RW              3

/*
 * Cache
 */
#define CACHE_EN                1
#define CACHE_NO                0

/*
 * Write Buffer
 */
#define BUFFER_EN               1
#define BUFFER_NO               0

/*
 * ������
 */
#define SECTION_ATTR(ap, domain, cache, buffer) \
        (ap << 10) + (domain << 5) + (1 << 4) + (cache << 3) + (buffer << 2) + (1 << 1) + (0 << 0)

/*
 * ��� ID
 */
uint32_t mmu_get_id(void);

/*
 * ��� cache ����
 */
uint32_t mmu_get_cache_type(void);

/*
 * ����ת�����ַ
 */
void mmu_set_ttb(register uint32_t i);

/*
 * ���� domain
 */
void mmu_set_domain(register uint32_t i);

/*
 * ʹ�� MMU
 */
void mmu_enable(void);

/*
 * ���� MMU
 */
void mmu_disable(void);

/*
 * ʹ�ܶ��������
 */
void mmu_enable_align_fault_check(void);

/*
 * ���ܶ��������
 */
void mmu_disable_align_fault_check(void);

/*
 * ����ϵͳ�� ROM ����λ
 */
void mmu_set_sys_rom_protect_bit(uint32_t s, uint32_t r);

/*
 * �����쳣������ַ
 */
void mmu_set_vector_addr(uint32_t v);

/*
 * ʹ�� D-Cache
 */
void mmu_enable_dcache(void);

/*
 * ���� D-Cache
 */
void mmu_disable_dcache(void);

/*
 * ʹ�� I-Cache
 */
void mmu_enable_icache(void);

/*
 * ���� I-Cache
 */
void mmu_disable_icache(void);

/*
 * ��Ч I-Cache �� D-Cache
 */
void mmu_invalidate_icache_dcache(void);

/*
 * ��Ч I-Cache
 */
void mmu_invalidate_icache(void);

/*
 * ��Ч D-Cache
 */
void mmu_invalidate_dcache(void);

/*
 * ����ָ�� index �� D-Cache
 */
void mmu_clean_dcache_index(uint32_t index);

/*
 * ������Чָ�� index �� D-Cache
 */
void mmu_clean_invalidate_dcache_index(uint32_t index);

/*
 * ��дд����
 */
void mmu_drain_write_buffer(void);

/*
 * �ȴ��ж�, �������ģʽ
 */
void mmu_wait_for_interrupt(void);

/*
 * ��Чת����·����
 */
void mmu_invalidate_itlb_dtlb(void);

/*
 * ��Чָ��ת����·����
 */
void mmu_invalidate_itlb(void);

/*
 * ��Ч����ת����·����
 */
void mmu_invalidate_dtlb(void);

/*
 * ӳ���
 */
void mmu_map_section(
        register uint32_t ttb,
        register uint32_t vaddr,
        register uint32_t paddr,
        register uint32_t n_mb,
        register uint32_t attr);

/*
 * ���ת����
 */
void mmu_clean_tt(uint32_t ttb);

/*
 * ����ת����, ��ʼ�� MMU Cache ��
 */
void mmu_init(void);

#endif                                                                  /*  MMU_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
