#include "util.h"

axdinterpolator::z3_sort_vector_unique::z3_sort_vector_unique(z3::context & ctx):
  z3::sort_vector(ctx),
  sort_ids({})
{
}

void axdinterpolator::z3_sort_vector_unique::push(z3::sort const & e){
  if(!inSet(e.id(), sort_ids)){
    sort_ids.insert(e.id());
    push_back(e);
  }
}
