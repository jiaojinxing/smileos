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
** Descriptions:            任务间通信机制
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-13
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             JiaoJinXing
** Modified date:           2012-3-25
** Version:                 1.1.0
** Descriptions:            增强 IPC 对象类型安全检查, 修复代码中存在的一处 BUG,
**                          修改恢复任务时不主动释放 CPU
**
*********************************************************************************************************/
#include "kern/config.h"
#include "kern/types.h"
#include "kern/kern.h"
#include "kern/ipc.h"
/*********************************************************************************************************
  操作宏
*********************************************************************************************************/
/*
 * 等待事件
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
 * 等待事件直至超时
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
 * 恢复任务
 */
#define resume_task(task, __wait_list, __resume_type)               \
                    task->timer = 0;                                \
                    task->state = TASK_RUNNING;                     \
                    __wait_list = task->next;                       \
                    task->wait_list = NULL;                         \
                    task->next = NULL;                              \
                    task->resume_type = __resume_type;              \
                    if (!in_interrupt() &&                          \
                        task->type == TASK_TYPE_THREAD &&           \
                        task->priority > current->priority) {       \
                        yield();                                    \
                    }

/*
 * IPC 对象类型放在 IPC 对象的首位, 有效性放在次位, 保证 IPC 对象关键成员变量兼容
 */
typedef enum {
    IPC_TYPE_MUTEX      = 0xABCD1A1A,                                   /*  互斥量                      */
    IPC_TYPE_SEM        = 0xABCD2B2B,                                   /*  信号量                      */
    IPC_TYPE_mqueue     = 0xABCD3C3C,                                   /*  消息队列                    */
    IPC_TYPE_DESTROY    = 0xABCD4D4D,                                   /*  已经销毁                    */
} ipc_type_t;
/*********************************************************************************************************
  互斥量
*********************************************************************************************************/
struct mutex {
    ipc_type_t   type;                                                  /*  IPC 类型                    */
    uint8_t      valid;                                                 /*  有效性                      */
    task_t      *wait_list;                                             /*  等待链表                    */
    task_t      *owner;                                                 /*  拥有者                      */
    uint32_t     lock;                                                  /*  上锁层次                    */
};

/*
 * 创建一个新的互斥量
 */
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

/*
 * 尝试对互斥量进行加锁
 */
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
 * 对互斥量进行加锁
 */
