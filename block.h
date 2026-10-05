#ifndef _NN_BLOCK_H
#define _NN_BLOCK_H

#include <stddef.h>
#include <stdlib.h>

typedef struct
{
  size_t offset;
  unsigned int dims[2];
} block_t;

void block_print (const float *buf, const block_t *block);
void block_randomize (float *buf, const block_t *block, int seed);

int block_load (float *buf, const block_t *block, const char *path);
int block_save (const float *buf, const block_t *block, const char *path);

static inline size_t
block_elem_count (const block_t *b)
{
  return b->dims[0] * b->dims[1];
}

#endif
