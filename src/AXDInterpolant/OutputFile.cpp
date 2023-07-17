#include "AXDInterpolant.h"

#define TEST_OUTPUT_CODE(PARSED_FROM_SOLVER_INTERPOLANT)                       \
  z3::expr_vector _part_a_vector(sig.ctx);                                     \
  z3::expr_vector _part_b_vector(sig.ctx);                                     \
  AB_VectorsSetup(_part_a_vector, part_a);                                     \
  AB_VectorsSetup(_part_b_vector, part_b);                                     \
  if (testOutput(PARSED_FROM_SOLVER_INTERPOLANT.assertions(), _part_a_vector,  \
		 _part_b_vector))                                              \
    state_output = fine;                                                       \
  else                                                                         \
    state_output = notfine;

void axdinterpolator::AXDInterpolant::z3OutputFile() {
  if (!is_unsat)
    throw "Input problem "
	  "is not unsatisfiable.";
  // Setup smt2 file with reduced formulas
  // in Index Theory + EUF
  system(("mkdir -p " + OUTPUT_DIR).c_str());
  std::ofstream z3_file(OUTPUT_DIR + "/" + m_file_name + "_reduced_z3.smt2");

  z3_file << "(set-option :produce-interpolants true)" << std::endl;
  // Setup logic engine
  z3_file << "(set-logic "
	  << ((sig.is_QF_TO() || sig.is_QF_IDL()) ? "QF_UFIDL" : "QF_UFLIA")
	  << ")" << std::endl;
  //"QF_UFLIA" : "QF_UFLIA") << ")" << std::endl;

  z3_file << solver.to_smt2_decls_only();
  z3_file << "(assert (! (and" << std::endl;
  // This true assertion prevents
  // an unary application of and
  z3_file << "true" << std::endl;
  SmtSolverOutStreamSetup(z3_file, instantiated_part_a);
  z3_file << ") :named part_a))" << std::endl;
  z3_file << "(assert (! (and" << std::endl;
  // This true assertion prevents
  // an unary application of and
  z3_file << "true" << std::endl;
  SmtSolverOutStreamSetup(z3_file, instantiated_part_b);
  z3_file << ") :named part_b))" << std::endl;
  z3_file << "(check-sat)" << std::endl;
  z3_file << "(get-interpolant part_a part_b)" << std::endl;

  // Obtain reduced interpolant in <m_file_name>temp.smt2
  system((CURRENT_DIR + "/bin/z3 " + OUTPUT_DIR + "/" + m_file_name +
	  "_reduced_z3.smt2 > " + OUTPUT_DIR + "/" + m_file_name + "temp.smt2")
	     .c_str());

  // Setup *_reduced_interpolant_z3.smt2 file
  // to parse reduced interpolant
  std::ifstream result(OUTPUT_DIR + "/" + m_file_name + "temp.smt2");
  std::string line(""), interpolant_from_file("");
  // We consume two lines because
  // z3 outputs "check-sat" followed
  // a line containing "(interpolants", followed
  // by the interpolant
  std::getline(result, line);
  std::getline(result, line);

  interpolant_from_file += solver.to_smt2_decls_only();
  interpolant_from_file += "(assert (and\n";
  while (std::getline(result, line)) {
    is_valid_result = true;
    interpolant_from_file += line + "\n";
  }
  // Only one parenthesis is needed to close
  // the above since the content of (interpolant *)
  // includes an additional parenthesis
  interpolant_from_file += ")\n";
  interpolant_from_file += "(check-sat)\n";

  system(("rm -rf " + OUTPUT_DIR + "/" + m_file_name + "temp.smt2").c_str());

  if (is_valid_result) {
    // Lift interpolant to MaxDiff(Index Theory)
    z3::solver z3_interpolant_parser(sig.ctx);
    z3_interpolant_parser.from_string(interpolant_from_file.c_str());

    is_interpolant_computed = true;
    current_interpolant = liftInterpolant(z3_interpolant_parser.assertions());

#if _TEST_OUTPUT_
    TEST_OUTPUT_CODE(z3_interpolant_parser);
#endif
  }
}

