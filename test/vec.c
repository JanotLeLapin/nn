#include "../vec.h"
#include <unity/unity.h>

void
test_vec_append ()
{
  vec_t v;
  float src[4] = { 1.0, 2.0, 3.0, 4.0 };

  TEST_ASSERT_NOT_EQUAL (-1, vec_alloc (&v, 4, 32));
  TEST_ASSERT_NOT_EQUAL (-1, vec_append (&v, src, 2));

  TEST_ASSERT_EQUAL (2, v.len);
  TEST_ASSERT_EQUAL (1.0, *(float *)vec_get (&v, 0));
  TEST_ASSERT_EQUAL (2.0, *(float *)vec_get (&v, 1));

  TEST_ASSERT_NOT_EQUAL (-1, vec_append (&v, &src[2], 2));

  TEST_ASSERT_EQUAL (4, v.len);
  TEST_ASSERT_EQUAL (1.0, *(float *)vec_get (&v, 0));
  TEST_ASSERT_EQUAL (2.0, *(float *)vec_get (&v, 1));
  TEST_ASSERT_EQUAL (3.0, *(float *)vec_get (&v, 2));

  vec_free (&v);
}
