/*
**  GNU Pth - The GNU Portable Threads
**  Copyright (c) 1999-2006 Ralf S. Engelschall <rse@engelschall.com>
**
**  This file is part of GNU Pth, a non-preemptive thread scheduling
**  library which can be found at http://www.gnu.org/software/pth/.
**
**  This library is free software; you can redistribute it and/or
**  modify it under the terms of the GNU Lesser General Public
**  License as published by the Free Software Foundation; either
**  version 2.1 of the License, or (at your option) any later version.
**
**  This library is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**  Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License along with this library; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
**  USA, or contact Ralf S. Engelschall <rse@engelschall.com>.
**
**  pth_high.c: Pth high-level replacement functions
*/
                             /* ``The difference between a computer
                                  industry job and open-source software
                                  hacking is about 30 hours a week.''
                                         -- Ralf S. Engelschall     */

/*
 *  These functions used by the applications instead of the
 *  regular Unix/POSIX functions. When the regular functions would
 *  block, these variants let only the thread sleep.
 */

#include "pth_p.h"

/* Pth variant of nanosleep(2) */
int pth_nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
    pth_time_t until;
    pth_time_t offset;
    pth_time_t now;
    pth_event_t ev;
    static pth_key_t ev_key = PTH_KEY_INIT;

    /* consistency checks for POSIX conformance */
    if (rqtp == NULL)
        return pth_error(-1, EFAULT);
    if (rqtp->tv_nsec < 0 || rqtp->tv_nsec > (1000*1000000))
        return pth_error(-1, EINVAL);

    /* short-circuit */
    if (rqtp->tv_sec == 0 && rqtp->tv_nsec == 0)
        return 0;

    /* calculate asleep time */
    offset = pth_time((long)(rqtp->tv_sec), (long)(rqtp->tv_nsec) / 1000);
    pth_time_set(&until, PTH_TIME_NOW);
    pth_time_add(&until, &offset);

    /* and let thread sleep until this time is elapsed */
    if ((ev = pth_event(PTH_EVENT_TIME|PTH_MODE_STATIC, &ev_key, until)) == NULL)
        return pth_error(-1, errno);
    pth_wait(ev);

    /* optionally provide amount of slept time */
    if (rmtp != NULL) {
        pth_time_set(&now, PTH_TIME_NOW);
        pth_time_sub(&until, &now);
        rmtp->tv_sec  = until.tv_sec;
        rmtp->tv_nsec = until.tv_usec * 1000;
    }

    return 0;
}

/* Pth variant of usleep(3) */
int pth_usleep(unsigned int usec)
{
    pth_time_t until;
    pth_time_t offset;
    pth_event_t ev;
    static pth_key_t ev_key = PTH_KEY_INIT;

    /* short-circuit */
    if (usec == 0)
        return 0;

    /* calculate asleep time */
    offset = pth_time((long)(usec / 1000000), (long)(usec % 1000000));
    pth_time_set(&until, PTH_TIME_NOW);
    pth_time_add(&until, &offset);

    /* and let thread sleep until this time is elapsed */
    if ((ev = pth_event(PTH_EVENT_TIME|PTH_MODE_STATIC, &ev_key, until)) == NULL)
        return pth_error(-1, errno);
    pth_wait(ev);

    return 0;
}

/* Pth variant of sleep(3) */
unsigned int pth_sleep(unsigned int sec)
{
    pth_time_t until;
    pth_time_t offset;
    pth_event_t ev;
    static pth_key_t ev_key = PTH_KEY_INIT;

    /* consistency check */
    if (sec == 0)
        return 0;

    /* calculate asleep time */
    offset = pth_time(sec, 0);
    pth_time_set(&until, PTH_TIME_NOW);
    pth_time_add(&until, &offset);

    /* and let thread sleep until this time is elapsed */
    if ((ev = pth_event(PTH_EVENT_TIME|PTH_MODE_STATIC, &ev_key, until)) == NULL)
        return sec;
    pth_wait(ev);

    return 0;
}
