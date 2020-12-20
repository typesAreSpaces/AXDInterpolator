#ifndef _AXD_INTERPOLANT_
#define _AXD_INTERPOLANT_

#define _DEBUG_AXD_INTER_           1
#define _TEST_OUTPUT_               0
#define _TEST_ORIGINAL_INPUT_       0

#define OUTPUT_DIR std::string("./output")

#include <iostream>
#include <utility>
#include <set>
#include <fstream>
#include "Preprocess.h"
#include "StandardInput.h"

// Notes:
// The input file is a smt2 file
// with two assertions.
// Each assertion is 
// of the form (and <pred_1> \dots <pred_n>).

class AXDInterpolant : public Preprocessor {

  z3::solver    solver;
  StandardInput part_a, part_b;
  std::string   m_file_name;
  bool          is_interpolant_computed,
                is_unsat;
  z3::expr      current_interpolant;

  void loop(unsigned);
  void testOutput(
      z3::expr const &, 
      z3::expr_vector &, 
      z3::expr_vector &);
  void SmtSolverSetup(z3::solver &, StandardInput const &);
  void SmtSolverOutStreamSetup(
      std::ostream &, 
      StandardInput const &);
  void setupPartA_B_Vectors(
      z3::expr_vector &, 
      z3::expr_vector &);

  z3::expr computeReducedInterpolant(
      z3::expr_vector const &, 
      z3::expr_vector const &);
  z3::expr liftInterpolant(z3::expr const &); 

  public: 
  AXDInterpolant(z3::context &, char const *, char const *, unsigned);

  void z3OutputFile();
  void mathsatOutputFile();
  void directComputation();

  std::string defineDeclarations(std::string) const;
  z3::expr defineDeclarations(z3::expr const &) const;

  friend std::ostream & operator << (std::ostream &, AXDInterpolant const &);

  class CircularPairIterator {
    friend class AXDInterpolant;

    z3_expr_set const & vars;
    z3_expr_set::iterator first, second;

    void avoidLowerDiagonal();

    public:
    CircularPairIterator(z3_expr_set const &);
    void next();
    StandardInput::DiffMap::z3_expr_pair operator *() const;

    friend std::ostream & operator << (std::ostream &, CircularPairIterator const &);
  };
};

#endif
