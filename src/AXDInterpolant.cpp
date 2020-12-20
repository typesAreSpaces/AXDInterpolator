#include "AXDInterpolant.h"
#include "z3++.h"

AXDInterpolant::AXDInterpolant(
    z3::context & ctx, 
    char const * file_name,
    char const * theory,
    unsigned allowed_attempts) : 
  Preprocessor(ctx, file_name),
  solver(ctx), 
  part_a(
      z3::mk_and(input_part_a), 
      part_a_index_vars, 
      part_a_array_vars, 
      theory),
  part_b(
      z3::mk_and(input_part_b), 
      part_b_index_vars, 
      part_b_array_vars,
      theory),
  m_file_name(std::string(file_name)),
  num_attempts(allowed_attempts),
  is_interpolant_computed(false), is_unsat(false),
  current_interpolant(ctx.bool_val(true))
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

// Precondition: part_a_vector and part_b_vector should be updated using
// setupPartA_B_Vectors
void AXDInterpolant::testOutput(
    z3::expr const & interpolant, 
    z3::expr_vector & part_a_vector, z3::expr_vector & part_b_vector){
  system(("mkdir -p " + OUTPUT_DIR).c_str());
  std::ofstream test1_file(
      OUTPUT_DIR + "/" + m_file_name + "_test1.smt2");
  std::ofstream test2_file(
      OUTPUT_DIR + "/" + m_file_name + "_test2.smt2");

  z3::expr x = ctx.constant("x", this->array_sort);
  z3::expr y = ctx.constant("y", this->array_sort);
  z3::expr e = ctx.constant("e", this->element_sort);
  z3::expr i = ctx.constant("i", this->int_sort);
  z3::expr j = ctx.constant("j", this->int_sort);

  z3::solver test1(ctx);
#if _TEST_ORIGINAL_INPUT_
  test1.add(not(z3::implies(z3::mk_and(input_part_a), interpolant)));
  // Adding axiomatization
  test1.add(forall(y, i, e, rd(wr(y, i, e), i) == e));
  test1.add(forall(y, i , j, e, z3::implies(i != j, rd(wr(y, i, e), j) == rd(y, j))));
  test1.add(forall(x, y, z3::implies(x != y, rd(x, diff(x, y)) != rd(y, diff(x, y)))));
  test1.add(forall(x, y, i, z3::implies(i > diff(x, y), rd(x, i) == rd(y, i))));
  test1.add(forall(x, diff(x, x) == 0));
  // Adding equations of new symbols
  for(auto const & diff_entry : part_a.diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned _index = 1;
    for(auto const & k_ : diff_seq){
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        test1.add(k_ == diff_k(ctx.int_val(_index), diff_a, diff_b));
      }
      _index++;
    }
  }

  for(auto const & diff_entry : part_b.diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned _index = 1;
    for(auto const & k_ : diff_seq){
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        test1.add(k_ == diff_k(ctx.int_val(_index), diff_a, diff_b));
      }
      _index++;
    }
  }
#else
  test1.add(not(z3::implies(z3::mk_and(part_a_vector), interpolant)));
#endif
  test1_file << test1.to_smt2();

  z3::solver test2(ctx);
#if _TEST_ORIGINAL_INPUT_
  test2.add(z3::mk_and(input_part_b) && interpolant);
  // Adding axiomatization
  test2.add(forall(y, i, e, rd(wr(y, i, e), i) == e));
  test2.add(forall(y, i , j, e, z3::implies(i != j, rd(wr(y, i, e), j) == rd(y, j))));
  test2.add(forall(x, y, z3::implies(x != y, rd(x, diff(x, y)) != rd(y, diff(x, y)))));
  test2.add(forall(x, y, i, z3::implies(i > diff(x, y), rd(x, i) == rd(y, i))));
  test2.add(forall(x, diff(x, x) == 0));
  // Adding equations of new symbols
  for(auto const & diff_entry : part_a.diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned _index = 1;
    for(auto const & k_ : diff_seq){
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        test2.add(k_ == diff_k(ctx.int_val(_index), diff_a, diff_b));
      }
      _index++;
    }
  }

  for(auto const & diff_entry : part_b.diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned _index = 1;
    for(auto const & k_ : diff_seq){
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        test2.add(k_ == diff_k(ctx.int_val(_index), diff_a, diff_b));
      }
      _index++;
    }
  }
#else
  test2.add(mk_and(part_b_vector) && interpolant);
#endif
  test2_file << test2.to_smt2();
}

