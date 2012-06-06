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
** File name:               posix_fix.c
** Last modified Date:      2012-5-3
** Last Version:            1.0.0
** Descriptions:            posix 修正
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-5-3
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
#include <fcntl.h>
#include <reent.h>
#include <unistd.h>

/*
 * 创建文件
 */
int
creat(const char *path,
      mode_t mode)
{
    return open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
}

/*
 * 创建目录
 */
int
mkdir(const char *path,
      mode_t mode)
{
    return _mkdir_r(_REENT, path, mode);
}

/*
 * 删除目录
 */
int
rmdir(const char *path)
{
    extern int _rmdir_r(struct _reent *reent, const char *path);

    return _rmdir_r(_REENT, path);
}

/*
 * 判断文件是不是一个 TTY
 */
int
isatty(int fd)
{
    return _isatty_r(_REENT, fd);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
