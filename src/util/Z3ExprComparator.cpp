#include "util.h"

bool axdinterpolator::Z3ExprComparator::operator() (z3::expr const & a, z3::expr const & b) const{
  return a.id() > b.id();
}