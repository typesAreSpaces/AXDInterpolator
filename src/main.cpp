#include "util.h"
#include "todo.h"

enum BENCHMARK_EXIT_CODE { SUCCESS, FAILED, TIMEOUT };

int main(int argc, char ** argv) {

  // axdinterpolator::run(argc, argv);
 
  z3::context ctx;
  auto A = ctx.uninterpreted_sort("A");
  auto a = ctx.constant("a", A);
  auto b = ctx.constant("b", A);
  auto f = ctx.function("f", A, A, A);
  auto f_f_a_b_a = f(f(a, b), a);
  traverse(f_f_a_b_a);

  test();

  return 0;

}
