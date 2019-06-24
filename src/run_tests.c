#include "CuTest.h"
#include "tests.h"

int RunAllTests(void) {
  CuString *output = CuStringNew();
  CuSuite *suite = CuSuiteNew();

  CuSuiteAddSuite(suite, GetSuite());

  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);
  printf("%s\n", output->buffer);

  if (suite->failCount == 0) {
    return 0;
  } else {
    return 1;
  }
}

int main(int argc, const char *argv[]) { return RunAllTests(); }
