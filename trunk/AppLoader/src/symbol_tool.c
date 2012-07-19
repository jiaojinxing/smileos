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
#include "symbol.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct _NODE {
    unsigned int    key;
    void           *data;
    void           *arg;
    struct _NODE   *next;
} NODE;

typedef struct {
    unsigned int    size;
    NODE           *lists[1];
} HASH_TBL;

/*
 * ������ϣ��
 */
static HASH_TBL *hash_tbl_create(unsigned int size)
{
    HASH_TBL *tbl = (HASH_TBL *)calloc(1, sizeof(HASH_TBL) + sizeof(NODE *) * (size - 1));
    if (tbl != NULL) {
        tbl->size = size;
    }
    return tbl;
}

/*
 * �ڹ�ϣ���в���
 */
static NODE *hash_tbl_lookup(HASH_TBL *tbl, unsigned int key, int (*compare)(void *, void *), void *arg)
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
            if (compare(arg, node->arg) == 0) {
                return node;
            }
        }
        node = node->next;
    }
    return NULL;
}

/*
 * �ڹ�ϣ���в���
 */
static int hash_tbl_insert(HASH_TBL *tbl, unsigned int key, void *data, int (*compare)(void *, void *), void *arg)
{
    NODE *node, *head;

    if (NULL == tbl)
        return -1;

    head = tbl->lists[key % tbl->size];
    if (NULL == head){
        node = (NODE *)calloc(1, sizeof(NODE));
        if (node == NULL) {
            return -1;
        }
        node->key  = key;
        node->data = data;
        node->arg  = arg;
        tbl->lists[key % tbl->size] = node;
        return 0;
    } else {
        if (NULL != hash_tbl_lookup(tbl, key, compare, arg)) {
            return -1;
        }

        node = (NODE *)calloc(1, sizeof(NODE));
        if (node == NULL) {
            return -1;
        }
        node->key  = key;
        node->data = data;
        node->arg  = arg;
        tbl->lists[key % tbl->size] = node;

        node->next = head;

        return 0;
    }
}

/*
 * �����ַ����Ĺ�ϣֵ
 */
static unsigned int string_hash(const char *str)
{
    unsigned int seed = 131;                                            /*  31 131 1313 13131 131313 etc*/
    unsigned int hash = 0;
    char         ch;

    while ((ch = *str++) != 0) {
        hash = hash * seed + ch;
    }

    return (hash & 0x7FFFFFFF);
}

/*
 * �Ƚ��ַ���
 */
static int string_compare(void *arg1, void *arg2)
{
    return strcmp(arg1, arg2);
}

/*
 * ���ŵĹ�ϣ��
 */
static HASH_TBL *smileos_symbol_hashtbl;

/*
 * ���ҷ���
 */
unsigned int symbol_lookup(const char *name, unsigned int type)
{
    NODE *node = hash_tbl_lookup(smileos_symbol_hashtbl, string_hash(name), string_compare, (void *)name);
    if (node != NULL) {
        SYMBOL *symbol = node->data;
        return (unsigned int)symbol->addr;
    }
    return 0;
}

/*
 * ����ϵͳ�ķ���
 */
int symbol_add(void)
{
    int i;
    SYMBOL *symbol;

    smileos_symbol_hashtbl = hash_tbl_create(128);
    if (smileos_symbol_hashtbl == NULL) {
        return -1;
    }

    for (i = 0, symbol = smileos_symbol_table; i < SYM_TABLE_SIZE; i++, symbol++) {
        hash_tbl_insert(smileos_symbol_hashtbl, string_hash(symbol->name), symbol, string_compare, symbol->name);
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
