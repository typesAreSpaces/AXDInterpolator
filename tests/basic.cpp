#include "AXDInterpolant.h"
#include "Preprocess.h"
#include "StandardInput.h"

int main(){

  z3::context ctx;
  AXDInterpolant test(ctx, "./tests/smt2-files/paper_example.smt2", 10);
  //AXDInterpolant test(ctx, "./tests/smt2-files/kapur_example.smt2", 10);

  return 0;
}
