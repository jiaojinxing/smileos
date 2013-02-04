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
** File name:               mmu.c
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
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "arch/arm920t/mmu.h"
#include "kern/addr_config.h"
#include <string.h>
/*********************************************************************************************************
** Function name:           mmu_get_cpu_id
** Descriptions:            获得 CPU ID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ID
*********************************************************************************************************/
uint32_t mmu_get_cpu_id(void)
{
    register uint32_t i;

    // MRC p15,0,Rd,c0,c0,0 ; returns ID register

    __asm__ __volatile__("MRC p15, 0, %0, c0, c0, 0":"=r"(i));

    return i;
}
/*********************************************************************************************************
** Function name:           mmu_get_cache_type
** Descriptions:            获得 Cache 类型
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          Cache 类型
*********************************************************************************************************/
uint32_t mmu_get_cache_type(void)
{
    register uint32_t i;

    // MRC p15,0,Rd,c0,c0,1 ; returns cache details

    __asm__ __volatile__("MRC p15, 0, %0, c0, c0, 1":"=r"(i));

    return i;
}
/*********************************************************************************************************
** Function name:           mmu_set_ttb
** Descriptions:            设置转换表基址
** input parameters:        i                   转换表基址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_ttb(register uint32_t i)
{
    // MRC p15, 0, Rd, c2, c0, 0 ; read TTB register

    // MCR p15, 0, Rd, c2, c0, 0 ; write TTB register

    __asm__ __volatile__("MCR p15, 0, %0, c2, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_set_domain
** Descriptions:            设置 Domain
** input parameters:        i                   Domain
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_domain(register uint32_t i)
{
    // MRC p15, 0, Rd, c3, c0, 0 ; read domain 15:0 access permissions

    // MCR p15, 0, Rd, c3, c0, 0 ; write domain 15:0 access permissions

    __asm__ __volatile__("MCR p15,0, %0, c3, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_enable
** Descriptions:            使能 MMU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("MRC p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 0);

    __asm__ __volatile__("MCR p15, 0, %0, c1, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_disable
** Descriptions:            禁能 MMU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("MRC p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 0);

    __asm__ __volatile__("MCR p15, 0, %0, c1, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_enable_align_fault_check
** Descriptions:            使能对齐错误检查
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable_align_fault_check(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("MRC p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 1);

    __asm__ __volatile__("MCR p15, 0, %0, c1, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_disable_align_fault_check
** Descriptions:            禁能对齐错误检查
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable_align_fault_check(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("MRC p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 1);

    __asm__ __volatile__("MCR p15, 0, %0, c1, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_set_sys_rom_protect_bit
** Descriptions:            设置系统和 ROM 保护位
** input parameters:        sys                 系统位
**                          rom                 ROM 保护位
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_sys_rom_protect_bit(uint32_t sys, uint32_t rom)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("MRC p15, 0, %0, c1, c0, 0":"=r"(i));

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

    __asm__ __volatile__("MCR p15, 0, %0, c1, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_set_vector_addr
** Descriptions:            设置异常向量地址
** input parameters:        vector_addr         异常向量地址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_vector_addr(uint32_t vector_addr)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("MRC p15, 0, %0, c1, c0, 0":"=r"(i));

    if (vector_addr) {
        i |=  (1 << 13);
    } else {
        i &= ~(1 << 13);
    }

    __asm__ __volatile__("MCR p15, 0, %0, c1, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_enable_dcache
** Descriptions:            使能 D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable_dcache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("MRC p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 2);

    __asm__ __volatile__("MCR p15, 0, %0, c1, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_disable_dcache
** Descriptions:            禁能 D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable_dcache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("MRC p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 2);

    __asm__ __volatile__("MCR p15, 0, %0, c1, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_enable_icache
** Descriptions:            使能 I-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable_icache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("MRC p15, 0, %0, c1, c0, 0":"=r"(i));

    i |= (1 << 12);

    __asm__ __volatile__("MCR p15, 0, %0, c1, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_disable_icache
** Descriptions:            禁能 I-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable_icache(void)
{
    register uint32_t i;

    // MRC p15, 0, Rd, c1, c0, 0 ; read control register

    // MCR p15, 0, Rd, c1, c0, 0 ; write control register

    __asm__ __volatile__("MRC p15, 0, %0, c1, c0, 0":"=r"(i));

    i &= ~(1 << 12);

    __asm__ __volatile__("MCR p15, 0, %0, c1, c0, 0": :"r"(i));
}
/*********************************************************************************************************
** Function name:           mmu_invalidate_icache_dcache
** Descriptions:            无效 I-Cache 和 D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_icache_dcache(void)
{
    // Invalidate ICache and DCache SBZ MCR p15,0,Rd,c7,c7,0

    __asm__ __volatile__("MCR p15, 0, %0, c7, c7, 0": :"r"(0));
}
/*********************************************************************************************************
** Function name:           mmu_invalidate_icache
** Descriptions:            无效 I-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_icache(void)
{
    // Invalidate ICache SBZ MCR p15,0,Rd,c7,c5,0

    __asm__ __volatile__("MCR p15, 0, %0, c7, c5, 0": :"r"(0));
}
/*********************************************************************************************************
** Function name:           mmu_invalidate_icache_mva
** Descriptions:            无效指定 MVA 的 I-Cache
** input parameters:        MVA                 修改后的虚拟地址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_icache_mva(register uint32_t MVA)
{
    // Invalidate ICache single entry (using MVA) MVA format MCR p15,0,Rd,c7,c5,1

    MVA &= ~0x1Ful;

    __asm__ __volatile__("MCR p15, 0, %0, c7, c5, 1": :"r"(MVA));
}
/*********************************************************************************************************
** Function name:           mmu_invalidate_dcache
** Descriptions:            无效 D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_dcache(void)
{
    // Invalidate DCache SBZ MCR p15,0,Rd,c7,c6,0

    __asm__ __volatile__("MCR p15, 0, %0, c7, c6, 0": :"r"(0));
}
/*********************************************************************************************************
** Function name:           mmu_clean_dcache_index
** Descriptions:            清理指定 index 的 D-Cache
** input parameters:        index               索引
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_clean_dcache_index(register uint32_t index)
{
    // Clean DCache single entry (using index) Index format MCR p15,0,Rd,c7,c10,2

    __asm__ __volatile__("MCR p15, 0, %0, c7, c10, 2": :"r"(index));
}
/*********************************************************************************************************
** Function name:           mmu_clean_invalidate_dcache_mva
** Descriptions:            清理并无效指定 MVA 的 D-Cache
** input parameters:        MVA                 修改后的虚拟地址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_clean_invalidate_dcache_mva(register uint32_t MVA)
{
    // Clean and Invalidate DCache entry (using MVA) MVA format MCR p15,0,Rd,c7,c14,1

    MVA &= ~0x1Ful;

    __asm__ __volatile__("MCR p15, 0, %0, c7, c14, 1": :"r"(MVA));
}
/*********************************************************************************************************
** Function name:           mmu_clean_invalidate_dcache_index
** Descriptions:            清理并无效指定 index 的 D-Cache
** input parameters:        index               索引
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_clean_invalidate_dcache_index(register uint32_t index)
{
    // Clean and Invalidate DCache entry (using index) Index format MCR p15,0,Rd,c7,c14,2

    __asm__ __volatile__("MCR p15, 0, %0, c7, c14, 2": :"r"(index));
}
/*********************************************************************************************************
** Function name:           mmu_drain_write_buffer
** Descriptions:            回写写缓冲
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_drain_write_buffer(void)
{
    // Drain write buffer SBZ MCR p15,0,Rd,c7,c10,4

    __asm__ __volatile__("MCR p15, 0, %0, c7, c10, 4": :"r"(0));
}
/*********************************************************************************************************
** Function name:           mmu_wait_for_interrupt
** Descriptions:            等待中断, 进入节能模式
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_wait_for_interrupt(void)
{
    // Wait for interrupt SBZ MCR p15,0,Rd,c7,c0,4

    __asm__ __volatile__("MCR p15, 0, %0, c7, c0, 4": :"r"(0));
}
/*********************************************************************************************************
** Function name:           mmu_invalidate_itlb_dtlb
** Descriptions:            无效转换旁路缓冲
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_itlb_dtlb(void)
{
    // Invalidate TLB(s) SBZ MCR p15,0,Rd,c8,c7,0

    __asm__ __volatile__("MCR p15, 0, %0, c8, c7, 0": :"r"(0));
}
/*********************************************************************************************************
** Function name:           mmu_invalidate_itlb
** Descriptions:            无效指令转换旁路缓冲
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_itlb(void)
{
    // Invalidate I TLB SBZ MCR p15,0,Rd,c8,c5,0

    __asm__ __volatile__("MCR p15, 0, %0, c8, c5, 0": :"r"(0));
}
/*********************************************************************************************************
** Function name:           mmu_invalidate_dtlb
** Descriptions:            无效数据转换旁路缓冲
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_dtlb(void)
{
    // Invalidate D TLB SBZ MCR p15,0,Rd,c8,c6,0

    __asm__ __volatile__("MCR p15, 0, %0, c8, c6, 0": :"r"(0));
}
/*********************************************************************************************************
** Function name:           mmu_invalidate_dtlb_mva
** Descriptions:            无效数据 MVA 的指令转换旁路缓冲
** input parameters:        MVA                 修改后的虚拟地址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_dtlb_mva(register uint32_t MVA)
{
    // Invalidate D TLB single entry (using MVA) MVA format MCR p15,0,Rd,c8,c6,1

    MVA &= ~0x1Ful;

    __asm__ __volatile__("MCR p15, 0, %0, c8, c6, 1": :"r"(MVA));
}
/*********************************************************************************************************
** Function name:           mmu_invalidate_itlb_mva
** Descriptions:            无效指定 MVA 的指令转换旁路缓冲
** input parameters:        MVA                 修改后的虚拟地址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_itlb_mva(register uint32_t MVA)
{
    // Invalidate I TLB single entry (using MVA) MVA format MCR p15,0,Rd,c8,c5,1

    MVA &= ~0x1Ful;

    __asm__ __volatile__("MCR p15, 0, %0, c8, c5, 1": :"r"(MVA));
}
/*********************************************************************************************************
** Function name:           mmu_get_prefetch_fault_status
** Descriptions:            获得预取指错误状态
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          预取指错误状态
*********************************************************************************************************/
uint32_t mmu_get_prefetch_fault_status(void)
{
    register uint32_t i;

    // read prefetch FSR value MRC p15, 0, Rd, c5, c0, 1

    __asm__ __volatile__("MRC p15, 0, %0, c5, c0, 1":"=r"(i));

    return i;
}
/*********************************************************************************************************
** Function name:           mmu_get_data_fault_status
** Descriptions:            获得数据访问错误状态
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          数据访问错误状态
*********************************************************************************************************/
uint32_t mmu_get_data_fault_status(void)
{
    register uint32_t i;

    // read data FSR value MRC p15, 0, Rd, c5, c0, 0

    __asm__ __volatile__("MRC p15, 0, %0, c5, c0, 0":"=r"(i));

    return i;
}
/*********************************************************************************************************
** Function name:           mmu_get_fault_address
** Descriptions:            获得错误地址
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          错误地址
*********************************************************************************************************/
uint32_t mmu_get_fault_address(void)
{
    register uint32_t i;

    // read FAR data MRC p15, 0, Rd, c6, c0, 0

    __asm__ __volatile__("MRC p15, 0, %0, c6, c0, 0":"=r"(i));

    return i;
}
/*********************************************************************************************************
** Function name:           mmu_unmap_section
** Descriptions:            取消映射段
** input parameters:        section_no          段号
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_unmap_section(uint32_t section_no)
{
    volatile uint32_t *entry = (volatile uint32_t *)MMU_TBL_BASE + section_no;

    *entry = 0;
}
/*********************************************************************************************************
** Function name:           mmu_map_section
** Descriptions:            映射段, 使用二级页表
** input parameters:        section_no          段号
**                          page_tbl_base       二级页表基址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_map_section(uint32_t section_no,
                     uint32_t page_tbl_base)
{
    volatile uint32_t *entry = (volatile uint32_t *)MMU_TBL_BASE + section_no;
    uint32_t           value;

    value = (page_tbl_base & (~(PAGE_TBL_SIZE - 1))) |
            (DOMAIN_CHECK << 5) |
            (1 << 4) |
            (1 << 0);

    *entry = value;
}
/*********************************************************************************************************
** Function name:           mmu_map_page
** Descriptions:            映射 4K 小页面
** input parameters:        page_tbl_base       二级页表基址
**                          page_no             段内页号
**                          phypage_base        物理页面基址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_map_page(uint32_t page_tbl_base,
                  uint32_t page_no,
                  uint32_t phy_page_base)
{
    volatile uint32_t *entry = (volatile uint32_t *)page_tbl_base + page_no;

    *entry = (phy_page_base & (~(VMM_PHY_PAGE_SIZE - 1))) |
            (AP_USER_RW << 10) |
            (AP_USER_RW <<  8) |
            (AP_USER_RW <<  6) |
            (AP_USER_RW <<  4) |
            (CACHE_EN   <<  3) |
            (BUFFER_EN  <<  2) |
            (1          <<  1);
}
/*********************************************************************************************************
** Function name:           mmu_map_region
** Descriptions:            映射区域
** input parameters:        virtual_base        虚拟基址
**                          physical_base       物理基址
**                          size                大小
**                          attr                属性
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_map_region(uint32_t virt_section_base,
                    uint32_t phys_section_base,
                    uint32_t size,
                    uint32_t attr)
{
    volatile uint32_t *entry;
    int                i;

    entry  = (volatile uint32_t *)MMU_TBL_BASE + (virt_section_base >> VMM_SECTION_OFFSET);

    size   = (size + VMM_SECTION_SIZE - 1) / VMM_SECTION_SIZE;

    for (i = 0; i < size; i++) {
        *entry++ = attr | (((phys_section_base >> VMM_SECTION_OFFSET) + i) << VMM_SECTION_OFFSET);
    }
}
/*********************************************************************************************************
** Function name:           arch_mmu_init
** Descriptions:            建立转换表, 初始化 MMU Cache 等
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void arch_mmu_init(void)
{
    int i, j;

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
    for (i = 0; i < VMM_SECTION_NR; i++) {
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
                   SECTION_ATTR(AP_USER_NO, DOMAIN_CHECK, CACHE_EN, BUFFER_EN));

    /*
     * 显存区
     */
    mmu_map_region(FB_MEM_BASE,
                   FB_MEM_BASE,
                   FB_MEM_SIZE,
                   SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));

    /*
     * 一级段表和二级页表都不可 Cache 及 WriteBuffer
     */
    mmu_map_region(MMU_TBL_BASE,
                   MMU_TBL_BASE,
                   MMU_TBL_SIZE + PAGE_TBL_NR * PAGE_TBL_SIZE,
                   SECTION_ATTR(AP_USER_NO, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));

    /*
     * DMA 内存区不可 Cache 及 WriteBuffer, 用户不可读写
     */
    mmu_map_region(DMA_MEM_BASE,
                   DMA_MEM_BASE,
                   DMA_MEM_SIZE,
                   SECTION_ATTR(AP_USER_NO, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));

    /*
     * HW_SHARE 内存区不可 Cache 及 WriteBuffer, 用户可读写
     *
     * HW_SHARE 内存区作为 DMA 内存区的补充, DMA 内存区供硬件 DMA 使用, 所以不可 Cache 及 WriteBuffer
     *
     * 但 DMA 内存区不能供应用程序使用, 而 HW_SHARE 内存区则可以
     *
     * 像 FrameBuffer 设备驱动可以使用 HW_SHARE 内存区
     */
    mmu_map_region(HW_SHARE_MEM_BASE,
                   HW_SHARE_MEM_BASE,
                   HW_SHARE_MEM_SIZE,
                   SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_NO, BUFFER_NO));

    /*
     * SW_SHARE 内存区可 Cache 及 WriteBuffer, 用户可读写
     *
     * 像共享内存设备驱动可以使用 SW_SHARE 内存区
     *
     * SW_SHARE 内存区不必像 HW_SHARE 内存区那样虚拟地址=物理地址
     */
    mmu_map_region(SW_SHARE_MEM_BASE,
                   SW_SHARE_MEM_BASE,
                   SW_SHARE_MEM_SIZE,
                   SECTION_ATTR(AP_USER_RW, DOMAIN_CHECK, CACHE_EN, BUFFER_EN));

    /*
     * 因为异常向量地址为 0xFFFF0000,
     * 将 0xFFF00000 映射到 INT_MEM_BASE,
     * 所以应该在 INT_MEM_BASE + 0xF0000 处(或 0xFFFF0000 处)放置异常向量跳转表
     */
    mmu_map_region(0xFFF00000,
                   INT_MEM_BASE,
                   1 * MB,
                   SECTION_ATTR(AP_USER_NO, DOMAIN_CHECK, CACHE_EN, BUFFER_EN));

    /*
     * CPU 内存映射
     */
    extern void cpu_mem_map(void);
    cpu_mem_map();

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
    memcpy((char *)VECTOR_P_ADDR, (char *)KERN_MEM_BASE, VMM_PAGE_SIZE);
}
/*********************************************************************************************************
** 系统保留虚拟地址空间
*********************************************************************************************************/
const mem_space_t sys_resv_space[] = {
        {
                PHY_MEM_BASE,
                PHY_MEM_SIZE
        },
        {
                0xFFF00000,
                1 * MB
        },
        {
                0,
                0
        }
};
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
