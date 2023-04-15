#include "AXDSignature.h"

axdinterpolator::LengthIndexVars::LengthIndexVars() :
  vars({})
{ 
}

void axdinterpolator::ArrayVars::insert(
    z3::expr const & array_var){
  unsigned index = array_var.get_sort().id();
  z3::expr freshIndexVar = array_var;
  if(vars.find(index) == vars.end())
    vars.insert(std::pair<unsigned, z3::expr>(index, freshIndexVar));
}
