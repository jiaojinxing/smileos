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
** File name:               sys_arch.c
** Last modified Date:      2012-2-22
** Last Version:            1.0.0
** Descriptions:            lwIP ϵͳģ���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-22
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "kern/kern.h"
#include "kern/ipc.h"
#include "lwip/sys.h"
#include <unistd.h>

/**
 * Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex
 */
err_t sys_mutex_new(sys_mutex_t *mutex)
{
    if (mutex_create(mutex) < 0) {
        return ERR_MEM;
    } else {
        return ERR_OK;
    }
}

/**
 * Lock a mutex
 * @param mutex the mutex to lock
 */
void sys_mutex_lock(sys_mutex_t *mutex)
{
    mutex_lock(mutex, 0);
}

/**
 * Unlock a mutex
 * @param mutex the mutex to unlock
 */
void sys_mutex_unlock(sys_mutex_t *mutex)
{
    mutex_unlock(mutex);
}

/**
 * Delete a semaphore
 * @param mutex the mutex to delete
 */
void sys_mutex_free(sys_mutex_t *mutex)
{
    mutex_destroy(mutex);
}

/**
 * Check if a mutex is valid/allocated
 * @return 1 for valid, 0 for invalid
 */
int sys_mutex_valid(sys_mutex_t *mutex)
{
    return mutex_valid(mutex);
}

/**
 * Set a mutex invalid so that sys_mutex_valid returns 0
 */
void sys_mutex_set_invalid(sys_mutex_t *mutex)
{
    mutex_set_valid(mutex, FALSE);
}

/**
 * Create a new semaphore
 * @param sem pointer to the semaphore to create
 * @param count initial count of the semaphore
 * @return ERR_OK if successful, another err_t otherwise
 */
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    if (sem_create(sem, count) < 0) {
        return ERR_MEM;
    } else {
        return ERR_OK;
    }
}

/**
 * Signals a semaphore
 * @param sem the semaphore to signal
 */
void sys_sem_signal(sys_sem_t *sem)
{
    sem_signal(sem);
}

/**
 * Wait for a semaphore for the specified timeout
 * @param sem the semaphore to wait for
 * @param timeout timeout in milliseconds to wait (0 = wait forever)
 * @return time (in milliseconds) waited for the semaphore
 *         or SYS_ARCH_TIMEOUT on timeout
 */
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    u32_t start;

    start = sys_now();

    if (sem_wait(sem, ms_to_tick(timeout)) < 0) {
        return SYS_ARCH_TIMEOUT;
    } else {
        return sys_now() - start;
    }
}

/**
 * Delete a semaphore
 * @param sem semaphore to delete
 */
void sys_sem_free(sys_sem_t *sem)
{
    sem_destroy(sem);
}

/**
 * Check if a sempahore is valid/allocated:
 * @return 1 for valid, 0 for invalid
 */
int sys_sem_valid(sys_sem_t *sem)
{
    return sem_valid(sem);
}

/**
 * Set a semaphore invalid so that sys_sem_valid returns 0
 */
void sys_sem_set_invalid(sys_sem_t *sem)
{
    sem_set_valid(sem, FALSE);
}

/**
 * Time functions.
 * only has a (close to) 1 jiffy resolution.
 */
void sys_msleep(u32_t ms)
{
    msleep(ms);
}

/**
 * Create a new mbox of specified size
 * @param mbox pointer to the mbox to create
 * @param size (miminum) number of messages in this mbox
 * @return ERR_OK if successful, another err_t otherwise
 */
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    if (mqueue_create(mbox, size) < 0) {
        return ERR_MEM;
    } else {
        return ERR_OK;
    }
}

/**
 * Post a message to an mbox - may not fail
 * -> blocks if full, only used from tasks not from ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL)
 */
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    mqueue_post(mbox, msg, 0);
}

/**
 * Try to post a message to an mbox - may fail if full or ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL)
 */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    if (mqueue_trypost(mbox, msg) < 0) {
        return ERR_BUF;
    } else {
        return ERR_OK;
    }
}

/**
 * Wait for a new message to arrive in the mbox
 * @param mbox mbox to get a message from
 * @param msg pointer where the message is stored
 * @param timeout maximum time (in milliseconds) to wait for a message
 * @return time (in milliseconds) waited for a message, may be 0 if not waited
           or SYS_ARCH_TIMEOUT on timeout
 *         The returned time has to be accurate to prevent timer jitter!
 */
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
    u32_t start;

    start = sys_now();

    if (mqueue_fetch(mbox, msg, ms_to_tick(timeout)) < 0) {
        return SYS_ARCH_TIMEOUT;
    } else {
        return sys_now() - start;
    }
}

/**
 * Wait for a new message to arrive in the mbox
 * Allow port to override with a macro, e.g. special timout for sys_arch_mbox_fetch()
 * @param mbox mbox to get a message from
 * @param msg pointer where the message is stored
 * @param timeout maximum time (in milliseconds) to wait for a message
 * @return 0 (milliseconds) if a message has been received
 *         or SYS_MBOX_EMPTY if the mailbox is empty
 */
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    if (mqueue_tryfetch(mbox, msg) < 0) {
        return SYS_MBOX_EMPTY;
    } else {
        return 0;
    }
}

/**
 * Delete an mbox
 * @param mbox mbox to delete
 */
void sys_mbox_free(sys_mbox_t *mbox)
{
    mqueue_destroy(mbox);
}

/**
 * Check if an mbox is valid/allocated:
 * @return 1 for valid, 0 for invalid
 */
int sys_mbox_valid(sys_mbox_t *mbox)
{
    return mqueue_valid(mbox);
}

/**
 * Set an mbox invalid so that sys_mbox_valid returns 0
 */
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    mqueue_set_valid(mbox, FALSE);
}

/**
 * The only thread function:
 * Creates a new thread
 * @param name human-readable name for the thread (used for debugging purposes)
 * @param thread thread-function
 * @param arg parameter passed to 'thread'
 * @param stacksize stack size in bytes for the new thread (may be ignored by ports)
 * @param prio priority of the new thread (may be ignored by ports)
 */
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
    return kthread_create(name, thread, arg, stacksize, prio);
}

/**
 * sys_init() must be called before anthing else.
 */
void sys_init(void)
{

}

/**
 * Ticks/jiffies since power up.
 */
u32_t sys_jiffies(void)
{
    return getticks();
}

/**
 * Returns the current time in milliseconds,
 * may be the same as sys_jiffies or at least based on it.
 */
u32_t sys_now(void)
{
    return tick_to_ms(getticks());
}

#include "lwip/ip_addr.h"

/**
 * ���ұ��� DNS ��¼
 */
u32_t sys_dns_lookup_local(const char *name)
{
    return IPADDR_NONE;
}

/**
 * �����߳�������Ŀ
 */
struct hostent *sys_thread_hostent(struct hostent *hostent)
{
    return NULL;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
