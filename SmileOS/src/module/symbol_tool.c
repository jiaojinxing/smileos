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
** File name:               symbol_tool.c
** Last modified Date:      2012-7-18
** Last Version:            1.0.0
** Descriptions:            ������ع��ߺ���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-18
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
#include "vfs/utils.h"
#include "module/symbol_tool.h"
#include <string.h>
/*********************************************************************************************************
** ��ϣ����
*********************************************************************************************************/
/*
 * �ڵ�
 */
typedef struct _node_t {
    uint32_t        key;                                                /*  ��ֵ                        */
    void           *data;                                               /*  ����                        */
    struct _node_t *next;                                               /*  ����                        */
} node_t;

/*
 * ��ϣ��
 */
typedef struct {
    uint32_t        size;                                               /*  ��С                        */
    node_t         *lists[1];                                           /*  �ڵ���������                */
} hash_tbl_t;
/*********************************************************************************************************
** Function name:           hash_tbl_create
** Descriptions:            ������ϣ��
** input parameters:        size                ��С
** output parameters:       NONE
** Returned value:          ��ϣ�� OR NULL
*********************************************************************************************************/
static hash_tbl_t *hash_tbl_create(uint32_t size)
{
    hash_tbl_t *tbl = (hash_tbl_t *)kzalloc(sizeof(hash_tbl_t) + sizeof(node_t *) * (size - 1), GFP_KERNEL);
    if (tbl != NULL) {
        tbl->size = size;
    }
    return tbl;
}
/*********************************************************************************************************
** Function name:           hash_tbl_lookup
** Descriptions:            �ڹ�ϣ���в���
** input parameters:        tbl                 ��ϣ��
**                          key                 ��ֵ
** output parameters:       NONE
** Returned value:          �ڵ�
*********************************************************************************************************/
static node_t *hash_tbl_lookup(hash_tbl_t *tbl, uint32_t key)
{
    node_t *node;

    if (NULL == tbl) {
        return NULL;
    }

    node = tbl->lists[key % tbl->size];
    while (node != NULL) {
        if (key == node->key) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}
/*********************************************************************************************************
** Function name:           hash_tbl_insert
** Descriptions:            �ڹ�ϣ���в���
** input parameters:        tbl                 ��ϣ��
**                          key                 ��ֵ
**                          data                ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
static int hash_tbl_insert(hash_tbl_t *tbl, uint32_t key, void *data)
{
    node_t *node;

    if (NULL == tbl) {
        return -1;
    }

    node = (node_t *)kzalloc(sizeof(node_t), GFP_KERNEL);
    if (node == NULL) {
        return -1;
    }
    node->key  = key;
    node->data = data;

    node->next = tbl->lists[key % tbl->size];
    tbl->lists[key % tbl->size] = node;

    return 0;
}
/*********************************************************************************************************
** ���Ź�ϣ��
*********************************************************************************************************/
static hash_tbl_t *symbol_hash_tbl;
/*********************************************************************************************************
** Function name:           symbol_lookup
** Descriptions:            ���ҷ���
** input parameters:        name                ������
**                          type                ��������
** output parameters:       NONE
** Returned value:          ���ŵ�ַ
*********************************************************************************************************/
uint32_t symbol_lookup(const char *name, uint32_t type)
{
    node_t *node = hash_tbl_lookup(symbol_hash_tbl, BKDRHash(name));
    if (node != NULL) {
        SYMBOL *symbol = node->data;
        return (uint32_t)symbol->addr;
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           symbol_init
** Descriptions:            ��ʼ�����ű�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int symbol_init(void)
{
    SYMBOL  *symbol;
    node_t  *node;
    uint32_t key;

    symbol_hash_tbl = hash_tbl_create(127);                             /*  �������Ź�ϣ��  ��          */
    if (symbol_hash_tbl == NULL) {
        return -1;
    }

    /*
     * �����ű��еķ��żӵ����Ź�ϣ��
     */
    extern SYMBOL symbol_tbl[];
    for (symbol = symbol_tbl; symbol->name != NULL; symbol++) {
        key  = BKDRHash(symbol->name);
        node = hash_tbl_lookup(symbol_hash_tbl, key);
        if (NULL != node) {
            SYMBOL *temp = node->data;
            if (strcmp(temp->name, symbol->name) == 0) {                /*  �����ظ���                ��*/
                continue;
            } else {                                                    /*  ���ų�ͻ��                  */
                return -1;
            }
        }
        if (hash_tbl_insert(symbol_hash_tbl, key, symbol) < 0) {        /*  �����Ų��뵽���Ź�ϣ��      */
            return -1;
        }
    }
    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
