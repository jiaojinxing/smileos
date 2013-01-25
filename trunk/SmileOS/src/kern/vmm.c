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
** Descriptions:            总结一下下面几个单词缩写的使用:
**
**  id      ID 号           例如线程的 ID 号, 一般被分配出来, 分配有一定的随机性, 可能在 0-2^32 范围
**  index   索引            表示元素在数组、链表等线性表中的位置
**  nr　　　数目            比方说现在有苹果有多少个, 可以发生变化, 初始化一般不为 0
**  no　　　号码            类似于 id 和 index
**  cnt     计数            用于统计次数, 一般发生变化, 变化一般是自增和自减, 初始化一般为 0
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/mmu.h"
#include <sys/tree.h>
#include <string.h>
/*********************************************************************************************************
** 定义
*********************************************************************************************************/
/*
 * 虚拟内存管理统计信息
 */
typedef struct {
    uint32_t                free_page_table_nr;                         /*  空闲页表数                  */
    uint32_t                free_frame_nr;                              /*  空闲页框数                  */
} vmm_stat_t;

/*
 * 页表
 */
struct page_table {
    RB_ENTRY(page_table)    node;                                       /*  红黑树节点                  */
    uint32_t                section_no;                                 /*  段号                        */
};
/*********************************************************************************************************
** 内部变量
*********************************************************************************************************/
static vmm_stat_t           vmm_stat;                                   /*  虚拟内存管理统计信息        */
static struct page_table    page_tables[PAGE_TBL_NR];                   /*  页表                        */
static struct page_table   *free_page_table_list;                       /*  空闲页表链表                */
static RB_HEAD(page_table_tree, page_table) used_page_table_tree;       /*  已用页表红黑树              */
/*********************************************************************************************************
** Function name:           vmm_page_table_compare
** Descriptions:            页表比较函数
** input parameters:        a                   页表 A
**                          b                   页表 B
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
** 产生页表红黑树代码
*********************************************************************************************************/
RB_GENERATE_INTERNAL(page_table_tree, page_table, node, vmm_page_table_compare, static);
/*********************************************************************************************************
** Function name:           vmm_page_table_lookup
** Descriptions:            根据段号查找页表
** input parameters:        section_no          段号
** output parameters:       NONE
** Returned value:          页表基址 OR 0
*********************************************************************************************************/
static uint32_t vmm_page_table_lookup(uint32_t section_no)
{
    struct page_table *tbl;
    struct page_table  tmp;

    tmp.section_no = section_no;

    tbl = page_table_tree_RB_FIND(&used_page_table_tree, &tmp);         /*  在已用页表红黑树中查找      */
    if (tbl != NULL) {
        return (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;     /*  返回页表基址                */
    } else {
        return 0;
    }
}
/*********************************************************************************************************
** Function name:           vmm_page_table_alloc
** Descriptions:            分配页表
** input parameters:        task                任务控制块
**                          section_no          段号
** output parameters:       NONE
** Returned value:          页表基址 OR 0
*********************************************************************************************************/
static uint32_t vmm_page_table_alloc(task_t *task, uint32_t section_no)
{
    struct page_table *tbl;
    uint32_t           page_tbl_base;

    tbl = free_page_table_list;                                         /*  在空闲页表链表链头处分配    */
    if (tbl != NULL) {
        free_page_table_list = tbl->node.rbe_right;

        tbl->section_no = section_no;
        page_table_tree_RB_INSERT(&used_page_table_tree, tbl);          /*  加入到已用页表红黑树中      */

        vmm_stat.free_page_table_nr--;                                  /*  空闲页表数--                */

        task->page_tbl_nr++;                                            /*  进程占用页表数++            */

                                                                        /*  计算页表基址                */
        page_tbl_base = (tbl - page_tables) * PAGE_TBL_SIZE + PAGE_TBL_BASE;

        memset((void *)page_tbl_base, 0, PAGE_TBL_SIZE);                /*  将页表清零                  */

        return page_tbl_base;                                           /*  返回页表基址                */
    } else {
        return 0;
    }
}
/*********************************************************************************************************
** Function name:           vmm_page_table_free
** Descriptions:            释放页表
** input parameters:        task                任务控制块
**                          page_tbl_base       页表基址
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void vmm_page_table_free(task_t *task, uint32_t page_tbl_base)
{
    struct page_table *tbl;

    memset((void *)page_tbl_base, 0, PAGE_TBL_SIZE);                    /*  将页表清零                  */

    tbl = page_tables + (page_tbl_base - PAGE_TBL_BASE) / PAGE_TBL_SIZE;/*  计算页表                    */

    page_table_tree_RB_REMOVE(&used_page_table_tree, tbl);              /*  从已用页表红黑树中移除      */

    tbl->node.rbe_right  = free_page_table_list;                        /*  加入到空闲页表链表中        */
    free_page_table_list = tbl;

    vmm_stat.free_page_table_nr++;                                      /*  空闲页表数++                */

    task->page_tbl_nr--;                                                /*  进程占用页表数--            */
}
/*********************************************************************************************************
** 定义
*********************************************************************************************************/
/*
 * 页框
 */
