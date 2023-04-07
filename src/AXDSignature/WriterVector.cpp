#include "AXDSignature.h"

axdinterpolator::WriteVector::WriteVector() : 
  m_vector({}) {}

// Keeps track of the a, b, index in 
// wr-equations of the form a = wr(b, index, e)
void axdinterpolator::WriteVector::add(
    z3::expr const & a, 
    z3::expr const & b, 
    z3::expr const & index,
    z3::expr const & element){
  m_vector.push_back(
      std::make_tuple(a, b, index, element));
}

namespace axdinterpolator {

std::ostream &
operator<<(std::ostream &os,
	   axdinterpolator::WriteVector const &wv) {
  for (auto const &x : wv.m_vector)
    os << std::get<0>(x) << ", " << std::get<1>(x) << ", " << std::get<2>(x)
       << std::endl;
  return os;

}

} // namespace axdinterpolator
