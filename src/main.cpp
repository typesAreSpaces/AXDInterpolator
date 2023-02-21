#include "todo.h"
#include "InputFormulaParser.h"

enum BENCHMARK_EXIT_CODE { SUCCESS, FAILED, TIMEOUT };

void test1();
void test2();

int main(int argc, char **argv) {

  axdinterpolator::run(argc, argv);
  test1();
  test2();
  return 0;
}

void test1() {
  z3::context ctx;

  auto A = ctx.uninterpreted_sort("A");
  auto a = ctx.constant("a", A);
  auto b = ctx.constant("b", A);
  auto f = ctx.function("f", A, A, A);
  auto f_f_a_b_a = f(f(a, b), a);

  traverse(f_f_a_b_a);
}

void test2() {
  z3::context ctx;

  auto mytype = ctx.uninterpreted_sort("B");
  auto a = ctx.constant("a", mytype);
  auto b = ctx.constant("b", mytype);
  auto f = ctx.function("f", mytype, mytype, mytype);
  auto expr1 = f(a, (f(a, b)));

  z3::expr_vector v1(ctx);
  v1.push_back(f(a, b));
  v1.push_back(a);
  std::cout << v1 << std::endl;

  z3::expr_vector v2(ctx);
  v2.push_back(a);
  v2.push_back(b);
  std::cout << v2 << std::endl;

  M_O_instantiation(expr1, v2, v1);
}
