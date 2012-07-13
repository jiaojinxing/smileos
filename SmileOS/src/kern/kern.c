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
** Modified by:             JiaoJinXing
** Modified date:           2012-3-25
** Version:                 1.1.0
** Descriptions:            �����ں�ģʽ���ں���־�̼߳� interrupt_exit_no_sched ����,
**                          strcpy ���ø���ȫ�� strlcpy,
**                          �������뵽���̵������ַ�ռ�ʱ���ж������ʵʱ��
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-26
** Version:                 1.2.0
** Descriptions:            ������������ַ�ռ�ı������ں��߳�ջ������
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-28
** Version:                 1.3.0
** Descriptions:            �� newlib ����, ������ƿ����� struct _reent ��Ա,
**                          �����л�ʱҲ�л� _impure_ptr ָ��
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-31
** Version:                 1.4.0
** Descriptions:            �޸���������㷨��һ������: �������о��������ʣ��ʱ��Ƭ��������������,
**                          �ں��̵߳ĵ����㷨��Ϊ�������ȼ�����ʣ��ʱ��Ƭ,
**                          Ϊ�����жϵװ벿����ʱ������, ����жϵװ벿�����̵߳����ȼ�̧���ϸ�ˮƽ
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/arm.h"
#include "kern/vmm.h"
#include "kern/mmu.h"
#include "vfs/vfs.h"
#include <string.h>
/*********************************************************************************************************
  �ں˱���
*********************************************************************************************************/
task_t                  tasks[TASK_NR];                                 /*  ������ƿ�                  */
task_t                 *current;                                        /*  ָ��ǰ���е�����          */
static uint64_t         tick;                                           /*  TICK                        */
static uint8_t          interrupt_nest;                                 /*  �ж�Ƕ�ײ��                */
static uint8_t          running;                                        /*  �ں��Ƿ���������            */
static struct _reent    reents[KTHREAD_NR + 1];

/*
 * logo
 */
const char smileos_logo[] =
" [[[[                       [[[[   [[[[   \n"
"[    [                     [    [ [    [  \n"
"[             @ [     [[[  [    [ [       \n"
" [[[[  [[  [[   [    [   [ [    [  [[[[   \n"
"     [ [ [[ [ [ [    [[[[  [    [      [  \n"
"[    [ [    [ [ [    [     [    [ [    [  \n"
" [[[[  [    [ [ [[[   [[[[  [[[[   [[[[   \n";

#define KTHREAD_STACK_MAGIC0        0xAA                                /*  �ں��߳�ջħ��              */

static void idle_create(void);
/*********************************************************************************************************
  �ں˺���
*********************************************************************************************************/
/*
 * ��ʼ���ں˱���
 */
static void kvars_init(void)
{
    task_t *task;
    int     i;

    running        = FALSE;                                             /*  �ں˻�û����                */
    interrupt_nest = 0;                                                 /*  �ж�Ƕ�ײ��Ϊ 0            */
    tick           = 0;                                                 /*  TICK Ϊ 0                   */
    current        = &tasks[0];                                         /*  ��ǰ����Ϊ���� 0            */
    _impure_ptr    = &reents[0];

    /*
     * ��ʼ�����е�������ƿ�
     */
    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
        memset(task, 0, sizeof(task_t));
        task->state = TASK_UNALLOCATE;
    }
}

/*
 * ��ʼ���ں�
 */
void kernel_init(void)
{
    mmu_init();                                                         /*  ��ʼ�� MMU                  */

    kvars_init();                                                       /*  ��ʼ���ں˱���              */

    extern void kheap_create(void);
    kheap_create();                                                     /*  �����ں��ڴ��              */

    idle_create();                                                      /*  �������н���                */

    extern void klogd_create(void);
    klogd_create();                                                     /*  �����ں���־�߳�            */

    vmm_init();                                                         /*  ��ʼ�������ڴ����          */
}

/*
 * �����ں�
 */
void kernel_start(void)
{
    running = TRUE;                                                     /*  �ں��Ѿ�����                */

    extern void __switch_to_process0(register uint32_t sp_svc);
    __switch_to_process0(current->content[0]);                          /*  �л������� 0 (���н���)     */
}

/*
 * �����л�
 */
