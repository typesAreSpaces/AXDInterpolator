#include "AXDInterpolant.h"
#include "z3++.h"

AXDInterpolant::AXDInterpolant(
    z3::context & ctx, 
    char const * file_name,
    char const * _theory_name,
    unsigned allowed_attempts) : 
  Preprocessor(ctx, file_name),
  solver(ctx), 
  part_a(
      z3::mk_and(input_part_a), 
      part_a_index_vars, 
      part_a_array_vars, 
      _theory_name),
  part_b(
      z3::mk_and(input_part_b), 
      part_b_index_vars, 
      part_b_array_vars,
      _theory_name),
  m_file_name(std::string(file_name)),
  num_attempts(allowed_attempts),
  is_interpolant_computed(false), is_unsat(false),
  current_interpolant(ctx.bool_val(true)),
  theory_name(_theory_name)
{
  m_out
    << "Solving file " << m_file_name 
    << std::endl;
  m_file_name = m_file_name
    .substr(0, m_file_name.find_last_of("."))
    .substr(m_file_name.find_last_of("\\/") + 1);

  loop();
}

void AXDInterpolant::loop(){
#if _DEBUG_AXD_INTER_
  unsigned const constant_allowed_attempts = allowed_attempts;
#endif
  CircularPairIterator search_common_pair(common_array_vars);

  while(--num_attempts){
    solver.push();
    // The following uses a z3::solver 
    // to check if part_a \land part_b
    // is unsat
    SmtSolverSetup(solver, part_a);
    SmtSolverSetup(solver, part_b);
    
    if(solver.check() == z3::unsat){
      is_unsat = true;
#if _DEBUG_AXD_INTER_
      m_out << "Unsat after " 
        << constant_allowed_attempts - allowed_attempts 
        << " iterations" << std::endl;
#endif
      return;
    }

    solver.pop();
#if _DEBUG_AXD_INTER_ 
    m_out << "A-part part 2: " << part_a.part_2 << std::endl;
    m_out << "B-part part 2: " << part_b.part_2 << std::endl;
#endif
    // Find pair of common array variables
    auto const & common_pair = *search_common_pair;

    unsigned part_a_dim = part_a.diff_map.size_of_entry(common_pair),
             part_b_dim = part_b.diff_map.size_of_entry(common_pair),
             min_dim = std::min(part_a_dim, part_b_dim);
    auto const & _new_index = fresh_index_constant();
    part_a.updateSaturation(common_pair, _new_index, min_dim);
    part_b.updateSaturation(common_pair, _new_index, min_dim);

    search_common_pair.next();
  }
}

// Precondition: 
// part_a_vector and part_b_vector 
// should have been updated using
// AB_VectorsSetup
z3::expr_vector AXDInterpolant::computeReducedInterpolant(
    z3::expr_vector const & part_a_vector, 
    z3::expr_vector const & part_b_vector){

  z3::expr marked_formula = 
    interpolant(z3::mk_and(part_a_vector)) 
    && z3::mk_and(part_b_vector);

  z3::expr_vector interpolant = ctx.get_interpolant(
      solver.proof(),
      marked_formula,
      z3::params(ctx));

  return interpolant;
}

z3::expr AXDInterpolant::liftInterpolant(
    z3::expr_vector const & interpolant){

  z3::expr_vector _interpolant(ctx);

  // TODO: this might need some optimization
  if(strcmp(theory_name, "QF_TO") == 0)
    for(auto const & x : interpolant)
      //_interpolant.push_back(x);
      _interpolant.push_back(QF_TO_Rewriter(x));
  else
    for(auto const & x : interpolant)
      _interpolant.push_back(x);

  z3::expr_vector from(ctx);
  z3::expr_vector to(ctx);

  liftInterpolantDiffSubs(from, to, part_a);
  liftInterpolantDiffSubs(from, to, part_b);

  return z3::mk_and(_interpolant).substitute(from, to);
}

void AXDInterpolant::liftInterpolantDiffSubs(
    z3::expr_vector & from,
    z3::expr_vector & to,
    StandardInput const & input
    ){
  for(auto const & diff_entry : input.diff_map.m_map){
    auto const & diff_a   = diff_entry.first.first;
    auto const & diff_b   = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned diff_iteration = 1;
    for(auto const & k_ : diff_seq){
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        from.push_back(k_);
        to.push_back(diff_k(ctx.int_val(diff_iteration++), diff_a, diff_b));
      }
    }
  }
}

