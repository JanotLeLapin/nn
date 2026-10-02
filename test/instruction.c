#include "../instruction.h"
#include "../block.h"
#include <string.h>
#include <unity/unity.h>

void
test_instr_transpose ()
{
  block_t a, b, c;
  float src[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 },
        dst[6] = { 1.0, 4.0, 2.0, 5.0, 3.0, 6.0 };

  a = block_alloc (2, 3);
  b = block_alloc (3, 2);
  c = block_alloc (2, 3);

  memcpy (a.data, src, 6 * sizeof (float));

  instr_apply (INSTR_TYPE_TRANSPOSE, &b, &a);
  instr_apply (INSTR_TYPE_TRANSPOSE, &c, &b);

  TEST_ASSERT_FLOAT_ARRAY_WITHIN (0.0, dst, b.data, 6);
  TEST_ASSERT_FLOAT_ARRAY_WITHIN (0.0, src, c.data, 6);

  block_free (&a);
  block_free (&b);
  block_free (&c);
}
