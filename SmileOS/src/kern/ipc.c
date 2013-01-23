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
** File name:               ipc.c
** Last modified Date:      2012-3-13
** Last Version:            1.0.0
** Descriptions:            �����ͨ�Ż���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-13
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
#include "kern/kvars.h"
/*********************************************************************************************************
** ������
*********************************************************************************************************/
/*
 * �ȴ��¼�
 */
#define wait_event_forever(__wait_list, __resume_type)              \
                    if (__wait_list == NULL) {                      \
                        __wait_list = current;                      \
                    } else {                                        \
                        task_t *end = __wait_list;                  \
                        while (end->next != NULL) {                 \
                            end = end->next;                        \
                        }                                           \
                        end->next = current;                        \
                    }                                               \
                    current->next = NULL;                           \
                                                                    \
                    current->delay = 0;                             \
                    current->status = TASK_SUSPEND;                 \
                    current->wait_list = &__wait_list;              \
                    current->resume_type = TASK_RESUME_UNKNOW;      \
                    schedule();                                     \
                    current->wait_list = NULL;                      \
                    current->next = NULL;                           \
                    __resume_type = current->resume_type;           \
                    current->resume_type = TASK_RESUME_UNKNOW

/*
 * �ȴ��¼�ֱ����ʱ
 */
#define wait_event_timeout(__wait_list, __resume_type, __timeout, __ticks)   \
                    if (__wait_list == NULL) {                      \
                        __wait_list = current;                      \
                    } else {                                        \
                        task_t *end = __wait_list;                  \
                        while (end->next != NULL) {                 \
                            end = end->next;                        \
                        }                                           \
                        end->next = current;                        \
                    }                                               \
                    current->next = NULL;                           \
                                                                    \
                    current->delay = __timeout;                     \
                    current->status = TASK_SLEEPING;                \
                    current->wait_list = &__wait_list;              \
                    current->resume_type = TASK_RESUME_UNKNOW;      \
                    __ticks = getticks();                           \
                    schedule();                                     \
                    __ticks = getticks() - __ticks;                 \
                    current->wait_list = NULL;                      \
                    current->next = NULL;                           \
                    current->delay = 0;                             \
                    __resume_type = current->resume_type;           \
                    current->resume_type = TASK_RESUME_UNKNOW

/*
 * �ָ�����
 */
#define resume_task(__task, __wait_list, __resume_type)             \
                    __task->delay = 0;                              \
                    __task->status = TASK_RUNNING;                  \
                    __wait_list = __task->next;                     \
                    __task->wait_list = NULL;                       \
                    __task->next = NULL;                            \
                    __task->resume_type = __resume_type;            \
                    if (!in_interrupt() &&                          \
                        __task->type == TASK_TYPE_KTHREAD &&        \
                        __task->priority >= current->priority) {    \
                        schedule();                                 \
                    }
