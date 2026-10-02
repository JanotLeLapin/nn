#include "../block.h"

/**
 * Let A in R^(M * N),
 * A^T in R^(N * M):
 * (A^T)[i,j] = A[j,i]
 */
static inline void
instr_transpose (block_t *dst, const block_t *src)
{
  size_t M = src->dims[0], N = src->dims[1], i, j;
  float v;

  for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
        {
          v = src->data[i * N + j];
          dst->data[j * M + i] = v;
        }
    }
}
