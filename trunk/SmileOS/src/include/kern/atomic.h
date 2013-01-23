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
** File name:               atomic.h
** Last modified Date:      2012-11-5
** Last Version:            1.0.0
** Descriptions:            原子量
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-11-5
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
#ifndef ATOMIC_H_
#define ATOMIC_H_

/*
 * 原子量
 */
typedef struct {
    volatile int            counter;                                    /*  计数器                      */
} atomic_t;
/*********************************************************************************************************
** Function name:           atomic_read
** Descriptions:            获得原子量的值
** input parameters:        v                   原子量
** output parameters:       NONE
** Returned value:          原子量的值
*********************************************************************************************************/
int atomic_read(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_set
** Descriptions:            设置原子量的值
** input parameters:        v                   原子量
**                          i                   原子量的值
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void atomic_set(atomic_t *v, int i);
/*********************************************************************************************************
** Function name:           atomic_add
** Descriptions:            原子量加上指定的值
** input parameters:        v                   原子量
**                          i                   增量
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void atomic_add(atomic_t *v, int i);
/*********************************************************************************************************
** Function name:           atomic_sub
** Descriptions:            原子量减去指定的值
** input parameters:        v                   原子量
**                          i                   减量
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void atomic_sub(atomic_t *v, int i);
/*********************************************************************************************************
** Function name:           atomic_inc
** Descriptions:            原子量自增
** input parameters:        v                   原子量
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void atomic_inc(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_dec
** Descriptions:            原子量自减
** input parameters:        v                   原子量
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void atomic_dec(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_sub_and_test
** Descriptions:            原子量减去指定的值并测试是否为 0
** input parameters:        v                   原子量
**                          i                   减量
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int atomic_sub_and_test(atomic_t *v, int i);
/*********************************************************************************************************
** Function name:           atomic_dec_and_test
** Descriptions:            原子量自减并测试是否为 0
** input parameters:        v                   原子量
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int atomic_dec_and_test(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_inc_and_test
** Descriptions:            原子量自增并测试是否为 0
** input parameters:        v                   原子量
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int atomic_inc_and_test(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_sub_return
** Descriptions:            原子量减去指定的值并返回新的值
** input parameters:        v                   原子量
**                          i                   减量
** output parameters:       NONE
** Returned value:          原子量新的值
*********************************************************************************************************/
int atomic_sub_return(atomic_t *v, int i);
/*********************************************************************************************************
** Function name:           atomic_inc_return
** Descriptions:            原子量自增并返回新的值
** input parameters:        v                   原子量
** output parameters:       NONE
** Returned value:          原子量新的值
*********************************************************************************************************/
int atomic_inc_return(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_dec_return
** Descriptions:            原子量自减并返回新的值
** input parameters:        v                   原子量
** output parameters:       NONE
** Returned value:          原子量新的值
*********************************************************************************************************/
int atomic_dec_return(atomic_t *v);

#endif                                                                  /*  ATOMIC_H_                   */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
