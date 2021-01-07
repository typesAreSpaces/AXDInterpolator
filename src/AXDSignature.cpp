#include "AXDSignature.h"

AXDSignature::AXDSignature(z3::context & ctx) :
  ctx(ctx),

  bool_sort(ctx.bool_sort()), 
  int_sort(ctx.int_sort()), 
  index_sort(ctx.int_sort()), 
  element_sort(ctx.uninterpreted_sort("ElementSort")), 
  array_sort(ctx.uninterpreted_sort("ArraySort")),

  undefined(ctx.constant("undefined", element_sort)),
  empty_array(ctx.constant("empty_array", array_sort)),

  diff(ctx.function("diff", array_sort, array_sort, index_sort)),
  diff_k(ctx.function("diff_", int_sort, array_sort, array_sort, index_sort)),
  wr(ctx.function("wr", array_sort, index_sort, element_sort, array_sort)),
  rd(ctx.function("rd", array_sort, index_sort, element_sort)),
  pred(ctx.function("pred", index_sort, index_sort)),
  succ(ctx.function("succ", index_sort, index_sort)),
  neg(ctx.function("neg", index_sort, index_sort)),
  add(ctx.function("add", index_sort, index_sort, index_sort)),
  // length is a conservative extension of any Th(T_I)
  // because \forall x . length(x) = diff(x, empty_array)
  length(ctx.function("length", array_sort, index_sort))
{
}

std::ostream & operator << (std::ostream & out, AXDSignature::z3_expr_set const & collection){
  for(auto const & x : collection)
    out << x << std::endl;
  return out;
}