static void task_switch(task_t *from, task_t *to)
{
    int     i;
    int     j;
    task_t *task;

    if (to->pid == 0) {                                                 /*  ���׼�������ں��߳�        */
        if (from->pid != 0) {                                           /*  ����ϴ����е��ǽ���        */
            /*
             * ����������Ч���̵������ַ�ռ�
             */
            for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {/*  �������еĽ��̿��ƿ�        */
                if (task->state != TASK_UNALLOCATE) {                   /*  ���������Ч                */
                    for (j = 0; j < PROCESS_SPACE_SIZE / SECTION_SIZE; j++) {
                        mmu_map_section(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + j, task->mmu_backup[j]);
                    }
                }
            }
            mmu_invalidate_itlb_dtlb();
        } else {
            /*
             * �ϴ����е����ں��߳�, DO NOTHING
             */
        }
    } else {
        /*
         * ׼�������½���
         */
        if (from->pid == 0) {                                           /*  ����ϴ����е����ں��߳�    */
            /*
             * ����������Ч���̵������ַ�ռ�
             */
            for (i = 1, task = tasks + 1; i < PROCESS_NR; i++, task++) {/*  �������еĽ��̿��ƿ�        */
                if (task != to && task->state != TASK_UNALLOCATE) {     /*  ���������Ч                */
                    for (j = 0; j < PROCESS_SPACE_SIZE / SECTION_SIZE; j++) {
                        mmu_map_section(task->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + j, 0);
                    }
                }
            }
            mmu_invalidate_itlb_dtlb();
        } else {
            /*
             * �ϴ����е�Ҳ�ǽ���
             */
            /*
             * ������һ�����̵������ַ�ռ�
             */
            for (j = 0; j < PROCESS_SPACE_SIZE / SECTION_SIZE; j++) {
                mmu_map_section(from->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + j, 0);
            }

            /*
             * �����½��̵������ַ�ռ�
             */
            for (j = 0; j < PROCESS_SPACE_SIZE / SECTION_SIZE; j++) {
                mmu_map_section(to->pid * PROCESS_SPACE_SIZE / SECTION_SIZE + j, to->mmu_backup[j]);
            }
            mmu_invalidate_itlb_dtlb();
        }
    }

    extern void __switch_to(register task_t *from, register task_t *to);
    __switch_to(from, to);                                              /*  �����л�                    */
}

/*
 * �������
 * ����֮ǰ������ж�
 */
void task_schedule(void)
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
         * �����ں��̵߳���, �������̵���
         */
        for (i = PROCESS_NR, task = tasks + PROCESS_NR; i < TASK_NR; i++, task++) {
            if ((task->state == TASK_RUNNING) && (max < (int32_t)task->priority)) {
                max  = (int32_t)task->priority;                         /*  �����ȼ���������            */
                next = i;
            }
        }

        if (max > 0) {                                                  /*  �ҵ���һ���������ں��߳�    */
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
        } else if (flag) {                                              /*  ���û��һ���������        */
            next = 0;                                                   /*  �����п��н���              */
            break;
        }

        for (i = 0, task = tasks; i < TASK_NR; i++, task++) {           /*  �������о��������ʣ��ʱ��Ƭ*/
            if (task->state == TASK_RUNNING) {
                task->counter = task->priority;
            }
        }

        flag = TRUE;
    }

    if (current == &tasks[next]) {                                      /*  �������Ҫ�л�����          */
        return;                                                         /*  ֱ�ӷ���                    */
    }

    task        = current;                                              /*  �ݴ� current ָ��           */
    current     = &tasks[next];                                         /*  ��д current ָ��           */
    _impure_ptr = current->reent;                                       /*  ��д _impure_ptr ָ��       */

    if ((current->content[3] & ARM_MODE_MASK) == ARM_SVC_MODE) {        /*  ����������������ں�ջָ��  */
        current->content[0] = (uint32_t)&current->kstack[KERN_STACK_SIZE];
    }

    task_switch(task, current);                                         /*  �����л�                    */
}

/*
 * �ں˶�ʱ��������
 */
