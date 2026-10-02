#include "../block.h"

static inline void
instr_transpose (block_t *dst, const block_t *src)
{
  size_t N = src->dims[0], M = src->dims[1], i, j;

  for (i = 0; i < src->dims[0]; i++)
    {
      for (j = 0; j < src->dims[1]; j++)
        {
          dst->data[j * M + i] = src->data[i * N + j];
        }
    }
}
