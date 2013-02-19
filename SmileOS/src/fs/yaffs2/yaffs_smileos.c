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
** Descriptions:            yaffs �ļ�ϵͳϵͳģ���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-27
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
** ȫ�ֱ���
*********************************************************************************************************/
unsigned int    yaffs_trace_mask = YAFFS_TRACE_ALWAYS;                  /*  TARCE ��������              */

static mutex_t  yaffs_lock;                                             /*  ��                          */

static LIST_HEAD(yaffs_context_list);                                   /*  YAFFS ����������            */
/*********************************************************************************************************
** Function name:           yaffsfs_SetError
** Descriptions:            yaffs ���� errno
** input parameters:        err                 ������
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void yaffsfs_SetError(int err)
{
    seterrno(-err);
}
/*********************************************************************************************************
** Function name:           yaffsfs_GetLastError
** Descriptions:            yaffs ��� errno
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ������
*********************************************************************************************************/
int yaffsfs_GetLastError(void)
{
    return errno;
}
/*********************************************************************************************************
** Function name:           yaffsfs_Lock
** Descriptions:            yaffs ����
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
** Descriptions:            yaffs ����
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
** Descriptions:            yaffs ��õ�ǰʱ��
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          ��ǰʱ��
*********************************************************************************************************/
u32 yaffsfs_CurrentTime(void)
{
    return 0;
}
/*********************************************************************************************************
** Function name:           yaffsfs_CheckMemRegion
** Descriptions:            yaffs ����ڴ������Ƿ�Ϸ�
** input parameters:        addr                �ڴ�ָ��
**                          size                ��С
**                          writeable           �Ƿ��д
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
** Descriptions:            ��ʼ�� yaffs
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
** �������������� yaffs �Ĵ�������������, �Ҳ���ȥ�޸� yaffs �Ĵ���
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
** Descriptions:            yaffs �����豸
** input parameters:        path                ·��
** output parameters:       restOfPath          ���µ�·��
** Returned value:          YAFFS �豸
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
** YAFFS ѡ��
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
** Descriptions:            yaffs ��������ѡ���ַ���
** input parameters:        options_str         ����ѡ���ַ���
** output parameters:       options             YAFFS ����ѡ��
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
** Descriptions:            ���� yaffs �ļ�ϵͳ
** input parameters:        yaffs_version       yaffs �汾
**                          point_name          ���ص���
**                          dev_name            �豸��
**                          data                ����ѡ���ַ���
** output parameters:       NONE
** Returned value:          ������
*********************************************************************************************************/
void *yaffsfs_Mount(int yaffs_version,
                    const char *point_name,
                    const char *dev_name,
                    void *data)
{
    int n_blocks;
    struct yaffs_dev *dev;
    struct mtd_info *mtd;
    char *data_str = (char *)data;
    struct yaffs_linux_context *context;
    struct yaffs_param *param;
    int read_only;
    struct yaffs_options options;
    struct stat st;

    /*
     * �������
     */
    if (!dev_name) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "device name is NULL\n");
        return NULL;
    }

    /*
     * ��� MTD �豸״̬
     */
    if (vfs_stat(dev_name, &st) < 0) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "MTD device \"%s\" doesn't appear to exist\n", dev_name);
        return NULL;
    }

    read_only = st.st_spare3;

    /*
     * ��������ѡ���ַ���
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

    yaffs_trace(YAFFS_TRACE_ALWAYS, "Attempting MTD mount on \"%s\"\n", dev_name);

    /*
     * ��� MTD �豸
     */
    mtd = (struct mtd_info *)st.st_spare1;
    if (mtd == NULL) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "MTD device \"%s\" doesn't appear to exist\n", dev_name);
        return NULL;
    }

    if (yaffs_version == 1 && WRITE_SIZE(mtd) >= 2048) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "auto selecting yaffs2");
        yaffs_version = 2;
    }

    /* Added NCB 26/5/2006 for completeness */
    if (yaffs_version == 2 && !options.inband_tags
        && WRITE_SIZE(mtd) == 512) {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "auto selecting yaffs1");
        yaffs_version = 1;
    }

    if (yaffs_verify_mtd(mtd, yaffs_version, options.inband_tags) < 0) {
        return NULL;
    }

    /*
     * OK, so if we got here, we have an MTD that's NAND and looks
     * like it has the right capabilities
     * Set the struct yaffs_dev up for mtd
     */
    if (!read_only && !(mtd->flags & MTD_WRITEABLE)) {
        read_only = 1;
        printk(KERN_INFO"yaffs: mtd is read only, setting superblock read only\n");
    }

    /*
     * �����豸��������
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
     * ���� MTD ��Ĵ�С����
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

#define YCALCBLOCKS(s, b)   ((s) / (b))

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
        n_blocks = YCALCBLOCKS(mtd->size,
                 YAFFS_CHUNKS_PER_BLOCK * YAFFS_BYTES_PER_CHUNK);

        param->chunks_per_block = YAFFS_CHUNKS_PER_BLOCK;
        param->total_bytes_per_chunk = YAFFS_BYTES_PER_CHUNK;
    }

    param->start_block = 0;
    param->end_block = n_blocks - 1;

    /*
     * ���� MTD ��ĺ���
     */
    yaffs_mtd_drv_install(dev);

    param->use_nand_ecc = 1;

    param->skip_checkpt_rd = options.skip_checkpoint_read;
    param->skip_checkpt_wr = options.skip_checkpoint_write;

    /*
     * ʹ���豸��Ϣ
     */
    param->n_reserved_blocks = st.st_spare2;
    param->start_block = st.st_spare4[0];
    param->end_block = st.st_spare4[1];

    yaffsfs_Lock();

    list_add_tail(&(yaffs_dev_to_lc(dev)->context_list), &yaffs_context_list);

    yaffsfs_Unlock();

    yaffs_add_device(dev);

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
** Descriptions:            ȡ������ yaffs �ļ�ϵͳ
** input parameters:        ctx                 ������
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
