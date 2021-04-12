#ifndef _PREPROCESS_
#define _PREPROCESS_

#define _DEBUG_PREPROCESS_ 0

#include <iostream>
#include <string>
#include "AXDSignature.h"

class Preprocessor {

  friend class AXDInterpolant;

  enum SideInterpolant { PART_A, PART_B };

  z3::expr remove_Not_Length_Apps(z3::expr const &);

  void flattenPredicate(
      z3::expr const &, SideInterpolant,
      unsigned &);
  void flattenPredicateAux(
      z3::expr const &, SideInterpolant, 
      unsigned &);
  void flattenTerm(
      z3::expr const &, SideInterpolant,
      unsigned &);
  void cojoin(z3::expr const &, 
      z3::expr const &, SideInterpolant,
      unsigned &);
  void cojoin_aux(z3::expr_vector &, 
      z3::expr const &, z3::expr const &,
      unsigned &);

  void updateArrayVars(
      z3::expr const &, SideInterpolant);
  void updateIndexVars(
      z3::expr const &, SideInterpolant);
  void updateVarsDB(
      z3::expr const &, z3::sort const &, 
      SideInterpolant);

  void removeDuplicates(z3::expr_vector &);

  z3::expr fresh_index_constant();
  z3::expr fresh_array_constant();
  z3::expr fresh_constant(z3::sort const &);

  AXDSignature &  sig;
  unsigned        fresh_index;
  z3::expr_vector input_part_a, input_part_b;

  z3::expr_vector 
    part_a_index_vars,
    part_b_index_vars;

  // [TODO]: lift the following
  // structures to map i.e.
  // map<ElementSorts, AXDSignature::z3_expr_set>
  AXDSignature::z3_expr_set
    part_a_array_vars, 
    part_b_array_vars,
    common_array_vars;

  public:
  Preprocessor(AXDSignature &, 
      z3::expr const &,
      z3::expr const &);
};

#endif
