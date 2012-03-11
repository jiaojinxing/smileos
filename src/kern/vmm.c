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
    struct _page_table_t *prev;                                         /*  ǰ��                        */
    struct _page_table_t *next;                                         /*  ����                        */
    struct _page_table_t *process_next;                                 /*  ���̺���(TODO: Ŀǰδʹ��)  */
    uint32_t              section_nr;                                   /*  �κ�                        */
} page_table_t;

static page_table_t  page_tables[PAGE_TBL_NR];                          /*  ҳ��                        */
static page_table_t *free_page_table_list;                              /*  ����ҳ������                */
static page_table_t *used_page_table_list;                              /*  ����ҳ������                */

/*
 * ���ݶκŲ���ҳ��
 *
 * TODO: �����Ĳ����㷨����ͦ����, �д��Ľ�
 */
uint32_t vmm_page_table_lookup(uint32_t section_nr)
{
    page_table_t *tbl;

    tbl = used_page_table_list;                                         /*  ������ҳ�������в���        */
    while (tbl != NULL) {
        if (tbl->section_nr == section_nr) {                            /*  �κ�ƥ��                    */
            break;
        }
        tbl = tbl->next;
    }

    if (tbl != NULL) {
        return (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;     /*  ����ҳ���ַ                */
    } else {
        return 0;
    }
}

/*
 * ����ҳ��
 */
uint32_t vmm_page_table_alloc(uint32_t section_nr)
{
    page_table_t *tbl;

    tbl = free_page_table_list;                                         /*  �ڿ���ҳ��������ͷ������    */
    if (tbl != NULL) {
        free_page_table_list = tbl->next;

        tbl->prev = NULL;                                               /*  ���뵽����ҳ��������        */
        tbl->next = used_page_table_list;
        if (used_page_table_list != NULL) {
            used_page_table_list->prev = tbl;
        }
        used_page_table_list = tbl;

        tbl->section_nr = section_nr;                                   /*  ��¼�κ�                    */

        return (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;     /*  ����ҳ���ַ                */
    } else {
        return 0;
    }
}

/*
 * �ͷ�ҳ��
 */
void vmm_page_table_free(uint32_t page_tbl_base)
{
    page_table_t *tbl;

    tbl = page_tables + (page_tbl_base - PAGE_TBL_BASE) / PAGE_TBL_SIZE;/*  ����ҳ��                    */

    if (tbl->prev != NULL) {                                            /*  ������ҳ��������ɾ��        */
        tbl->prev->next = tbl->next;
    } else {
        used_page_table_list = tbl->next;
    }

    if (tbl->next != NULL) {
        tbl->next->prev = tbl->prev;
    }

    tbl->next = free_page_table_list;                                   /*  ���뵽����ҳ��������        */
    free_page_table_list = tbl;
}

/*
 * ҳ��
 */
struct _vmm_frame_t;
typedef struct _vmm_frame_t vmm_frame_t;
struct _vmm_frame_t {
    struct _vmm_frame_t *prev;                                          /*  ǰ��                        */
    struct _vmm_frame_t *next;                                          /*  ����                        */
    struct _vmm_frame_t *process_next;                                  /*  ���̺���                    */
};

static vmm_frame_t vmm_frames[VMM_FRAME_NR];                            /*  ҳ��                        */

static vmm_frame_t *free_frame_list;                                    /*  ����ҳ������                */
static vmm_frame_t *used_frame_list;                                    /*  ����ҳ������                */

/*
 * ����ҳ��
 */
vmm_frame_t *vmm_frame_alloc(task_t *task)
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
void vmm_frame_free(vmm_frame_t *frame)
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
uint32_t vmm_get_frame_addr(vmm_frame_t *frame)
{
    return (frame - vmm_frames) * VMM_FRAME_SIZE + VMM_MEM_BASE;
}

/*
 * ͨ��һ�������ַӳ����������ַ�ռ��е�һ��ҳ��
 */
int vmm_map_process_page(task_t *task, uint32_t va)
{
    vmm_frame_t *frame;
    int          flag = 0;

    if (   va >= PROCESS_SPACE_SIZE *  task->pid                        /*  �ж������ַ�Ƿ��ڽ���      */
        && va <  PROCESS_SPACE_SIZE * (task->pid + 1)) {                /*  �������ַ�ռ䷶Χ��        */

        uint32_t section_nr = va >> SECTION_OFFSET;                     /*  ���������ַ�Ķκ�          */

        uint32_t tbl = vmm_page_table_lookup(section_nr);               /*  ���Ҹöε�ҳ��              */
        if (tbl == 0) {                                                 /*  û�ҵ�                      */
            tbl = vmm_page_table_alloc(section_nr);                     /*  ����һ�����е�ҳ��          */
            if (tbl != NULL) {
                mmu_map_section_as_page(section_nr, tbl);               /*  ӳ��ö�                    */
                flag = 1;
            } else {
                printk("failed to alloc page table, map failed, va=0x%x, pid=%d\n", va, task->pid);
                return -1;
            }
        }

        frame = vmm_frame_alloc(task);                                  /*  ����һ�����е�ҳ��          */
        if (frame != NULL) {                                            /*  ���������ַ��ҳ�����ҳ��  */
            uint32_t page_nr = (va & (SECTION_SIZE - 1)) >> PAGE_OFFSET;
            mmu_map_page(tbl, page_nr, vmm_get_frame_addr(frame));      /*  ҳ��ӳ��                    */
            return 0;
        } else {
            if (flag) {
                mmu_unmap_section(section_nr);
                vmm_page_table_free(tbl);
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
 * �ͷŽ��̵������ַ�ռ�
 */
void vmm_free_process_space(task_t *task)
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
    }
}

/*
 * ��ʼ�������ڴ����
 */
void vmm_init(void)
{
    int           i;
    vmm_frame_t  *frame;
    page_table_t *tbl;

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
        tbl->next = tbl + 1;
    }
    tbl->next = NULL;
    free_page_table_list = page_tables;

    /*
     * ��ʼ������ҳ������
     */
    used_page_table_list = NULL;

    /*
     * �������ҳ��
     */
    memset((void *)PAGE_TBL_BASE, 0, PAGE_TBL_SIZE * PAGE_TBL_NR);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
