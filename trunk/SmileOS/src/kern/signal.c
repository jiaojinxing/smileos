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
** File name:               signal.c
** Last modified Date:      2013-1-6
** Last Version:            1.0.0
** Descriptions:            信号
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-1-6
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
** 信号的设计实在太复杂了!
**
** 信号处理函数工作在 SYS 模式, 这是很有意义的, 首先内核线程工作在 SYS 模式,
**
** 那信号处理函数工作在 SYS 模式是毫无疑问的, 但进程工作在 USR 模式,
**
** 那信号处理函数工作在 SYS 模式会否有权限过宽的问题呢? 答案是肯定的!
**
** 但与其在实现上带来的便宜相比, 这点不足对于我的小系统来说不算什么.
**
** 首先我无需在 USR 模式下构建 USR 空间里的信号上下文, 这实在太麻烦了, 要考虑的东西也多,
**
** 其次我也无需考虑信号处理完毕后怎样回到内核继续处理(只能通过系统调用)这一麻烦的问题.
**
** 好了, 我承认我怕麻烦:-)
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/kvars.h"
#include "kern/arm.h"
#include "kern/ipc.h"
#include <string.h>

static void signal_finish(task_t *task, signal_ctx_t *ctx);
static void signal_next(task_t *task, signal_ctx_t *ctx, int sigint);
int signal_schedule(task_t *task, signal_ctx_t *ctx);

