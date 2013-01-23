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
** Descriptions:            ������ؿ⺯��
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
#include "module/symbol_tool.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*
 * �ڵ�
 */
typedef struct _NODE {
    uint32_t        key;                                                /*  ��ֵ                        */
    void           *data;                                               /*  ����                        */
    struct _NODE   *next;                                               /*  ����                        */
} NODE;

/*
 * ��ϣ��
 */
typedef struct {
    uint32_t        size;                                               /*  ��С                        */
    NODE           *lists[1];                                           /*  �ڵ���������                */
} HASH_TBL;

/*
 * ���ŵĹ�ϣ��
 */
static HASH_TBL *smileos_symbol_hashtbl;
/*********************************************************************************************************
** Function name:           hash_tbl_create
** Descriptions:            ������ϣ��
** input parameters:        size                ��С
** output parameters:       NONE
** Returned value:          ��ϣ��
*********************************************************************************************************/
static HASH_TBL *hash_tbl_create(uint32_t size)
{
    HASH_TBL *tbl = (HASH_TBL *)kcalloc(1, sizeof(HASH_TBL) + sizeof(NODE *) * (size - 1));
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
**                          compare             �ȽϺ���
**                          arg                 ����
** output parameters:       NONE
** Returned value:          �ڵ�
*********************************************************************************************************/
static NODE *hash_tbl_lookup(HASH_TBL *tbl,
                             uint32_t key)
{
    NODE *node;

    if (NULL == tbl) {
        return NULL;
    }

    if (NULL == (node = tbl->lists[key % tbl->size])) {
        return NULL;
    }

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
** Returned value:          �ڵ�
*********************************************************************************************************/
static int hash_tbl_insert(HASH_TBL *tbl,
                           uint32_t key,
                           void *data)
{
    NODE *node, *head;

    if (NULL == tbl) {
        return -1;
    }

    head = tbl->lists[key % tbl->size];
    if (NULL == head){
        node = (NODE *)kcalloc(1, sizeof(NODE));
        if (node == NULL) {
            return -1;
        }
        node->key  = key;
        node->data = data;
        tbl->lists[key % tbl->size] = node;
        return 0;
    } else {
        node = (NODE *)kcalloc(1, sizeof(NODE));
        if (node == NULL) {
            return -1;
        }
        node->key  = key;
        node->data = data;
        node->next = head;
        tbl->lists[key % tbl->size] = node;
        return 0;
    }
}
/*********************************************************************************************************
** Function name:           bkrd_hash
** Descriptions:            �����ַ����Ĺ�ϣֵ
** input parameters:        str                 �ַ���
** output parameters:       NONE
** Returned value:          �ַ����Ĺ�ϣֵ
*********************************************************************************************************/
static uint32_t bkrd_hash(const char *str)
{
    uint32_t seed = 131;                                                /*  31 131 1313 13131 131313 etc*/
    uint32_t hash = 0;
    char     ch;

    while ((ch = *str++) != 0) {
        hash = hash * seed + ch;
    }

    return (hash & 0x7FFFFFFF);
}
/*********************************************************************************************************
** Function name:           symbol_lookup
** Descriptions:            ���ҷ���
** input parameters:        name                ������
**                          type                ��������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
uint32_t symbol_lookup(const char *name, uint32_t type)
{
    NODE *node = hash_tbl_lookup(smileos_symbol_hashtbl, bkrd_hash(name));
    if (node != NULL) {
        SYMBOL *symbol = node->data;
        return (uint32_t)symbol->addr;
    }
    return 0;
}
/*********************************************************************************************************
** Function name:           symbol_init
** Descriptions:            ����ϵͳ�ķ���
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int symbol_init(void)
{
    SYMBOL *symbol;
    SYMBOL *temp;
    NODE   *node;
    unsigned int key;

    smileos_symbol_hashtbl = hash_tbl_create(127);                      /*  127 �Ǹ�����                */
    if (smileos_symbol_hashtbl == NULL) {
        return -1;
    }

    extern SYMBOL smileos_symbol_table[];
    for (symbol = smileos_symbol_table;
         symbol->name != NULL;
         symbol++) {
        key  = bkrd_hash(symbol->name);
        node = hash_tbl_lookup(smileos_symbol_hashtbl, key);
        if (NULL != node) {
            temp = node->data;
            if (strcmp(temp->name, symbol->name) == 0) {
                continue;
            } else {
                return -1;
            }
        }
        if (hash_tbl_insert(smileos_symbol_hashtbl, key, symbol) < 0) {
            return -1;
        }
    }
    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

