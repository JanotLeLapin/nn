#include <unity/unity.h>

extern void test_instr_transpose ();
extern void test_instr_gemm ();

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
  return UNITY_END ();
}
