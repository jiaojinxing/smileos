/*
 * list.h
 *
 *  Created on: 2013-2-5
 *      Author: jiaojinxing
 */

#ifndef LIST_H_
#define LIST_H_

typedef struct _NODE {
    struct _NODE *prev;
    struct _NODE *next;
} NODE, LIST;

void lstInit(LIST *l);

NODE *lstNext(NODE *n);

NODE *lstFirst(LIST *l);

void lstAdd(LIST *l, NODE *n);

void lstDelete(LIST *l, NODE *n);

#endif /* LIST_H_ */
