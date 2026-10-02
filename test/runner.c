#include <unity/unity.h>

extern void test_instr_transpose ();

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
  return UNITY_END ();
}
