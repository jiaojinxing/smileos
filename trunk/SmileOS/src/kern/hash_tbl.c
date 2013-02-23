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
** File name:               hash_tbl.c
** Last modified Date:      2013-2-22
** Last Version:            1.0.0
** Descriptions:            哈希表
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-22
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
#include "kern/func_config.h"
#if CONFIG_MODULE_EN > 0
#include "kern/kern.h"
#include "kern/hash_tbl.h"
/*********************************************************************************************************
** Function name:           hash_tbl_create
** Descriptions:            创建哈希表
** input parameters:        size                大小
** output parameters:       NONE
** Returned value:          哈希表 OR NULL
*********************************************************************************************************/
hash_tbl_t *hash_tbl_create(size_t size)
{
    hash_tbl_t *tbl;

    tbl = (hash_tbl_t *)kzalloc(sizeof(hash_tbl_t) + sizeof(hash_node_t *) * (size - 1), GFP_KERNEL);
    if (tbl != NULL) {
        tbl->size = size;
    }
    return tbl;
}
/*********************************************************************************************************
** Function name:           hash_tbl_destroy
** Descriptions:            销毁哈希表
** input parameters:        tbl                 哈希表
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int hash_tbl_destroy(hash_tbl_t *tbl)
{
    size_t i;
    hash_node_t *next;
    hash_node_t *node;

    if (tbl == NULL) {
        return -1;
    }

    for (i = 0; i < tbl->size; i++) {
        node = tbl->lists[i];
        while (node != NULL) {
            next = node->next;
            kfree(node);
            node = next;
        }
    }

    kfree(tbl);

    return 0;
}
/*********************************************************************************************************
** Function name:           hash_tbl_lookup
** Descriptions:            在哈希表中查找
** input parameters:        tbl                 哈希表
**                          key                 键值
** output parameters:       NONE
** Returned value:          节点 OR NULL
*********************************************************************************************************/
hash_node_t *hash_tbl_lookup(hash_tbl_t *tbl, unsigned int key)
{
    hash_node_t *node;

    if (tbl == NULL) {
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
** Descriptions:            在哈希表中插入
** input parameters:        tbl                 哈希表
**                          key                 键值
**                          data                数据
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int hash_tbl_insert(hash_tbl_t *tbl, unsigned int key, void *data)
{
    hash_node_t *node;

    if (tbl == NULL) {
        return -1;
    }

    node = (hash_node_t *)kzalloc(sizeof(hash_node_t), GFP_KERNEL);
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
** Function name:           bkdr_hash
** Descriptions:            BKDR Hash Function
**                          各种字符串 Hash 函数比较
**                          http://www.byvoid.com/blog/string-hash-compare/
** input parameters:        str                 字符串
** output parameters:       NONE
** Returned value:          BKDR Hash
*********************************************************************************************************/
unsigned int bkdr_hash(const char *str)
{
    unsigned int seed = 131;                                            /*  31 131 1313 13131 131313 etc*/
    unsigned int hash = 0;
    char ch;

    if (str == NULL) {
        return -1;
    }

    while ((ch = *str++) != 0) {
        hash = hash * seed + ch;
    }

    return (hash & 0x7FFFFFFF);
}
#endif
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
