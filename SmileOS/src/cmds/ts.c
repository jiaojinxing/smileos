/*
 * ts.c
 *
 *  Created on: 2013-1-4
 *      Author: jiaojinxing
 */

#include "cmds/cmds.h"

#include <stdio.h>
#include "kern/kern.h"
#include "kern/kvars.h"

int task_stat(int tid, char *buf)
{
    const char *state;
    task_t *task;
    uint32_t reg;

    if (tid < 0 || tid >= TASK_NR) {
        return -1;
    }

    reg = interrupt_disable();

    task = tasks + tid;

    switch (task->status) {
    case TASK_UNALLOCATE:
        interrupt_resume(reg);
        return -1;

    case TASK_RUNNING:
        state = "ready";
        break;

    case TASK_SLEEPING:
        state = "sleep";
        break;

    case TASK_SUSPEND:
        state = "wait";
        break;

    default:
        state = "unknow";
        break;
    }

    if (strlen(task->name) < 7) {
        sprintf(buf, "%s\t %s\t\t %4u\t %s\t %4u\t %10u\t %4u\t %4u%%\t %4u%%\t %4u\t %4u\n",
                task->type == TASK_TYPE_PROCESS ? "process" : "kthread",
                task->name,
                task->tid,
                state,
                task->timeslice,
                task->delay,
                task->priority,
                task->cpu_usage,
                100,
                task->frame_nr,
                task->dabt_cnt);
    } else {
        sprintf(buf, "%s\t %s\t %4u\t %s\t %4u\t %10u\t %4u\t %4u%%\t %4u%%\t %4u\t %4u\n",
                task->type == TASK_TYPE_PROCESS ? "process" : "kthread",
                task->name,
                task->tid,
                state,
                task->timeslice,
                task->delay,
                task->priority,
                task->cpu_usage,
                100,
                task->frame_nr,
                task->dabt_cnt);
    }

    interrupt_resume(reg);

    return 0;
}

int
ts_main(int argc, char **argv)
{
    int i;
    char buf[LINE_MAX];

    printf("type\t name\t\t pid\t state\t count\t timer\t\t prio\t cpu\t stack\t page\t dabt\n");

    for (i = 0; i < TASK_NR; i++) {
        if (task_stat(i, buf) == 0) {
            printf(buf);
        }
    }

    return 0;
}

SMILEOS_CMD(
    ts,    ts_main,
    "bmp     - manipulate BMP image data\n",
    "info <imageAddr>          - display image info\n"
    "bmp display <imageAddr> [x y] - display image at x,y\n"
);
