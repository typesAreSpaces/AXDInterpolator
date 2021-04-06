#include "AXDInterpolant.h"

#define TEST_OUTPUT_CODE(PARSED_FROM_SOLVER_INTERPOLANT) \
  z3::expr_vector _part_a_vector(ctx); \
  z3::expr_vector _part_b_vector(ctx); \
  AB_VectorsSetup(_part_a_vector, part_a); \
  AB_VectorsSetup(_part_b_vector, part_b); \
  z3::expr_vector part_a_vector(ctx); \
  z3::expr_vector part_b_vector(ctx); \
  for(auto const & x : _part_a_vector) \
  part_a_vector.push_back(defineDeclarations(x)); \
  for(auto const & x : _part_b_vector) \
  part_b_vector.push_back(defineDeclarations(x)); \
  if(testOutput(PARSED_FROM_SOLVER_INTERPOLANT.assertions(), \
        part_a_vector, part_b_vector)) \
        state_output = fine; \
        else \
        state_output = notfine;

void AXDInterpolant::z3OutputFile(){
  if(!is_unsat)
    throw "Input problem is not unsatisfiable.";
  // Setup smt2 file with reduced formulas
  // in Index Theory + EUF
  system(("mkdir -p " + OUTPUT_DIR).c_str());
  std::ofstream z3_file(
      OUTPUT_DIR + "/" + m_file_name 
      + "_reduced_z3.smt2");

  z3_file 
    << "(set-option :produce-interpolants true)" 
    << std::endl;
  // Setup logic engine
  z3_file 
    << "(set-logic " 
    << ((strcmp(theory_name, "QF_TO") == 0 || 
          strcmp(theory_name, "QF_IDL") == 0) ? 
        "QF_UFIDL" : "QF_UFLIA") << ")" << std::endl;

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
  system((CURRENT_DIR 
        + "/bin/z3 " 
        + OUTPUT_DIR + "/" + m_file_name 
        + "_reduced_z3.smt2 > "
        + OUTPUT_DIR + "/temp.smt2").c_str());

  // Setup *_reduced_interpolant_z3.smt2 file
  // to parse reduced interpolant
  std::ifstream result(OUTPUT_DIR + "/temp.smt2");
  std::string line(""), interpolant_from_file("");
  // We consume two lines because
  // z3 outputs "check-sat" followed
  // a line containing "(interpolants", followed
  // by the interpolant
  std::getline(result, line);
  std::getline(result, line);

  interpolant_from_file += solver.to_smt2_decls_only();
  interpolant_from_file += "(assert (and\n";
  while(std::getline(result, line))
    interpolant_from_file += line + "\n";
  // Only one parenthesis is needed to close
  // the above since the content of (interpolant *)
  // includes an additional parenthesis
  interpolant_from_file += ")\n";
  interpolant_from_file += "(check-sat)\n";
  system(("rm -rf " + OUTPUT_DIR + "/temp.smt2").c_str());

  // Lift interpolant to MaxDiff(Index Theory)
  z3::solver z3_interpolant_parser(ctx);
  z3_interpolant_parser.from_string(interpolant_from_file.c_str());

  is_interpolant_computed = true;
  current_interpolant = liftInterpolant(
      z3_interpolant_parser.assertions());

#if _TEST_OUTPUT_
  TEST_OUTPUT_CODE(z3_interpolant_parser);
#endif
}

void AXDInterpolant::mathsatOutputFile(){
  if(!is_unsat)
    throw "Input problem is not unsatisfiable.";
  // Setup smt2 file with reduced formulas
  // in Index Theory + EUF
  system(("mkdir -p " + OUTPUT_DIR).c_str());
  std::ofstream mathsat_file(
      OUTPUT_DIR + "/" + m_file_name 
      + "_reduced_mathsat.smt2");

  mathsat_file 
    << "(set-option :produce-interpolants true)" 
    << std::endl;
  // Setup logic engine
  mathsat_file 
    << "(set-logic " 
    << ((strcmp(theory_name, "QF_TO") == 0 || 
          strcmp(theory_name, "QF_IDL") == 0) ? 
        "QF_UFIDL" : "QF_UFLIA") << ")" << std::endl;

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
  system((CURRENT_DIR 
        + "/bin/mathsat " 
        + OUTPUT_DIR  + "/" + m_file_name 
        + "_reduced_mathsat.smt2 > " 
        + OUTPUT_DIR + "/temp.smt2").c_str());

  // Setup *_reduced_interpolant_mathsat.smt2 file
  // to parse reduced interpolant
  std::ifstream result(OUTPUT_DIR + "/temp.smt2");
  std::string line(""), interpolant_from_file("");
  // We consume one line because
  // mathsat outputs "check-sat" followed
  // by the interpolant
  std::getline(result, line);
  interpolant_from_file += solver.to_smt2_decls_only();
  interpolant_from_file += "(assert \n";
  while(std::getline(result, line))
    interpolant_from_file += line + "\n";
  interpolant_from_file += ")\n";
  interpolant_from_file += "(check-sat)\n";
  system(("rm -rf " + OUTPUT_DIR + "/temp.smt2").c_str());

  // Lift interpolant to MaxDiff(Index Theory)
  z3::solver mathsat_interpolant_parser(ctx);
  mathsat_interpolant_parser.from_string(interpolant_from_file.c_str());

  is_interpolant_computed = true;
  current_interpolant = liftInterpolant(
      mathsat_interpolant_parser.assertions());

#if _TEST_OUTPUT_
  TEST_OUTPUT_CODE(mathsat_interpolant_parser);
#endif
}

