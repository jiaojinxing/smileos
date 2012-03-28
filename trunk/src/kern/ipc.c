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
** Modified by:             JiaoJinXing
** Modified date:           2012-3-25
** Version:                 1.1.0
** Descriptions:            ��ǿ IPC �������Ͱ�ȫ���, �޸������д��ڵ�һ�� BUG,
**                          �޸Ļָ�����ʱ�������ͷ� CPU
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/ipc.h"
/*********************************************************************************************************
  ������
*********************************************************************************************************/
/*
 * �ȴ��¼�
 */
#define wait_event_forever(__wait_list, __resume_type)              \
                    if (!__wait_list) {                             \
                        __wait_list = current;                      \
                    } else {                                        \
                        task_t *end = __wait_list;                  \
                        while (end->next) {                         \
                            end = end->next;                        \
                        }                                           \
                        end->next = current;                        \
                    }                                               \
                    current->next = NULL;                           \
                                                                    \
                    current->timer = 0;                             \
                    current->state = TASK_SUSPEND;                  \
                    current->wait_list = &__wait_list;              \
                    current->resume_type = TASK_RESUME_UNKNOW;      \
                    yield();                                        \
                    current->wait_list = NULL;                      \
                    current->next = NULL;                           \
                    __resume_type = current->resume_type;           \
                    current->resume_type = TASK_RESUME_UNKNOW

/*
 * �ȴ��¼�ֱ����ʱ
 */
#define wait_event_timeout(__wait_list, __resume_type, timeout)     \
                    if (!__wait_list) {                             \
                        __wait_list = current;                      \
                    } else {                                        \
                        task_t *end = __wait_list;                  \
                        while (end->next) {                         \
                            end = end->next;                        \
                        }                                           \
                        end->next = current;                        \
                    }                                               \
                    current->next = NULL;                           \
                                                                    \
                    current->timer = timeout;                       \
                    current->state = TASK_SLEEPING;                 \
                    current->wait_list = &__wait_list;              \
                    current->resume_type = TASK_RESUME_UNKNOW;      \
                    yield();                                        \
                    current->wait_list = NULL;                      \
                    current->next = NULL;                           \
                    current->timer = 0;                             \
                    __resume_type = current->resume_type;           \
                    current->resume_type = TASK_RESUME_UNKNOW

/*
 * �ָ�����
 * ��֤���жϻ��ѵ�����(һ�����жϵװ벿����), ���ж���ȫ�˳�����������
 * ̧�������ʣ��ʱ��Ƭ, ���������ʱ, ���нϸߵľ������ȼ�
 */
#define resume_task(task, __wait_list, __resume_type)               \
                    task->timer = 0;                                \
                    task->state = TASK_RUNNING;                     \
                    __wait_list = task->next;                       \
                    task->wait_list = NULL;                         \
                    task->next = NULL;                              \
                    task->resume_type = __resume_type;              \
                    if (in_interrupt()) {                           \
                        task->counter = task->priority + 2;         \
                    }

/*
 * IPC �������ͷ��� IPC �������λ, ��Ч�Է��ڴ�λ, ��֤ IPC ����ؼ���Ա��������
 */
typedef enum {
    IPC_TYPE_MUTEX      = 0xABCD1A1A,                                   /*  ������                      */
    IPC_TYPE_SEM        = 0xABCD2B2B,                                   /*  �ź���                      */
    IPC_TYPE_MBOX       = 0xABCD3C3C,                                   /*  ����                        */
    IPC_TYPE_DESTROY    = 0xABCD4D4D,                                   /*  �Ѿ�����                    */
} ipc_type_t;
/*********************************************************************************************************
  ������
*********************************************************************************************************/
struct kern_mutex {
    ipc_type_t   type;                                                  /*  IPC ����                    */
    uint8_t      valid;                                                 /*  ��Ч��                      */
    task_t      *wait_list;                                             /*  �ȴ�����                    */
    task_t      *owner;                                                 /*  ӵ����                      */
    uint32_t     lock;                                                  /*  �������                    */
};

/*
 * ����һ���µĻ�����
 */
