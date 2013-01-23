/*
 * cd.c
 *
 *  Created on: 2012-6-8
 *      Author: jiaojinxing
 */

#include "cmds/cmds.h"

#include <unistd.h>

/*
 * cd √¸¡Ó
 */
int
cd_main(int argc, char **argv)
{
    if (argc == 2) {
        return chdir(argv[1]);
    } else {
        return -1;
    }
}

SMILEOS_CMD(
    cd,    cd_main,
    "bmp     - manipulate BMP image data\n",
    "info <imageAddr>          - display image info\n"
    "bmp display <imageAddr> [x y] - display image at x,y\n"
);