void kernel_timer(void)
{
    int      i;
    int      flag;
    task_t  *task;
    uint32_t reg;

    reg = interrupt_disable();

    current->tick++;                                                    /*  ��ǰ�����жϵĴ�����һ    */

    tick++;                                                             /*  �ں� TICK ��һ              */
    if (tick % TICK_PER_SECOND == 0) {                                  /*  ����Ѿ���ȥ��һ��          */
        flag = TRUE;
    } else {
        flag = FALSE;
    }

    if (current->counter > 0) {                                         /*  �����ǰ������ʣ��ʱ��Ƭ  */
        current->counter--;                                             /*  ��ǰ�����ʣ��ʱ��Ƭ��һ    */
    }

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {               /*  ������������                */
        if (task->state != TASK_UNALLOCATE) {                           /*  ���������Ч                */
            /*
             * TODO: ����ͳ������� CPU ռ���ʲ��Ǻ�׼ȷ
             */
            if (flag) {
                task->cpu_rate = task->tick;                            /*  ����� CPU ռ����           */
                task->tick     = 0;                                     /*  ���ø����񱻶�ʱ���жϵĴ���*/

                if (task->type == TASK_TYPE_KTHREAD) {                  /*  ����������ں��߳�          */
                    /*
                     * ͳ���ں��̵߳�ջʹ����, ջ������
                     *
                     * ע��: ������˱������ -fsigned-char, char ���з��ŵ�!
                     *
                     * THREAD_STACK_MAGIC0 = 0xAA, ����Ҫ�� uint8_t ����, ����...
                     */
                    uint8_t *pos = (uint8_t *)task->stack_base;
                    uint8_t *end = (uint8_t *)(task->stack_base + task->stack_size);

                    while (*pos == KTHREAD_STACK_MAGIC0 && pos < end) {
                        pos++;
                    }

                    if (pos == (uint8_t *)task->stack_base) {
                        printk("kthread %s tid=%d stack overflow!\n", task->name, task->tid);
                        task_kill(task->tid, SIGSEGV);
                        continue;
                    } else {
                        task->stack_rate = 100 * ((uint32_t)(end - pos)) / task->stack_size;
                    }
                }
            }

            if (task->state == TASK_SLEEPING) {                         /*  �������������˯            */
                task->timer--;                                          /*  ������ʱ��һ                */
                if (task->timer == 0) {                                 /*  ���������ʱ����            */
                    task->state       = TASK_RUNNING;                   /*  ����������̬              */
                    task->resume_type = TASK_RESUME_TIMEOUT;            /*  ��������Ļָ�����Ϊ��ʱ    */
                    if (task->wait_list != NULL) {                      /*  ���������ĳ���ȴ�����      */
                        task_t *prev = *task->wait_list;                /*  ��õȴ��������ͷ����      */
                        if (task == prev) {                             /*  ������������ͷ����        */
                            *task->wait_list = task->next;              /*  �޸ĵȴ��������ͷ          */
                        } else {
                            while (prev != NULL && prev->next != task) {/*  �ҳ������ڵȴ������е�ǰ��  */
                                prev = prev->next;
                            }
                            if (prev != NULL) {                         /*  ������ҵ�                  */
                                prev->next = task->next;                /*  �޸�ǰ���ĺ���              */
                            }
                        }
                        task->next      = NULL;
                        task->wait_list = NULL;
                    }
                }
            }
        }
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

    /*
     * TODO: ���ڸ����쳣���ö�ջ, ������ʱ��֧���ж�Ƕ��
     */
    if (interrupt_nest != 0) {
        kcomplain("interrupt_nest error!\n");
        while (1);
    }

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
        task_schedule();                                                /*  �������                    */
    }
    interrupt_resume(reg);
}

/*
 * �˳��ж�, ����Ҫ�����������
 */
void interrupt_exit_no_sched(void)
{
    uint32_t reg;

    reg = interrupt_disable();

    interrupt_nest--;                                                   /*  �ж�Ƕ�ײ�μ�һ            */

    interrupt_resume(reg);
}

/*
 * �ж������ַ�ռ��Ƿ����
 */
static int vspace_usable(uint32_t base, uint32_t size)
{
    uint32_t end = base + size;
    uint32_t high, low;
    int      i;

    extern space_t sys_resv_space[];
    extern space_t bsp_resv_space[];

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
 * �������н���
 */
static void idle_create(void)
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
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->cpu_rate     = 0;
    task->frame_nr     = 0;
    task->page_tbl_nr  = 0;
    task->stack_base   = 0;
    task->stack_size   = 0;
    task->stack_rate   = 0;
    task->thread       = NULL;
    task->arg          = NULL;
    task->dabt_cnt     = 0;
    task->file_size    = 0;
    task->reent        = &reents[0];
    _REENT_INIT_PTR(task->reent);
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));

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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * ��������
 */
