#include "instruction.h"

void
instr_eval_seq (size_t count, const instr_t *instrs, block_t *blocks)
{
  size_t i;
  instr_t instr;

  for (i = 0; i < count; i++)
    {
      instr = instrs[i];
      instr_apply (instr, blocks);
    }
}
