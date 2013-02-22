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
** File name:               module.h
** Last modified Date:      2012-7-18
** Last Version:            1.0.0
** Descriptions:            内核模块
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
#ifndef MODULE_H_
#define MODULE_H_

#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************************************************
** 模块
*********************************************************************************************************/
struct module;
typedef struct module module_t;
/*********************************************************************************************************
** Function name:           module_load
** Descriptions:            加载内核模块
** input parameters:        path                内核模块文件路径
**                          argc                参数个数
**                          argv                参数数组
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_load(const char *path, int argc, char **argv);
/*********************************************************************************************************
** Function name:           module_unload
** Descriptions:            卸载内核模块
** input parameters:        path                内核模块文件路径
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_unload(const char *path);
/*********************************************************************************************************
** Function name:           module_unref
** Descriptions:            解取对内核模块的引用
** input parameters:        mod                 内核模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_unref(module_t *mod);
/*********************************************************************************************************
** Function name:           module_ref_by_addr
** Descriptions:            通过内核模块的一个地址引用内核模块
** input parameters:        addr                地址
** output parameters:       NONE
** Returned value:          内核模块 OR NULL
*********************************************************************************************************/
module_t *module_ref_by_addr(void *addr);
/*********************************************************************************************************
** Function name:           module_open
** Descriptions:            打开应用模块
** input parameters:        path                应用模块文件路径
**                          mode                加载模式
** output parameters:       NONE
** Returned value:          模块 OR NULL
*********************************************************************************************************/
module_t *module_open(const char *path, int mode);
/*********************************************************************************************************
** Function name:           module_close
** Descriptions:            关闭应用模块
** input parameters:        mod                 应用模块
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int module_close(module_t *mod);
/*********************************************************************************************************
** Function name:           module_symbol
** Descriptions:            在模块里查找符号
** input parameters:        mod                 模块
**                          name                符号名
** output parameters:       NONE
** Returned value:          符号地址 OR NULL
*********************************************************************************************************/
void *module_symbol(module_t *mod, const char *name);
/*********************************************************************************************************
** Function name:           symbol_name
** Descriptions:            查找与指定地址最接近的符号
** input parameters:        addr                指定地址
** output parameters:       pdiff               差值
** Returned value:          符号名
*********************************************************************************************************/
const char *symbol_name(mem_ptr_t addr, mem_ptr_t *pdiff);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  MODULE_H_                   */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
