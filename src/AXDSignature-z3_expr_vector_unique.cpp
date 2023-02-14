#include "AXDSignature.h"

axdinterpolator::AXDSignature::z3_expr_vector_unique::z3_expr_vector_unique(z3::context & ctx):
  z3::expr_vector(ctx),
  expr_ids({})
{
}

void axdinterpolator::AXDSignature::z3_expr_vector_unique::push(z3::expr const & e){
  if(!inSet(e.id(), expr_ids)){
    expr_ids.insert(e.id());
    push_back(e);
  }
}
