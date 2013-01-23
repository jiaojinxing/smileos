
#ifndef __YAFFS_FLASH_H__
#define __YAFFS_FLASH_H__

#include "yaffs_guts.h"
int yflash_EraseBlockInNAND(yaffs_Device *dev, int blockNumber);
int yflash_WriteChunkToNAND(yaffs_Device *dev,int chunkInNAND,const __u8 *data, const yaffs_Spare *spare);
int yflash_ReadChunkFromNAND(yaffs_Device *dev,int chunkInNAND, __u8 *data, yaffs_Spare *spare);
int yflash_EraseBlockInNAND(yaffs_Device *dev, int blockNumber);
int yflash_InitialiseNAND(yaffs_Device *dev);

#endif
