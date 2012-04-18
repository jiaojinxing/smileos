/*
 * rmdir.c
 *
 *  Created on: 2012-4-18
 *      Author: Administrator
 */

#include <reent.h>
#include <unistd.h>

int
_DEFUN(rmdir, (path),
       const char *path)
{
    int _rmdir_r(struct _reent *reent, const char *path);

    return _rmdir_r (_REENT, path);
}
