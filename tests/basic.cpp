#include "AXDInterpolant.h"
#include "Preprocess.h"
#include "StandardInput.h"

int main(){

  z3::context ctx;
  //AXDInterpolant test(ctx, "./tests/smt2-files/paper_example2.smt2");
  //AXDInterpolant test(ctx, "./tests/smt2-files/paper_example3.smt2");
  AXDInterpolant test(ctx, "./tests/smt2-files/paper_example.smt2");

  return 0;
}
