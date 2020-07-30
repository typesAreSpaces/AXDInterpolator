#ifndef _STANDARD_INPUT_
#define _STANDARD_INPUT_
#define _DEBUG_STDINPUT_ 0

#include <z3++.h>

class StandardInput {

  z3::expr_vector part_1, part_2;

  public:
  StandardInput(z3::expr const &);
  void update();
};

#endif