int kern_mutex_new(kern_mutex_t *mutex)
{
    struct kern_mutex *m;

    m = kmalloc(sizeof(struct kern_mutex));
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

/*
 * ���ԶԻ��������м���
 */
int kern_mutex_trylock(kern_mutex_t *mutex)
{
    struct kern_mutex *m;
    uint32_t reg;

    if (in_interrupt()) {
        return -1;
    }

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX) {
            if (m->valid) {
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
    }

    error:
    interrupt_resume(reg);
    return -1;
}

/*
 * �Ի��������м���
 */
int kern_mutex_lock(kern_mutex_t *mutex, uint32_t timeout)
{
    struct kern_mutex *m;
    uint32_t reg;
    uint32_t resume_type;

    if (in_interrupt()) {
        return -1;
    }

    reg = interrupt_disable();
    if (mutex) {
        again:
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX) {
            if (m->valid) {
                if (!m->lock) {
                    m->lock++;
                    m->owner = current;
                } else if (m->owner == current) {
                    m->lock++;
                } else {
                    if (timeout != 0) {
                        wait_event_timeout(m->wait_list, resume_type, timeout);
                    } else {
                        wait_event_forever(m->wait_list, resume_type);
                    }
                    if (resume_type & TASK_RESUME_INTERRUPT || resume_type & TASK_RESUME_TIMEOUT) {
                        goto error;
                    } else {
                        /*
                         * TODO: ���� timeout != 0, ������, �����Ѿ�˯���� tick,
                         *       ����������ʧ��, ����˯��, Ӧ��˯�� timeout - tick,
                         *       ��� timeout - tick = 0, ��Ӧ���� -1,
                         *       �����ڻ���˯�� timeout, ������׼ȷ, �д�����
                         */
                        goto again;
                    }
                }
                interrupt_resume(reg);
                return 0;
            }
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}

/*
 * �Ի��������н���
 */
int kern_mutex_unlock(kern_mutex_t *mutex)
{
    struct kern_mutex *m;
    task_t *task;
    uint32_t reg;

    if (in_interrupt()) {
        return -1;
    }

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX) {
            if (m->valid) {
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
    }
    interrupt_resume(reg);
    return -1;
}

/*
 * ��ֹ�ȴ�������
 */
int kern_mutex_abort(kern_mutex_t *mutex)
{
    struct kern_mutex *m;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX) {
            if (m->valid) {
                while ((task = m->wait_list) != NULL) {
                    resume_task(task, m->wait_list, TASK_RESUME_INTERRUPT);
                }
                interrupt_resume(reg);
                return 0;
            }
        }
    }
    interrupt_resume(reg);
    return -1;
}

/*
 * ɾ��������
 */
