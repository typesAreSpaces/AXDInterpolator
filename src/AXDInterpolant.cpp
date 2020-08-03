#include "AXDInterpolant.h"
#include <z3++.h>

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

std::ostream & operator << (std::ostream & os, 
    AXDInterpolant::CircularPairIterator const & cpi){
  return os << *cpi.first << ", " << *cpi.second;
}

AXDInterpolant::AXDInterpolant(z3::context & ctx, char const * file, unsigned allowed_attempts) : 
  Preprocessor(ctx, file),
  //solver(ctx, "QF_LIA"), 
  solver(ctx), 
  part_a(assertions[0], part_a_index_vars, part_a_array_vars),
  part_b(assertions[1], part_b_index_vars, part_b_array_vars)
{
  loop(allowed_attempts);
}

void AXDInterpolant::loop(unsigned allowed_attempts){
  unsigned const constant_allowed_attempts = allowed_attempts;
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
    for(auto const & index : part_a.index_vars)
      solver.add(index >= 0);
    for(auto const & index : part_b.index_vars)
      solver.add(index >= 0);
    if(solver.check() == z3::unsat){
#if _OUTPUT_FILE_
      file << solver.to_smt2();
#endif

#if 1 -_OUTPUT_FILE_
      // -------------------------------------------------------------------
      // Interpolant computation
      z3::expr_vector part_a_vector(ctx);
      z3::expr_vector part_b_vector(ctx);
      for(auto const & assertion : part_a.part_2)
        part_a_vector.push_back(assertion);
      for(auto const & assertion : part_b.part_2)
        part_b_vector.push_back(assertion);
      for(auto const & index : part_a.index_vars)
        part_a_vector.push_back(index >= 0);
      for(auto const & index : part_b.index_vars)
        part_b_vector.push_back(index >= 0);
      z3::expr marked_formula = 
        interpolant(z3::mk_and(part_a_vector)) && z3::mk_and(part_b_vector);
      z3::expr_vector interpolant = ctx.get_interpolant(solver.proof(),
          marked_formula,
          z3::params(ctx));
      std::cout << interpolant[0] << std::endl;
      // -------------------------------------------------------------------
#endif
      solver.pop();

      z3::solver test1(ctx);
      //test1.add(not(z3::implies(assertions[0], interpolant[0])));
      test1.add(not(z3::implies(z3::mk_and(part_a_vector), interpolant[0])));
      std::cout << "Checking if it is interpolant of original input: test 1" << std::endl;
      //std::cout << test1.assertions() << std::endl;
      switch(test1.check()){ 
        case z3::unsat:
          std::cout << "As expected" << std::endl;
          break;
        case z3::sat:
        case z3::unknown:
          std::cout << "Bad news" << std::endl;
          break;
      }
      z3::solver test2(ctx);
      //test2.add(assertions[1] && interpolant[0]);
      test2.add(z3::mk_and(part_b_vector) && interpolant[0]);
      std::cout << "Checking if it is interpolant of original input: test 2" << std::endl;
      //std::cout << test2.assertions() << std::endl;
      switch(test2.check()){
        case z3::unsat:
          std::cout << "As expected" << std::endl;
          break;
        case z3::sat:
        case z3::unknown:
          std::cout << "Bad news" << std::endl;
          break;
      }

      std::cout << "Unsat after " 
        << constant_allowed_attempts - allowed_attempts 
        << " iterations" << std::endl;
      return;
    }
    solver.pop();

#if _DEBUG_AXD_INTER_ 
    std::cout << "A-part part 2: " << part_a.part_2 << std::endl;
    std::cout << "B-part part 2: " << part_b.part_2 << std::endl;
#endif

    // Find pair of common array variables
    auto const & common_pair = *it;

    unsigned part_a_dim = part_a.diff_map.size_of_entry(common_pair),
             part_b_dim = part_b.diff_map.size_of_entry(common_pair),
             min_dim = std::min(part_a_dim, part_b_dim);

    auto const & _new_index = fresh_index_constant();
    part_a.updateSaturation(common_pair, _new_index, min_dim);
    part_b.updateSaturation(common_pair, _new_index, min_dim);

    it.next();
  }
  if(!allowed_attempts)
    std::cout << "Input formula is satisfiable / "
      "Or internal failure" << std::endl;
}