struct vmm_frame;
typedef struct vmm_frame vmm_frame_t;
struct vmm_frame {
    uint32_t                MVA;                                        /*  页框映射到的页面的虚拟基址  */
    struct vmm_frame       *prev;                                       /*  前趋                        */
    struct vmm_frame       *next;                                       /*  后趋                        */
    struct vmm_frame       *process_next;                               /*  进程后趋                    */
};
/*********************************************************************************************************
** 内部变量
*********************************************************************************************************/
static vmm_frame_t          frames[VMM_FRAME_NR];                       /*  页框                        */
static vmm_frame_t         *free_frame_list;                            /*  空闲页框链表                */
static vmm_frame_t         *used_frame_list;                            /*  已用页框链表                */
/*********************************************************************************************************
** Function name:           vmm_frame_alloc
** Descriptions:            分配页框
** input parameters:        task                任务控制块
** output parameters:       NONE
** Returned value:          页框 OR NULL
*********************************************************************************************************/
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

        vmm_stat.free_frame_nr--;                                       /*  空闲页框数--                */

        frame->process_next = task->frame_list;                         /*  加入到进程页框链表中        */
        task->frame_list    = frame;

        task->frame_nr++;                                               /*  进程占用页框数++            */
    }
    /*
     * TODO:
     * 目前分配出来的页框没有被清零, 这可能是个安全问题,
     * 比方说, 在多用户的操作系统下, 一个用户正在发送邮件, 邮件的部分内容可能会被在别的地方登录到
     * 该系统的其它用户意外看到. 但在 SmileOS 下, 这可能不是什么问题:-)
     */
    return frame;
}
/*********************************************************************************************************
** Function name:           vmm_frame_free
** Descriptions:            释放页框
** input parameters:        task                任务控制块
**                          frame               页框
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int vmm_frame_free(task_t *task, vmm_frame_t *frame)
{
    vmm_frame_t *temp, *priv;

    temp = task->frame_list;                                            /*  在进程页框链表中查找该页框  */
    priv = NULL;
    while (temp != NULL && temp != frame) {
        priv = temp;
        temp = temp->process_next;
    }

    if (temp == NULL) {                                                 /*  没找到                      */
        return -1;
    }
                                                                        /*  从进程页框链表中删除        */
    if (priv != NULL) {                                                 /*  不在链头                    */
        priv->process_next = temp->process_next;
    } else {                                                            /*  在链头                      */
        task->frame_list   = temp->process_next;
    }

    task->frame_nr--;                                                   /*  进程占用页框数--            */

    if (frame->prev != NULL) {                                          /*  从已用页框链表中删除        */
        frame->prev->next = frame->next;
    } else {
        used_frame_list   = frame->next;
    }

    if (frame->next != NULL) {
        frame->next->prev = frame->prev;
    }

    frame->next     = free_frame_list;                                  /*  加入到空闲页框链表中        */
    free_frame_list = frame;

    vmm_stat.free_frame_nr++;                                           /*  空闲页框数++                */

    return 0;
}
/*********************************************************************************************************
** Function name:           vmm_frame_addr
** Descriptions:            获得页框的物理地址
** input parameters:        frame               页框
** output parameters:       NONE
** Returned value:          页框的物理地址
*********************************************************************************************************/
static inline uint32_t vmm_frame_addr(vmm_frame_t *frame)
{
    return (frame - frames) * VMM_FRAME_SIZE + VMM_MEM_BASE;
}
/*********************************************************************************************************
** Function name:           vmm_page_map
** Descriptions:            通过一个虚拟地址映射进程虚拟地址空间中的一个页面
** input parameters:        task                任务控制块
**                          MVA                 修改的虚拟地址
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vmm_page_map(task_t *task, uint32_t MVA)
{
    vmm_frame_t *frame;
    int          flag = FALSE;

    if (   MVA >= PROCESS_SPACE_SIZE *  task->pid                       /*  判断虚拟地址是否在进程      */
        && MVA <  PROCESS_SPACE_SIZE * (task->pid + 1)) {               /*  的虚拟地址空间范围内        */

        uint32_t section_no = MVA >> SECTION_OFFSET;                    /*  计算虚拟地址的段号          */

        uint32_t tbl = vmm_page_table_lookup(section_no);               /*  查找该段的页表              */

        if (tbl == 0) {                                                 /*  没找到                      */

            tbl = vmm_page_table_alloc(task, section_no);               /*  分配一个空闲的页表          */
            if (tbl != 0) {
                task->mmu_backup[section_no % (PROCESS_SPACE_SIZE / SECTION_SIZE)] =
                        mmu_map_section_as_page(section_no, tbl);       /*  映射该段                    */

                flag = TRUE;
            } else {
                printk("failed to alloc page table, mem map failed, MVA=0x%x, PID=%d\n", MVA, task->pid);
                return -1;
            }
        }

        frame = vmm_frame_alloc(task);                                  /*  分配一个空闲的页框          */
        if (frame != NULL) {
            uint32_t page_no;

            page_no    = (MVA & (SECTION_SIZE - 1)) >> PAGE_OFFSET;     /*  计算虚拟地址在页表里的页号  */

            frame->MVA = MVA & (~(PAGE_SIZE - 1));                      /*  页框映射到的页面的虚拟基址  */

            mmu_map_page(tbl, page_no, vmm_frame_addr(frame));          /*  页面映射                    */

            return 0;
        } else {
            /*
             * TODO: 现在还不支持 SWAP, 也没此计划, 毕竟嵌入式系统很少带硬盘, FLASH 芯片有擦写寿命问题,
             * 但不反对有兴趣的朋友尝试下:-)
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
** Descriptions:            清理进程的虚拟内存空间
** input parameters:        task                任务控制块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vmm_process_cleanup(task_t *task)
{
    int          i;
    vmm_frame_t *frame;
    uint32_t     tbl;

    while ((frame = task->frame_list) != NULL) {                        /*  释放进程占用的页框          */
        vmm_frame_free(task, frame);                                    /*  释放页框                    */
    }

    for (i = 0; i < PROCESS_SPACE_SIZE / SECTION_SIZE; i++) {           /*  释放进程占用的页表          */

        tbl = vmm_page_table_lookup(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + i);
        if (tbl != 0) {
            vmm_page_table_free(task, tbl);                             /*  释放该段的页表              */
        }
                                                                        /*  取消映射该段                */
        mmu_unmap_section(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + i);
        task->mmu_backup[i] = 0;
    }

    /*
     * 一级段表和二级页表都是 UnCache 的, 所以就不用为 Cache 担心了:-)
     */

    mmu_invalidate_itlb_dtlb();                                         /*  无效转换旁路缓冲            */

    return 0;
}
/*********************************************************************************************************
** Function name:           vmm_process_init
** Descriptions:            初始化进程的虚拟内存空间
** input parameters:        task                任务控制块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vmm_process_init(task_t *task)
{
    int i;

    /*
     * 为拷贝代码到进程的虚拟地址空间, 预先映射好页面
     */
    for (i = 0; i < (task->file_size + PAGE_SIZE - 1) / PAGE_SIZE; i++) {
        if (vmm_page_map(task, task->pid * PROCESS_SPACE_SIZE + i * PAGE_SIZE) < 0) {
            vmm_process_cleanup(task);
            return -1;
        }
    }

    /*
     * 为拷贝参数到进程的参数空间, 预先映射好页面
     */
    for (i = 0; i < PROCESS_PARAM_SIZE / PAGE_SIZE; i++) {
        if (vmm_page_map(task, (task->pid + 1) * PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE + i * PAGE_SIZE) < 0) {
            vmm_process_cleanup(task);
            return -1;
        }
    }

    /*
     * 为进程栈空间映射一个页面
     */
    if (vmm_page_map(task, (task->pid + 1) * PROCESS_SPACE_SIZE - PROCESS_PARAM_SIZE - PAGE_SIZE) < 0) {
        vmm_process_cleanup(task);
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           vmm_init
** Descriptions:            初始化虚拟内存管理
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
     * 初始化空闲页框链表
     */
    for (i = 0, frame = frames; i < VMM_FRAME_NR - 1; i++, frame++) {
        frame->next = frame + 1;
    }
    frame->next     = NULL;
    free_frame_list = frames;

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
    tbl->node.rbe_right  = NULL;
    free_page_table_list = page_tables;

    /*
     * 初始化已用页表红黑树
     */
    RB_INIT(&used_page_table_tree);

    vmm_stat.free_frame_nr      = VMM_FRAME_NR;                         /*  空闲页框数                  */
    vmm_stat.free_page_table_nr = PAGE_TBL_NR;                          /*  空闲页框数                  */
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