void AXDInterpolant::SmtSolverSetup(
    z3::solver & solver, 
    StandardInput const & side_part){

  for(auto const & assertion : side_part.part_2)
    solver.add(assertion);

  // axiom_8
  side_part.instantiate(solver, (z3::expr &) side_part.axiom_8);

  // axiom_9
  side_part.instantiate(solver, (z3::expr &) side_part.axiom_9);

  // axiom_11_2 [WriteVector: a, b, i],
  for(auto const & _4tuple 
      : side_part.write_vector.m_vector){
    auto const & a = std::get<0>(_4tuple);
    auto const & b = std::get<1>(_4tuple);
    auto const & i = std::get<2>(_4tuple);

    z3::expr axiom_11_2 = 
      z3::implies(
          side_part.index_var != i,
          rd(a, side_part.index_var) == rd(b, side_part.index_var)
          );
    side_part.instantiate(solver, axiom_11_2);
  }

  // axiom_18 (axiom_12_2) [DiffMap: a, b, l]
  for(auto const & diff_entry 
      : side_part.diff_map.m_map){
    auto const & a = diff_entry.first.first;
    auto const & b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;

    if(diff_seq.size() == 0)
      continue;

    unsigned last_one = diff_seq.size() - 1;
    z3::expr_vector disj_equalities(ctx);
    disj_equalities.push_back(
        rd(a, side_part.index_var) 
        == rd(b, side_part.index_var));
    for(unsigned i = 0; i < last_one; i++)
      disj_equalities.push_back(
          side_part.index_var == diff_seq[i]);

    z3::expr axiom_18 = z3::implies(
        side_part.index_var > diff_seq[last_one], 
        z3::mk_or(disj_equalities));
    side_part.instantiate(solver, axiom_18);
  }
}

void AXDInterpolant::SmtSolverOutStreamSetup(
    std::ostream & out, 
    StandardInput const & side_part){
  for(auto const & assertion : side_part.part_2)
    out << assertion << std::endl;

  // axiom_8
  side_part.instantiate(out, (z3::expr &) side_part.axiom_8);

  // axiom_9
  side_part.instantiate(out, (z3::expr &) side_part.axiom_9);

  // axiom_11_2 [WriteVector: a, b, i],
  for(auto const & _4tuple 
      : side_part.write_vector.m_vector){
    auto const & a = std::get<0>(_4tuple);
    auto const & b = std::get<1>(_4tuple);
    auto const & i = std::get<2>(_4tuple);

    z3::expr axiom_11_2 = 
      z3::implies(
          side_part.index_var != i,
          rd(a, side_part.index_var) == rd(b, side_part.index_var)
          );
    side_part.instantiate(out, axiom_11_2);
  }

  // axiom_18 (axiom_12_2) [DiffMap: a, b, l]
  for(auto const & diff_entry 
      : side_part.diff_map.m_map){
    auto const & a = diff_entry.first.first;
    auto const & b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;

    if(diff_seq.size() == 0)
      continue;

    unsigned last_one = diff_seq.size() - 1;
    z3::expr_vector disj_equalities(ctx);
    disj_equalities.push_back(
        rd(a, side_part.index_var) 
        == rd(b, side_part.index_var));
    for(unsigned i = 0; i < last_one; i++)
      disj_equalities.push_back(
          side_part.index_var == diff_seq[i]);

    z3::expr axiom_18 = z3::implies(
        side_part.index_var > diff_seq[last_one], 
        z3::mk_or(disj_equalities));
    side_part.instantiate(out, axiom_18);
  }
}

void AXDInterpolant::setupPartA_B_Vectors(
    z3::expr_vector & part_a_vector, 
    z3::expr_vector & part_b_vector){
  for(auto const & assertion : part_a.part_2)
    part_a_vector.push_back(assertion);
  for(auto const & assertion : part_b.part_2)
    part_b_vector.push_back(assertion);
  for(auto const & index : part_a.index_vars)
    part_a_vector.push_back(index >= 0);
  for(auto const & index : part_b.index_vars)
    part_b_vector.push_back(index >= 0);
}

// Precondition: part_a_vector and part_b_vector should be updated using
// setupPartA_B_Vectors
z3::expr AXDInterpolant::computeReducedInterpolant(
    z3::expr_vector const & part_a_vector, 
    z3::expr_vector const & part_b_vector){
  z3::expr marked_formula = 
    interpolant(z3::mk_and(part_a_vector)) 
    && z3::mk_and(part_b_vector);
  z3::expr_vector interpolant = ctx.get_interpolant(
      solver.proof(),
      marked_formula,
      z3::params(ctx));

  return interpolant[0];
}

