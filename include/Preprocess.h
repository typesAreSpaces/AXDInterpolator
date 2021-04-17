#ifndef _PREPROCESS_
#define _PREPROCESS_

#define _DEBUG_PREPROCESS_ 0

#include <iostream>
#include <string>
#include "AXDSignature.h"

class Preprocessor {

  friend class AXDInterpolant;

  enum SideInterpolant { PART_A, PART_B };

  // [NOTICE] This structure only contains
  // array vars of different 
  // parametrized types
  public:
  class ArrayVars { 

    friend class AXDInterpolant;

    typedef std::map<
      unsigned, 
      AXDSignature::z3_expr_set> Container;

    Container vars; 
    unsigned  size;

    public:
    ArrayVars();
    void insert(z3::expr const &);
    bool isEmpty() const;
    unsigned getSize() const;

    Container::const_iterator begin() const;
    Container::const_iterator end() const;
  };

  private:

  // normalizeInput accomplishes the following:
  // 1. Removes applications of length(x)
  // replacing them with diff(x, empty_array)
  // of the appropriate type
  // 2. Remove applications of not
  // 3. Replace select by rd of the appropriate
  // type
  // 4. Replace store by wr of the appropriate
  // type
  z3::expr normalizeInputDefault(z3::expr const &);
  z3::expr normalizeInput(z3::expr const &);

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

  void updateArrayVars(
      z3::expr const &, SideInterpolant);
  void updateIndexVars(
      z3::expr const &, SideInterpolant);
  void updateVarsDB(
      z3::expr const &, SideInterpolant);

  z3::expr fresh_index_constant();
  z3::expr fresh_array_constant(z3::sort const &);
  z3::expr fresh_constant(z3::sort const &);

  AXDSignature &  sig;
  unsigned        fresh_num;
  z3::expr_vector input_part_a, input_part_b;

  AXDSignature::z3_expr_vector_unique
    part_a_index_vars,
    part_b_index_vars;

  ArrayVars part_a_array_vars,
            part_b_array_vars,
            common_array_vars;

  //AXDSignature::z3_expr_set
  //part_a_array_vars, 
  //part_b_array_vars,
  //common_array_vars;

  public:
  Preprocessor(AXDSignature &, 
      z3::expr const &,
      z3::expr const &);

  friend std::ostream & operator << (
      std::ostream &, Preprocessor::ArrayVars const &);
};

#endif