int32_t process_create(const char *path, uint32_t priority)
{
    int32_t  pid;
    task_t  *task;
    uint32_t reg;
    struct stat st;
    int fd;
    ssize_t len;
    size_t total;

    if (path == NULL) {
        return -1;
    }

    if (stat(path, &st) < 0) {
        return -1;
    }

    if (!S_ISREG(st.st_mode)) {
        return -1;
    }

    if (priority < 5) {                                                 /*  ���ȼ���СΪ 5              */
        priority = 5;
    }

    reg = interrupt_disable();

    /*
     * ������̿��ƿ�
     */
    for (pid = 1, task = tasks + 1; pid < PROCESS_NR; pid++, task++) {  /*  �������еĽ��̿��ƿ�        */
        if (task->state == TASK_UNALLOCATE) {                           /*  ������̿��ƿ���Ч          */
                                                                        /*  ������̵������ַ�ռ����  */
            if (vspace_usable(pid * PROCESS_SPACE_SIZE, PROCESS_SPACE_SIZE)) {
                break;
            }
        }
    }

    if (pid == PROCESS_NR) {                                            /*  û�п��еĽ��̿��ƿ�        */
        interrupt_resume(reg);
        return -1;
    }

    /*
     * ��ʼ�����̿��ƿ�
     */
    task->pid          = pid;
    task->tid          = pid;
    task->state        = TASK_SUSPEND;                                  /*  �ȿ����������پ���        */
    task->counter      = priority;
    task->timer        = 0;
    task->priority     = priority;
    task->type         = TASK_TYPE_PROCESS;                             /*  ��������Ϊ����              */
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->cpu_rate     = 0;
    task->frame_nr     = 0;
    task->page_tbl_nr  = 0;
    task->stack_base   = 0;
    task->stack_size   = 0;
    task->stack_rate   = 0;
    task->thread       = NULL;
    task->arg          = NULL;
    task->dabt_cnt     = 0;
    task->file_size    = st.st_size;
    task->reent        = NULL;
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));

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

    if (path != NULL) {
        strlcpy(task->name, path, sizeof(task->name));
    } else {
        strcpy(task->name, "???");
    }

    if (vfs_task_init(pid) < 0) {                                       /*  ��ʼ�����̵��ļ���Ϣ        */
        goto __exit0;
    }

    if (vmm_process_init(task, st.st_size) < 0) {                       /*  ��ʼ�����̵������ڴ���Ϣ    */
        goto __exit1;
    }

    interrupt_resume(reg);

    /*
     * �������뵽���̵������ַ�ռ�
     */
    fd = open(path, O_RDONLY, 0666);
    if (fd < 0) {
        goto __exit2;
    }

    total = 0;

    while (st.st_size - total > 0) {
        len = read(fd, (char *)(pid * PROCESS_SPACE_SIZE) + total, st.st_size - total);
        if (len < 0) {
            close(fd);
            goto __exit2;
        }
        total += len;
    }
    close(fd);

    /*
     * �ղſ����Ĵ��뻹�� D-Cache, ��������ʱ��ȡ I-Cache ������, �����ǲ�һ�µ�!
     * ���Ա���Ҫ��� D-Cache, ���д����, ��Ҫ��Ч I-Cache,
     * ��Ч D-Cache ���ó� D-Cache, �����������̵� D-Cache �������� D-Cache Miss, ���������ٶ�
     *
     * ARM ���ݷ��ʵĻ�������ͼ:
     * http://www.ibm.com/developerworks/cn/linux/l-skyeye/part3/s3/index.html
     */
    {
        int i;

        /*
         * �������Ч D-Cache
         */
        for (i = 0; i < (st.st_size + 31) / 32; i++) {
            mmu_clean_invalidate_dcache_mva(pid * PROCESS_SPACE_SIZE + i * 32);
        }

        /*
         * ���д����
         */
        mmu_drain_write_buffer();

        /*
         * ��Ч I-Cache
         */
        for (i = 0; i < (st.st_size + 31) / 32; i++) {
            mmu_invalidate_icache_mva(pid * PROCESS_SPACE_SIZE + i * 32);
        }
    }

    reg = interrupt_disable();

    task->state = TASK_RUNNING;                                         /*  ���̽������̬              */

    interrupt_resume(reg);

    return pid;

    __exit2:
    vmm_process_cleanup(task);

    __exit1:
    vfs_task_cleanup(pid);

    __exit0:
    task->state = TASK_UNALLOCATE;
    interrupt_resume(reg);
    return -1;
}

