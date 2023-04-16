#include "AXDInterpolant.h"

void axdinterpolator::AXDInterpolant::step_1() {
#if _DEBUG_STEPS_
  m_out << std::endl;
  m_out << "Step 1:" << std::endl;
#endif
  
  if (common_array_vars.areCommonPairsAvaible()) {
    m_out << std::endl;
    m_out << ">> There are common symbols" << std::endl;

    CircularPairIterator common_array_pairs(common_array_vars, false);

    while (!common_array_pairs.end()) {
      // TODO: Compute Step 1, i.e. the
      // chain of equations of the form
      // diff_n(c_1, c_2) = k_n for n = 1 ... N
      // and update part_2 with the translation
      // lemma of these equations using Lemma 3.5
      auto const &common_pair = *common_array_pairs;
      m_out << ">> First component: ";
      m_out << common_pair.first << std::endl;
      m_out << ">> Id: ";
      m_out << common_pair.first.id() << std::endl;
      m_out << ">> Name of index representing its length: ";
      m_out << getLengthIndexVar(common_pair.first) << std::endl;
      m_out << ">> Second component: ";
      m_out << common_pair.second << std::endl;
      m_out << ">> Id: ";
      m_out << common_pair.second.id() << std::endl;
      m_out << ">> Name of index representing its length: ";
      m_out << getLengthIndexVar(common_pair.second) << std::endl;
      common_array_pairs.next();
    }
  }
}

void axdinterpolator::AXDInterpolant::step_2() {
// TODO:
// -) Apply 0-instantiations to `part_2` of the
// separated pair structures
#if _DEBUG_STEPS_
  m_out << std::endl;
  m_out << "Step 2:" << std::endl;
#endif
}

void axdinterpolator::AXDInterpolant::step_3() {
// TODO:
// -) Setup the solver and check for satisfiability
// -) Lift and replace back AB-common
// constants introduced by the transformations
#if _DEBUG_STEPS_
  m_out << std::endl;
  m_out << "Step 3:" << std::endl;
#endif
}
