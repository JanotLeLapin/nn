#include "../block.h"
#include <math.h>
#include <stdio.h>

static inline void
instr_softmax (block_t *restrict dst, const block_t *restrict src)
{
  size_t i, len = src->dims[0] * src->dims[1];
  double max = src->data[0], sum = 0.0;

  for (i = 1; i < len; i++)
    {
      if (max < src->data[i])
        {
          max = src->data[i];
        }
    }

  for (i = 0; i < len; i++)
    {
      sum += exp ((double)src->data[i] - max);
    }

  for (i = 0; i < len; i++)
    {
      dst->data[i] = (float)(exp ((double)src->data[i] - max) / sum);
    }
}
