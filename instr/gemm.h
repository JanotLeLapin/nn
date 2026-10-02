#include "../block.h"

static inline void
instr_gemm (block_t *dst, const block_t *a, const block_t *b)
{
  size_t L = a->dims[0], M = a->dims[1], N = b->dims[1], i, j, k;
  float v;

  for (i = 0; i < L; i++)
    {
      for (j = 0; j < N; j++)
        {
          v = 0.0;
          for (k = 0; k < M; k++)
            {
              v += a->data[i * M + k] * b->data[k * M + j];
            }
          dst->data[i * L + j] = v;
        }
    }
}
