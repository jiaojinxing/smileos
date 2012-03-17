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
** File name:               kern.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            �ں�Դ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/arm.h"
#include "kern/vmm.h"
#include "kern/heap.h"
#include <string.h>
#include <stdarg.h>
/*********************************************************************************************************
  �ں˱���
*********************************************************************************************************/
task_t          tasks[TASK_NR];                                         /*  ������ƿ�                  */
task_t         *current;                                                /*  ָ��ǰ���е�����          */
static uint64_t tick;                                                   /*  TICK                        */
static uint8_t  interrupt_nest;                                         /*  �ж�Ƕ�ײ��                */
static uint8_t  running;                                                /*  �ں��Ƿ���������            */
/*********************************************************************************************************
  �ں˺���
*********************************************************************************************************/
/*
 * ��ʼ���ں˱���
 */
static void kern_vars_init(void)
{
    task_t *task;
    int     i;

    running        = FALSE;                                             /*  �ں˻�û����                */
    interrupt_nest = 0;                                                 /*  �ж�Ƕ�ײ��Ϊ 0            */
    tick           = 0;                                                 /*  TICK Ϊ 0                   */
    current        = &tasks[0];                                         /*  ��ǰ����Ϊ���� 0            */

    /*
     * ��ʼ�����е�������ƿ�
     */
    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
        task->pid          = -1;
        task->tid          = -1;
        task->state        = TASK_UNALLOCATE;
        task->counter      = 0;
        task->timer        = 0;
        task->priority     = 0;
        task->type         = TASK_TYPE_PROCESS;
        task->errno        = 0;
        task->resume_type  = TASK_RESUME_UNKNOW;
        task->next         = NULL;
        task->wait_list    = NULL;
        task->frame_list   = NULL;
        task->tick         = 0;
        task->utilization  = 0;
        task->frame_nr     = 0;
        task->stack        = 0;
        task->thread       = NULL;
        task->arg          = NULL;
        memset(task->name, 0, sizeof(task->name));
    }
}

/*
 * �������н���
 */
static void idle_process_create(void)
{
    task_t *task;

    /*
     * ��ʼ�����̿��ƿ� 0
     */
    task               = &tasks[0];
    task->pid          = 0;
    task->tid          = 0;
    task->state        = TASK_RUNNING;
    task->counter      = 20;
    task->timer        = 0;
    task->priority     = 20;
    task->type         = TASK_TYPE_PROCESS;                             /*  ��������Ϊ����              */
    task->errno        = 0;
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->utilization  = 0;
    task->frame_nr     = 0;
    task->stack        = 0;
    task->thread       = NULL;
    task->arg          = NULL;

    /*
     * ��ʼ������������
     */
    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];      /*  svc ģʽ�� sp (����ջ�ݼ�)  */
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;        /*  cpsr, sys ģʽ, �� IRQ      */
    task->content[2]   = PROCESS0_STACK_BASE;                           /*  sys ģʽ�� sp               */
    task->content[3]   = ARM_SVC_MODE;                                  /*  spsr, svc ģʽ              */
    task->content[4]   = 0;                                             /*  lr                          */
    task->content[5]   = 0;                                             /*  r0 ~ r12                    */
    task->content[6]   = 1;
    task->content[7]   = 2;
    task->content[8]   = 3;
    task->content[9]   = 4;
    task->content[10]  = 5;
    task->content[11]  = 6;
    task->content[12]  = 7;
    task->content[13]  = 8;
    task->content[14]  = 9;
    task->content[15]  = 10;
    task->content[16]  = 11;
    task->content[17]  = 12;
    task->content[18]  = 0;                                             /*  pc                          */

    strcpy(task->name, "idle");
}

/*
 * ��ʼ���ں�
 */
void kernel_init(void)
{
    kern_vars_init();                                                   /*  ��ʼ���ں˱���              */

    extern void kern_heap_create(void);
    kern_heap_create();                                                 /*  �����ں��ڴ��              */

    idle_process_create();                                              /*  �������н���                */

    vmm_init();                                                         /*  ��ʼ�������ڴ����          */
}

/*
 * �����ں�
 */
void kernel_start(void)
{
    running = TRUE;                                                     /*  �ں��Ѿ�����                */

    extern void __switch_to_process0(register uint32_t sp_svc);

    __switch_to_process0(current->content[0]);                          /*  �л������� 0                */
}

/*
 * �������
 * ����֮ǰ������ж�
 */
