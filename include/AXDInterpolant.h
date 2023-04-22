#ifndef _AXD_INTERPOLANT_
#define _AXD_INTERPOLANT_

#include "z3++.h"
#include <iostream>
#include <utility>
#include <set>
#include <cstring>
#include <fstream>
#include "Preprocess.h"
#include "StandardInput.h"

#define _DEBUG_AXD_LOOP_           0
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

class AXDInterpolant : public Preprocessor {

  class CircularPairIterator {
    friend class AXDInterpolant;

    typedef Preprocessor::ArrayVars PArrayVars;

    PArrayVars const & vars;
    PArrayVars::Container::const_iterator
      p_array_vars_it;
    AXDSignature::z3_expr_set::const_iterator
      z3_expr_set_first, 
      z3_expr_set_second;

    void avoidLowerDiagonalAndDifferentTypes();

    public:
    CircularPairIterator(Preprocessor::ArrayVars const &);
    void next();
    StandardInput::DiffMap::z3_expr_pair operator *() const;
  };

  enum StateOutput { undefined, fine, notfine };

  StandardInput        part_a, part_b;
  std::string          m_file_name;
  unsigned             num_attempts, remaining_fuel;
  bool                 is_interpolant_computed,
                       is_unsat,
                       is_valid_result;
  StateOutput          state_output;
  z3::expr             current_interpolant;
  z3::solver           solver;

  void loop();

  bool testOutput(
      z3::expr_vector const &, 
      z3::expr_vector &, 
      z3::expr_vector &);
  void testOutputArrayAxiomatization(z3::solver &);
  void testOutputDiffLifting(
      z3::solver & s, 
      StandardInput const &);

  void SmtSolverSetup(
      z3::solver &, 
      StandardInput const &);
  void SmtSolverOutStreamSetup(
      std::ostream &, 
      StandardInput const &);
  void AB_VectorsSetup(
      z3::expr_vector &, 
      StandardInput const &);

  z3::expr liftInterpolant(z3::expr_vector const &); 
  void     liftInterpolantDiffSubs(
      z3::expr_vector &, 
      z3::expr_vector &, 
      StandardInput const &); 

  public: 
  AXDInterpolant(
      AXDSignature &, 
      z3::expr const &,
      z3::expr const &,
      char const *, 
      unsigned);

  void z3OutputFile();
  void mathsatOutputFile();
  void smtInterpolOutputFile();

  bool isUnsat();

  friend std::ostream & operator << (
      std::ostream &, AXDInterpolant const &);
};

#endif
