/*
 * gui_time.c
 *
 *  Created on: 2012-7-17
 *      Author: jiaojinxing
 */

#include <sys/time.h>

int get_sys_time()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
