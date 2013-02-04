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
** File name:               interrupt.c
** Last modified Date:      2012-10-23
** Last Version:            1.0.0
** Descriptions:            中断
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-10-23
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
#include "kern/kern.h"
#include "kern/kvars.h"
/*********************************************************************************************************
** 配置
*********************************************************************************************************/
#define INTERRUPT_NR                (32)                                /*  中断向量数目, 如果大于请修改*/
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
static isr_t  isr_table[INTERRUPT_NR];                                  /*  中断服务程序表              */
static void  *isr_arg_table[INTERRUPT_NR];                              /*  中断服务程序参数表          */
/*********************************************************************************************************
** Function name:           interrupt_enter
** Descriptions:            进入中断
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_enter(void)
{
    reg_t reg;

    reg = interrupt_disable();

    if (interrupt_nest == 255u) {
        printk(KERN_ERR"os error: interrupt_nest == 255u at %s %d\n", __func__, __LINE__);
        interrupt_resume(reg);
        return;
    }

    interrupt_nest++;                                                   /*  中断嵌套层次加一            */

    interrupt_resume(reg);
}
/*********************************************************************************************************
** Function name:           interrupt_exit
** Descriptions:            退出中断
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_exit(void)
{
    reg_t reg;

    reg = interrupt_disable();

    if (interrupt_nest == 0u) {
        printk(KERN_ERR"os error: interrupt_nest == 0u at %s %d\n", __func__, __LINE__);
        interrupt_resume(reg);
        return;
    }

    interrupt_nest--;                                                   /*  中断嵌套层次减一            */
    if (interrupt_nest == 0) {                                          /*  如果已经完全退出了中断      */
        schedule();                                                     /*  任务调度                    */
    }
    interrupt_resume(reg);
}
/*********************************************************************************************************
** Function name:           in_interrupt
** Descriptions:            判断是否在中断处理程序中
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
bool_t in_interrupt(void)
{
    bool_t   ret;
    reg_t    reg;

    reg = interrupt_disable();

    ret = interrupt_nest > 0 ? TRUE : FALSE;

    interrupt_resume(reg);

    return ret;
}
/*********************************************************************************************************
** Function name:           isr_invaild
** Descriptions:            无效中断服务程序
** input parameters:        interrupt           中断号
**                          arg                 参数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int isr_invaild(intno_t interrupt, void *arg)
{
    printk(KERN_ERR"os error: interrupt %d invaild at %s %d\n", interrupt, __func__, __LINE__);

    return -1;
}
/*********************************************************************************************************
** Function name:           interrupt_install
** Descriptions:            安装中断服务程序
** input parameters:        interrupt           中断号
**                          new_isr             新的中断服务程序
**                          arg                 中断服务程序参数
** output parameters:       old_isr             旧的中断服务程序
** Returned value:          NONE
*********************************************************************************************************/
int interrupt_install(intno_t interrupt, isr_t new_isr, isr_t *old_isr, void *arg)
{
    reg_t reg;

    if (interrupt < INTERRUPT_NR) {

        reg = interrupt_disable();

        if (old_isr != NULL) {
            *old_isr = isr_table[interrupt];
        }

        if (new_isr != NULL) {
            isr_table[interrupt]     = new_isr;
            isr_arg_table[interrupt] = arg;
        } else {
            isr_table[interrupt]     = (isr_t)isr_invaild;
        }

        interrupt_resume(reg);

        return 0;
    } else {
        printk(KERN_ERR"os error: interrupt %d > INTERRUPT_NR at %s %d\n", interrupt, __func__, __LINE__);
        return -1;
    }
}
/*********************************************************************************************************
** Function name:           interrupt_init
** Descriptions:            初始化中断
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_init(void)
{
    int i;

    for (i = 0; i < INTERRUPT_NR; i++) {                                /*  初始化中断服务程序及其参数表*/
        isr_table[i]     = (isr_t)isr_invaild;
        isr_arg_table[i] = NULL;
    }
}
/*********************************************************************************************************
** Function name:           interrupt_exec
** Descriptions:            执行中断服务程序
** input parameters:        interrupt           中断号
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void interrupt_exec(intno_t interrupt)
{
    reg_t reg;
    isr_t isr;
    void *arg;

    if (interrupt < INTERRUPT_NR) {

        reg = interrupt_disable();

        isr = isr_table[interrupt];
        arg = isr_arg_table[interrupt];

        interrupt_resume(reg);

        if (isr != NULL) {
            isr(interrupt, arg);
        }
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
