#include "AXDInterpolant.h"

AXDInterpolant::AXDInterpolant(char const * file) : 
  Preprocessor(file),
  solver(ctx),
  part_a(getPartA()), part_b(getPartB())
{
  for(auto const & form : part_a.getPart_2())
    solver.add(form);
  for(auto const & form : part_b.getPart_2())
    solver.add(form);

  loop();
}

void AXDInterpolant::loop(){
  unsigned dont_stop = 100;

  while(solver.check() != z3::unsat && dont_stop){
    // TODO: implement the rest
    dont_stop--;
  }
}
