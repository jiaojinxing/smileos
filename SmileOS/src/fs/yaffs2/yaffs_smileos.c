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
** File name:               yaffs_smileos.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            yaffs 文件系统系统模拟层
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-27
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
#include "kern/kern.h"
#include "kern/ipc.h"
#include "vfs/types.h"
#include "./src/yaffs_guts.h"

#include <errno.h>

unsigned int    yaffs_traceMask = 0xF0000000;                           /*  TARCE 屏蔽掩码              */

static mutex_t  yaffs_lock;                                             /*  锁                          */

/*********************************************************************************************************
** Function name:           yaffsfs_SetError
** Descriptions:            设置 errno
** input parameters:        err                 错误码
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_SetError(int err)
{
    seterrno(-err);
}
/*********************************************************************************************************
** Function name:           yaffsfs_Lock
** Descriptions:            上锁
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_Lock(void)
{
    mutex_lock(&yaffs_lock, 0);
}
/*********************************************************************************************************
** Function name:           yaffsfs_Unlock
** Descriptions:            解锁
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_Unlock(void)
{
    mutex_unlock(&yaffs_lock);
}
/*********************************************************************************************************
** Function name:           yaffsfs_CurrentTime
** Descriptions:            获得当前时间
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          当前时间
*********************************************************************************************************/
__u32 yaffsfs_CurrentTime(void)
{
    return 0;
}
/*********************************************************************************************************
** Function name:           yaffs_malloc
** Descriptions:            分配内存
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          内存
*********************************************************************************************************/
void *yaffs_malloc(size_t size)
{
    return kmalloc(size, GFP_KERNEL);
}
/*********************************************************************************************************
** Function name:           yaffs_free
** Descriptions:            释放内存
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffs_free(void *ptr)
{
    kfree(ptr);
}
/*********************************************************************************************************
** Function name:           yaffsfs_LocalInitialisation
** Descriptions:            本地初始化
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_LocalInitialisation(void)
{
    mutex_new(&yaffs_lock);
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
