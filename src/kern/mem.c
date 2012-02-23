/*
 * mem.c
 *
 *  Created on: 2012-2-23
 *      Author: jiaojinxing
 */

#include "config.h"
#include "types.h"
#include "kern.h"

typedef struct {
  /** index (-> ram[next]) of the next struct */
  uint32_t next;
  /** index (-> ram[prev]) of the previous struct */
  uint32_t prev;
  /** 1: this area is used; 0: this area is unused */
  u8_t used;
} mem_block;

typedef struct {
    /** pointer to the heap (ram_heap): for alignment, ram is now a pointer instead of an array */
    u8_t *base;

    /** the last entry, always unused! */
    mem_block *end;

    /** pointer to the lowest free block, this is used for faster search */
    mem_block *free;


} heap;
