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
#if _DEBUG_STEPS_
    m_out << std::endl;
    m_out << "N bound is " << N_bound << std::endl;
#endif

    DiffMap diff_common_map(common_array_vars, sig);

    while (!common_array_pairs.end()) {
      auto const &common_pair = *common_array_pairs;

      for (unsigned n = 1; n <= N_bound; n++) {
	auto const & new_index = fresh_index_constant();
	diff_common_map.add(common_pair.first, common_pair.second,
			    new_index);
	updateIndexVars(new_index, PART_A);
	updateIndexVars(new_index, PART_B);
#if _DEBUG_STEPS_
	m_out << std::endl;
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
	m_out << "The new index is" << std::endl;
	m_out << new_index << std::endl;
#endif
      }

      common_array_pairs.next();
    }

#if _DEBUG_STEPS_
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
      auto const &size = indexes.size();
      auto const &curr_rd = sig.getRdBySort(c_1.get_sort());
#if _DEBUG_STEPS_
      m_out << "Pair: ";
      m_out << c_1 << ", ";
      m_out << c_2 << std::endl;
      m_out << indexes << std::endl;
#endif
      assert(size > 0);
      // -----------------------------------------------------
      // First predicate from (22)
      for (unsigned l = 0; l < size - 1; l++) {
	part_a.part_2.push_back(indexes[l] >= indexes[l + 1]);
	part_b.part_2.push_back(indexes[l] >= indexes[l + 1]);
      }
      part_a.part_2.push_back(indexes[size - 1] >= 0);
      part_b.part_2.push_back(indexes[size - 1] >= 0);
      // -----------------------------------------------------
      // -----------------------------------------------------
      // Second predicate from (22)
      for (unsigned l = 0; l < size - 1; l++) {
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
      for (unsigned l = 0; l < size - 1; l++) {
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
      for (unsigned l = 0; l < size; l++) {
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
      for (unsigned l = 0; l < size - 1; l++) {
	// part_a.index_var == part_b.index_var
	// since both use the same context
	temp_disjs.push_back(part_a.index_var == indexes[l]);
      }
#if _DEBUG_STEPS_
      m_out << std::endl;
      m_out << "Debugging chain of diff_k equations" << std::endl;
      m_out << (z3::implies(part_a.index_var == indexes[size - 1],
			    curr_rd(c_1, part_a.index_var) ==
				curr_rd(c_2, part_a.index_var)) ||
		z3::mk_or(temp_disjs))
	    << std::endl;
#endif
      part_a.part_2.push_back(z3::implies(part_a.index_var > indexes[size - 1],
					  curr_rd(c_1, part_a.index_var) ==
					      curr_rd(c_2, part_a.index_var)) ||
			      z3::mk_or(temp_disjs));
      part_b.part_2.push_back(z3::implies(part_a.index_var > indexes[size - 1],
					  curr_rd(c_1, part_a.index_var) ==
					      curr_rd(c_2, part_a.index_var)) ||
			      z3::mk_or(temp_disjs));
      // -----------------------------------------------------
      // -----------------------------------------------------
      // Sixth predicate from (22)
      part_a.part_2.push_back(z3::implies(length_c_1 > length_c_2,
					  indexes[0] == indexes[size - 1] &&
					      indexes[size - 1] == length_c_1));
      part_b.part_2.push_back(z3::implies(length_c_1 > length_c_2,
					  indexes[0] == indexes[size - 1] &&
					      indexes[size - 1] == length_c_1));
      // -----------------------------------------------------
      // -----------------------------------------------------
      // Seventh predicate from (22)
      part_a.part_2.push_back(z3::implies(length_c_2 > length_c_1,
					  indexes[0] == indexes[size - 1] &&
					      indexes[size - 1] == length_c_1));
      part_b.part_2.push_back(z3::implies(length_c_2 > length_c_1,
					  indexes[0] == indexes[size - 1] &&
					      indexes[size - 1] == length_c_2));
      // -----------------------------------------------------
    }
  }
}

void axdinterpolator::AXDInterpolant::step_2() {
#if _DEBUG_STEPS_
  m_out << std::endl;
  m_out << "Step 2:" << std::endl;
  m_out << "Part A part_1" << std::endl;
  m_out << part_a.part_1 << std::endl;
  m_out << "Part A part_2" << std::endl;
  m_out << part_a.part_2 << std::endl;
  m_out << std::endl;
  m_out << "Part A index variables" << std::endl;
  m_out << part_a_index_vars << std::endl;
  m_out << "Part B part_1" << std::endl;
  m_out << part_b.part_1 << std::endl;
  m_out << "Part B part_2" << std::endl;
  m_out << part_b.part_2 << std::endl;
  m_out << "Part B index variables" << std::endl;
  m_out << part_b_index_vars << std::endl;
#endif

  // TODO:
  // Make instantiations more efficient
  // Hint: just make substitutions on
  // formulas involving index_var

  z3::expr_vector from_parametric_index(sig.ctx);
  from_parametric_index.push_back(part_a.index_var);

  z3::expr conj_A = z3::mk_and(part_a.part_2);
  z3::expr_vector temp_index_A(sig.ctx);
  for (unsigned i = 0; i < part_a_index_vars.size(); i++) {
    temp_index_A.push_back(part_a_index_vars[i]);
#if _DEBUG_STEPS_
    m_out << "Part A: Replacing index_var with " << part_a_index_vars[i]
	  << std::endl;
    m_out << ">>> After" << std::endl;
    m_out << conj_A.substitute(from_parametric_index, temp_index_A)
	  << std::endl;
#endif 
    solver.add(conj_A.substitute(from_parametric_index, temp_index_A));
    temp_index_A.pop_back();
  }

  z3::expr conj_B = z3::mk_and(part_b.part_2);
  z3::expr_vector temp_index_B(sig.ctx);
  for (unsigned i = 0; i < part_b_index_vars.size(); i++) {
    temp_index_B.push_back(part_b_index_vars[i]);
#if _DEBUG_STEPS_
    m_out << "Part B: Replacing index_var with " << part_b_index_vars[i]
	  << std::endl;
    m_out << ">>> After" << std::endl;
    m_out << conj_B.substitute(from_parametric_index, temp_index_B)
	  << std::endl;
#endif
    solver.add(conj_B.substitute(from_parametric_index, temp_index_B));
    temp_index_B.pop_back();
  }
}

void axdinterpolator::AXDInterpolant::step_3() {
// TODO:
// -) Lift and replace back AB-common
// constants introduced by the transformations
#if _DEBUG_STEPS_
  m_out << std::endl;
  m_out << "Step 3:" << std::endl;
  m_out << solver << std::endl;
#endif
  switch (solver.check()) {
  case z3::unsat:
    m_out << "Unsatisfiable formula" << std::endl;
    is_unsat = true;
    break;
  case z3::sat:
    m_out << "Satisfiable formula" << std::endl;
    break;
  case z3::unknown:
    m_out << "Unknown formula" << std::endl;
    break;
  }
}