/*
 * fork һ���ӽ���
 */
int process_fork(void)
{
    int32_t  pid;
    task_t  *task;

    if (current->type != TASK_TYPE_PROCESS) {
        return -1;
    }

    /*
     * ������̿��ƿ�
     */
    for (pid = 1, task = tasks + 1; pid < PROCESS_NR; pid++, task++) {  /*  �������еĽ��̿��ƿ�        */
        if (task->state == TASK_UNALLOCATE) {                           /*  ������̿��ƿ���Ч          */
                                                                        /*  ������̵������ַ�ռ����  */
            if (vspace_usable(pid * PROCESS_SPACE_SIZE, PROCESS_SPACE_SIZE)) {
                break;
            }
        }
    }

    if (pid == PROCESS_NR) {                                            /*  û�п��еĽ��̿��ƿ�        */
        return -1;
    }

    /*
     * ��ʼ�����̿��ƿ�
     */
    task->pid          = pid;
    task->tid          = pid;
    task->state        = TASK_SUSPEND;                                  /*  �ȿ����������پ���        */
    task->counter      = current->priority;
    task->timer        = 0;
    task->priority     = current->priority;
    task->type         = TASK_TYPE_PROCESS;                             /*  ��������Ϊ����              */
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->cpu_rate     = 0;
    task->frame_nr     = 0;
    task->page_tbl_nr  = 0;
    task->stack_base   = 0;
    task->stack_size   = 0;
    task->stack_rate   = 0;
    task->thread       = NULL;
    task->arg          = NULL;
    task->dabt_cnt     = 0;
    task->file_size    = current->file_size;
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));

    /*
     * ��ʼ������������
     */
    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];      /*  svc ģʽ�� sp (����ջ�ݼ�)  */
    extern uint32_t get_spsr(void);
    task->content[1]   = get_spsr();                                    /*  cpsr, sys ģʽ, �� IRQ      */
    extern void get_sys_sp(uint32_t *sp);
    get_sys_sp(&task->content[2]);                                      /*  sys ģʽ�� sp               */
    task->content[3]   = ARM_SVC_MODE;                                  /*  spsr, svc ģʽ              */
    task->content[4]   = 0;                                             /*  lr, ��������Ϊ��Чֵ, ��Ϊ  */
                                                                        /*  ��ϵͳ����ǰ�ѱ��浽����ջ  */
    task->content[5]   = 0;                                             /*  r0 ~ r12                    */
    task->content[6]   = current->kstack[KERN_STACK_SIZE - 1];          /*  r1 ���ڴ��� pc              */
    task->content[7]   = 2;
    task->content[8]   = 3;
    task->content[9]   = current->kstack[KERN_STACK_SIZE - 10];
    task->content[10]  = current->kstack[KERN_STACK_SIZE - 9];
    task->content[11]  = current->kstack[KERN_STACK_SIZE - 8];
    task->content[12]  = current->kstack[KERN_STACK_SIZE - 7];
    task->content[13]  = current->kstack[KERN_STACK_SIZE - 6];
    task->content[14]  = current->kstack[KERN_STACK_SIZE - 5];
    task->content[15]  = current->kstack[KERN_STACK_SIZE - 4];
    task->content[16]  = current->kstack[KERN_STACK_SIZE - 3];
    task->content[17]  = current->kstack[KERN_STACK_SIZE - 2];
    extern void child_process_shell(void);
    task->content[18]  = (uint32_t)child_process_shell;                 /*  pc, �ȷ��ص�����, ��Ϊ����  */
                                                                        /*  r0 Ҫ����, r1 ���ڴ��� pc   */

    strcpy(task->name, current->name);

    if (vfs_process_fork(pid, current->pid) < 0) {                      /*  ��ʼ�����̵��ļ���Ϣ        */
        goto __exit0;
    }

    if (vmm_process_fork(task, current) < 0) {                          /*  ��ʼ�����̵������ڴ���Ϣ    */
        goto __exit1;
    }

    {
        int i;

        /*
         * �������Ч D-Cache
         */
        for (i = 0; i < (task->file_size + 31) / 32; i++) {
            mmu_clean_invalidate_dcache_mva(pid * PROCESS_SPACE_SIZE + i * 32);
        }

        /*
         * ���д����
         */
        mmu_drain_write_buffer();

        /*
         * ��Ч I-Cache
         */
        for (i = 0; i < (task->file_size + 31) / 32; i++) {
            mmu_invalidate_icache_mva(pid * PROCESS_SPACE_SIZE + i * 32);
        }
    }

    /*
     * ��ʼ�����̵� reent �ṹ
     */
    task->reent = (struct _reent *)(((uint32_t)current->reent) % PROCESS_SPACE_SIZE + task->pid * PROCESS_SPACE_SIZE);

    task->state = TASK_RUNNING;                                         /*  ���̽������̬              */

    return pid;

    __exit1:
    vfs_task_cleanup(pid);

    __exit0:
    task->state = TASK_UNALLOCATE;
    return -1;
}

