#include "../instruction.h"
#include "../block.h"
#include <string.h>
#include <unity/unity.h>

void
test_instr_transpose ()
{
  instr_t instrs[2]
      = { (instr_t){ .t = INSTR_TYPE_TRANSPOSE, .dst = 1, .src = 0 },
          (instr_t){ .t = INSTR_TYPE_TRANSPOSE, .dst = 2, .src = 1 } };
  block_t b[3];
  float src[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 },
        dst[6] = { 1.0, 4.0, 2.0, 5.0, 3.0, 6.0 };

  b[0] = block_alloc (2, 3);
  b[1] = block_alloc (3, 2);
  b[2] = block_alloc (2, 3);

  memcpy (b[0].data, src, 6 * sizeof (float));

  instr_eval_seq (2, instrs, b);

  TEST_ASSERT_FLOAT_ARRAY_WITHIN (0.0, dst, b[1].data, 6);
  TEST_ASSERT_FLOAT_ARRAY_WITHIN (0.0, src, b[2].data, 6);

  block_free (&b[0]);
  block_free (&b[1]);
  block_free (&b[2]);
}

void
test_instr_gemm ()
{
  instr_t instrs[1]
      = { (instr_t){ .t = INSTR_TYPE_GEMM, .dst = 3, .src = 0 } };
  block_t b[4];

  float ad[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 },
        bd[6] = { 7.0, 8.0, 9.0, 10.0, 11.0, 12.0 },
        cd[6] = { 0.5, 0.0, 0.5, 0.0 }, dd[6] = { 58.5, 64, 139.5, 154 };

  b[0] = block_alloc (2, 3);
  b[1] = block_alloc (3, 2);
  b[2] = block_alloc (2, 2);
  b[3] = block_alloc (2, 2);

  memcpy (b[0].data, ad, 6 * sizeof (float));
  memcpy (b[1].data, bd, 6 * sizeof (float));
  memcpy (b[2].data, cd, 4 * sizeof (float));

  instr_eval_seq (1, instrs, b);

  TEST_ASSERT_FLOAT_ARRAY_WITHIN (0.0, dd, b[3].data, 4);

  block_free (&b[0]);
  block_free (&b[1]);
  block_free (&b[2]);
  block_free (&b[3]);
}
