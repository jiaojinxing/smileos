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

#include "kern/types.h"

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

/*
 * 获得 ID
 */
uint32_t mmu_get_id(void);

/*
 * 获得 Cache 类型
 */
uint32_t mmu_get_cache_type(void);

/*
 * 设置转换表基址
 */
void mmu_set_ttb(register uint32_t i);

/*
 * 设置 Domain
 */
void mmu_set_domain(register uint32_t i);

/*
 * 使能 MMU
 */
void mmu_enable(void);

/*
 * 禁能 MMU
 */
void mmu_disable(void);

/*
 * 使能对齐错误检查
 */
void mmu_enable_align_fault_check(void);

/*
 * 禁能对齐错误检查
 */
void mmu_disable_align_fault_check(void);

/*
 * 设置系统和 ROM 保护位
 */
void mmu_set_sys_rom_protect_bit(register uint32_t sys, register uint32_t rom);

/*
 * 设置异常向量地址
 */
void mmu_set_vector_addr(register uint32_t vector_addr);

/*
 * 使能 D-Cache
 */
void mmu_enable_dcache(void);

/*
 * 禁能 D-Cache
 */
void mmu_disable_dcache(void);

/*
 * 使能 I-Cache
 */
void mmu_enable_icache(void);

/*
 * 禁能 I-Cache
 */
void mmu_disable_icache(void);

/*
 * 无效 I-Cache 和 D-Cache
 */
void mmu_invalidate_icache_dcache(void);

/*
 * 无效 I-Cache
 */
void mmu_invalidate_icache(void);

/*
 * 无效 D-Cache
 */
void mmu_invalidate_dcache(void);

/*
 * 清理指定 index 的 D-Cache
 */
void mmu_clean_dcache_index(register uint32_t index);

/*
 * 清理并无效指定 index 的 D-Cache
 */
void mmu_clean_invalidate_dcache_index(register uint32_t index);

/*
 * 回写写缓冲
 */
void mmu_drain_write_buffer(void);

/*
 * 等待中断, 进入节能模式
 */
void mmu_wait_for_interrupt(void);

/*
 * 无效转换旁路缓冲
 */
void mmu_invalidate_itlb_dtlb(void);

/*
 * 无效指令转换旁路缓冲
 */
void mmu_invalidate_itlb(void);

/*
 * 无效数据转换旁路缓冲
 */
void mmu_invalidate_dtlb(void);

/*
 * 获得预取指错误状态
 */
uint32_t mmu_get_prefetch_fault_status(void);

/*
 * 获得数据访问错误状态
 */
uint32_t mmu_get_data_fault_status(void);

/*
 * 获得错误地址
 */
uint32_t mmu_get_fault_address(void);

/*
 * 取消映射段
 */
void mmu_unmap_section(register uint32_t section_nr);

/*
 * 映射段, 通过参数
 */
void mmu_map_section(register uint32_t section_nr,
                     register uint32_t value);

/*
 * 映射区域
 */
void mmu_map_region(register uint32_t virtual_base,
                    register uint32_t physical_base,
                    register uint32_t size,
                    register uint32_t attr);

/*
 * 映射段, 使用二级页表
 */
uint32_t mmu_map_section_as_page(register uint32_t section_nr,
                                 register uint32_t page_tbl_base);

/*
 * 映射 4K 小页面
 */
void mmu_map_page(register uint32_t page_tbl_base,
                  register uint32_t page_nr,
                  register uint32_t frame_base);

/*
 * 建立转换表, 初始化 MMU Cache 等
 */
void mmu_init(void);

#endif                                                                  /*  MMU_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
