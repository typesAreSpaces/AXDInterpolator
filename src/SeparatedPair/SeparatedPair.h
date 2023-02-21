#ifndef _SEPARATED_PAIR_
#define _SEPARATED_PAIR_

#define _DEBUG_STDINPUT_ 0

#ifndef NDEBUG
#   define ASSERT(condition, message) \
  do { \
    if (! (condition)) { \
      std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
      << " line " << __LINE__ << ": " << message << std::endl; \
      std::terminate(); \
    } \
  } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

#include "z3++.h"
#include <set>
#include <utility>
#include <map>
#include <vector>
#include <tuple>
#include "Preprocess.h"

namespace axdinterpolator {

class SeparatedPair {

  friend class AXDInterpolant;
 
  AXDSignature const &sig;
  DiffMap diff_map;
  WriteVector write_vector;
  InstantiatedTerms instantiated_terms;

  // -) part_1 contains wr-equations and diff(k)-equations
  // of the original input
  // -) part_2 contains the rest
  z3::expr_vector part_1, part_2;

  z3::expr const index_var;
  z3::expr axiom_8, axiom_9;

  z3::expr generateAxiom8(ArrayVars const &) const;
  z3::expr generateAxiom9() const;

public:
  SeparatedPair(AXDSignature const &, z3::expr_vector const &,
		z3::expr_vector &, ArrayVars const &);

  void initSaturation();
  void updateSaturation(DiffMap::z3_expr_pair const &, z3::expr const &,
			unsigned min_dim);

  void instantiate(z3::solver &, z3::expr &) const;
  void instantiate(std::ostream &, z3::expr &) const;
  void instantiate(z3::expr_vector &, z3::expr &) const;

  friend std::ostream &operator<<(std::ostream &, SeparatedPair const &);
};

} // namespace axdinterpolator

#endif
