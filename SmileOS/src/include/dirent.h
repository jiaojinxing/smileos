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
** File name:               dirent.h
** Last modified Date:      2012-3-22
** Last Version:            1.0.0
** Descriptions:            Ŀ¼��
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-22
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
#ifndef DIRENT_H_
#define DIRENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <syslimits.h>
#include "vfs/types.h"
#include <sys/types.h>

#define DT_UNKNOWN  0
#define DT_FIFO     1
#define DT_CHR      2
#define DT_DIR      4
#define DT_BLK      6
#define DT_REG      8
#define DT_LNK      10
#define DT_SOCK     12
#define DT_WHT      14

#define ATTR_MODE   1
#define ATTR_UID    2
#define ATTR_GID    4
#define ATTR_SIZE   8
#define ATTR_ATIME  16
#define ATTR_MTIME  32
#define ATTR_CTIME  64

struct iattr {
    unsigned int ia_valid;
    unsigned ia_mode;
    unsigned ia_uid;
    unsigned ia_gid;
    unsigned ia_size;
    unsigned ia_atime;
    unsigned ia_mtime;
    unsigned ia_ctime;
    unsigned int ia_attr_flags;
};

/*
 * Ŀ¼��
 */
struct dirent {
    long   d_ino;
    char   d_name[NAME_MAX];
};
/*********************************************************************************************************
** Function name:           opendir
** Descriptions:            ��Ŀ¼
** input parameters:        path                Ŀ¼ PATH
** output parameters:       NONE
** Returned value:          Ŀ¼ָ��
*********************************************************************************************************/
DIR *opendir(const char *path);
/*********************************************************************************************************
** Function name:           closedir
** Descriptions:            �ر�Ŀ¼
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int closedir(DIR *dir);
/*********************************************************************************************************
** Function name:           readdir
** Descriptions:            ��Ŀ¼��
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          Ŀ¼��
*********************************************************************************************************/
struct dirent *readdir(DIR *dir);
/*********************************************************************************************************
** Function name:           rewinddir
** Descriptions:            ����Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int rewinddir(DIR *dir);
/*********************************************************************************************************
** Function name:           rewinddir
** Descriptions:            ����Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
**                          loc                 �µĶ���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int seekdir(DIR *dir, long loc);
/*********************************************************************************************************
** Function name:           telldir
** Descriptions:            ���Ŀ¼����
** input parameters:        dir                 Ŀ¼ָ��
** output parameters:       NONE
** Returned value:          ��ǰ����
*********************************************************************************************************/
long telldir(DIR *dir);

#ifdef __cplusplus
}
#endif

#endif                                                                  /*  DIRENT_H_                   */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
