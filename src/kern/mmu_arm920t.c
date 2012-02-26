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
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "config.h"
#include "types.h"
#include "mmu.h"

/*
 * ��� ID
 */
uint32_t mmu_get_id(void)
{
    register uint32_t i;

    // MRC p15,0,Rd,c0,c0,0 ; returns ID register

    __asm__("mrc p15, 0, %0, c0, c0, 0":"=r"(i));

    return i;
}

/*
 * ��� cache ����
 */
uint32_t mmu_get_cache_type(void)
{
    register uint32_t i;

    // MRC p15,0,Rd,c0,c0,1 ; returns cache details

    __asm__("mrc p15, 0, %0, c0, c0, 1":"=r"(i));

    return i;
}

/*
 * ����ת�����ַ
 */
void mmu_set_ttb(register uint32_t i)
{
    // MRC p15, 0, Rd, c2, c0, 0 ; read TTB register

    // MCR p15, 0, Rd, c2, c0, 0 ; write TTB register

    __asm__("mcr p15, 0, %0, c2, c0, 0": :"r"(i));
}

/*
 * ���� domain
 */
void mmu_set_domain(register uint32_t i)
{
    // MRC p15, 0, Rd, c3, c0, 0 ; read domain 15:0 access permissions

    // MCR p15, 0, Rd, c3, c0, 0 ; write domain 15:0 access permissions

    __asm__("mcr p15,0, %0, c3, c0, 0": :"r"(i));
}

/*
 * ʹ�� MMU
 */
void mmu_enable(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 0);

    __asm__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ���� MMU
 */
void mmu_disable(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 0);

    __asm__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ʹ�ܶ��������
 */
void mmu_enable_align_fault_check(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 1);

    __asm__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ���ܶ��������
 */
void mmu_disable_align_fault_check(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 1);

    __asm__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ����ϵͳ�� ROM ����λ
 */
void mmu_set_sys_rom_protect_bit(uint32_t s, uint32_t r)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    if (s) {
        i |=  (1 << 8);
    } else {
        i &= ~(1 << 8);
    }

    if (r) {
        i |=  (1 << 9);
    } else {
        i &= ~(1 << 9);
    }

    __asm__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * �����쳣������ַ
 */
void mmu_set_vector_addr(uint32_t v)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    if (v) {
        i |=  (1 << 13);
    } else {
        i &= ~(1 << 13);
    }

    __asm__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ʹ�� D-Cache
 */
void mmu_enable_dcache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 2);

    __asm__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ���� D-Cache
 */
void mmu_disable_dcache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 2);

    __asm__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ʹ�� I-Cache
 */
void mmu_enable_icache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 12);

    __asm__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ���� I-Cache
 */
void mmu_disable_icache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__("mrc p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 12);

    __asm__("mcr p15, 0, %0, c1, c0, 0": :"r"(i));
}

/*
 * ��Ч I-Cache �� D-Cache
 */
void mmu_invalidate_icache_dcache(void)
{
    // Invalidate ICache and DCache SBZ MCR p15,0,Rd,c7,c7,0

    __asm__("mcr p15, 0, %0, c7, c7, 0": :"r"(0));
}

/*
 * ��Ч I-Cache
 */
void mmu_invalidate_icache(void)
{
    // Invalidate ICache SBZ MCR p15,0,Rd,c7,c5,0

    __asm__("mcr p15, 0, %0, c7, c5, 0": :"r"(0));
}

/*
 * ��Ч D-Cache
 */
void mmu_invalidate_dcache(void)
{
    // Invalidate DCache SBZ MCR p15,0,Rd,c7,c6,0

    __asm__("mcr p15, 0, %0, c7, c6, 0": :"r"(0));
}

/*
 * ����ָ�� index �� D-Cache
 */
void mmu_clean_dcache_index(register uint32_t index)
{
    // Clean DCache single entry (using index) Index format MCR p15,0,Rd,c7,c10,2

    __asm__("mcr p15, 0, %0, c7, c10, 2": :"r"(index));
}

/*
 * ������Чָ�� index �� D-Cache
 */
void mmu_clean_invalidate_dcache_index(register uint32_t index)
{
    // Clean and Invalidate DCache entry (using index) Index format MCR p15,0,Rd,c7,c14,2

    __asm__("mcr p15, 0, %0, c7, c14, 2": :"r"(index));
}

/*
 * ��дд����
 */
void mmu_drain_write_buffer(void)
{
    // Drain write buffer SBZ MCR p15,0,Rd,c7,c10,4

    __asm__("mcr p15, 0, %0, c7, c10, 4": :"r"(0));
}

