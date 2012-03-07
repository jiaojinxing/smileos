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
#include "kern/kern.h"
#include "kern/sys_call.h"
#include "lwip/sys.h"

/*
 * 互斥量
 */
struct sys_mutex {
    task_t      *owner;
    task_t      *wait_list;
    int          lock;
    int          valid;
};

/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex */
err_t sys_mutex_new(sys_mutex_t *mutex)
{
    struct sys_mutex *m;

    m = kmalloc(sizeof(struct sys_mutex));
    if (m) {
        m->owner     = NULL;
        m->wait_list = NULL;
        m->lock      = 0;
        m->valid     = TRUE;

        *mutex       = m;
        return ERR_OK;
    }

    *mutex = NULL;
    return ERR_MEM;
}

/** Lock a mutex
 * @param mutex the mutex to lock */
void sys_mutex_lock(sys_mutex_t *mutex)
{
    struct sys_mutex *m;
    task_t *end;
    uint32_t reg;

    if (in_interrupt()) {
        return;
    }

    reg = interrupt_disable();
    if (mutex) {
        again:
        m = *mutex;
        if (m) {
            if (m->valid) {
                if (!m->lock) {
                    m->lock++;
                    m->owner = current;
                } else if (m->owner == current) {
                    m->lock++;
                } else {
                    if (!m->wait_list) {
                        m->wait_list = current;
                    } else {
                        end = m->wait_list;
                        while (end->next) {
                            end = end->next;
                        }
                        end->next = current;
                    }
                    current->next = NULL;

                    current->timer = 0;
                    current->state = TASK_SUSPEND;
                    current->wait_list = &m->wait_list;
                    current->resume_type = TASK_RESUME_UNKNOW;
                    yield();
                    current->wait_list = NULL;
                    current->next = NULL;
                    current->resume_type = TASK_RESUME_UNKNOW;
                    goto again;
                }
            }
        }
    }
    interrupt_resume(reg);
}

/** Unlock a mutex
 * @param mutex the mutex to unlock */
void sys_mutex_unlock(sys_mutex_t *mutex)
{
    struct sys_mutex *m;
    task_t *task;
    uint32_t reg;

    if (in_interrupt()) {
        return;
    }

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m) {
            if (m->valid) {
                if (m->lock) {
                    if (m->owner == current) {
                        m->lock--;
                        if (!m->lock) {
                            task = m->wait_list;
                            if (task) {
                                task->timer = 0;
                                task->state = TASK_RUNNING;
                                m->wait_list = task->next;
                                task->wait_list = NULL;
                                task->next = NULL;
                                task->resume_type = TASK_RESUME_MUTEX_COME;
                                yield();
                            }
                        }
                    }
                }
            }
        }
    }
    interrupt_resume(reg);
}

/** Delete a semaphore
 * @param mutex the mutex to delete */
void sys_mutex_free(sys_mutex_t *mutex)
{
    struct sys_mutex *m;
    uint32_t reg;

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m) {
            if (m->valid) {
                if (!m->lock && !m->wait_list) {
                    m->valid = FALSE;
                    kfree(m);
                    *mutex = NULL;
                }
            }
        }
    }
    interrupt_resume(reg);
}

/** Check if a mutex is valid/allocated: return 1 for valid, 0 for invalid */
int sys_mutex_valid(sys_mutex_t *mutex)
{
    struct sys_mutex *m;
    uint32_t reg;
    int valid = 0;

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m) {
            valid = m->valid;
        }
    }
    interrupt_resume(reg);
    return valid;
}

/** Set a mutex invalid so that sys_mutex_valid returns 0 */
void sys_mutex_set_invalid(sys_mutex_t *mutex)
{
    struct sys_mutex *m;
    uint32_t reg;

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m) {
            m->valid = FALSE;
        }
    }
    interrupt_resume(reg);
}

/*
 * 信号量
 */
struct sys_sem {
    task_t      *wait_list;
    int          count;
    int          valid;
};