int mutex_lock(mutex_t *mutex, uint32_t timeout)
{
    struct mutex *m;
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
                         * TODO: 假如 timeout != 0, 到这里, 任务已经睡眠了 tick,
                         *       假如任务竞争失败, 则还须睡眠, 应该睡眠 timeout - tick,
                         *       如果 timeout - tick = 0, 则应返回 -1,
                         *       但现在还是睡眠 timeout, 这样不准确, 有待改正
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
 * 对互斥量进行解锁
 */
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
 * 终止等待互斥量
 */
int mutex_abort(mutex_t *mutex)
{
    struct mutex *m;
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
 * 删除互斥量
 */
int mutex_free(mutex_t *mutex)
{
    struct mutex *m;
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
 * 判断互斥量是否有效
 */
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

/*
 * 设置互斥量的有效性
 */
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
  信号量
*********************************************************************************************************/
struct sem {
    ipc_type_t   type;                                                  /*  IPC 类型                    */
    uint8_t      valid;                                                 /*  有效性                      */
    task_t      *wait_list;                                             /*  等待链表                    */
    uint32_t     count;                                                 /*  信号计数                    */
};

/*
 * 创建一个新的信号量
 */
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

/*
 * 尝试获得信号量
 */
int sem_trywait(sem_t *sem)
{
    struct sem *s;
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
 * 获得信号量
 */
int sem_wait(sem_t *sem, uint32_t timeout)
{
    struct sem *s;
    uint32_t reg;
    uint32_t resume_type;

    if (in_interrupt()) {
        return sem_trywait(sem);
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
 * 发送一个信号量
 */
int sem_signal(sem_t *sem)
{
    struct sem *s;
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
 * 终止等待信号量
 */
int sem_abort(sem_t *sem)
{
    struct sem *s;
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
 * 删除信号量
 */
int sem_free(sem_t *sem)
{
    struct sem *s;
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
 * 判断信号量是否有效
 */
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

/*
 * 设置信号量的有效性
 */
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
  消息队列
*********************************************************************************************************/
struct mqueue {
    ipc_type_t   type;                                                  /*  IPC 类型                    */
    uint8_t      valid;                                                 /*  有效性                      */
    task_t      *r_wait_list;                                           /*  读消息队列等待链表          */
    task_t      *w_wait_list;                                           /*  写消息队列等待链表          */
    uint32_t     size;                                                  /*  消息队列可容纳多少封消息    */
    uint32_t     cnt;                                                   /*  消息队列里的未读消息数      */
    uint32_t     in;                                                    /*  入队点                      */
    uint32_t     out;                                                   /*  出队点                      */
    void        *msg[1];                                                /*  消息队列                    */
};

/*
 * 创建一个新的消息队列
 */
int mqueue_new(mqueue_t *mqueue, uint32_t size)
{
    struct mqueue *q;

    if (size < 10) {
        size = 10;
    }

    q = kmalloc(sizeof(struct mqueue) + (size - 1) * sizeof(void *));
    if (q) {
        q->type        = IPC_TYPE_mqueue;
        q->r_wait_list = NULL;
        q->w_wait_list = NULL;
        q->size        = size;
        q->cnt         = 0;
        q->in          = 0;
        q->out         = 0;
        q->valid       = TRUE;

        *mqueue          = q;
        return 0;
    }

    *mqueue = NULL;
    return -1;
}

/*
 * 尝试投递消息到消息队列
 */
int mqueue_trypost(mqueue_t *mqueue, void *msg)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
            if (q->valid) {
                if (q->cnt < q->size) {
                    q->msg[q->in] = msg;
                    q->in++;
                    q->in %= q->size;
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
 * 投递消息到消息队列
 */
int mqueue_post(mqueue_t *mqueue, void *msg, uint32_t timeout)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;
    uint32_t resume_type;

    if (in_interrupt()) {
        return mqueue_trypost(mqueue, msg);
    }

    reg = interrupt_disable();
    if (mqueue) {
        again:
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
            if (q->valid) {
                if (q->cnt < q->size) {
                    q->msg[q->in] = msg;
                    q->in++;
                    q->in %= q->size;
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
 * 尝试从消息队列里取出消息
 */
int mqueue_tryfetch(mqueue_t *mqueue, void **msg)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
            if (q->valid) {
                if (q->cnt) {
                    *msg = q->msg[q->out];
                    q->out++;
                    q->out %= q->size;
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
 * 从消息队列里取出消息
 */
int mqueue_fetch(mqueue_t *mqueue, void **msg, uint32_t timeout)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;
    uint32_t resume_type;

    if (in_interrupt()) {
        return mqueue_tryfetch(mqueue, msg);
    }

    reg = interrupt_disable();
    if (mqueue) {
        again:
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
            if (q->valid) {
                if (q->cnt) {
                    *msg = q->msg[q->out];
                    q->out++;
                    q->out %= q->size;
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
 * 清空消息队列
 */
int mqueue_flush(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;
    int i;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
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
 * 终止等待读取消息
 */
int mqueue_abort_fetch(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
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
 * 终止等待投递消息
 */
int mqueue_abort_post(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
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
 * 终止等待消息队列
 */
int mqueue_abort(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
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
 * 删除消息队列
 */
int mqueue_free(mqueue_t *mqueue)
{
    struct mqueue *q;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
            if (q->valid) {
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
    }
    interrupt_resume(reg);
    return -1;
}

/*
 * 判断消息队列是否有效
 */
int mqueue_valid(mqueue_t *mqueue)
{
    struct mqueue *q;
    uint32_t reg;
    int valid = FALSE;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
            valid = q->valid;
        }
    }
    interrupt_resume(reg);
    return valid;
}

/*
 * 设置消息队列的有效性
 */
int mqueue_set_valid(mqueue_t *mqueue, int valid)
{
    struct mqueue *q;
    uint32_t reg;

    reg = interrupt_disable();
    if (mqueue) {
        q = *mqueue;
        if (q && q->type == IPC_TYPE_mqueue) {
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
