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

AXDInterpolant::AXDInterpolant(z3::context & ctx, char const * file_name, unsigned allowed_attempts) : 
  Preprocessor(ctx, file_name),
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

#if _Z3_OUTPUT_FILE_
      std::ofstream file("output.smt2");
      // TODO: Design proper output file for:
      // - Z3
      file << solver.to_smt2();
#endif
#if _MATHSAT5_OUTPUT_FILE_
      std::ofstream file("output.smt2");
      // TODO: Design proper output file for:
      // - Mathsat5
      file << solver.to_smt2();
#endif

#if _DIRECT_INTERP_COMPUTATION_
      z3::expr interpolant = computeInterpolant();
      // Show interpolant
      std::cout << interpolant << std::endl;

#if _TEST_OUTPUT
      testOutput(interpolant);
#endif
      solver.pop();

      std::cout << "Unsat after " 
        << constant_allowed_attempts - allowed_attempts 
        << " iterations" << std::endl;
      return;
    }
#endif

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

z3::expr AXDInterpolant::computeInterpolant(){
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
  z3::expr_vector interpolant = ctx.get_interpolant(
      solver.proof(),
      marked_formula,
      z3::params(ctx));

  return interpolant[0];
}

void AXDInterpolant::testOutput(z3::expr const & interpolant){
  std::ofstream test1_file("test1.smt2");
  std::ofstream test2_file("test2.smt2");

  z3::expr x = ctx.constant("x", this->array_sort);
  z3::expr y = ctx.constant("y", this->array_sort);
  z3::expr e = ctx.constant("e", this->element_sort);
  z3::expr i = ctx.constant("i", this->int_sort);
  z3::expr j = ctx.constant("j", this->int_sort);

  z3::solver test1(ctx);
#if _TEST_ORIGINAL_INPUT
  test1.add(not(z3::implies(assertions[0], interpolant)));
  // Adding axiomatization
  test1.add(forall(y, i, e, rd(wr(y, i, e), i) == e));
  test1.add(forall(y, i , j, e, z3::implies(i != j, rd(wr(y, i, e), j) == rd(y, j))));
  test1.add(forall(x, y, z3::implies(x != y, rd(x, diff(x, y)) != rd(y, diff(x, y)))));
  test1.add(forall(x, y, i, z3::implies(i > diff(x, y), rd(x, i) == rd(y, i))));
  test1.add(forall(x, diff(x, x) == 0));
#else
  test1.add(not(z3::implies(z3::mk_and(part_a_vector), interpolant)));
#endif
  test1_file << test1.to_smt2();

  z3::solver test2(ctx);
#if _TEST_ORIGINAL_INPUT
  test2.add(assertions[1] && interpolant);
  // Adding axiomatization
  test2.add(forall(y, i, e, rd(wr(y, i, e), i) == e));
  test2.add(forall(y, i , j, e, z3::implies(i != j, rd(wr(y, i, e), j) == rd(y, j))));
  test2.add(forall(x, y, z3::implies(x != y, rd(x, diff(x, y)) != rd(y, diff(x, y)))));
  test2.add(forall(x, y, i, z3::implies(i > diff(x, y), rd(x, i) == rd(y, i))));
  test2.add(forall(x, diff(x, x) == 0));
#else
  test2.add(mk_and(part_b_vector) && interpolant);
#endif
  test2_file << test2.to_smt2();
}