/** Create a new semaphore
 * @param sem pointer to the semaphore to create
 * @param count initial count of the semaphore
 * @return ERR_OK if successful, another err_t otherwise */
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    struct sys_sem *s;

    s = kmalloc(sizeof(struct sys_sem));
    if (s) {
        s->wait_list = NULL;
        s->count     = count;
        s->valid     = TRUE;

        *sem         = s;
        return ERR_OK;
    }

    *sem = NULL;
    return ERR_MEM;
}

/** Signals a semaphore
 * @param sem the semaphore to signal */
void sys_sem_signal(sys_sem_t *sem)
{
    struct sys_sem *s;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s) {
            if (s->valid) {
                s->count++;
                task = s->wait_list;
                if (task) {
                    task->timer = 0;
                    task->state = TASK_RUNNING;
                    s->wait_list = task->next;
                    task->wait_list = NULL;
                    task->next = NULL;
                    task->resume_type = TASK_RESUME_SEM_COME;
                    if (!in_interrupt()) {
                        yield();
                    }
                }
            }
        }
    }
    interrupt_resume(reg);
}

/** Wait for a semaphore for the specified timeout
 * @param sem the semaphore to wait for
 * @param timeout timeout in milliseconds to wait (0 = wait forever)
 * @return time (in milliseconds) waited for the semaphore
 *         or SYS_ARCH_TIMEOUT on timeout */
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    struct sys_sem *s;
    task_t *end;
    u32_t start;
    uint32_t reg;

    if (in_interrupt()) {
        return SYS_ARCH_TIMEOUT;
    }

    start = sys_now();

    reg = interrupt_disable();
    if (sem) {
        again:
        s = *sem;
        if (s) {
            if (s->valid) {
                if (s->count) {
                    s->count--;
                    interrupt_resume(reg);
                    return sys_now() - start;
                } else {
                    if (!s->wait_list) {
                        s->wait_list = current;
                    } else {
                        end = s->wait_list;
                        while (end->next) {
                            end = end->next;
                        }
                        end->next = current;
                    }
                    current->next = NULL;

                    if (timeout == 0) {
                        current->timer = (u32_t)-1;
                    } else {
                        current->timer = timeout * 1000 / TICK_PER_SECOND;
                    }
                    current->state = TASK_SLEEPING;
                    current->wait_list = &s->wait_list;
                    current->resume_type = TASK_RESUME_UNKNOW;
                    yield();
                    current->wait_list = NULL;
                    current->next = NULL;
                    current->timer = 0;
                    if (current->resume_type & TASK_RESUME_TIMEOUT) {
                        current->resume_type = TASK_RESUME_UNKNOW;
                        goto out;
                    }
                    current->resume_type = TASK_RESUME_UNKNOW;
                    goto again;
                }
            } else {
                goto out;
            }
        } else {
            goto out;
        }
    }
    out:
    interrupt_resume(reg);
    return SYS_ARCH_TIMEOUT;
}

/** Delete a semaphore
 * @param sem semaphore to delete */
void sys_sem_free(sys_sem_t *sem)
{
    struct sys_sem *s;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s) {
            if (s->valid) {
                if (!s->wait_list) {
                    s->valid = FALSE;
                    kfree(s);
                    *sem = NULL;
                }
            }
        }
    }
    interrupt_resume(reg);
}

/** Check if a sempahore is valid/allocated: return 1 for valid, 0 for invalid */
int sys_sem_valid(sys_sem_t *sem)
{
    struct sys_sem *s;
    uint32_t reg;
    int valid = 0;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s) {
            valid = s->valid;
        }
    }
    interrupt_resume(reg);
    return valid;
}

/** Set a semaphore invalid so that sys_sem_valid returns 0 */
void sys_sem_set_invalid(sys_sem_t *sem)
{
    struct sys_sem *s;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s) {
            s->valid = FALSE;
        }
    }
    interrupt_resume(reg);
}

/* Time functions. */
void sys_msleep(u32_t ms) /* only has a (close to) 1 jiffy resolution. */
{
    usleep(1000 * ms);
}

