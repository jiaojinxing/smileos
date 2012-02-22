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
** Descriptions:            �ں�ͷ�ļ�
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
#ifndef KERN_H_
#define KERN_H_

#include "config.h"
#include "types.h"

/*
 * ��������
 */
#define TASK_TYPE_PROCESS 0
#define TASK_TYPE_THREAD  1

/*
 * ����״̬
 */
#define TASK_UNALLOCATE      ((uint32_t)-1)
#define TASK_RUNNING         0
#define TASK_SLEEPING        1

/*
 * ������ƿ�
 */
typedef struct _task {
    int             pid;
    int             tid;
    uint32_t        state;
    uint32_t        count;
    uint32_t        timer;
    uint32_t        priority;
    uint32_t        content[20];
    uint32_t        kstack[KERN_STACK_SIZE];
    int             type;
    struct _task   *child;
    struct _task   *next;
    struct _task   *father;
} task_t;

/*
 * ��ǰ���е�����
 */
extern task_t *current;

/*
 * ϵͳ���ô���
 */
typedef uint32_t sys_do_t;

/*
 * ��ʼ��������
 */
void sched_init(void);

/*
 * ����
 */
void schedule(void);

/*
 * �ں˶�ʱ��������
 */
void do_timer(void);

/*
 * �л������� 0
 */
void __switch_to_process0(uint32_t sp_svc);

/*
 * ��������
 */
int create_process(uint8_t *code, uint32_t size, uint32_t priority);

/*
 * �����߳�
 */
int create_thread(uint32_t pc, uint32_t sp, uint32_t priority);

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
 * �����ַת�����ַ
 */
static inline uint32_t virt_to_phy(uint32_t va)
{
    if (current->pid)
        return PROCESS_MEM_BASE + PROCESS_MEM_SIZE * (current->pid - 1) + va;
    else
        return va;
}

/*
 * �����ַת�����ַ
 */
static inline uint32_t __virt_to_phy(uint32_t va, int pid)
{
    if (pid)
        return PROCESS_MEM_BASE + PROCESS_MEM_SIZE * (pid - 1) + va;
    else
        return va;
}

#endif                                                                  /*  KERN_H_                     */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