z3::expr AXDInterpolant::liftInterpolant(
    z3::expr const & interpolant){

  z3::expr_vector from(ctx);
  z3::expr_vector to(ctx);

  for(auto const & diff_entry : part_a.diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned _index = 1;
    for(auto const & k_ : diff_seq){
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        from.push_back(k_);
        to.push_back(diff_k(ctx.int_val(_index), diff_a, diff_b));
      }
      _index++;
    }
  }

  for(auto const & diff_entry : part_b.diff_map.m_map){
    auto const & diff_a   = diff_entry.first.first;
    auto const & diff_b   = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;
    unsigned _index = 1;
    for(auto const & k_ : diff_seq){
      if(func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0){
        from.push_back(k_);
        to.push_back(diff_k(ctx.int_val(_index), diff_a, diff_b));
      }
      _index++;
    }
  }

  return ((z3::expr)interpolant).substitute(from, to);
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
      z3::mk_and(z3_parser.assertions()))
    .simplify();
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
      z3::mk_and(mathsat_parser.assertions()))
    .simplify();
  system(("rm -rf " + OUTPUT_DIR + "/" + m_file_name 
        + "_reduced_interpolant_mathsat.smt2").c_str());
}

void AXDInterpolant::directComputation(){
  if(!is_unsat)
    return;
  z3::expr_vector _part_a_vector(ctx);
  z3::expr_vector _part_b_vector(ctx);
  setupPartA_B_Vectors(_part_a_vector, _part_b_vector);

  z3::expr_vector part_a_vector(ctx);
  z3::expr_vector part_b_vector(ctx);
  for(unsigned i = 0; i < _part_a_vector.size(); i++)
    part_a_vector.push_back(defineDeclarations(_part_a_vector[i]));
  for(unsigned i = 0; i < _part_b_vector.size(); i++)
    part_b_vector.push_back(defineDeclarations(_part_b_vector[i]));

  z3::expr reduced_interpolant = computeReducedInterpolant(
      part_a_vector, part_b_vector);
  is_interpolant_computed = true;
  current_interpolant = liftInterpolant(reduced_interpolant).simplify();
#if _TEST_OUTPUT_
  testOutput(reduced_interpolant, part_a_vector, part_b_vector);
#endif
}

std::string AXDInterpolant::defineDeclarations(std::string decls) const {
  std::string pred_decl = "(declare-fun pred (Int) Int)";
  std::string succ_decl = "(declare-fun succ (Int) Int)";
  std::string neg_decl  = "(declare-fun neg (Int) Int)";
  std::string add_decl  = "(declare-fun add (Int Int) Int)";
  std::string result = decls;

  auto position = decls.find(pred_decl);
  if(position != std::string::npos)
    result = result.replace(position, pred_decl.size(), "(define-fun pred ((x Int)) Int (- x 1))");

  position = result.find(succ_decl);
  if(position != std::string::npos)
    result = result.replace(position, succ_decl.size(), "(define-fun succ ((x Int)) Int (+ x 1))");

  position = result.find(neg_decl);
  if(position != std::string::npos)
    result = result.replace(position, neg_decl.size(), "(define-fun neg ((x Int)) Int (- x))");

  position = result.find(add_decl);
  if(position != std::string::npos)
    result = result.replace(position, add_decl.size(), "(define-fun add ((x Int) (y Int)) Int (+ x y))");

  return result;
}

z3::expr AXDInterpolant::defineDeclarations(z3::expr const & e) const {
  if(e.is_app()){
    if(e.num_args() > 0){
      auto const & func_name = func_name(e);
      if(func_name == "pred")
        return (defineDeclarations(e.arg(0)) - 1);
      if(func_name == "succ")
        return (defineDeclarations(e.arg(0)) + 1);
      if(func_name == "neg")
        return (-defineDeclarations(e.arg(0)));
      if(func_name == "add")
        return (defineDeclarations(e.arg(0)) 
            + defineDeclarations(e.arg(1)));
      z3::func_decl f_name = e.decl();
      z3::expr_vector args(ctx);
      for(unsigned i = 0; i < e.num_args(); ++i)
        args.push_back(defineDeclarations(e.arg(i)));
      return f_name(args);
    }
    return e;
  }
  throw "Problem @ "
    "AXDInterpolant::defineDeclarations"
    "Not an application";
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
