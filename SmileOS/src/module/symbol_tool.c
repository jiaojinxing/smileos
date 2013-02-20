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
** Descriptions:            符号相关工具函数
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
#include "kern/func_config.h"
#if CONFIG_MODULE_EN > 0
#include "kern/kern.h"
#include "module/symbol_tool.h"
#include <string.h>
/*********************************************************************************************************
** 哈希表函数
*********************************************************************************************************/
/*
 * 节点
 */
typedef struct _node_t {
    uint32_t        key;                                                /*  键值                        */
    void           *data;                                               /*  数据                        */
    struct _node_t *next;                                               /*  后趋                        */
} node_t;

/*
 * 哈希表
 */
typedef struct {
    size_t          size;                                               /*  大小                        */
    node_t         *lists[1];                                           /*  节点链表数组                */
} hash_tbl_t;
/*********************************************************************************************************
** Function name:           hash_tbl_create
** Descriptions:            创建哈希表
** input parameters:        size                大小
** output parameters:       NONE
** Returned value:          哈希表 OR NULL
*********************************************************************************************************/
static hash_tbl_t *hash_tbl_create(size_t size)
{
    hash_tbl_t *tbl;

    tbl = (hash_tbl_t *)kzalloc(sizeof(hash_tbl_t) + sizeof(node_t *) * (size - 1), GFP_KERNEL);
    if (tbl != NULL) {
        tbl->size = size;
    }
    return tbl;
}
/*********************************************************************************************************
** Function name:           hash_tbl_lookup
** Descriptions:            在哈希表中查找
** input parameters:        tbl                 哈希表
**                          key                 键值
** output parameters:       NONE
** Returned value:          节点 OR NULL
*********************************************************************************************************/
static node_t *hash_tbl_lookup(hash_tbl_t *tbl, uint32_t key)
{
    node_t *node;

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
static int hash_tbl_insert(hash_tbl_t *tbl, uint32_t key, void *data)
{
    node_t *node;

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
** 符号表
*********************************************************************************************************/
static hash_tbl_t *text_symbol_tbl;                                     /*  TEXT 段符号表               */
static hash_tbl_t *data_symbol_tbl;                                     /*  DATA 段符号表               */
/*********************************************************************************************************
** Function name:           sys_symbol_lookup
** Descriptions:            查找系统符号
** input parameters:        name                符号名
**                          type                符号类型
** output parameters:       NONE
** Returned value:          符号地址 OR NULL
*********************************************************************************************************/
void *sys_symbol_lookup(const char *name, uint8_t type)
{
    node_t *node;

    node = hash_tbl_lookup(type == SYMBOL_TEXT ? text_symbol_tbl : data_symbol_tbl, bkdr_hash(name));
    if (node != NULL) {
        symbol_t *symbol = node->data;
        return symbol->addr;
    }
    return NULL;
}
/*********************************************************************************************************
** Function name:           sys_symbol_init
** Descriptions:            初始化系统符号表
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sys_symbol_init(void)
{
    hash_tbl_t *tbl;
    symbol_t   *symbol;
    node_t     *node;
    uint32_t    key;

    text_symbol_tbl = hash_tbl_create(127);                             /*  创建 TEXT 段符号表          */
    if (text_symbol_tbl == NULL) {
        printk(KERN_ERR"%s: failed to create text symbol hash table\n", __func__);
        return -1;
    }

    data_symbol_tbl = hash_tbl_create(127);                             /*  创建 DATA 段符号表          */
    if (data_symbol_tbl == NULL) {
        printk(KERN_ERR"%s: failed to create data symbol hash table\n", __func__);
        return -1;
    }

    /*
     * 将系统符号表中的符号加到对应的符号表中
     */
    extern symbol_t symbol_tbl[];
    for (symbol = symbol_tbl; symbol->name != NULL; symbol++) {

        tbl  = symbol->flags == SYMBOL_TEXT ? text_symbol_tbl : data_symbol_tbl;

        key  = bkdr_hash(symbol->name);

        node = hash_tbl_lookup(tbl, key);                               /*  先查找该符号                */

        if (NULL != node) {

            symbol_t *temp = node->data;

            if (strcmp(temp->name, symbol->name) == 0) {                /*  符号重复了                　*/
                continue;
            } else {                                                    /*  字符串哈希错误              */
                printk(KERN_ERR"%s: string hash error\n", __func__);
                return -1;
            }
        }

        if (hash_tbl_insert(tbl, key, symbol) < 0) {                    /*  将符号加到符号表            */
            printk(KERN_ERR"%s: failed to insert symbol %s to symbol hash table\n", __func__, symbol->name);
            return -1;
        }
    }
    return 0;
}
#endif
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
