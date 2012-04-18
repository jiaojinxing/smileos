/*
 * mkdir.c
 *
 *  Created on: 2012-4-18
 *      Author: jiaojinxing
 */

#include <reent.h>
#include <unistd.h>

int
_DEFUN(mkdir, (path, mode),
       const char *path _AND
       mode_t mode)
{
    return _mkdir_r (_REENT, path, mode);
}
