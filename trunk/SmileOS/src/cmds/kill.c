/*
 * kill.c
 *
 *  Created on: 2012-6-15
 *      Author: jiaojinxing
 */

#include "cmds/cmds.h"

#include "kern/kern.h"
#include <stdlib.h>
#include <signal.h>

int kill_main(int argc, char **argv)
{
    if (argc == 3) {
        return kill(atoi(argv[1]), atoi(argv[2]));
    } else {
        return -1;
    }
}

SMILEOS_CMD(
    kill,    kill_main,
    "bmp     - manipulate BMP image data\n",
    "info <imageAddr>          - display image info\n"
    "bmp display <imageAddr> [x y] - display image at x,y\n"
);
