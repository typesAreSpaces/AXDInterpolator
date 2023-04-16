#include "AXDInterpolant.h"
#include <algorithm>

void axdinterpolator::AXDInterpolant::step_1() {
#if _DEBUG_STEPS_
  m_out << std::endl;
  m_out << "Step 1:" << std::endl;
#endif
  
  if (common_array_vars.areCommonPairsAvaible()) {
#if _DEBUG_STEPS_
    m_out << std::endl;
    m_out << ">> There are common symbols" << std::endl;
#endif

    CircularPairIterator common_array_pairs(common_array_vars, false);
    unsigned N_bound = 1 + std::max(n_IndexALocal, n_IndexBLocal);

    DiffMap diff_common_map(common_array_vars, sig);

    while (!common_array_pairs.end()) {
      auto const &common_pair = *common_array_pairs;

      for (unsigned n = 1; n < N_bound; n++) {
	diff_common_map.add(common_pair.first, common_pair.second,
			    fresh_index_constant());
#if 0
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
#endif
      }

      common_array_pairs.next();
    }

    m_out << diff_common_map << std::endl;
    // TODO:
    // -) Update part_a.part_2 using diff_common_map
    // with the translattion lemma of these equations
    // using Lemma 3.5
    // -) Update part_b.part_2 using diff_common_map
    // with the translattion lemma of these equations
    // using Lemma 3.5
    // *) Remember the following:
    // replace terms |a| by the index constant i
    // such that |a| = i
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
