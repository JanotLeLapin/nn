#include "../instruction.h"
#include "../block.h"
#include <string.h>
#include <unity/unity.h>

void
test_instr_transpose ()
{
  block_t b[3];
  float src[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 },
        dst[6] = { 1.0, 4.0, 2.0, 5.0, 3.0, 6.0 };

  b[0] = block_alloc (2, 3);
  b[1] = block_alloc (3, 2);
  b[2] = block_alloc (2, 3);

  memcpy (b[0].data, src, 6 * sizeof (float));

  instr_apply ((instr_t){ .t = INSTR_TYPE_TRANSPOSE, .dst = 1, .src = 0 }, b);
  instr_apply ((instr_t){ .t = INSTR_TYPE_TRANSPOSE, .dst = 2, .src = 1 }, b);

  TEST_ASSERT_FLOAT_ARRAY_WITHIN (0.0, dst, b[1].data, 6);
  TEST_ASSERT_FLOAT_ARRAY_WITHIN (0.0, src, b[2].data, 6);

  block_free (&b[0]);
  block_free (&b[1]);
  block_free (&b[2]);
}