void AXDInterpolant::z3OutputFile(){
  if(!is_unsat)
    return;
  // Setup smt2 file with reduced formulas
  // in Index Theory + EUF
  system(("mkdir -p " + OUTPUT_DIR).c_str());
  std::ofstream z3_file(
      OUTPUT_DIR + "/" + m_file_name + "_reduced_z3.smt2");
  z3_file << "(set-option :produce-interpolants true)" << std::endl;
  z3_file << defineDeclarations(solver.to_smt2_decls_only());
  z3_file << "(assert (! (and" << std::endl;
  SmtSolverOutStreamSetup(z3_file, part_a);
  z3_file << ") :named part_a))" << std::endl;
  z3_file << "(assert (! (and" << std::endl;
  SmtSolverOutStreamSetup(z3_file, part_b);
  z3_file << ") :named part_b))" << std::endl;
  z3_file << "(check-sat)" << std::endl;
  z3_file << "(get-interpolant part_a part_b)" << std::endl;

  // Obtain reduced interpolant in temp.smt2
  system(("./bin/z3 " + OUTPUT_DIR 
        + "/" + m_file_name 
        + "_reduced_z3.smt2 > "
        + OUTPUT_DIR + "/temp.smt2").c_str());

  // Setup *_reduced_interpolant_z3.smt2 file
  // to parse reduced interpolant
  std::ifstream result(OUTPUT_DIR + "/temp.smt2");
  std::string line;
  // We consume two lines because
  // z3 outputs "check-sat" followed
  // a line containing "(interpolants", followed
  // by the interpolant
  std::getline(result, line);
  std::getline(result, line);
  std::ofstream z3_reduced_interpolant(OUTPUT_DIR + "/" 
      + m_file_name + "_reduced_interpolant_z3.smt2" );
  // It is necessary to include declarations
  // in *_reduced_mathsat_lifted.smt2 because the
  // file will be parsed again
  z3_reduced_interpolant << solver.to_smt2_decls_only();
  z3_reduced_interpolant << "(assert (and" << std::endl;
  while(std::getline(result, line))
    z3_reduced_interpolant << line << std::endl;
  // Only one parenthesis is needed to close
  // the above since the content of (interpolant *)
  // includes an additional parenthesis
  z3_reduced_interpolant << ")" << std::endl;
  z3_reduced_interpolant << "(check-sat)" << std::endl;
  system(("rm -rf " + OUTPUT_DIR + "/temp.smt2").c_str());

  // Lift interpolant to MaxDiff(Index Theory)
  z3::solver z3_parser(ctx);
  z3_parser.from_file((OUTPUT_DIR + "/" + m_file_name 
        + "_reduced_interpolant_z3.smt2").c_str());

  is_interpolant_computed = true;
  current_interpolant = liftInterpolant(
      z3_parser.assertions())
#if _SIMPLIFY_OUTPUT
    .simplify()
#endif
    ;

#if _TEST_OUTPUT_
  z3::expr_vector _part_a_vector(ctx);
  z3::expr_vector _part_b_vector(ctx);
  AB_VectorsSetup(_part_a_vector, part_a);
  AB_VectorsSetup(_part_b_vector, part_b);

  z3::expr_vector part_a_vector(ctx);
  z3::expr_vector part_b_vector(ctx);
  for(unsigned i = 0; i < _part_a_vector.size(); i++)
    part_a_vector.push_back(defineDeclarations(_part_a_vector[i]));
  for(unsigned i = 0; i < _part_b_vector.size(); i++)
    part_b_vector.push_back(defineDeclarations(_part_b_vector[i]));

  if(testOutput(
        z3_parser.assertions(), 
        part_a_vector, part_b_vector)
    )
    std::cout 
      << "Successful Test: formula is an interpolant" 
      << std::endl;
  else
    std::cout 
      << "Unsuccessful Test: formula isn't an interpolant" 
      << std::endl;
#endif

  system(("rm -rf " + OUTPUT_DIR + "/" + m_file_name 
        + "_reduced_interpolant_z3.smt2").c_str());
}

