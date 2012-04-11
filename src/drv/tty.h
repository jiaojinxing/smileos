/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <JiaoJinXing1987@gmail.com>
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
** File name:               tty.h
** Last modified Date:      2012-4-10
** Last Version:            1.0.0
** Descriptions:            tty 设备驱动
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-4-10
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
#ifndef TTY_H_
#define TTY_H_

#include <sys/cdefs.h>
#include <sys/signal.h>
#include <sys/termios.h>
#include <limits.h>
#include "kern/kern.h"
#include "kern/ipc.h"

#define TTYQ_SIZE       INPUT_MAX
#define TTYQ_HIWAT      (TTYQ_SIZE - 10)

/*
 * tty 队列
 */
struct tty_queue {
    char                tq_buf[TTYQ_SIZE];
    int                 tq_head;
    int                 tq_tail;
    int                 tq_count;
    mutex_t             tq_lock;
};

/*
 * tty
 */
struct tty {
    struct tty_queue    t_rawq;                             /*  raw input queue                         */
    struct tty_queue    t_canq;                             /*  canonical queue                         */
    struct tty_queue    t_outq;                             /*  ouput queue                             */
    struct termios      t_termios;                          /*  termios state                           */
    struct winsize      t_winsize;                          /*  window size                             */
    int                 t_state;                            /*  driver state                            */
    int                 t_column;                           /*  tty output column                       */
    pid_t               t_pgid;                             /*  foreground process group                */
    void (*t_oproc)(struct tty *);                          /*  routine to start output                 */
    task_t             *t_input;                            /*  等待输入数据到达                        */
    task_t             *t_output;                           /*  等待输出数据完成                        */
};

#define t_iflag         t_termios.c_iflag
#define t_oflag         t_termios.c_oflag
#define t_cflag         t_termios.c_cflag
#define t_lflag         t_termios.c_lflag
#define t_cc            t_termios.c_cc
#define t_ispeed        t_termios.c_ispeed
#define t_ospeed        t_termios.c_ospeed


/*
 * These flags are kept in t_state.
 */
#define TS_ASLEEP       0x00001                             /*  Process waiting for tty.                */
#define TS_BUSY         0x00004                             /*  Draining output.                        */
#define TS_TIMEOUT      0x00100                             /*  Wait for output char processing.        */
#define TS_TTSTOP       0x00200                             /*  Output paused.                          */
#define TS_ISIG         0x00400                             /*  Input is interrupted by signal.         */

/*
 * Attach a tty to the tty list.
 */
int tty_attach(struct tty *tp);

/*
 * Ioctls for all tty devices.
 */
int tty_ioctl(struct tty *tp, u_long cmd, void *data);

/*
 * Process a write call on a tty device.
 */
int tty_write(struct tty *tp, const char *buf, size_t *nbyte);

/*
 * Process a read call on a tty device.
 */
int tty_read(struct tty *tp, char *buf, size_t *nbyte);

/*
 * Process input of a single character received on a tty.
 * echo if required.
 * This may be called with interrupt level.
 */
void tty_input(int c, struct tty *tp);

/*
 * Output is completed.
 */
void tty_done(struct tty *tp);

/*
 * Get a character from a queue.
 */
int tty_getc(struct tty_queue *tq);

#endif                                                                  /*  TTY_H_                      */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
