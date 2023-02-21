#include "SeparatedPair.h"
#include "z3++.h"

#define PUSH_INSTANTIATIONS(ARRAY_OF_ARRAYS)\
  for(auto const & vec_pointer : ARRAY_OF_ARRAYS){\
    for(auto const & elem : *vec_pointer){\
      std::cout << ">>> ahhh " << elem << std::endl;\
      terms.insert(elem);\
    }\
    vec_pointer->resize(0);\
  }\
  std::cout << "Done PUSH_INSTANTIATIONS" << std::endl;


#define UNARY_INSTANTIATION(TERM, ARRAY, OP)\
  for(auto const & TERM : terms){\
    z3::expr new_term = (OP).simplify();\
    if(!inSet(new_term, terms))\
    ARRAY.push_back(new_term);\
  }

#define BINARY_INSTANTIATIONS(TERM1, TERM2, ARRAY, OP)\
  for(auto const & TERM1 : terms)\
    for(auto const & TERM2 : terms){\
      z3::expr new_term = (OP).simplify();\
      if(!inSet(new_term, terms))\
      ARRAY.push_back(new_term);\
    }

axdinterpolator::SeparatedPair::InstantiatedTerms::InstantiatedTerms(
    AXDSignature const & sig,
    z3::expr_vector const & vars):
  sig(sig),
  terms({}),
  num_of_instantiations(0),
  num_of_new_index(0),
  new_succs(sig.ctx), new_preds(sig.ctx), new_minus(sig.ctx),
  new_adds(sig.ctx), new_subtracts(sig.ctx)
{
  for(auto var : vars)
    terms.insert(var);
}

axdinterpolator::z3_expr_set const &
axdinterpolator::SeparatedPair::InstantiatedTerms::getInstantiatedTerms() const {
  return terms;
}

unsigned axdinterpolator::SeparatedPair::InstantiatedTerms::getNumOfNewIndex() const {
  return num_of_new_index;
}

void axdinterpolator::SeparatedPair::InstantiatedTerms::operator++(){
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

void axdinterpolator::SeparatedPair::InstantiatedTerms::instantiate_QF_IDL(){
  std::vector<z3::expr_vector*> 
    collection({&new_succs, &new_preds});

  UNARY_INSTANTIATION(term, new_succs, term + 1);
  UNARY_INSTANTIATION(term, new_preds, term - 1);

  PUSH_INSTANTIATIONS(collection);
}

void axdinterpolator::SeparatedPair::InstantiatedTerms::instantiate_QF_UTVPI(){
  std::vector<z3::expr_vector*> 
    collection({&new_succs, &new_preds, &new_minus});

  UNARY_INSTANTIATION(term, new_succs, term + 1);
  UNARY_INSTANTIATION(term, new_preds, term - 1);
  UNARY_INSTANTIATION(term, new_minus, -term);

  PUSH_INSTANTIATIONS(collection);
}

void axdinterpolator::SeparatedPair::InstantiatedTerms::instantiate_QF_LIA(){
  std::vector<z3::expr_vector*> 
    collection({&new_succs, &new_preds, &new_minus
        , &new_adds, &new_subtracts});

  UNARY_INSTANTIATION(term, new_succs, term + 1);
  UNARY_INSTANTIATION(term, new_preds, term - 1);
  UNARY_INSTANTIATION(term, new_minus, -term);
  BINARY_INSTANTIATIONS(term1, term2, new_adds, term1 + term2);
  BINARY_INSTANTIATIONS(term1, term2, new_subtracts, term1 - term2);

  PUSH_INSTANTIATIONS(collection);
}

void axdinterpolator::SeparatedPair::InstantiatedTerms::add_var(z3::expr const & var){
  terms.insert(var);
  num_of_new_index++;
}
