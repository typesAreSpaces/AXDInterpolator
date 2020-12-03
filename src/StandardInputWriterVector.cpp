#include "StandardInput.h"

StandardInput::WriteVector::WriteVector() : 
  m_vector({}) {}

// Keeps track of the a, b, index in 
// wr-equations of the form a = wr(b, index, e)
void StandardInput::WriteVector::add(
    z3::expr const & a, 
    z3::expr const & b, 
    z3::expr const & index,
    z3::expr const & element){
  m_vector.push_back(
      std::make_tuple(a, b, index, element)
      );
}
