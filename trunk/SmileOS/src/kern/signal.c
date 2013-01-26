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
** Descriptions:            �ź�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-1-6
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
** �źŵ����ʵ��̫������!
**
** �źŴ����������� SYS ģʽ, ���Ǻ��������, �����ں��̹߳����� SYS ģʽ,
**
** ���źŴ����������� SYS ģʽ�Ǻ������ʵ�, �����̹����� USR ģʽ,
**
** ���źŴ����������� SYS ģʽ�����Ȩ�޹����������? ���ǿ϶���!
**
** ��������ʵ���ϴ����ı������, ��㲻������ҵ�Сϵͳ��˵����ʲô.
**
** ������������ USR ģʽ�¹��� USR �ռ�����ź�������, ��ʵ��̫�鷳��, Ҫ���ǵĶ���Ҳ��,
**
** �����Ҳ���迼���źŴ�����Ϻ������ص��ں˼�������(ֻ��ͨ��ϵͳ����)��һ�鷳������.
**
** ����, �ҳ��������鷳:-)
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
** Descriptions:            ���źŴ�����
** input parameters:        sig                 �ź�
**                          func                �źŴ�����
** output parameters:       NONE
** Returned value:          ԭ�е��źŴ�����
*********************************************************************************************************/
sighandler_t signal_signal(int sig, sighandler_t func)
{
    task_t         *task;
    uint32_t        reg;
    sighandler_t    ret;

    if (sig < 0 || sig >= NSIG) {                                       /*  �źż��                    */
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
** Descriptions:            �����źŶ���
** input parameters:        sig                 �ź�
**                          act                 �µ��źŶ���
**                          oact                �ɵ��źŶ���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int signal_action(int sig, const struct sigaction *act, struct sigaction *oact)
{
    task_t         *task;
    uint32_t        reg;

    if (sig < 0 || sig >= NSIG) {                                       /*  �źż��                    */
        seterrno(EINVAL);
        return -1;
    }

    reg  = interrupt_disable();

    task = current;

    if (oact != NULL) {                                                 /*  ����ɵ��źŶ���            */
        memcpy(oact, &task->signal_action[sig], sizeof(struct sigaction));
    }

    if (act == NULL) {                                                  /*  �����µ��źŶ���            */
        memcpy(&task->signal_action[sig], act, sizeof(struct sigaction));
        sigdelset(&task->signal_action[sig].sa_mask, SIGKILL);
        sigdelset(&task->signal_action[sig].sa_mask, SIGSTOP);

        schedule();                                                     /*  ���µ���                    */
    }

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           signal_pending
** Descriptions:            ��ѯδ���źż�
** input parameters:        NONE
** output parameters:       set                 δ���źż�
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
** Descriptions:            �����ź���������
** input parameters:        how                 ��ʽ
**                          set                 �µ�ȫ���ź���������
**                          oset                �ɵ�ȫ���ź���������
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
        *oset = task->sigset_mask;                                      /*  ����ȫ���ź���������        */
    }

    if (set != NULL) {
        switch (how) {
        case SIG_BLOCK:                                                 /*  �����µ��źż�              */
            task->sigset_mask |= *set;
            sigdelset(&task->sigset_mask, SIGKILL);
            sigdelset(&task->sigset_mask, SIGSTOP);
            break;

        case SIG_UNBLOCK:
            task->sigset_mask &= ~(*set);                               /*  ȡ�������źż�              */
            schedule();                                                 /*  ���µ���                    */
            break;

        case SIG_SETMASK:                                               /*  ���������źż�              */
            task->sigset_mask = *set;
            sigdelset(&task->sigset_mask, SIGKILL);
            sigdelset(&task->sigset_mask, SIGSTOP);
            schedule();                                                 /*  ���µ���                    */
            break;
        }
    }

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           signal_suspend
** Descriptions:            �ȴ��ź�
** input parameters:        set                 ��ʱ��ȫ���ź���������
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

    task->sigset_old  = task->sigset_mask;                              /*  ����ȫ���ź���������        */

    task->sigset_mask = *set;                                           /*  �����µ�ȫ���ź���������    */
    sigdelset(&task->sigset_mask, SIGKILL);
    sigdelset(&task->sigset_mask, SIGSTOP);

    if (signal_schedule(task, NULL)) {                                  /*  �źŵ���                    */
        task->signal_ctx->ref++;                                        /*  ���ź�δ��                  */
        schedule();                                                     /*  �������                    */
    } else {
        task_pause();                                                   /*  ��ͣ����                    */
    }

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           signal_queue
** Descriptions:            �������ź�
** input parameters:        tid                 ���� TID
**                          sig                 �ź�
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int signal_queue(int32_t tid, int sig)
{
    int             ret = -1;
    task_t         *task;
    uint32_t        reg;
    signal_ctx_t   *ctx;

    if (tid <= 0 || tid >= TASK_NR) {                                   /*  TID ���                    */
        seterrno(EINVAL);
        return ret;
    }

    if (sig < 0 || sig >= NSIG) {                                       /*  �źż��                    */
        seterrno(EINVAL);
        return ret;
    }

    reg  = interrupt_disable();

    task = &tasks[tid];                                                 /*  ���������ƿ�              */

    if (task->status != TASK_UNALLOCATE) {                              /*  ������Ч                    */

        if (sig < SIGRTMIN) {                                           /*  ���ɿ��ź�                  */
            if (sigismember(&task->sigset_pend, sig)) {                 /*  ���ڴ���                    */
                seterrno(0);
                ret = 0;
                goto ret;
            }
            sigaddset(&task->sigset_pend, sig);
        } else {                                                        /*  �ɿ��ź�                    */
            sigaddset(&task->sigset_pend, sig);
        }

        ctx = kmalloc(sizeof(signal_ctx_t), GFP_KERNEL);                /*  �����ź�������              */
        if (ctx == NULL) {
            seterrno(ENOMEM);
            goto ret;
        }

        ctx->ref     = 1;                                               /*  ����һ��                    */
        ctx->sig     = sig;                                             /*  �ź���                      */
        ctx->sig_int = task->signal_current;                            /*  �жϵ�ǰ�źŻ�����          */

        ret = mqueue_trypost((mqueue_t *)&task->signal_queue, ctx);     /*  Ͷ���ź�                    */
        if (ret < 0) {
            if (sig < SIGRTMIN) {
                sigdelset(&task->sigset_pend, sig);
            }
            kfree(ctx);
            seterrno(ENOSPC);
            goto ret;
        }

        if (interrupt_nest == 0) {                                      /*  �����ж���                  */
            schedule();                                                 /*  ���µ���                    */
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
** Descriptions:            �źŴ�����
** input parameters:        ctx                 �ź�������
**                          sigint              �Ƿ��յ��� SIGINT �ź�
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
    case SIGHUP:                                                        /*  �ն˵ĹҶ�                  */
    case SIGINT:                                                        /*  ���Լ��̵��ж��ź�          */
    case SIGQUIT:                                                       /*  ���Լ��̵��뿪�ź�          */
    case SIGABRT:                                                       /*  ���� abort ���쳣�ź�       */
//  case SIGIOT:                                                        /*  IOT ����, ͬ SIGABRT        */
    case SIGPIPE:                                                       /*  �ܵ���                    */
    case SIGEMT:                                                        /*  EMT instruction             */
    case SIGALRM:                                                       /*  ���� alarm �ļ�ʱ����ʱ�ź� */
    case SIGTERM:                                                       /*  ��ֹ                        */
    case SIGUSR1:                                                       /*  �û��Զ����ź� 1            */
    case SIGUSR2:                                                       /*  �û��Զ����ź� 2            */
    case SIGSYS:                                                        /*  �Ƿ�ϵͳ����                */
    case SIGTRAP:                                                       /*  ����/�ϵ�����               */
//  case SIGVTALRM:
//  case SIGXCPU:
//  case SIGXFSZ:
//  case SIGPROF:
//  case SIGPWR:
//  case SIGINFO:
//  case SIGLOST:
//  case SIGUNUSED:
        if (func == SIG_DFL || func == SIG_ERR) {                       /*  Ĭ�϶���ɱ������            */
            printk("signal_handler: task %s get signal %d\n", task->name, sig);
            task_cleanup(task);
        } else if (func != SIG_IGN) {                                   /*  ���˴�����              */
            interrupt_resume(reg);
            func(sig);
            interrupt_disable();
        }
        if (sig == SIGINT) {
            sigint = TRUE;
        }
        break;

    case SIGCHLD:                                                       /*  �ӽ���ֹͣ����ֹ            */
//    case SIGCLD:                                                      /*  ͬ SIGCHLD                  */
    case SIGURG:                                                        /*  socket �����ź�             */
    case SIGWINCH:                                                      /*  ���ڴ�С�ı�                */
    case SIGIO:                                                         /*  �������Ͽ��Խ��� IO ����    */
//  case SIGPOLL:                                                       /*  pollable �¼�����, ͬ SIGIO */
        if (func == SIG_ERR) {
            printk("signal_handler: task %s get signal %d\n", task->name, sig);
            task_cleanup(task);
        } else if (func != SIG_DFL && func != SIG_IGN) {                /*  ���˴�����              */
            interrupt_resume(reg);
            func(sig);
            interrupt_disable();
        } else {                                                        /*  Ĭ�϶��Ǻ����ź�            */

        }
        break;

    case SIGILL:                                                        /*  �Ƿ�ָ��                    */
    case SIGFPE:                                                        /*  ��������                    */
    case SIGKILL:                                                       /*  ɱ��                        */
    case SIGSEGV:                                                       /*  �ηǷ�����(�ڴ�������Ч)    */
    case SIGBUS:                                                        /*  ���ߴ���(�ڴ���ʴ���)      */
                                                                        /*  Ĭ�϶���ɱ������            */
                                                                        /*  ������󶨴�����          */
        printk("signal_handler: task %s get signal %d\n", task->name, sig);
        task_cleanup(task);
        break;

    case SIGCONT:                                                       /*  ���ֹͣ, ����ִ��          */
        task->signal_status = TASK_RUNNING;
        task->signal_delay  = 0;
        break;

    case SIGSTOP:                                                       /*  �������ն˵�ֹͣ�ź�        */
    case SIGTSTP:                                                       /*  �����ն˵�ֹͣ�ź�          */
    case SIGTTIN:                                                       /*  ��̨���̶��ն�              */
    case SIGTTOU:                                                       /*  ��̨����д�ն�              */
        if (func == SIG_ERR) {
            printk("signal_handler: task %s get signal %d\n", task->name, sig);
            task_cleanup(task);
        } else {
            task_pause();                                               /*  ֹͣ����                    */
        }
        break;
    }

    signal_finish(task, ctx);                                           /*  ��ɸ��ź�                  */

    signal_next(task, ctx, sigint);                                     /*  ������һ���ź�              */
}
/*********************************************************************************************************
** Function name:           signal_finish
** Descriptions:            ���һ���ź�
** input parameters:        task                ������ƿ�
**                          ctx                 �ź�������
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void signal_finish(task_t *task, signal_ctx_t *ctx)
{
    if (ctx->sig < SIGRTMIN) {                                          /*  ���ɿ��ź�                  */
        sigdelset(&task->sigset_pend, ctx->sig);                        /*  ������źŵĵȴ���־        */
    } else {
        int i;
        int sig_nr = mqueue_msg_nr((mqueue_t *)&task->signal_queue);    /*  �������źŵ���Ŀ            */
        int find   = FALSE;

        for (i = 0; i < sig_nr; i++) {                                  /*  ���ÿһ���ź�              */
            signal_ctx_t *tmp;

            if (mqueue_tryfetch((mqueue_t *)&task->signal_queue, (void **)&tmp) == 0) {
                if (tmp != ctx) {                                       /*  ���ǽ�Ҫ��ɵ��ź�          */
                    if (tmp->sig == ctx->sig) {                         /*  �ҵ�ͬ���͵��ź�            */
                        find = TRUE;
                    }
                }
                mqueue_trypost((mqueue_t *)&task->signal_queue, (void *)tmp);
            }
        }
        if (!find) {                                                    /*  ������û��ͬ���͵��ź�      */
            sigdelset(&task->sigset_pend, ctx->sig);                    /*  ������źŵĵȴ���־        */
        }
    }

    task->signal_current = NULL;
}
/*********************************************************************************************************
** Function name:           signal_print_regs
** Descriptions:            ��ӡ�Ĵ���
** input parameters:        regs                �Ĵ�����
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
** Descriptions:            �źŵ��Ȳ�������һ���źŻ�ص�����ϵ�
** input parameters:        task                ������ƿ�
**                          ctx                 ��ǰ�ź�������
**                          sigint              �Ƿ��յ��� SIGINT �ź�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void signal_next(task_t *task, signal_ctx_t *ctx, int sigint)
{
    signal_ctx_t   *tmp;
    int             i;
    int             sig_nr;

    sig_nr = mqueue_msg_nr((mqueue_t *)&task->signal_queue);            /*  �������źŵ���Ŀ            */

    for (i = 0; i < sig_nr; i++) {                                      /*  ���źŶ����ｫ�ź�ɾ��      */
        if (mqueue_tryfetch((mqueue_t *)&task->signal_queue, (void **)&tmp) == 0) {
            if (tmp == ctx) {
                continue;
            }
            mqueue_trypost((mqueue_t *)&task->signal_queue, (void *)tmp);
        }
    }

    if (sig_nr > 1 && signal_schedule(task, ctx)) {                     /*  �źŵ���                    */

        signal_ctx_t *new_ctx = task->signal_ctx;                       /*  ��һ���ź�                  */

        task->signal_current = new_ctx;                                 /*  �޸ĵ�ǰ�ź�                */

        memcpy(new_ctx->regs, ctx->regs, sizeof(new_ctx->regs));        /*  ת�Ƶ����źŵı��ݼĴ�����  */

        /*
         * ��ʼ�����źŵ�������
         */
        task->regs[0]   = (uint32_t)&new_ctx->kstack[SIG_KERN_STACK_SIZE];  /*  SVC ģʽ�� SP           */
        task->regs[1]   = ARM_SYS_MODE | ARM_FIQ_EN | ARM_IRQ_EN;       /*  CPSR, SYS ģʽ              */
        task->regs[2]   = (uint32_t)&new_ctx->stack[SIG_USR_STACK_SIZE];    /*  SYS ģʽ�� SP           */
        task->regs[3]   = ARM_SVC_MODE;                                 /*  SPSR, SVC ģʽ              */
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

        kfree(ctx);                                                     /*  �ͷ��ź�������              */

        extern void signal_handler_exit(uint32_t *from, uint32_t *to);
        signal_handler_exit(NULL, task->regs);                          /*  �źŴ����˳�                */
    } else {
        /*
         * �ص�����ϵ�
         */
        task->signal_current  = NULL;                                   /*  ��ǰ�ź���Ч                */

        if (sigint) {                                                   /*  �յ��� SIGINT �ź�          */
            task->status      = TASK_RUNNING;
            task->delay       = 0;
            task->resume_type = TASK_RESUME_INTERRUPT;                  /*  ϵͳ���ñ��ж�              */
        } else {
            task->status      = task->signal_status;
            task->delay       = task->signal_delay;
            task->resume_type = TASK_RESUME_AGAIN;                      /*  ����ʱӦ������ϵͳ����      */
        }

        memcpy(task->regs, ctx->regs, sizeof(task->regs));              /*  �ָ�����ϵ�������          */

        kfree(ctx);                                                     /*  �ͷ��ź�������              */

        if (task->status == TASK_RUNNING) {
            extern void signal_handler_exit(uint32_t *from, uint32_t *to);
            signal_handler_exit(NULL, task->regs);                      /*  �źŴ����˳�                */
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
** Descriptions:            �źŵ���
** input parameters:        task                ������ƿ�
**                          ctx                 �ź�������
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

    task->signal_ctx = NULL;                                            /*  Ĭ�ϲ���Ҫ�л������ź�      */

    sig_nr = mqueue_msg_nr((mqueue_t *)&task->signal_queue);            /*  �������źŵ���Ŀ            */
    if (sig_nr == 0) {                                                  /*  �Ż����źŵ����            */
        return FALSE;
    }

    sigset_mask = task->sigset_mask;                                    /*  ȫ�ֵ��ź���������          */

    if (ctx == NULL && task->signal_current != NULL) {                  /*  ����������ڵ�ǰ�ź��˳�    */
                                                                        /*  ����ǰ�����źŵ���          */
                                                                        /*  ���뵱ǰ�źŵ��ź���������  */
        sigset_mask |= task->signal_action[task->signal_current->sig].sa_mask;

        sigaddset(&sigset_mask, task->signal_current->sig);             /*  ��Ҫ���ε�ǰ�ź�            */
    }

    sigset_pend = task->sigset_pend & ~sigset_mask;                     /*  �����δ���ź��б�          */

    if (sigset_pend != 0) {                                             /*  �������δ���ź�            */

        if (ctx != NULL) {                                              /*  ��������ڵ�ǰ�ź��˳�      */
            /*
             * ����ǰ�ź��жϵ��źŽ��ź� A
             *
             * ���ڵ�ǰ�ź��˳�, �������жԵ�ǰ�ź��жϵ��źŶ�����Ϊ���ź� A ��������ж�
             *
             * ������������������ɵ�
             */
            for (i = 0; i < sig_nr; i++) {                              /*  ���ÿһ���ź�              */
                if (mqueue_tryfetch((mqueue_t *)&task->signal_queue, (void **)&tmp) == 0) {
                    if (tmp->sig_int == ctx) {
                        tmp->sig_int = ctx->sig_int;
                    }
                    mqueue_trypost((mqueue_t *)&task->signal_queue, (void *)tmp);
                }
            }

            /*
             * ctx->sig_int != NULL Ӧ�õ��ȳ��ж����ź� A �������ź�, ����ص��ź� A
             */
            /*
             * ctx->sig_int == NULL Ӧ�õ��ȳ��ж�������������ź�, ����ص�����
             */
            ctx = ctx->sig_int;
        } else {
            /*
             * task->signal_current != NULL Ӧ�õ��ȳ��ж��˵�ǰ�źŵ������ź�, ����ص���ǰ�ź�
             */
            /*
             * task->signal_current == NULL Ӧ�õ��ȳ��ж�������������ź�, ����ص�����
             */
            ctx = task->signal_current;
        }

        /*
         * �����Ǻϲ�����źŵ��ȴ���
         */

        for (i = 0; i < sig_nr; i++) {                                  /*  ���ÿһ���ź�              */
            if (mqueue_tryfetch((mqueue_t *)&task->signal_queue, (void **)&tmp) == 0) {
                if (tmp->ref != 0) {                                    /*  δ���ź�                    */
                    if (task->signal_ctx == NULL && sigismember(&sigset_pend, tmp->sig)) {
                        /*
                         * δ���ȳ���һ���źŲ��Ҹ�δ���ź��������
                         */
                        if (tmp->sig_int == ctx) {
                            task->signal_ctx = tmp;                     /*  ��һ���ź�                  */
                            tmp->ref--;                                 /*  ���ź��Ѿ�                  */
                        }
                    }
                }
                mqueue_trypost((mqueue_t *)&task->signal_queue, (void *)tmp);
            }
        }

        if (task->signal_ctx == NULL) {
            task->signal_ctx = ctx;
        }

        if (task->signal_ctx != NULL) {                                 /*  ���ȳ���һ���ź�            */
            if (task->sigset_old != (sigset_t)-1) {                     /*  �оɵ�ȫ���ź���������      */
                task->sigset_mask = task->sigset_old;                   /*  �ָ��ɵ�ȫ���ź���������    */
                sigfillset(&task->sigset_old);
            }
            return TRUE;
        }
    }

    return FALSE;
}
/*********************************************************************************************************
** Function name:           signal_switch
** Descriptions:            �źŴ������л�
** input parameters:        task                ������ƿ�
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void signal_switch(task_t *task)
{
    signal_ctx_t *ctx = task->signal_ctx;

    if (ctx != NULL) {                                                  /*  �����Ҫ�л������ź�        */

        task->signal_ctx     = NULL;

        task->signal_current = ctx;                                     /*  ��ǰ�ź�                    */

        /*
         * ����ɵ�������
         */
        memcpy(ctx->regs, task->regs, sizeof(ctx->regs));

        /*
         * ��ʼ�����źŵ�������
         */
        task->regs[0]   = (uint32_t)&ctx->kstack[SIG_KERN_STACK_SIZE];  /*  SVC ģʽ�� SP (����ջ�ݼ�)  */
        task->regs[1]   = ARM_SYS_MODE | ARM_FIQ_EN | ARM_IRQ_EN;       /*  CPSR, SYS ģʽ              */
        task->regs[2]   = (uint32_t)&ctx->stack[SIG_USR_STACK_SIZE];    /*  SYS ģʽ�� SP (����ջ�ݼ�)  */
        task->regs[3]   = ARM_SVC_MODE;                                 /*  SPSR, SVC ģʽ              */
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
** Descriptions:            ��ʼ��������ź�ϵͳ
** input parameters:        task                ������ƿ�
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
** Descriptions:            ����������ź�ϵͳ
** input parameters:        task                ������ƿ�
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
