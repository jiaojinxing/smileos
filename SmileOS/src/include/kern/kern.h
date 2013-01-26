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

#include "kern/config.h"
#include "kern/types.h"

#if defined(SMILEOS_KERNEL) || defined(SMILEOS_MODULE)
/*********************************************************************************************************
  内核数据类型
*********************************************************************************************************/
/*
 * 任务类型
 */
#define TASK_TYPE_PROCESS           0                                   /*  进程                        */
#define TASK_TYPE_KTHREAD           1                                   /*  内核线程                    */

/*
 * 任务状态
 */
#define TASK_UNALLOCATE             ((uint32_t)-1)                      /*  未分配                      */
#define TASK_RUNNING                0                                   /*  就绪                        */
#define TASK_SLEEPING               1                                   /*  休睡                        */
#define TASK_SUSPEND                2                                   /*  挂起                        */

/*
 * 任务恢复类型
 */
#define TASK_RESUME_UNKNOW          0                                   /*  未知                        */
#define TASK_RESUME_MUTEX_COME      (1 << 1)                            /*  互斥量到达                  */
#define TASK_RESUME_SEM_COME        (1 << 2)                            /*  信号到达                    */
#define TASK_RESUME_TIMEOUT         (1 << 3)                            /*  超时                        */
#define TASK_RESUME_MSG_COME        (1 << 4)                            /*  消息到达                    */
#define TASK_RESUME_MSG_OUT         (1 << 5)                            /*  消息被读取                  */
#define TASK_RESUME_INTERRUPT       (1 << 6)                            /*  等待被中断                  */
#define TASK_RESUME_SELECT_EVENT    (1 << 7)                            /*  select 事件                 */
#define TASK_RESUME_AGAIN           (1 << 8)                            /*  需要重新试一次              */

#include <signal.h>
/*
 * 信号上下文
 */
typedef struct _signal_ctx_t {
    int                     ref;
    int                     sig;                                        /*  信号                        */
    struct _signal_ctx_t   *sig_int;
    uint32_t                regs[20];                                   /*  上下文                      */
    uint32_t                stack[SIG_USR_STACK_SIZE];                  /*  普通栈                      */
    uint32_t                kstack[SIG_KERN_STACK_SIZE];                /*  内核栈                      */
} signal_ctx_t;

/*
 * 设置 errno
 */
#include <errno.h>
#define seterrno(err)       errno = (err)

struct vmm_frame;

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
    uint32_t                status;                                     /*  状态                        */
    uint32_t                timeslice;                                  /*  剩余时间片                  */
    uint32_t                delay;                                      /*  休睡剩余 TICK 数            */
    uint32_t                priority;                                   /*  优先级                      */
    uint32_t                regs[20];                                   /*  上下文                      */
    uint32_t                kstack[KERN_STACK_SIZE];                    /*  内核栈                      */
    uint32_t                type;                                       /*  任务类型                    */
    uint32_t                resume_type;                                /*  恢复类型                    */
    uint32_t                cpu_usage;                                  /*  CPU 占用率                  */
    uint32_t                ticks;                                      /*  任务被定时器中断的次数      */
    char                    name[NAME_MAX];                             /*  名字                        */
    uint32_t                dabt_cnt;                                   /*  数据访问中止次数            */
    struct _reent          *reent;                                      /*  可重入结构指针              */
