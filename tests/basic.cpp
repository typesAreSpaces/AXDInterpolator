#include "AXDInterpolant.h"
#include <z3++.h>

int main(){

  z3::set_param("proof", true);
  z3::context ctx;
  AXDInterpolant test(ctx, "./tests/smt2-files/paper_example.smt2", 10);
  //AXDInterpolant test(ctx, "./tests/smt2-files/kapur_example.smt2", 10);

  return 0;
}
