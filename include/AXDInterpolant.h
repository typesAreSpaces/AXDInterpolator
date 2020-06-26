#ifndef _AXD_INTERPOLANT_
#define _AXD_INTERPOLANT_
#define _DEBUG_AXD_INTER_ 1

#include <iostream>
#include <utility>
#include <set>
#include "AXDSignature.h"

#define inSet(element, set) (set.find(element) != set.end())

class AXDInterpolant : AXDSignature {

  typedef std::pair<z3::expr_vector, z3::expr_vector> Part;

  z3::context & ctx;
  z3::solver solver;
  
  Part part_a, part_b;

  unsigned fresh_index;

  z3::expr_vector       common_array_vars;
  std::set<std::string> common_array_var_names;
  std::set<std::string> aux_common_array_var_names;

  void initial_split(z3::expr const &, Part &);
  void traverseA(z3::expr const &);
  void traverseB(z3::expr const &);

  void saturate(Part &, z3::expr const &);
  void loop();

  public: 
  AXDInterpolant(z3::context &, z3::expr const &, z3::expr const &);
  friend std::ostream & operator << (std::ostream &, AXDInterpolant const &);
};

#endif
