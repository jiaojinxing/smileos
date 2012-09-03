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
** File name:               ff_smileos.c
** Last modified Date:      2012-3-18
** Last Version:            1.0.0
** Descriptions:            FatFS 系统模拟层
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-17
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/ipc.h"
#include "ff.h"
#include "diskio.h"

/*------------------------------------------------------------------------*/
/* Create a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount function to create a new
/  synchronization object, such as semaphore and mutex. When a zero is
/  returned, the f_mount function fails with FR_INT_ERR.
*/
int ff_cre_syncobj (    /* TRUE:Function succeeded, FALSE:Could not create due to any error */
    BYTE vol,           /* Corresponding logical drive being processed */
    _SYNC_t* sobj       /* Pointer to return the created sync object */
)
{
    return !mutex_new(sobj);
}

/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount function to delete a synchronization
/  object that created with ff_cre_syncobj function. When a zero is
/  returned, the f_mount function fails with FR_INT_ERR.
*/
int ff_del_syncobj (    /* TRUE:Function succeeded, FALSE:Could not delete due to any error */
    _SYNC_t sobj        /* Sync object tied to the logical drive to be deleted */
)
{
    return !mutex_free(&sobj);
}

/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a zero is returned, the file function fails with FR_TIMEOUT.
*/
int ff_req_grant (      /* TRUE:Got a grant to access the volume, FALSE:Could not get a grant */
    _SYNC_t sobj        /* Sync object to wait */
)
{
    return !mutex_lock(&sobj, _FS_TIMEOUT);
}

/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/
void ff_rel_grant (
    _SYNC_t sobj        /* Sync object to be signaled */
)
{
    mutex_unlock(&sobj);
}

/*------------------------------------------------------------------------*/
/* Allocate a memory block                                                */
/*------------------------------------------------------------------------*/
/* If a NULL is returned, the file function fails with FR_NOT_ENOUGH_CORE.
*/
void* ff_memalloc (     /* Returns pointer to the allocated memory block */
    UINT size           /* Number of bytes to allocate */
)
{
    return kmalloc(size);
}

/*------------------------------------------------------------------------*/
/* Free a memory block                                                    */
/*------------------------------------------------------------------------*/
void ff_memfree(
    void* mblock        /* Pointer to the memory block to free */
)
{
    kfree(mblock);
}

DWORD get_fattime (
    void
)
{
    return 0;
}

DSTATUS disk_initialize (
    BYTE Drive          /* Physical drive number */
)
{
    return RES_OK;
}

DSTATUS disk_status (
    BYTE Drive          /* Physical drive number */
)
{
    return RES_OK;
}

DRESULT disk_read (
    BYTE Drive,         /* Physical drive number */
    BYTE* Buffer,       /* Pointer to the read data buffer */
    DWORD SectorNumber, /* Start sector number */
    BYTE SectorCount    /* Number of sectros to read */
)
{
    extern int sd_read_block(uint32_t blk_nr, uint32_t blk_cnt, uint8_t *buf);
    int ret = sd_read_block(SectorNumber, SectorCount, Buffer);
    if (ret < 0) {
        return RES_ERROR;
    } else {
        return RES_OK;
    }
}

DRESULT disk_write (
    BYTE Drive,         /* Physical drive number */
    const BYTE* Buffer, /* Pointer to the write data (may be non aligned) */
    DWORD SectorNumber, /* Sector number to write */
    BYTE SectorCount    /* Number of sectors to write */
)
{
    extern int sd_write_block(uint32_t blk_nr, uint32_t blk_cnt, const uint8_t *buf);
    int ret = sd_write_block(SectorNumber, SectorCount, Buffer);
    if (ret < 0) {
        return RES_ERROR;
    } else {
        return RES_OK;
    }
}

DRESULT disk_ioctl (
    BYTE Drive,         /* Drive number */
    BYTE Command,       /* Control command code */
    void* Buffer        /* Parameter and data buffer */
)
{
    return RES_OK;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
