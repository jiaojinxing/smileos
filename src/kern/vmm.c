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
** Modified by:             JiaoJinXing
** Modified date:           2012-3-12
** Version:                 1.1.0
** Descriptions:            把已用页表链表改为已用页表红黑树, 提升查找性能
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-26
** Version:                 1.2.0
** Descriptions:            加入进程的一级段表记录, 以实现进程虚拟地址空间的保护
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/mmu.h"
#include <sys/tree.h>
#include <string.h>

/*
 * 页表
 */
struct page_table {
    RB_ENTRY(page_table)    node;                                       /*  红黑树节点                  */
    uint32_t                section_nr;                                 /*  段号                        */
};

static struct page_table  page_tables[PAGE_TBL_NR];                     /*  页表                        */
static struct page_table *free_page_table_list;                         /*  空闲页表链表                */

/*
 * 页表比较函数
 */
static inline int vmm_page_table_compare(struct page_table *a, struct page_table *b)
{
#if 0
    if (a->section_nr < b->section_nr) {
        return -1;
    } else if (a->section_nr > b->section_nr) {
        return 1;
    } else {
        return 0;
    }
#else
    return a->section_nr - b->section_nr;
#endif
}

static RB_HEAD(page_table_tree, page_table) used_page_table_tree;       /*  已用页表红黑树              */

/*
 * 产生页表红黑树代码
 */
RB_GENERATE_INTERNAL(page_table_tree, page_table, node, vmm_page_table_compare, static);

/*
 * 根据段号查找页表
 */
