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

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
** 用户程序不需要知道内核的配置和数据类型及接口
*********************************************************************************************************/
#include "kern/types.h"

#if defined(SMILEOS_KERNEL) || defined(SMILEOS_MODULE)

#include "kern/config.h"

/*********************************************************************************************************
** 内核数据类型
*********************************************************************************************************/
#define HZ                          (TICK_PER_SECOND)                   /*  HZ                          */

/*
 * 任务类型
 */
#define TASK_TYPE_PROCESS           0                                   /*  进程                        */
#define TASK_TYPE_KTHREAD           1                                   /*  内核线程                    */

/*
 * 任务状态
 */
#define TASK_UNALLOCATE             ((uint8_t)-1)                       /*  未分配                      */
#define TASK_RUNNING                0                                   /*  就绪                        */
#define TASK_SLEEPING               1                                   /*  休睡                        */
#define TASK_SUSPEND                2                                   /*  挂起                        */

/*
 * 任务恢复类型
 */
#define TASK_RESUME_UNKNOW          (0)                                 /*  未知                        */
#define TASK_RESUME_MUTEX_COME      (1)                                 /*  互斥量到达                  */
#define TASK_RESUME_SEM_COME        (2)                                 /*  信号到达                    */
#define TASK_RESUME_SELECT_EVENT    (7)                                 /*  select 事件                 */
#define TASK_RESUME_MSG_COME        (4)                                 /*  消息到达                    */
#define TASK_RESUME_MSG_OUT         (5)                                 /*  消息被读取                  */
#define TASK_RESUME_TIMEOUT         (6)                                 /*  超时                        */
#define TASK_RESUME_INTERRUPT       (7)                                 /*  等待被中断                  */

/*
 * 设置 errno
 */
#include <errno.h>
#define seterrno(err)               errno = (err)
#define geterrno(err)               err = errno

#include <sys/reent.h>                                                  /*  for struct _reent           */

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
#if CPU_REGS_SIZE > 0
    reg_t                   regs[CPU_REGS_SIZE];                        /*  上下文                      */
#endif
#if KERN_STACK_SIZE > 0
    reg_t                   kstack[KERN_STACK_SIZE];                    /*  内核栈                      */
#endif
    uint8_t                 type;                                       /*  任务类型                    */
/********************************************************************************************************/
    uint8_t                 status;                                     /*  状态                        */
    uint8_t                 priority;                                   /*  优先级                      */
    uint8_t                 timeslice;                                  /*  剩余时间片                  */
    tick_t                  delay;                                      /*  休睡剩余 TICK 数            */
    uint8_t                 resume_type;                                /*  恢复类型                    */
    char                    name[NAME_MAX];                             /*  名字                        */
    struct _reent          *reent;                                      /*  可重入结构指针              */

    /*
     * 内核线程专属信息
     */
    void                  (*thread)(void *arg);                         /*  线程函数                    */
    void                   *arg;                                        /*  线程参数                    */
    void                   *stack_base;                                 /*  线程栈基址                  */
    size_t                  stack_size;                                 /*  线程栈大小                  */

    /*
     * IPC 专属信息
     */
    struct task            *next;                                       /*  后趋                        */
    struct task           **wait_list;                                  /*  等待链表                    */

    /*
     * PROCESS 专属信息
     */
    void                   *pinfo;                                      /*  进程信息指针                */

    /*
     * VMM 专属信息
     */
    void                   *vmm;
} task_t;

/*
 * 系统调用处理
 */
typedef int (*sys_do_t)();

/*
 * 系统调用参数
 */
typedef struct {
    void *arg0;
    void *arg1;
    void *arg2;
    void *arg3;
    void *arg4;
    void *arg5;
    void *arg6;
    void *arg7;
    void *arg8;
    void *arg9;
} syscall_args_t;

typedef tick_t mseconds_t;
/*********************************************************************************************************
  内核函数
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:           kernel_init
** Descriptions:            初始化内核
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_init(void);
/*********************************************************************************************************
** Function name:           kernel_start
** Descriptions:            启动内核
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_start(void);
/*********************************************************************************************************
** Function name:           kernel_timer
** Descriptions:            内核定时器处理函数
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_timer(void);
/*********************************************************************************************************
** Function name:           process_create
** Descriptions:            创建进程
** input parameters:        path                进程二进制文件 PATH
**                          timeslice           时间片
**                          argc                参数个数
**                          argv                参数数组
** output parameters:       NONE
** Returned value:          进程 PID
*********************************************************************************************************/
int32_t process_create(const char *path, uint8_t timeslice, int argc, char **argv);
/*********************************************************************************************************
** Function name:           kthread_create
** Descriptions:            创建内核线程
** input parameters:        name                内核线程名字
**                          func                内核线程进入点函数
**                          arg                 内核线程参数
**                          stack_size          内核线程栈空间大小
**                          priority            内核线程优先级
** output parameters:       NONE
** Returned value:          内核线程 TID
*********************************************************************************************************/
int32_t kthread_create(const char *name,
                       void (*func)(void *),
                       void *arg,
                       size_t stack_size,
                       uint8_t priority);
