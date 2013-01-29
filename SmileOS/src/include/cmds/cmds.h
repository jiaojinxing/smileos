/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <jiaojinxing1987@gmail.com>
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
** File name:               cmds.h
** Last modified Date:      2013-1-5
** Last Version:            1.0.0
** Descriptions:            命令头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-1-5
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
#ifndef CMDS_H_
#define CMDS_H_

/*
 * 命令结构
 */
typedef struct {
    char   *name;                                                       /*  名字                      　*/
    int   (*main)(int, char *[]);                                       /*  命令功能函数              　*/
} cmd_t;

/*
 * 命令属性
 */
#define cmd_attr     __attribute__ ((unused, section (".smileos_cmd")))

/*
 * SmileOS 命令
 */
#define SMILEOS_CMD(name) \
extern int main_##name(int, char **); \
cmd_t   smileos_cmd_##name  cmd_attr = {#name, main_##name}

#endif                                                                  /*  CMDS_H_                     */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
