#include "AXDInterpolant.h"
#include <fstream>

AXDInterpolant::CircularPairIterator::CircularPairIterator(z3_expr_set const & vars) : 
  vars(vars),
  first(vars.begin()), second(vars.begin())
{
  avoidLowerDiagonal();
}

void AXDInterpolant::CircularPairIterator::next(){
  second++;

  if(second == vars.end()){
    first++;
    second = vars.begin();
  }
  if(first == vars.end())
    first = vars.begin();

  avoidLowerDiagonal();
}

StandardInput::DiffMap::z3_expr_pair AXDInterpolant::CircularPairIterator::operator *() const {
  return StandardInput::DiffMap::z3_expr_pair(*first, *second);
}

void AXDInterpolant::CircularPairIterator::avoidLowerDiagonal(){
  while(!Z3ExprComparator()(*first, *second)) 
    next();
}

std::ostream & operator << (std::ostream & os, AXDInterpolant::CircularPairIterator const & cpi){
  return os << *cpi.first << ", " << *cpi.second;
}

AXDInterpolant::AXDInterpolant(z3::context & ctx, char const * file) : 
  Preprocessor(ctx, file),
  //solver(ctx, "QF_LIA"), 
  solver(ctx), 
  part_a(assertions[0], part_a_index_vars, part_a_array_vars),
  part_b(assertions[1], part_b_index_vars, part_b_array_vars)
{
  //std::cout << "A-part part 2: " << part_a.part_2 << std::endl;
  //std::cout << "B-part part 2: " << part_b.part_2 << std::endl;
  loop();
}

void AXDInterpolant::loop(){
  unsigned allowed_attempts = 100;
  CircularPairIterator it(common_array_vars);

#if _OUTPUT_FILE_
  std::ofstream file("output.smt2");
#endif 

  while(--allowed_attempts){
    solver.push();
    for(auto const & assertion : part_a.part_2)
      solver.add(assertion);
    for(auto const & assertion : part_b.part_2)
      solver.add(assertion);
    if(solver.check() == z3::unsat){
#if _OUTPUT_FILE_
      file << solver.to_smt2();
#endif
      solver.pop();
      return;
    }
    solver.pop();

    // Find pair of common array variables
    auto const & entry = *it;

    unsigned part_a_dim = part_a.diff_map.size_of_entry(entry),
    part_b_dim = part_b.diff_map.size_of_entry(entry),
    min_dim = std::min(part_a_dim, part_b_dim);
    
    std::cout << "Part a dim " << part_a_dim << std::endl;
    std::cout << "Part b dim " << part_b_dim << std::endl;

    auto const & _new_index = fresh_index_constant();
    part_a.updateSaturation(entry, _new_index, min_dim);
    part_b.updateSaturation(entry, _new_index, min_dim);

    it.next();
  }
  if(!allowed_attempts)
    std::cout << "Input formula is satisfiable / "
      "Or internal failure" << std::endl;
}
