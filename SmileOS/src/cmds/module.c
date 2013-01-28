/*
 * cd.c
 *
 *  Created on: 2012-6-8
 *      Author: jiaojinxing
 */

#include "cmds/cmds.h"
#include "module/module.h"
#include <unistd.h>

/*
 * module ÃüÁî
 */
int
module_main(int argc, char **argv)
{
    if (argc > 0) {
        return module_load(argv[0], argc - 1, &argv[1]);
    } else {
        return -1;
    }
}

SMILEOS_CMD(
    module,    module_main,
    "bmp     - manipulate BMP image data\n",
    "info <imageAddr>          - display image info\n"
    "bmp display <imageAddr> [x y] - display image at x,y\n"
);
