#include "../block.h"

static inline void
instr_gemm (float *buf, block_t *restrict dst, const block_t *restrict src)
{
  const block_t *a = &src[0], *b = &src[1], *c = &src[2];
  size_t L = a->dims[0], M = a->dims[1], N = b->dims[1], i, j, k;
  float v;

#ifdef INSTR_OMP
#pragma omp parallel for
#endif
  for (i = 0; i < L; i++)
    {
      for (j = 0; j < N; j++)
        {
          v = 0.0;
          for (k = 0; k < M; k++)
            {
              v += buf[a->offset + i * M + k] * buf[b->offset + k * N + j];
            }
          buf[dst->offset + i * N + j] = v + buf[c->offset + i * N + j];
        }
    }
}
