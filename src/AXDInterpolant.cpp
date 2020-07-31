#include "AXDInterpolant.h"

AXDInterpolant::AXDInterpolant(z3::context & ctx, char const * file) : 
  Preprocessor(ctx, file),
  solver(ctx),
  part_a(assertions[0], all_index_vars, part_a_array_var_ids),
  part_b(assertions[1], all_index_vars, part_b_array_var_ids)
{
  std::cout << "A-part part 2: " << part_a.part_2 << std::endl;
  std::cout << "B-part part 2: " << part_b.part_2 << std::endl;
  for(auto const & form : part_a.part_2)
    solver.add(form);
  for(auto const & form : part_b.part_2)
    solver.add(form);

  loop();
}

void AXDInterpolant::loop(){
  unsigned dont_stop = 100;

  while(dont_stop){
    // TODO: implement the rest
    solver.push();
    //solver.add();
    if(solver.check() == z3::unsat) {
      solver.pop();
      return;
    }
    solver.pop();
    dont_stop--;
  }
}
