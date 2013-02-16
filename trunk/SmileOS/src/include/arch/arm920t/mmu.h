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

#ifdef __cplusplus
extern "C" {
#endif

#include "kern/types.h"
/*********************************************************************************************************
** ����
*********************************************************************************************************/
/*
 * Domain
 */
#define DOMAIN_NO_ACCESS        0ul
#define DOMAIN_CHECK            1ul
#define DOMAIN_NO_CHECK         3ul

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
#define AP_ROM                  0ul
#define AP_USER_NO              1ul
#define AP_USER_R               2ul
#define AP_USER_RW              3ul

/*
 * Cache
 */
#define CACHE_EN                1ul
#define CACHE_NO                0ul

/*
 * Write Buffer
 */
#define BUFFER_EN               1ul
#define BUFFER_NO               0ul

/*
 * ������
 */
#define SECTION_ATTR(ap, domain, cache, buffer) \
        (ap << 10) + (domain << 5) + (1 << 4) + (cache << 3) + (buffer << 2) + (1 << 1) + (0 << 0)
/*********************************************************************************************************
** Function name:           mmu_get_cpu_id
** Descriptions:            ��� CPU ID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ID
*********************************************************************************************************/
uint32_t mmu_get_cpu_id(void);
/*********************************************************************************************************
** Function name:           mmu_get_cache_type
** Descriptions:            ��� Cache ����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          Cache ����
*********************************************************************************************************/
uint32_t mmu_get_cache_type(void);
/*********************************************************************************************************
** Function name:           mmu_set_ttb
** Descriptions:            ����ת�����ַ
** input parameters:        i                   ת�����ַ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_ttb(register uint32_t i);
/*********************************************************************************************************
** Function name:           mmu_set_domain
** Descriptions:            ���� Domain
** input parameters:        i                   Domain
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_domain(register uint32_t i);
/*********************************************************************************************************
** Function name:           mmu_enable
** Descriptions:            ʹ�� MMU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable(void);
/*********************************************************************************************************
** Function name:           mmu_disable
** Descriptions:            ���� MMU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable(void);
/*********************************************************************************************************
** Function name:           mmu_enable_align_fault_check
** Descriptions:            ʹ�ܶ��������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable_align_fault_check(void);
/*********************************************************************************************************
** Function name:           mmu_disable_align_fault_check
** Descriptions:            ���ܶ��������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable_align_fault_check(void);
/*********************************************************************************************************
** Function name:           mmu_set_sys_rom_protect_bit
** Descriptions:            ����ϵͳ�� ROM ����λ
** input parameters:        sys                 ϵͳλ
**                          rom                 ROM ����λ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_sys_rom_protect_bit(uint32_t sys, uint32_t rom);
/*********************************************************************************************************
** Function name:           mmu_set_vector_addr
** Descriptions:            �����쳣������ַ
** input parameters:        vector_addr         �쳣������ַ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_vector_addr(uint32_t vector_addr);
/*********************************************************************************************************
** Function name:           mmu_enable_dcache
** Descriptions:            ʹ�� D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable_dcache(void);
/*********************************************************************************************************
** Function name:           mmu_disable_dcache
** Descriptions:            ���� D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable_dcache(void);
/*********************************************************************************************************
** Function name:           mmu_enable_icache
** Descriptions:            ʹ�� I-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable_icache(void);
/*********************************************************************************************************
** Function name:           mmu_disable_icache
** Descriptions:            ���� I-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable_icache(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_icache_dcache
** Descriptions:            ��Ч I-Cache �� D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_icache_dcache(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_icache
** Descriptions:            ��Ч I-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_icache(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_icache_mva
** Descriptions:            ��Чָ�� MVA �� I-Cache
** input parameters:        MVA                 �޸ĺ�������ַ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_icache_mva(register uint32_t MVA);
/*********************************************************************************************************
** Function name:           mmu_invalidate_dcache
** Descriptions:            ��Ч D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_dcache(void);
/*********************************************************************************************************
** Function name:           mmu_clean_dcache_index
** Descriptions:            ����ָ�� index �� D-Cache
** input parameters:        index               ����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_clean_dcache_index(register uint32_t index);
/*********************************************************************************************************
** Function name:           mmu_clean_invalidate_dcache_mva
** Descriptions:            ������Чָ�� MVA �� D-Cache
** input parameters:        MVA                 �޸ĺ�������ַ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_clean_invalidate_dcache_mva(register uint32_t MVA);
/*********************************************************************************************************
** Function name:           mmu_clean_invalidate_dcache_index
** Descriptions:            ������Чָ�� index �� D-Cache
** input parameters:        index               ����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_clean_invalidate_dcache_index(register uint32_t index);
/*********************************************************************************************************
** Function name:           mmu_drain_write_buffer
** Descriptions:            ��дд����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_drain_write_buffer(void);
/*********************************************************************************************************
** Function name:           mmu_wait_for_interrupt
** Descriptions:            �ȴ��ж�, �������ģʽ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_wait_for_interrupt(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_itlb_dtlb
** Descriptions:            ��Чת����·����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_itlb_dtlb(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_itlb
** Descriptions:            ��Чָ��ת����·����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_itlb(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_dtlb
** Descriptions:            ��Ч����ת����·����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_dtlb(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_dtlb_mva
** Descriptions:            ��Ч���� MVA ��ָ��ת����·����
** input parameters:        MVA                 �޸ĺ�������ַ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_dtlb_mva(register uint32_t MVA);
/*********************************************************************************************************
** Function name:           mmu_invalidate_itlb_mva
** Descriptions:            ��Чָ�� MVA ��ָ��ת����·����
** input parameters:        MVA                 �޸ĺ�������ַ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_itlb_mva(register uint32_t MVA);
/*********************************************************************************************************
** Function name:           mmu_get_prefetch_fault_status
** Descriptions:            ���Ԥȡָ����״̬
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          Ԥȡָ����״̬
*********************************************************************************************************/
uint32_t mmu_get_prefetch_fault_status(void);
/*********************************************************************************************************
** Function name:           mmu_get_data_fault_status
** Descriptions:            ������ݷ��ʴ���״̬
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ���ݷ��ʴ���״̬
*********************************************************************************************************/
uint32_t mmu_get_data_fault_status(void);
/*********************************************************************************************************
** Function name:           mmu_get_fault_address
** Descriptions:            ��ô����ַ
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          �����ַ
*********************************************************************************************************/
uint32_t mmu_get_fault_address(void);
/*********************************************************************************************************
** Function name:           mmu_map_section
** Descriptions:            ӳ���, ʹ�ö���ҳ��
** input parameters:        section_no          �κ�
**                          page_tbl_base       ����ҳ���ַ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_map_section(uint32_t section_no,
                     uint32_t page_tbl_base);
/*********************************************************************************************************
** Function name:           mmu_unmap_section
** Descriptions:            ȡ��ӳ���
** input parameters:        section_no          �κ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_unmap_section(uint32_t section_no);
/*********************************************************************************************************
** Function name:           mmu_map_page
** Descriptions:            ӳ�� 4K Сҳ��
** input parameters:        page_tbl_base       ����ҳ���ַ
**                          page_no             ����ҳ��
**                          phypage_base        ����ҳ���ַ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_map_page(uint32_t page_tbl_base,
                  uint32_t page_no,
                  uint32_t phy_page_base);
/*********************************************************************************************************
** Function name:           mmu_map_region
** Descriptions:            ӳ������
** input parameters:        virt_section_base   �����ַ
**                          phys_section_base   �����ַ
**                          size                ��С
**                          attr                ����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_map_region(uint32_t virt_section_base,
                    uint32_t phys_section_base,
                    uint32_t size,
                    uint32_t attr);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  MMU_H_                      */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