int kern_mutex_free(kern_mutex_t *mutex)
{
    struct kern_mutex *m;
    uint32_t reg;

    reg = interrupt_disable();
    if (mutex) {
        m = *mutex;
        if (m && m->type == IPC_TYPE_MUTEX) {
            if (m->valid) {
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
    }
    interrupt_resume(reg);
    return -1;
}

/*
 * �жϻ������Ƿ���Ч
 */
int kern_mutex_valid(kern_mutex_t *mutex)
{
    struct kern_mutex *m;
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

/*
 * ���û���������Ч��
 */
int kern_mutex_set_valid(kern_mutex_t *mutex, int valid)
{
    struct kern_mutex *m;
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
  �ź���
*********************************************************************************************************/
struct kern_sem {
    ipc_type_t   type;                                                  /*  IPC ����                    */
    uint8_t      valid;                                                 /*  ��Ч��                      */
    task_t      *wait_list;                                             /*  �ȴ�����                    */
    uint32_t     count;                                                 /*  �źż���                    */
};

/*
 * ����һ���µ��ź���
 */
int kern_sem_new(kern_sem_t *sem, uint32_t count)
{
    struct kern_sem *s;

    s = kmalloc(sizeof(struct kern_sem));
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

/*
 * ���Ի���ź���
 */
int kern_sem_trywait(kern_sem_t *sem)
{
    struct kern_sem *s;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM) {
            if (s->valid) {
                if (s->count) {
                    s->count--;
                    interrupt_resume(reg);
                    return 0;
                } else {
                    goto error;
                }
            }
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}

/*
 * ����ź���
 */
int kern_sem_wait(kern_sem_t *sem, uint32_t timeout)
{
    struct kern_sem *s;
    uint32_t reg;
    uint32_t resume_type;

    if (in_interrupt()) {
        return kern_sem_trywait(sem);
    }

    reg = interrupt_disable();
    if (sem) {
        again:
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM) {
            if (s->valid) {
                if (s->count) {
                    s->count--;
                    interrupt_resume(reg);
                    return 0;
                } else {
                    if (timeout != 0) {
                        wait_event_timeout(s->wait_list, resume_type, timeout);
                    } else {
                        wait_event_forever(s->wait_list, resume_type);
                    }
                    if (resume_type & TASK_RESUME_INTERRUPT || resume_type & TASK_RESUME_TIMEOUT) {
                        goto error;
                    } else {
                        goto again;
                    }
                }
            }
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}

/*
 * ����һ���ź���
 */
int kern_sem_signal(kern_sem_t *sem)
{
    struct kern_sem *s;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM) {
            if (s->valid) {
                s->count++;
                task = s->wait_list;
                if (task) {
                    resume_task(task, s->wait_list, TASK_RESUME_SEM_COME);
                }
                interrupt_resume(reg);
                return 0;
            }
        }
    }
    interrupt_resume(reg);
    return -1;
}

/*
 * ��ֹ�ȴ��ź���
 */
int kern_sem_abort(kern_sem_t *sem)
{
    struct kern_sem *s;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM) {
            if (s->valid) {
                while ((task = s->wait_list) != NULL) {
                    resume_task(task, s->wait_list, TASK_RESUME_INTERRUPT);
                }
                interrupt_resume(reg);
                return 0;
            }
        }
    }
    interrupt_resume(reg);
    return -1;
}

/*
 * ɾ���ź���
 */
int kern_sem_free(kern_sem_t *sem)
{
    struct kern_sem *s;
    uint32_t reg;

    reg = interrupt_disable();
    if (sem) {
        s = *sem;
        if (s && s->type == IPC_TYPE_SEM) {
            if (s->valid) {
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
    }
    interrupt_resume(reg);
    return -1;
}

/*
 * �ж��ź����Ƿ���Ч
 */
int kern_sem_valid(kern_sem_t *sem)
{
    struct kern_sem *s;
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

/*
 * �����ź�������Ч��
 */
int kern_sem_set_valid(kern_sem_t *sem, int valid)
{
    struct kern_sem *s;
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
  ����
*********************************************************************************************************/
struct kern_mbox {
    ipc_type_t   type;                                                  /*  IPC ����                    */
    uint8_t      valid;                                                 /*  ��Ч��                      */
    task_t      *r_wait_list;                                           /*  ������ȴ�����              */
    task_t      *w_wait_list;                                           /*  д����ȴ�����              */
    uint32_t     size;                                                  /*  ��������ɶ��ٷ��ʼ�        */
    uint32_t     cnt;                                                   /*  �������δ���ʼ���          */
    uint32_t     in;                                                    /*  ��ӵ�                      */
    uint32_t     out;                                                   /*  ���ӵ�                      */
    void        *msg[1];                                                /*  �ʼ�����                    */
};

/*
 * ����һ���µ�����
 */
int kern_mbox_new(kern_mbox_t *mbox, uint32_t size)
{
    struct kern_mbox *q;

    if (size < 10) {
        size = 10;
    }

    q = kmalloc(sizeof(struct kern_mbox) + (size - 1) * sizeof(void *));
    if (q) {
        q->type        = IPC_TYPE_MBOX;
        q->r_wait_list = NULL;
        q->w_wait_list = NULL;
        q->size        = size;
        q->cnt         = 0;
        q->in          = 0;
        q->out         = 0;
        q->valid       = TRUE;

        *mbox          = q;
        return 0;
    }

    *mbox = NULL;
    return -1;
}

/*
 * ����Ͷ���ʼ�������
 */
int kern_mbox_trypost(kern_mbox_t *mbox, void *msg)
{
    struct kern_mbox *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            if (q->valid) {
                if (q->cnt < q->size) {
                    q->msg[q->in] = msg;
                    q->in = ++q->in % q->size;
                    q->cnt++;

                    task = q->r_wait_list;
                    if (task) {
                        resume_task(task, q->r_wait_list, TASK_RESUME_MSG_COME);
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

/*
 * Ͷ���ʼ�������
 */
int kern_mbox_post(kern_mbox_t *mbox, void *msg, uint32_t timeout)
{
    struct kern_mbox *q;
    task_t *task;
    uint32_t reg;
    uint32_t resume_type;

    if (in_interrupt()) {
        return kern_mbox_trypost(mbox, msg);
    }

    reg = interrupt_disable();
    if (mbox) {
        again:
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            if (q->valid) {
                if (q->cnt < q->size) {
                    q->msg[q->in] = msg;
                    q->in = ++q->in % q->size;
                    q->cnt++;

                    task = q->r_wait_list;
                    if (task) {
                        resume_task(task, q->r_wait_list, TASK_RESUME_MSG_COME);
                    }
                    interrupt_resume(reg);
                    return 0;
                } else {
                    if (timeout != 0) {
                        wait_event_timeout(q->w_wait_list, resume_type, timeout);
                    } else {
                        wait_event_forever(q->w_wait_list, resume_type);
                    }
                    if (resume_type & TASK_RESUME_INTERRUPT || resume_type & TASK_RESUME_TIMEOUT) {
                        goto error;
                    } else {
                        goto again;
                    }
                }
            }
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}

/*
 * ���Դ�������ȡ���ʼ�
 */
int kern_mbox_tryfetch(kern_mbox_t *mbox, void **msg)
{
    struct kern_mbox *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            if (q->valid) {
                if (q->cnt) {
                    *msg = q->msg[q->out];
                    q->out = ++q->out % q->size;
                    q->cnt--;

                    task = q->w_wait_list;
                    if (task) {
                        resume_task(task, q->w_wait_list, TASK_RESUME_MSG_OUT);
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

/*
 * ��������ȡ���ʼ�
 */
int kern_mbox_fetch(kern_mbox_t *mbox, void **msg, uint32_t timeout)
{
    struct kern_mbox *q;
    task_t *task;
    uint32_t reg;
    uint32_t resume_type;

    if (in_interrupt()) {
        return kern_mbox_tryfetch(mbox, msg);
    }

    reg = interrupt_disable();
    if (mbox) {
        again:
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            if (q->valid) {
                if (q->cnt) {
                    *msg = q->msg[q->out];
                    q->out = ++q->out % q->size;
                    q->cnt--;

                    task = q->w_wait_list;
                    if (task) {
                        resume_task(task, q->w_wait_list, TASK_RESUME_MSG_OUT);
                    }
                    interrupt_resume(reg);
                    return 0;
                } else {
                    if (timeout != 0) {
                        wait_event_timeout(q->r_wait_list, resume_type, timeout);
                    } else {
                        wait_event_forever(q->r_wait_list, resume_type);
                    }
                    if (resume_type & TASK_RESUME_INTERRUPT || resume_type & TASK_RESUME_TIMEOUT) {
                        goto error;
                    } else {
                        goto again;
                    }
                }
            }
        }
    }

    error:
    interrupt_resume(reg);
    return -1;
}

/*
 * �������
 */
int kern_mbox_flush(kern_mbox_t *mbox)
{
    struct kern_mbox *q;
    task_t *task;
    uint32_t reg;
    int i;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            if (q->valid) {
                q->cnt  = 0;
                q->in   = 0;
                q->out  = 0;

                for (i = 0; ((task = q->w_wait_list) != NULL) && i < q->size; i++) {
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

/*
 * ��ֹ�ȴ���ȡ�ʼ�
 */
int kern_mbox_abort_fetch(kern_mbox_t *mbox)
{
    struct kern_mbox *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            if (q->valid) {
                while ((task = q->r_wait_list) != NULL) {
                    resume_task(task, q->r_wait_list, TASK_RESUME_INTERRUPT);
                }
                interrupt_resume(reg);
                return 0;
            }
        }
    }

    interrupt_resume(reg);
    return -1;
}

/*
 * ��ֹ�ȴ�Ͷ���ʼ�
 */
int kern_mbox_abort_post(kern_mbox_t *mbox)
{
    struct kern_mbox *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            if (q->valid) {
                while ((task = q->w_wait_list) != NULL) {
                    resume_task(task, q->w_wait_list, TASK_RESUME_INTERRUPT);
                }
                interrupt_resume(reg);
                return 0;
            }
        }
    }

    interrupt_resume(reg);
    return -1;
}

/*
 * ��ֹ�ȴ�����
 */
int kern_mbox_abort(kern_mbox_t *mbox)
{
    struct kern_mbox *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            if (q->valid) {
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
    }

    interrupt_resume(reg);
    return -1;
}

/*
 * ɾ������
 */
int kern_mbox_free(kern_mbox_t *mbox)
{
    struct kern_mbox *q;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            if (q->valid) {
                if (!q->r_wait_list && !q->w_wait_list) {
                    q->valid = FALSE;
                    q->type = IPC_TYPE_DESTROY;
                    kfree(q);
                    *mbox = NULL;
                    interrupt_resume(reg);
                    return 0;
                }
            }
        }
    }
    interrupt_resume(reg);
    return -1;
}

/*
 * �ж������Ƿ���Ч
 */
int kern_mbox_valid(kern_mbox_t *mbox)
{
    struct kern_mbox *q;
    uint32_t reg;
    int valid = FALSE;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            valid = q->valid;
        }
    }
    interrupt_resume(reg);
    return valid;
}

/*
 * �����������Ч��
 */
int kern_mbox_set_valid(kern_mbox_t *mbox, int valid)
{
    struct kern_mbox *q;
    uint32_t reg;

    reg = interrupt_disable();
    if (mbox) {
        q = *mbox;
        if (q && q->type == IPC_TYPE_MBOX) {
            q->valid = valid > 0 ? TRUE : FALSE;
            interrupt_resume(reg);
            return 0;
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/