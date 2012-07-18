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
** File name:               main.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            主函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
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
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int a = 1111;
int b = 0;
static int c = 10;
static int d;

extern int e;
extern int f;
/*
 * main 函数
 */
int main(int argc, char *argv[])
{
    extern int hello(void);
    hello();
    extern int hellob(void);
    hellob();
    e = -1;
    f = 0xaa;
    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
