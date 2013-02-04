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
** File name:               select.c
** Last modified Date:      2012-11-5
** Last Version:            1.0.0
** Descriptions:            select ����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-11-5
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
#include "kern/kvars.h"
#include "vfs/device.h"
#include "vfs/driver.h"
#include <fcntl.h>
/*********************************************************************************************************
** Function name:           select_select
** Descriptions:            ����ǰ������뵽�ļ��ĵȴ��б�
** input parameters:        select              select �ṹ
**                          file                �ļ�
**                          type                �ȴ�����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_select(select_struct_t *select, file_t *file, int type)
{
    select_node_t *node;
    reg_t reg;

    if (select == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    node = kmalloc(sizeof(select_node_t), GFP_KERNEL);                  /*  ����ڵ�                    */
    if (node == NULL) {
        seterrno(ENOMEM);
        return -1;
    }

    reg = interrupt_disable();

    node->select_type = type;                                           /*  ��ʼ���ڵ�                  */
    node->task        = current;

    select->wait_list.next->prev = node;                                /*  �ӵ��ȴ��б�                */
    node->next                   = select->wait_list.next;
    select->wait_list.next       = node;
    node->prev                   = &select->wait_list;

    file->select_node = node;

    interrupt_resume(reg);

    return 0;
}
/*********************************************************************************************************
** Function name:           select_unselect
** Descriptions:            ����ǰ������ļ��ĵȴ��б����Ƴ�
** input parameters:        select              select �ṹ
**                          file                �ļ�
**                          type                �ȴ�����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_unselect(select_struct_t *select, file_t *file, int type)
{
    select_node_t *node;
    reg_t reg;

    if (select == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    reg = interrupt_disable();

    node = file->select_node;
    if (node == NULL) {
        interrupt_resume(reg);
        seterrno(EINVAL);
        return -1;
    }

    node->next->prev = node->prev;                                      /*  �ӵȴ��б����Ƴ��ڵ�        */
    node->prev->next = node->next;

    file->select_node = NULL;

    interrupt_resume(reg);

    kfree(node);                                                        /*  �ͷŽڵ�                    */

    return 0;
}
/*********************************************************************************************************
** Function name:           select_report
** Descriptions:            �ر��¼�
** input parameters:        select              select �ṹ
**                          type                �ر�����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_report(select_struct_t *select, int type)
{
    select_node_t *node;
    task_t *task;
    int flag = FALSE;
    reg_t reg;

    reg = interrupt_disable();

    select->flags.counter |= type;                                      /*  ��¼�¼�                    */

    /*
     * �ָ��ȴ��б��������ͽ���������
     */
    node = select->wait_list.next;
    while (node != &select->wait_list) {

        task = node->task;

        if ((type & node->select_type) &&                               /*  �ر�������ȴ������н���    */
            task->resume_type != TASK_RESUME_INTERRUPT) {               /*  ��������ж�, �����      */

            task->delay        = 0;
            task->status       = TASK_RUNNING;                          /*  �ָ�����                    */
            task->resume_type  = TASK_RESUME_SELECT_EVENT;

            if (!in_interrupt()) {                                      /*  �����Ҫ�����������        */
                flag = TRUE;
            }
        }

        node = node->next;
    }

    interrupt_resume(reg);

    if (flag) {
        schedule();                                                     /*  �������                    */
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           select_helper
** Descriptions:            select ��������
** input parameters:        select              select �ṹ
**                          scan                ɨ�躯��
**                          ctx                 ������
**                          file                �ļ��ṹ
**                          type                �ر�����
** output parameters:       NONE
** Returned value:          0 OR -1 OR 1
*********************************************************************************************************/
int select_helper(select_struct_t *select,
                  int (*scan)(void *, file_t *, int),
                  void *ctx,
                  file_t *file,
                  int type)
{
    reg_t reg;
    int ret;
    int resume_type;

    if (file->flags & O_NONBLOCK) {                                     /*  �ϲ㲻������                */
        seterrno(EAGAIN);
        return 0;
    } else {
        reg = interrupt_disable();

        ret = scan(ctx, file, type);                                    /*  ɨ��һ��                    */
        if (ret & type) {
            interrupt_resume(reg);
            return 1;
        }

        ret = select_select(select, file, type);                        /*  ����ǰ������뵽�ȴ��б�    */
        if (ret < 0) {
            interrupt_resume(reg);
            return -1;
        }

        current->delay   = 0;
        current->status  = TASK_SUSPEND;                                /*  ����ǰ����              ��*/

        current->resume_type = TASK_RESUME_UNKNOW;

        schedule();                                                     /*  �������                    */

        select_unselect(select, file, type);                            /*  ����ǰ����ӵȴ��б����Ƴ�  */

        current->delay       = 0;
        resume_type          = current->resume_type;                    /*  ��ûָ�����                */
        current->resume_type = TASK_RESUME_UNKNOW;

        if (resume_type & TASK_RESUME_SELECT_EVENT) {                   /*  �� select �¼��ָ�      ��  */

            ret = scan(ctx, file, type);                                /*  ɨ��һ��                    */

            interrupt_resume(reg);

            if (ret & type) {
                return 1;
            } else {
                seterrno(EAGAIN);
                return 0;
            }
        } else {
            interrupt_resume(reg);
            seterrno(EINTR);                                            /*  select ���ж��ˡ���         */
            return -1;
        }
    }
}
/*********************************************************************************************************
** Function name:           select_init
** Descriptions:            ��ʼ�� select
** input parameters:        select              select �ṹ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int select_init(select_struct_t *select)
{
    atomic_set(&select->flags, 0);

    select->wait_list.next = select->wait_list.prev = &select->wait_list;

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