/********************************************************************************************************/
    /*
     * 内核线程专属信息
     */
    void                  (*thread)(void *arg);                         /*  线程函数                    */
    void                   *arg;                                        /*  线程参数                    */
    uint32_t                stack_base;                                 /*  线程栈基址                  */
    uint32_t                stack_size;                                 /*  线程栈大小                  */

    /*
     * 进程专属信息
     */
    uint32_t                frame_nr;                                   /*  页框数                      */
    uint32_t                page_tbl_nr;                                /*  页表数                      */
    struct vmm_frame       *frame_list;                                 /*  页框链表                    */
    uint32_t                mmu_backup[PROCESS_SPACE_SIZE / SECTION_SIZE];  /*  一级段表备份            */
    uint32_t                file_size;                                  /*  进程二进制文件大小          */
    void                   *pinfo;                                      /*  进程信息指针                */

    /*
     * IPC
     */
    struct task            *next;                                       /*  后趋                        */
    struct task           **wait_list;                                  /*  等待链表                    */

    /*
     * 信号
     */
    sigset_t                sigset_pend;                                /*  未决信号                    */
    sigset_t                sigset_mask;                                /*  未决信号                    */
    sigset_t                sigset_old;
    void                   *signal_queue;                               /*  信号队列                    */
    signal_ctx_t           *signal_ctx;                                 /*  信号上下文                  */
    struct sigaction        signal_action[NSIG];
    signal_ctx_t           *signal_current;
    uint32_t                signal_status;
    uint32_t                signal_delay;
    uint32_t                signal_resume;
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
**                          priority            进程时间片
**                          argc                参数个数
**                          argv                参数数组
** output parameters:       NONE
** Returned value:          进程 PID
*********************************************************************************************************/
int32_t process_create(const char *path, uint32_t priority, int argc, char **argv);
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
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stack_size, uint32_t priority);
/*********************************************************************************************************
** Function name:           printk
** Descriptions:            因为里面用了 kmalloc, 所以不能用在 kmalloc 失败之后,
**                          终止内核前的报警也不能使用
** input parameters:        fmt                 格式字符串
**                          ...                 其余参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void printk(const char *fmt, ...);
#define KERN_EMERG
#define KERN_ALERT
#define KERN_CRIT
#define KERN_ERR
#define KERN_WARNING
#define KERN_NOTICE
#define KERN_INFO
#define KERN_DEBUG
/*********************************************************************************************************
** Function name:           netjob_add
** Descriptions:            增加一个网络工作
** input parameters:        func                网络工作函数
**                          arg                 网络工作函数参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int netjob_add(void (*func)(void *), void *arg);
/*********************************************************************************************************
** Function name:           kcomplain
** Descriptions:            内核抱怨(供不能使用 printk 时使用)
** input parameters:        fmt                 格式字符串
**                          ...                 其余参数
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kcomplain(const char *fmt, ...);
/*********************************************************************************************************
** Function name:           __kmalloc
** Descriptions:            从内核内存堆里分配内存
** input parameters:        func                调用者的函数名
**                          line                调用者的行号
**                          size                需要分配的大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *__kmalloc(const char *func, int line, size_t size);
#define GFP_KERNEL      0
#define kmalloc(a, b)   __kmalloc(__func__, __LINE__, a)
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
** Function name:           __kcalloc
** Descriptions:            从内核内存堆里分配内存
** input parameters:        func                调用者的函数名
**                          line                调用者的行号
**                          nelem               元素的个数
**                          elsize              元素的大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *__kcalloc(const char *func, int line, size_t nelem, size_t elsize);
#define kcalloc(a, b)   __kcalloc(__func__, __LINE__, a, b)
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
** Function name:           getticks
** Descriptions:            获得 TICK
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TICK
*********************************************************************************************************/
uint64_t getticks(void);
/*********************************************************************************************************
** Function name:           gettid
** Descriptions:            获得当前任务的 TID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          当前任务的 TID
*********************************************************************************************************/
int32_t gettid(void);
/*********************************************************************************************************
** Function name:           in_interrupt
** Descriptions:            判断是否在中断处理程序中
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int in_interrupt(void);
/*********************************************************************************************************
** Function name:           schedule
** Descriptions:            重新调度
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int schedule(void);
/*********************************************************************************************************
** Function name:           in_kernel
** Descriptions:            判断是否在内核模式
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int in_kernel(void);
/*********************************************************************************************************
** Function name:           interrupt_disable
** Descriptions:            进入临界区域
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          进入临界区域前的 CPSR
*********************************************************************************************************/
uint32_t interrupt_disable(void);
/*********************************************************************************************************
** Function name:           interrupt_resume
** Descriptions:            退出临界区域
** input parameters:        reg                 进入临界区域前的 CPSR
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_resume(register uint32_t reg);
/*********************************************************************************************************
** Function name:           kputc
** Descriptions:            通过串口输出一个字符
** input parameters:        c                   字符
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kputc(unsigned char c);
/*********************************************************************************************************
** Function name:           kgetc
** Descriptions:            通过串口读取一个字符
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          一个字符
*********************************************************************************************************/
unsigned char kgetc(void);
/*********************************************************************************************************
** Function name:           va_to_mva
** Descriptions:            将进程空间的虚拟地址转换为修改后的虚拟地址
** input parameters:        VA                  进程空间的虚拟地址
** output parameters:       NONE
** Returned value:          修改后的虚拟地址
*********************************************************************************************************/
void *va_to_mva(const void *VA);
/*********************************************************************************************************
** Function name:           mva_to_va
** Descriptions:            将修改后的虚拟地址转换为进程空间的虚拟地址
** input parameters:        MVA                 修改后的虚拟地址
** output parameters:       NONE
** Returned value:          进程空间的虚拟地址
*********************************************************************************************************/
void *mva_to_va(const void *MVA);
/*********************************************************************************************************
** Function name:           vspace_usable
** Descriptions:            判断虚拟地址空间是否可用
** input parameters:        base                虚拟地址空间的基址
**                          size                虚拟地址空间的大小
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int vspace_usable(uint32_t base, uint32_t size);
/*********************************************************************************************************
** 中断服务程序类型
*********************************************************************************************************/
typedef int (*isr_t)(uint32_t interrupt, void *arg);
/*********************************************************************************************************
** Function name:           isr_invaild
** Descriptions:            无效中断服务程序
** input parameters:        interrupt           中断号
**                          arg                 参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int isr_invaild(uint32_t interrupt, void *arg);
/*********************************************************************************************************
** Function name:           interrupt_init
** Descriptions:            初始化中断
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_init(void);
/*********************************************************************************************************
** Function name:           interrupt_mask
** Descriptions:            屏蔽中断
** input parameters:        interrupt           中断号
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_mask(uint32_t interrupt);
/*********************************************************************************************************
** Function name:           interrupt_unmask
** Descriptions:            取消屏蔽中断
** input parameters:        interrupt           中断号
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_unmask(uint32_t interrupt);
/*********************************************************************************************************
** Function name:           interrupt_install
** Descriptions:            安装中断服务程序
** input parameters:        interrupt           中断号
**                          new_isr             新的中断服务程序
**                          arg                 中断服务程序参数
** output parameters:       old_isr             旧的中断服务程序
** Returned value:          NONE
*********************************************************************************************************/
int interrupt_install(uint32_t interrupt, isr_t new_isr, isr_t *old_isr, void *arg);

#endif

#endif                                                                  /*  KERN_H_                     */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
