#include "../block.h"
#include "util.h"

static inline float
relu (float v)
{
  // this WILL be optimized
  return v > 0 ? v : 0;
}

static inline void
instr_relu (block_t *dst, const block_t *src)
{
  ELEMENT_WISE (dst, src, relu);
}
