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
** Descriptions:            �ܽ�һ�����漸��������д��ʹ��:
**
**  id      ID ��           �����̵߳� ID ��, һ�㱻�������, ������һ���������, ������ 0-2^32 ��Χ
**  index   ����            ��ʾԪ�������顢��������Ա��е�λ��
**  nr��������Ŀ            �ȷ�˵������ƻ���ж��ٸ�, ���Է����仯, ��ʼ��һ�㲻Ϊ 0
**  no����������            ������ id �� index
**  cnt     ����            ����ͳ�Ƹ��������, һ�㷢���仯, �仯һ�����������Լ�, ��ʼ��һ��Ϊ 0
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/mmu.h"
#include <sys/tree.h>
#include <string.h>
/*********************************************************************************************************
** ����
*********************************************************************************************************/
/*
 * �����ڴ����ͳ����Ϣ
 */
typedef struct {
    uint32_t                free_page_table_nr;                         /*  ����ҳ����                  */
    uint32_t                free_frame_nr;                              /*  ����ҳ����                  */
} vmm_stat_t;

/*
 * ҳ��
 */
struct page_table {
    RB_ENTRY(page_table)    node;                                       /*  ������ڵ�                  */
    uint32_t                section_no;                                 /*  �κ�                        */
};
/*********************************************************************************************************
** �ڲ�����
*********************************************************************************************************/
static vmm_stat_t           vmm_stat;                                   /*  �����ڴ����ͳ����Ϣ        */
static struct page_table    page_tables[PAGE_TBL_NR];                   /*  ҳ��                        */
static struct page_table   *free_page_table_list;                       /*  ����ҳ������                */
static RB_HEAD(page_table_tree, page_table) used_page_table_tree;       /*  ����ҳ������              */
/*********************************************************************************************************
** Function name:           vmm_page_table_compare
** Descriptions:            ҳ��ȽϺ���
** input parameters:        a                   ҳ�� A
**                          b                   ҳ�� B
** output parameters:       NONE
** Returned value:          -1 OR 1 OR 0
*********************************************************************************************************/
static inline int vmm_page_table_compare(struct page_table *a, struct page_table *b)
{
#if 0
    if (a->section_no < b->section_no) {
        return -1;
    } else if (a->section_no > b->section_no) {
        return 1;
    } else {
        return 0;
    }
#else
    return a->section_no - b->section_no;
#endif
}
/*********************************************************************************************************
** ����ҳ����������
*********************************************************************************************************/
RB_GENERATE_INTERNAL(page_table_tree, page_table, node, vmm_page_table_compare, static);
/*********************************************************************************************************
** Function name:           vmm_page_table_lookup
** Descriptions:            ���ݶκŲ���ҳ��
** input parameters:        section_no          �κ�
** output parameters:       NONE
** Returned value:          ҳ���ַ OR 0
*********************************************************************************************************/
static uint32_t vmm_page_table_lookup(uint32_t section_no)
{
    struct page_table *tbl;
    struct page_table  tmp;

    tmp.section_no = section_no;

    tbl = page_table_tree_RB_FIND(&used_page_table_tree, &tmp);         /*  ������ҳ�������в���      */
    if (tbl != NULL) {
        return (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;     /*  ����ҳ���ַ                */
    } else {
        return 0;
    }
}
/*********************************************************************************************************
** Function name:           vmm_page_table_alloc
** Descriptions:            ����ҳ��
** input parameters:        task                ������ƿ�
**                          section_no          �κ�
** output parameters:       NONE
** Returned value:          ҳ���ַ OR 0
*********************************************************************************************************/
static uint32_t vmm_page_table_alloc(task_t *task, uint32_t section_no)
{
    struct page_table *tbl;
    uint32_t           page_tbl_base;

    tbl = free_page_table_list;                                         /*  �ڿ���ҳ��������ͷ������    */
    if (tbl != NULL) {
        free_page_table_list = tbl->node.rbe_right;

        tbl->section_no = section_no;
        page_table_tree_RB_INSERT(&used_page_table_tree, tbl);          /*  ���뵽����ҳ��������      */

        vmm_stat.free_page_table_nr--;                                  /*  ����ҳ����--                */

        task->page_tbl_nr++;                                            /*  ����ռ��ҳ����++            */

                                                                        /*  ����ҳ���ַ                */
        page_tbl_base = (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;

        memset((void *)page_tbl_base, 0, PAGE_TBL_SIZE);                /*  ��ҳ������                  */

        return page_tbl_base;                                           /*  ����ҳ���ַ                */
    } else {
        return 0;
    }
}
/*********************************************************************************************************
** Function name:           vmm_page_table_free
** Descriptions:            �ͷ�ҳ��
** input parameters:        task                ������ƿ�
**                          page_tbl_base       ҳ���ַ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void vmm_page_table_free(task_t *task, uint32_t page_tbl_base)
{
    struct page_table *tbl;

    memset((void *)page_tbl_base, 0, PAGE_TBL_SIZE);                    /*  ��ҳ������                  */

    tbl = page_tables + (page_tbl_base - PAGE_TBL_BASE) / PAGE_TBL_SIZE;/*  ����ҳ��                    */

    page_table_tree_RB_REMOVE(&used_page_table_tree, tbl);              /*  ������ҳ���������Ƴ�      */

    tbl->node.rbe_right  = free_page_table_list;                        /*  ���뵽����ҳ��������        */
    free_page_table_list = tbl;

    vmm_stat.free_page_table_nr++;                                      /*  ����ҳ����++                */

    task->page_tbl_nr--;                                                /*  ����ռ��ҳ����--            */
}
/*********************************************************************************************************
** ����
*********************************************************************************************************/
/*
 * ҳ��
 */
struct vmm_frame;
typedef struct vmm_frame vmm_frame_t;
struct vmm_frame {
    uint32_t                MVA;                                        /*  ҳ��ӳ�䵽��ҳ��������ַ  */
    struct vmm_frame       *prev;                                       /*  ǰ��                        */
    struct vmm_frame       *next;                                       /*  ����                        */
    struct vmm_frame       *process_next;                               /*  ���̺���                    */
};
/*********************************************************************************************************
** �ڲ�����
*********************************************************************************************************/
static vmm_frame_t          frames[VMM_FRAME_NR];                       /*  ҳ��                        */
static vmm_frame_t         *free_frame_list;                            /*  ����ҳ������                */
static vmm_frame_t         *used_frame_list;                            /*  ����ҳ������                */
/*********************************************************************************************************
** Function name:           vmm_frame_alloc
** Descriptions:            ����ҳ��
** input parameters:        task                ������ƿ�
** output parameters:       NONE
** Returned value:          ҳ�� OR NULL
*********************************************************************************************************/
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

        vmm_stat.free_frame_nr--;                                       /*  ����ҳ����--                */

        frame->process_next = task->frame_list;                         /*  ���뵽����ҳ��������        */
        task->frame_list    = frame;

        task->frame_nr++;                                               /*  ����ռ��ҳ����++            */
    }
    /*
     * TODO:
     * Ŀǰ���������ҳ��û�б�����, ������Ǹ���ȫ����,
     * �ȷ�˵, �ڶ��û��Ĳ���ϵͳ��, һ���û����ڷ����ʼ�, �ʼ��Ĳ������ݿ��ܻᱻ�ڱ�ĵط���¼��
     * ��ϵͳ�������û����⿴��. ���� SmileOS ��, ����ܲ���ʲô����:-)
     */
    return frame;
}
/*********************************************************************************************************
** Function name:           vmm_frame_free
** Descriptions:            �ͷ�ҳ��
** input parameters:        task                ������ƿ�
**                          frame               ҳ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int vmm_frame_free(task_t *task, vmm_frame_t *frame)
{
    vmm_frame_t *temp, *priv;

    temp = task->frame_list;                                            /*  �ڽ���ҳ�������в��Ҹ�ҳ��  */
    priv = NULL;
    while (temp != NULL && temp != frame) {
        priv = temp;
        temp = temp->process_next;
    }

    if (temp == NULL) {                                                 /*  û�ҵ�                      */
        return -1;
    }
                                                                        /*  �ӽ���ҳ��������ɾ��        */
    if (priv != NULL) {                                                 /*  ������ͷ                    */
        priv->process_next = temp->process_next;
    } else {                                                            /*  ����ͷ                      */
        task->frame_list   = temp->process_next;
    }

    task->frame_nr--;                                                   /*  ����ռ��ҳ����--            */

    if (frame->prev != NULL) {                                          /*  ������ҳ��������ɾ��        */
        frame->prev->next = frame->next;
    } else {
        used_frame_list   = frame->next;
    }

    if (frame->next != NULL) {
        frame->next->prev = frame->prev;
    }

    frame->next     = free_frame_list;                                  /*  ���뵽����ҳ��������        */
    free_frame_list = frame;

    vmm_stat.free_frame_nr++;                                           /*  ����ҳ����++                */

    return 0;
}
/*********************************************************************************************************
** Function name:           vmm_frame_addr
** Descriptions:            ���ҳ��������ַ
** input parameters:        frame               ҳ��
** output parameters:       NONE
** Returned value:          ҳ��������ַ
*********************************************************************************************************/
static inline uint32_t vmm_frame_addr(vmm_frame_t *frame)
{
    return (frame - frames) * VMM_FRAME_SIZE + VMM_MEM_BASE;
}
/*********************************************************************************************************
** Function name:           vmm_page_map
** Descriptions:            ͨ��һ�������ַӳ����������ַ�ռ��е�һ��ҳ��
** input parameters:        task                ������ƿ�
**                          MVA                 �޸ĵ������ַ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vmm_page_map(task_t *task, uint32_t MVA)
{
    vmm_frame_t *frame;
    int          flag = FALSE;

    if (   MVA >= PROCESS_SPACE_SIZE *  task->pid                       /*  �ж������ַ�Ƿ��ڽ���      */
        && MVA <  PROCESS_SPACE_SIZE * (task->pid + 1)) {               /*  �������ַ�ռ䷶Χ��        */

        uint32_t section_no = MVA >> SECTION_OFFSET;                    /*  ���������ַ�Ķκ�          */

        uint32_t tbl = vmm_page_table_lookup(section_no);               /*  ���Ҹöε�ҳ��              */

        if (tbl == 0) {                                                 /*  û�ҵ�                      */

            tbl = vmm_page_table_alloc(task, section_no);               /*  ����һ�����е�ҳ��          */
            if (tbl != 0) {
                task->mmu_backup[section_no % (PROCESS_SPACE_SIZE / SECTION_SIZE)] =
                        mmu_map_section_as_page(section_no, tbl);       /*  ӳ��ö�                    */

                flag = TRUE;
            } else {
                printk("failed to alloc page table, mem map failed, MVA=0x%x, PID=%d\n", MVA, task->pid);
                return -1;
            }
        }

        frame = vmm_frame_alloc(task);                                  /*  ����һ�����е�ҳ��          */
        if (frame != NULL) {
            uint32_t page_no;

            page_no    = (MVA & (SECTION_SIZE - 1)) >> PAGE_OFFSET;     /*  ���������ַ��ҳ�����ҳ��  */

            frame->MVA = MVA & (~(PAGE_SIZE - 1));                      /*  ҳ��ӳ�䵽��ҳ��������ַ  */

            mmu_map_page(tbl, page_no, vmm_frame_addr(frame));          /*  ҳ��ӳ��                    */

            return 0;
        } else {
            /*
             * TODO: ���ڻ���֧�� SWAP, Ҳû�˼ƻ�, �Ͼ�Ƕ��ʽϵͳ���ٴ�Ӳ��, FLASH оƬ�в�д��������,
             * ������������Ȥ�����ѳ�����:-)
             */
            if (flag) {
                mmu_unmap_section(section_no);

                task->mmu_backup[section_no % (PROCESS_SPACE_SIZE / SECTION_SIZE)] = 0;

                vmm_page_table_free(task, tbl);
            }
            printk("failed to alloc page, mem map failed, MVA=0x%x, PID=%d\n", MVA, task->pid);
            return -1;
        }
    } else {
        printk("invalid MVA=0x%x, mem map failed, PID=%d\n", MVA, task->pid);
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           vmm_process_cleanup
** Descriptions:            ������̵������ڴ�ռ�
** input parameters:        task                ������ƿ�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vmm_process_cleanup(task_t *task)
{
    int          i;
    vmm_frame_t *frame;
    uint32_t     tbl;

    while ((frame = task->frame_list) != NULL) {                        /*  �ͷŽ���ռ�õ�ҳ��          */
        vmm_frame_free(task, frame);                                    /*  �ͷ�ҳ��                    */
    }

    for (i = 0; i < PROCESS_SPACE_SIZE / SECTION_SIZE; i++) {           /*  �ͷŽ���ռ�õ�ҳ��          */

        tbl = vmm_page_table_lookup(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + i);
        if (tbl != 0) {
            vmm_page_table_free(task, tbl);                             /*  �ͷŸöε�ҳ��              */
        }
                                                                        /*  ȡ��ӳ��ö�                */
        mmu_unmap_section(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + i);
        task->mmu_backup[i] = 0;
    }

    /*
     * һ���α�Ͷ���ҳ���� UnCache ��, ���ԾͲ���Ϊ Cache ������:-)
     */

    mmu_invalidate_itlb_dtlb();                                         /*  ��Чת����·����            */

    return 0;
}
/*********************************************************************************************************
** Function name:           vmm_process_init
** Descriptions:            ��ʼ�����̵������ڴ�ռ�
** input parameters:        task                ������ƿ�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vmm_process_init(task_t *task)
{
    int i;

    /*
     * Ϊ�������뵽���̵������ַ�ռ�, Ԥ��ӳ���ҳ��
     */
    for (i = 0; i < (task->file_size + PAGE_SIZE - 1) / PAGE_SIZE; i++) {
        if (vmm_page_map(task, task->pid * PROCESS_SPACE_SIZE + i * PAGE_SIZE) < 0) {
            vmm_process_cleanup(task);
            return -1;
        }
    }

    /*
     * Ϊ�������������̵Ĳ����ռ�, Ԥ��ӳ���ҳ��
     */
    for (i = 0; i < PROCESS_PARAM_SIZE / PAGE_SIZE; i++) {
        if (vmm_page_map(task, (task->pid + 1) * PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE + i * PAGE_SIZE) < 0) {
            vmm_process_cleanup(task);
            return -1;
        }
    }

    /*
     * Ϊ����ջ�ռ�ӳ��һ��ҳ��
     */
    if (vmm_page_map(task, (task->pid + 1) * PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE - PAGE_SIZE) < 0) {
        vmm_process_cleanup(task);
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           vmm_init
** Descriptions:            ��ʼ�������ڴ����
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void vmm_init(void)
{
    int                i;
    vmm_frame_t       *frame;
    struct page_table *tbl;

    /*
     * ��ʼ������ҳ������
     */
    for (i = 0, frame = frames; i < VMM_FRAME_NR - 1; i++, frame++) {
        frame->next = frame + 1;
    }
    frame->next     = NULL;
    free_frame_list = frames;

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
    tbl->node.rbe_right  = NULL;
    free_page_table_list = page_tables;

    /*
     * ��ʼ������ҳ������
     */
    RB_INIT(&used_page_table_tree);

    vmm_stat.free_frame_nr      = VMM_FRAME_NR;                         /*  ����ҳ����                  */
    vmm_stat.free_page_table_nr = PAGE_TBL_NR;                          /*  ����ҳ����                  */
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
