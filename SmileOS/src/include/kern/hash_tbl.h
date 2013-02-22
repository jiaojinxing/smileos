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
#ifndef HASH_TBL_H_
#define HASH_TBL_H_

#include <sys/types.h>
/*********************************************************************************************************
** 数据类型定义
*********************************************************************************************************/
/*
 * 哈希节点
 */
struct hash_node;
typedef struct hash_node hash_node_t;

struct hash_node {
    unsigned int        key;                                            /*  键值                        */
    void               *data;                                           /*  数据                        */
    struct hash_node   *next;                                           /*  后趋                        */
};

/*
 * 哈希表
 */
typedef struct {
    size_t              size;                                           /*  大小                        */
    hash_node_t        *lists[1];                                       /*  节点链表数组                */
} hash_tbl_t;
/*********************************************************************************************************
** Function name:           hash_tbl_create
** Descriptions:            创建哈希表
** input parameters:        size                大小
** output parameters:       NONE
** Returned value:          哈希表 OR NULL
*********************************************************************************************************/
hash_tbl_t *hash_tbl_create(size_t size);
/*********************************************************************************************************
** Function name:           hash_tbl_destroy
** Descriptions:            销毁哈希表
** input parameters:        tbl                 哈希表
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int hash_tbl_destroy(hash_tbl_t *tbl);
/*********************************************************************************************************
** Function name:           hash_tbl_lookup
** Descriptions:            在哈希表中查找
** input parameters:        tbl                 哈希表
**                          key                 键值
** output parameters:       NONE
** Returned value:          节点 OR NULL
*********************************************************************************************************/
hash_node_t *hash_tbl_lookup(hash_tbl_t *tbl, unsigned int key);
/*********************************************************************************************************
** Function name:           hash_tbl_insert
** Descriptions:            在哈希表中插入
** input parameters:        tbl                 哈希表
**                          key                 键值
**                          data                数据
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int hash_tbl_insert(hash_tbl_t *tbl, unsigned int key, void *data);
/*********************************************************************************************************
** Function name:           bkdr_hash
** Descriptions:            BKDR Hash Function
**                          各种字符串 Hash 函数比较
**                          http://www.byvoid.com/blog/string-hash-compare/
** input parameters:        str                 字符串
** output parameters:       NONE
** Returned value:          BKDR Hash
*********************************************************************************************************/
unsigned int bkdr_hash(const char *str);

#endif                                                                  /*  HASH_TBL_H_                 */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
