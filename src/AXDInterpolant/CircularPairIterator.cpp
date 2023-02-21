#include "AXDInterpolant.h"

axdinterpolator::AXDInterpolant::CircularPairIterator::CircularPairIterator(
    Preprocessor::ArrayVars const & vars) : 
  vars(vars),
  p_array_vars_it(vars.begin()), 
  z3_expr_set_first((p_array_vars_it->second).begin()),
  z3_expr_set_second((p_array_vars_it->second).begin())
{ 
  avoidLowerDiagonalAndDifferentTypes(); 
}

void axdinterpolator::AXDInterpolant::CircularPairIterator::next(){
  z3_expr_set_second++;

  if(z3_expr_set_second == p_array_vars_it->second.end()){
    z3_expr_set_first++;
    z3_expr_set_second = p_array_vars_it->second.begin();
  }
  if(z3_expr_set_first == p_array_vars_it->second.end()){
    p_array_vars_it++;
    z3_expr_set_first = p_array_vars_it->second.begin();
    z3_expr_set_second = p_array_vars_it->second.begin();
  }
  if(p_array_vars_it == vars.end()){
    p_array_vars_it = vars.begin();
    z3_expr_set_first = p_array_vars_it->second.begin();
    z3_expr_set_second = p_array_vars_it->second.begin();
  }

  avoidLowerDiagonalAndDifferentTypes();
}

axdinterpolator::SeparatedPair::DiffMap::z3_expr_pair
axdinterpolator::AXDInterpolant::CircularPairIterator::operator*() const {
  return axdinterpolator::SeparatedPair::DiffMap::z3_expr_pair(
      *z3_expr_set_first, *z3_expr_set_second);
}

void axdinterpolator::AXDInterpolant::CircularPairIterator::
    avoidLowerDiagonalAndDifferentTypes() {
  while (!AXDSignature::Z3ExprComparator()(*z3_expr_set_first,
					   *z3_expr_set_second) ||
	 (*z3_expr_set_first).get_sort().id() !=
	     (*z3_expr_set_second).get_sort().id()) {
    next();
  }
}