void schedule(void)
{
    int     i;
    int     next = 0;
    int     flag = FALSE;
    int32_t max  = -1;
    task_t *task;

    if (!running) {                                                     /*  ����ں˻�û����            */
        return;                                                         /*  ֱ�ӷ���                    */
    }

    if (interrupt_nest > 0) {                                           /*  �����û��ȫ�˳��ж�        */
        return;                                                         /*  ֱ�ӷ���                    */
    }

    while (1) {
        /*
         * �����̵߳���, �������̵���
         */
        for (i = PROCESS_NR, task = tasks + PROCESS_NR; i < TASK_NR; i++, task++) {
            if ((task->state == TASK_RUNNING) && (max < (int32_t)task->counter)) {
                max  = (int32_t)task->counter;                          /*  ��ʣ��ʱ��Ƭ��������        */
                next = i;
            }
        }

        if (max > 0) {                                                  /*  �ҵ���һ����ʣ��ʱ��Ƭ���߳�*/
            break;
        }

        for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {    /*  ���� 0 �����뾺��           */
            if ((task->state == TASK_RUNNING) && (max < (int32_t)task->counter)) {
                max  = (int32_t)task->counter;                          /*  ��ʣ��ʱ��Ƭ��������        */
                next = i;
            }
        }

        if (max > 0) {                                                  /*  �ҵ���һ����ʣ��ʱ��Ƭ�Ľ���*/
            break;
        } else if (flag) {
            next = 0;
            break;
        }

        for (i = 0, task = tasks; i < TASK_NR; i++, task++) {           /*  �������������ʣ��ʱ��Ƭ    */
            task->counter = task->priority;
        }

        flag = TRUE;
    }

    if (current == &tasks[next]) {                                      /*  �������Ҫ�л�����          */
        return;                                                         /*  ֱ�ӷ���                    */
    }

    task    = current;                                                  /*  �ݴ� current ָ��           */
    current = &tasks[next];                                             /*  ��д current ָ��           */

    if ((current->content[3] & ARM_MODE_MASK) == ARM_SVC_MODE) {        /*  ����������������ں�ջָ��  */
        current->content[0] = (uint32_t)&current->kstack[KERN_STACK_SIZE];
    }

    extern void __switch_to(register task_t *from, register task_t *to);
    __switch_to(task, current);                                         /*  �����л�                    */
}

/*
 * �ں˶�ʱ����������
 */
void do_timer(void)
{
    int      i;
    task_t  *task;
    uint32_t reg;
    int      flag = 0;

    reg = interrupt_disable();

    current->tick++;                                                    /*  ��ǰ�����жϵĴ�����һ    */

    tick++;                                                             /*  �ں� TICK ��һ              */
    if (tick % TICK_PER_SECOND == 0) {                                  /*  ����Ѿ���ȥ��һ��          */
        flag = 1;
    }

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {               /*  ������������                */
        if (task->state != TASK_UNALLOCATE) {                           /*  ���������Ч                */
            if (flag) {
                task->utilization = task->tick;                         /*  ����� CPU ռ����           */
                task->tick        = 0;                                  /*  ���ø����񱻶�ʱ���жϵĴ���*/
            }

            if (task->state == TASK_SLEEPING) {                         /*  �������������˯            */
                task->timer--;                                          /*  ������ʱ��һ                */
                if (task->timer == 0) {                                 /*  ���������ʱ����            */
                    task->state = TASK_RUNNING;                         /*  ����������̬              */
                    if (task->wait_list != NULL) {                      /*  ���������ĳ���ȴ�����      */
                        task_t *prev = *task->wait_list;                /*  ��õȴ���������ͷ����      */
                        if (task == prev) {                             /*  ������������ͷ����        */
                            *task->wait_list = task->next;              /*  �޸ĵȴ���������ͷ          */
                        } else {
                            while (prev != NULL && prev->next != task) {/*  �ҳ������ڵȴ������е�ǰ��  */
                                prev = prev->next;
                            }
                            if (prev != NULL) {                         /*  ������ҵ�                  */
                                prev->next = task->next;                /*  �޸�ǰ���ĺ���              */
                            }
                        }
                        task->next        = NULL;
                        task->wait_list   = NULL;
                        task->resume_type = TASK_RESUME_TIMEOUT;        /*  ��������Ļָ�����Ϊ��ʱ    */
                    }
                }
            }
        }
    }

    if (current->counter > 0) {                                         /*  �����ǰ������ʣ��ʱ��Ƭ  */
        current->counter--;                                             /*  ��ǰ�����ʣ��ʱ��Ƭ��һ    */
    }

    interrupt_resume(reg);
}

/*
 * �����ж�
 */