/*********************************************************************************************************
** IPC �������ͷ��� IPC �������λ, ��Ч�Է��ڴ�λ, ��֤ IPC ����ؼ���Ա��������
*********************************************************************************************************/
typedef enum {
    IPC_TYPE_MUTEX   = 0xABCD1A1A,                                      /*  ������                      */
    IPC_TYPE_SEM     = 0xABCD2B2B,                                      /*  �ź���                      */
    IPC_TYPE_MQUEUE  = 0xABCD3C3C,                                      /*  ��Ϣ����                    */
    IPC_TYPE_DESTROY = 0xABCD4D4D,                                      /*  �Ѿ�����                    */
} ipc_type_t;
/*********************************************************************************************************
** ������
*********************************************************************************************************/
struct mutex {
    ipc_type_t  type;                                                   /*  IPC ����                    */
    int         valid;                                                  /*  ��Ч��                      */
    task_t     *wait_list;                                              /*  �ȴ�����                    */
    task_t     *owner;                                                  /*  ӵ����                      */
    uint32_t    lock;                                                   /*  �������                    */
};
/*********************************************************************************************************
** Function name:           mutex_new
** Descriptions:            ����һ���µĻ�����
** input parameters:        NONE
** output parameters:       mutex               ������
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_new(mutex_t *mutex)
{
    struct mutex *m;

    m = kmalloc(sizeof(struct mutex));
    if (m) {
        m->type      = IPC_TYPE_MUTEX;
        m->owner     = NULL;
        m->wait_list = NULL;
        m->lock      = 0;
        m->valid     = TRUE;

        *mutex       = m;
        return 0;
    }

    *mutex = NULL;
    return -1;
}
/*********************************************************************************************************
** Function name:           mutex_trylock
** Descriptions:            ���ԶԻ��������м���
** input parameters:        mutex               ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_trylock(mutex_t *mutex)
{
    struct mutex *m;
    uint32_t reg;

    if (in_interrupt()) {
        return -1;
    }

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX && m->valid) {
            if (!m->lock) {
                m->lock++;
                m->owner = current;
            } else if (m->owner == current) {
                m->lock++;
            } else {
                goto error;
            }
            interrupt_resume(reg);
            return 0;
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mutex_lock
** Descriptions:            �Ի��������м���
** input parameters:        mutex               ������
**                          timeout             ��ʱ TICK ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_lock(mutex_t *mutex, uint32_t timeout)
{
    struct mutex *m;
    uint32_t reg;
    uint32_t resume_type;
    uint64_t ticks;

    if (in_interrupt()) {
        return -1;
    }

    reg = interrupt_disable();
    if (mutex) {
        again:
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX && m->valid) {
            if (!m->lock) {
                m->lock++;
                m->owner = current;
            } else if (m->owner == current) {
                m->lock++;
            } else {
                printk("%s: %s lock mutex, %s wait it\n", __func__, m->owner->name, current->name);
                if (timeout != 0) {
                    wait_event_timeout(m->wait_list, resume_type, timeout, ticks);
                } else {
                    wait_event_forever(m->wait_list, resume_type);
                }
                if (resume_type & TASK_RESUME_INTERRUPT || resume_type & TASK_RESUME_TIMEOUT) {
                    goto error;
                } else {
                    if (timeout != 0) {
                        if (timeout <= ticks) {
                            goto error;
                        }
                        timeout -= ticks;
                    }
                    goto again;
                }
            }
            interrupt_resume(reg);
            return 0;
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mutex_unlock
** Descriptions:            �Ի��������н���
** input parameters:        mutex               ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_unlock(mutex_t *mutex)
{
    struct mutex *m;
    task_t *task;
    uint32_t reg;

    if (in_interrupt()) {
        return -1;
    }

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX && m->valid) {
            if (m->lock) {
                if (m->owner == current) {
                    m->lock--;
                    if (!m->lock) {
                        task = m->wait_list;
                        if (task) {
                            resume_task(task, m->wait_list, TASK_RESUME_MUTEX_COME);
                        }
                    }
                    interrupt_resume(reg);
                    return 0;
                }
            }
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mutex_abort
** Descriptions:            ��ֹ�ȴ�������
** input parameters:        mutex               ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_abort(mutex_t *mutex)
{
    struct mutex *m;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX && m->valid) {
            while ((task = m->wait_list) != NULL) {
                resume_task(task, m->wait_list, TASK_RESUME_INTERRUPT);
            }
            interrupt_resume(reg);
            return 0;
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mutex_free
** Descriptions:            ɾ��������
** input parameters:        mutex               ������
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_free(mutex_t *mutex)
{
    struct mutex *m;
    uint32_t reg;

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX && m->valid) {
            if (!m->lock) {
                m->valid = FALSE;
                m->type = IPC_TYPE_DESTROY;
                kfree(m);
                *mutex = NULL;
                interrupt_resume(reg);
                return 0;
            }
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mutex_valid
** Descriptions:            �жϻ������Ƿ���Ч
** input parameters:        mutex               ������
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int mutex_valid(mutex_t *mutex)
{
    struct mutex *m;
    uint32_t reg;
    int valid = FALSE;

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX) {
            valid = m->valid;
        }
    }
    interrupt_resume(reg);
    return valid;
}
/*********************************************************************************************************
** Function name:           mutex_set_valid
** Descriptions:            ���û���������Ч��
** input parameters:        mutex               ������
**                          valid               <= 0 : ��Ч, > 0 : ��Ч
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_set_valid(mutex_t *mutex, int valid)
{
    struct mutex *m;
    uint32_t reg;

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX) {
            m->valid = valid > 0 ? TRUE : FALSE;
            interrupt_resume(reg);
            return 0;
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** �ź���
*********************************************************************************************************/
struct sem {
    ipc_type_t  type;                                                   /*  IPC ����                    */
    int         valid;                                                  /*  ��Ч��                      */
    task_t     *wait_list;                                              /*  �ȴ�����                    */
    uint32_t    count;                                                  /*  �źż���                    */
};
/*********************************************************************************************************
** Function name:           sem_new
** Descriptions:            ����һ���µ��ź���
** input parameters:        count               �źų�ʼ����
** output parameters:       sem                 �ź���
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_new(sem_t *sem, uint32_t count)
{
    struct sem *s;

    s = kmalloc(sizeof(struct sem));
    if (s) {
        s->type      = IPC_TYPE_SEM;
        s->wait_list = NULL;
        s->count     = count;
        s->valid     = TRUE;

        *sem         = s;
        return 0;
    }

    *sem = NULL;
    return -1;
}
/*********************************************************************************************************
** Function name:           sem_trywait
** Descriptions:            ���Ի���ź���
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_trywait(sem_t *sem)
{
    struct sem *s;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM && s->valid) {
            if (s->count) {
                s->count--;
                interrupt_resume(reg);
                return 0;
            } else {
                goto error;
            }
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           sem_wait
** Descriptions:            ����ź���
** input parameters:        sem                 �ź���
**                          timeout             ��ʱ TICK ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_wait(sem_t *sem, uint32_t timeout)
{
    struct sem *s;
    uint32_t reg;
    uint32_t resume_type;
    uint64_t ticks;

    if (in_interrupt()) {
        return sem_trywait(sem);
    }

    reg = interrupt_disable();
    if (sem) {
        again:
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM && s->valid) {
            if (s->count) {
                s->count--;
                interrupt_resume(reg);
                return 0;
            } else {
                if (timeout != 0) {
                    wait_event_timeout(s->wait_list, resume_type, timeout, ticks);
                } else {
                    wait_event_forever(s->wait_list, resume_type);
                }
                if (resume_type & TASK_RESUME_INTERRUPT || resume_type & TASK_RESUME_TIMEOUT) {
                    goto error;
                } else {
                    if (timeout != 0) {
                        if (timeout <= ticks) {
                            goto error;
                        }
                        timeout -= ticks;
                    }
                    goto again;
                }
            }
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           sem_signal
** Descriptions:            ����һ���ź���
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_signal(sem_t *sem)
{
    struct sem *s;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM && s->valid) {
            s->count++;
            task = s->wait_list;
            if (task) {
                resume_task(task, s->wait_list, TASK_RESUME_SEM_COME);
            }
            interrupt_resume(reg);
            return 0;
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           sem_sync
** Descriptions:            ����������ڵȴ�, ����һ���ź���
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_sync(sem_t *sem)
{
    struct sem *s;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM && s->valid) {
            task = s->wait_list;
            if (task) {
                s->count++;
                resume_task(task, s->wait_list, TASK_RESUME_SEM_COME);
            }
            interrupt_resume(reg);
            return 0;
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           sem_abort
** Descriptions:            ��ֹ�ȴ��ź���
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_abort(sem_t *sem)
{
    struct sem *s;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM && s->valid) {
            while ((task = s->wait_list) != NULL) {
                resume_task(task, s->wait_list, TASK_RESUME_INTERRUPT);
            }
            interrupt_resume(reg);
            return 0;
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           sem_free
** Descriptions:            ɾ���ź���
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_free(sem_t *sem)
{
    struct sem *s;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM && s->valid) {
            if (!s->wait_list) {
                s->valid = FALSE;
                s->type = IPC_TYPE_DESTROY;
                kfree(s);
                *sem = NULL;
                interrupt_resume(reg);
                return 0;
            }
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           sem_valid
** Descriptions:            �ж��ź����Ƿ���Ч
** input parameters:        sem                 �ź���
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int sem_valid(sem_t *sem)
{
    struct sem *s;
    uint32_t reg;
    int valid = FALSE;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM) {
            valid = s->valid;
        }
    }
    interrupt_resume(reg);
    return valid;
}
/*********************************************************************************************************
** Function name:           sem_set_valid
** Descriptions:            �����ź�������Ч��
** input parameters:        sem                 �ź���
**                          valid               <= 0 : ��Ч, > 0 : ��Ч
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_set_valid(sem_t *sem, int valid)
{
    struct sem *s;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM) {
            s->valid = valid > 0 ? TRUE : FALSE;
            interrupt_resume(reg);
            return 0;
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** ��Ϣ����
*********************************************************************************************************/
struct mqueue {
    ipc_type_t  type;                                                   /*  IPC ����                    */
    int         valid;                                                  /*  ��Ч��                      */
    task_t     *r_wait_list;                                            /*  ����Ϣ���еȴ�����          */
    task_t     *w_wait_list;                                            /*  д��Ϣ���еȴ�����          */
    uint32_t    size;                                                   /*  ��Ϣ���еĴ�С              */
    uint32_t    nr;                                                     /*  ��Ϣ�������δ����Ϣ��      */
    uint32_t    in;                                                     /*  ��ӵ�                      */
    uint32_t    out;                                                    /*  ���ӵ�                      */
    void       *msg[1];                                                 /*  ��Ϣ����                    */
};
/*********************************************************************************************************
** Function name:           mqueue_new
** Descriptions:            ����һ���µ���Ϣ����
** input parameters:        size                ��Ϣ���еĴ�С
** output parameters:       mqueue              ��Ϣ����
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_new(mqueue_t *mqueue, uint32_t size)
{
    struct mqueue *q;

    if (size < 10) {
        size = 10;
    }

    q = kmalloc(sizeof(struct mqueue) + (size - 1) * sizeof(void *));
    if (q) {
        q->type        = IPC_TYPE_MQUEUE;
        q->r_wait_list = NULL;
        q->w_wait_list = NULL;
        q->size        = size;
        q->nr          = 0;
        q->in          = 0;
        q->out         = 0;
        q->valid       = TRUE;

        *mqueue        = q;
        return 0;
    }

    *mqueue = NULL;
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_trypost
** Descriptions:            ����Ͷ����Ϣ����Ϣ����
** input parameters:        mqueue              ��Ϣ����
**                          msg                 ��Ϣ
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_trypost(mqueue_t *mqueue, void *msg)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE && q->valid) {
            if (q->nr < q->size) {
                q->msg[q->in] = msg;
                q->in++;
                q->in %= q->size;
                q->nr++;

                task = q->r_wait_list;
                if (task) {
                    resume_task(task, q->r_wait_list, TASK_RESUME_MSG_COME);
                }
                interrupt_resume(reg);
                return 0;
            }
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_post
** Descriptions:            Ͷ����Ϣ����Ϣ����
** input parameters:        mqueue              ��Ϣ����
**                          msg                 ��Ϣ
**                          timeout             ��ʱ TICK ��
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_post(mqueue_t *mqueue, void *msg, uint32_t timeout)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;
    uint32_t resume_type;
    uint64_t ticks;

    if (in_interrupt()) {
        return mqueue_trypost(mqueue, msg);
    }

    reg = interrupt_disable();
    if (mqueue) {
        again:
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE && q->valid) {
            if (q->nr < q->size) {
                q->msg[q->in] = msg;
                q->in++;
                q->in %= q->size;
                q->nr++;

                task = q->r_wait_list;
                if (task) {
                    resume_task(task, q->r_wait_list, TASK_RESUME_MSG_COME);
                }
                interrupt_resume(reg);
                return 0;
            } else {
                if (timeout != 0) {
                    wait_event_timeout(q->w_wait_list, resume_type, timeout, ticks);
                } else {
                    wait_event_forever(q->w_wait_list, resume_type);
                }
                if (resume_type & TASK_RESUME_INTERRUPT || resume_type & TASK_RESUME_TIMEOUT) {
                    goto error;
                } else {
                    if (timeout != 0) {
                        if (timeout <= ticks) {
                            goto error;
                        }
                        timeout -= ticks;
                    }
                    goto again;
                }
            }
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_tryfetch
** Descriptions:            ���Դ���Ϣ������ȡ����Ϣ
** input parameters:        mqueue              ��Ϣ����
** output parameters:       msg                 ��Ϣ
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_tryfetch(mqueue_t *mqueue, void **msg)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE && q->valid) {
            if (q->nr) {
                *msg = q->msg[q->out];
                q->out++;
                q->out %= q->size;
                q->nr--;

                task = q->w_wait_list;
                if (task) {
                    resume_task(task, q->w_wait_list, TASK_RESUME_MSG_OUT);
                }
                interrupt_resume(reg);
                return 0;
            }
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_fetch
** Descriptions:            ����Ϣ������ȡ����Ϣ
** input parameters:        mqueue              ��Ϣ����
**                          timeout             ��ʱ TICK ��
** output parameters:       msg                 ��Ϣ
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_fetch(mqueue_t *mqueue, void **msg, uint32_t timeout)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;
    uint32_t resume_type;
    uint64_t ticks;

    if (in_interrupt()) {
        return mqueue_tryfetch(mqueue, msg);
    }

    reg = interrupt_disable();
    if (mqueue) {
        again:
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE && q->valid) {
            if (q->nr) {
                *msg = q->msg[q->out];
                q->out++;
                q->out %= q->size;
                q->nr--;

                task = q->w_wait_list;
                if (task) {
                    resume_task(task, q->w_wait_list, TASK_RESUME_MSG_OUT);
                }
                interrupt_resume(reg);
                return 0;
            } else {
                if (timeout != 0) {
                    wait_event_timeout(q->r_wait_list, resume_type, timeout, ticks);
                } else {
                    wait_event_forever(q->r_wait_list, resume_type);
                }
                if (resume_type & TASK_RESUME_INTERRUPT || resume_type & TASK_RESUME_TIMEOUT) {
                    goto error;
                } else {
                    if (timeout != 0) {
                        if (timeout <= ticks) {
                            goto error;
                        }
                        timeout -= ticks;
                    }
                    goto again;
                }
            }
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_flush
** Descriptions:            �����Ϣ����
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_flush(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;
    int i;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE && q->valid) {
            q->nr  = 0;
            q->in  = 0;
            q->out = 0;

            for (i = 0; ((task = q->w_wait_list) != NULL) && i < q->size; i++) {
                resume_task(task, q->w_wait_list, TASK_RESUME_MSG_OUT);
            }
            interrupt_resume(reg);
            return 0;
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_msg_nr
** Descriptions:            ������Ϣ��������Ϣ����Ŀ
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          ��Ϣ����Ŀ OR -1
*********************************************************************************************************/
int mqueue_msg_nr(mqueue_t *mqueue)
{
    struct mqueue *q;
    uint32_t reg;
    uint32_t nr;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE && q->valid) {
            nr = q->nr;
            interrupt_resume(reg);
            return nr;
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_abort_fetch
** Descriptions:            ��ֹ�ȴ���ȡ��Ϣ
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort_fetch(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE && q->valid) {
            while ((task = q->r_wait_list) != NULL) {
                resume_task(task, q->r_wait_list, TASK_RESUME_INTERRUPT);
            }
            interrupt_resume(reg);
            return 0;
        }
    }

    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_abort_post
