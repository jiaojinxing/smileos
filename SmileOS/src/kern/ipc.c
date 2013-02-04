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
** Descriptions:            任务间通信机制
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-3-13
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
#include "kern/kern.h"
#include "kern/ipc.h"
#include "kern/kvars.h"
/*********************************************************************************************************
** 配置
*********************************************************************************************************/
#define MQUEUE_MIN_SIZE             (10)                                /*  消息队列的最小容量          */
#define MQUEUE_MAX_SIZE             (10000)                             /*  消息队列的最大容量          */
/*********************************************************************************************************
** 操作宏
*********************************************************************************************************/
/*
 * 等待事件
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
 * 等待事件直至超时
 */
#define wait_event_timeout(__wait_list, __resume_type, __timeout, __tick)   \
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
                    __tick = getticks();                            \
                    schedule();                                     \
                    __tick = getticks() - __tick;                   \
                    current->wait_list = NULL;                      \
                    current->next = NULL;                           \
                    current->delay = 0;                             \
                    __resume_type = current->resume_type;           \
                    current->resume_type = TASK_RESUME_UNKNOW

/*
 * 恢复任务
 */
#define resume_task(__task, __wait_list, __resume_type)             \
                    __task->delay = 0;                              \
                    __task->status = TASK_RUNNING;                  \
                    __wait_list = __task->next;                     \
                    __task->wait_list = NULL;                       \
                    __task->next = NULL;                            \
                    __task->resume_type = __resume_type;            \
                    if (!in_interrupt()) {                          \
                        schedule();                                 \
                    }
