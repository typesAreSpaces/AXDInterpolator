#include "AXDInterpolant.h"
#include <fstream>

AXDInterpolant::AXDInterpolant(z3::context & ctx, char const * file) : 
  Preprocessor(ctx, file),
  //solver(ctx, "QF_LIA"), 
  solver(ctx), 
  part_a(assertions[0], all_index_vars, part_a_array_vars),
  part_b(assertions[1], all_index_vars, part_b_array_vars)
{
  //std::cout << "A-part part 2: " << part_a.part_2 << std::endl;
  //std::cout << "B-part part 2: " << part_b.part_2 << std::endl;
  for(auto const & form : part_a.part_2)
    solver.add(form);
  for(auto const & form : part_b.part_2)
    solver.add(form);

  loop();
}

void AXDInterpolant::loop(){
  unsigned allowed_attempts = 100;

#if _OUTPUT_FILE_
  std::ofstream file("output.smt2");
#endif 

  while(allowed_attempts--){
    // TODO: implement the rest
    solver.push();
    for(auto const & assertion : part_a.part_2)
      solver.add(assertion);
    for(auto const & assertion : part_b.part_2)
      solver.add(assertion);
    if(solver.check() == z3::unsat && 
        (
#if _OUTPUT_FILE_
         file << solver.to_smt2(),
#endif
         solver.pop(), 
         true))
      return;
  }
  if(!allowed_attempts)
    std::cout << "Input formula is satisfiable / "
      "Or internal failure" << std::endl;
}
