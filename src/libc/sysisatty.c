/* connector for isatty */

#include <reent.h>
#include <unistd.h>

int
_DEFUN (isatty, (fd),
     int fd)
{
  return _isatty_r (_REENT, fd);
}
