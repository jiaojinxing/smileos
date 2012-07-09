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
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-6-15
** Version:                 1.2.0
** Descriptions:            修改代码中的一处错误:
**                          原来: frame->mva = mva & (PAGE_SIZE - 1);
**
**                          修正: frame->mva = mva & (~(PAGE_SIZE - 1));
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-7-4
** Version:                 1.2.0
** Descriptions:            修改代码中的一处比较严重的错误, 释放的页表应该清零! 不然下次再用这个页表时,
**                          也不知道它指向了那些旧的页框, 引发不可预测的错误!
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
static int                free_page_table_nr = PAGE_TBL_NR;

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

        free_page_table_nr--;

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

    /*
     * !!! 这个 BUG 找得我好苦 !!!
     */
    memset((void *)page_tbl_base, 0, PAGE_TBL_SIZE);

    tbl = page_tables + (page_tbl_base - PAGE_TBL_BASE) / PAGE_TBL_SIZE;/*  计算页表                    */

    page_table_tree_RB_REMOVE(&used_page_table_tree, tbl);              /*  从已用页表红黑树中移除      */

    tbl->node.rbe_right  = free_page_table_list;                        /*  加入到空闲页表链表中        */
    free_page_table_list = tbl;

    free_page_table_nr++;
}

/*
 * 页框
 */
struct vmm_frame;
typedef struct vmm_frame vmm_frame_t;
struct vmm_frame {
    uint32_t          mva;                                              /*  映射到的页面的虚拟基址      */
    struct vmm_frame *prev;                                             /*  前趋                        */
    struct vmm_frame *next;                                             /*  后趋                        */
    struct vmm_frame *process_next;                                     /*  进程后趋                    */
};

static vmm_frame_t vmm_frames[VMM_FRAME_NR];                            /*  页框                        */

static vmm_frame_t *free_frame_list;                                    /*  空闲页框链表                */
static vmm_frame_t *used_frame_list;                                    /*  已用页框链表                */
static int          free_frame_nr = VMM_FRAME_NR;

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

        free_frame_nr--;
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

    free_frame_nr++;
}

/*
 * 获得页框的物理地址
 */
static inline uint32_t vmm_frame_addr(vmm_frame_t *frame)
{
    return (frame - vmm_frames) * VMM_FRAME_SIZE + VMM_MEM_BASE;
}

/*
 * 通过一个虚拟地址映射进程虚拟地址空间中的一个页面
 */
int vmm_page_map(task_t *task, uint32_t mva)
{
    vmm_frame_t *frame;
    int          flag = FALSE;

    if (   mva >= PROCESS_SPACE_SIZE *  task->pid                       /*  判断虚拟地址是否在进程      */
        && mva <  PROCESS_SPACE_SIZE * (task->pid + 1)) {               /*  的虚拟地址空间范围内        */

        uint32_t section_nr = mva >> SECTION_OFFSET;                    /*  计算虚拟地址的段号          */

        uint32_t tbl = vmm_page_table_lookup(section_nr);               /*  查找该段的页表              */
        if (tbl == 0) {                                                 /*  没找到                      */
            tbl = vmm_page_table_alloc(section_nr);                     /*  分配一个空闲的页表          */
            if (tbl != 0) {
                task->mmu_backup[section_nr % (PROCESS_SPACE_SIZE / SECTION_SIZE)] =
                        mmu_map_section_as_page(section_nr, tbl);       /*  映射该段                    */
                flag = TRUE;
            } else {
                printk("failed to alloc page table, mem map failed, mva=0x%x, pid=%d\n", mva, task->pid);
                return -1;
            }
        }

        frame = vmm_frame_alloc(task);                                  /*  分配一个空闲的页框          */
        if (frame != NULL) {                                            /*  计算虚拟地址在页表里的页号  */
            uint32_t page_nr = (mva & (SECTION_SIZE - 1)) >> PAGE_OFFSET;
            frame->mva = mva & (~(PAGE_SIZE - 1));
            mmu_map_page(tbl, page_nr, vmm_frame_addr(frame));          /*  页面映射                    */
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
            printk("failed to alloc page, mem map failed, mva=0x%x, pid=%d\n", mva, task->pid);
            return -1;
        }
    } else {
        printk("invalid mva=0x%x, mem map failed, pid=%d\n", mva, task->pid);
        return -1;
    }
}

/*
 * 清理进程的虚拟内存信息
 */
int vmm_process_cleanup(task_t *task)
{
    int          i;
    vmm_frame_t *frame;
    uint32_t     tbl;

    while ((frame = task->frame_list) != NULL) {                        /*  释放进程占用的页框          */
        task->frame_list = frame->process_next;
        vmm_frame_free(frame);                                          /*  释放页框                    */
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

    mmu_invalidate_itlb_dtlb();

    return 0;
}

/*
 * 初始化进程的虚拟内存信息
 */
int vmm_process_init(task_t *task, uint32_t file_size)
{
    int i;

    /*
     * 为拷贝代码到进程的虚拟地址空间, 预先映射好页面
     */
    for (i = 0; i < (file_size + PAGE_SIZE - 1) / PAGE_SIZE; i++) {
        if (vmm_page_map(task, task->pid * PROCESS_SPACE_SIZE + i * PAGE_SIZE) < 0) {
            vmm_process_cleanup(task);
            return -1;
        }
    }

    /*
     * 为进程栈空间映射一个页面
     */
    if (vmm_page_map(task, (task->pid + 1) * PROCESS_SPACE_SIZE - PAGE_SIZE) < 0) {
        vmm_process_cleanup(task);
        return -1;
    }

    return 0;
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

#include <stdio.h>
#include <unistd.h>

/*
 * 打印 vmm 信息到文件
 */
int vmm_show(int fd)
{
    char   buf[LINE_MAX];
    int    len;

    if (fd < 0) {
        return -1;
    }

    len = sprintf(buf, "********** vmm info **********\n");
    write(fd, buf, len);

    len = sprintf(buf, "free page table_nr = %d\n", free_page_table_nr);
    write(fd, buf, len);

    len = sprintf(buf, "free frame nr      = %d\n", free_frame_nr);
    write(fd, buf, len);

    len = sprintf(buf, "************* end *************\n");
    write(fd, buf, len);

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
