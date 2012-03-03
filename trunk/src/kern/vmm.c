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
** File name:               vmm.c
** Last modified Date:      2012-3-1
** Last Version:            1.0.0
** Descriptions:            虚拟内存管理
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-1
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/mmu.h"
#include <string.h>

/*
 * 页表
 */
typedef struct _page_table_t {
    struct _page_table_t *prev;
    struct _page_table_t *next;
    uint32_t              section_nr;
} page_table_t;

static page_table_t  page_tables[PAGE_TBL_NR];
static page_table_t *free_page_table_list;
static page_table_t *used_page_table_list;

/*
 * 根据段号查找页表
 *
 * TODO: 这样的查找算法还是挺慢的, 有待改进
 */
uint32_t page_table_lookup(uint32_t section_nr)
{
    page_table_t *tbl;

    tbl = used_page_table_list;
    while (tbl) {
        if (tbl->section_nr == section_nr) {
            break;
        }
        tbl = tbl->next;
    }

    if (tbl) {
        return (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;
    } else {
        return 0;
    }
}

/*
 * 分配页表
 */
uint32_t page_table_alloc(uint32_t section_nr)
{
    page_table_t *tbl;

    tbl = free_page_table_list;
    if (tbl) {
        free_page_table_list = tbl->next;

        tbl->prev = NULL;
        tbl->next = used_page_table_list;
        if (used_page_table_list) {
            used_page_table_list->prev = tbl;
        }
        used_page_table_list = tbl;

        tbl->section_nr = section_nr;

        return (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;
    } else {
        return 0;
    }
}

/*
 * 释放页表
 */
void page_table_free(uint32_t page_tbl_base)
{
    page_table_t *tbl;

    tbl = page_tables + (page_tbl_base - PAGE_TBL_BASE) / PAGE_TBL_SIZE;

    if (tbl->prev) {
        tbl->prev->next = tbl->next;
    } else {
        used_page_table_list = tbl->next;
    }

    if (tbl->next) {
        tbl->next->prev = tbl->prev;
    }

    tbl->next = free_page_table_list;
    free_page_table_list = tbl;
}

/*
 * 页框
 */
typedef struct _frame_t {
    uint32_t         virtual_addr;
    struct _frame_t *prev;
    struct _frame_t *next;
    struct _frame_t *process_next;
} frame_t;

static frame_t frames[FRAME_NR];

static frame_t *free_frame_list;
static frame_t *used_frame_list;

/*
 * 分配页框
 */
frame_t *frame_alloc(void)
{
    frame_t *free_frame;

    free_frame = free_frame_list;
    if (free_frame) {
        free_frame_list = free_frame->next;
    }
    return free_frame;
}

/*
 * 释放页框
 */
void frame_free(frame_t *frame)
{
    frame->next = free_frame_list;
    free_frame_list = frame;
}

/*
 * 获得页框的物理地址
 */
uint32_t get_frame_addr(frame_t *frame)
{
    return (frame - frames) * FRAME_SIZE + VMM_MEM_BASE;
}

/*
 * 页面映射
 */
int vmm_map_page(task_t *task, uint32_t va)
{
    frame_t *frame;
    int      flag = 0;

    if (   va >= PROCESS_SPACE_SIZE *  task->pid                        /*  判断虚拟地址是否在进程      */
        && va <  PROCESS_SPACE_SIZE * (task->pid + 1)) {                /*  的虚拟地址空间范围内        */

        uint32_t section_nr = va >> SECTION_OFFSET;                     /*  计算虚拟地址的段号          */

        uint32_t tbl = page_table_lookup(section_nr);                   /*  查找该段的页表              */
        if (!tbl) {                                                     /*  没找到                      */
            tbl = page_table_alloc(section_nr);                         /*  分配一个空闲的页表          */
            if (tbl) {
                mmu_map_section_as_page(section_nr, tbl);               /*  映射该段                    */
                flag = 1;
            } else {
                printk("failed to alloc page table, map failed, va=0x%x, pid=%d\n", va, task->pid);
                return -1;
            }
        }

        frame = frame_alloc();                                          /*  分配一个空闲的页框          */
        if (frame) {                                                    /*  计算虚拟地址在页表里的页号  */
            uint32_t page_nr = (va & (SECTION_SIZE - 1)) >> PAGE_OFFSET;

            mmu_map_page(tbl, page_nr, get_frame_addr(frame));          /*  页面映射                    */

            frame->prev = NULL;
            frame->next = used_frame_list;
            if (used_frame_list) {
                used_frame_list->prev = frame;
            }

            frame->process_next = task->frame_list;
            task->frame_list = frame;

            return 0;
        } else {
            if (flag) {
                mmu_unmap_section(section_nr);
                page_table_free(tbl);
            }
            printk("failed to alloc frame, map failed, va=0x%x, pid=%d\n", va, task->pid);
            return -1;
        }
    } else {
        printk("invalid va=0x%x, map failed, pid=%d\n", va, task->pid);
        return -1;
    }
}

/*
 * 初始化虚拟内存管理
 */
void vmm_init(void)
{
    int           i;
    frame_t      *frame;
    page_table_t *tbl;

    for (i = 0, frame = frames; i < FRAME_NR - 1; i++, frame++) {
        frame->next = frame + 1;
    }
    frame->next = NULL;
    free_frame_list = frames;
    used_frame_list = NULL;

    for (i = 0, tbl = page_tables; i < PAGE_TBL_NR - 1; i++, tbl++) {
        tbl->next = tbl + 1;
    }
    tbl->next = NULL;
    free_page_table_list = page_tables;
    used_page_table_list = NULL;

    memset((void *)PAGE_TBL_BASE, 0, PAGE_TBL_SIZE * PAGE_TBL_NR);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
