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
** File name:               kern.h
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            内核头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#ifndef KERN_H_
#define KERN_H_

#include "config.h"
#include "types.h"

#ifdef SMILEOS_KERNEL

/*
 * 任务类型
 */
#define TASK_TYPE_PROCESS       0
#define TASK_TYPE_THREAD        1

/*
 * 任务状态
 */
#define TASK_UNALLOCATE         ((uint32_t)-1)
#define TASK_RUNNING            0
#define TASK_SLEEPING           1
#define TASK_SUSPEND            2

/*
 * 任务恢复类型
 */
#define TASK_RESUME_UNKNOW      0
#define TASK_RESUME_MUTEX_COME  (1 << 1)
#define TASK_RESUME_SEM_COME    (1 << 2)
#define TASK_RESUME_TIMEOUT     (1 << 3)
#define TASK_RESUME_MSG_COME    (1 << 4)
#define TASK_RESUME_MSG_OUT     (1 << 5)



struct _frame_t;
/*
 * 任务控制块
 */
typedef struct _task {
    int32_t             pid;
    int32_t             tid;
    uint32_t            state;
    uint32_t            count;
    uint32_t            timer;
    uint32_t            prio;
    uint32_t            content[20];
    uint32_t            kstack[KERN_STACK_SIZE];
    int                 type;
    int                 errno;
    int                 resume_type;
    struct _task       *next;
    struct _task      **wait_list;
    struct _frame_t    *frame_list;
} task_t;

/*
 * 当前运行的任务
 */
extern task_t *current;

/*
 * TICK
 */
extern uint64_t tick;

/*
 * 系统调用处理
 */
typedef uint32_t sys_do_t;

/*
 * 初始化内核
 */
void kernel_init(void);

/*
 * 启动内核
 */
void kernel_start(void);

/*
 * 任务调度, 调用之前必须关中断
 */
void schedule(void);

/*
 * 内核定时器处理函数
 */
void do_timer(void);

/*
 * 创建任务
 */
int32_t process_create(uint8_t *code, uint32_t size, uint32_t prio);

/*
 * 创建内核线程
 */
int32_t kthread_create(void (*func)(void *), void *arg, uint32_t stk_size, uint32_t prio);

/*
 * printk
 */
void printk(const char *fmt, ...);

/*
 * kputc
 */
void kputc(unsigned char c);

/*
 * kgetc
 */
unsigned char kgetc(void);

/*
 * 从内核内存堆分配内存
 */
void *kmalloc(uint32_t size);

/*
 * 释放内存回内核内存堆
 */
void kfree(void *ptr);

/*
 * 进入临界区域
 */
uint32_t interrupt_disable(void);

/*
 * 退出临界区域
 */
void interrupt_resume(register uint32_t reg);

/*
 * 进入中断
 */
void interrupt_enter(void);

/*
 * 退出中断
 */
void interrupt_exit(void);

/*
 * 获得 TICK
 */
uint64_t get_tick(void);

#endif

#endif                                                                  /*  KERN_H_                     */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