/*
 * �ں��߳����
 */
static void kthread_shell(task_t *task)
{
    open("/dev/ttyS0", O_RDONLY, 0666);                                 /*  ��������׼�ļ�            */
    stdin  = fdopen(STDIN_FILENO,  "r");

    open("/dev/ttyS0", O_WRONLY, 0666);
    stdout = fdopen(STDOUT_FILENO, "w");

    open("/dev/ttyS0", O_WRONLY, 0666);
    stderr = fdopen(STDERR_FILENO, "w");

    putenv("PATH=/");                                                   /*  ���û�������                */
    putenv("HOME=/");

    task->thread(task->arg);                                            /*  �����������ں��̺߳���      */

    _exit(0);                                                           /*  �˳��ں��߳�                */
}

/*
 * �����ں��߳�
 */
int32_t kthread_create(const char *name, void (*func)(void *), void *arg, uint32_t stack_size, uint32_t priority)
{
    int32_t  tid;
    task_t  *task;
    uint32_t reg;

    if (func == NULL) {
        return -1;
    }

    if (stack_size < 1 * KB) {                                          /*  ջ�ռ���СΪ 1 KB           */
        stack_size = 1 * KB;
    }
    stack_size = MEM_ALIGN_SIZE(stack_size);                            /*  ����ջ�ռ��С              */

    reg = interrupt_disable();

    /*
     * �����ں��߳̿��ƿ�
     */
                                                                        /*  �������е��ں��߳̿��ƿ�    */
    for (tid = PROCESS_NR, task = tasks + PROCESS_NR; tid < TASK_NR; tid++, task++) {
        if (task->state == TASK_UNALLOCATE) {                           /*  ����ں��߳̿��ƿ���Ч      */
            break;
        }
    }

    if (tid == TASK_NR) {                                               /*  û�п��е��ں��߳̿��ƿ�    */
        interrupt_resume(reg);
        return -1;
    }

    task->stack_base = (uint32_t)kmalloc(stack_size);                   /*  ����ջ�ռ�                  */
    if (task->stack_base == 0) {
        interrupt_resume(reg);
        return -1;
    }

    memset((char *)task->stack_base, KTHREAD_STACK_MAGIC0, stack_size); /*  ��ʼ��ջ�ռ�                */

    /*
     * ��ʼ���ں��߳̿��ƿ�
     */
    task->pid          = 0;                                             /*  ���� ID Ϊ 0                */
    task->tid          = tid;                                           /*  ���� ID                     */
    task->state        = TASK_RUNNING;
    task->counter      = priority;
    task->timer        = 0;
    task->priority     = priority;
    task->type         = TASK_TYPE_KTHREAD;                             /*  ��������Ϊ�ں��߳�          */
    task->resume_type  = TASK_RESUME_UNKNOW;
    task->next         = NULL;
    task->wait_list    = NULL;
    task->frame_list   = NULL;
    task->tick         = 0;
    task->cpu_rate     = 0;
    task->frame_nr     = 0;
    task->page_tbl_nr  = 0;
    task->stack_size   = stack_size;
    task->stack_rate   = 0;
    task->thread       = func;
    task->arg          = arg;
    task->dabt_cnt     = 0;
    task->file_size    = 0;
    task->reent        = &reents[tid - PROCESS_NR + 1];
    _REENT_INIT_PTR(task->reent);
    memset(task->mmu_backup, 0, sizeof(task->mmu_backup));

    /*
     * ��ʼ���ں��߳�������
     */
    task->content[0]   = (uint32_t)&task->kstack[KERN_STACK_SIZE];      /*  svc ģʽ��ջָ��(����ջ�ݼ�)*/
    task->content[1]   = ARM_SYS_MODE | ARM_FIQ_NO | ARM_IRQ_EN;        /*  cpsr, sys ģʽ, �� IRQ      */
    task->content[2]   = task->stack_base + stack_size;                 /*  sys ģʽ�� sp               */
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
        strlcpy(task->name, name, sizeof(task->name));
    } else {
        strcpy(task->name, "???");
    }

    if (vfs_task_init(tid) < 0) {                                       /*  ��ʼ���ں��̵߳��ļ���Ϣ    */
        kfree((void *)task->stack_base);
        task->state = TASK_UNALLOCATE;
        interrupt_resume(reg);
        return -1;
    }

    if (running) {                                                      /*  ����ں��Ѿ�����            */
        yield();                                                        /*  ���µ���                    */
    }

    interrupt_resume(reg);

    return tid;
}

