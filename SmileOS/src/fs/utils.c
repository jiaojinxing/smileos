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
** File name:               utils.c
** Last modified Date:      2012-11-5
** Last Version:            1.0.0
** Descriptions:            工具函数
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
    uint32_t reg;

    if (select == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = kmalloc(sizeof(select_node_t));
    if (node == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    reg = interrupt_disable();

    node->select_type = type;
    node->task        = current;

    select->wait_list.next->prev = node;
    node->next = select->wait_list.next;
    select->wait_list.next = node;
    node->prev = &select->wait_list;

    file->select_node = node;

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
    select_node_t *node;
    uint32_t reg;

    if (select == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    reg = interrupt_disable();

    node = file->select_node;
    if (node == NULL) {
        interrupt_resume(reg);
        seterrno(EINVAL);
        return -1;
    }

    node->next->prev = node->prev;
    node->prev->next = node->next;

    file->select_node = NULL;

    interrupt_resume(reg);

    kfree(node);

    return 0;
}
/*********************************************************************************************************
** Function name:           select_report
** Descriptions:            回报事件
** input parameters:        select              select 结构
**                          type                回报类型
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_report(select_struct_t *select, int type)
{
    select_node_t *node;
    task_t *task;
    int flag = FALSE;
    uint32_t reg;

    reg = interrupt_disable();

    select->flags.counter |= type;

    node = select->wait_list.next;
    while (node != &select->wait_list) {
        task = node->task;
        if ((type & node->select_type || type & VFS_FILE_ERROR) &&
            task->resume_type != TASK_RESUME_INTERRUPT) {
            task->delay        = 0;
            task->status       = TASK_RUNNING;
            task->resume_type  = TASK_RESUME_SELECT_EVENT;
            if (!in_interrupt() &&
                task->type == TASK_TYPE_KTHREAD &&
                task->priority >= current->priority) {
                flag = TRUE;
            }
        }
        node = node->next;
    }

    interrupt_resume(reg);

    if (flag) {
        schedule();
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           select_helper
** Descriptions:            select 辅助函数
** input parameters:        select              select 结构
**                          scan                扫描函数
**                          ctx                 上下文
**                          file                文件结构
**                          type                回报类型
** output parameters:       NONE
** Returned value:          0 OR -1 OR 1
*********************************************************************************************************/
int select_helper(select_struct_t *select,
                  int (*scan)(void *, file_t *, int),
                  void *ctx, file_t *file, int type)
{
    uint32_t reg;
    int ret;
    int resume_type;

    if (file->flags & O_NONBLOCK) {
        seterrno(EAGAIN);
        return 0;
    } else {
        reg = interrupt_disable();

        ret = select_select(select, file, type);
        if (ret < 0) {
            interrupt_resume(reg);
            seterrno(ENOMEM);
            return -1;
        }

        current->delay   = 0;
        current->status  = TASK_SUSPEND;

        current->resume_type = TASK_RESUME_UNKNOW;

        schedule();

        select_unselect(select, file, type);

        current->delay       = 0;
        resume_type          = current->resume_type;
        current->resume_type = TASK_RESUME_UNKNOW;

        if (resume_type & TASK_RESUME_SELECT_EVENT) {

            ret = scan(ctx, file, type);

            interrupt_resume(reg);

            if (ret & type) {
                return 1;
            } else {
                seterrno(EAGAIN);
                return 0;
            }
        } else {
            interrupt_resume(reg);
            seterrno(EINTR);
            return -1;
        }
    }
}
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
    select->wait_list.next = select->wait_list.prev = &select->wait_list;

    return 0;
}
/*********************************************************************************************************
** Function name:           BKDRHash
** Descriptions:            BKDR Hash Function
**                          各种字符串 Hash 函数比较
**                          http://www.byvoid.com/blog/string-hash-compare/
** input parameters:        str                 字符串
** output parameters:       NONE
** Returned value:          BKDR Hash
*********************************************************************************************************/
unsigned int BKDRHash(const char *str)
{
    unsigned int seed = 131;                                            /*  31 131 1313 13131 131313 etc*/
    unsigned int hash = 0;
    char ch;

    while ((ch = *str++) != 0) {
        hash = hash * seed + ch;
    }

    return (hash & 0x7FFFFFFF);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