void AXDInterpolant::mathsatOutputFile(){
  if(!is_unsat)
    return;
  // Setup smt2 file with reduced formulas
  // in Index Theory + EUF
  system(("mkdir -p " + OUTPUT_DIR).c_str());
  std::ofstream mathsat_file(OUTPUT_DIR + "/" 
      + m_file_name + "_reduced_mathsat.smt2");
  mathsat_file << "(set-option :produce-interpolants true)" << std::endl;
  mathsat_file << defineDeclarations(solver.to_smt2_decls_only());
  mathsat_file << "(assert (! (and" << std::endl;
  SmtSolverOutStreamSetup(mathsat_file, part_a);
  mathsat_file << ") :interpolation-group part_a))" << std::endl;
  mathsat_file << "(assert (! (and " << std::endl;
  SmtSolverOutStreamSetup(mathsat_file, part_b);
  mathsat_file << ") :interpolation-group part_b))" << std::endl;
  mathsat_file << "(check-sat)" << std::endl;
  mathsat_file << "(get-interpolant (part_a))" << std::endl;
  mathsat_file << "(exit)" << std::endl;

  // Obtain reduced interpolant in temp.smt2
  system(("./bin/mathsat " + OUTPUT_DIR 
        + "/" + m_file_name 
        + "_reduced_mathsat.smt2 > " + OUTPUT_DIR 
        + "/temp.smt2").c_str());

  // Setup *_reduced_interpolant_mathsat.smt2 file
  // to parse reduced interpolant
  std::ifstream result(OUTPUT_DIR + "/temp.smt2");
  std::string line;
  // We consume one line because
  // mathsat outputs "check-sat" followed
  // by the interpolant
  std::getline(result, line);
  std::ofstream mathsat_reduced_interpolant(
      OUTPUT_DIR + "/" 
      + m_file_name + "_reduced_interpolant_mathsat.smt2" );
  // It is necessary to include declarations
  // in *_reduced_mathsat_lifted.smt2 because the
  // file will be parsed again
  mathsat_reduced_interpolant << solver.to_smt2_decls_only();
  mathsat_reduced_interpolant << "(assert " << std::endl;
  while(std::getline(result, line))
    mathsat_reduced_interpolant << line << std::endl;
  mathsat_reduced_interpolant << ")" << std::endl;
  mathsat_reduced_interpolant << "(check-sat)" << std::endl;
  system(("rm -rf " + OUTPUT_DIR + "/temp.smt2").c_str());

  // Lift interpolant to MaxDiff(Index Theory)
  z3::solver mathsat_parser(ctx);
  mathsat_parser.from_file((OUTPUT_DIR + "/" + m_file_name 
        + "_reduced_interpolant_mathsat.smt2").c_str());

  is_interpolant_computed = true;
  current_interpolant = liftInterpolant(
      mathsat_parser.assertions())
#if _SIMPLIFY_OUTPUT
    .simplify()
#endif
    ;

#if _TEST_OUTPUT_
  z3::expr_vector _part_a_vector(ctx);
  z3::expr_vector _part_b_vector(ctx);
  AB_VectorsSetup(_part_a_vector, part_a);
  AB_VectorsSetup(_part_b_vector, part_b);

  z3::expr_vector part_a_vector(ctx);
  z3::expr_vector part_b_vector(ctx);
  for(unsigned i = 0; i < _part_a_vector.size(); i++)
    part_a_vector.push_back(defineDeclarations(_part_a_vector[i]));
  for(unsigned i = 0; i < _part_b_vector.size(); i++)
    part_b_vector.push_back(defineDeclarations(_part_b_vector[i]));

  if(testOutput(
        mathsat_parser.assertions(), 
        part_a_vector, part_b_vector)
    )
    std::cout 
      << "Successful Test: formula is an interpolant" 
      << std::endl;
  else
    std::cout 
      << "Unsuccessful Test: formula isn't an interpolant" 
      << std::endl;
#endif

  system(("rm -rf " + OUTPUT_DIR + "/" + m_file_name 
        + "_reduced_interpolant_mathsat.smt2").c_str());
}

void AXDInterpolant::directComputation(){
  if(!is_unsat)
    return;
  z3::expr_vector _part_a_vector(ctx);
  z3::expr_vector _part_b_vector(ctx);
  AB_VectorsSetup(_part_a_vector, part_a);
  AB_VectorsSetup(_part_b_vector, part_b);

  z3::expr_vector part_a_vector(ctx);
  z3::expr_vector part_b_vector(ctx);
  for(unsigned i = 0; i < _part_a_vector.size(); i++)
    part_a_vector.push_back(defineDeclarations(_part_a_vector[i]));
  for(unsigned i = 0; i < _part_b_vector.size(); i++)
    part_b_vector.push_back(defineDeclarations(_part_b_vector[i]));

  z3::expr_vector reduced_interpolant = computeReducedInterpolant(
      part_a_vector, part_b_vector);

  is_interpolant_computed = true;
  current_interpolant = liftInterpolant(reduced_interpolant)
#if _SIMPLIFY_OUTPUT
    .simplify()
#endif
    ;

#if _TEST_OUTPUT_
  if(testOutput(
        reduced_interpolant, 
        part_a_vector, part_b_vector)
    )
    std::cout 
      << "Successful Test: formula is an interpolant" 
      << std::endl;
  else
    std::cout 
      << "Unsuccessful Test: formula isn't an interpolant" 
      << std::endl;
#endif
}

std::ostream & operator << (std::ostream & os, 
    AXDInterpolant const & axd){

  if(!axd.num_attempts)
    return os 
      << "Unknown: Input formula might be satisfiable or unsatisfiable."
      << std::endl;
  if(!axd.is_unsat)
    return os 
      << "Satisfiable: No interpolant available.";
  if(axd.is_interpolant_computed)
    return (os 
        << "Unsatisfiable: " << std::endl
        << axd.current_interpolant);
  else
    return (os << 
        "Interpolant hasn't been computed.\n"
        "Use .z3OutputFile or .mathsatOutputFile\n"
        "or .directComputation on a AXDInterpolant\n" 
        "object to obtain an interpolant.");
}
