#ifndef _NN_INSTRUCTION_H
#define _NN_INSTRUCTION_H

#include "block.h"

#include "instr/transpose.h"

typedef enum
{
  INSTR_TYPE_TRANSPOSE,
} instr_type_t;

static inline void
instr_apply (instr_type_t instr, block_t *dst, const block_t *src)
{
  switch (instr)
    {
    case INSTR_TYPE_TRANSPOSE:
      instr_transpose (dst, src);
      break;
    }
}

#endif
