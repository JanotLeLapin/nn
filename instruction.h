#ifndef _NN_INSTRUCTION_H
#define _NN_INSTRUCTION_H

#include "block.h"

#include "instr/gemm.h"
#include "instr/relu.h"
#include "instr/softmax.h"
#include "instr/transpose.h"

#define INSTR_TYPE(X)                                                         \
  X (TRANSPOSE, transpose)                                                    \
  X (GEMM, gemm)                                                              \
  X (RELU, relu)                                                              \
  X (SOFTMAX, softmax)

typedef enum
{
#define X(variant, func) INSTR_TYPE_##variant,
  INSTR_TYPE (X)
#undef X
} instr_type_t;

typedef struct
{
  instr_type_t t;
  int dst;
  int src;
} instr_t;

static inline void
instr_forward (instr_t instr, block_t *blocks)
{
  block_t *dst = &blocks[instr.dst];
  const block_t *src = &blocks[instr.src];

  switch (instr.t)
    {
#define X(variant, func)                                                      \
  case INSTR_TYPE_##variant:                                                  \
    instr_##func (dst, src);                                                  \
    break;
      INSTR_TYPE (X)
#undef X
    }
}

void instr_forward_seq (size_t count, const instr_t *instrs, block_t *blocks);
void instr_summary (size_t count, const instr_t *instrs);

#endif
