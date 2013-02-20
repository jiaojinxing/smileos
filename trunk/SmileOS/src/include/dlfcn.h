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
** File name:               dlfcn.h
** Last modified Date:      2013-2-20
** Last Version:            1.0.0
** Descriptions:            动态链接库头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-2-20
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
**　http://pubs.opengroup.org/onlinepubs/009695399/functions/dlopen.html
**
*********************************************************************************************************/
#ifndef DLFCN_H_
#define DLFCN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Relocations are performed at an implementation-defined time.
 */
#define RTLD_LAZY       (1 << 0)

/*
 * Relocations are performed when the object is loaded.
 */
#define RTLD_NOW        (1 << 1)

/*
 * All symbols are available for relocation processing of other modules.
 */
#define RTLD_GLOBAL     (1 << 2)

/*
 * All symbols are not made available for relocation processing by other modules.
 */
#define RTLD_LOCAL      (1 << 3)

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/dlopen.html
 */
void  *dlopen(const char *path, int mode);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/dlsym.html
 */
void  *dlsym(void *mod, const char *name);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/dlerror.html
 */
char  *dlerror(void);

/*
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/dlclose.html
 */
int    dlclose(void *mod);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  DLFCN_H_                    */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
