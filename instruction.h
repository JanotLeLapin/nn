#ifndef _NN_INSTRUCTION_H
#define _NN_INSTRUCTION_H

#include "block.h"

#include "instr/gemm.h"
#include "instr/relu.h"
#include "instr/transpose.h"

typedef enum
{
  INSTR_TYPE_TRANSPOSE,
  INSTR_TYPE_GEMM,

  INSTR_TYPE_RELU,
} instr_type_t;

typedef struct
{
  instr_type_t t;
  int dst;
  int src;
} instr_t;

static inline void
instr_apply (instr_t instr, block_t *blocks)
{
  block_t *dst = &blocks[instr.dst];
  const block_t *src = &blocks[instr.src];

  switch (instr.t)
    {
    case INSTR_TYPE_TRANSPOSE:
      instr_transpose (dst, src);
      break;
    case INSTR_TYPE_GEMM:
      instr_gemm (dst, src);
      break;
    case INSTR_TYPE_RELU:
      instr_relu (dst, src);
      break;
    }
}

void instr_eval_seq (size_t count, const instr_t *instrs, block_t *blocks);

#endif
