#ifndef _SEPARATED_PAIR_
#define _SEPARATED_PAIR_

#define _DEBUG_SEPARATED_PAIR_ 1

#include "Preprocess.h"

namespace axdinterpolator {

class SeparatedPair {

  friend class AXDInterpolant;

  AXDSignature const &sig;
  Preprocessor const &preprocessor;

  DiffMap diff_map;

  // -) part_1 contains:
  // --) wr-equations
  // --) diff(k)-equations
  // --) |a| = i like equations
  // -) part_2 contains the rest
  z3::expr_vector part_1, part_2;

  z3::expr const index_var;

  void separateIntoPair(z3::expr_vector const &);
  void processPart_1();

public:
  SeparatedPair(AXDSignature const &, Preprocessor const &,
		z3::expr_vector const &, ArrayVars const &);

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
