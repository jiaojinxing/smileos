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
#include "vfs/vfs.h"
#include "./src/yaffs_guts.h"
#include "./src/yaffs_mtdif.h"
#include "./src/yaffs_mtdif1.h"
#include "./src/yaffs_mtdif2.h"
#include "./src/yaffs_linux.h"
#include "./src/yaffs_trace.h"
#include "./src/yaffsfs.h"
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <errno.h>
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
unsigned int    yaffs_traceMask = 0xF0000000;                           /*  TARCE 屏蔽掩码              */

static mutex_t  yaffs_lock;                                             /*  锁                          */

static YLIST_HEAD(yaffs_context_list);                                  /*  YAFFS 上下文链表            */
/*********************************************************************************************************
** Function name:           yaffsfs_SetError
** Descriptions:            yaffs 设置 errno
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
** Descriptions:            yaffs 上锁
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
** Descriptions:            yaffs 解锁
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
** Descriptions:            yaffs 获得当前时间
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
** Descriptions:            yaffs 分配内存
** input parameters:        size                内存大小
** output parameters:       NONE
** Returned value:          内存指针
*********************************************************************************************************/
void *yaffs_malloc(size_t size)
{
    return kmalloc(size, GFP_KERNEL);
}
/*********************************************************************************************************
** Function name:           yaffs_free
** Descriptions:            yaffs 释放内存
** input parameters:        ptr                 内存指针
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffs_free(void *ptr)
{
    kfree(ptr);
}
/*********************************************************************************************************
** Function name:           yaffsfs_Init
** Descriptions:            初始化 yaffs
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int yaffsfs_Init(void)
{
    extern int yaffsfs_InitHandles(void);
    yaffsfs_InitHandles();

    return mutex_new(&yaffs_lock);
}
/*********************************************************************************************************
** Function name:           yaffsfs_FindDevice
** Descriptions:            yaffs 查找设备
** input parameters:        path                路径
** output parameters:       restOfPath          余下的路径
** Returned value:          YAFFS 设备
*********************************************************************************************************/
yaffs_Device *yaffsfs_FindDevice(const YCHAR *path, YCHAR **restOfPath)
{
    struct ylist_head *item;
    const YCHAR *leftOver;
    const YCHAR *p;
    yaffs_Device *retval = NULL;
    int thisMatchLength;
    int longestMatch = -1;
    int matching;

    extern int yaffsfs_Match(YCHAR a, YCHAR b);
    extern int yaffsfs_IsPathDivider(YCHAR ch);

    // Check all context, choose the one that:
    // 1) Actually matches a prefix (ie /a amd /abc will not match
    // 2) Matches the longest.
    ylist_for_each(item, &yaffs_context_list) {
        struct yaffs_LinuxContext *dc = ylist_entry(item, struct yaffs_LinuxContext, contextList);
        yaffs_Device *dev = dc->dev;

        leftOver = path;
        p = dev->param.name;
        thisMatchLength = 0;
        matching = 1;

        while(matching && *p && *leftOver){
            // Skip over any /s
            while (yaffsfs_IsPathDivider(*p))
                  p++;

            // Skip over any /s
            while (yaffsfs_IsPathDivider(*leftOver))
                      leftOver++;

            // Now match the text part
            while (matching &&
                   *p && !yaffsfs_IsPathDivider(*p) &&
                   *leftOver && !yaffsfs_IsPathDivider(*leftOver)) {
                    if (yaffsfs_Match(*p,*leftOver)) {
                        p++;
                        leftOver++;
                        thisMatchLength++;
                } else {
                    matching = 0;
                }
            }
        }

        // Skip over any /s in leftOver
        while (yaffsfs_IsPathDivider(*leftOver))
                  leftOver++;

        if (matching && (thisMatchLength > longestMatch)) {
            // Matched prefix
            *restOfPath = (YCHAR *)leftOver;
            retval = dev;
            longestMatch = thisMatchLength;
        }
    }
    return retval;
}
/*********************************************************************************************************
** YAFFS 选项
*********************************************************************************************************/
typedef struct {
    int inband_tags;
    int skip_checkpoint_read;
    int skip_checkpoint_write;
    int no_cache;
    int tags_ecc_on;
    int tags_ecc_overridden;
    int lazy_loading_enabled;
    int lazy_loading_overridden;
    int empty_lost_and_found;
    int empty_lost_and_found_overridden;
} yaffs_options;
/*********************************************************************************************************
** Function name:           yaffs_parse_options
** Descriptions:            yaffs 分析挂载选项字符串
** input parameters:        options_str         挂载选项字符串
** output parameters:       options             YAFFS 挂载选项
** Returned value:          0 OR 1
*********************************************************************************************************/
#define MAX_OPT_LEN 30
static int yaffs_parse_options(yaffs_options *options, const char *options_str)
{
    char cur_opt[MAX_OPT_LEN + 1];
    int p;
    int error = 0;

    /* Parse through the options which is a comma seperated list */

    while (options_str && *options_str && !error) {
        memset(cur_opt, 0, MAX_OPT_LEN + 1);
        p = 0;

        while (*options_str == ',')
            options_str++;

        while (*options_str && *options_str != ',') {
            if (p < MAX_OPT_LEN) {
                cur_opt[p] = *options_str;
                p++;
            }
            options_str++;
        }

        if (!strcmp(cur_opt, "inband-tags"))
            options->inband_tags = 1;
        else if (!strcmp(cur_opt, "tags-ecc-off")){
            options->tags_ecc_on = 0;
            options->tags_ecc_overridden=1;
        } else if (!strcmp(cur_opt, "tags-ecc-on")){
            options->tags_ecc_on = 1;
            options->tags_ecc_overridden = 1;
        } else if (!strcmp(cur_opt, "lazy-loading-off")){
            options->lazy_loading_enabled = 0;
            options->lazy_loading_overridden=1;
        } else if (!strcmp(cur_opt, "lazy-loading-on")){
            options->lazy_loading_enabled = 1;
            options->lazy_loading_overridden = 1;
        } else if (!strcmp(cur_opt, "empty-lost-and-found-off")){
            options->empty_lost_and_found = 0;
            options->empty_lost_and_found_overridden=1;
        } else if (!strcmp(cur_opt, "empty-lost-and-found-on")){
            options->empty_lost_and_found = 1;
            options->empty_lost_and_found_overridden=1;
        } else if (!strcmp(cur_opt, "no-cache"))
            options->no_cache = 1;
        else if (!strcmp(cur_opt, "no-checkpoint-read"))
            options->skip_checkpoint_read = 1;
        else if (!strcmp(cur_opt, "no-checkpoint-write"))
            options->skip_checkpoint_write = 1;
        else if (!strcmp(cur_opt, "no-checkpoint")) {
            options->skip_checkpoint_read = 1;
            options->skip_checkpoint_write = 1;
        } else {
            printk(KERN_INFO "yaffs: Bad mount option \"%s\"\n",
                    cur_opt);
            error = 1;
        }
    }

    return error;
}
/*********************************************************************************************************
** Function name:           yaffsfs_Mount
** Descriptions:            挂载 yaffs 文件系统
** input parameters:        yaffsVersion        yaffs 版本
**                          point_name          挂载点名
**                          dev_name            设备名
**                          data_str            挂载选项字符串
** output parameters:       NONE
** Returned value:          上下文
*********************************************************************************************************/
void *yaffsfs_Mount(int yaffsVersion,
                    const char *point_name,
                    const char *dev_name,
                    const char *data_str)
{
    yaffs_Device *dev;
    struct stat st;
    yaffs_options options;
    struct mtd_info *mtd;
    struct yaffs_LinuxContext *context;
    yaffs_DeviceParam *param;
    int nBlocks;

    printk(KERN_INFO "yaffs: dev name is \"%s\"\n", dev_name);

    if (data_str == NULL) {
        data_str = "";
    }

    printk(KERN_INFO "yaffs: passed flags \"%s\"\n", data_str);

    memset(&options, 0, sizeof(options));

    if (yaffs_parse_options(&options, data_str)) {
        return NULL;
    }

    if (vfs_stat(dev_name, &st) < 0) {
        T(YAFFS_TRACE_ALWAYS, ("yaffs: MTD device \"%s\" doesn't appear to exist\n", dev_name));
        return NULL;
    }

    T(YAFFS_TRACE_OS, ("yaffs: Using yaffs%d\n", yaffsVersion));
    T(YAFFS_TRACE_OS, ("yaffs: block size %d\n", (int)(st.st_blksize)));

#ifdef CONFIG_YAFFS_DISABLE_WRITE_VERIFY
    T(YAFFS_TRACE_OS, ("yaffs: Write verification disabled, All guarantees null and void\n"));
#endif

    T(YAFFS_TRACE_ALWAYS, ("yaffs: Attempting MTD mount on \"%s\"\n", dev_name));

    /* Get the device */
    mtd = (struct mtd_info *)st.st_spare1;
    if (mtd == NULL) {
        T(YAFFS_TRACE_ALWAYS, ("yaffs: MTD device \"%s\" doesn't appear to exist\n", dev_name));
        return NULL;
    }

    /* Check it's NAND */
    if (mtd->type != MTD_NANDFLASH) {
        T(YAFFS_TRACE_ALWAYS, ("yaffs: MTD device is not NAND, it's type %d\n", mtd->type));
        return NULL;
    }

    T(YAFFS_TRACE_OS, (" erase %p\n", mtd->erase));
    T(YAFFS_TRACE_OS, (" read %p\n", mtd->read));
    T(YAFFS_TRACE_OS, (" write %p\n", mtd->write));
    T(YAFFS_TRACE_OS, (" readoob %p\n", mtd->read_oob));
    T(YAFFS_TRACE_OS, (" writeoob %p\n", mtd->write_oob));
    T(YAFFS_TRACE_OS, (" block_isbad %p\n", mtd->block_isbad));
    T(YAFFS_TRACE_OS, (" block_markbad %p\n", mtd->block_markbad));
#define WRITE_SIZE_STR "writesize"
#define WRITE_SIZE(mtd) ((mtd)->writesize)
    T(YAFFS_TRACE_OS, (" %s %d\n", WRITE_SIZE_STR, WRITE_SIZE(mtd)));
    T(YAFFS_TRACE_OS, (" oobsize %d\n", mtd->oobsize));
    T(YAFFS_TRACE_OS, (" erasesize %d\n", mtd->erasesize));
    T(YAFFS_TRACE_OS, (" size %d\n", mtd->size));

#ifdef CONFIG_YAFFS_AUTO_YAFFS2

    if (yaffsVersion == 1 && WRITE_SIZE(mtd) >= 2048) {
        T(YAFFS_TRACE_ALWAYS, ("yaffs: auto selecting yaffs2\n"));
        yaffsVersion = 2;
    }

    /* Added NCB 26/5/2006 for completeness */
    if (yaffsVersion == 2 && !options.inband_tags && WRITE_SIZE(mtd) == 512) {
        T(YAFFS_TRACE_ALWAYS, ("yaffs: auto selecting yaffs1\n"));
        yaffsVersion = 1;
    }

#endif

    if (yaffsVersion == 2) {
        /* Check for version 2 style functions */
        if (!mtd->erase ||
            !mtd->block_isbad ||
            !mtd->block_markbad ||
            !mtd->read ||
            !mtd->write ||
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
            !mtd->read_oob || !mtd->write_oob) {
#else
            !mtd->write_ecc ||
            !mtd->read_ecc || !mtd->read_oob || !mtd->write_oob) {
#endif
            T(YAFFS_TRACE_ALWAYS, ("yaffs: MTD device does not support required functions\n"));;
            return NULL;
        }

        if ((WRITE_SIZE(mtd) < YAFFS_MIN_YAFFS2_CHUNK_SIZE ||
            mtd->oobsize < YAFFS_MIN_YAFFS2_SPARE_SIZE) &&
            !options.inband_tags) {
            T(YAFFS_TRACE_ALWAYS, ("yaffs: MTD device does not have the right page sizes\n"));
            return NULL;
        }
    } else {
        /* Check for V1 style functions */
        if (!mtd->erase ||
            !mtd->read ||
            !mtd->write ||
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
            !mtd->read_oob || !mtd->write_oob) {
#else
            !mtd->write_ecc ||
            !mtd->read_ecc || !mtd->read_oob || !mtd->write_oob) {
#endif
            T(YAFFS_TRACE_ALWAYS, ("yaffs: MTD device does not support required functions\n"));;
            return NULL;
        }

        if (WRITE_SIZE(mtd) < YAFFS_BYTES_PER_CHUNK ||
            mtd->oobsize != YAFFS_BYTES_PER_SPARE) {
            T(YAFFS_TRACE_ALWAYS, ("yaffs: MTD device does not support have the right page sizes\n"));
            return NULL;
        }
    }

    /* OK, so if we got here, we have an MTD that's NAND and looks
     * like it has the right capabilities
     * Set the yaffs_Device up for mtd
     */

    dev = kzalloc(sizeof(yaffs_Device) + sizeof(struct yaffs_LinuxContext), GFP_KERNEL);
    if (dev == NULL) {
        T(YAFFS_TRACE_ALWAYS, ("yaffs: Failed trying to allocate yaffs_Device\n"));
        return NULL;
    }

    context = (struct yaffs_LinuxContext *)((char *)dev + sizeof(yaffs_Device));

    context->dev = dev;

    dev->context = (void *)context;

    YINIT_LIST_HEAD(&(context->contextList));

    param = &dev->param;

    yaffs_DeviceToContext(dev)->mtd = mtd;
    param->name = point_name;

    /* Set up the memory size parameters.... */

#define YCALCBLOCKS(s, b) ((s)/(b))
    nBlocks = YCALCBLOCKS(mtd->size, (YAFFS_CHUNKS_PER_BLOCK * YAFFS_BYTES_PER_CHUNK));

    param->startBlock = 0;
    param->endBlock = nBlocks - 1;
    param->nChunksPerBlock = YAFFS_CHUNKS_PER_BLOCK;
    param->totalBytesPerChunk = YAFFS_BYTES_PER_CHUNK;
    param->nReservedBlocks = 5;
    param->nShortOpCaches = (options.no_cache) ? 0 : 10;
    param->inbandTags = options.inband_tags;

#ifdef CONFIG_YAFFS_DISABLE_LAZY_LOAD
    param->disableLazyLoad = 1;
#endif
    if(options.lazy_loading_overridden)
        param->disableLazyLoad = !options.lazy_loading_enabled;

#ifdef CONFIG_YAFFS_DISABLE_TAGS_ECC
    param->noTagsECC = 1;
#endif

#ifdef CONFIG_YAFFS_DISABLE_BACKGROUND
#else
    param->deferDirectoryUpdate = 1;
#endif

    if(options.tags_ecc_overridden)
        param->noTagsECC = !options.tags_ecc_on;

#ifdef CONFIG_YAFFS_EMPTY_LOST_AND_FOUND
    param->emptyLostAndFound = 1;
#endif

#ifdef CONFIG_YAFFS_DISABLE_BLOCK_REFRESHING
    param->refreshPeriod = 0;
#else
    param->refreshPeriod = 10000;
#endif

    if(options.empty_lost_and_found_overridden)
        param->emptyLostAndFound = options.empty_lost_and_found;

    /* ... and the functions. */
    if (yaffsVersion == 2) {
        param->writeChunkWithTagsToNAND =
            nandmtd2_WriteChunkWithTagsToNAND;
        param->readChunkWithTagsFromNAND =
            nandmtd2_ReadChunkWithTagsFromNAND;
        param->markNANDBlockBad = nandmtd2_MarkNANDBlockBad;
        param->queryNANDBlock = nandmtd2_QueryNANDBlock;
        yaffs_DeviceToContext(dev)->spareBuffer = YMALLOC(mtd->oobsize);
        param->isYaffs2 = 1;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
        param->totalBytesPerChunk = mtd->writesize;
        param->nChunksPerBlock = mtd->erasesize / mtd->writesize;
#else
        param->totalBytesPerChunk = mtd->oobblock;
        param->nChunksPerBlock = mtd->erasesize / mtd->oobblock;
#endif
        nBlocks = YCALCBLOCKS(mtd->size, mtd->erasesize);

        param->startBlock = 0;
        param->endBlock = nBlocks - 1;
    } else {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
        /* use the MTD interface in yaffs_mtdif1.c */
        param->writeChunkWithTagsToNAND =
            nandmtd1_WriteChunkWithTagsToNAND;
        param->readChunkWithTagsFromNAND =
            nandmtd1_ReadChunkWithTagsFromNAND;
        param->markNANDBlockBad = nandmtd1_MarkNANDBlockBad;
        param->queryNANDBlock = nandmtd1_QueryNANDBlock;
#else
        param->writeChunkToNAND = nandmtd_WriteChunkToNAND;
        param->readChunkFromNAND = nandmtd_ReadChunkFromNAND;
#endif
        param->isYaffs2 = 0;
    }
    /* ... and common functions */
    param->eraseBlockInNAND = nandmtd_EraseBlockInNAND;
    param->initialiseNAND = nandmtd_InitialiseNAND;

#ifndef CONFIG_YAFFS_DOES_ECC
    param->useNANDECC = 1;
#endif

#ifdef CONFIG_YAFFS_DISABLE_WIDE_TNODES
    param->wideTnodesDisabled = 1;
#endif

    param->skipCheckpointRead = options.skip_checkpoint_read;
    param->skipCheckpointWrite = options.skip_checkpoint_write;

    /*
     * 使用设备信息
     */
    param->nReservedBlocks = st.st_spare2;
    param->startBlock = st.st_spare4[0];
    param->endBlock = st.st_spare4[1];

    extern void yaffsfs_RemoveObjectCallback(yaffs_Object *obj);
    dev->param.removeObjectCallback = yaffsfs_RemoveObjectCallback;

    yaffsfs_Lock();
    ylist_add_tail(&(yaffs_DeviceToContext(dev)->contextList), &yaffs_context_list);
    yaffsfs_Unlock();

    if (yaffs_mount(point_name) < 0) {
        yaffsfs_Lock();
        ylist_del_init(&(yaffs_DeviceToContext(dev)->contextList));
        yaffsfs_Lock();

        if (yaffs_DeviceToContext(dev)->spareBuffer) {
            YFREE(yaffs_DeviceToContext(dev)->spareBuffer);
            yaffs_DeviceToContext(dev)->spareBuffer = NULL;
        }

        kfree(dev);
        return NULL;
    }

    return dev;
}
/*********************************************************************************************************
** Function name:           yaffsfs_Unmount
** Descriptions:            取消挂载 yaffs 文件系统
** input parameters:        ctx                 上下文
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int yaffsfs_Unmount(void *ctx)
{
    yaffs_Device *dev = ctx;
    int ret;

    if (dev == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = yaffs_unmount(dev->param.name);
    if (ret < 0) {
        return ret;
    }

    yaffsfs_Lock();
    ylist_del_init(&(yaffs_DeviceToContext(dev)->contextList));
    yaffsfs_Lock();

    if (yaffs_DeviceToContext(dev)->spareBuffer) {
        YFREE(yaffs_DeviceToContext(dev)->spareBuffer);
        yaffs_DeviceToContext(dev)->spareBuffer = NULL;
    }

    kfree(dev);
    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
