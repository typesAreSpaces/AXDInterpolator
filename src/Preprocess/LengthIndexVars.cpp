#include "Preprocess.h"

void axdinterpolator::Preprocessor::updateLengthIndexVars(
    z3::expr const &array_var, bool isEmptyArray) {
  unsigned index = array_var.id();
  // If array_var is not an empty array,
  // set the equation |array_var| = i
  // for some fresh constant i
  if (!isEmptyArray) {
    z3::expr freshIndexVar = fresh_index_constant();
    if (length_index_vars.find(index) == length_index_vars.end())
      length_index_vars.insert(
	  std::pair<unsigned, z3::expr>(index, freshIndexVar));
    return;
  }
  // Otherwise, set |empty_array| = 0
  if (length_index_vars.find(index) == length_index_vars.end())
    length_index_vars.insert(
	std::pair<unsigned, z3::expr>(index, sig.ctx.int_val(0)));
}

z3::expr
axdinterpolator::Preprocessor::getLengthIndexVar(z3::expr const &array_var) {
  assert(length_index_vars.find(array_var.id()) != length_index_vars.end());
  return length_index_vars.find(array_var.id())->second;
}
