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
** File name:               mmu_arm920t.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            ARM920T MMU �ײ�ӿں�����, ����ת����, ��ʼ�� MMU Cache ��
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-26
** Version:                 1.1.0
** Descriptions:            �޸� mmu_map_section_as_page �ͼ��� mmu_map_section ����,
**                          ��ʵ�ֽ��������ַ�ռ�ı���
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/mmu.h"
#include <string.h>

/*
 * ��� ID
 */
uint32_t mmu_get_id(void)
{
    register uint32_t i;

    // MRC p15,0,Rd,c0,c0,0 ; returns ID register

    __asm__ __volatile__("mrc p15, 0, %0, c0, c0, 0":"=r"(i));

    return i;
}

/*
 * ��� Cache ����
 */
uint32_t mmu_get_cache_type(void)
{
    register uint32_t i;

    // MRC p15,0,Rd,c0,c0,1 ; returns cache details

    __asm__ __volatile__("mrc p15, 0, %0, c0, c0, 1":"=r"(i));

    return i;
}

/*
 * ����ת�����ַ
 */
void mmu_set_ttb(register uint32_t i)
{
    // MRC p15, 0, Rd, c2, c0, 0 ; read TTB register

    // MCR p15, 0, Rd, c2, c0, 0 ; write TTB register

    __asm__ __volatile__("mcr p15, 0, %0, c2, c0, 0": :"r"(i));
}

/*
 * ���� Domain
 */
void mmu_set_domain(register uint32_t i)
{
    // MRC p15, 0, Rd, c3, c0, 0 ; read domain 15:0 access permissions

    // MCR p15, 0, Rd, c3, c0, 0 ; write domain 15:0 access permissions

    __asm__ __volatile__("mcr p15,0, %0, c3, c0, 0": :"r"(i));
}

/*
 * ʹ�� MMU
 */
void mmu_enable(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 0);

    __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ���� MMU
 */
void mmu_disable(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 0);

    __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ʹ�ܶ��������
 */
void mmu_enable_align_fault_check(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 1);

    __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ���ܶ��������
 */
void mmu_disable_align_fault_check(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 1);

    __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ����ϵͳ�� ROM ����λ
 */
void mmu_set_sys_rom_protect_bit(register uint32_t sys, register uint32_t rom)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    if (sys) {
        i |=  (1 << 8);
    } else {
        i &= ~(1 << 8);
    }

    if (rom) {
        i |=  (1 << 9);
    } else {
        i &= ~(1 << 9);
    }

    __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * �����쳣������ַ
 */
void mmu_set_vector_addr(register uint32_t vector_addr)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    if (vector_addr) {
        i |=  (1 << 13);
    } else {
        i &= ~(1 << 13);
    }

    __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ʹ�� D-Cache
 */
void mmu_enable_dcache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 2);

    __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ���� D-Cache
 */
void mmu_disable_dcache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 2);

    __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ʹ�� I-Cache
 */
void mmu_enable_icache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 12);

    __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ���� I-Cache
 */
void mmu_disable_icache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 12);

    __asm__ __volatile__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ��Ч I-Cache �� D-Cache
 */
void mmu_invalidate_icache_dcache(void)
{
    // Invalidate ICache and DCache SBZ MCR p15,0,Rd,c7,c7,0

    __asm__ __volatile__("mcr p15, 0, %0, c7, c7, 0": :"r"(0));
}

/*
 * ��Ч I-Cache
 */
void mmu_invalidate_icache(void)
{
    // Invalidate ICache SBZ MCR p15,0,Rd,c7,c5,0

    __asm__ __volatile__("mcr p15, 0, %0, c7, c5, 0": :"r"(0));
}

/*
 * ��Ч D-Cache
 */
void mmu_invalidate_dcache(void)
{
    // Invalidate DCache SBZ MCR p15,0,Rd,c7,c6,0

    __asm__ __volatile__("mcr p15, 0, %0, c7, c6, 0": :"r"(0));
}

/*
 * ����ָ�� index �� D-Cache
 */
void mmu_clean_dcache_index(register uint32_t index)
{
    // Clean DCache single entry (using index) Index format MCR p15,0,Rd,c7,c10,2

    __asm__ __volatile__("mcr p15, 0, %0, c7, c10, 2": :"r"(index));
}

