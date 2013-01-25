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
** Descriptions:            ARM920T MMU 底层接口函数库
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
#ifndef MMU_H_
#define MMU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "kern/types.h"
/*********************************************************************************************************
** 定义
*********************************************************************************************************/
/*
 * Domain
 */
#define DOMAIN_NO_ACCESS        0
#define DOMAIN_CHECK            1
#define DOMAIN_NO_CHECK         3

#define DOMAIN0_ATTR            DOMAIN_NO_ACCESS
#define DOMAIN1_ATTR            DOMAIN_CHECK
#define DOMAIN3_ATTR            DOMAIN_NO_CHECK

/*
 * 系统和 ROM 保护位分别设置为 0, 1
 */
#define SYS_PROTECT_BIT         0
#define ROM_PROTECT_BIT         1

/*
 * 异常向量地址
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
 * 段属性
 */
#define SECTION_ATTR(ap, domain, cache, buffer) \
        (ap << 10) + (domain << 5) + (1 << 4) + (cache << 3) + (buffer << 2) + (1 << 1) + (0 << 0)
/*********************************************************************************************************
** Function name:           mmu_get_cpu_id
** Descriptions:            获得 CPU ID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ID
*********************************************************************************************************/
uint32_t mmu_get_cpu_id(void);
/*********************************************************************************************************
** Function name:           mmu_get_cache_type
** Descriptions:            获得 Cache 类型
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          Cache 类型
*********************************************************************************************************/
uint32_t mmu_get_cache_type(void);
/*********************************************************************************************************
** Function name:           mmu_set_ttb
** Descriptions:            设置转换表基址
** input parameters:        i                   转换表基址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_ttb(register uint32_t i);
/*********************************************************************************************************
** Function name:           mmu_set_domain
** Descriptions:            设置 Domain
** input parameters:        i                   Domain
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_domain(register uint32_t i);
/*********************************************************************************************************
** Function name:           mmu_enable
** Descriptions:            使能 MMU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable(void);
/*********************************************************************************************************
** Function name:           mmu_disable
** Descriptions:            禁能 MMU
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable(void);
/*********************************************************************************************************
** Function name:           mmu_enable_align_fault_check
** Descriptions:            使能对齐错误检查
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable_align_fault_check(void);
/*********************************************************************************************************
** Function name:           mmu_disable_align_fault_check
** Descriptions:            禁能对齐错误检查
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable_align_fault_check(void);
/*********************************************************************************************************
** Function name:           mmu_set_sys_rom_protect_bit
** Descriptions:            设置系统和 ROM 保护位
** input parameters:        sys                 系统位
**                          rom                 ROM 保护位
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_sys_rom_protect_bit(uint32_t sys, uint32_t rom);
/*********************************************************************************************************
** Function name:           mmu_set_vector_addr
** Descriptions:            设置异常向量地址
** input parameters:        vector_addr         异常向量地址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_set_vector_addr(uint32_t vector_addr);
/*********************************************************************************************************
** Function name:           mmu_enable_dcache
** Descriptions:            使能 D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable_dcache(void);
/*********************************************************************************************************
** Function name:           mmu_disable_dcache
** Descriptions:            禁能 D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable_dcache(void);
/*********************************************************************************************************
** Function name:           mmu_enable_icache
** Descriptions:            使能 I-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_enable_icache(void);
/*********************************************************************************************************
** Function name:           mmu_disable_icache
** Descriptions:            禁能 I-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_disable_icache(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_icache_dcache
** Descriptions:            无效 I-Cache 和 D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_icache_dcache(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_icache
** Descriptions:            无效 I-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_icache(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_icache_mva
** Descriptions:            无效指定 MVA 的 I-Cache
** input parameters:        MVA                 修改后的虚拟地址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_icache_mva(register uint32_t MVA);
/*********************************************************************************************************
** Function name:           mmu_invalidate_dcache
** Descriptions:            无效 D-Cache
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_dcache(void);
/*********************************************************************************************************
** Function name:           mmu_clean_dcache_index
** Descriptions:            清理指定 index 的 D-Cache
** input parameters:        index               索引
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_clean_dcache_index(register uint32_t index);
/*********************************************************************************************************
** Function name:           mmu_clean_invalidate_dcache_mva
** Descriptions:            清理并无效指定 MVA 的 D-Cache
** input parameters:        MVA                 修改后的虚拟地址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_clean_invalidate_dcache_mva(register uint32_t MVA);
/*********************************************************************************************************
** Function name:           mmu_clean_invalidate_dcache_index
** Descriptions:            清理并无效指定 index 的 D-Cache
** input parameters:        index               索引
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_clean_invalidate_dcache_index(register uint32_t index);
/*********************************************************************************************************
** Function name:           mmu_drain_write_buffer
** Descriptions:            回写写缓冲
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_drain_write_buffer(void);
/*********************************************************************************************************
** Function name:           mmu_wait_for_interrupt
** Descriptions:            等待中断, 进入节能模式
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_wait_for_interrupt(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_itlb_dtlb
** Descriptions:            无效转换旁路缓冲
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_itlb_dtlb(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_itlb
** Descriptions:            无效指令转换旁路缓冲
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_itlb(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_dtlb
** Descriptions:            无效数据转换旁路缓冲
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_dtlb(void);
/*********************************************************************************************************
** Function name:           mmu_invalidate_dtlb_mva
** Descriptions:            无效数据 MVA 的指令转换旁路缓冲
** input parameters:        MVA                 修改后的虚拟地址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_dtlb_mva(register uint32_t MVA);
/*********************************************************************************************************
** Function name:           mmu_invalidate_itlb_mva
** Descriptions:            无效指定 MVA 的指令转换旁路缓冲
** input parameters:        MVA                 修改后的虚拟地址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_invalidate_itlb_mva(register uint32_t MVA);
/*********************************************************************************************************
** Function name:           mmu_get_prefetch_fault_status
** Descriptions:            获得预取指错误状态
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          预取指错误状态
*********************************************************************************************************/
uint32_t mmu_get_prefetch_fault_status(void);
/*********************************************************************************************************
** Function name:           mmu_get_data_fault_status
** Descriptions:            获得数据访问错误状态
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          数据访问错误状态
*********************************************************************************************************/
uint32_t mmu_get_data_fault_status(void);
/*********************************************************************************************************
** Function name:           mmu_get_fault_address
** Descriptions:            获得错误地址
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          错误地址
*********************************************************************************************************/
uint32_t mmu_get_fault_address(void);
/*********************************************************************************************************
** Function name:           mmu_map_section
** Descriptions:            映射段, 通过参数
** input parameters:        section_no          段号
**                          value               参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_map_section(uint32_t section_no,
                     uint32_t value);
/*********************************************************************************************************
** Function name:           mmu_unmap_section
** Descriptions:            取消映射段
** input parameters:        section_no          段号
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_unmap_section(uint32_t section_no);
/*********************************************************************************************************
** Function name:           mmu_map_section_as_page
** Descriptions:            映射段, 使用二级页表
** input parameters:        section_no          段号
**                          page_tbl_base       二级页表基址
** output parameters:       NONE
** Returned value:          可用作 mmu_map_section 参数的值
*********************************************************************************************************/
uint32_t mmu_map_section_as_page(uint32_t section_no,
                                 uint32_t page_tbl_base);
/*********************************************************************************************************
** Function name:           mmu_map_page
** Descriptions:            映射 4K 小页面
** input parameters:        page_tbl_base       二级页表基址
**                          page_no             段内页号
**                          frame_base          页框基址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_map_page(uint32_t page_tbl_base,
                  uint32_t page_no,
                  uint32_t frame_base);
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
void mmu_map_region(uint32_t virtual_base,
                    uint32_t physical_base,
                    uint32_t size,
                    uint32_t attr);
/*********************************************************************************************************
** Function name:           mmu_init
** Descriptions:            建立转换表, 初始化 MMU Cache 等
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void mmu_init(void);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  MMU_H_                      */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