/*
 * �ȴ��ж�, �������ģʽ
 */
void mmu_wait_for_interrupt(void)
{
    // Wait for interrupt SBZ MCR p15,0,Rd,c7,c0,4

    __asm__("mcr p15, 0, %0, c7, c0, 4": :"r"(0));
}

/*
 * ��Чת����·����
 */
void mmu_invalidate_itlb_dtlb(void)
{
    // Invalidate TLB(s) SBZ MCR p15,0,Rd,c8,c7,0

    __asm__("mcr p15, 0, %0, c8, c7, 0": :"r"(0));
}

/*
 * ��Чָ��ת����·����
 */
void mmu_invalidate_itlb(void)
{
    // Invalidate I TLB SBZ MCR p15,0,Rd,c8,c5,0

    __asm__("mcr p15, 0, %0, c8, c5, 0": :"r"(0));
}

/*
 * ��Ч����ת����·����
 */
void mmu_invalidate_dtlb(void)
{
    // Invalidate D TLB SBZ MCR p15,0,Rd,c8,c6,0

    __asm__("mcr p15, 0, %0, c8, c6, 0": :"r"(0));
}

/*
 * ���Ԥȡָ����״̬
 */
uint32_t mmu_get_prefetch_fault_status(void)
{
    register uint32_t i;

    // read prefetch FSR value MRC p15, 0, Rd, c5, c0, 1

    __asm__("mrc p15, 0, %0, c5, c0, 1":"=r"(i));

    return i;
}

/*
 * ���Ԥȡָ�����ַ
 */
uint32_t mmu_get_fault_address(void)
{
    register uint32_t i;

    // read FAR data MRC p15, 0, Rd, c6, c0, 0

    __asm__("mrc p15, 0, %0, c6, c0, 0":"=r"(i));

    return i;
}

/*
 * ӳ���
 */
void mmu_map_section(
        register uint32_t ttb,
        register uint32_t vaddr,
        register uint32_t paddr,
        register uint32_t n_mb,
        register uint32_t attr)
{
    volatile uint32_t *tte;
    register int i;

    tte  = (uint32_t *)ttb + (vaddr >> 20);

    for (i = 0; i < n_mb; i++) {
        *tte++ = attr | (((paddr >> 20) + i) << 20);
    }
}

/*
 * ���ת����
 */
void mmu_clean_tt(register uint32_t ttb)
{
    register int i;

    for (i = 0; i < 4096; i++) {
        *((volatile uint32_t *)ttb + i) = 0;
    }
}

extern void bsp_mem_map(void);

/*
 * ����ת����, ��ʼ�� MMU Cache ��
 */
void mmu_init(void)
{
    int i, j;

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
    mmu_clean_tt(MMU_TBL_BASE);

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
    mmu_map_section(MMU_TBL_BASE,
            PHY_MEM_BASE,
            PHY_MEM_BASE,
            PHY_MEM_SIZE / MB,
            SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_EN, BUFFER_EN));

    /*
     * ��Ϊ�쳣������ַΪ 0xFFFF0000,
     * �� 0xFFF00000 ӳ�䵽 INT_MEM_BASE,
     * ����Ӧ���� INT_MEM_BASE + 0xF0000 ��(�� 0xFFFF0000 ��)�����쳣������ת��
     */
    mmu_map_section(MMU_TBL_BASE,
            0xFFF00000,
            INT_MEM_BASE,
            INT_MEM_SIZE / MB,
            SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_EN, BUFFER_EN));

    /*
     * process 1 ~ PROCESS_NR
     */
    /*
     *          pa             va
     * dm9000   0x20000000     0x20000000
     * sdram    0x30000000     0x30000000
     * sfr      0x48000000     0x48000000
     *
     * 16 * 32 MB = 0x20000000, ��ཨ�� 16 ������, 64MB / 16 = 4MB
     *
     * ʵ�� PROCESS_NR = 14
     *
     * ÿ�����̵� va_base = 0, mva = va + 32 * MB * pid,
     *
     */
    for (i = 1; i <= PROCESS_NR; i++) {
        mmu_map_section(MMU_TBL_BASE,
                0 + PROCESS_SPACE_SIZE * i,
                PROCESS_MEM_BASE + PROCESS_MEM_SIZE * (i - 1),
                PROCESS_MEM_SIZE / MB,
                SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_EN, BUFFER_EN));
    }

    /*
     * BSP �ڴ�ӳ��
     */
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
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