** Descriptions:            ��ֹ�ȴ�Ͷ����Ϣ
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort_post(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE && q->valid) {
            while ((task = q->w_wait_list) != NULL) {
                resume_task(task, q->w_wait_list, TASK_RESUME_INTERRUPT);
            }
            interrupt_resume(reg);
            return 0;
        }
    }

    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_abort
** Descriptions:            ��ֹ�ȴ���Ϣ����
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE && q->valid) {
            while ((task = q->w_wait_list) != NULL) {
                resume_task(task, q->w_wait_list, TASK_RESUME_INTERRUPT);
            }

            while ((task = q->r_wait_list) != NULL) {
                resume_task(task, q->r_wait_list, TASK_RESUME_INTERRUPT);
            }
            interrupt_resume(reg);
            return 0;
        }
    }

    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_free
** Descriptions:            ɾ����Ϣ����
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_free(mqueue_t *mqueue)
{
    struct mqueue *q;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE && q->valid) {
            if (!q->r_wait_list && !q->w_wait_list) {
                q->valid = FALSE;
                q->type = IPC_TYPE_DESTROY;
                kfree(q);
                *mqueue = NULL;
                interrupt_resume(reg);
                return 0;
            }
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mqueue_valid
** Descriptions:            �ж���Ϣ�����Ƿ���Ч
** input parameters:        mqueue              ��Ϣ����
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
int mqueue_valid(mqueue_t *mqueue)
{
    struct mqueue *q;
    uint32_t reg;
    int valid = FALSE;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE) {
            valid = q->valid;
        }
    }
    interrupt_resume(reg);
    return valid;
}
/*********************************************************************************************************
** Function name:           mqueue_set_valid
** Descriptions:            ������Ϣ���е���Ч��
** input parameters:        mqueue              ��Ϣ����
**                          valid               <= 0 : ��Ч, > 0 : ��Ч
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_set_valid(mqueue_t *mqueue, int valid)
{
    struct mqueue *q;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_MQUEUE) {
            q->valid = valid > 0 ? TRUE : FALSE;
            interrupt_resume(reg);
            return 0;
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           ipc_task_init
** Descriptions:            ��ʼ������� IPC
** input parameters:        task                ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int ipc_task_init(task_t *task)
{
    task->next      = NULL;
    task->wait_list = NULL;
    return 0;
}
/*********************************************************************************************************
** Function name:           ipc_task_cleanup
** Descriptions:            ��������� IPC
** input parameters:        task                ����
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int ipc_task_cleanup(task_t *task)
{
    if (task->wait_list != NULL) {                                      /*  ���������ĳ���ȴ�����      */
        task_t *prev = *task->wait_list;                                /*  ��õȴ��������ͷ����      */
        if (task == prev) {                                             /*  ������������ͷ����        */
            *task->wait_list = task->next;                              /*  �޸ĵȴ��������ͷ          */
        } else {
            while (prev != NULL && prev->next != task) {                /*  �ҳ������ڵȴ������е�ǰ��  */
                prev = prev->next;
            }
            if (prev != NULL) {                                         /*  ������ҵ�                  */
                prev->next = task->next;                                /*  �޸�ǰ���ĺ���              */
            }
        }
        task->next      = NULL;
        task->wait_list = NULL;
    }
    return 0;
}
/*********************************************************************************************************
** END FILE
*********************************************************************************************************/