/*********************************************************************************************************
** IPC 对象类型放在 IPC 对象的首位, 有效性放在次位, 保证 IPC 对象关键成员变量兼容
*********************************************************************************************************/
typedef enum {
    IPC_TYPE_MUTEX   = 0xABCD1A1A,                                      /*  互斥量                      */
    IPC_TYPE_SEM     = 0xABCD2B2B,                                      /*  信号量                      */
    IPC_TYPE_MQUEUE  = 0xABCD3C3C,                                      /*  消息队列                    */
    IPC_TYPE_DESTROY = 0xABCD4D4D,                                      /*  已经销毁                    */
} ipc_type_t;
/*********************************************************************************************************
** 互斥量
*********************************************************************************************************/
struct mutex {
    ipc_type_t  type;                                                   /*  IPC 类型                    */
    bool_t      valid;                                                  /*  有效性                      */
    uint32_t    lock;                                                   /*  上锁层次                    */
    task_t     *wait_list;                                              /*  等待链表                    */
    task_t     *owner;                                                  /*  拥有者                      */
};
/*********************************************************************************************************
** Function name:           mutex_new
** Descriptions:            创建一个新的互斥量
** input parameters:        NONE
** output parameters:       mutex               互斥量
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_new(mutex_t *mutex)
{
    struct mutex *m;

    m = kmalloc(sizeof(struct mutex), GFP_KERNEL);
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
** Descriptions:            尝试对互斥量进行加锁
** input parameters:        mutex               互斥量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_trylock(mutex_t *mutex)
{
    struct mutex *m;
    reg_t reg;

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
** Descriptions:            对互斥量进行加锁
** input parameters:        mutex               互斥量
**                          timeout             超时 TICK 数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_lock(mutex_t *mutex, tick_t timeout)
{
    struct mutex *m;
    reg_t reg;
    uint8_t resume_type;
    tick_t tick;

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
                printk(KERN_DEBUG"%s: %s lock mutex, %s wait it\n", __func__, m->owner->name, current->name);
                if (timeout != 0) {
                    wait_event_timeout(m->wait_list, resume_type, timeout, tick);
                } else {
                    wait_event_forever(m->wait_list, resume_type);
                }
                if (resume_type == TASK_RESUME_INTERRUPT || resume_type == TASK_RESUME_TIMEOUT) {
                    goto error;
                } else {
                    if (timeout != 0) {
                        if (timeout <= tick) {
                            goto error;
                        }
                        timeout -= tick;
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
** Descriptions:            对互斥量进行解锁
** input parameters:        mutex               互斥量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_unlock(mutex_t *mutex)
{
    struct mutex *m;
    task_t *task;
    reg_t reg;

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
                        m->owner = NULL;
                        task = m->wait_list;
                        if (task) {
                            resume_task(task, m->wait_list, TASK_RESUME_MUTEX_COME);
                        }
                    }
                    interrupt_resume(reg);
                    return 0;
                } else {

                }
            } else {

            }
        }
    }
    interrupt_resume(reg);
    return -1;
}
/*********************************************************************************************************
** Function name:           mutex_abort
** Descriptions:            终止等待互斥量
** input parameters:        mutex               互斥量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_abort(mutex_t *mutex)
{
    struct mutex *m;
    task_t *task;
    reg_t reg;

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
** Descriptions:            删除互斥量
** input parameters:        mutex               互斥量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_free(mutex_t *mutex)
{
    struct mutex *m;
    reg_t reg;

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
** Descriptions:            判断互斥量是否有效
** input parameters:        mutex               互斥量
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
bool_t mutex_valid(mutex_t *mutex)
{
    struct mutex *m;
    reg_t reg;
    bool_t valid = FALSE;

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
** Descriptions:            设置互斥量的有效性
** input parameters:        mutex               互斥量
**                          valid               有效性
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mutex_set_valid(mutex_t *mutex, bool_t valid)
{
    struct mutex *m;
    reg_t reg;

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
** 信号量
*********************************************************************************************************/
struct sem {
    ipc_type_t  type;                                                   /*  IPC 类型                    */
    bool_t      valid;                                                  /*  有效性                      */
    uint32_t    count;                                                  /*  信号计数                    */
    task_t     *wait_list;                                              /*  等待链表                    */
};
/*********************************************************************************************************
** Function name:           sem_new
** Descriptions:            创建一个新的信号量
** input parameters:        count               信号初始计数
** output parameters:       sem                 信号量
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_new(sem_t *sem, uint32_t count)
{
    struct sem *s;

    s = kmalloc(sizeof(struct sem), GFP_KERNEL);
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
** Descriptions:            尝试获得信号量
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_trywait(sem_t *sem)
{
    struct sem *s;
    reg_t reg;

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
** Descriptions:            获得信号量
** input parameters:        sem                 信号量
**                          timeout             超时 TICK 数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_wait(sem_t *sem, tick_t timeout)
{
    struct sem *s;
    reg_t reg;
    uint8_t resume_type;
    tick_t tick;

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
                    wait_event_timeout(s->wait_list, resume_type, timeout, tick);
                } else {
                    wait_event_forever(s->wait_list, resume_type);
                }
                if (resume_type == TASK_RESUME_INTERRUPT || resume_type == TASK_RESUME_TIMEOUT) {
                    goto error;
                } else {
                    if (timeout != 0) {
                        if (timeout <= tick) {
                            goto error;
                        }
                        timeout -= tick;
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
** Descriptions:            发送一个信号量
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_signal(sem_t *sem)
{
    struct sem *s;
    task_t *task;
    reg_t reg;

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
** Descriptions:            如果有任务在等待, 则发送一个信号量
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_sync(sem_t *sem)
{
    struct sem *s;
    task_t *task;
    reg_t reg;

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
** Descriptions:            终止等待信号量
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_abort(sem_t *sem)
{
    struct sem *s;
    task_t *task;
    reg_t reg;

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
** Descriptions:            删除信号量
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_free(sem_t *sem)
{
    struct sem *s;
    reg_t reg;

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
** Descriptions:            判断信号量是否有效
** input parameters:        sem                 信号量
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
bool_t sem_valid(sem_t *sem)
{
    struct sem *s;
    reg_t reg;
    bool_t valid = FALSE;

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
** Descriptions:            设置信号量的有效性
** input parameters:        sem                 信号量
**                          valid               有效性
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int sem_set_valid(sem_t *sem, bool_t valid)
{
    struct sem *s;
    reg_t reg;

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
** 消息队列
*********************************************************************************************************/
struct mqueue {
    ipc_type_t  type;                                                   /*  IPC 类型                    */
    bool_t      valid;                                                  /*  有效性                      */
    task_t     *r_wait_list;                                            /*  读消息队列等待链表          */
    task_t     *w_wait_list;                                            /*  写消息队列等待链表          */
    size_t      size;                                                   /*  消息队列的大小              */
    size_t      nr;                                                     /*  消息队列里的未读消息数      */
    size_t      in;                                                     /*  入队点                      */
    size_t      out;                                                    /*  出队点                      */
    void       *msg[1];                                                 /*  消息队列                    */
};
/*********************************************************************************************************
** Function name:           mqueue_new
** Descriptions:            创建一个新的消息队列
** input parameters:        size                消息队列的大小
** output parameters:       mqueue              消息队列
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_new(mqueue_t *mqueue, size_t size)
{
    struct mqueue *q;

    if (size < MQUEUE_MIN_SIZE) {
        size = MQUEUE_MIN_SIZE;
    }

    if (size > MQUEUE_MAX_SIZE) {
        size = MQUEUE_MAX_SIZE;
    }

    q = kmalloc(sizeof(struct mqueue) + (size - 1) * sizeof(void *), GFP_KERNEL);
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
** Descriptions:            尝试投递消息到消息队列
** input parameters:        mqueue              消息队列
**                          msg                 消息
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_trypost(mqueue_t *mqueue, void *msg)
{
    struct mqueue *q;
    task_t *task;
    reg_t reg;

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
** Descriptions:            投递消息到消息队列
** input parameters:        mqueue              消息队列
**                          msg                 消息
**                          timeout             超时 TICK 数
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_post(mqueue_t *mqueue, void *msg, tick_t timeout)
{
    struct mqueue *q;
    task_t *task;
    reg_t reg;
    uint8_t resume_type;
    tick_t tick;

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
                    wait_event_timeout(q->w_wait_list, resume_type, timeout, tick);
                } else {
                    wait_event_forever(q->w_wait_list, resume_type);
                }
                if (resume_type == TASK_RESUME_INTERRUPT || resume_type == TASK_RESUME_TIMEOUT) {
                    goto error;
                } else {
                    if (timeout != 0) {
                        if (timeout <= tick) {
                            goto error;
                        }
                        timeout -= tick;
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
** Descriptions:            尝试从消息队列里取出消息
** input parameters:        mqueue              消息队列
** output parameters:       msg                 消息
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_tryfetch(mqueue_t *mqueue, void **msg)
{
    struct mqueue *q;
    task_t *task;
    reg_t reg;

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
** Descriptions:            从消息队列里取出消息
** input parameters:        mqueue              消息队列
**                          timeout             超时 TICK 数
** output parameters:       msg                 消息
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_fetch(mqueue_t *mqueue, void **msg, tick_t timeout)
{
    struct mqueue *q;
    task_t *task;
    reg_t reg;
    uint8_t resume_type;
    tick_t tick;

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
                    wait_event_timeout(q->r_wait_list, resume_type, timeout, tick);
                } else {
                    wait_event_forever(q->r_wait_list, resume_type);
                }
                if (resume_type == TASK_RESUME_INTERRUPT || resume_type == TASK_RESUME_TIMEOUT) {
                    goto error;
                } else {
                    if (timeout != 0) {
                        if (timeout <= tick) {
                            goto error;
                        }
                        timeout -= tick;
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
** Descriptions:            清空消息队列
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_flush(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    reg_t reg;
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
** Descriptions:            返回消息队列中消息的数目
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          消息的数目 OR -1
*********************************************************************************************************/
int mqueue_msg_nr(mqueue_t *mqueue)
{
    struct mqueue *q;
    reg_t reg;
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
** Descriptions:            终止等待读取消息
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort_fetch(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    reg_t reg;

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
** Descriptions:            终止等待投递消息
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort_post(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    reg_t reg;

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
** Descriptions:            终止等待消息队列
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_abort(mqueue_t *mqueue)
{
    struct mqueue *q;
    task_t *task;
    reg_t reg;

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
** Descriptions:            删除消息队列
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_free(mqueue_t *mqueue)
{
    struct mqueue *q;
    reg_t reg;

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
** Descriptions:            判断消息队列是否有效
** input parameters:        mqueue              消息队列
** output parameters:       NONE
** Returned value:          TRUE OR FALSE
*********************************************************************************************************/
bool_t mqueue_valid(mqueue_t *mqueue)
{
    struct mqueue *q;
    reg_t reg;
    bool_t valid = FALSE;

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
** Descriptions:            设置消息队列的有效性
** input parameters:        mqueue              消息队列
**                          valid               有效性
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int mqueue_set_valid(mqueue_t *mqueue, bool_t valid)
{
    struct mqueue *q;
    reg_t reg;

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
** Descriptions:            初始化任务的 IPC
** input parameters:        task                任务
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
** Descriptions:            清理任务的 IPC
** input parameters:        task                任务
** output parameters:       NONE
** Returned value:          0 OR -1
*********************************************************************************************************/
int ipc_task_cleanup(task_t *task)
{
    if (task->wait_list != NULL) {                                      /*  如果任务在某个等待链表      */
        task_t *prev = *task->wait_list;                                /*  获得等待链表的链头任务      */
        if (task == prev) {                                             /*  如果任务就是链头任务        */
            *task->wait_list = task->next;                              /*  修改等待链表的链头          */
        } else {
            while (prev != NULL && prev->next != task) {                /*  找出任务在等待链表中的前趋  */
                prev = prev->next;
            }
            if (prev != NULL) {                                         /*  如果有找到                  */
                prev->next = task->next;                                /*  修改前趋的后趋              */
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