/*
 * �������ź�
 */
int task_kill(int32_t tid, int sig)
{
    task_t  *task;
    uint32_t reg;

    if (tid > 0 && tid < TASK_NR) {                                     /*  ���� ID �Ϸ����ж�          */

        reg = interrupt_disable();

        task = &tasks[tid];                                             /*  ���������ƿ�              */

        if (task->state != TASK_UNALLOCATE) {

            vfs_task_cleanup(tid);                                      /*  ����������ļ���Ϣ          */

            if (task->type == TASK_TYPE_PROCESS) {                      /*  ��������ǽ���              */

                printk("process %s pid=%d exit!\n", task->name, task->pid);

                vmm_process_cleanup(task);                              /*  ������̵������ڴ���Ϣ      */

            } else {                                                    /*  ����������ں��߳�          */
                printk("kthread %s tid=%d exit!\n", task->name, tid);

                kfree((void *)task->stack_base);                        /*  �ͷ��ں��̵߳�ջ�ռ�        */
            }

            task->state = TASK_UNALLOCATE;                              /*  �ͷ������������ƿ�        */

            if (tid == current->tid) {
                task_schedule();
            }
        }

        interrupt_resume(reg);

        return 0;
    } else {
        return -1;
    }
}

/*
 * ��� TICK
 */
uint64_t gettick(void)
{
    uint64_t ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = tick;

    interrupt_resume(reg);

    return ret;
}

/*
 * ������� ID
 */
int32_t gettid(void)
{
    int32_t  ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = current->tid;

    interrupt_resume(reg);

    return ret;
}

/*
 * �ж��Ƿ����жϴ��������
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

/*
 * ���������Ϣ
 */
int task_stat(int tid, char *buf)
{
    const char *state;
    task_t *task;
    uint32_t reg;

    if (tid < 0 || tid >= TASK_NR) {
        return -1;
    }

    reg = interrupt_disable();

    task = tasks + tid;

    switch (task->state) {
    case TASK_UNALLOCATE:
        interrupt_resume(reg);
        return -1;

    case TASK_RUNNING:
        state = "ready";
        break;

    case TASK_SLEEPING:
        state = "sleep";
        break;

    case TASK_SUSPEND:
        state = "wait";
        break;

    default:
        state = "unknow";
        break;
    }

    if (strlen(task->name) < 7) {
        sprintf(buf, "%s\t %s\t\t %4u\t %s\t %4u\t %10u\t %4u\t %4u%%\t %4u%%\t %4u\t %4u\n",
                task->type == TASK_TYPE_PROCESS ? "process" : "kthread",
                task->name,
                task->tid,
                state,
                task->counter,
                task->timer,
                task->priority,
                task->cpu_rate,
                task->stack_rate,
                task->frame_nr,
                task->dabt_cnt);
    } else {
        sprintf(buf, "%s\t %s\t %4u\t %s\t %4u\t %10u\t %4u\t %4u%%\t %4u%%\t %4u\t %4u\n",
                task->type == TASK_TYPE_PROCESS ? "process" : "kthread",
                task->name,
                task->tid,
                state,
                task->counter,
                task->timer,
                task->priority,
                task->cpu_rate,
                task->stack_rate,
                task->frame_nr,
                task->dabt_cnt);
    }

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
