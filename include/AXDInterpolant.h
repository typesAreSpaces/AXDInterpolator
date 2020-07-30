#ifndef _AXD_INTERPOLANT_
#define _AXD_INTERPOLANT_
#define _DEBUG_AXD_INTER_ 1

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
  AXDInterpolant(char const *);
};

#endif
