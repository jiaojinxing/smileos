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
** File name:               select.c
** Last modified Date:      2012-11-5
** Last Version:            1.0.0
** Descriptions:            select 函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-11-5
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
#include "kern/kern.h"
#include "kern/kvars.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include <fcntl.h>
/*********************************************************************************************************
** Function name:           select_init
** Descriptions:            初始化 select
** input parameters:        select              select 结构
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_init(select_struct_t *select)
{
    atomic_set(&select->flags, 0);

    INIT_LIST_HEAD(&select->wait_list);

    return 0;
}
/*********************************************************************************************************
** Function name:           select_select
** Descriptions:            将当前任务加入到文件的等待列表
** input parameters:        select              select 结构
**                          file                文件
**                          type                等待类型
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_select(select_struct_t *select, file_t *file, int type)
{
    select_node_t *node;
    reg_t reg;

    if (select == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = kmalloc(sizeof(select_node_t), GFP_KERNEL);                  /*  分配节点                    */
    if (node == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    reg = interrupt_disable();

    node->select_type = type;                                           /*  初始化节点                  */
    node->task        = current;

    list_add_tail(&node->node_list, &select->wait_list);                /*  加到等待列表                */

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           select_unselect
** Descriptions:            将当前任务从文件的等待列表中移除
** input parameters:        select              select 结构
**                          file                文件
**                          type                等待类型
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_unselect(select_struct_t *select, file_t *file, int type)
{
    struct list_head *item;
    select_node_t *node;
    reg_t reg;

    if (select == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    reg = interrupt_disable();

    list_for_each(item, &select->wait_list) {
        node = list_entry(item, select_node_t, node_list);
        if (node->task == current) {
            break;
        }
    }

    if (item == &select->wait_list) {
        interrupt_resume(reg);
        seterrno(EINVAL);
        return -1;
    }

    list_del(&node->node_list);                                         /*  从等待列表中移除节点        */

    interrupt_resume(reg);

    kfree(node);                                                        /*  释放节点                    */

    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_event_report
** Descriptions:            回报事件
** input parameters:        select              select 结构
**                          type                回报类型
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int vfs_event_report(select_struct_t *select, int type)
{
    struct list_head *item;
    select_node_t *node;
    task_t *task;
    int flag = FALSE;
    reg_t reg;

    reg = interrupt_disable();

    select->flags.counter |= type;                                      /*  记录事件                    */

    /*
     * 恢复等待列表里有类型交集的任务
     */
    list_for_each(item, &select->wait_list) {
        node = list_entry(item, select_node_t, node_list);

        task = node->task;

        if ((type & node->select_type) &&                               /*  回报类型与等待类型有交集    */
            task->resume_type != TASK_RESUME_INTERRUPT) {               /*  如果任务被中断, 则忽略      */

            task->delay        = 0;
            task->status       = TASK_RUNNING;                          /*  恢复任务                    */
            task->resume_type  = TASK_RESUME_SELECT_EVENT;

            if (!in_interrupt()) {                                      /*  如果需要重新任务调度        */
                flag = TRUE;
            }
        }
    }

    interrupt_resume(reg);

    if (flag) {
        schedule();                                                     /*  任务调度                    */
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           vfs_block_helper
** Descriptions:            阻塞辅助函数
** input parameters:        select              select 结构
**                          scan                扫描函数
**                          ctx                 上下文
**                          file                文件结构
**                          type                回报类型
** output parameters:       NONE
** Returned value:          0 OR -1 OR 1
*********************************************************************************************************/
int vfs_block_helper(select_struct_t *select,
                     int (*scan)(void *, file_t *, int),
                     void *ctx,
                     file_t *file,
                     int type)
{
    reg_t reg;
    int ret;
    int resume_type;

    if (file->flags & O_NONBLOCK) {                                     /*  上层不想阻塞                */
        seterrno(EAGAIN);
        return 0;
    } else {
        reg = interrupt_disable();

        ret = scan(ctx, file, type);                                    /*  扫描一下                    */
        if (ret & type) {
            interrupt_resume(reg);
            return 1;
        }

        ret = select_select(select, file, type);                        /*  将当前任务加入到等待列表    */
        if (ret < 0) {
            interrupt_resume(reg);
            return -1;
        }

        current->delay   = 0;
        current->status  = TASK_SUSPEND;                                /*  挂起当前任务              　*/

        current->resume_type = TASK_RESUME_UNKNOW;

        schedule();                                                     /*  任务调度                    */

        select_unselect(select, file, type);                            /*  将当前任务从等待列表中移除  */

        current->delay       = 0;
        resume_type          = current->resume_type;                    /*  获得恢复类型                */
        current->resume_type = TASK_RESUME_UNKNOW;

        if (resume_type & TASK_RESUME_SELECT_EVENT) {                   /*  被 select 事件恢复      　  */

            ret = scan(ctx, file, type);                                /*  扫描一下                    */

            interrupt_resume(reg);

            if (ret & type) {
                return 1;
            } else {
                seterrno(EAGAIN);
                return 0;
            }
        } else {
            interrupt_resume(reg);
            seterrno(EINTR);                                            /*  select 被中断了　　         */
            return -1;
        }
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
