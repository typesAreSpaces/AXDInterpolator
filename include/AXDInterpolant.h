#ifndef _AXD_INTERPOLANT_
#define _AXD_INTERPOLANT_
#define _DEBUG_AXD_INTER_ 0
#define _OUTPUT_FILE_ 1

#include <iostream>
#include <utility>
#include <set>
#include "Preprocess.h"
#include "StandardInput.h"

class AXDInterpolant : public Preprocessor {

  z3::solver solver;
  StandardInput part_a, part_b;

  void loop(unsigned);

  public: 
  AXDInterpolant(z3::context &, char const *, unsigned);

  class CircularPairIterator {
    friend class AXDInterpolant;
    z3_expr_set const & vars;
    z3_expr_set::iterator first, second;
    void avoidLowerDiagonal();
    public:
    CircularPairIterator(z3_expr_set const &);
    void next();
    StandardInput::DiffMap::z3_expr_pair operator *() const;

    friend std::ostream & operator << (std::ostream &, CircularPairIterator const &);
  };

};

#endif
