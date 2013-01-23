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
** Descriptions:            FatFS 文件系统系统模拟层
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
    void* drive         /* Physical drive number */
)
{
    device_t *dev = (device_t *)drive;
    int ret;

    if (dev == NULL ||
        dev->drv == NULL ||
        dev->drv->ioctl == NULL) {
        return RES_ERROR;
    }

    ret = dev->drv->ioctl(dev->ctx, NULL, BLKDEV_CMD_INIT, NULL);
    if (ret < 0) {
        return STA_NODISK;
    } else {
        return RES_OK;
    }
}

DSTATUS disk_status (
    void* drive         /* Physical drive number */
)
{
    device_t *dev = (device_t *)drive;
    int ret;
    int status;

    if (dev == NULL ||
        dev->drv == NULL ||
        dev->drv->ioctl == NULL) {
        return RES_ERROR;
    }

    ret = dev->drv->ioctl(dev->ctx, NULL, BLKDEV_CMD_STATUS, &status);
    if (ret < 0) {
        return RES_ERROR;
    } else {
        switch (status) {
        case BLKDEV_STA_NOINIT:
            return STA_NOINIT;

        case BLKDEV_STA_NODISK:
            return STA_NODISK;

        case BLKDEV_STA_PROTECT:
            return STA_PROTECT;

        default:
            return RES_OK;
        }
    }
}

DRESULT disk_read (
    void* drive,        /* Physical drive number */
    BYTE* Buffer,       /* Pointer to the read data buffer */
    DWORD SectorNumber, /* Start sector number */
    BYTE SectorCount    /* Number of sectros to read */
)
{
    device_t *dev = (device_t *)drive;
    int ret;

    if (dev == NULL ||
        dev->drv == NULL ||
        dev->drv->readblk == NULL) {
        return RES_ERROR;
    }

    ret = dev->drv->readblk(dev->ctx, NULL, SectorNumber, SectorCount, Buffer);
    if (ret < 0) {
        return RES_ERROR;
    } else {
        return RES_OK;
    }
}

DRESULT disk_write (
    void* drive,        /* Physical drive number */
    const BYTE* Buffer, /* Pointer to the write data (may be non aligned) */
    DWORD SectorNumber, /* Sector number to write */
    BYTE SectorCount    /* Number of sectors to write */
)
{
    device_t *dev = (device_t *)drive;
    int ret;

    if (dev == NULL ||
        dev->drv == NULL ||
        dev->drv->writeblk == NULL) {
        return RES_ERROR;
    }

    ret = dev->drv->writeblk(dev->ctx, NULL, SectorNumber, SectorCount, Buffer);
    if (ret < 0) {
        return RES_ERROR;
    } else {
        return RES_OK;
    }
}

DRESULT disk_ioctl (
    void* drive,        /* Drive number */
    BYTE Command,       /* Control command code */
    void* Buffer        /* Parameter and data buffer */
)
{
    device_t *dev = (device_t *)drive;
    int ret;
    int cmd;

    if (dev == NULL ||
        dev->drv == NULL ||
        dev->drv->ioctl == NULL) {
        return RES_ERROR;
    }

    switch (Command) {
    case CTRL_SYNC:
        cmd = BLKDEV_CMD_SYNC;
        break;

    case GET_SECTOR_COUNT:
        cmd = BLKDEV_CMD_SECT_NR;
        break;

    case GET_SECTOR_SIZE:
        cmd = BLKDEV_CMD_SECT_SZ;
        break;

    case GET_BLOCK_SIZE:
        cmd = BLKDEV_CMD_BLK_SZ;
        break;

    case CTRL_ERASE_SECTOR:
        cmd = BLKDEV_CMD_ERASE;
        break;

    default:
        return RES_ERROR;
    }

    ret = dev->drv->ioctl(dev->ctx, NULL, cmd, Buffer);
    if (ret < 0) {
        return RES_ERROR;
    } else {
        return RES_OK;
    }
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
