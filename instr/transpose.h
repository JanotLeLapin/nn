#include "../block.h"

/**
 * Let A in R^(M * N),
 * A^T in R^(N * M):
 * (A^T)[i,j] = A[j,i]
 */
static inline void
instr_transpose (float *buf, block_t *restrict dst,
                 const block_t *restrict src)
{
  size_t M = src->dims[0], N = src->dims[1], i, j;
  float v;

  for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
        {
          v = buf[src->offset + i * N + j];
          buf[dst->offset + j * M + i] = v;
        }
    }
}
