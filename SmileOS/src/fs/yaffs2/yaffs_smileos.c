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
#include "src/yaffs_guts.h"
#include "src/yaffs_mtdif.h"
#include "src/yaffs_linux.h"
#include "src/yaffs_trace.h"
#include "src/direct/yaffsfs.h"
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <errno.h>
/*********************************************************************************************************
** 全局变量
*********************************************************************************************************/
unsigned int    yaffs_trace_mask = YAFFS_TRACE_ALWAYS | YAFFS_TRACE_OS | YAFFS_TRACE_CHECKPOINT | YAFFS_TRACE_ERASE; /*  TARCE 屏蔽掩码              */

static mutex_t  yaffs_lock;                                             /*  锁                          */

static LIST_HEAD(yaffs_context_list);                                   /*  YAFFS 上下文链表            */
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
** Function name:           yaffsfs_GetLastError
** Descriptions:            yaffs 获得 errno
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          错误码
*********************************************************************************************************/
int yaffsfs_GetLastError(void)
{
    return errno;
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
u32 yaffsfs_CurrentTime(void)
{
    return 0;
}
/*********************************************************************************************************
** Function name:           yaffsfs_CheckMemRegion
** Descriptions:            yaffs 检查内存区域是否合法
** input parameters:        addr                内存指针
**                          size                大小
**                          writeable           是否可写
** output parameters:       NONE
** Returned value:          O OR -1
*********************************************************************************************************/
int yaffsfs_CheckMemRegion(const void *addr, size_t size, int writeable)
{
    if (addr == NULL) {
        return -1;
    }
    return 0;
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
    INIT_LIST_HEAD(&yaffs_context_list);

    if (mutex_create(&yaffs_lock) < 0) {
        return -1;
    }

    return 0;
}
/*********************************************************************************************************
** 下面两个函数从 yaffs 的代码里拷贝出来的, 我不想去修改 yaffs 的代码
*********************************************************************************************************/
/*
 *  Stuff to handle names.
 */
#ifdef CONFIG_YAFFS_CASE_INSENSITIVE

static int yaffs_toupper(YCHAR a)
{
    if (a >= 'a' && a <= 'z')
        return (a - 'a') + 'A';
    else
        return a;
}

static int yaffsfs_Match(YCHAR a, YCHAR b)
{
    return (yaffs_toupper(a) == yaffs_toupper(b));
}
#else
static int yaffsfs_Match(YCHAR a, YCHAR b)
{
    /* case sensitive */
    return (a == b);
}
#endif

static int yaffsfs_IsPathDivider(YCHAR ch)
{
    const YCHAR *str = YAFFS_PATH_DIVIDERS;

    while (*str) {
        if (*str == ch)
            return 1;
        str++;
    }

    return 0;
}
/*********************************************************************************************************
** Function name:           yaffsfs_FindDevice
** Descriptions:            yaffs 查找设备
** input parameters:        path                路径
** output parameters:       restOfPath          余下的路径
** Returned value:          YAFFS 设备
*********************************************************************************************************/
struct yaffs_dev *yaffsfs_FindDevice(const YCHAR *path, YCHAR **restOfPath)
{
    struct list_head *item;
    const YCHAR *leftOver;
    const YCHAR *p;
    struct yaffs_dev *retval = NULL;
    int thisMatchLength;
    int longestMatch = -1;
    int matching;

    // Check all context, choose the one that:
    // 1) Actually matches a prefix (ie /a amd /abc will not match
    // 2) Matches the longest.
    list_for_each(item, &yaffs_context_list) {
        struct yaffs_linux_context *dc = list_entry(item, struct yaffs_linux_context, context_list);
        struct yaffs_dev *dev = dc->dev;

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
struct yaffs_options {
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
    int disable_summary;
};
/*********************************************************************************************************
** Function name:           yaffs_parse_options
** Descriptions:            yaffs 分析挂载选项字符串
** input parameters:        options_str         挂载选项字符串
** output parameters:       options             YAFFS 挂载选项
** Returned value:          0 OR 1
*********************************************************************************************************/
#define MAX_OPT_LEN 30
static int yaffs_parse_options(struct yaffs_options *options,
                   const char *options_str)
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

        if (!strcmp(cur_opt, "inband-tags")) {
            options->inband_tags = 1;
        } else if (!strcmp(cur_opt, "tags-ecc-off")) {
            options->tags_ecc_on = 0;
            options->tags_ecc_overridden = 1;
        } else if (!strcmp(cur_opt, "tags-ecc-on")) {
            options->tags_ecc_on = 1;
            options->tags_ecc_overridden = 1;
        } else if (!strcmp(cur_opt, "lazy-loading-off")) {
            options->lazy_loading_enabled = 0;
            options->lazy_loading_overridden = 1;
        } else if (!strcmp(cur_opt, "lazy-loading-on")) {
            options->lazy_loading_enabled = 1;
            options->lazy_loading_overridden = 1;
        } else if (!strcmp(cur_opt, "disable-summary")) {
            options->disable_summary = 1;
        } else if (!strcmp(cur_opt, "empty-lost-and-found-off")) {
            options->empty_lost_and_found = 0;
            options->empty_lost_and_found_overridden = 1;
        } else if (!strcmp(cur_opt, "empty-lost-and-found-on")) {
            options->empty_lost_and_found = 1;
            options->empty_lost_and_found_overridden = 1;
        } else if (!strcmp(cur_opt, "no-cache")) {
            options->no_cache = 1;
        } else if (!strcmp(cur_opt, "no-checkpoint-read")) {
            options->skip_checkpoint_read = 1;
        } else if (!strcmp(cur_opt, "no-checkpoint-write")) {
            options->skip_checkpoint_write = 1;
        } else if (!strcmp(cur_opt, "no-checkpoint")) {
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
** input parameters:        yaffs_version       yaffs 版本
**                          point_name          挂载点名
**                          dev_name            设备名
**                          data                挂载选项字符串
** output parameters:       NONE
** Returned value:          上下文
*********************************************************************************************************/
void *yaffsfs_Mount(int yaffs_version,
                    const char *point_name,
                    const char *dev_name,
                    void *data)
{
    int n_blocks;
    struct yaffs_dev *dev = 0;
    struct mtd_info *mtd;
    char *data_str = (char *)data;
    struct yaffs_linux_context *context = NULL;
    struct yaffs_param *param;
    int read_only = 0;
    struct yaffs_options options;
    struct stat st;

    /*
     * 参数检查
     */
    if (!dev_name) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "device name is NULL\n");
        return NULL;
    }

    /*
     * 获得 MTD 设备状态
     */
    if (vfs_stat(dev_name, &st) < 0) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "MTD device \"%s\" doesn't appear to exist\n", dev_name);
        return NULL;
    }

    /*
     * 分析挂载选项字符串
     */
    if (!data_str) {
        data_str = "";
    }

    yaffs_trace(YAFFS_TRACE_OS, "passed flags \"%s\"\n", data_str);

    memset(&options, 0, sizeof(options));

    if (yaffs_parse_options(&options, data_str)) {
        return NULL;
    }

    yaffs_trace(YAFFS_TRACE_OS, "Using yaffs%d\n", yaffs_version);

    yaffs_trace(YAFFS_TRACE_OS, "block size %d\n", (int)(st.st_blksize));

    yaffs_trace(YAFFS_TRACE_OS, "Attempting MTD mount on \"%s\"\n", dev_name);

    /*
     * 获得 MTD 设备
     */
    mtd = (struct mtd_info *)st.st_spare1;
    if (mtd == NULL) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "MTD device \"%s\" doesn't appear to exist\n", dev_name);
        return NULL;
    }

    /*
     * 检查它的类型, 保证是 NANDFLASH
     */
    if (mtd->type != MTD_NANDFLASH) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "MTD device is not NAND it's type %d", mtd->type);
        return NULL;
    }

    yaffs_trace(YAFFS_TRACE_OS, "erase %p", mtd->erase);
    yaffs_trace(YAFFS_TRACE_OS, "read %p", mtd->read);
    yaffs_trace(YAFFS_TRACE_OS, "write %p", mtd->write);
    yaffs_trace(YAFFS_TRACE_OS, "readoob %p", mtd->read_oob);
    yaffs_trace(YAFFS_TRACE_OS, "writeoob %p", mtd->write_oob);
    yaffs_trace(YAFFS_TRACE_OS, "block_isbad %p", mtd->block_isbad);
    yaffs_trace(YAFFS_TRACE_OS, "block_markbad %p", mtd->block_markbad);
