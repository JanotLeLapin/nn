#ifndef _NN_INSTRUCTION_H
#define _NN_INSTRUCTION_H

#include "block.h"

#include "instr/transpose.h"

typedef enum
{
  INSTR_TYPE_TRANSPOSE,
  INSTR_TYPE_GEMM,
} instr_type_t;

typedef struct
{
  instr_type_t t;
  int dst;
  int src;
} instr_t;

static inline void
instr_apply (instr_type_t instr, block_t *dst, const block_t *src)
{
  switch (instr)
    {
    case INSTR_TYPE_TRANSPOSE:
      instr_transpose (dst, src);
      break;
    case INSTR_TYPE_GEMM:
      // TODO: can't call gemm quite yet
      break;
    }
}

void instr_eval_seq (size_t count, const instr_t *instrs, block_t *blocks);

#endif
