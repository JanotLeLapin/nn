#include "../block.h"

#ifndef INSTR_IKJ
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

#else

static inline void
instr_gemm (float *buf, block_t *restrict dst, const block_t *restrict src)
{
  const block_t *a = &src[0], *b = &src[1], *c = &src[2];
  size_t L = a->dims[0], M = a->dims[1], N = b->dims[1], i, j, k;
  float *restrict A = &buf[a->offset], *restrict B = &buf[b->offset],
                  *restrict C = &buf[c->offset],
                  *restrict D = &buf[dst->offset], *d, aik;
  const float *arow, *brow, *crow;

#ifdef INSTR_OMP
#pragma omp parallel for
#endif
  for (i = 0; i < L; i++)
    {
      d = D + i * N;
      crow = C + i * N;
      arow = A + i * M;
      for (j = 0; j < N; j++)
        {
          d[j] = crow[j];
        }

      for (k = 0; k < M; k++)
        {
          aik = arow[k];
          brow = B + k * N;

          for (j = 0; j < N; j++)
            {
              d[j] += aik * brow[j];
            }
        }
    }
}
#endif