int ipc_task_cleanup(task_t *task);
void task_cleanup(task_t *task);
extern int task_pause(void);
/*********************************************************************************************************
** Function name:           signal_signal
** Descriptions:            绑定信号处理函数
** input parameters:        sig                 信号
**                          func                信号处理函数
** output parameters:       NONE
** Returned value:          原有的信号处理函数
*********************************************************************************************************/
sighandler_t signal_signal(int sig, sighandler_t func)
{
    task_t         *task;
    uint32_t        reg;
    sighandler_t    ret;

    if (sig < 0 || sig >= NSIG) {                                       /*  信号检查                    */
        seterrno(EINVAL);
        return NULL;
    }

    reg  = interrupt_disable();

    task = current;

    ret  = task->signal_action[sig].sa_handler;

    task->signal_action[sig].sa_handler = func;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           signal_action
** Descriptions:            设置信号动作
** input parameters:        sig                 信号
**                          act                 新的信号动作
**                          oact                旧的信号动作
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int signal_action(int sig, const struct sigaction *act, struct sigaction *oact)
{
    task_t         *task;
    uint32_t        reg;

    if (sig < 0 || sig >= NSIG) {                                       /*  信号检查                    */
        seterrno(EINVAL);
        return -1;
    }

    reg  = interrupt_disable();

    task = current;

    if (oact != NULL) {                                                 /*  保存旧的信号动作            */
        memcpy(oact, &task->signal_action[sig], sizeof(struct sigaction));
    }

    if (act == NULL) {                                                  /*  设置新的信号动作            */
        memcpy(&task->signal_action[sig], act, sizeof(struct sigaction));
        sigdelset(&task->signal_action[sig].sa_mask, SIGKILL);
        sigdelset(&task->signal_action[sig].sa_mask, SIGSTOP);

        schedule();                                                     /*  重新调度                    */
    }

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           signal_pending
** Descriptions:            查询未决信号集
** input parameters:        NONE
** output parameters:       set                 未决信号集
** Returned value:          0 OR -1
*********************************************************************************************************/
int signal_pending(sigset_t *set)
{
    task_t         *task;
    uint32_t        reg;
    int             i;
    int             sig_nr;
    signal_ctx_t   *tmp;

    if (set == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    reg  = interrupt_disable();

    sigemptyset(set);

    task = current;

    sig_nr = mqueue_msg_nr((mqueue_t *)&task->signal_queue);

    for (i = 0; i < sig_nr; i++) {
        if (mqueue_tryfetch((mqueue_t *)&task->signal_queue, (void **)&tmp) == 0) {
            if (tmp->ref > 0) {
                sigaddset(set, tmp->sig);
            }
            mqueue_trypost((mqueue_t *)&task->signal_queue, (void *)tmp);
        }
    }

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           signal_procmask
** Descriptions:            处理信号屏蔽掩码
** input parameters:        how                 方式
**                          set                 新的全局信号屏蔽掩码
**                          oset                旧的全局信号屏蔽掩码
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int signal_procmask(int how, const sigset_t *set, sigset_t *oset)
{
    task_t     *task;
    uint32_t    reg;

    reg  = interrupt_disable();

    task = current;

    if (oset != NULL) {
        *oset = task->sigset_mask;                                      /*  保存全局信号屏蔽掩码        */
    }

    if (set != NULL) {
        switch (how) {
        case SIG_BLOCK:                                                 /*  阻塞新的信号集              */
            task->sigset_mask |= *set;
            sigdelset(&task->sigset_mask, SIGKILL);
            sigdelset(&task->sigset_mask, SIGSTOP);
            break;

        case SIG_UNBLOCK:
            task->sigset_mask &= ~(*set);                               /*  取消阻塞信号集              */
            schedule();                                                 /*  重新调度                    */
            break;

        case SIG_SETMASK:                                               /*  设置阻塞信号集              */
            task->sigset_mask = *set;
            sigdelset(&task->sigset_mask, SIGKILL);
            sigdelset(&task->sigset_mask, SIGSTOP);
            schedule();                                                 /*  重新调度                    */
            break;
        }
    }

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           signal_suspend
** Descriptions:            等待信号
** input parameters:        set                 临时的全局信号屏蔽掩码
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int signal_suspend(const sigset_t *set)
{
    task_t     *task;
    uint32_t    reg;

    if (set == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    reg  = interrupt_disable();

    task = current;

    task->sigset_old  = task->sigset_mask;                              /*  保存全局信号屏蔽掩码        */

    task->sigset_mask = *set;                                           /*  设置新的全局信号屏蔽掩码    */
    sigdelset(&task->sigset_mask, SIGKILL);
    sigdelset(&task->sigset_mask, SIGSTOP);

    if (signal_schedule(task, NULL)) {                                  /*  信号调度                    */
        task->signal_ctx->ref++;                                        /*  该信号未决                  */
        schedule();                                                     /*  任务调度                    */
    } else {
        task_pause();                                                   /*  暂停任务                    */
    }

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           signal_queue
** Descriptions:            给任务发信号
** input parameters:        tid                 任务 TID
**                          sig                 信号
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int signal_queue(int32_t tid, int sig)
{
    int             ret = -1;
    task_t         *task;
    uint32_t        reg;
    signal_ctx_t   *ctx;

    if (tid <= 0 || tid >= TASK_NR) {                                   /*  TID 检查                    */
        seterrno(EINVAL);
        return ret;
    }

    if (sig < 0 || sig >= NSIG) {                                       /*  信号检查                    */
        seterrno(EINVAL);
        return ret;
    }

    reg  = interrupt_disable();

    task = &tasks[tid];                                                 /*  获得任务控制块              */

    if (task->status != TASK_UNALLOCATE) {                              /*  任务有效                    */

        if (sig < SIGRTMIN) {                                           /*  不可靠信号                  */
            if (sigismember(&task->sigset_pend, sig)) {                 /*  正在处理                    */
                seterrno(0);
                ret = 0;
                goto ret;
            }
            sigaddset(&task->sigset_pend, sig);
        } else {                                                        /*  可靠信号                    */
            sigaddset(&task->sigset_pend, sig);
        }

        ctx = kmalloc(sizeof(signal_ctx_t));                            /*  分配信号上下文              */
        if (ctx == NULL) {
            seterrno(ENOMEM);
            goto ret;
        }

        ctx->ref     = 1;                                               /*  引用一次                    */
        ctx->sig     = sig;                                             /*  信号码                      */
        ctx->sig_int = task->signal_current;                            /*  中断当前信号或任务          */

        ret = mqueue_trypost((mqueue_t *)&task->signal_queue, ctx);     /*  投递信号                    */
        if (ret < 0) {
            if (sig < SIGRTMIN) {
                sigdelset(&task->sigset_pend, sig);
            }
            kfree(ctx);
            seterrno(ENOSPC);
            goto ret;
        }

        if (interrupt_nest == 0) {                                      /*  不在中断中                  */
            schedule();                                                 /*  重新调度                    */
        }

        seterrno(0);
        ret = 0;
    } else {
        seterrno(EINVAL);
    }

    ret:
    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           signal_handler
** Descriptions:            信号处理函数
** input parameters:        ctx                 信号上下文
**                          sigint              是否收到过 SIGINT 信号
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void signal_handler(signal_ctx_t *ctx, int sigint)
{
    task_t         *task;
    int             sig;
    uint32_t        reg;
    sighandler_t    func;
    void          (*act)(int, siginfo_t *, void *);

    reg  = interrupt_disable();

    task = current;

    sig  = ctx->sig;

    func = task->signal_action[sig].sa_handler;
    act  = task->signal_action[sig].sa_sigaction;

    switch (sig) {
    default:
    case SIGHUP:                                                        /*  终端的挂断                  */
    case SIGINT:                                                        /*  来自键盘的中断信号          */
    case SIGQUIT:                                                       /*  来自键盘的离开信号          */
    case SIGABRT:                                                       /*  来自 abort 的异常信号       */
//  case SIGIOT:                                                        /*  IOT 自陷, 同 SIGABRT        */
    case SIGPIPE:                                                       /*  管道损坏                    */
    case SIGEMT:                                                        /*  EMT instruction             */
    case SIGALRM:                                                       /*  来自 alarm 的计时器到时信号 */
    case SIGTERM:                                                       /*  终止                        */
    case SIGUSR1:                                                       /*  用户自定义信号 1            */
    case SIGUSR2:                                                       /*  用户自定义信号 2            */
    case SIGSYS:                                                        /*  非法系统调用                */
    case SIGTRAP:                                                       /*  跟踪/断点自陷               */
//  case SIGVTALRM:
//  case SIGXCPU:
//  case SIGXFSZ:
//  case SIGPROF:
//  case SIGPWR:
//  case SIGINFO:
//  case SIGLOST:
//  case SIGUNUSED:
        if (func == SIG_DFL || func == SIG_ERR) {                       /*  默认都是杀死任务            */
            printk("signal_handler: task %s get signal %d\n", task->name, sig);
            task_cleanup(task);
        } else if (func != SIG_IGN) {                                   /*  绑定了处理函数              */
            interrupt_resume(reg);
            func(sig);
            interrupt_disable();
        }
        if (sig == SIGINT) {
            sigint = TRUE;
        }
        break;

    case SIGCHLD:                                                       /*  子进程停止或终止            */
//    case SIGCLD:                                                      /*  同 SIGCHLD                  */
    case SIGURG:                                                        /*  socket 紧急信号             */
    case SIGWINCH:                                                      /*  窗口大小改变                */
    case SIGIO:                                                         /*  描述符上可以进行 IO 操作    */
//  case SIGPOLL:                                                       /*  pollable 事件发生, 同 SIGIO */
        if (func == SIG_ERR) {
            printk("signal_handler: task %s get signal %d\n", task->name, sig);
            task_cleanup(task);
        } else if (func != SIG_DFL && func != SIG_IGN) {                /*  绑定了处理函数              */
            interrupt_resume(reg);
            func(sig);
            interrupt_disable();
        } else {                                                        /*  默认都是忽略信号            */

        }
        break;

    case SIGILL:                                                        /*  非法指令                    */
    case SIGFPE:                                                        /*  浮点例外                    */
    case SIGKILL:                                                       /*  杀死                        */
    case SIGSEGV:                                                       /*  段非法错误(内存引用无效)    */
    case SIGBUS:                                                        /*  总线错误(内存访问错误)      */
                                                                        /*  默认都是杀死任务            */
                                                                        /*  不允许绑定处理函数          */
        printk("signal_handler: task %s get signal %d\n", task->name, sig);
        task_cleanup(task);
        break;

    case SIGCONT:                                                       /*  如果停止, 继续执行          */
        task->signal_status = TASK_RUNNING;
        task->signal_delay  = 0;
        break;

    case SIGSTOP:                                                       /*  非来自终端的停止信号        */
    case SIGTSTP:                                                       /*  来自终端的停止信号          */
    case SIGTTIN:                                                       /*  后台进程读终端              */
    case SIGTTOU:                                                       /*  后台进程写终端              */
        if (func == SIG_ERR) {
            printk("signal_handler: task %s get signal %d\n", task->name, sig);
            task_cleanup(task);
        } else {
            task_pause();                                               /*  停止任务                    */
        }
        break;
    }

    signal_finish(task, ctx);                                           /*  完成该信号                  */

    signal_next(task, ctx, sigint);                                     /*  调度下一个信号              */
}
/*********************************************************************************************************
** Function name:           signal_finish
** Descriptions:            完成一个信号
** input parameters:        task                任务控制块
**                          ctx                 信号上下文
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void signal_finish(task_t *task, signal_ctx_t *ctx)
{
    if (ctx->sig < SIGRTMIN) {                                          /*  不可靠信号                  */
        sigdelset(&task->sigset_pend, ctx->sig);                        /*  清除该信号的等待标志        */
    } else {
        int i;
        int sig_nr = mqueue_msg_nr((mqueue_t *)&task->signal_queue);    /*  队列中信号的数目            */
        int find   = FALSE;

        for (i = 0; i < sig_nr; i++) {                                  /*  查访每一个信号              */
            signal_ctx_t *tmp;

            if (mqueue_tryfetch((mqueue_t *)&task->signal_queue, (void **)&tmp) == 0) {
                if (tmp != ctx) {                                       /*  不是将要完成的信号          */
                    if (tmp->sig == ctx->sig) {                         /*  找到同类型的信号            */
                        find = TRUE;
                    }
                }
                mqueue_trypost((mqueue_t *)&task->signal_queue, (void *)tmp);
            }
        }
        if (!find) {                                                    /*  队列中没有同类型的信号      */
            sigdelset(&task->sigset_pend, ctx->sig);                    /*  清除该信号的等待标志        */
        }
    }

    task->signal_current = NULL;
}
/*********************************************************************************************************
** Function name:           signal_print_regs
** Descriptions:            打印寄存器
** input parameters:        regs                寄存器组
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
#if 1
static void signal_print_regs(uint32_t *regs)
{
    int i;

    kcomplain(" REG \n", regs[18]);
    kcomplain("SP_svc\t= 0x%x\n", regs[0]);
    kcomplain("CPSR\t= 0x%x\n", regs[1]);
    kcomplain("SP_sys\t= 0x%x\n", regs[2]);
    kcomplain("SPSR\t= 0x%x\n", regs[3]);
    kcomplain("LR\t= 0x%x\n", regs[4]);
    for (i = 0; i < 13; i++) {
        kcomplain("R%d\t= 0x%x\n", i, regs[5 + i]);
    }
    kcomplain("PC\t= 0x%x\n", regs[18]);
}
#endif
/*********************************************************************************************************
** Function name:           signal_next
** Descriptions:            信号调度并进入下一个信号或回到任务断点
** input parameters:        task                任务控制块
**                          ctx                 当前信号上下文
**                          sigint              是否收到过 SIGINT 信号
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void signal_next(task_t *task, signal_ctx_t *ctx, int sigint)
{
    signal_ctx_t   *tmp;
    int             i;
    int             sig_nr;

    sig_nr = mqueue_msg_nr((mqueue_t *)&task->signal_queue);            /*  队列中信号的数目            */

    for (i = 0; i < sig_nr; i++) {                                      /*  从信号队列里将信号删除      */
        if (mqueue_tryfetch((mqueue_t *)&task->signal_queue, (void **)&tmp) == 0) {
            if (tmp == ctx) {
                continue;
            }
            mqueue_trypost((mqueue_t *)&task->signal_queue, (void *)tmp);
        }
    }

    if (sig_nr > 1 && signal_schedule(task, ctx)) {                     /*  信号调度                    */

        signal_ctx_t *new_ctx = task->signal_ctx;                       /*  下一个信号                  */

        task->signal_current = new_ctx;                                 /*  修改当前信号                */

        memcpy(new_ctx->regs, ctx->regs, sizeof(new_ctx->regs));        /*  转移到新信号的备份寄存器组  */

        /*
         * 初始化新信号的上下文
         */
        task->regs[0]   = (uint32_t)&new_ctx->kstack[SIG_KERN_STACK_SIZE];  /*  SVC 模式的 SP           */
        task->regs[1]   = ARM_SYS_MODE | ARM_FIQ_EN | ARM_IRQ_EN;       /*  CPSR, SYS 模式              */
        task->regs[2]   = (uint32_t)&new_ctx->stack[SIG_USR_STACK_SIZE];    /*  SYS 模式的 SP           */
        task->regs[3]   = ARM_SVC_MODE;                                 /*  SPSR, SVC 模式              */
        task->regs[4]   = 0;                                            /*  LR                          */
        task->regs[5]   = (uint32_t)new_ctx;                            /*  R0 ~ R12                    */
        task->regs[6]   = sigint;
        task->regs[7]   = 2;
        task->regs[8]   = 3;
        task->regs[9]   = 4;
        task->regs[10]  = 5;
        task->regs[11]  = 6;
        task->regs[12]  = 7;
        task->regs[13]  = 8;
        task->regs[14]  = 9;
        task->regs[15]  = 10;
        task->regs[16]  = 11;
        task->regs[17]  = 12;
        task->regs[18]  = (uint32_t)signal_handler;                     /*  PC                          */

        kfree(ctx);                                                     /*  释放信号上下文              */

        extern void signal_handler_exit(uint32_t *from, uint32_t *to);
        signal_handler_exit(NULL, task->regs);                          /*  信号处理退出                */
    } else {
        /*
         * 回到任务断点
         */
        task->signal_current  = NULL;                                   /*  当前信号无效                */

        if (sigint) {                                                   /*  收到过 SIGINT 信号          */
            task->status      = TASK_RUNNING;
            task->delay       = 0;
            task->resume_type = TASK_RESUME_INTERRUPT;                  /*  系统调用被中断              */
        } else {
            task->status      = task->signal_status;
            task->delay       = task->signal_delay;
            task->resume_type = TASK_RESUME_AGAIN;                      /*  唤醒时应该重试系统调用      */
        }

        memcpy(task->regs, ctx->regs, sizeof(task->regs));              /*  恢复任务断点上下文          */

        kfree(ctx);                                                     /*  释放信号上下文              */

        if (task->status == TASK_RUNNING) {
            extern void signal_handler_exit(uint32_t *from, uint32_t *to);
            signal_handler_exit(NULL, task->regs);                      /*  信号处理退出                */
        } else {
//            extern void task_schedule(void);
//            task_schedule();
            signal_print_regs(task->regs);
            while (1);
        }
    }
}
/*********************************************************************************************************
** Function name:           signal_schedule
** Descriptions:            信号调度
** input parameters:        task                任务控制块
**                          ctx                 信号上下文
** output parameters:       NONE
** Returned value:          TRUE OF FALSE
*********************************************************************************************************/
int signal_schedule(task_t *task, signal_ctx_t *ctx)
{
    sigset_t        sigset_mask;
    sigset_t        sigset_pend;
    signal_ctx_t   *tmp;
    int             i;
    int             sig_nr;

    task->signal_ctx = NULL;                                            /*  默认不需要切换到新信号      */

    sig_nr = mqueue_msg_nr((mqueue_t *)&task->signal_queue);            /*  队列中信号的数目            */
    if (sig_nr == 0) {                                                  /*  优化无信号的情况            */
        return FALSE;
    }

    sigset_mask = task->sigset_mask;                                    /*  全局的信号屏蔽掩码          */

    if (ctx == NULL && task->signal_current != NULL) {                  /*  如果不是由于当前信号退出    */
                                                                        /*  并当前是在信号当中          */
                                                                        /*  加入当前信号的信号屏蔽掩码  */
        sigset_mask |= task->signal_action[task->signal_current->sig].sa_mask;

        sigaddset(&sigset_mask, task->signal_current->sig);             /*  还要屏蔽当前信号            */
    }

    sigset_pend = task->sigset_pend & ~sigset_mask;                     /*  允许的未决信号列表          */

    if (sigset_pend != 0) {                                             /*  有允许的未决信号            */

        if (ctx != NULL) {                                              /*  如果是由于当前信号退出      */
            /*
             * 被当前信号中断的信号叫信号 A
             *
             * 由于当前信号退出, 所以所有对当前信号中断的信号都将变为对信号 A 或任务的中断
             *
             * 这是由于屏蔽掩码造成的
             */
            for (i = 0; i < sig_nr; i++) {                              /*  查访每一个信号              */
                if (mqueue_tryfetch((mqueue_t *)&task->signal_queue, (void **)&tmp) == 0) {
                    if (tmp->sig_int == ctx) {
                        tmp->sig_int = ctx->sig_int;
                    }
                    mqueue_trypost((mqueue_t *)&task->signal_queue, (void *)tmp);
                }
            }

            /*
             * ctx->sig_int != NULL 应该调度出中断了信号 A 的其它信号, 否则回到信号 A
             */
            /*
             * ctx->sig_int == NULL 应该调度出中断了任务的其它信号, 否则回到任务
             */
            ctx = ctx->sig_int;
        } else {
            /*
             * task->signal_current != NULL 应该调度出中断了当前信号的其它信号, 否则回到当前信号
             */
            /*
             * task->signal_current == NULL 应该调度出中断了任务的其它信号, 否则回到任务
             */
            ctx = task->signal_current;
        }

        /*
         * 下面是合并后的信号调度代码
         */

        for (i = 0; i < sig_nr; i++) {                                  /*  查访每一个信号              */
            if (mqueue_tryfetch((mqueue_t *)&task->signal_queue, (void **)&tmp) == 0) {
                if (tmp->ref != 0) {                                    /*  未决信号                    */
                    if (task->signal_ctx == NULL && sigismember(&sigset_pend, tmp->sig)) {
                        /*
                         * 未调度出下一个信号并且该未决信号是允许的
                         */
                        if (tmp->sig_int == ctx) {
                            task->signal_ctx = tmp;                     /*  下一个信号                  */
                            tmp->ref--;                                 /*  该信号已决                  */
                        }
                    }
                }
                mqueue_trypost((mqueue_t *)&task->signal_queue, (void *)tmp);
            }
        }

        if (task->signal_ctx == NULL) {
            task->signal_ctx = ctx;
        }

        if (task->signal_ctx != NULL) {                                 /*  调度出下一个信号            */
            if (task->sigset_old != (sigset_t)-1) {                     /*  有旧的全局信号屏蔽掩码      */
                task->sigset_mask = task->sigset_old;                   /*  恢复旧的全局信号屏蔽掩码    */
                sigfillset(&task->sigset_old);
            }
            return TRUE;
        }
    }

    return FALSE;
}
/*********************************************************************************************************
** Function name:           signal_switch
** Descriptions:            信号处理函数切换
** input parameters:        task                任务控制块
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void signal_switch(task_t *task)
{
    signal_ctx_t *ctx = task->signal_ctx;

    if (ctx != NULL) {                                                  /*  如果需要切换到新信号        */

        task->signal_ctx     = NULL;

        task->signal_current = ctx;                                     /*  当前信号                    */

        /*
         * 保存旧的上下文
         */
        memcpy(ctx->regs, task->regs, sizeof(ctx->regs));

        /*
         * 初始化新信号的上下文
         */
        task->regs[0]   = (uint32_t)&ctx->kstack[SIG_KERN_STACK_SIZE];  /*  SVC 模式的 SP (满堆栈递减)  */
        task->regs[1]   = ARM_SYS_MODE | ARM_FIQ_EN | ARM_IRQ_EN;       /*  CPSR, SYS 模式              */
        task->regs[2]   = (uint32_t)&ctx->stack[SIG_USR_STACK_SIZE];    /*  SYS 模式的 SP (满堆栈递减)  */
        task->regs[3]   = ARM_SVC_MODE;                                 /*  SPSR, SVC 模式              */
        task->regs[4]   = 0;                                            /*  LR                          */
        task->regs[5]   = (uint32_t)ctx;                                /*  R0 ~ R12                    */
        task->regs[6]   = FALSE;
        task->regs[7]   = 2;
        task->regs[8]   = 3;
        task->regs[9]   = 4;
        task->regs[10]  = 5;
        task->regs[11]  = 6;
        task->regs[12]  = 7;
        task->regs[13]  = 8;
        task->regs[14]  = 9;
        task->regs[15]  = 10;
        task->regs[16]  = 11;
        task->regs[17]  = 12;
        task->regs[18]  = (uint32_t)signal_handler;                     /*  PC                          */
    }
}
/*********************************************************************************************************
** Function name:           signal_task_init
** Descriptions:            初始化任务的信号系统
** input parameters:        task                任务控制块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int signal_task_init(task_t *task)
{
    sigemptyset(&task->sigset_pend);
    sigemptyset(&task->sigset_mask);
    sigfillset(&task->sigset_old);

    memset(task->signal_action, 0, sizeof(task->signal_action));

    task->signal_current = NULL;

    task->signal_ctx     = NULL;

    task->signal_status  = 0;
    task->signal_delay   = 0;
    task->signal_resume  = 0;

    return mqueue_new((mqueue_t *)&task->signal_queue, NSIG / 2);
}
/*********************************************************************************************************
** Function name:           signal_task_cleanup
** Descriptions:            清理任务的信号系统
** input parameters:        task                任务控制块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int signal_task_cleanup(task_t *task)
{
    void *msg;

    while (mqueue_tryfetch((mqueue_t *)&task->signal_queue, &msg) == 0) {
        kfree(msg);
    }

    mqueue_free((mqueue_t *)&task->signal_queue);

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