/*
 * ������Чָ�� index �� D-Cache
 */
void mmu_clean_invalidate_dcache_index(register uint32_t index)
{
    // Clean and Invalidate DCache entry (using index) Index format MCR p15,0,Rd,c7,c14,2

    __asm__ __volatile__("mcr p15, 0, %0, c7, c14, 2": :"r"(index));
}

/*
 * ��дд����
 */
void mmu_drain_write_buffer(void)
{
    // Drain write buffer SBZ MCR p15,0,Rd,c7,c10,4

    __asm__ __volatile__("mcr p15, 0, %0, c7, c10, 4": :"r"(0));
}

/*
 * �ȴ��ж�, �������ģʽ
 */
void mmu_wait_for_interrupt(void)
{
    // Wait for interrupt SBZ MCR p15,0,Rd,c7,c0,4

    __asm__ __volatile__("mcr p15, 0, %0, c7, c0, 4": :"r"(0));
}

/*
 * ��Чת����·����
 */
void mmu_invalidate_itlb_dtlb(void)
{
    // Invalidate TLB(s) SBZ MCR p15,0,Rd,c8,c7,0

    __asm__ __volatile__("mcr p15, 0, %0, c8, c7, 0": :"r"(0));
}

/*
 * ��Чָ��ת����·����
 */
void mmu_invalidate_itlb(void)
{
    // Invalidate I TLB SBZ MCR p15,0,Rd,c8,c5,0

    __asm__ __volatile__("mcr p15, 0, %0, c8, c5, 0": :"r"(0));
}

/*
 * ��Ч����ת����·����
 */
void mmu_invalidate_dtlb(void)
{
    // Invalidate D TLB SBZ MCR p15,0,Rd,c8,c6,0

    __asm__ __volatile__("mcr p15, 0, %0, c8, c6, 0": :"r"(0));
}

/*
 * ���Ԥȡָ����״̬
 */
uint32_t mmu_get_prefetch_fault_status(void)
{
    register uint32_t i;

    // read prefetch FSR value MRC p15, 0, Rd, c5, c0, 1

    __asm__ __volatile__("mrc p15, 0, %0, c5, c0, 1":"=r"(i));

    return i;
}

/*
 * ������ݷ��ʴ���״̬
 */
uint32_t mmu_get_data_fault_status(void)
{
    register uint32_t i;

    // read data FSR value MRC p15, 0, Rd, c5, c0, 0

    __asm__ __volatile__("mrc p15, 0, %0, c5, c0, 0":"=r"(i));

    return i;
}

/*
 * ��ô����ַ
 */
uint32_t mmu_get_fault_address(void)
{
    register uint32_t i;

    // read FAR data MRC p15, 0, Rd, c6, c0, 0

    __asm__ __volatile__("mrc p15, 0, %0, c6, c0, 0":"=r"(i));

    return i;
}

/*
 * ӳ���, ͨ������
 */
void mmu_map_section(register uint32_t section_nr,
                     register uint32_t value)
{
    register uint32_t *entry = (uint32_t *)MMU_TBL_BASE + section_nr;

    *entry = value;
}

/*
 * ȡ��ӳ���
 */
void mmu_unmap_section(register uint32_t section_nr)
{
    register uint32_t *entry = (uint32_t *)MMU_TBL_BASE + section_nr;

    *entry = 0;
}

/*
 * ӳ���, ʹ�ö���ҳ��
 */
uint32_t mmu_map_section_as_page(register uint32_t section_nr,
                                 register uint32_t page_tbl_base)
{
    register uint32_t *entry = (uint32_t *)MMU_TBL_BASE + section_nr;
    register uint32_t  value;

    value = (page_tbl_base & (~(PAGE_TBL_SIZE - 1))) |
            (DOMAIN_CHECK << 5) |
            (1 << 4) |
            (1 << 0);

    *entry = value;

    return value;
}

/*
 * ӳ�� 4K Сҳ��
 */
void mmu_map_page(register uint32_t page_tbl_base,
                  register uint32_t page_nr,
                  register uint32_t frame_base)
{
    register uint32_t *entry = (uint32_t *)page_tbl_base + page_nr;

    *entry = (frame_base & (~(VMM_FRAME_SIZE - 1))) |
            (AP_USER_RW << 10) |
            (AP_USER_RW << 8) |
            (AP_USER_RW << 6) |
            (AP_USER_RW << 4) |
            (CACHE_EN   << 3) |
            (BUFFER_EN  << 2) |
            (1          << 1);
}

