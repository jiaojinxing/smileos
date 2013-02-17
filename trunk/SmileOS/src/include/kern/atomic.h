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
** Descriptions:            ԭ����
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
#ifndef ATOMIC_H_
#define ATOMIC_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ԭ����
 */
typedef struct {
    volatile int            counter;                                    /*  ������                      */
} atomic_t;
/*********************************************************************************************************
** Function name:           atomic_read
** Descriptions:            ���ԭ������ֵ
** input parameters:        v                   ԭ����
** output parameters:       NONE
** Returned value:          ԭ������ֵ
*********************************************************************************************************/
int atomic_read(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_set
** Descriptions:            ����ԭ������ֵ
** input parameters:        v                   ԭ����
**                          i                   ԭ������ֵ
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void atomic_set(atomic_t *v, int i);
/*********************************************************************************************************
** Function name:           atomic_add
** Descriptions:            ԭ��������ָ����ֵ
** input parameters:        v                   ԭ����
**                          i                   ����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void atomic_add(atomic_t *v, int i);
/*********************************************************************************************************
** Function name:           atomic_sub
** Descriptions:            ԭ������ȥָ����ֵ
** input parameters:        v                   ԭ����
**                          i                   ����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void atomic_sub(atomic_t *v, int i);
/*********************************************************************************************************
** Function name:           atomic_inc
** Descriptions:            ԭ��������
** input parameters:        v                   ԭ����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void atomic_inc(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_dec
** Descriptions:            ԭ�����Լ�
** input parameters:        v                   ԭ����
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void atomic_dec(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_sub_and_test
** Descriptions:            ԭ������ȥָ����ֵ�������Ƿ�Ϊ 0
** input parameters:        v                   ԭ����
**                          i                   ����
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int atomic_sub_and_test(atomic_t *v, int i);
/*********************************************************************************************************
** Function name:           atomic_dec_and_test
** Descriptions:            ԭ�����Լ��������Ƿ�Ϊ 0
** input parameters:        v                   ԭ����
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int atomic_dec_and_test(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_inc_and_test
** Descriptions:            ԭ���������������Ƿ�Ϊ 0
** input parameters:        v                   ԭ����
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int atomic_inc_and_test(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_sub_return
** Descriptions:            ԭ������ȥָ����ֵ�������µ�ֵ
** input parameters:        v                   ԭ����
**                          i                   ����
** output parameters:       NONE
** Returned value:          ԭ�����µ�ֵ
*********************************************************************************************************/
int atomic_sub_return(atomic_t *v, int i);
/*********************************************************************************************************
** Function name:           atomic_inc_return
** Descriptions:            ԭ���������������µ�ֵ
** input parameters:        v                   ԭ����
** output parameters:       NONE
** Returned value:          ԭ�����µ�ֵ
*********************************************************************************************************/
int atomic_inc_return(atomic_t *v);
/*********************************************************************************************************
** Function name:           atomic_dec_return
** Descriptions:            ԭ�����Լ��������µ�ֵ
** input parameters:        v                   ԭ����
** output parameters:       NONE
** Returned value:          ԭ�����µ�ֵ
*********************************************************************************************************/
int atomic_dec_return(atomic_t *v);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  ATOMIC_H_                   */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
