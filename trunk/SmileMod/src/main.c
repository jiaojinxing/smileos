/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <JiaoJinXing1987@gmail.com>
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
** File name:               main.c
** Last modified Date:      2012-3-27
** Last Version:            1.0.0
** Descriptions:            TEST 驱动和设备模块
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
#include "vfs/device.h"
#include "vfs/driver.h"
#include "kern/kern.h"
#include <errno.h>

/*
 * 私有信息
 */
typedef struct {
    VFS_SELECT_MEMBERS;
} privinfo_t;

/*
 * 打开 test
 */
static int test_open(void *ctx, file_t *file, int oflag, mode_t mode)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * 关闭 test
 */
static int test_close(void *ctx, file_t *file)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    return 0;
}

/*
 * 读 test
 */
static ssize_t test_read(void *ctx, file_t *file, void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }
    return 0;
}

/*
 * 写 test
 */
static ssize_t test_write(void *ctx, file_t *file, const void *buf, size_t len)
{
    privinfo_t *priv = ctx;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }
    if (priv->flags & VFS_FILE_ERROR) {
        seterrno(EIO);
        return -1;
    }
    return len;
}

/*
 * 扫描
 */
static int test_scan(void *ctx, file_t *file, int flags)
{
    privinfo_t *priv = ctx;
    int ret;

    if (priv == NULL) {
        seterrno(EINVAL);
        return -1;
    }

    ret = 0;
    if (flags & VFS_FILE_WRITEABLE) {
        ret |= VFS_FILE_WRITEABLE;
    }
    return ret;
}

#include "drv/selectdrv.h"

/*
 * test 驱动
 */
static driver_t test_drv = {
        .name     = "test",
        .open     = test_open,
        .write    = test_write,
        .read     = test_read,
        .close    = test_close,
        .scan     = test_scan,
        .select   = select_select,
        .unselect = select_unselect,
};

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static void test_thread(void *arg)
{
    int fd;
    int i;

    fd = open("/dev/test", O_RDWR, 0666);
    if (fd < 0) {
        printf("failed to open /dev/test\n");
        return;
    }

    for (i = 0; i < 100000; i++) {
        printf("hello SmileOS, in test kernel module, i = %d\n", i);
        sleep(10);
    }

    close(fd);
}

/*
 * 初始化 test
 */
int module_init(int argc, char **argv)
{
    privinfo_t *priv;

    driver_install(&test_drv);

    priv = kmalloc(sizeof(privinfo_t));
    if (priv != NULL) {
        select_init(priv);
        if (device_create("/dev/test", "test", priv) < 0) {
            kfree(priv);
            return -1;
        }
        kthread_create("test", test_thread, NULL, 8 * KB, 10);
        return 0;
    } else {
        return -1;
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
