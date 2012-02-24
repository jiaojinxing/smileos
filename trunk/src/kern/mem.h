/*
 * mem.h
 *
 *  Created on: 2012-2-24
 *      Author: Administrator
 */

#ifndef MEM_H_
#define MEM_H_

#include "types.h"

struct _mem_block;
typedef struct _mem_block mem_block;

typedef struct {
    mem_block      *free_list;
    mem_block      *block_list;
    uint8_t        *base;
    uint32_t        size;
} mem_heap;

#endif /* MEM_H_ */
