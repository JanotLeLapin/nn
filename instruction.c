#include "instruction.h"
#include <stdio.h>

void
instr_forward_seq (size_t count, const instr_t *instrs, block_t *blocks)
{
  size_t i;
  instr_t instr;

  for (i = 0; i < count; i++)
    {
      instr = instrs[i];
      instr_forward (instr, blocks);
    }
}

void
instr_summary (size_t count, const instr_t *instrs)
{
  size_t i;
  instr_t instr;
  const char *name;

  for (i = 0; i < count; i++)
    {
      instr = instrs[i];
      switch (instr.t)
        {
        case INSTR_TYPE_GEMM:
          name = "gemm";
          break;
        case INSTR_TYPE_TRANSPOSE:
          name = "transpose";
          break;
        case INSTR_TYPE_RELU:
          name = "relu";
          break;
        default:
          continue;
        }

      fprintf (stderr, "%ld: %s: %d->%d\n", i, name, instr.src, instr.dst);
    }
}
