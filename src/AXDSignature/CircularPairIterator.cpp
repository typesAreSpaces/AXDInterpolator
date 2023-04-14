#include "AXDSignature.h"

axdinterpolator::CircularPairIterator::CircularPairIterator(
    ArrayVars const &vars, bool enableCircularity = true)
    : vars(vars), p_array_vars_it(vars.begin()),
      z3_expr_set_first((p_array_vars_it->second).begin()),
      z3_expr_set_second((p_array_vars_it->second).begin()),
      enableCircularity(enableCircularity) {
  // std::cout << "Creating new CircularPairIterator" << std::endl;
  avoidLowerDiagonalAndDifferentTypes();
  // std::cout << "DONE Creating new CircularPairIterator" << std::endl;
}

void axdinterpolator::CircularPairIterator::next() {
  // std::cout << "using next()" << std::endl;
  z3_expr_set_second++;

  if (z3_expr_set_second == p_array_vars_it->second.end()) {
    z3_expr_set_first++;
    z3_expr_set_second = p_array_vars_it->second.begin();
  }
  if (z3_expr_set_first == p_array_vars_it->second.end()) {
    p_array_vars_it++;
    z3_expr_set_first = p_array_vars_it->second.begin();
    z3_expr_set_second = p_array_vars_it->second.begin();
  }
  if (!this->enableCircularity && end()) {
    // std::cout << ">> end pointer detected" << std::endl;
    // z3_expr_set_first = p_array_vars_it->second.begin();
    // z3_expr_set_second = p_array_vars_it->second.begin();
    return;
  }
  if (end()) {
    // std::cout << ">> end pointer detected but it will repeat" << std::endl;
    p_array_vars_it = vars.begin();
    z3_expr_set_first = p_array_vars_it->second.begin();
    z3_expr_set_second = p_array_vars_it->second.begin();
  }

  // std::cout << "about to enter avoidLower ..." << std::endl;

  avoidLowerDiagonalAndDifferentTypes();
}

bool axdinterpolator::CircularPairIterator::end() {
  // std::cout << "using end()" << std::endl;
  return p_array_vars_it == vars.end();
}

axdinterpolator::DiffMap::z3_expr_pair
axdinterpolator::CircularPairIterator::operator*() const {
  return axdinterpolator::DiffMap::z3_expr_pair(*z3_expr_set_first,
						*z3_expr_set_second);
}

void axdinterpolator::CircularPairIterator::
    avoidLowerDiagonalAndDifferentTypes() {
  while (!Z3ExprComparator()(*z3_expr_set_first, *z3_expr_set_second) ||
	 (*z3_expr_set_first).get_sort().id() !=
	     (*z3_expr_set_second).get_sort().id()) {
    next();
  }
}
