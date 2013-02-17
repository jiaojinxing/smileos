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
#include "kern/kern.h"
#define KVARS_INC
#include "kern/kvars.h"
#include "kern/func_config.h"
#include <string.h>
#include <stdarg.h>
/*********************************************************************************************************
** �ڲ�����
*********************************************************************************************************/
static tick_t               os_ticks;                                   /*  TICK                        */
/*********************************************************************************************************
** Function name:           kvars_init
** Descriptions:            ��ʼ���ں˱���
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void kvars_init(void)
{
    task_t *task;
    int     i;

    os_started     = FALSE;                                             /*  �ں˻�û����                */
    os_ticks       = 0;                                                 /*  TICK Ϊ 0                   */
    interrupt_nest = 0;                                                 /*  �ж�Ƕ�ײ��Ϊ 0            */
    current        = &tasks[0];                                         /*  ��ǰ����Ϊ���� 0            */
    _impure_ptr    = &reents[0];                                        /*  ��ʼ�� _impure_ptr ָ��     */

    /*
     * ��ʼ�����е�������ƿ�
     */
    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {
        memset(task, 0, sizeof(task_t));
        task->status = TASK_UNALLOCATE;
    }
}
/*********************************************************************************************************
** Function name:           kernel_init
** Descriptions:            ��ʼ���ں�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_init(void)
{
    arch_mmu_init();                                                    /*  ��ʼ�� MMU                  */

    kvars_init();                                                       /*  ��ʼ���ں˱���              */

    extern void interrupt_init(void);
    interrupt_init();                                                   /*  ��ʼ���ж�                  */

    extern void kheap_create(void);
    kheap_create();                                                     /*  �����ں��ڴ��              */

    extern void vmm_init(void);
    vmm_init();                                                         /*  ��ʼ�������ڴ����          */

#if CONFIG_VFS_EN > 0
    extern int vfs_init(void);
    vfs_init();                                                         /*  ��ʼ�������ļ�ϵͳ          */
#endif

    extern void kidle_create(void);
    kidle_create();                                                     /*  �������н���                */

    extern void klogd_create(void);
    klogd_create();                                                     /*  �����ں���־�߳�            */

    extern void default_workqueue_create(void);
    default_workqueue_create();                                         /*  ����ȱʡ��������            */
}
/*********************************************************************************************************
** Function name:           kernel_start
** Descriptions:            �����ں�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_start(void)
{
    if (os_started) {                                                   /*  �ں��Ѿ�����                */
        printk(KERN_ERR"%s: kernel has started\n", __func__);
    } else {
        os_started = TRUE;

        arch_switch_context_to(NULL, current);
    }
}
/*********************************************************************************************************
** Function name:           kernel_timer
** Descriptions:            �ں˶�ʱ��������
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kernel_timer(void)
{
    reg_t   reg;
    int     i;
    task_t *task;

    reg = interrupt_disable();

    os_ticks++;                                                         /*  �ں� TICK ��һ              */

    if (current->type == TASK_TYPE_PROCESS) {                           /*  �����ǰ�����ǽ�������      */
        if (current->timeslice > 0) {                                   /*  �����ǰ���̻���ʣ��ʱ��Ƭ  */
            current->timeslice--;                                       /*  ��ǰ���̵�ʣ��ʱ��Ƭ��һ    */
        }
    }

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {               /*  ������������                */
        if (task->status == TASK_SLEEPING) {                            /*  �������������˯            */
            if (task->delay != 0) {
                task->delay--;                                          /*  ������ʱ��һ                */
            }
            if (task->delay == 0) {                                     /*  ���������ʱ����            */
                task->status      = TASK_RUNNING;                       /*  ����������̬              */
                task->resume_type = TASK_RESUME_TIMEOUT;                /*  ��������Ļָ�����Ϊ��ʱ    */
                extern int ipc_task_cleanup(task_t *task);
                ipc_task_cleanup(task);                                 /*  �������� IPC                */
            }
        }
    }

    interrupt_resume(reg);
}
/*********************************************************************************************************
** Function name:           getticks
** Descriptions:            ��� TICK
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TICK
*********************************************************************************************************/
tick_t getticks(void)
{
    tick_t ret;
    reg_t  reg;

    reg = interrupt_disable();

    ret = os_ticks;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           bkdr_hash
** Descriptions:            BKDR Hash Function
**                          �����ַ��� Hash �����Ƚ�
**                          http://www.byvoid.com/blog/string-hash-compare/
** input parameters:        str                 �ַ���
** output parameters:       NONE
** Returned value:          BKDR Hash
*********************************************************************************************************/
unsigned int bkdr_hash(const char *str)
{
    unsigned int seed = 131;                                            /*  31 131 1313 13131 131313 etc*/
    unsigned int hash = 0;
    char ch;

    while ((ch = *str++) != 0) {
        hash = hash * seed + ch;
    }

    return (hash & 0x7FFFFFFF);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
