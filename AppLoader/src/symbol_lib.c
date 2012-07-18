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
** File name:               symbol_lib.c
** Last modified Date:      2012-7-18
** Last Version:            1.0.0
** Descriptions:            符号相关库函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-18
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
#include "symbol.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct _NODE {
    unsigned int    key;
    void           *data;
    struct _NODE   *next;
} NODE;

typedef struct {
    unsigned int    size;
    NODE           *nodes[1];
} HASH_TBL;

/*
 * 创建哈希表
 */
static HASH_TBL *hash_tbl_create(int size)
{
    HASH_TBL *tbl = (HASH_TBL *)calloc(1, sizeof(HASH_TBL) + sizeof(NODE *) * (size - 1));
    if (tbl != NULL) {
        tbl->size = size;
    }
    return tbl;
}

/*
 * 在哈希表中查找
 */
static NODE *hash_tbl_lookup(HASH_TBL *tbl, unsigned int key)
{
    NODE *node;

    if (NULL == tbl) {
        return NULL;
    }

    if (NULL == (node = tbl->nodes[key % tbl->size])) {
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

/*
 * 在哈希表中插入
 */
static int hash_tbl_insert(HASH_TBL *tbl, unsigned int key, void *data)
{
    NODE *node, *head;

    if (NULL == tbl)
        return -1;

    head = tbl->nodes[key % tbl->size];
    if (NULL == head){
        node = (NODE *)calloc(1, sizeof(NODE));
        if (node == NULL) {
            return -1;
        }
        node->key  = key;
        node->data = data;
        tbl->nodes[key % tbl->size] = node;
        return 0;
    } else {
        if (NULL != hash_tbl_lookup(tbl, key)) {
            return -1;
        }

        node = (NODE *)calloc(1, sizeof(NODE));
        if (node == NULL) {
            return -1;
        }
        node->key  = key;
        node->data = data;
        tbl->nodes[key % tbl->size] = node;

        node->next = head;

        return 0;
    }
}

/*
 * 计算字符串的哈希值
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
 * 符号的哈希表
 */
static HASH_TBL *symbol_hash_tbl;

/*
 * 查找符号
 */
unsigned int symbol_lookup(const char *name, unsigned int type)
{
    NODE *node = hash_tbl_lookup(symbol_hash_tbl, string_hash(name));
    if (node != NULL) {
        LW_STATIC_SYMBOL *symbol = node->data;
        return (unsigned int)symbol->LWSSYMBOL_pcAddr;
    }
    return 0;
}

/*
 * 增加系统的符号
 */
int symbol_add(void)
{
    int i;
    int ret;
    LW_STATIC_SYMBOL *symbol;

    symbol_hash_tbl = hash_tbl_create(128);
    if (symbol_hash_tbl == NULL) {
        return -1;
    }

    for (i = 0, symbol = _G_symLibSmileOS; i < SYM_TABLE_SIZE; i++, symbol++) {
        ret = hash_tbl_insert(symbol_hash_tbl, string_hash(symbol->LWSSYMBOL_pcName), symbol);
        if (ret < 0) {
            return ret;
        }
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
