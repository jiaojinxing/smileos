/*
 * cd.c
 *
 *  Created on: 2012-6-8
 *      Author: jiaojinxing
 */

#include <unistd.h>

/*
 * cd ÃüÁî
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
