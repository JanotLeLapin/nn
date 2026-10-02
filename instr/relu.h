#include "../block.h"

static inline float
relu (float v)
{
  // this WILL be optimized
  return v > 0 ? v : 0;
}

static inline void
instr_relu (block_t *dst, const block_t *src)
{
  size_t i;

  for (i = 0; i < src->dims[0] * src->dims[1]; i++)
    {
      dst->data[i] = relu (src->data[i]);
    }
}
