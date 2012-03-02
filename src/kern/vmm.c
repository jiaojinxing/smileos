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
 * ҳ��
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
 * ���ݶκŲ���ҳ��
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
 * ����ҳ��
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
 * �ͷ�ҳ��
 */
void page_table_free(uint32_t base)
{
    page_table_t *tbl;

    tbl = page_tables + (base - PAGE_TBL_BASE) / PAGE_TBL_SIZE;

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
 * ҳ��
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
 * ����ҳ��
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
 * �ͷ�ҳ��
 */
void frame_free(frame_t *frame)
{
    frame->next = free_frame_list;
    free_frame_list = frame;
}

/*
 * ���ҳ��������ַ
 */
uint32_t get_frame_addr(frame_t *frame)
{
    return (frame - frames) * FRAME_SIZE + VMM_MEM_BASE;
}

/*
 * ӳ����̿ռ���������ַ, ҳ��ӳ��
 */
int process_space_page_map(task_t *task, uint32_t mva)
{
    frame_t *frame;
    int      flag = 0;

    if (   mva >= PROCESS_SPACE_SIZE *  task->pid                       /*  �ж������ַ�Ƿ��ڽ���      */
        && mva <  PROCESS_SPACE_SIZE * (task->pid + 1)) {               /*  ���Է��ʵĵ�ַ�ռ䷶Χ��    */

        uint32_t section_nr = mva >> SECTION_OFFSET;                    /*  ���������ַ�Ķκ�          */

        uint32_t tbl = page_table_lookup(section_nr);                   /*  ���Ҹöε�ҳ��              */
        if (!tbl) {                                                     /*  û�ҵ�                      */
            tbl = page_table_alloc(section_nr);                         /*  ����һ�����е�ҳ��          */
            if (tbl) {
                mmu_map_section_as_page(section_nr, tbl);               /*  ӳ��ö�                    */
                flag = 1;
            } else {
                printk("failed to alloc page table, map failed, mva=0x%x, pid=%d\n", mva, task->pid);
                return -1;
            }
        }

        frame = frame_alloc();                                          /*  ����һ�����е�ҳ��          */
        if (frame) {                                                    /*  ���������ַ��ҳ�����ҳ��  */
            uint32_t page_nr = (mva & (SECTION_SIZE - 1)) >> PAGE_OFFSET;

            mmu_map_page(tbl, page_nr, get_frame_addr(frame));          /*  ҳ��ӳ��                    */

            frame->next     = used_frame_list;
            used_frame_list = frame;

            frame->process_next = task->frame_list;
            task->frame_list = frame;

            return 0;
        } else {
            if (flag) {
                mmu_unmap_section(section_nr);
                page_table_free(tbl);
            }
            printk("failed to alloc frame, map failed, mva=0x%x, pid=%d\n", mva, task->pid);
            return -1;
        }
    } else {
        printk("invalid mva=0x%x, map failed, pid=%d\n", mva, task->pid);
        return -1;
    }
}

/*
 * ��ʼ�������ڴ����
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
