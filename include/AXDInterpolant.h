#ifndef _AXD_INTERPOLANT_
#define _AXD_INTERPOLANT_
#define _DEBUG_AXD_INTER_           0
#define _Z3_OUTPUT_FILE_            1
#define _MATHSAT5_OUTPUT_FILE_      1
#define _DIRECT_INTERP_COMPUTATION_ 0
#define _TEST_OUTPUT_               0
#define _TEST_ORIGINAL_INPUT_       0

#include <iostream>
#include <utility>
#include <set>
#include <fstream>
#include "Preprocess.h"
#include "StandardInput.h"

class AXDInterpolant : public Preprocessor {

  z3::solver solver;
  StandardInput part_a, part_b;
  std::string m_file_name;

  void loop(unsigned);
  void testOutput(z3::expr const &);

  public: 
  AXDInterpolant(z3::context &, char const *, unsigned);

  z3::expr computeInterpolant();
  // TODO: implementation is missing
  z3::expr liftInterpolant(); 

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
