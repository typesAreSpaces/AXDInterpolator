#include "AXDInterpolant.h"

int main(){

  z3::context ctx;

  AXDSignature sig(ctx);
   
  z3::expr a  = ctx.constant("a",  sig.array_sort);
  z3::expr c1 = ctx.constant("c1", sig.array_sort);
  z3::expr i1 = ctx.constant("i1", sig.index_sort);
  z3::expr b  = ctx.constant("b",  sig.array_sort);
  z3::expr c2 = ctx.constant("c2", sig.array_sort);
  z3::expr i2 = ctx.constant("i2", sig.index_sort);
  z3::expr a1 = ctx.constant("a1", sig.array_sort);
  z3::expr i3 = ctx.constant("i3", sig.index_sort);
  z3::expr e3 = ctx.constant("e3", sig.element_sort);
  z3::expr e1 = ctx.constant("e1", sig.element_sort);

  z3::expr part_a = 
    sig.diff(a, c1) == i1 
    && sig.diff(b, c2) == i1 
    && a == sig.wr(a1, i3, e3)
    && a1 == sig.wr(b, i1, e1);
  z3::expr part_b = 
    i1 < i2 
    && i2 < i3
    && sig.rd(c1, i2) != sig.rd(c2, i2);

  AXDInterpolant test(ctx, part_a, part_b);
  std::cout << test << std::endl;

  return 0;
}
