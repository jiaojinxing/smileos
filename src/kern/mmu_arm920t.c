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
** Descriptions:            ARM920T MMU 底层接口函数库, 建立转换表, 初始化 MMU Cache 等
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-26
** Version:                 1.1.0
** Descriptions:            修改 mmu_map_section_as_page 和加入 mmu_map_section 函数,
**                          以实现进程虚拟地址空间的保护
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/mmu.h"
#include <string.h>

/*
 * 获得 ID
 */
uint32_t mmu_get_id(void)
{
    register uint32_t i;

    // MRC p15,0,Rd,c0,c0,0 ; returns ID register

    __asm__ __volatile__("mrc p15, 0, %0, c0, c0, 0":"=r"(i));

    return i;
}

/*
 * 获得 Cache 类型
 */
uint32_t mmu_get_cache_type(void)
{
    register uint32_t i;

    // MRC p15,0,Rd,c0,c0,1 ; returns cache details

    __asm__ __volatile__("mrc p15, 0, %0, c0, c0, 1":"=r"(i));

    return i;
}

/*
 * 设置转换表基址
 */
void mmu_set_ttb(register uint32_t i)
{
    // MRC p15, 0, Rd, c2, c0, 0 ; read TTB register

    // MCR p15, 0, Rd, c2, c0, 0 ; write TTB register

    __asm__ __volatile__("mcr p15, 0, %0, c2, c0, 0": :"r"(i));
}

/*
 * 设置 Domain
 */
void mmu_set_domain(register uint32_t i)
{
    // MRC p15, 0, Rd, c3, c0, 0 ; read domain 15:0 access permissions

    // MCR p15, 0, Rd, c3, c0, 0 ; write domain 15:0 access permissions

    __asm__ __volatile__("mcr p15,0, %0, c3, c0, 0": :"r"(i));
}

/*
 * 使能 MMU
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
 * 禁能 MMU
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
 * 使能对齐错误检查
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
 * 禁能对齐错误检查
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
 * 设置系统和 ROM 保护位
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
 * 设置异常向量地址
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
 * 使能 D-Cache
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
 * 禁能 D-Cache
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
 * 使能 I-Cache
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
 * 禁能 I-Cache
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
 * 无效 I-Cache 和 D-Cache
 */
void mmu_invalidate_icache_dcache(void)
{
    // Invalidate ICache and DCache SBZ MCR p15,0,Rd,c7,c7,0

    __asm__ __volatile__("mcr p15, 0, %0, c7, c7, 0": :"r"(0));
}

/*
 * 无效 I-Cache
 */
void mmu_invalidate_icache(void)
{
    // Invalidate ICache SBZ MCR p15,0,Rd,c7,c5,0

    __asm__ __volatile__("mcr p15, 0, %0, c7, c5, 0": :"r"(0));
}

/*
 * 清理并无效指定 mva 的 I-Cache
 */
void mmu_invalidate_icache_mva(register uint32_t mva)
{
    // Invalidate ICache single entry (using MVA) MVA format MCR p15,0,Rd,c7,c5,1

    mva &= ~0x1Ful;

    __asm__ __volatile__("mcr p15, 0, %0, c7, c5, 1": :"r"(mva));
}

/*
 * 无效 D-Cache
 */
void mmu_invalidate_dcache(void)
{
    // Invalidate DCache SBZ MCR p15,0,Rd,c7,c6,0

    __asm__ __volatile__("mcr p15, 0, %0, c7, c6, 0": :"r"(0));
}

/*
 * 清理指定 index 的 D-Cache
 */
void mmu_clean_dcache_index(register uint32_t index)
{
    // Clean DCache single entry (using index) Index format MCR p15,0,Rd,c7,c10,2

    __asm__ __volatile__("mcr p15, 0, %0, c7, c10, 2": :"r"(index));
}

/*
 * 清理并无效指定 mva 的 D-Cache
 */
void mmu_clean_invalidate_dcache_mva(register uint32_t mva)
{
    // Clean and Invalidate DCache entry (using MVA) MVA format MCR p15,0,Rd,c7,c14,1

    mva &= ~0x1Ful;

    __asm__ __volatile__("mcr p15, 0, %0, c7, c14, 1": :"r"(mva));
}

/*
 * 清理并无效指定 index 的 D-Cache
 */
void mmu_clean_invalidate_dcache_index(register uint32_t index)
{
    // Clean and Invalidate DCache entry (using index) Index format MCR p15,0,Rd,c7,c14,2

    __asm__ __volatile__("mcr p15, 0, %0, c7, c14, 2": :"r"(index));
}

/*
 * 回写写缓冲
 */
void mmu_drain_write_buffer(void)
{
    // Drain write buffer SBZ MCR p15,0,Rd,c7,c10,4

    __asm__ __volatile__("mcr p15, 0, %0, c7, c10, 4": :"r"(0));
}

/*
 * 等待中断, 进入节能模式
 */
void mmu_wait_for_interrupt(void)
{
    // Wait for interrupt SBZ MCR p15,0,Rd,c7,c0,4

    __asm__ __volatile__("mcr p15, 0, %0, c7, c0, 4": :"r"(0));
}

/*
 * 无效转换旁路缓冲
 */
void mmu_invalidate_itlb_dtlb(void)
{
    // Invalidate TLB(s) SBZ MCR p15,0,Rd,c8,c7,0

    __asm__ __volatile__("mcr p15, 0, %0, c8, c7, 0": :"r"(0));
}