void interrupt_enter(void)
{
    uint32_t reg;

    reg = interrupt_disable();

    interrupt_nest++;                                                   /*  �ж�Ƕ�ײ�μ�һ            */

    interrupt_resume(reg);
}

/*
 * �˳��ж�
 */
void interrupt_exit(void)
{
    uint32_t reg;

    reg = interrupt_disable();

    interrupt_nest--;                                                   /*  �ж�Ƕ�ײ�μ�һ            */
    if (interrupt_nest == 0) {                                          /*  ����Ѿ���ȫ�˳����ж�      */
        schedule();                                                     /*  �������                    */
    }
    interrupt_resume(reg);
}


/*
 * �ж������ַ�ռ��Ƿ����
 */
int virtual_space_usable(uint32_t base, uint32_t size)
{
    uint32_t end = base + size;
    uint32_t high, low;
    int i;

    extern virtual_space_t sys_resv_space[];
    extern virtual_space_t bsp_resv_space[];

#define max(a, b)   (a) > (b) ? (a) : (b)
#define min(a, b)   (a) < (b) ? (a) : (b)

    for (i = 0; sys_resv_space[i].size != 0; i++) {
        high = max(sys_resv_space[i].base, base);
        low  = min(sys_resv_space[i].base + sys_resv_space[i].size, end);
        if (high < low) {
            return FALSE;
        }
    }

    for (i = 0; bsp_resv_space[i].size != 0; i++) {
        high = max(bsp_resv_space[i].base, base);
        low  = min(bsp_resv_space[i].base + bsp_resv_space[i].size, end);
        if (high < low) {
            return FALSE;
        }
    }

    return TRUE;
}

/*
 * ��������
 */
int32_t process_create(const char *name, uint8_t *code, uint32_t size, uint32_t priority)
{
    int      i;
    task_t  *task;
    uint32_t reg;

    if (code == NULL || size == 0 || priority == 0) {
        return -1;
    }

    reg = interrupt_disable();

    /*
     * ������̿��ƿ�
     */
    for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {        /*  �������еĽ��̿��ƿ�        */
        if (task->state == TASK_UNALLOCATE) {                           /*  ������̿��ƿ���Ч          */
                                                                        /*  ������̵������ַ�ռ����  */
            if (virtual_space_usable(i * PROCESS_SPACE_SIZE, PROCESS_SPACE_SIZE)) {
                break;
            }
        }
    }

    if (i == PROCESS_NR) {                                              /*  û�п��еĽ��̿��ƿ�        */
        interrupt_resume(reg);
        return -1;
    }

    /*
     * ��ʼ�����̿��ƿ�
     */
    task->pid          = i;
    task->tid          = i;
    task->state        = TASK_RUNNING;
    task->counter      = priority;
    task->timer        = 0;
    task->priority     = priority;
    task->type         = TASK_TYPE_PROCESS;                             /*  ��������Ϊ����              */
    task->errno        = 0;
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->utilization  = 0;
    task->frame_nr     = 0;
    task->stack        = 0;
    task->thread       = NULL;
    task->arg          = NULL;

    /*
     * ��ʼ������������
     */
    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];      /*  svc ģʽ�� sp (����ջ�ݼ�)  */
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;        /*  cpsr, sys ģʽ, �� IRQ      */
    task->content[2]   = PROCESS_SPACE_SIZE;                            /*  sys ģʽ�� sp               */
    task->content[3]   = ARM_SVC_MODE;                                  /*  spsr, svc ģʽ              */
    task->content[4]   = 0;                                             /*  lr                          */
    task->content[5]   = 0;                                             /*  r0 ~ r12                    */
    task->content[6]   = 1;
    task->content[7]   = 2;
    task->content[8]   = 3;
    task->content[9]   = 4;
    task->content[10]  = 5;
    task->content[11]  = 6;
    task->content[12]  = 7;
    task->content[13]  = 8;
    task->content[14]  = 9;
    task->content[15]  = 10;
    task->content[16]  = 11;
    task->content[17]  = 12;
    task->content[18]  = 0;                                             /*  pc                          */

    if (name != NULL) {
        strcpy(task->name, name);
    } else {
        strcpy(task->name, "???");
    }

    /*
     * Ϊ�������뵽���̵������ַ�ռ�, Ԥ��ӳ���ҳ��
     */
    for (i = 0; i < (size + PAGE_SIZE - 1) / PAGE_SIZE; i++) {
        if (vmm_map_process_page(task, task->pid * PROCESS_SPACE_SIZE + i * PAGE_SIZE) < 0) {
            vmm_free_process_space(task);
            task->state = TASK_UNALLOCATE;
            interrupt_resume(reg);
            return -1;
        }
    }

    /*
     * Ϊ����ջ�ռ�ӳ��һ��ҳ��
     */
    if (vmm_map_process_page(task, (task->pid + 1) * PROCESS_SPACE_SIZE - PAGE_SIZE) < 0) {
        vmm_free_process_space(task);
        task->state = TASK_UNALLOCATE;
        interrupt_resume(reg);
        return -1;
    }

    /*
     * �������뵽���̵������ַ�ռ�
     */
    memcpy((char *)(task->pid * PROCESS_SPACE_SIZE), code, size);

    interrupt_resume(reg);

    return i;
}

