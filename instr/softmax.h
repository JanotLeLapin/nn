#include "../block.h"
#include <math.h>
#include <stdio.h>

static inline void
instr_softmax (float *buf, block_t *restrict dst, const block_t *restrict src)
{
  size_t i, len = src->dims[0] * src->dims[1];
  double max = buf[src->offset], sum = 0.0;

  for (i = 1; i < len; i++)
    {
      if (max < buf[src->offset + i])
        {
          max = buf[src->offset + i];
        }
    }

  for (i = 0; i < len; i++)
    {
      sum += exp ((double)buf[src->offset + i] - max);
    }

  for (i = 0; i < len; i++)
    {
      buf[dst->offset + i]
          = (float)(exp ((double)buf[src->offset + i] - max) / sum);
    }
}
