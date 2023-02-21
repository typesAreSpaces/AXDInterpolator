#include "SeparatedPair.h"

z3::expr axdinterpolator::SeparatedPair::generateAxiom8(Preprocessor::ArrayVars const & array_var_ids) const {
  z3::expr_vector conjs(sig.ctx);

  // Instantiate with all the current array elements
  // since this won't change, i.e. the algorithm
  // does not produce additional array vars
  for(auto const & array_element_entry : array_var_ids){
    unsigned sort_index = array_element_entry.first;
    for(auto const & array_element 
        : array_element_entry.second){
      auto const & curr_rd = sig.getRdBySort(sort_index);
      auto const & curr_undefined = sig.getUndefinedBySort(sort_index);
      conjs.push_back(
          z3::implies(
            index_var < 0, 
            curr_rd(array_element, index_var) == curr_undefined
            ));
    }
  }

  switch(conjs.size()){
    case 0:
      return sig.ctx.bool_val(true);
    case 1:
      return conjs[0];
    default:
      return z3::mk_and(conjs);
  }
}

z3::expr axdinterpolator::SeparatedPair::generateAxiom9() const {
  z3::expr_vector conjs(sig.ctx);

  for(unsigned i = 0; i < sig.element_sorts.size(); i++){
    auto const & curr_rd = sig.rd_es[i];
    auto const & curr_empty_array = sig.empty_array_es[i];
    auto const & curr_undefined = sig.undefined_es[i];
    conjs.push_back(curr_rd(curr_empty_array, index_var) 
        == curr_undefined);
  }

  switch(conjs.size()){
    case 0:
      return sig.ctx.bool_val(true);
    case 1:
      return conjs[0];
    default:
      return z3::mk_and(conjs);
  }
}
