#ifndef _SEPARATED_PAIR_
#define _SEPARATED_PAIR_

#define _DEBUG_STDINPUT_ 0

#include "Preprocess.h"

namespace axdinterpolator {

class SeparatedPair {

  friend class AXDInterpolant;
 
  AXDSignature const & sig;

  DiffMap diff_map;
  WriteVector write_vector;

  // -) part_1 contains:
  // --) wr-equations
  // --) diff(k)-equations
  // --) |a| = i like equations
  // -) part_2 contains the rest
  z3::expr_vector part_1, part_2;

  z3::expr const index_var;

public:
  SeparatedPair(AXDSignature const &, z3::expr_vector const &,
		z3::expr_vector &, ArrayVars const &);

  void initSaturation();
  void updateSaturation(DiffMap::z3_expr_pair const &, z3::expr const &,
			unsigned min_dim);

  // void instantiate(z3::solver &, z3::expr &) const;
  // void instantiate(std::ostream &, z3::expr &) const;
  // void instantiate(z3::expr_vector &, z3::expr &) const;

  friend std::ostream &operator<<(std::ostream &, SeparatedPair const &);
};

} // namespace axdinterpolator

#endif
