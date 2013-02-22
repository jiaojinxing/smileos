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
** File name:               sys_symbol_tbl.c
** Last modified Date:      2012-7-18
** Last Version:            1.0.0
** Descriptions:            SmileOS ����ϵͳ���ű�
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
#include "kern/func_config.h"
#if CONFIG_MODULE_EN > 0
#include "kern/kern.h"
#include "kern/hash_tbl.h"
#include "module/symbol.h"
#include <string.h>
/*********************************************************************************************************
** ���ű�
*********************************************************************************************************/
static hash_tbl_t *text_symbol_tbl;                                     /*  TEXT �η��ű�               */
static hash_tbl_t *data_symbol_tbl;                                     /*  DATA �η��ű�               */
/*********************************************************************************************************
** Function name:           sys_symbol_lookup
** Descriptions:            ����ϵͳ����
** input parameters:        name                ������
**                          type                ��������
** output parameters:       NONE
** Returned value:          ���ŵ�ַ OR NULL
*********************************************************************************************************/
void *sys_symbol_lookup(const char *name, uint8_t type)
{
    hash_node_t *node;

    node = hash_tbl_lookup(type == SYMBOL_TEXT ? text_symbol_tbl : data_symbol_tbl, bkdr_hash(name));
    if (node != NULL) {
        symbol_t *symbol = node->data;
        return symbol->addr;
    }
    return NULL;
}
/*********************************************************************************************************
** Function name:           sys_symbol_tbl_init
** Descriptions:            ��ʼ��ϵͳ���ű�
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sys_symbol_tbl_init(void)
{
    hash_tbl_t     *tbl;
    symbol_t       *symbol;
    hash_node_t    *node;
    unsigned int    key;

    text_symbol_tbl = hash_tbl_create(127);                             /*  ���� TEXT �η��ű�          */
    if (text_symbol_tbl == NULL) {
        printk(KERN_ERR"%s: failed to create system text symbol hash table\n", __func__);
        return -1;
    }

    data_symbol_tbl = hash_tbl_create(127);                             /*  ���� DATA �η��ű�          */
    if (data_symbol_tbl == NULL) {
        printk(KERN_ERR"%s: failed to create system data symbol hash table\n", __func__);
        return -1;
    }

    /*
     * ��ϵͳ���ű��еķ��żӵ���Ӧ�ķ��ű���
     */
    extern symbol_t sys_symbol_tbl[];
    for (symbol = sys_symbol_tbl; symbol->name != NULL; symbol++) {

        tbl  = symbol->flags == SYMBOL_TEXT ? text_symbol_tbl : data_symbol_tbl;

        key  = bkdr_hash(symbol->name);

        node = hash_tbl_lookup(tbl, key);                               /*  �Ȳ��Ҹ÷���                */

        if (NULL != node) {

            symbol_t *temp = node->data;

            if (strcmp(temp->name, symbol->name) == 0) {                /*  �����ظ���                ��*/
                continue;
            } else {                                                    /*  �ַ�����ϣ����              */
                printk(KERN_ERR"%s: string hash error\n", __func__);
                return -1;
            }
        }

        if (hash_tbl_insert(tbl, key, symbol) < 0) {                    /*  �����żӵ����ű�            */
            printk(KERN_ERR"%s: failed to insert symbol %s to system symbol hash table\n", __func__, symbol->name);
            return -1;
        }
    }
    return 0;
}
#endif
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
