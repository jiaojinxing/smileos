/*
 * GUI_Touch_X.c
 *
 *  Created on: 2013-2-2
 *      Author: jiaojinxing
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <drv/input.h>

#include "GUI.h"
/*********************************************************************************************************
** 触摸屏采样读取相关函数
*********************************************************************************************************/
struct ts_sample {
    int          x;
    int          y;
    unsigned int pressure;
};

static int ts_read_raw(int ts_fd, struct ts_sample *samp, int nr)
{
    struct input_event  notify;
    fd_set              rfds;

    FD_ZERO(&rfds);

    FD_SET(ts_fd, &rfds);

    if (select(ts_fd + 1, &rfds, NULL, NULL, NULL) != 1) {
        return -1;
    }

    if (read(ts_fd, (char *)&notify, sizeof(struct input_event)) == sizeof(struct input_event)) {
        samp->x        = notify.x;
        samp->y        = notify.y;
        samp->pressure = notify.press;
        return 1;
    } else {
        return -1;
    }
}

struct tslib_linear {
    int         swap_xy;
    int         p_offset;
    int         p_mult;
    int         p_div;
    int         a[7];
};

static struct tslib_linear tslib_linear;

static int tslib_linear_init(void)
{
    int             cal_fd;
    char            buf[128];

    // Use default values that leave ts numbers unchanged after transform
    tslib_linear.a[0]     = 1;
    tslib_linear.a[1]     = 0;
    tslib_linear.a[2]     = 0;
    tslib_linear.a[3]     = 0;
    tslib_linear.a[4]     = 1;
    tslib_linear.a[5]     = 0;
    tslib_linear.a[6]     = 1;
    tslib_linear.p_offset = 0;
    tslib_linear.p_mult   = 1;
    tslib_linear.p_div    = 1;
    tslib_linear.swap_xy  = 0;

    /*
     * Check calibration file
     */
    cal_fd = open("/rootfs/calibration", O_RDONLY);
    if (cal_fd < 0) {
        return -1;
    }

    if (read(cal_fd, buf, 128) <= 0) {
        printk("failed to read /rootfs/calibration\n");
        close(cal_fd);
        return -1;
    }

    sscanf(buf, "%d %d %d %d %d %d %d",
            &tslib_linear.a[0],
            &tslib_linear.a[1],
            &tslib_linear.a[2],
            &tslib_linear.a[3],
            &tslib_linear.a[4],
            &tslib_linear.a[5],
            &tslib_linear.a[6]);

    close(cal_fd);

    return 0;
}

static void tslib_linear_work(struct ts_sample *samp)
{
    int xtemp, ytemp;

    xtemp = samp->x;

    ytemp = samp->y;

    xtemp = (tslib_linear.a[2] +
             tslib_linear.a[0] * xtemp +
             tslib_linear.a[1] * ytemp) / tslib_linear.a[6];

    ytemp = (tslib_linear.a[5] +
             tslib_linear.a[3] * xtemp +
             tslib_linear.a[4] * ytemp) / tslib_linear.a[6];

    if (tslib_linear.swap_xy) {
        int tmp = xtemp;
        xtemp   = ytemp;
        ytemp   = tmp;
    }

    samp->x = xtemp;
    samp->y = ytemp;
}

void TouchTask(void *arg)
{
    int                 ts_fd;
    struct ts_sample    samp;
    GUI_PID_STATE       state = {0};

    if (tslib_linear_init() < 0) {
        printk("failed to init tslib\n");
        goto error0;
    }

    ts_fd = open("/dev/event0", O_RDONLY, 0666);
    if (ts_fd < 0) {
        printk("failed to open /dev/event0\n");
        goto error1;
    }

    while (1) {
        if (ts_read_raw(ts_fd, &samp, 1) == 1) {

            tslib_linear_work(&samp);

            state.x = samp.x;
            state.y = samp.y;
            state.Pressed = samp.pressure;

            printk("you touch %d %d %d\r\n", samp.x, samp.y, samp.pressure);

            GUI_TOUCH_StoreStateEx(&state);
        }
    }

    close(ts_fd);

    error1:
    error0:
    return;
}
