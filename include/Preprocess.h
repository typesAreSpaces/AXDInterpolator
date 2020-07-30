#ifndef _PREPROCESS_
#define _PREPROCESS_

#include <iostream>
#include <string>
#include <unordered_set>
#include "AXDSignature.h"

class Preprocessor : public AXDSignature {

    enum SideInterpolant {
    PART_A, PART_B
  };

  z3::solver really_a_parser;

  unsigned fresh_index, num_args_aux;

  std::unordered_set<unsigned>
    part_a_array_var_ids, 
    part_b_array_var_ids;

  void flattenPredicate(z3::expr const &, SideInterpolant);
  void flattenTerm(z3::expr const &, SideInterpolant);
  z3::expr fresh_array_constant();
  z3::expr fresh_element_constant();
  z3::expr fresh_index_constant();
  void cojoin(z3::expr const &, z3::expr const &, SideInterpolant);
  void updateArrayVarIds(z3::expr const &, SideInterpolant);
  void removeDuplicates(z3::expr_vector &);

  protected:
  z3::expr_vector assertions, initial_index_vars;

  public:
  Preprocessor(char const *);

  bool isArrayVarCommon(z3::expr const &);
  z3::expr getPartA() const;
  z3::expr getPartB() const;
  z3::expr_vector getIndexVars() const;
};

#endif
