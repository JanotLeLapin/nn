#include "../block.h"
#include <math.h>
#include <stdio.h>

static inline void
instr_softmax (block_t *restrict dst, const block_t *restrict src)
{
  size_t i, len = src->dims[0] * src->dims[1];
  double sum = 0.0;

  for (i = 0; i < len; i++)
    {
      sum += exp ((double)src->data[i]);
    }

  for (i = 0; i < len; i++)
    {
      dst->data[i] = (float)(exp ((double)src->data[i]) / sum);
    }
}
