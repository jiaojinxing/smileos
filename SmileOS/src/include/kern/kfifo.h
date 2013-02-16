/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <jiaojinxing1987@gmail.com>
**
** Licensed under the Academic Free License version 2.1
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**--------------------------------------------------------------------------------------------------------
** File name:               kfifo.h
** Last modified Date:      2013-1-27
** Last Version:            1.0.0
** Descriptions:            FIFO 模板
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2013-1-27
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef KFIFO_H_
#define KFIFO_H_

#include <string.h>

/*
 * FIFO
 */
typedef struct {
    unsigned char  *buf;
    unsigned int    size;
    unsigned int    mask;
    unsigned int    in;
    unsigned int    out;
} kfifo_t;

static inline int kfifo_init(kfifo_t *f, unsigned int size)
{
    f->size = 1;
    for(; f->size <= size; f->size <<= 1) {
        ;
    }

    f->in   = 0;
    f->out  = 0;
    f->mask = f->size - 1;
    f->buf  = kmalloc(f->size, GFP_KERNEL);
    return f->buf ? 0 : -1;
}

static inline void kfifo_free(kfifo_t *f)
{
    kfree(f->buf);
    f->buf = NULL;
}

static inline unsigned int kfifo_len(kfifo_t *f)
{
    return f->in - f->out;
}

static inline unsigned int kfifo_size(kfifo_t *f)
{
    return f->size;
}

static inline unsigned int kfifo_is_empty(kfifo_t *f)
{
    return f->in == f->out;
}

static inline unsigned int kfifo_is_full(kfifo_t *f)
{
    return kfifo_len(f) > f->mask;
}

static inline void kfifo_reset(kfifo_t *f)
{
    f->in   = 0;
    f->out  = 0;
}

static inline unsigned int kfifo_put(kfifo_t *f, const void *buf, unsigned int len)
{
    unsigned int l;

    if (kfifo_is_full(f)) {
        return 0;
    }

    len = min(len, f->size - f->in + f->out);

    /*
     * first put the data starting from f->in to buffer end
     */
    l = min(len, f->size - (f->in & f->mask));

    memcpy(f->buf + (f->in & f->mask), buf, l);

    /*
     * then put the rest (if any) at the beginning of the buffer
     */
    memcpy(f->buf, (const char *)buf + l, len - l);

    f->in += len;

    return len;
}

static inline unsigned int kfifo_get(kfifo_t *f, void *buf, unsigned int len)
{
    unsigned int l;

    if (kfifo_is_empty(f)) {
        return 0;
    }

    len = min(len, f->in - f->out);

    /*
     * first get the data from fifo->out until the end of the buffer
     */
    l = min(len, f->size - (f->out & f->mask));

    memcpy(buf, f->buf + (f->out & f->mask), l);

    /*
     * then get the rest (if any) from the beginning of the buffer
     */
    memcpy((char *)buf + l, f->buf, len - l);

    f->out += len;

    return len;
}

#endif                                                                  /*  KFIFO_H_                    */
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