void AXDInterpolant::smtInterpolOutputFile(){
  if(!is_unsat)
    throw "Input problem is not unsatisfiable.";
  // Setup smt2 file with reduced formulas
  // in Index Theory + EUF
  system(("mkdir -p " + OUTPUT_DIR).c_str());
  std::ofstream smtinterpol_file(
      OUTPUT_DIR + "/" + m_file_name 
      + "_reduced_smtinterpol.smt2");

  smtinterpol_file 
    << "(set-option :print-success false)\n" 
    << "(set-option :produce-interpolants true)" 
    << std::endl;
  // Setup logic engine
  smtinterpol_file 
    << "(set-logic " 
    << ((strcmp(theory_name, "QF_TO") == 0 || 
          strcmp(theory_name, "QF_IDL") == 0) ? 
        "QF_UFIDL" : "QF_UFLIA") << ")" << std::endl;

  smtinterpol_file << defineDeclarations(solver.to_smt2_decls_only());
  smtinterpol_file << "(assert (! (and" << std::endl;
  SmtSolverOutStreamSetup(smtinterpol_file, part_a);
  smtinterpol_file << ") :named part_a))" << std::endl;
  smtinterpol_file << "(assert (! (and" << std::endl;
  SmtSolverOutStreamSetup(smtinterpol_file, part_b);
  smtinterpol_file << ") :named part_b))" << std::endl;
  smtinterpol_file << "(check-sat)" << std::endl;
  smtinterpol_file << "(get-interpolants part_a part_b)" << std::endl;

  // Obtain reduced interpolant in temp.smt2
  system(("java -jar " + CURRENT_DIR 
        + "/bin/smtinterpol-2.5-663-gf15aa217.jar -w " 
        + OUTPUT_DIR + "/" + m_file_name 
        + "_reduced_smtinterpol.smt2 > "
        + OUTPUT_DIR + "/temp.smt2").c_str());

  // Setup *_reduced_interpolant_smtinterpol.smt2 file
  // to parse reduced interpolant
  std::ifstream result(OUTPUT_DIR + "/temp.smt2");
  std::string line(""), interpolant_from_file("");
  // We consume one line because
  // mathsat outputs "check-sat" followed
  // by the interpolant
  std::getline(result, line);
  interpolant_from_file += solver.to_smt2_decls_only();
  interpolant_from_file += "(assert \n";
  // SMTINTERPOL returns a single
  // line containing a list
  // of sequence interpolants
  // Thus, we remove the initial and
  // last character which are '(' and ')' 
  // respectively
  std::getline(result, line);
  interpolant_from_file += line.erase(0, 1) + "\n";
  while(std::getline(result, line))
    interpolant_from_file += line + "\n";
  // The following removes the last parenthesis 
  // and the extra '\n'
  interpolant_from_file.erase(interpolant_from_file.size() - 2, 2);
  // Only one parenthesis is needed to close
  // the above since the content of (interpolant *)
  // includes an additional parenthesis
  interpolant_from_file += ")\n";
  interpolant_from_file += "(check-sat)\n";
  system(("rm -rf " + OUTPUT_DIR + "/temp.smt2").c_str());

  // Lift interpolant to MaxDiff(Index Theory)
  z3::solver smtinterpol_interpolant_parser(ctx);
  smtinterpol_interpolant_parser.from_string(interpolant_from_file.c_str());

  is_interpolant_computed = true;
  current_interpolant = liftInterpolant(
      smtinterpol_interpolant_parser.assertions());

#if _TEST_OUTPUT_
  TEST_OUTPUT_CODE(smtinterpol_interpolant_parser);
#endif
}