#define WRITE_SIZE_STR "writesize"
#define WRITE_SIZE(mtd) ((mtd)->writesize)
    yaffs_trace(YAFFS_TRACE_OS, "%s %d", WRITE_SIZE_STR, WRITE_SIZE(mtd));
    yaffs_trace(YAFFS_TRACE_OS, "oobsize %d", mtd->oobsize);
    yaffs_trace(YAFFS_TRACE_OS, "erasesize %d", mtd->erasesize);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 29)
    yaffs_trace(YAFFS_TRACE_OS, "size %u", mtd->size);
#else
    yaffs_trace(YAFFS_TRACE_OS, "size %lld", mtd->size);
#endif

    /*
     * 自动选择 yaffs 版本
     */
    if (yaffs_version == 1 && WRITE_SIZE(mtd) >= 2048) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "auto selecting yaffs2");
        yaffs_version = 2;
    }

    /*
     * Added NCB 26/5/2006 for completeness
     */
    if (yaffs_version == 2 && !options.inband_tags
        && WRITE_SIZE(mtd) == 512) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "auto selecting yaffs1");
        yaffs_version = 1;
    }

    if (yaffs_version == 2) {
        /*
         * Check for version 2 style functions
         */
        if (!mtd->erase ||
            !mtd->block_isbad ||
            !mtd->block_markbad || !mtd->read || !mtd->write ||
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
            !mtd->read_oob || !mtd->write_oob) {
#else
            !mtd->write_ecc ||
            !mtd->read_ecc || !mtd->read_oob || !mtd->write_oob) {
#endif
            yaffs_trace(YAFFS_TRACE_ALWAYS, "MTD device does not support required functions");
            return NULL;
        }

        if ((WRITE_SIZE(mtd) < YAFFS_MIN_YAFFS2_CHUNK_SIZE ||
             mtd->oobsize < YAFFS_MIN_YAFFS2_SPARE_SIZE) &&
            !options.inband_tags) {
            yaffs_trace(YAFFS_TRACE_ALWAYS, "MTD device does not have the right page sizes");
            return NULL;
        }
    } else {
        /*
         * Check for V1 style functions
         */
        if (!mtd->erase || !mtd->read || !mtd->write ||
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
            !mtd->read_oob || !mtd->write_oob) {
#else
            !mtd->write_ecc ||
            !mtd->read_ecc || !mtd->read_oob || !mtd->write_oob) {
#endif
            yaffs_trace(YAFFS_TRACE_ALWAYS, "MTD device does not support required functions");
            return NULL;
        }

        if (WRITE_SIZE(mtd) < YAFFS_BYTES_PER_CHUNK ||
            mtd->oobsize != YAFFS_BYTES_PER_SPARE) {
            yaffs_trace(YAFFS_TRACE_ALWAYS, "MTD device does not support have the right page sizes");
            return NULL;
        }
    }

    /*
     * 分配设备和上下文
     */
    dev = kzalloc(sizeof(struct yaffs_dev) + sizeof(struct yaffs_linux_context), GFP_KERNEL);
    if (!dev) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "Failed trying to allocate struct yaffs_dev.");
        return NULL;
    }
    context = (struct yaffs_linux_context *)((char *)dev + sizeof(struct yaffs_dev));

    dev->os_context = context;
    context->dev = dev;

    INIT_LIST_HEAD(&(context->context_list));

    dev->read_only = read_only;

    dev->driver_context = mtd;

    param = &(dev->param);
    param->name = point_name;

    /*
     * 设置 MTD 块的大小参数
     */
    param->n_reserved_blocks = 5;
    param->n_caches = (options.no_cache) ? 0 : 10;
    param->inband_tags = options.inband_tags;

    param->enable_xattr = 1;
    if (options.lazy_loading_overridden)
        param->disable_lazy_load = !options.lazy_loading_enabled;

    param->defered_dir_update = 1;

    if (options.tags_ecc_overridden)
        param->no_tags_ecc = !options.tags_ecc_on;

    param->empty_lost_n_found = 1;
    param->refresh_period = 500;
    param->disable_summary = options.disable_summary;

    if (options.empty_lost_and_found_overridden)
        param->empty_lost_n_found = options.empty_lost_and_found;

