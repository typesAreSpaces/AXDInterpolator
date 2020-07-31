#ifndef _PREPROCESS_
#define _PREPROCESS_
#define _DEBUG_PREPROCESS_ 0

#include <iostream>
#include <string>
#include "AXDSignature.h"

class Preprocessor : public AXDSignature {

  enum SideInterpolant {
    PART_A, PART_B
  };

  z3::solver really_a_parser;

  unsigned fresh_index, num_args_aux;

  void flattenPredicate(z3::expr const &, SideInterpolant);
  void flattenPredicateAux(z3::expr const &, SideInterpolant);
  void flattenTerm(z3::expr const &, SideInterpolant);
  
  z3::expr fresh_array_constant();
  z3::expr fresh_element_constant();
  z3::expr fresh_index_constant();

  void cojoin(z3::expr const &, z3::expr const &, SideInterpolant);
  void updateArrayVarIds(z3::expr const &, SideInterpolant);
  void removeDuplicates(z3::expr_vector &);

  protected:
  z3::expr_vector assertions, all_index_vars;

  z3_expr_set
    part_a_array_vars, 
    part_b_array_vars,
    common_array_vars;

  public:
  Preprocessor(z3::context &, char const *);
};

#endif
