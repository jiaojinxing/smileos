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
** Descriptions:            �����ڴ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-1
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-12
** Version:                 1.1.0
** Descriptions:            ������ҳ�������Ϊ����ҳ������, ������������
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-26
** Version:                 1.2.0
** Descriptions:            ������̵�һ���α��¼, ��ʵ�ֽ��������ַ�ռ�ı���
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/mmu.h"
#include <sys/tree.h>
#include <string.h>

/*
 * ҳ��
 */
struct page_table {
    RB_ENTRY(page_table)    node;                                       /*  ������ڵ�                  */
    uint32_t                section_nr;                                 /*  �κ�                        */
};

static struct page_table  page_tables[PAGE_TBL_NR];                     /*  ҳ��                        */
static struct page_table *free_page_table_list;                         /*  ����ҳ������                */

/*
 * ҳ��ȽϺ���
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

static RB_HEAD(page_table_tree, page_table) used_page_table_tree;       /*  ����ҳ������              */

/*
 * ����ҳ����������
 */
RB_GENERATE_INTERNAL(page_table_tree, page_table, node, vmm_page_table_compare, static);

/*
 * ���ݶκŲ���ҳ��
 */
static uint32_t vmm_page_table_lookup(uint32_t section_nr)
{
    struct page_table *tbl;
    struct page_table  tmp;

    tmp.section_nr = section_nr;

    tbl = page_table_tree_RB_FIND(&used_page_table_tree, &tmp);         /*  ������ҳ�������в���      */
    if (tbl != NULL) {
        return (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;     /*  ����ҳ���ַ                */
    } else {
        return 0;
    }
}

/*
 * ����ҳ��
 */
static uint32_t vmm_page_table_alloc(uint32_t section_nr)
{
    struct page_table *tbl;

    tbl = free_page_table_list;                                         /*  �ڿ���ҳ��������ͷ������    */
    if (tbl != NULL) {
        free_page_table_list = tbl->node.rbe_right;

        tbl->section_nr = section_nr;
        page_table_tree_RB_INSERT(&used_page_table_tree, tbl);          /*  ���뵽����ҳ��������      */

        return (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;     /*  ����ҳ���ַ                */
    } else {
        return 0;
    }
}

/*
 * �ͷ�ҳ��
 */
static void vmm_page_table_free(uint32_t page_tbl_base)
{
    struct page_table *tbl;

    tbl = page_tables + (page_tbl_base - PAGE_TBL_BASE) / PAGE_TBL_SIZE;/*  ����ҳ��                    */

    page_table_tree_RB_REMOVE(&used_page_table_tree, tbl);              /*  ������ҳ���������Ƴ�      */

    tbl->node.rbe_right  = free_page_table_list;                        /*  ���뵽����ҳ��������        */
    free_page_table_list = tbl;
}

/*
 * ҳ��
 */
struct vmm_frame;
typedef struct vmm_frame vmm_frame_t;
struct vmm_frame {
    struct vmm_frame *prev;                                             /*  ǰ��                        */
    struct vmm_frame *next;                                             /*  ����                        */
    struct vmm_frame *process_next;                                     /*  ���̺���                    */
};

static vmm_frame_t vmm_frames[VMM_FRAME_NR];                            /*  ҳ��                        */

static vmm_frame_t *free_frame_list;                                    /*  ����ҳ������                */
static vmm_frame_t *used_frame_list;                                    /*  ����ҳ������                */

/*
 * ����ҳ��
 */
static vmm_frame_t *vmm_frame_alloc(task_t *task)
{
    vmm_frame_t *frame;

    frame = free_frame_list;                                            /*  �ڿ���ҳ��������ͷ������    */
    if (frame != NULL) {
        free_frame_list = frame->next;

        frame->prev = NULL;                                             /*  ���뵽����ҳ��������        */
        frame->next = used_frame_list;
        if (used_frame_list != NULL) {
            used_frame_list->prev = frame;
        }
        used_frame_list = frame;

        frame->process_next = task->frame_list;                         /*  ���뵽����ҳ��������        */
        task->frame_list = frame;
        task->frame_nr++;
    }
    return frame;
}

/*
 * �ͷ�ҳ��
 */
static void vmm_frame_free(vmm_frame_t *frame)
{
    if (frame->prev != NULL) {                                          /*  ������ҳ��������ɾ��        */
        frame->prev->next = frame->next;
    } else {
        used_frame_list = frame->next;
    }

    if (frame->next != NULL) {
        frame->next->prev = frame->prev;
    }

    frame->next = free_frame_list;                                      /*  ���뵽����ҳ��������        */
    free_frame_list = frame;
}

/*
 * ���ҳ��������ַ
 */
static inline uint32_t vmm_frame_addr(vmm_frame_t *frame)
{
    return (frame - vmm_frames) * VMM_FRAME_SIZE + VMM_MEM_BASE;
}

/*
 * ͨ��һ�������ַӳ����������ַ�ռ��е�һ��ҳ��
 */
int vmm_page_map(task_t *task, uint32_t va)
{
    vmm_frame_t *frame;
    int          flag = FALSE;

    if (   va >= PROCESS_SPACE_SIZE *  task->pid                        /*  �ж������ַ�Ƿ��ڽ���      */
        && va <  PROCESS_SPACE_SIZE * (task->pid + 1)) {                /*  �������ַ�ռ䷶Χ��        */

        uint32_t section_nr = va >> SECTION_OFFSET;                     /*  ���������ַ�Ķκ�          */

        uint32_t tbl = vmm_page_table_lookup(section_nr);               /*  ���Ҹöε�ҳ��              */
        if (tbl == 0) {                                                 /*  û�ҵ�                      */
            tbl = vmm_page_table_alloc(section_nr);                     /*  ����һ�����е�ҳ��          */
            if (tbl != NULL) {
                task->mmu_backup[section_nr % (PROCESS_SPACE_SIZE / SECTION_SIZE)] =
                        mmu_map_section_as_page(section_nr, tbl);       /*  ӳ��ö�                    */
                flag = TRUE;
            } else {
                printk("failed to alloc page table, mem map failed, va=0x%x, pid=%d\n", va, task->pid);
                return -1;
            }
        }

        frame = vmm_frame_alloc(task);                                  /*  ����һ�����е�ҳ��          */
        if (frame != NULL) {                                            /*  ���������ַ��ҳ�����ҳ��  */
            uint32_t page_nr = (va & (SECTION_SIZE - 1)) >> PAGE_OFFSET;
            mmu_map_page(tbl, page_nr, vmm_frame_addr(frame));          /*  ҳ��ӳ��                    */
            return 0;
        } else {
            /*
             * TODO: ���ڻ���֧�ֽ���
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
 * ��ʼ�����̵������ڴ���Ϣ
 */
int vmm_process_init(task_t *task, uint32_t file_size)
{
    int i;

    /*
     * Ϊ�������뵽���̵������ַ�ռ�, Ԥ��ӳ���ҳ��
     */
    for (i = 0; i < (file_size + PAGE_SIZE - 1) / PAGE_SIZE; i++) {
        if (vmm_page_map(task, task->pid * PROCESS_SPACE_SIZE + i * PAGE_SIZE) < 0) {
            vmm_process_cleanup(task);
            return -1;
        }
    }

    /*
     * Ϊ����ջ�ռ�ӳ��һ��ҳ��
     */
    if (vmm_page_map(task, (task->pid + 1) * PROCESS_SPACE_SIZE - PAGE_SIZE) < 0) {
        vmm_process_cleanup(task);
        return -1;
    }

    return 0;
}

/*
 * ������̵������ڴ���Ϣ
 */
int vmm_process_cleanup(task_t *task)
{
    int          i;
    vmm_frame_t *next;
    uint32_t     tbl;

    while (task->frame_list != NULL) {                                  /*  �ͷŽ���ռ�õ�ҳ��          */
        next = task->frame_list->process_next;
        vmm_frame_free(task->frame_list);
        task->frame_list = next;
    }
    task->frame_nr = 0;

    for (i = 0; i < PROCESS_SPACE_SIZE / SECTION_SIZE; i++) {           /*  �ͷŽ���ռ�õ�ҳ��          */
        tbl = vmm_page_table_lookup(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + i);
        if (tbl != 0) {
            vmm_page_table_free(tbl);                                   /*  �ͷ�ҳ��                    */
        }                                                               /*  ȡ��ӳ���                  */
        mmu_unmap_section(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + i);
        task->mmu_backup[i] = 0;
    }

    return 0;
}

/*
 * ��ʼ�������ڴ����
 */
void vmm_init(void)
{
    int                i;
    vmm_frame_t       *frame;
    struct page_table *tbl;

    /*
     * ��ʼ������ҳ������
     */
    for (i = 0, frame = vmm_frames; i < VMM_FRAME_NR - 1; i++, frame++) {
        frame->next = frame + 1;
    }
    frame->next = NULL;
    free_frame_list = vmm_frames;

    /*
     * ��ʼ������ҳ������
     */
    used_frame_list = NULL;

    /*
     * ��ʼ������ҳ������
     */
    for (i = 0, tbl = page_tables; i < PAGE_TBL_NR - 1; i++, tbl++) {
        tbl->node.rbe_right = tbl + 1;
    }
    tbl->node.rbe_right = NULL;
    free_page_table_list = page_tables;

    /*
     * ��ʼ������ҳ������
     */
    RB_INIT(&used_page_table_tree);

    /*
     * �������ҳ��
     */
    memset((void *)PAGE_TBL_BASE, 0, PAGE_TBL_SIZE * PAGE_TBL_NR);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
