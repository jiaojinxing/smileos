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
** File name:               sys_do.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            ϵͳ���ô���
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
#include "config.h"
#include "types.h"
#include "kern.h"
/*********************************************************************************************************
  ϵͳ���ô���
*********************************************************************************************************/
/*
 * �����˳�
 */
static int do_exit(int error_code)
{
    current->state = TASK_UNALLOCATE;

    schedule();

    return 0;
}

/*
 * ��������
 */
static int do_sleep(int time)
{
    current->timer = time;

    current->state = TASK_SLEEPING;

    schedule();

    return 0;
}

/*
 * ����д
 */
static int do_write(char *str)
{
    printk((char *)virt_to_phy((uint32_t)str));

    return 0;
}
/*********************************************************************************************************
  ϵͳ���ô����
*********************************************************************************************************/
sys_do_t sys_do_table[] = {
        /*
         * do_xxx ��λ�ñ���Ҫ�� SYS_CALL_XXX ��ֵһ��
         */
        (sys_do_t)do_exit,
        (sys_do_t)do_sleep,
        (sys_do_t)do_write
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
