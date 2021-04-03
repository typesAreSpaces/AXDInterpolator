#include "AXDSignature.h"

bool AXDSignature::Z3ExprComparator::operator() (z3::expr const & a, z3::expr const & b) const{
  return a.id() > b.id();
}