#define YCALCBLOCKS(s, b) ((s) / (b))

    if (yaffs_version == 2) {
        param->is_yaffs2 = 1;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 17))
        param->total_bytes_per_chunk = mtd->writesize;
        param->chunks_per_block = mtd->erasesize / mtd->writesize;
#else
        param->total_bytes_per_chunk = mtd->oobblock;
        param->chunks_per_block = mtd->erasesize / mtd->oobblock;
#endif
        n_blocks = YCALCBLOCKS(mtd->size, mtd->erasesize);

        param->start_block = 0;
        param->end_block = n_blocks - 1;
    } else {
        param->is_yaffs2 = 0;
        n_blocks = YCALCBLOCKS(mtd->size, YAFFS_CHUNKS_PER_BLOCK * YAFFS_BYTES_PER_CHUNK);

        param->chunks_per_block = YAFFS_CHUNKS_PER_BLOCK;
        param->total_bytes_per_chunk = YAFFS_BYTES_PER_CHUNK;
    }

    param->start_block = 0;
    param->end_block = n_blocks - 1;

    /*
     * 设置 MTD 块的函数
     */
    yaffs_mtd_drv_install(dev);

    param->use_nand_ecc = 1;

    param->skip_checkpt_rd = options.skip_checkpoint_read;
    param->skip_checkpt_wr = options.skip_checkpoint_write;

    /*
     * 使用设备信息
     */
    param->n_reserved_blocks = st.st_spare2;
    param->start_block = st.st_spare4[0];
    param->end_block = st.st_spare4[1];

    yaffsfs_Lock();

    list_add_tail(&(yaffs_dev_to_lc(dev)->context_list), &yaffs_context_list);

    yaffsfs_Unlock();

    if (yaffs_mount(point_name) < 0) {

        yaffsfs_Lock();

        list_del_init(&(yaffs_dev_to_lc(dev)->context_list));

        yaffsfs_Lock();

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
    struct yaffs_dev *dev = ctx;
    struct mtd_info *mtd;
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

    list_del_init(&(yaffs_dev_to_lc(dev)->context_list));

    yaffsfs_Lock();

    if (yaffs_dev_to_lc(dev)->spare_buffer) {
        kfree(yaffs_dev_to_lc(dev)->spare_buffer);
        yaffs_dev_to_lc(dev)->spare_buffer = NULL;
    }

    mtd = yaffs_dev_to_mtd(dev);

    kfree(dev);

    if (mtd && mtd->sync) {
        mtd->sync(mtd);
    }

    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
