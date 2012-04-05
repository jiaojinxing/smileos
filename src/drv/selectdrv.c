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
** File name:               selectdrv.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            select 驱动和设备模板
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-27
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "kern/kern.h"

/*
 * 私有信息
 */
typedef struct {
    int             flags;
    select_node_t   wait_list;
} privinfo_t;

/*
 * 打开 select
 */
static int select_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 0;
}

/*
 * 控制 select
 */
static int select_ioctl(void *ctx, file_t *file, int cmd, void *arg)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 0;
}

/*
 * 关闭 select
 */
static int select_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 0;
}

/*
 * select 是不是一个 TTY
 */
static int select_isatty(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 1;
}

/*
 * 读 select
 */
static ssize_t select_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 0;
}

/*
 * 写 select
 */
static ssize_t select_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        return -1;
    }
    return 0;
}

/*
 * 扫描
 */
static int select_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        return -1;
    }

    ret = 0;
    if (priv->flags & flags & VFS_FILE_READBLE) {
        ret |= VFS_FILE_READBLE;
    }
    if (priv->flags & flags & VFS_FILE_WRITEBLE) {
        ret |= VFS_FILE_WRITEBLE;
    }
    if (priv->flags & flags & VFS_FILE_ERROR) {
        ret |= VFS_FILE_ERROR;
    }
    return ret;
}

/*
 * select
 */
static int select_select(void *ctx, file_t *file, int type)
{
    privinfo_t *priv = ctx;
    select_node_t *node;
    uint32_t reg;

    if (priv == NULL) {
        return -1;
    }
    node = kmalloc(sizeof(sizeof(select_node_t)));
    if (node == NULL) {
        return -1;
    }

    reg = interrupt_disable();

    node->select_type = type;
    node->task        = current;

    priv->wait_list.next->prev = node;
    node->next = priv->wait_list.next;
    priv->wait_list.next = node;
    node->prev = &priv->wait_list;

    interrupt_resume(reg);

    file->ctx1 = node;

    return 0;
}

/*
 * unselect
 */
static int select_unselect(void *ctx, file_t *file, int type)
{
    privinfo_t *priv = ctx;
    select_node_t *node = file->ctx1;
    uint32_t reg;

    if (priv == NULL || node == NULL) {
        return -1;
    }

    reg = interrupt_disable();

    node->next->prev = node->prev;
    node->prev->next = node->next;

    interrupt_resume(reg);

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
    while (node != NULL) {
        task = node->task;
        if (type & task->select_type && task->resume_type != TASK_RESUME_INTERRUPT) {
            task->timer       = 0;
            task->state       = TASK_RUNNING;
            task->resume_type = TASK_RESUME_SELECT;
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
 * select 驱动
 */
static driver_t select_drv = {
        .name     = "select",
        .open     = select_open,
        .write    = select_write,
        .read     = select_read,
        .isatty   = select_isatty,
        .ioctl    = select_ioctl,
        .close    = select_close,
        .scan     = select_scan,
        .select   = select_select,
        .unselect = select_unselect,
};

/*
 * 初始化 select
 */
int select_init(void)
{
    privinfo_t *priv;

    driver_install(&select_drv);

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        priv->flags = 0;
        priv->wait_list.next = priv->wait_list.prev = &priv->wait_list;
        if (device_create("/dev/select", "select", priv) < 0) {
            kfree(priv);
            return -1;
        }
        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
