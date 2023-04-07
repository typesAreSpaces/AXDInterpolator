#include "../include/todo.h"
#include "util.h"
#include "z3++.h"
#include <sys/wait.h>
#include "../include/Preprocess.h"

enum BENCHMARK_EXIT_CODE { SUCCESS, FAILED, TIMEOUT };

void test1();
void test2();

int main(int argc, char **argv) {

  // axdinterpolator::run(argc, argv);
  test1();
  return 0;
}


void test1() {
  z3::context ctx;

  //auto A = ctx.uninterpreted_sort("A");
  auto A = ctx.int_sort();
  auto a = ctx.constant("a", A);
  auto b = ctx.constant("b", A);
  auto c = ctx.constant("c", A);
  auto d = ctx.constant("d", A);
  auto e = ctx.constant("e", A);
  auto f = ctx.function("f", A, A, A);
  auto g = ctx.function("g", A, A, A, A);
  auto function = f(f(a, b), f(g(a, b, f(c, b)), f(a, b)));

  std::cout << function.get_sort() << std::endl;

  // expected output:
  // var0 = (f a b)
  // var1 = (f c b)
  // var2 = (g a b var1)
  // var3 = (f var2 var0)
  // (f var0 var3)
  // a
  // b
  // c
  // MIGHT BE IN DIFFERENT ORDER!

  z3::solver solver1(ctx);
  
  z3::expr_vector vector_of_final_expr = flattening(function, ctx);

  for (unsigned int i = 0; i < vector_of_final_expr.size(); i++) {
    if (i == vector_of_final_expr.size() - 1) {
      solver1.add(vector_of_final_expr[i] != function);
    } else {
      solver1.add(vector_of_final_expr[i]);
    }
    
  }
  
  std::cout << solver1 << std::endl;
  std::cout << solver1.to_smt2() << std::endl;
  std::cout << solver1.check() << std::endl;
}

void test2() {
  z3::context ctx;

  auto mytype = ctx.uninterpreted_sort("B");
  auto a = ctx.constant("a", mytype);
  auto b = ctx.constant("b", mytype);
  auto f = ctx.function("f", mytype, mytype, mytype);
  auto expr1 = f(a, f(a, b));

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