/*
 * 无效指令转换旁路缓冲
 */
void mmu_invalidate_itlb(void)
{
    // Invalidate I TLB SBZ MCR p15,0,Rd,c8,c5,0

    __asm__ __volatile__("mcr p15, 0, %0, c8, c5, 0": :"r"(0));
}

/*
 * 无效数据转换旁路缓冲
 */
void mmu_invalidate_dtlb(void)
{
    // Invalidate D TLB SBZ MCR p15,0,Rd,c8,c6,0

    __asm__ __volatile__("mcr p15, 0, %0, c8, c6, 0": :"r"(0));
}

/*
 * 获得预取指错误状态
 */
uint32_t mmu_get_prefetch_fault_status(void)
{
    register uint32_t i;

    // read prefetch FSR value MRC p15, 0, Rd, c5, c0, 1

    __asm__ __volatile__("mrc p15, 0, %0, c5, c0, 1":"=r"(i));

    return i;
}

/*
 * 获得数据访问错误状态
 */
uint32_t mmu_get_data_fault_status(void)
{
    register uint32_t i;

    // read data FSR value MRC p15, 0, Rd, c5, c0, 0

    __asm__ __volatile__("mrc p15, 0, %0, c5, c0, 0":"=r"(i));

    return i;
}

/*
 * 获得错误地址
 */
uint32_t mmu_get_fault_address(void)
{
    register uint32_t i;

    // read FAR data MRC p15, 0, Rd, c6, c0, 0

    __asm__ __volatile__("mrc p15, 0, %0, c6, c0, 0":"=r"(i));

    return i;
}

/*
 * 映射段, 通过参数
 */
void mmu_map_section(register uint32_t section_nr,
                     register uint32_t value)
{
    register uint32_t *entry = (uint32_t *)MMU_TBL_BASE + section_nr;

    *entry = value;
}

/*
 * 取消映射段
 */
void mmu_unmap_section(register uint32_t section_nr)
{
    register uint32_t *entry = (uint32_t *)MMU_TBL_BASE + section_nr;

    *entry = 0;
}

/*
 * 映射段, 使用二级页表
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
 * 映射 4K 小页面
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
 * 映射区域
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
 * 建立转换表, 初始化 MMU Cache 等
 */
void mmu_init(void)
{
    register int i, j;

    /*
     * 将 D-Cache 和 I-Cache 禁能
     */
    mmu_disable_dcache();

    mmu_disable_icache();

    /*
     * 清除并无效 D-Cache
     */
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 8; j++) {
            mmu_clean_invalidate_dcache_index((i << 26) | (j << 5));
        }
    }

    /*
     * 回写写缓冲
     */
    mmu_drain_write_buffer();

    /*
     * 无效 I-Cache
     */
    mmu_invalidate_icache();

    /*
     * 使能 I-Cache, 加快建立转换表
     */
    mmu_enable_icache();

    /*
     * 禁能 MMU
     */
    mmu_disable();

    /*
     * 无效转换旁路缓冲
     */
    mmu_invalidate_itlb_dtlb();

    /*
     * 清除转换表
     */
    for (i = 0; i < SECTION_NR; i++) {
        mmu_unmap_section(i);
    }

    /*
     * 设置系统和 ROM 保护位
     */
    mmu_set_sys_rom_protect_bit(SYS_PROTECT_BIT, ROM_PROTECT_BIT);

    /*
     * 设置异常向量地址为 0xFFFF0000
     */
    mmu_set_vector_addr(VECTOR_ADDR_FFFF0000);

    /*
     * 设置 domain
     */
    mmu_set_domain((DOMAIN0_ATTR << 0) | (DOMAIN1_ATTR << 2) | (DOMAIN3_ATTR << 6));

    /*
     * 将物理内存映射到相同的地址, kernel, process 0 运行在 PHYSICAL_MEM_BASE 以上空间
     */
    mmu_map_region(PHY_MEM_BASE,
                   PHY_MEM_BASE,
                   PHY_MEM_SIZE,
                   SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_EN, BUFFER_EN));

    /*
     * 因为异常向量地址为 0xFFFF0000,
     * 将 0xFFF00000 映射到 INT_MEM_BASE,
     * 所以应该在 INT_MEM_BASE + 0xF0000 处(或 0xFFFF0000 处)放置异常向量跳转表
     */
    mmu_map_region(0xFFF00000,
                   INT_MEM_BASE,
                   1 * MB,
                   SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_EN, BUFFER_EN));
    /*
     * BSP 内存映射
     */
    extern void bsp_mem_map(void);
    bsp_mem_map();

    /*
     * 设置转换表基址
     */
    mmu_set_ttb(MMU_TBL_BASE);

    /*
     * 使能对齐错误检查
     */
    mmu_enable_align_fault_check();

    /*
     * 使能 MMU
     */
    mmu_enable();

    /*
     * 使能 I-Cache
     */
    mmu_enable_icache();

    /*
     * 使能 D-Cache
     */
    mmu_enable_dcache();

    /*
     * 拷贝异常向量跳转表
     */
    memcpy((char *)VECTOR_P_ADDR, (char *)KERN_LOAD_ADDR, PAGE_SIZE);
}

/*
 * 系统保留虚拟地址空间
 */
space_t sys_resv_space[] = {
        {
                PHY_MEM_BASE,
                PHY_MEM_SIZE
        },
        {
                /*
                 * 在高端地址处保留 1GB 虚拟地址空间以作后用
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