/*
 * 邮箱
 */
struct sys_mbox {
    task_t      *r_wait_list;
    task_t      *w_wait_list;
    int          size;
    int          cnt;
    int          in;
    int          out;
    int          valid;
    void        *msg[1];
};

/** Create a new mbox of specified size
 * @param mbox pointer to the mbox to create
 * @param size (miminum) number of messages in this mbox
 * @return ERR_OK if successful, another err_t otherwise */
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    struct sys_mbox *q;

    if (size < 100) {
        size = 100;
    }

    q = kmalloc(sizeof(struct sys_mbox) + (size - 1) * sizeof(void *));
    if (q) {
        q->r_wait_list = NULL;
        q->w_wait_list = NULL;
        q->size        = size;
        q->cnt         = 0;
        q->in          = 0;
        q->out         = 0;
        q->valid       = TRUE;

        *mbox          = q;
        return ERR_OK;
    }

    *mbox = NULL;
    return ERR_MEM;
}

/** Post a message to an mbox - may not fail
 * -> blocks if full, only used from tasks not from ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL) */
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    struct sys_mbox *q;
    task_t *end;
    task_t *task;
    uint32_t reg;

    if (in_interrupt()) {
        return;
    }

    reg = interrupt_disable();
    if (mbox) {
        again:
        q = *mbox;
        if (q) {
            if (q->valid) {
                if (q->cnt < q->size) {
                    q->msg[q->in] = msg;
                    q->in = ++q->in % q->size;
                    q->cnt++;

                    task = q->r_wait_list;
                    if (task) {
                        task->timer = 0;
                        task->state = TASK_RUNNING;
                        q->r_wait_list = task->next;
                        task->wait_list = NULL;
                        task->next = NULL;
                        task->resume_type = TASK_RESUME_MSG_COME;
                        yield();
                    }
                } else {
                    if (!q->w_wait_list) {
                        q->w_wait_list = current;
                    } else {
                        end = q->w_wait_list;
                        while (end->next) {
                            end = end->next;
                        }
                        end->next = current;
                    }
                    current->next = NULL;

                    current->timer = 0;
                    current->state = TASK_SUSPEND;
                    current->wait_list = &q->w_wait_list;
                    current->resume_type = TASK_RESUME_UNKNOW;
                    yield();
                    current->wait_list = NULL;
                    current->next = NULL;
                    current->resume_type = TASK_RESUME_UNKNOW;
                    goto again;
                }
            }
        }
    }
    interrupt_resume(reg);
}

