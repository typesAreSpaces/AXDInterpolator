#include "AXDInterpolant.h"

AXDInterpolant::CircularPairIterator::CircularPairIterator(
    Preprocessor::ArrayVars const & vars) : 
  vars(vars),
  p_array_vars_it(vars.begin()), 
  z3_expr_set_first((p_array_vars_it->second).begin()),
  z3_expr_set_second((p_array_vars_it->second).begin())
{ 
  avoidLowerDiagonal(); 
}

void AXDInterpolant::CircularPairIterator::next(){
  z3_expr_set_second++;

  if(z3_expr_set_second == p_array_vars_it->second.end()){
    z3_expr_set_first++;
    z3_expr_set_second = p_array_vars_it->second.begin();
  }
  if(z3_expr_set_first == p_array_vars_it->second.end()){
    p_array_vars_it++;
    z3_expr_set_first = p_array_vars_it->second.begin();
  }
  if(p_array_vars_it == vars.end())
    p_array_vars_it = vars.begin();

  avoidLowerDiagonal();
}

StandardInput::DiffMap::z3_expr_pair AXDInterpolant::CircularPairIterator::operator *() const {
  return StandardInput::DiffMap::z3_expr_pair(*z3_expr_set_first, *z3_expr_set_second);
}

void AXDInterpolant::CircularPairIterator::avoidLowerDiagonal(){
  while(!AXDSignature::Z3ExprComparator()(
        *z3_expr_set_first, *z3_expr_set_second)) next();
}
