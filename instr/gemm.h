#include "../block.h"

static inline void
instr_gemm (block_t *dst, const block_t *src)
{
  const block_t *a = &src[0], *b = &src[1], *c = &src[2];
  size_t L = a->dims[0], M = a->dims[1], N = b->dims[1], i, j, k;
  float v;

  for (i = 0; i < L; i++)
    {
      for (j = 0; j < N; j++)
        {
          v = 0.0;
          for (k = 0; k < M; k++)
            {
              v += a->data[i * M + k] * b->data[k * N + j];
            }
          dst->data[i * N + j] = v + c->data[i * N + j];
        }
    }
}
