#ifndef _NN_BLOCK_H
#define _NN_BLOCK_H

#include <stddef.h>
#include <stdlib.h>

typedef struct
{
  float *data;
  unsigned int dims[2];
} block_t;

block_t block_alloc (unsigned int width, unsigned int height);
void block_free (block_t *block);
void block_print (const block_t *block);
void block_randomize (block_t *block, int seed);

static inline size_t
block_elem_count (const block_t *b)
{
  return b->dims[0] * b->dims[1];
}

#endif
