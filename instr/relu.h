#include "../block.h"
#include "util.h"

static inline float
relu (float v)
{
  // this WILL be optimized
  return v > 0 ? v : 0;
}

static inline void
instr_relu (block_t *restrict dst, const block_t *restrict src)
{
  ELEMENT_WISE (dst, src, relu);
}
