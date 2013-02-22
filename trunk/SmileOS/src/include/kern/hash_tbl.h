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
** File name:               hash_tbl.h
** Last modified Date:      2013-2-22
** Last Version:            1.0.0
** Descriptions:            ��ϣ��
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-22
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
#ifndef HASH_TBL_H_
#define HASH_TBL_H_

#include <sys/types.h>
/*********************************************************************************************************
** �������Ͷ���
*********************************************************************************************************/
/*
 * ��ϣ�ڵ�
 */
struct hash_node;
typedef struct hash_node hash_node_t;

struct hash_node {
    unsigned int        key;                                            /*  ��ֵ                        */
    void               *data;                                           /*  ����                        */
    struct hash_node   *next;                                           /*  ����                        */
};

/*
 * ��ϣ��
 */
typedef struct {
    size_t              size;                                           /*  ��С                        */
    hash_node_t        *lists[1];                                       /*  �ڵ���������                */
} hash_tbl_t;
/*********************************************************************************************************
** Function name:           hash_tbl_create
** Descriptions:            ������ϣ��
** input parameters:        size                ��С
** output parameters:       NONE
** Returned value:          ��ϣ�� OR NULL
*********************************************************************************************************/
hash_tbl_t *hash_tbl_create(size_t size);
/*********************************************************************************************************
** Function name:           hash_tbl_destroy
** Descriptions:            ���ٹ�ϣ��
** input parameters:        tbl                 ��ϣ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int hash_tbl_destroy(hash_tbl_t *tbl);
/*********************************************************************************************************
** Function name:           hash_tbl_lookup
** Descriptions:            �ڹ�ϣ���в���
** input parameters:        tbl                 ��ϣ��
**                          key                 ��ֵ
** output parameters:       NONE
** Returned value:          �ڵ� OR NULL
*********************************************************************************************************/
hash_node_t *hash_tbl_lookup(hash_tbl_t *tbl, unsigned int key);
/*********************************************************************************************************
** Function name:           hash_tbl_insert
** Descriptions:            �ڹ�ϣ���в���
** input parameters:        tbl                 ��ϣ��
**                          key                 ��ֵ
**                          data                ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int hash_tbl_insert(hash_tbl_t *tbl, unsigned int key, void *data);
/*********************************************************************************************************
** Function name:           bkdr_hash
** Descriptions:            BKDR Hash Function
**                          �����ַ��� Hash �����Ƚ�
**                          http://www.byvoid.com/blog/string-hash-compare/
** input parameters:        str                 �ַ���
** output parameters:       NONE
** Returned value:          BKDR Hash
*********************************************************************************************************/
unsigned int bkdr_hash(const char *str);

#endif                                                                  /*  HASH_TBL_H_                 */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
