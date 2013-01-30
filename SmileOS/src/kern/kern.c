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
#include "kern/vmm.h"
#include "kern/mmu.h"
#include <string.h>
#include <stdarg.h>
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

    running        = FALSE;                                             /*  �ں˻�û����                */
    interrupt_nest = 0;                                                 /*  �ж�Ƕ�ײ��Ϊ 0            */
    ticks          = 0;                                                 /*  TICK Ϊ 0                   */
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
    extern void cpu_kernel_init(void);
    cpu_kernel_init();                                                  /*  ��ʼ�� CPU                  */

    mmu_init();                                                         /*  ��ʼ�� MMU                  */

    kvars_init();                                                       /*  ��ʼ���ں˱���              */

    extern void kheap_create(void);
    kheap_create();                                                     /*  �����ں��ڴ��              */

    extern void kidle_create(void);
    kidle_create();                                                     /*  �������н���                */

    extern void klogd_create(void);
    klogd_create();                                                     /*  �����ں���־�߳�            */

    extern void alarmd_create(void);
    alarmd_create();                                                    /*  ���� alarm �߳�             */

    extern void netjob_create(void);
    netjob_create();                                                    /*  �������繤���߳�            */

    vmm_init();                                                         /*  ��ʼ�������ڴ����          */
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
    if (!running) {
        running = TRUE;                                                 /*  �ں��Ѿ�����                */

        extern void process0_enter(register uint32_t sp_svc);
        process0_enter(current->regs[0]);                               /*  ������� 0 (���н���)       */
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
    uint32_t reg;
    int      i;
    task_t  *task;

    reg = interrupt_disable();

    ticks++;                                                            /*  �ں� TICK ��һ              */

    current->ticks++;                                                   /*  ��ǰ�����жϵĴ�����һ    */

    if (current->type == TASK_TYPE_PROCESS) {                           /*  �����ǰ�����ǽ�������      */
        if (current->timeslice > 0) {                                   /*  �����ǰ���̻���ʣ��ʱ��Ƭ  */
            current->timeslice--;                                       /*  ��ǰ���̵�ʣ��ʱ��Ƭ��һ    */
        }
    }

    if (ticks % TICK_PER_SECOND == 0) {                                 /*  ����Ѿ���ȥ��һ��          */
        for (i = 0, task = tasks; i < TASK_NR; i++, task++) {           /*  ������������                */
            if (task->status != TASK_UNALLOCATE) {                      /*  ���������Ч                */
                task->cpu_usage = task->ticks;                          /*  ͳ������� CPU ռ����       */
                task->ticks     = 0;                                    /*  ���ø����񱻶�ʱ���жϵĴ���*/
            }
        }
    }

    for (i = 0, task = tasks; i < TASK_NR; i++, task++) {               /*  ������������                */
        if (task->status == TASK_SLEEPING) {                            /*  �������������˯            */
            task->delay--;                                              /*  ������ʱ��һ                */
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
uint64_t getticks(void)
{
    uint64_t ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = ticks;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           gettid
** Descriptions:            ��õ�ǰ����� TID
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ��ǰ����� TID
*********************************************************************************************************/
int32_t gettid(void)
{
    int32_t  ret;
    uint32_t reg;

    reg = interrupt_disable();

    ret = current->tid;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           va_to_mva
** Descriptions:            �����̿ռ�������ַת��Ϊ�޸ĺ�������ַ
** input parameters:        VA                  ���̿ռ�������ַ
** output parameters:       NONE
** Returned value:          �޸ĺ�������ַ
*********************************************************************************************************/
void *va_to_mva(const void *VA)
{
    uint32_t reg;
    void *ret;

    reg = interrupt_disable();

    if ((current->pid != 0) && (uint32_t)VA < PROCESS_SPACE_SIZE && VA != NULL) {
        ret = (uint8_t *)VA + current->pid * PROCESS_SPACE_SIZE;
    } else {
        ret = (void *)VA;
    }

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           mva_to_va
** Descriptions:            ���޸ĺ�������ַת��Ϊ���̿ռ�������ַ
** input parameters:        MVA                 �޸ĺ�������ַ
** output parameters:       NONE
** Returned value:          ���̿ռ�������ַ
*********************************************************************************************************/
void *mva_to_va(const void *MVA)
{
    return (void *)((uint32_t)MVA % PROCESS_SPACE_SIZE);
}
/*********************************************************************************************************
** Function name:           vspace_usable
** Descriptions:            �ж������ַ�ռ��Ƿ����
** input parameters:        base                �����ַ�ռ�Ļ�ַ
**                          size                �����ַ�ռ�Ĵ�С
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int vspace_usable(uint32_t base, uint32_t size)
{
    uint32_t end = base + size;
    uint32_t high, low;
    int      i;

    /*
     * �����ַ�ռ䲻�ܺ�ϵͳ�����������ַ�ռ��ص�
     */
    extern const space_t sys_resv_space[];
    for (i = 0; sys_resv_space[i].size != 0; i++) {
        high = max(sys_resv_space[i].base, base);
        low  = min(sys_resv_space[i].base + sys_resv_space[i].size, end);
        if (high < low) {
            return FALSE;
        }
    }

    /*
     * �����ַ�ռ䲻�ܺ� BSP �����������ַ�ռ��ص�
     */
    extern const space_t bsp_resv_space[];
    for (i = 0; bsp_resv_space[i].size != 0; i++) {
        high = max(bsp_resv_space[i].base, base);
        low  = min(bsp_resv_space[i].base + bsp_resv_space[i].size, end);
        if (high < low) {
            return FALSE;
        }
    }

    /*
     * �����ַ�ռ䲻�ܺ� CPU �����������ַ�ռ��ص�
     */
    extern const space_t cpu_resv_space[];
    for (i = 0; cpu_resv_space[i].size != 0; i++) {
        high = max(cpu_resv_space[i].base, base);
        low  = min(cpu_resv_space[i].base + cpu_resv_space[i].size, end);
        if (high < low) {
            return FALSE;
        }
    }

    return TRUE;
}
/*********************************************************************************************************
** Function name:           kcomplain
** Descriptions:            �ں˱�Թ(������ʹ�� printk ʱʹ��)
** input parameters:        fmt                 ��ʽ�ַ���
**                          ...                 �������
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void kcomplain(const char *fmt, ...)
{
    static const char digits[] = "0123456789abcdef";
    va_list  va;
    char     buf[10];
    char    *s;
    char     c;
    unsigned u;
    int      i, pad;

    va_start(va, fmt);
    while ((c = *fmt++) != '\0') {
        if (c == '%') {
            c = *fmt++;
            /*
             * ignore long
             */
            if (c == 'l') {
                c = *fmt++;
            }
            switch (c) {
            case 'c':
                kputc(va_arg(va, int));
                continue;

            case 's':
                s = va_arg(va, char *);
                if (s == NULL) {
                    s = "<NULL>";
                }
                for (; *s != '\0'; s++) {
                    kputc(*s);
                }
                continue;

            case 'd':
                c = 'u';
            case 'u':
            case 'x':
                u = va_arg(va, unsigned);
                s = buf;
                if (c == 'u') {
                    do {
                        *s++ = digits[u % 10U];
                    } while (u /= 10U);
                } else {
                    pad = 0;
                    for (i = 0; i < 8; i++) {
                        if (pad) {
                            *s++ = '0';
                        } else {
                            *s++ = digits[u % 16U];
                            if ((u /= 16U) == 0) {
                                pad = 1;
                            }
                        }
                    }
                }
                while (--s >= buf) {
                    kputc(*s);
                }
                continue;
            }
        }
        if (c == '\n') {
            kputc('\r');
        }
        kputc((int)c);
    }
    va_end(va);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
