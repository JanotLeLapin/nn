#include <unity/unity.h>

extern void test_instr_transpose ();
extern void test_instr_gemm ();

extern void test_vec_append ();

void
setUp ()
{
}

void
tearDown ()
{
}

int
main ()
{
  UNITY_BEGIN ();
  RUN_TEST (test_instr_transpose);
  RUN_TEST (test_instr_gemm);
  RUN_TEST (test_vec_append);
  return UNITY_END ();
}
