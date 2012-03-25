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
/*********************************************************************************************************
  内核数据类型
*********************************************************************************************************/
/*
 * 任务类型
 */
#define TASK_TYPE_PROCESS       0                                       /*  进程                        */
#define TASK_TYPE_THREAD        1                                       /*  内核线程                    */

/*
 * 任务状态
 */
#define TASK_UNALLOCATE         ((uint32_t)-1)                          /*  未分配                      */
#define TASK_RUNNING            0                                       /*  就绪                        */
#define TASK_SLEEPING           1                                       /*  休睡                        */
#define TASK_SUSPEND            2                                       /*  挂起                        */

/*
 * 任务恢复类型
 */
#define TASK_RESUME_UNKNOW      0                                       /*  未知                        */
#define TASK_RESUME_MUTEX_COME  (1 << 1)                                /*  互斥量到达                  */
#define TASK_RESUME_SEM_COME    (1 << 2)                                /*  信号到达                    */
#define TASK_RESUME_TIMEOUT     (1 << 3)                                /*  超时                        */
#define TASK_RESUME_MSG_COME    (1 << 4)                                /*  消息到达                    */
#define TASK_RESUME_MSG_OUT     (1 << 5)                                /*  消息被读取                  */
#define TASK_RESUME_INTERRUPT   (1 << 6)                                /*  等待被中断                  */

struct vmm_frame;
/*
 * 任务控制块
 */
typedef struct task {
/********************************************************************************************************/
    /*
     * 请勿修改该区域的成员变量, 位置也不能变!
     */
    int32_t                 pid;                                        /*  进程 ID                     */
    int32_t                 tid;                                        /*  任务 ID                     */
    uint32_t                state;                                      /*  状态                        */
    uint32_t                counter;                                    /*  剩余时间片                  */
    uint32_t                timer;                                      /*  休睡剩余 TICK 数            */
    uint32_t                priority;                                   /*  优先级                      */
    uint32_t                content[20];                                /*  上下文                      */
    uint32_t                kstack[KERN_STACK_SIZE];                    /*  内核栈                      */
/********************************************************************************************************/
    void                  (*thread)(void *arg);                         /*  线程函数                    */
    void                   *arg;                                        /*  线程参数                    */
    uint32_t                stack;                                      /*  线程栈基址                  */
    uint32_t                type;                                       /*  任务类型                    */
    uint32_t                resume_type;                                /*  恢复类型                    */
    uint32_t                frame_nr;                                   /*  页框数                      */
    uint32_t                utilization;                                /*  CPU 占用率                  */
    uint32_t                tick;                                       /*  任务被定时器中断的次数      */
    int                     errno;                                      /*  错误号                      */
    char                    name[32];                                   /*  名字                        */
    struct task            *next;                                       /*  后趋                        */
    struct task           **wait_list;                                  /*  等待链表                    */
    struct vmm_frame       *frame_list;                                 /*  页框链表                    */
} task_t;

/*
 * 系统调用处理
 */
typedef int (*sys_do_t)();
/*********************************************************************************************************
  内核变量
*********************************************************************************************************/
extern task_t               tasks[TASK_NR];                             /*  任务控制块                  */
extern task_t              *current;                                    /*  指向当前运行的任务          */
/*********************************************************************************************************
  内核函数
*********************************************************************************************************/
/*
 * 初始化内核
 */
void kernel_init(void);

/*
 * 启动内核
 */
void kernel_start(void);

/*
 * 任务调度
 * 调用之前必须关中断
 */
void schedule(void);

/*
 * 内核定时器处理函数
 */
void do_timer(void);

/*
 * 创建进程
 */
int32_t process_create(const char *name, uint8_t *code, uint32_t size, uint32_t priority);

/*
 * 创建内核线程
 */
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stk_size, uint32_t priority);

/*
 * printk
 * 因为里面用了 kmalloc, 所以不能用在 kmalloc 失败之后, 终止内核前的报警也不能使用
 */
void printk(const char *fmt, ...);

/*
 * 内核抱怨
 * 供异常处理程序使用
 */
void kcomplain(const char *fmt, ...);

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
 * kcalloc
 */
void *kcalloc(uint32_t nelem, uint32_t elsize);

/*
 * 打印内核内存堆信息
 */
void kern_heap_show(void);

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

/*
 * 判断是否在中断处理中
 */
int in_interrupt(void);

/*
 * 判断是否在内核模式
 */
int in_kernel(void);
#endif

#endif                                                                  /*  KERN_H_                     */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