void axdinterpolator::AXDInterpolant::mathsatOutputFile() {
  if (!is_unsat)
    throw "Input problem is not unsatisfiable.";
  // Setup smt2 file with reduced formulas
  // in Index Theory + EUF
  system(("mkdir -p " + OUTPUT_DIR).c_str());
  std::ofstream mathsat_file(OUTPUT_DIR + "/" + m_file_name +
			     "_reduced_mathsat.smt2");

  mathsat_file << "(set-option :produce-interpolants true)" << std::endl;
  // Setup logic engine
  mathsat_file << "(set-logic "
	       << ((sig.is_QF_TO() || sig.is_QF_IDL()) ? "QF_UFIDL"
						       : "QF_UFLIA")
	       << ")" << std::endl;

  mathsat_file << solver.to_smt2_decls_only();
  mathsat_file << "(assert (! (and" << std::endl;
  // This true assertion prevents
  // an unary application of and
  mathsat_file << "true" << std::endl;
  SmtSolverOutStreamSetup(mathsat_file, part_a);
  mathsat_file << ") :interpolation-group part_a))" << std::endl;
  mathsat_file << "(assert (! (and " << std::endl;
  // This true assertion prevents
  // an unary application of and
  mathsat_file << "true" << std::endl;
  SmtSolverOutStreamSetup(mathsat_file, part_b);
  mathsat_file << ") :interpolation-group part_b))" << std::endl;
  mathsat_file << "(check-sat)" << std::endl;
  mathsat_file << "(get-interpolant (part_a))" << std::endl;
  mathsat_file << "(exit)" << std::endl;

  // Obtain reduced interpolant in <m_file_name>temp.smt2
  system((CURRENT_DIR + "/bin/mathsat " + OUTPUT_DIR + "/" + m_file_name +
	  "_reduced_mathsat.smt2 > " + OUTPUT_DIR + "/" + m_file_name +
	  "temp.smt2")
	     .c_str());

  // Setup *_reduced_interpolant_mathsat.smt2 file
  // to parse reduced interpolant
  std::ifstream result(OUTPUT_DIR + "/" + m_file_name + "temp.smt2");
  std::string line(""), interpolant_from_file("");
  // We consume one line because
  // mathsat outputs "check-sat" followed
  // by the interpolant
  std::getline(result, line);
  interpolant_from_file += solver.to_smt2_decls_only();
  interpolant_from_file += "(assert \n";
  while (std::getline(result, line)) {
    is_valid_result = true;
    interpolant_from_file += line + "\n";
  }
  interpolant_from_file += ")\n";
  interpolant_from_file += "(check-sat)\n";
  system(("rm -rf " + OUTPUT_DIR + "/" + m_file_name + "temp.smt2").c_str());

  if (is_valid_result) {
    // Lift interpolant to MaxDiff(Index Theory)
    z3::solver mathsat_interpolant_parser(sig.ctx);
    mathsat_interpolant_parser.from_string(interpolant_from_file.c_str());

    is_interpolant_computed = true;
    current_interpolant =
	liftInterpolant(mathsat_interpolant_parser.assertions());

#if _TEST_OUTPUT_
    TEST_OUTPUT_CODE(mathsat_interpolant_parser);
#endif
  }
}

void axdinterpolator::AXDInterpolant::smtInterpolOutputFile() {
  if (!is_unsat)
    throw "Input problem is not unsatisfiable.";
  // Setup smt2 file with reduced formulas
  // in Index Theory + EUF
  system(("mkdir -p " + OUTPUT_DIR).c_str());
  std::ofstream smtinterpol_file(OUTPUT_DIR + "/" + m_file_name +
				 "_reduced_smtinterpol.smt2");

  smtinterpol_file << "(set-option :print-success false)\n"
		   << "(set-option :produce-interpolants true)" << std::endl;
  // Setup logic engine
  smtinterpol_file << "(set-logic "
		   << ((sig.is_QF_TO() || sig.is_QF_IDL()) ? "QF_UFIDL"
							   : "QF_UFLIA")
		   << ")" << std::endl;

  smtinterpol_file << solver.to_smt2_decls_only();
  smtinterpol_file << "(assert (! (and" << std::endl;
  // This true assertion prevents
  // an unary application of and
  smtinterpol_file << "true" << std::endl;
  SmtSolverOutStreamSetup(smtinterpol_file, part_a);
  smtinterpol_file << ") :named part_a))" << std::endl;
  smtinterpol_file << "(assert (! (and" << std::endl;
  // This true assertion prevents
  // an unary application of and
  smtinterpol_file << "true" << std::endl;
  SmtSolverOutStreamSetup(smtinterpol_file, part_b);
  smtinterpol_file << ") :named part_b))" << std::endl;
  smtinterpol_file << "(check-sat)" << std::endl;
  smtinterpol_file << "(get-interpolants part_a part_b)" << std::endl;

  // Obtain reduced interpolant in <m_file_name>temp.smt2
  system(("java -jar " + CURRENT_DIR +
	  "/bin/smtinterpol-2.5-663-gf15aa217.jar -w " + OUTPUT_DIR + "/" +
	  m_file_name + "_reduced_smtinterpol.smt2 > " + OUTPUT_DIR + "/" +
	  m_file_name + "temp.smt2")
	     .c_str());

  // Setup *_reduced_interpolant_smtinterpol.smt2 file
  // to parse reduced interpolant
  std::ifstream result(OUTPUT_DIR + "/" + m_file_name + "temp.smt2");
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
  // The following removes the last parenthesis
  // and the extra '\n'
  interpolant_from_file.erase(interpolant_from_file.size() - 2, 2);
  is_valid_result = interpolant_from_file.size() > 0;
  // Only one parenthesis is needed to close
  // the above since the content of (interpolant *)
  // includes an additional parenthesis
  interpolant_from_file += ")\n";
  interpolant_from_file += "(check-sat)\n";
#if _DEBUG_SMTINTERPOLOUTPUTFILE_
  m_out << "Printing smt2 file from smtInterpolOutputFile" << std::endl;
  m_out << interpolant_from_file << std::endl;
#endif
  // system(("rm -rf "
  //+ OUTPUT_DIR + "/" + m_file_name + "temp.smt2").c_str());

  if (is_valid_result) {
    // Lift interpolant to MaxDiff(Index Theory)
    z3::solver smtinterpol_interpolant_parser(sig.ctx);
    smtinterpol_interpolant_parser.from_string(interpolant_from_file.c_str());

    is_interpolant_computed = true;
    current_interpolant =
	liftInterpolant(smtinterpol_interpolant_parser.assertions());

#if _TEST_OUTPUT_
    TEST_OUTPUT_CODE(smtinterpol_interpolant_parser);
#endif
  }
}
