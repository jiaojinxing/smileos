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
** File name:               selectdrv.h
** Last modified Date:      2012-4-6
** Last Version:            1.0.0
** Descriptions:            select 驱动头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-6
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
#ifndef SELECTDRV_H_
#define SELECTDRV_H_

/*
 * 将当前任务加入到文件的等待列表
 */
static int select_select(void *ctx, file_t *file, int type)
{
    privinfo_t *priv = ctx;
    select_node_t *node;

    if (priv == NULL) {
        return -1;
    }
    node = kmalloc(sizeof(sizeof(select_node_t)));
    if (node == NULL) {
        return -1;
    }

    node->select_type = type;
    node->task        = current;

    priv->wait_list.next->prev = node;
    node->next = priv->wait_list.next;
    priv->wait_list.next = node;
    node->prev = &priv->wait_list;

    file->ctx1 = node;

    return 0;
}

/*
 * 将当前任务从文件的等待列表中移除
 */
static int select_unselect(void *ctx, file_t *file, int type)
{
    privinfo_t *priv = ctx;
    select_node_t *node = file->ctx1;

    if (priv == NULL || node == NULL) {
        return -1;
    }

    node->next->prev = node->prev;
    node->prev->next = node->next;

    kfree(node);
    file->ctx1 = NULL;

    return 0;
}

/*
 * 回报事件
 */
static int select_report(void *ctx, int type)
{
    privinfo_t *priv = ctx;
    select_node_t *node;
    task_t *task;
    int flags = FALSE;
    uint32_t reg;

    reg = interrupt_disable();

    priv->flags = type;

    node = priv->wait_list.next;
    while (node != &priv->wait_list) {
        task = node->task;
        if (type & node->select_type && task->resume_type != TASK_RESUME_INTERRUPT) {
            task->timer       = 0;
            task->state       = TASK_RUNNING;
            task->resume_type = TASK_RESUME_SELECT_EVENT;
            if (!in_interrupt() &&
                task->type == TASK_TYPE_THREAD &&
                task->priority > current->priority) {
                flags = TRUE;
            }
        }
        node = node->next;
    }

    interrupt_resume(reg);

    if (flags) {
        yield();
    }
    return 0;
}

/*
 * 初始化 select
 */
static void select_init(void *ctx)
{
    privinfo_t *priv = ctx;

    priv->flags = 0;
    priv->wait_list.next = priv->wait_list.prev = &priv->wait_list;
}

#endif                                                                  /*  SELECTDRV_H_                */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
