#include "AXDInterpolant.h"
#include "z3++.h"
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

#if 0
    m_out << ">> Chain of equation of the form diff_n(c_1, c_2) = k_n"
	  << std::endl;
#endif
    for (auto const &entry : diff_common_map.m_map) {
      auto const &pair = entry.first;
      auto const &c_1 = pair.first;
      auto const &c_2 = pair.second;
      auto const &length_c_1 = getLengthIndexVar(c_1);
      auto const &length_c_2 = getLengthIndexVar(c_2);
      auto const &indexes = entry.second;
      auto const &curr_rd = sig.getRdBySort(c_1.get_sort());
#if 0
      m_out << "Pair: ";
      m_out << c_1 << ", ";
      m_out << c_2 << std::endl;
      m_out << indexes << std::endl;
#endif
      // -----------------------------------------------------
      // First predicate from (22)
      for (unsigned l = 0; l < indexes.size() - 1; l++) {
	part_a.part_2.push_back(indexes[l] >= indexes[l + 1]);
	part_b.part_2.push_back(indexes[l] >= indexes[l + 1]);
      }
      part_a.part_2.push_back(indexes[indexes.size() - 1] >= 0);
      part_b.part_2.push_back(indexes[indexes.size() - 1] >= 0);
      // -----------------------------------------------------
      // -----------------------------------------------------
      // Second predicate from (22)
      for (unsigned l = 0; l < indexes.size() - 1; l++) {
	part_a.part_2.push_back(
	    z3::implies(indexes[l] > indexes[l + 1],
			curr_rd(c_1, indexes[l]) != curr_rd(c_2, indexes[l])));
	part_b.part_2.push_back(
	    z3::implies(indexes[l] > indexes[l + 1],
			curr_rd(c_1, indexes[l]) != curr_rd(c_2, indexes[l])));
      }
      // -----------------------------------------------------
      // -----------------------------------------------------
      // Third predicate from (22)
      for (unsigned l = 0; l < indexes.size() - 1; l++) {
	part_a.part_2.push_back(z3::implies(length_c_1 == length_c_2 &&
						indexes[l] == indexes[l + 1],
					    indexes[l] == 0));
	part_b.part_2.push_back(z3::implies(length_c_1 == length_c_2 &&
						indexes[l] == indexes[l + 1],
					    indexes[l] == 0));
      }
      // -----------------------------------------------------
      // -----------------------------------------------------
      // Forth predicate from (22)
      for (unsigned l = 0; l < indexes.size(); l++) {
	part_a.part_2.push_back(
	    z3::implies(curr_rd(c_1, indexes[l]) == curr_rd(c_2, indexes[l]),
			indexes[l] == 0));
	part_b.part_2.push_back(
	    z3::implies(curr_rd(c_1, indexes[l]) == curr_rd(c_2, indexes[l]),
			indexes[l] == 0));
      }
      // -----------------------------------------------------
      // -----------------------------------------------------
      // Fifth predicate from (22)
      z3::expr_vector temp_disjs(sig.ctx);
      for (unsigned l = 0; l < indexes.size() - 1; l++) {
	// part_a.index_var == part_b.index_var
	// since both use the same context
	temp_disjs.push_back(part_a.index_var == indexes[l]);
      }
      part_a.part_2.push_back(
	  z3::implies(part_a.index_var == indexes[indexes.size() - 1],
		      curr_rd(c_1, part_a.index_var) ==
			  curr_rd(c_2, part_a.index_var)) ||
	  z3::mk_or(temp_disjs));
      part_b.part_2.push_back(
	  z3::implies(part_a.index_var == indexes[indexes.size() - 1],
		      curr_rd(c_1, part_a.index_var) ==
			  curr_rd(c_2, part_a.index_var)) ||
	  z3::mk_or(temp_disjs));
      // -----------------------------------------------------
      // -----------------------------------------------------
      // Sixth predicate from (22)
      part_a.part_2.push_back(
	  z3::implies(length_c_1 > length_c_2,
		      indexes[0] == indexes[indexes.size() - 1] &&
			  indexes[indexes.size() - 1] == length_c_1));
      part_b.part_2.push_back(
	  z3::implies(length_c_1 > length_c_2,
		      indexes[0] == indexes[indexes.size() - 1] &&
			  indexes[indexes.size() - 1] == length_c_1));
      // -----------------------------------------------------
      // -----------------------------------------------------
      // Seventh predicate from (22)
      part_a.part_2.push_back(
	  z3::implies(length_c_2 > length_c_1,
		      indexes[0] == indexes[indexes.size() - 1] &&
			  indexes[indexes.size() - 1] == length_c_1));
      part_b.part_2.push_back(
	  z3::implies(length_c_2 > length_c_1,
		      indexes[0] == indexes[indexes.size() - 1] &&
			  indexes[indexes.size() - 1] == length_c_2));
      // -----------------------------------------------------
    }
  }
}

void axdinterpolator::AXDInterpolant::step_2() {
  m_out << std::endl;
  m_out << "Part A part_2" << std::endl;
  m_out << part_a.part_2 << std::endl;
  m_out << std::endl;
  m_out << "Part A index variables" << std::endl;
  m_out << part_a_index_vars << std::endl;
  m_out << "Part B part_2" << std::endl;
  m_out << part_b.part_2 << std::endl;
  m_out << "Part B index variables" << std::endl;
  m_out << part_b_index_vars << std::endl;
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