/*********************************************************************************************************
** Function name:           printk
** Descriptions:            终止内核前的报警也不能使用
** input parameters:        fmt                 格式字符串
**                          ...                 其余参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void printk(const char *fmt, ...);
#define KERN_EMERG      "<0>"
#define KERN_ALERT      "<1>"
#define KERN_CRIT       "<2>"
#define KERN_ERR        "<3>"
#define KERN_WARNING    "<4>"
#define KERN_NOTICE     "<5>"
#define KERN_INFO       "<6>"
#define KERN_DEBUG      "<7>"
/*********************************************************************************************************
** Function name:           __kmalloc
** Descriptions:            从内核内存堆里分配内存
** input parameters:        func                调用者的函数名
**                          line                调用者的行号
**                          size                需要分配的大小
**                          flags               分配标志
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *__kmalloc(const char *func, int line, size_t size, int flags);
#define GFP_ATOMIC      (0)
#define GFP_NOFS        (0)
#define GFP_KERNEL      GFP_ATOMIC
#define GFP_DMA         (1 << 0)
#define GFP_SHARE       (1 << 1)
#define GFP_ZERO        (1 << 8)
#define kmalloc(a, b)   __kmalloc(__func__, __LINE__, a, b)
#define kzalloc(a, b)   __kmalloc(__func__, __LINE__, a, b | GFP_ZERO)
/*********************************************************************************************************
** Function name:           __kfree
** Descriptions:            释放内存回内核内存堆
** input parameters:        func                调用者的函数名
**                          line                调用者的行号
**                          ptr                 内存指针
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void __kfree(const char *func, int line, void *ptr);
#define kfree(a)        __kfree(__func__, __LINE__, a)
/*********************************************************************************************************
** Function name:           getticks
** Descriptions:            获得 TICK
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TICK
*********************************************************************************************************/
tick_t getticks(void);
/*********************************************************************************************************
** Function name:           schedule
** Descriptions:            重新调度
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int schedule(void);
/*********************************************************************************************************
** 中断服务程序类型
*********************************************************************************************************/
typedef int (*isr_t)(intno_t interrupt, void *arg);
/*********************************************************************************************************
** Function name:           in_interrupt
** Descriptions:            判断是否在中断处理程序中
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
bool_t in_interrupt(void);
/*********************************************************************************************************
** Function name:           interrupt_enter
** Descriptions:            进入中断
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_enter(void);
/*********************************************************************************************************
** Function name:           interrupt_exit
** Descriptions:            退出中断
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_exit(void);
/*********************************************************************************************************
** Function name:           isr_invaild
** Descriptions:            无效的中断服务程序
** input parameters:        interrupt           中断号
**                          arg                 参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int isr_invaild(intno_t interrupt, void *arg);
/*********************************************************************************************************
** Function name:           interrupt_install
** Descriptions:            安装中断服务程序
** input parameters:        interrupt           中断号
**                          new_isr             新的中断服务程序
**                          arg                 中断服务程序参数
** output parameters:       old_isr             旧的中断服务程序
** Returned value:          0 OR -1
*********************************************************************************************************/
int interrupt_install(intno_t interrupt, isr_t new_isr, isr_t *old_isr, void *arg);
/*********************************************************************************************************
** Function name:           interrupt_exec
** Descriptions:            执行中断服务程序
** input parameters:        interrupt           中断号
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int interrupt_exec(intno_t interrupt);
/*********************************************************************************************************
** Function name:           msleep
** Descriptions:            休眠指定的毫秒数
** input parameters:        mseconds            毫秒数
** output parameters:       NONE
** Returned value:          0 OR -1
**
** 因为系统滴嗒是毫秒级, 所以提供该系统调用
**
*********************************************************************************************************/
int msleep(mseconds_t mseconds);
/*********************************************************************************************************
** Function name:           gettid
** Descriptions:            获得当前任务的 TID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          当前任务的 TID
*********************************************************************************************************/
int32_t gettid(void);
/*********************************************************************************************************
** Function name:           bkdr_hash
** Descriptions:            BKDR Hash Function
**                          各种字符串 Hash 函数比较
**                          http://www.byvoid.com/blog/string-hash-compare/
** input parameters:        str                 字符串
** output parameters:       NONE
** Returned value:          BKDR Hash
*********************************************************************************************************/
unsigned int bkdr_hash(const char *str);
/*********************************************************************************************************
** 工作队列
*********************************************************************************************************/
struct workqueue;
typedef struct workqueue workqueue_t;
/*********************************************************************************************************
** Function name:           workqueue_create
** Descriptions:            创建工作队列
** input parameters:        name                名字
**                          size                大小
**                          priority            优先级
** output parameters:       wq                  工作队列
** Returned value:          0 OR -1
*********************************************************************************************************/
int workqueue_create(workqueue_t *wq, const char *name, size_t size, uint8_t priority);
/*********************************************************************************************************
** Function name:           workqueue_add
** Descriptions:            增加一个工作
** input parameters:        wq                  工作队列
**                          func                工作函数
**                          arg                 工作函数参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int workqueue_add(workqueue_t *wq, void (*func)(void *), void *arg);
/*********************************************************************************************************
** Function name:           ms_to_tick
** Descriptions:            将毫秒转换成 tick
** input parameters:        ms                  毫秒
** output parameters:       NONE
** Returned value:          tick
*********************************************************************************************************/
static inline tick_t ms_to_tick(mseconds_t ms)
{
    if (ms != 0) {
        if (ms <= 1000 / HZ) {
            ms = 1;
        } else {
            ms = ms * HZ / 1000;
        }
    }
    return ms;
}
/*********************************************************************************************************
** Function name:           tick_to_ms
** Descriptions:            将 tick 转换成毫秒
** input parameters:        tick                  tick
** output parameters:       NONE
** Returned value:          毫秒
*********************************************************************************************************/
static inline mseconds_t tick_to_ms(tick_t tick)
{
    if (tick != 0) {
        tick = tick * 1000 / HZ;
    }
    return tick;
}
/*********************************************************************************************************
** Function name:           kheap_check
** Descriptions:            检查内核内存堆
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kheap_check(void);

#include "arch/if.h"

#endif

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  KERN_H_                     */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
