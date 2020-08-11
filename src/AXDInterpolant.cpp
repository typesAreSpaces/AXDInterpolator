#include "AXDInterpolant.h"
#include <fstream>
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
  part_b(assertions[1], part_b_index_vars, part_b_array_vars),
  m_file_name(std::string(file_name))
{
  m_file_name = 
    m_file_name
    .substr(0, m_file_name.find_last_of("."))
    .substr(m_file_name.find_last_of("\\/") + 1);
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
      z3OutputFile();
#endif
#if _MATHSAT5_OUTPUT_FILE_
      mathsatOutputFile();
#endif
#if _DIRECT_INTERP_COMPUTATION_
     directComputation();
#endif
      solver.pop();
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

// Precondition: part_a_vector and part_b_vector should be updated using
// setupPart_A_B_Vectors
z3::expr AXDInterpolant::computeInterpolant(z3::expr_vector & part_a_vector, z3::expr_vector & part_b_vector){

  z3::expr marked_formula = 
    interpolant(z3::mk_and(part_a_vector)) && z3::mk_and(part_b_vector);
  z3::expr_vector interpolant = ctx.get_interpolant(
      solver.proof(),
      marked_formula,
      z3::params(ctx));

  return interpolant[0];
}

//z3::expr AXDInterpolant::liftInterpolant(z3::expr const & interpolant){
  //z3::goal g(ctx);
  //z3::solver s(ctx);

  //for(auto const & diff_entry : part_a.diff_map.m_map){
    //auto const & diff_a = diff_entry.first.first;
    //auto const & diff_b = diff_entry.first.second;
    //auto const & diff_seq = diff_entry.second.new_index_vars;
    //unsigned _index = 1;
    //for(auto const & k_ : diff_seq){
      //g.add(k_ == diff_k(ctx.int_val(_index), diff_a, diff_b));
      //s.add(k_ == diff_k(ctx.int_val(_index), diff_a, diff_b));
      //_index++;
    //}
  //}

  //g.add(interpolant);
  //s.add(interpolant);

  //std::cout << s.to_smt2_decls_only() << std::endl;
  //std::cout << g << std::endl;
  ////std::cout <<  z3::tactic(ctx, "solve-eqs")(g) << std::endl;
  
   ////
  
  //return interpolant;
//}

z3::expr AXDInterpolant::liftInterpolant(z3::expr & interpolant){

  z3::expr_vector from(ctx);
  z3::expr_vector to(ctx);

  for(auto const & diff_entry : part_a.diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second.new_index_vars;
    unsigned _index = 1;
    for(auto const & k_ : diff_seq){
      if(k_.decl().name().str().rfind("fresh_", 0) == 0){
        from.push_back(k_);
        to.push_back(diff_k(ctx.int_val(_index), diff_a, diff_b));
      }
      _index++;
    }
  }

  for(auto const & diff_entry : part_b.diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second.new_index_vars;
    unsigned _index = 1;
    for(auto const & k_ : diff_seq){
      if(k_.decl().name().str().rfind("fresh_", 0) == 0){
        from.push_back(k_);
        to.push_back(diff_k(ctx.int_val(_index), diff_a, diff_b));
      }
      _index++;
    }
  }

  return interpolant.substitute(from, to);
}

// Precondition: part_a_vector and part_b_vector should be updated using
// setupPart_A_B_Vectors
void AXDInterpolant::testOutput(z3::expr const & interpolant, 
    z3::expr_vector & part_a_vector, z3::expr_vector & part_b_vector){
  std::ofstream test1_file("./output/" + m_file_name + "_test1.smt2");
  std::ofstream test2_file("./output/" + m_file_name + "_test2.smt2");

  z3::expr x = ctx.constant("x", this->array_sort);
  z3::expr y = ctx.constant("y", this->array_sort);
  z3::expr e = ctx.constant("e", this->element_sort);
  z3::expr i = ctx.constant("i", this->int_sort);
  z3::expr j = ctx.constant("j", this->int_sort);

  z3::solver test1(ctx);
#if _TEST_ORIGINAL_INPUT_
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
#if _TEST_ORIGINAL_INPUT_
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

void AXDInterpolant::z3OutputFile(){
  std::ofstream z3_file("./output/" + m_file_name + "_reduced_z3.smt2" );
  z3_file << solver.to_smt2_decls_only();

  z3_file << "(define-fun part_a () Bool (and " << std::endl;
  for(auto const & assertion : part_a.part_2)
    z3_file << assertion << std::endl;
  for(auto const & index : part_a.index_vars)
    z3_file << (index >= 0) << std::endl;
  z3_file << "))" << std::endl;

  z3_file << "(define-fun part_b () Bool (and " << std::endl;
  for(auto const & assertion : part_b.part_2)
    z3_file << assertion << std::endl;
  for(auto const & index : part_b.index_vars)
    z3_file << (index >= 0) << std::endl;
  z3_file << "))" << std::endl;

  z3_file << "(compute-interpolant (interp part_a) part_b)" << std::endl;

  system(("z3 ./output/" + m_file_name + "_reduced_z3.smt2").c_str());
}

void AXDInterpolant::mathsatOutputFile(){
  std::ofstream mathsat_file("./output/" + m_file_name + "_reduced_mathsat.smt2");
  mathsat_file << "(set-option :produce-interpolants true)" << std::endl;
  mathsat_file << solver.to_smt2_decls_only();

  mathsat_file << "(assert (! (and" << std::endl;
  for(auto const & assertion : part_a.part_2)
    mathsat_file << assertion << std::endl;
  for(auto const & index : part_a.index_vars)
    mathsat_file << (index >= 0) << std::endl;
  mathsat_file << ") :interpolation-group part_a))" << std::endl;

  mathsat_file << "(assert (! (and " << std::endl;
  for(auto const & assertion : part_b.part_2)
    mathsat_file << assertion << std::endl;
  for(auto const & index : part_b.index_vars)
    mathsat_file << (index >= 0) << std::endl;
  mathsat_file << ") :interpolation-group part_b))" << std::endl;

  mathsat_file << "(check-sat)" << std::endl;
  mathsat_file << "(get-interpolant (part_a))" << std::endl;
  mathsat_file << "(exit)" << std::endl;

  system(("mathsat ./output/" + m_file_name + "_reduced_mathsat.smt2").c_str());
}

void AXDInterpolant::directComputation(){
  z3::expr_vector part_a_vector(ctx);
  z3::expr_vector part_b_vector(ctx);
  setupPartA_B_Vectors(part_a_vector, part_b_vector);
  z3::expr interpolant = computeInterpolant(part_a_vector, part_b_vector);
  std::ofstream interpolant_file("./output/" + m_file_name + "_interpolant.smt2");
  //interpolant_file << interpolant;
  interpolant_file << liftInterpolant(interpolant);


#if _TEST_OUTPUT_
  testOutput(interpolant, part_a_vector, part_b_vector);
#endif
}

void AXDInterpolant::setupPartA_B_Vectors(z3::expr_vector & part_a_vector, z3::expr_vector & part_b_vector){
  for(auto const & assertion : part_a.part_2)
    part_a_vector.push_back(assertion);
  for(auto const & assertion : part_b.part_2)
    part_b_vector.push_back(assertion);
  for(auto const & index : part_a.index_vars)
    part_a_vector.push_back(index >= 0);
  for(auto const & index : part_b.index_vars)
    part_b_vector.push_back(index >= 0);
}
