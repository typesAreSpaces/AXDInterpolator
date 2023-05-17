#include "Preprocess.h"

void axdinterpolator::Preprocessor::updateLengthIndexVars(
    z3::expr const &array_var, bool isEmptyArray, SideInterpolant side) {
  unsigned index = array_var.id(); 
  // If array_var is not an empty array,
  // set the equation |array_var| = i
  // for some fresh constant i
  if (!isEmptyArray) {
    if (!inSet(index, length_index_vars)) {
      z3::expr freshIndexVar = fresh_index_constant();
      length_index_vars.insert(
	  std::pair<unsigned, z3::expr>(index, freshIndexVar));
      // Q: Should the algorithm consider the
      // fresh bound as index?
      // A: Yes, since i in |a| = i is considered
      // an index constant as suggested in Example 6.8
      // of the paper
      updateIndexVars(freshIndexVar, side);
      return;
    }
    // Q: Should the algorithm consider the
    // fresh bound as index?
    // A: Yes, since i in |a| = i is considered
    // an index constant as suggested in Example 6.8
    // of the paper
    updateIndexVars(getLengthIndexVar(array_var), side);
    return;
  }
  // Otherwise, set |empty_array| = 0
  if (!inSet(index, length_index_vars))
    length_index_vars.insert(
	std::pair<unsigned, z3::expr>(index, sig.ctx.int_val(0)));
}

z3::expr
axdinterpolator::Preprocessor::getLengthIndexVar(z3::expr const &array_var) const {
  assert(inSet(array_var.id(), length_index_vars));
  return length_index_vars.find(array_var.id())->second;
}