/** Try to post a message to an mbox - may fail if full or ISR
 * @param mbox mbox to posts the message
 * @param msg message to post (ATTENTION: can be NULL) */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    struct sys_mbox *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q) {
            if (q->valid) {
                if (q->cnt < q->size) {
                    q->msg[q->in] = msg;
                    q->in = ++q->in % q->size;
                    q->cnt++;

                    task = q->r_wait_list;
                    if (task) {
                        task->timer = 0;
                        task->state = TASK_RUNNING;
                        q->r_wait_list = task->next;
                        task->wait_list = NULL;
                        task->next = NULL;
                        task->resume_type = TASK_RESUME_MSG_COME;
                        if (!in_interrupt()) {
                            yield();
                        }
                    }
                    interrupt_resume(reg);
                    return ERR_OK;
                }
            }
        }
    }
    interrupt_resume(reg);
    return ERR_BUF;
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
    struct sys_mbox *q;
    task_t *end;
    task_t *task;
    u32_t start;
    uint32_t reg;

    if (in_interrupt()) {
        return SYS_ARCH_TIMEOUT;
    }

    start = sys_now();

    reg = interrupt_disable();
    if (mbox) {
        again:
        q = *mbox;
        if (q) {
            if (q->valid) {
                if (q->cnt) {
                    *msg = q->msg[q->out];
                    q->out = ++q->out % q->size;
                    q->cnt--;

                    task = q->w_wait_list;
                    if (task) {
                        task->timer = 0;
                        task->state = TASK_RUNNING;
                        q->w_wait_list = task->next;
                        task->wait_list = NULL;
                        task->next = NULL;
                        task->resume_type = TASK_RESUME_MSG_OUT;
                        yield();
                    }
                    interrupt_resume(reg);
                    return sys_now() - start;
                } else {
                    if (!q->r_wait_list) {
                        q->r_wait_list = current;
                    } else {
                        end = q->r_wait_list;
                        while (end->next) {
                            end = end->next;
                        }
                        end->next = current;
                    }
                    current->next = NULL;

                    if (timeout == 0) {
                        current->timer = (u32_t)-1;
                    } else {
                        current->timer = timeout * 1000 / TICK_PER_SECOND;
                    }
                    current->state = TASK_SLEEPING;
                    current->wait_list = &q->r_wait_list;
                    current->resume_type = TASK_RESUME_UNKNOW;
                    yield();
                    current->wait_list = NULL;
                    current->next = NULL;
                    current->timer = 0;
                    if (current->resume_type & TASK_RESUME_TIMEOUT) {
                        current->resume_type = TASK_RESUME_UNKNOW;
                        goto out;
                    }
                    current->resume_type = TASK_RESUME_UNKNOW;
                    goto again;
                }
            }
        }
    }
    out:
    interrupt_resume(reg);
    return SYS_ARCH_TIMEOUT;
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
    struct sys_mbox *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q) {
            if (q->valid) {
                if (q->cnt) {
                    *msg = q->msg[q->out];
                    q->out = ++q->out % q->size;
                    q->cnt--;

                    task = q->w_wait_list;
                    if (task) {
                        task->timer = 0;
                        task->state = TASK_RUNNING;
                        q->w_wait_list = task->next;
                        task->wait_list = NULL;
                        task->next = NULL;
                        task->resume_type = TASK_RESUME_MSG_OUT;
                        if (!in_interrupt()) {
                            yield();
                        }
                    }
                    interrupt_resume(reg);
                    return 0;
                }
            }
        }
    }
    interrupt_resume(reg);
    return SYS_MBOX_EMPTY;
}

/** Delete an mbox
 * @param mbox mbox to delete */
void sys_mbox_free(sys_mbox_t *mbox)
{
    struct sys_mbox *q;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q) {
            if (q->valid) {
                if (!q->r_wait_list && !q->w_wait_list) {
                    q->valid = FALSE;
                    kfree(q);
                    *mbox = NULL;
                }
            }
        }
    }
    interrupt_resume(reg);
}

/** Check if an mbox is valid/allocated: return 1 for valid, 0 for invalid */
int sys_mbox_valid(sys_mbox_t *mbox)
{
    struct sys_mbox *q;
    uint32_t reg;
    int valid = 0;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q) {
            valid = q->valid;
        }
    }
    interrupt_resume(reg);
    return valid;
}

/** Set an mbox invalid so that sys_mbox_valid returns 0 */
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    struct sys_mbox *q;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q) {
            q->valid = FALSE;
        }
    }
    interrupt_resume(reg);
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
    if (stacksize < 32 * KB) {
        stacksize = 32 * KB;
    }

    if (prio < 10) {
        prio = 10;
    }

    return kthread_create(name, thread, arg, stacksize, prio);
}

/* sys_init() must be called before anthing else. */
void sys_init(void)
{

}

/** Ticks/jiffies since power up. */
u32_t sys_jiffies(void)
{
    struct timeval tv;
    u32_t jiffies;

    gettimeofday(&tv, NULL);

    jiffies = tv.tv_sec * TICK_PER_SECOND + tv.tv_usec * TICK_PER_SECOND / 1000000;

    return jiffies;
}

/** Returns the current time in milliseconds,
 * may be the same as sys_jiffies or at least based on it. */
u32_t sys_now(void)
{
    return sys_jiffies() / TICK_PER_SECOND * 1000;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
