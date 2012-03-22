/*
 * dirent.h
 *
 *  Created on: 2012-3-22
 *      Author: Administrator
 */

#ifndef DIRENT_H_
#define DIRENT_H_

struct dirent {
    ino_t  d_ino;
    char   d_name[NAME_MAX];
};

#endif /* DIRENT_H_ */
