#include "todo.h"
#include "InputFormulaParser.h"
#include "util.h"

enum BENCHMARK_EXIT_CODE { SUCCESS, FAILED, TIMEOUT };

int main(int argc, char **argv) {

  axdinterpolator::run(argc, argv);

  return 0;
}
