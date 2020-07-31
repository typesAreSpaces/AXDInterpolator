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

  void loop();

  public: 
  AXDInterpolant(z3::context &, char const *);
};

#endif
