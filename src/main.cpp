#include "todo.h"
#include "InputFormulaParser.h"
#include "util.h"

enum BENCHMARK_EXIT_CODE { SUCCESS, FAILED, TIMEOUT };

void test1();
void test2();
void testZ3ExprVectorSet();

int main(int argc, char **argv) {

  axdinterpolator::run(argc, argv);

  return 0;
}