/*
 * ӳ������
 */
void mmu_map_region(register uint32_t virtual_base,
                    register uint32_t physical_base,
                    register uint32_t size,
                    register uint32_t attr)
{
    register uint32_t *entry;
    register int       i;

    entry  = (uint32_t *)MMU_TBL_BASE + (virtual_base >> SECTION_OFFSET);

    size   = (size + SECTION_SIZE - 1) / SECTION_SIZE;

    for (i = 0; i < size; i++) {
        *entry++ = attr | (((physical_base >> SECTION_OFFSET) + i) << SECTION_OFFSET);
    }
}

/*
 * ����ת����, ��ʼ�� MMU Cache ��
 */
void mmu_init(void)
{
    register int i, j;

    /*
     * �� D-Cache �� I-Cache ����
     */
    mmu_disable_dcache();

    mmu_disable_icache();

    /*
     * �������Ч D-Cache
     */
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 8; j++) {
            mmu_clean_invalidate_dcache_index((i << 26) | (j << 5));
        }
    }

    /*
     * ��дд����
     */
    mmu_drain_write_buffer();

    /*
     * ��Ч I-Cache
     */
    mmu_invalidate_icache();

    /*
     * ʹ�� I-Cache, �ӿ콨��ת����
     */
    mmu_enable_icache();

    /*
     * ���� MMU
     */
    mmu_disable();

    /*
     * ��Чת����·����
     */
    mmu_invalidate_itlb_dtlb();

    /*
     * ���ת����
     */
    for (i = 0; i < SECTION_NR; i++) {
        mmu_unmap_section(i);
    }

    /*
     * ����ϵͳ�� ROM ����λ
     */
    mmu_set_sys_rom_protect_bit(SYS_PROTECT_BIT, ROM_PROTECT_BIT);

    /*
     * �����쳣������ַΪ 0xFFFF0000
     */
    mmu_set_vector_addr(VECTOR_ADDR_FFFF0000);

    /*
     * ���� domain
     */
    mmu_set_domain((DOMAIN0_ATTR << 0) | (DOMAIN1_ATTR << 2) | (DOMAIN3_ATTR << 6));

    /*
     * �������ڴ�ӳ�䵽��ͬ�ĵ�ַ, kernel, process 0 ������ PHYSICAL_MEM_BASE ���Ͽռ�
     */
    mmu_map_region(PHY_MEM_BASE,
                   PHY_MEM_BASE,
                   PHY_MEM_SIZE,
                   SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_EN, BUFFER_EN));

    /*
     * ��Ϊ�쳣������ַΪ 0xFFFF0000,
     * �� 0xFFF00000 ӳ�䵽 INT_MEM_BASE,
     * ����Ӧ���� INT_MEM_BASE + 0xF0000 ��(�� 0xFFFF0000 ��)�����쳣������ת��
     */
    mmu_map_region(0xFFF00000,
                   INT_MEM_BASE,
                   1 * MB,
                   SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_EN, BUFFER_EN));
    /*
     * BSP �ڴ�ӳ��
     */
    extern void bsp_mem_map(void);
    bsp_mem_map();

    /*
     * ����ת�����ַ
     */
    mmu_set_ttb(MMU_TBL_BASE);

    /*
     * ʹ�ܶ��������
     */
    mmu_enable_align_fault_check();

    /*
     * ʹ�� MMU
     */
    mmu_enable();

    /*
     * ʹ�� I-Cache
     */
    mmu_enable_icache();

    /*
     * ʹ�� D-Cache
     */
    mmu_enable_dcache();

    /*
     * �����쳣������ת��
     */
    memcpy((char *)VECTOR_P_ADDR, (char *)KERN_LOAD_ADDR, PAGE_SIZE);
}

/*
 * ϵͳ���������ַ�ռ�
 */
space_t sys_resv_space[] = {
        {
                PHY_MEM_BASE,
                PHY_MEM_SIZE
        },
        {
                /*
                 * �ڸ߶˵�ַ������ 1GB �����ַ�ռ���������
                 */
                0xC0000000,
                1 * GB
        },
        {
                0,
                0
        }
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
