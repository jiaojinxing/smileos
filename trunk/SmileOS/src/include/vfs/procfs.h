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
** File name:               procfs.h
** Last modified Date:      2012-10-25
** Last Version:            1.0.0
** Descriptions:            PROCFS
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-10-25
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
#ifndef VFS_PROCFS_H_
#define VFS_PROCFS_H_

#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************************************************
** 节点
*********************************************************************************************************/
struct _procfs_node;
typedef struct _procfs_node procfs_node_t;
/*********************************************************************************************************
** 节点读写函数类型
*********************************************************************************************************/
typedef ssize_t (*procfs_node_read_t)(void *ctx, void *buf, size_t len);
typedef ssize_t (*procfs_node_write_t)(void *ctx, const void *buf, size_t len);
/*********************************************************************************************************
** Function name:           procfs_creat
** Descriptions:            创建 procfs 文件系统文件
** input parameters:        dir_node            目录节点
**                          read                读节点函数
**                          write               写节点函数
**                          ctx                 上下文
**                          name                名字
** output parameters:       NONE
** Returned value:          procfs 文件系统文件节点
*********************************************************************************************************/
procfs_node_t *procfs_creat(procfs_node_t *dir_node,
                            procfs_node_read_t read,
                            procfs_node_write_t write,
                            void *ctx,
                            const char *name);
/*********************************************************************************************************
** Function name:           procfs_mkdir
** Descriptions:            创建 procfs 文件系统目录
** input parameters:        dir_node            目录节点
**                          name                名字
** output parameters:       NONE
** Returned value:          procfs 文件系统目录节点
*********************************************************************************************************/
procfs_node_t *procfs_mkdir(procfs_node_t *dir_node, const char *name);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  VFS_PROCFS_H_               */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
