#include "StandardInput.h"

void StandardInput::instantiate(z3::solver & s, z3::expr & e) const {
  for(auto const & index_term : current_instantiated_index_terms){
    z3::expr_vector from(ctx), to(ctx);
    from.push_back(index_var);
    to.push_back(index_term);
    s.add(e.substitute(from, to));
  }
}

void StandardInput::instantiate(std::ostream & os, z3::expr & e) const {
  for(auto const & index_term : current_instantiated_index_terms){
    z3::expr_vector from(ctx), to(ctx);
    from.push_back(index_var);
    to.push_back(index_term);
    os << (e.substitute(from, to)) << std::endl;
  }
}

void StandardInput::instantiate(z3::expr_vector & v, z3::expr & e) const {
  for(auto const & index_term : current_instantiated_index_terms){
    z3::expr_vector from(ctx), to(ctx);
    from.push_back(index_var);
    to.push_back(index_term);
    v.push_back(e.substitute(from, to));
  }
}