static uint32_t vmm_page_table_lookup(uint32_t section_nr)
{
    struct page_table *tbl;
    struct page_table  tmp;

    tmp.section_nr = section_nr;

    tbl = page_table_tree_RB_FIND(&used_page_table_tree, &tmp);         /*  在已用页表红黑树中查找      */
    if (tbl != NULL) {
        return (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;     /*  返回页表基址                */
    } else {
        return 0;
    }
}

/*
 * 分配页表
 */
static uint32_t vmm_page_table_alloc(uint32_t section_nr)
{
    struct page_table *tbl;

    tbl = free_page_table_list;                                         /*  在空闲页表链表链头处分配    */
    if (tbl != NULL) {
        free_page_table_list = tbl->node.rbe_right;

        tbl->section_nr = section_nr;
        page_table_tree_RB_INSERT(&used_page_table_tree, tbl);          /*  加入到已用页表红黑树中      */

        return (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;     /*  返回页表基址                */
    } else {
        return 0;
    }
}

/*
 * 释放页表
 */
static void vmm_page_table_free(uint32_t page_tbl_base)
{
    struct page_table *tbl;

    tbl = page_tables + (page_tbl_base - PAGE_TBL_BASE) / PAGE_TBL_SIZE;/*  计算页表                    */

    page_table_tree_RB_REMOVE(&used_page_table_tree, tbl);              /*  从已用页表红黑树中移除      */

    tbl->node.rbe_right = free_page_table_list;                         /*  加入到空闲页表链表中        */
    free_page_table_list = tbl;
}

/*
 * 页框
 */
struct vmm_frame;
typedef struct vmm_frame vmm_frame_t;
struct vmm_frame {
    struct vmm_frame *prev;                                             /*  前趋                        */
    struct vmm_frame *next;                                             /*  后趋                        */
    struct vmm_frame *process_next;                                     /*  进程后趋                    */
};

static vmm_frame_t vmm_frames[VMM_FRAME_NR];                            /*  页框                        */

static vmm_frame_t *free_frame_list;                                    /*  空闲页框链表                */
static vmm_frame_t *used_frame_list;                                    /*  已用页框链表                */

/*
 * 分配页框
 */
static vmm_frame_t *vmm_frame_alloc(task_t *task)
{
    vmm_frame_t *frame;

    frame = free_frame_list;                                            /*  在空闲页框链表链头处分配    */
    if (frame != NULL) {
        free_frame_list = frame->next;

        frame->prev = NULL;                                             /*  加入到已用页框链表中        */
        frame->next = used_frame_list;
        if (used_frame_list != NULL) {
            used_frame_list->prev = frame;
        }
        used_frame_list = frame;

        frame->process_next = task->frame_list;                         /*  加入到进程页框链表中        */
        task->frame_list = frame;
        task->frame_nr++;
    }
    return frame;
}

/*
 * 释放页框
 */
static void vmm_frame_free(vmm_frame_t *frame)
{
    if (frame->prev != NULL) {                                          /*  从已用页框链表中删除        */
        frame->prev->next = frame->next;
    } else {
        used_frame_list = frame->next;
    }

    if (frame->next != NULL) {
        frame->next->prev = frame->prev;
    }

    frame->next = free_frame_list;                                      /*  加入到空闲页框链表中        */
    free_frame_list = frame;
}

/*
 * 获得页框的物理地址
 */
static uint32_t vmm_get_frame_addr(vmm_frame_t *frame)
{
    return (frame - vmm_frames) * VMM_FRAME_SIZE + VMM_MEM_BASE;
}

/*
 * 通过一个虚拟地址映射进程虚拟地址空间中的一个页面
 */
int vmm_map_process_page(task_t *task, uint32_t va)
{
    vmm_frame_t *frame;
    int          flag = FALSE;

    if (   va >= PROCESS_SPACE_SIZE *  task->pid                        /*  判断虚拟地址是否在进程      */
        && va <  PROCESS_SPACE_SIZE * (task->pid + 1)) {                /*  的虚拟地址空间范围内        */

        uint32_t section_nr = va >> SECTION_OFFSET;                     /*  计算虚拟地址的段号          */

        uint32_t tbl = vmm_page_table_lookup(section_nr);               /*  查找该段的页表              */
        if (tbl == 0) {                                                 /*  没找到                      */
            tbl = vmm_page_table_alloc(section_nr);                     /*  分配一个空闲的页表          */
            if (tbl != NULL) {
                task->mmu_backup[section_nr % (PROCESS_SPACE_SIZE / SECTION_SIZE)] =
                        mmu_map_section_as_page(section_nr, tbl);       /*  映射该段                    */
                flag = TRUE;
            } else {
                printk("failed to alloc page table, mem map failed, va=0x%x, pid=%d\n", va, task->pid);
                return -1;
            }
        }

        frame = vmm_frame_alloc(task);                                  /*  分配一个空闲的页框          */
        if (frame != NULL) {                                            /*  计算虚拟地址在页表里的页号  */
            uint32_t page_nr = (va & (SECTION_SIZE - 1)) >> PAGE_OFFSET;
            mmu_map_page(tbl, page_nr, vmm_get_frame_addr(frame));      /*  页面映射                    */
            return 0;
        } else {
            /*
             * TODO: 现在还不支持交换
             */
            if (flag) {
                mmu_unmap_section(section_nr);
                task->mmu_backup[section_nr % (PROCESS_SPACE_SIZE / SECTION_SIZE)] = 0;
                vmm_page_table_free(tbl);
            }
            printk("failed to alloc page, mem map failed, va=0x%x, pid=%d\n", va, task->pid);
            return -1;
        }
    } else {
        printk("invalid va=0x%x, mem map failed, pid=%d\n", va, task->pid);
        return -1;
    }
}

/*
 * 释放进程的虚拟地址空间
 */
void vmm_free_process_space(task_t *task)
{
    int          i;
    vmm_frame_t *next;
    uint32_t     tbl;

    while (task->frame_list != NULL) {                                  /*  释放进程占用的页框          */
        next = task->frame_list->process_next;
        vmm_frame_free(task->frame_list);
        task->frame_list = next;
    }
    task->frame_nr = 0;

    for (i = 0; i < PROCESS_SPACE_SIZE / SECTION_SIZE; i++) {           /*  释放进程占用的页表          */
        tbl = vmm_page_table_lookup(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + i);
        if (tbl != 0) {
            vmm_page_table_free(tbl);                                   /*  释放页表                    */
        }                                                               /*  取消映射段                  */
        mmu_unmap_section(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + i);
        task->mmu_backup[i] = 0;
    }
}

/*
 * 初始化虚拟内存管理
 */
void vmm_init(void)
{
    int                i;
    vmm_frame_t       *frame;
    struct page_table *tbl;

    /*
     * 初始化空闲页框链表
     */
    for (i = 0, frame = vmm_frames; i < VMM_FRAME_NR - 1; i++, frame++) {
        frame->next = frame + 1;
    }
    frame->next = NULL;
    free_frame_list = vmm_frames;

    /*
     * 初始化已用页框链表
     */
    used_frame_list = NULL;

    /*
     * 初始化空闲页表链表
     */
    for (i = 0, tbl = page_tables; i < PAGE_TBL_NR - 1; i++, tbl++) {
        tbl->node.rbe_right = tbl + 1;
    }
    tbl->node.rbe_right = NULL;
    free_page_table_list = page_tables;

    /*
     * 初始化已用页表红黑树
     */
    RB_INIT(&used_page_table_tree);

    /*
     * 清除所有页表
     */
    memset((void *)PAGE_TBL_BASE, 0, PAGE_TBL_SIZE * PAGE_TBL_NR);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
