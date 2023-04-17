#include "AXDInterpolant.h"
#include "z3++.h"

#define SETUP(VAR, COMMAND)                                                    \
  for (auto const &assertion : side_part.part_2)                               \
    COMMAND;

#define SETUP2(VAR, COMMAND)                                                   \
  for (unsigned i = 0; i < side_part.size(); i++)                              \
    COMMAND;

void axdinterpolator::AXDInterpolant::SmtSolverSetup(
    z3::solver &solver, SeparatedPair const &side_part) {
  SETUP(solver, solver.add(assertion));
}

void axdinterpolator::AXDInterpolant::SmtSolverOutStreamSetup(
    std::ostream &out, SeparatedPair const &side_part) {
  SETUP(out, out << assertion << std::endl);
}

void axdinterpolator::AXDInterpolant::SmtSolverOutStreamSetup(
    std::ostream &out, z3::expr_vector const &side_part) {
  SETUP2(out, out << side_part[i] << std::endl);
}

void axdinterpolator::AXDInterpolant::AB_VectorsSetup(
    z3::expr_vector &part_vector, SeparatedPair const &side_part) {
  SETUP(part_vector, part_vector.push_back(assertion));
}