/*
 * �ں��߳����
 */
static void kthread_shell(task_t *task)
{
    task->thread(task->arg);

    extern void exit(int error);
    exit(0);
}

/*
 * �����ں��߳�
 */
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stk_size, uint32_t priority)
{
    int      i;
    task_t  *task;
    uint32_t reg;

    if (func == NULL) {
        return -1;
    }

    reg = interrupt_disable();
                                                                        /*  �������е��߳̿��ƿ�        */
    for (i = PROCESS_NR, task = tasks + PROCESS_NR; i < TASK_NR; i++, task++) {
        if (task->state == TASK_UNALLOCATE) {                           /*  ����߳̿��ƿ���Ч          */
            break;
        }
    }

    if (i == TASK_NR) {                                                 /*  û�п��е��߳̿��ƿ�        */
        interrupt_resume(reg);
        return -1;
    }

    if (priority < 2) {                                                 /*  ���ȼ���СΪ 2              */
        priority = 2;
    }

    if (stk_size < 8 * KB) {                                            /*  ��ջ�ռ���СΪ 8 KB         */
        stk_size = 8 * KB;
    }
    stk_size = MEM_ALIGN_SIZE(stk_size);                                /*  �����ջ�ռ��С            */

    task->stack = (uint32_t)kmalloc(stk_size);                          /*  �����ջ�ռ�                */
    if (task->stack == 0) {
        interrupt_resume(reg);
        return -1;
    }

    task->pid          = current->pid;                                  /*  ���� ID Ϊ��ǰ����Ľ��� ID */
    task->tid          = i;
    task->state        = TASK_RUNNING;
    task->counter      = priority;
    task->timer        = 0;
    task->priority     = priority;
    task->type         = TASK_TYPE_THREAD;                              /*  ��������Ϊ�߳�              */
    task->errno        = 0;
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->utilization  = 0;
    task->frame_nr     = 0;
    task->thread       = func;
    task->arg          = arg;

    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];      /*  svc ģʽ��ջָ��(����ջ�ݼ�)*/
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;        /*  cpsr, sys ģʽ, �� IRQ      */
    task->content[2]   = task->stack + stk_size;                        /*  sys ģʽ�� sp               */
    task->content[3]   = ARM_SVC_MODE;                                  /*  spsr, svc ģʽ              */
    task->content[4]   = (uint32_t)kthread_shell;                       /*  lr                          */
    task->content[5]   = (uint32_t)task;                                /*  r0 ~ r12                    */
    task->content[6]   = 1;
    task->content[7]   = 2;
    task->content[8]   = 3;
    task->content[9]   = 4;
    task->content[10]  = 5;
    task->content[11]  = 6;
    task->content[12]  = 7;
    task->content[13]  = 8;
    task->content[14]  = 9;
    task->content[15]  = 10;
    task->content[16]  = 11;
    task->content[17]  = 12;
    task->content[18]  = (uint32_t)kthread_shell;                       /*  pc                          */

    if (name != NULL) {
        strcpy(task->name, name);
    } else {
        strcpy(task->name, "???");
    }

    interrupt_resume(reg);

    return i;
}

/*
 * ��� TICK
 */
uint64_t get_tick(void)
{
    uint64_t __tick;
    uint32_t reg;

    reg = interrupt_disable();

    __tick = tick;

    interrupt_resume(reg);

    return __tick;
}

/*
 * �ж��Ƿ����жϴ�����
 */
int in_interrupt(void)
{
    int      ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = interrupt_nest > 0 ? TRUE : FALSE;

    interrupt_resume(reg);

    return ret;
}

#include <stdio.h>

/*
 * printk
 */
void printk(const char *fmt, ...)
{
    va_list va;
    char    buf[256];
    int     i;

    va_start(va, fmt);

    vsprintf(buf, fmt, va);

    i = 0;
    while (buf[i]) {
        kputc(buf[i]);
        i++;
    }

    va_end(va);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/