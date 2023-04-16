#ifndef _AXD_INTERPOLANT_
#define _AXD_INTERPOLANT_

#include "z3++.h"
#include <iostream>
#include <utility>
#include <set>
#include <cstring>
#include <fstream>
#include "Preprocess.h"
#include "SeparatedPair.h"

#define _DEBUG_AXD_LOOP_           0
#define _DEBUG_STEPS_              0
#define _DEBUG_AXD_CONSTRUCTOR_    0
#define _TEST_OUTPUT_              0
#define _TEST_OUTPUT_ORIGINAL_THY_ 0
#define _INCLUDE_OUTPUT_           1

#define CURRENT_DIR std::string("replace_once")
#define OUTPUT_DIR  CURRENT_DIR + std::string("/output")

// Notes:
// The input file is a smt2 file
// with two assertions.
// Each assertion is 
// of the form (and <pred_1> \dots <pred_n>).

namespace axdinterpolator {

class AXDInterpolant : public Preprocessor {

  enum StateOutput { undefined, fine, notfine };

  SeparatedPair part_a, part_b;
  std::string m_file_name;

  bool is_interpolant_computed, is_unsat, is_valid_result;
  StateOutput state_output;

  z3::expr current_interpolant;

  z3::solver solver;

  void loop();

  void step_1();
  void step_2();
  void step_3();

  bool testOutput(z3::expr_vector const &, z3::expr_vector &,
		  z3::expr_vector &);
  void testOutputArrayAxiomatization(z3::solver &);
  void testOutputDiffLifting(z3::solver &s, SeparatedPair const &);

  void SmtSolverSetup(z3::solver &, SeparatedPair const &);
  void SmtSolverOutStreamSetup(std::ostream &, SeparatedPair const &);
  void AB_VectorsSetup(z3::expr_vector &, SeparatedPair const &);

  z3::expr liftInterpolant(z3::expr_vector const &);
  void liftInterpolantDiffSubs(z3::expr_vector &, z3::expr_vector &,
			       SeparatedPair const &);

public:
  AXDInterpolant(AXDSignature &, z3::expr, z3::expr, char const *);

  void z3OutputFile();
  void mathsatOutputFile();
  void smtInterpolOutputFile();

  friend std::ostream &operator<<(std::ostream &, AXDInterpolant const &);
};

} // namespace axdinterpolator

#endif
