#include "StandardInput.h"
#include "z3++.h"

#define PUSH_INSTANTIATIONS(TERM, TERMS, ARRAY)\
  for(auto const & TERM : ARRAY)\
  TERMS.insert(TERM);\
  ARRAY.resize(0);

#define UNARY_INSTANTIATION(TERM, TERMS, ARRAY, OP)\
  for(auto const & TERM : TERMS){\
    z3::expr new_term = (OP).simplify();\
    if(!inSet(new_term, TERMS))\
    ARRAY.push_back(new_term);\
  }\
  PUSH_INSTANTIATIONS(TERM, TERMS, ARRAY);

#define BINARY_INSTANTIATIONS(TERM1, TERM2, TERMS, ARRAY, OP)\
  for(auto const & TERM1 : TERMS){\
    for(auto const & TERM2 : TERMS){\
      z3::expr new_term = (OP).simplify();\
      if(!inSet(new_term, TERMS))\
      ARRAY.push_back(new_term);\
    }\
  }\
  PUSH_INSTANTIATIONS(TERM1, TERMS, ARRAY);

StandardInput::InstantiatedTerms::InstantiatedTerms(
    AXDSignature const & sig,
    z3::expr_vector const & vars):
  sig(sig),
  terms({}),
  num_of_instantiations(0),
  num_of_new_index(0)
{
  for(auto var : vars)
    terms.insert(var);
}

AXDSignature::z3_expr_set const & StandardInput::InstantiatedTerms::getInstantiatedTerms() const {
  return terms;
}

void StandardInput::InstantiatedTerms::operator++(){
  // TODO: implement/compare more heuristics
  // to increase N_instantiation
  // TODO: more testing is needed
  //
  // Heuristic for triggering N-instantiation
  if(num_of_new_index % 5 == 0){
    num_of_instantiations++;
    switch(sig.getTheoryName()){
      case AXDSignature::QF_TO:
        return;
      case AXDSignature::QF_IDL:
        instantiate_QF_IDL();
        return;
      case AXDSignature::QF_UTVPI:
        instantiate_QF_UTVPI();
        return;
      case AXDSignature::QF_LIA:
        instantiate_QF_LIA();
        return;
    }
  }
}

void StandardInput::InstantiatedTerms::instantiate_QF_IDL(){
  z3::expr_vector temp(sig.ctx);

  // Successor
  UNARY_INSTANTIATION(term, terms, temp, term + 1);
  // Predecessor
  UNARY_INSTANTIATION(term, terms, temp, term - 1);
}

void StandardInput::InstantiatedTerms::instantiate_QF_UTVPI(){
  instantiate_QF_IDL();

  z3::expr_vector temp(sig.ctx);

  UNARY_INSTANTIATION(term, terms, temp, -term);
}

void StandardInput::InstantiatedTerms::instantiate_QF_LIA(){
  instantiate_QF_UTVPI();

  z3::expr_vector temp(sig.ctx);

  BINARY_INSTANTIATIONS(term1, term2, terms, temp, term1 + term2);
  BINARY_INSTANTIATIONS(term1, term2, terms, temp, term1 - term2);
  BINARY_INSTANTIATIONS(term1, term2, terms, temp, term1 * term2);
  BINARY_INSTANTIATIONS(term1, term2, terms, temp, term1 / term2);
  BINARY_INSTANTIATIONS(term1, term2, terms, temp, z3::mod(term1,term2));
  BINARY_INSTANTIATIONS(term1, term2, terms, temp, z3::rem(term1,term2));
}

void StandardInput::InstantiatedTerms::add_var(z3::expr const & var){
  terms.insert(var);
  num_of_new_index++;
}
