#ifndef _AXD_INTERPOLANT_
#define _AXD_INTERPOLANT_

#include "z3++.h"
#define _SIMPLIFY_OUTPUT      0
#define _DEBUG_QF_TO_REWRITER 0
#define _DEBUG_AXD_INTER_     0
#define _DEBUG_AXD_LOOP_      0
#define _TEST_OUTPUT_         0
#define _TEST_ORIGINAL_INPUT_ 0

#define CURRENT_DIR std::string("replace_once")
#define OUTPUT_DIR CURRENT_DIR + std::string("/output")

#include <iostream>
#include <utility>
#include <set>
#include <cstring>
#include <fstream>
#include "Preprocess.h"
#include "StandardInput.h"

// Notes:
// The input file is a smt2 file
// with two assertions.
// Each assertion is 
// of the form (and <pred_1> \dots <pred_n>).

class AXDInterpolant : public Preprocessor {

  class CircularPairIterator {
    friend class AXDInterpolant;

    z3_expr_set const & vars;
    z3_expr_set::iterator first, second;

    void avoidLowerDiagonal();

    public:
    CircularPairIterator(z3_expr_set const &);
    void next();
    StandardInput::DiffMap::z3_expr_pair operator *() const;
  };


  z3::solver    solver;
  StandardInput part_a, part_b;
  std::string   m_file_name;
  unsigned      num_attempts;
  bool          is_interpolant_computed,
                is_unsat;
  z3::expr      current_interpolant;
  char const *  theory_name;

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

  z3::expr QF_TO_Rewriter(z3::expr const &);
  z3::expr QF_TO_RewriterAux(z3::expr const &);

  z3::expr liftInterpolant(z3::expr_vector const &); 
  void liftInterpolantDiffSubs(
      z3::expr_vector &, 
      z3::expr_vector &, 
      StandardInput const &); 

  std::string defineDeclarations(std::string) const;

  public: 
  AXDInterpolant(
      z3::context &, 
      z3::expr const &,
      char const *, 
      char const *, 
      unsigned);

  z3::expr const & z3OutputFile();
  z3::expr const & mathsatOutputFile();
  z3::expr const & smtInterpolOutputFile();

  friend std::ostream & operator << (
      std::ostream &, AXDInterpolant const &);
};

#endif
