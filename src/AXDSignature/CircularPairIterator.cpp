#include "AXDSignature.h"

axdinterpolator::CircularPairIterator::CircularPairIterator(
    ArrayVars const &vars, bool enableCircularity = true)
    : vars(vars), p_array_vars_it(vars.begin()),
      z3_expr_set_first((p_array_vars_it->second).begin()),
      z3_expr_set_second((p_array_vars_it->second).begin()),
      enableCircularity(enableCircularity) {
  avoidLowerDiagonal();
}

void axdinterpolator::CircularPairIterator::next() {
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
  if (!enableCircularity && end()) return;
  if (end()) {
    p_array_vars_it = vars.begin();
    z3_expr_set_first = p_array_vars_it->second.begin();
    z3_expr_set_second = p_array_vars_it->second.begin();
  }

  avoidLowerDiagonal();
}

bool axdinterpolator::CircularPairIterator::end() {
  return p_array_vars_it == vars.end();
}

axdinterpolator::DiffMap::z3_expr_pair
axdinterpolator::CircularPairIterator::operator*() const {
  return axdinterpolator::DiffMap::z3_expr_pair(*z3_expr_set_first,
						*z3_expr_set_second);
}

void axdinterpolator::CircularPairIterator::avoidLowerDiagonal() {
  // while (!Z3ExprComparator()(*z3_expr_set_first, *z3_expr_set_second) ||
  // 	 (*z3_expr_set_first).get_sort().id() !=
  // 	     (*z3_expr_set_second).get_sort().id()) {
  //   next();
  // }
  while ((enableCircularity || !end()) &&
	 !Z3ExprComparator()(*z3_expr_set_first, *z3_expr_set_second))
    next();
}
