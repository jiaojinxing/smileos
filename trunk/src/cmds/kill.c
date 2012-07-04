/*
 * kill.c
 *
 *  Created on: 2012-6-15
 *      Author: jiaojinxing
 */

#include "kern/kern.h"
#include <stdlib.h>
#include <signal.h>

int kill_main(int argc, char **argv)
{
    if (argc == 2) {
        return kill(atoi(argv[1]), SIGKILL);
    } else {
        return -1;
    }
}
