/*
 * fs.c
 *
 *  Created on: 2012-3-9
 *      Author: Administrator
 */

#if 0
#include "kern/config.h"
#include "kern/types.h"

/*
 * 块驱动
 */
typedef struct _blk_drv {
    char name[16];
    int (*init)(void *arg);
    int (*read_blk)(void *arg, uint32_t blk, uint8_t *buf);
    struct _blk_drv *next;
} blk_drv_t;

/*
 * 块设备
 */
typedef struct _blk_dev {
    char name[16];
    void *arg;
    blk_drv_t *drv;
    struct _blk_dev *next;
} blk_dev_t;

/*
 * 文件系统
 */
typedef struct _fs_t {
    char name[16];
    int (*mount)(void *arg);
    struct _fs_t *next;
} fs_t;

typedef struct _mount_point_t {
    char name[16];
    struct _mount_point_t *next;
} mount_point_t;

blk_drv_t *blk_drv_list;
blk_dev_t *blk_dev_list;
fs_t      *fs_list;

/*
 * 安装块驱动
 */
int blk_drv_install(blk_drv_t *drv)
{
    drv->next = blk_drv_list;
    blk_drv_list = drv;
    return 0;
}

/*
 * 创建块设备
 */
int blk_dev_create(const char *name, const char *drv_name, void *arg)
{
    blk_drv_t *drv = blk_drv_list;
    blk_dev_t *dev;
    int ret = -1;

    while (drv && strcmp(drv->name, drv_name)) {
        drv = drv->next;
    }

    if (drv) {
        dev = (blk_dev_t *)kmalloc(sizeof(blk_dev_t));
        if (dev) {
            strcpy(dev->name, name);
            dev->arg = arg;
            dev->drv = drv;

            drv->next = blk_dev_list;
            blk_dev_list =dev;

            ret = 0;
        }
    }

    return ret;
}

/*
 * 安装文件系统
 */
int fs_install(fs_t *fs)
{
    fs->next = fs_list;
    fs_list = fs;
    return 0;
}

int fs_mount(const char *dev_name, const char *path, const char *fs)
{
    blk_dev_t *dev;

    while (dev && strcmp(dev->name, dev_name)) {
        dev = dev->next;
    }

    if (dev) {

    }
}
#endif

