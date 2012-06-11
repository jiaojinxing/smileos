/*
 * ts.c
 *
 *  Created on: 2012-6-8
 *      Author: jiaojinxing
 */

#include <stdio.h>
#include "kern/kern.h"

/*
 * ts ÃüÁî
 */
int
ts_main(int argc, char **argv)
{
    int i;
    char buf[LINE_MAX];

    printf("type\t name\t\t pid\t state\t count\t timer\t\t prio\t cpu\t stack\t page\t dabt\n");

    for (i = 0; i < TASK_NR; i++) {
        if (tstat(i, buf) == 0) {
            printf(buf);
        }
    }

    return 0;
}
