/*
 * ftpdLibFix.c
 *
 *  Created on: 2013-2-5
 *      Author: jiaojinxing
 */

#include "kern/kern.h"
#include "ftpdLibFix.h"
#include "list.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

int taskSpawn(const char *name, int priority, int options, int stack_size,
        FUNCPTR func, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6,
        int arg7, int arg8, int arg9, int arg10)
{
    return kthread_create(name, (void (*)(void *))func, (void *)arg1, stack_size, priority);
}

void logMsg(char *format, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6)
{
    printk(format, arg1, arg2, arg3, arg4, arg5, arg6);
}

void pathCat(const char *a, const char *b, char *c)
{
    sprintf(c, "%s/%s", a, b);
}

void pathCondense(char *a)
{
    extern int vfs_path_normalization(char path[PATH_MAX], int sprit_end);
    vfs_path_normalization(a, 0);
}

static int vfdprintf(int fd, const char *format, va_list ap)
{
    char *buf=0;
    int ret;

    ret = vasprintf(&buf, format, ap);
    if (ret < 0)
        goto end;

    ret = write(fd, buf, ret);

    free(buf);
end:
    return ret;
}

int fdprintf(int fd, const char *format, ...)
{
    va_list ap;
    int ret;

    va_start(ap, format);

    ret = vfdprintf(fd, format, ap);

    va_end(ap);

    return ret;
}

void lstInit(LIST *l)
{
    l->next = NULL;
    l->prev = NULL;
}

NODE *lstNext(NODE *n)
{
    if (!n) {
        return NULL;
    }

    return n->next;
}

NODE *lstFirst(LIST *l)
{
    return l->next;
}

void lstAdd(LIST *l, NODE *n)
{
    if (!n) {
        return;
    }

    while (l->next != NULL) {
        l = l->next;
    }

    l->next = n;
    n->next = NULL;
}

void lstDelete(LIST *l, NODE *n)
{
    if (!n) {
        return;
    }

    while (l->next && l->next != n) {
        l = l->next;
    }

    if (l->next == n) {
        l->next = n->next;
    }
}
