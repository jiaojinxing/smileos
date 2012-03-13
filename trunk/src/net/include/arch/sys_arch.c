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
** File name:               sys_arch.c
** Last modified Date:      2012-2-22
** Last Version:            1.0.0
** Descriptions:            LwIP 系统模拟层
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-22
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
#include "kern/config.h"
#include "kern/types.h"
#include "kern/ipc.h"
#include "kern/kern.h"
#include "kern/sys_call.h"
#include "lwip/sys.h"

/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex */
err_t sys_mutex_new(sys_mutex_t *mutex)
{
    if (kern_mutex_new(mutex) < 0) {
        return ERR_MEM;
    } else {
        return ERR_OK;
    }
}

/** Lock a mutex
 * @param mutex the mutex to lock */
void sys_mutex_lock(sys_mutex_t *mutex)
{
    kern_mutex_lock(mutex, 0);
}

/** Unlock a mutex
 * @param mutex the mutex to unlock */
void sys_mutex_unlock(sys_mutex_t *mutex)
{
    kern_mutex_unlock(mutex);
}

/** Delete a semaphore
 * @param mutex the mutex to delete */
void sys_mutex_free(sys_mutex_t *mutex)
{
    kern_mutex_free(mutex);
}

/** Check if a mutex is valid/allocated: return 1 for valid, 0 for invalid */
int sys_mutex_valid(sys_mutex_t *mutex)
{
    return kern_mutex_valid(mutex);
}

/** Set a mutex invalid so that sys_mutex_valid returns 0 */
void sys_mutex_set_invalid(sys_mutex_t *mutex)
{
    kern_mutex_set_valid(mutex, FALSE);
}

/** Create a new semaphore
 * @param sem pointer to the semaphore to create
 * @param count initial count of the semaphore
 * @return ERR_OK if successful, another err_t otherwise */
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    if (kern_sem_new(sem, count) < 0) {
        return ERR_MEM;
    } else {
        return ERR_OK;
    }
}

/** Signals a semaphore
 * @param sem the semaphore to signal */
void sys_sem_signal(sys_sem_t *sem)
{
    kern_sem_signal(sem);
}

/** Wait for a semaphore for the specified timeout
 * @param sem the semaphore to wait for
 * @param timeout timeout in milliseconds to wait (0 = wait forever)
 * @return time (in milliseconds) waited for the semaphore
 *         or SYS_ARCH_TIMEOUT on timeout */
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    if (timeout != 0) {
        timeout = timeout * 1000 * TICK_PER_SECOND;
    }

    if (kern_sem_wait(sem, timeout) < 0) {
        return SYS_ARCH_TIMEOUT;
    } else {
        /*
         * TODO: return time (in milliseconds) waited for the semaphore
         */
        return 0;
    }
}

/** Delete a semaphore
 * @param sem semaphore to delete */
void sys_sem_free(sys_sem_t *sem)
{
    kern_sem_free(sem);
}

/** Check if a sempahore is valid/allocated: return 1 for valid, 0 for invalid */
int sys_sem_valid(sys_sem_t *sem)
{
    return kern_sem_valid(sem);
}

/** Set a semaphore invalid so that sys_sem_valid returns 0 */
void sys_sem_set_invalid(sys_sem_t *sem)
{
    kern_sem_set_valid(sem, FALSE);
}

/** Time functions. */
void sys_msleep(u32_t ms) /* only has a (close to) 1 jiffy resolution. */
{
    usleep(1000 * ms);
}

/** Create a new mbox of specified size
 * @param mbox pointer to the mbox to create
 * @param size (miminum) number of messages in this mbox
 * @return ERR_OK if successful, another err_t otherwise */
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    if (kern_mbox_new(mbox, size) < 0) {
        return ERR_MEM;
    } else {
        return ERR_OK;
    }
}

/** Post a message to an mbox - may not fail
 * -> blocks if full, only used from tasks not from ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL) */
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    kern_mbox_post(mbox, msg, 0);
}

/** Try to post a message to an mbox - may fail if full or ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL) */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    if (kern_mbox_trypost(mbox, msg) < 0) {
        return ERR_BUF;
    } else {
        return ERR_OK;
    }
}

/** Wait for a new message to arrive in the mbox
 * @param mbox mbox to get a message from
 * @param msg pointer where the message is stored
 * @param timeout maximum time (in milliseconds) to wait for a message
 * @return time (in milliseconds) waited for a message, may be 0 if not waited
           or SYS_ARCH_TIMEOUT on timeout
 *         The returned time has to be accurate to prevent timer jitter! */
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
    if (timeout != 0) {
        timeout = timeout * 1000 * TICK_PER_SECOND;
    }

    if (kern_mbox_fetch(mbox, msg, timeout) < 0) {
        return SYS_ARCH_TIMEOUT;
    } else {
        /*
         * TODO: return time (in milliseconds) waited for a message
         */
        return 0;
    }
}

/* Allow port to override with a macro, e.g. special timout for sys_arch_mbox_fetch() */
/** Wait for a new message to arrive in the mbox
 * @param mbox mbox to get a message from
 * @param msg pointer where the message is stored
 * @param timeout maximum time (in milliseconds) to wait for a message
 * @return 0 (milliseconds) if a message has been received
 *         or SYS_MBOX_EMPTY if the mailbox is empty */
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    if (kern_mbox_tryfetch(mbox, msg) < 0) {
        return SYS_MBOX_EMPTY;
    } else {
        return 0;
    }
}

/** Delete an mbox
 * @param mbox mbox to delete */
void sys_mbox_free(sys_mbox_t *mbox)
{
    kern_mbox_free(mbox);
}

/** Check if an mbox is valid/allocated: return 1 for valid, 0 for invalid */
int sys_mbox_valid(sys_mbox_t *mbox)
{
    return kern_mbox_valid(mbox);
}

/** Set an mbox invalid so that sys_mbox_valid returns 0 */
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    kern_mbox_set_valid(mbox, FALSE);
}

/** The only thread function:
 * Creates a new thread
 * @param name human-readable name for the thread (used for debugging purposes)
 * @param thread thread-function
 * @param arg parameter passed to 'thread'
 * @param stacksize stack size in bytes for the new thread (may be ignored by ports)
 * @param prio priority of the new thread (may be ignored by ports) */
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
    if (prio < 20) {                                                    /*  保证网络协议栈相关的任务    */
        prio = 20;                                                      /*  的优先级在合理的范围        */
    } else if (prio > 40) {                                             /*  以保证网络性能              */
        prio = 40;
    }

    return kthread_create(name, thread, arg, stacksize, prio);
}

/** sys_init() must be called before anthing else. */
void sys_init(void)
{

}

/** Ticks/jiffies since power up. */
u32_t sys_jiffies(void)
{
    return get_tick();
}

/** Returns the current time in milliseconds,
 * may be the same as sys_jiffies or at least based on it. */
u32_t sys_now(void)
{
    return sys_jiffies() * 1000 / TICK_PER_SECOND;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
