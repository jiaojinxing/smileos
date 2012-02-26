/*
 * uio.h
 *
 *  Created on: 2012-2-26
 *      Author: Administrator
 */

#ifndef UIO_H_
#define UIO_H_

#include <sys/types.h>

/* fake a readv(2)/writev(2) environment */
struct iovec {
    void  *iov_base;  /* memory base address */
    size_t iov_len;   /* memory chunk length */
};

#define UIO_MAXIOV 1024

#endif /* UIO_H_ */
