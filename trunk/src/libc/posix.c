/*
 * posix.c
 *
 *  Created on: 2012-5-3
 *      Author: jiaojinxing
 */

#include <fcntl.h>
#include <reent.h>
#include <unistd.h>

int
_DEFUN(creat, (path, mode),
       const char *path,
       mode_t mode)
{
  return open (path, O_WRONLY | O_CREAT | O_TRUNC, mode);
}

int
_DEFUN(mkdir, (path, mode),
       const char *path,
       mode_t mode)
{
    return _mkdir_r (_REENT, path, mode);
}

int
_DEFUN(rmdir, (path),
       const char *path)
{
    extern int _rmdir_r(struct _reent *reent, const char *path);

    return _rmdir_r (_REENT, path);
}

int
_DEFUN(isatty, (fd),
     int fd)
{
    return _isatty_r (_REENT, fd);
}
