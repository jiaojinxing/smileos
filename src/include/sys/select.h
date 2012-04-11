/*
 * select.h
 *
 *  Created on: 2012-4-11
 *      Author: jiaojinxing
 */

#ifndef SELECT_H_
#define SELECT_H_

#include <sys/time.h>

/*
 * select
 */
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);

#endif /* SELECT_H_ */
