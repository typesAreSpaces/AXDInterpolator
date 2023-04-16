#include "SeparatedPair.h"
#include "Preprocess.h"
#include "util.h"
#include "z3++.h"

axdinterpolator::SeparatedPair::SeparatedPair(
    AXDSignature const &sig, Preprocessor const &preprocessor,
    z3::expr_vector const &conjunction, ArrayVars const &array_var_ids)
    : sig(sig), preprocessor(preprocessor),

      diff_map(array_var_ids, sig),

      part_1(sig.ctx), part_2(sig.ctx),

      // Convention: we use Int
      // to encode the type of the INDEX sort
      index_var(sig.ctx.constant("index_var", sig.int_sort)) {

  separateIntoPair(conjunction);

#if 0
  m_out << std::endl;
  m_out << "Part 1: " << part_1 << std::endl;
  m_out << "Part 2: " << part_2 << std::endl;
#endif

  processPart_1();

#if 0
  m_out << std::endl;
  m_out << "Start - Printing current DiffMap" << std::endl;
  m_out << diff_map << std::endl;
  m_out << "End DiffMap" << std::endl;
#endif

  initSaturation();
}

void axdinterpolator::SeparatedPair::initSaturation() {
  // This section was commented out
  // Because this corresponds to the
  // AXD(T_I) approach
#if 0
  // ------------------------------------------------
  // Processing equations of the form a = wr(b, i, e)
  // The following adds (18) predicates
  for (auto const &_4tuple : write_vector.m_vector) {
    auto const &a = std::get<0>(_4tuple);
    auto const &i = std::get<2>(_4tuple);
    auto const &e = std::get<3>(_4tuple);

    auto const &curr_rd = sig.getRdBySort(a.get_sort());

    // The following adds i >=0 \rightarrow rd(a, i) = e
    part_2.push_back(z3::implies(i >= 0, curr_rd(a, i) == e));

    // [11] predicates are processed in
    // AXDInterpolant::SmtSolverSetup(z3::solver &);
  }
  // ------------------------------------------------
#endif

// This section was commented out
// Because this corresponds to the
// AXD(T_I) approach
#if 0
  // -----------------------------------------------
  // Processing equations of the form diff(a, b) = i
  // The following adds [12] predicates
  for (auto const &entry : diff_map.m_map) {
    auto const &a = entry.first.first;
    auto const &b = entry.first.second;
    auto const &seq = entry.second;

    auto const &curr_rd = sig.getRdBySort(a.get_sort());

    if (seq.size() > 0) {
      auto const &i = seq[0];

      // The following adds
      // i >= 0
      part_2.push_back(i >= 0);

      // [18] predicates are processed in
      // AXDInterpolant::SmtSolverSetup(z3::solver &);

      // The following adds
      // i > 0 \rightarrow rd(a, i) \neq rd(b, i)
      part_2.push_back(
	  z3::implies(i > sig.ctx.int_val(0), curr_rd(a, i) != curr_rd(b, i)));
    }
  }
  // -----------------------------------------------
#endif
}

void axdinterpolator::SeparatedPair::updateSaturation(
    DiffMap::z3_expr_pair const &entry, z3::expr const &_new_index,
    unsigned min_dim) {

  auto const &c1 = entry.first;
  auto const &c2 = entry.second;
  auto const &map_element = diff_map.m_map.find(entry);
  auto const &current_indices = map_element->second;
  unsigned old_dim = current_indices.size();

  // instantiated_terms.add_var(_new_index);

  // Heuristic for triggering N-instantiation
  // if(instantiated_terms.getNumOfNewIndex() % 1000 == 0)
  // if(false)
  //   ++instantiated_terms;

  // [11] predicates are processed in
  // AXDInterpolant::SmtSolverSetup(z3::solver &);

  // Processing equations of the form diff_1(a, b) = k_1
  // \land \dots \land diff_l(a, b) = k_l [13]
  if (min_dim < old_dim)
    part_2.push_back(_new_index == map_element->second[min_dim]);
  else {
    auto const &curr_rd = sig.getRdBySort(c1.get_sort());
    diff_map.add(entry.first, entry.second, _new_index);
    // old_dim > 0 guarantees having a previous index
    if (old_dim > 0) {
      auto const &_previous_index = map_element->second[old_dim - 1];

      // The following adds [14] predicates
      part_2.push_back(_previous_index >= _new_index);
      part_2.push_back(_new_index >= sig.ctx.int_val(0));

      // The following adds [15] predicates
      part_2.push_back(z3::implies(_previous_index > _new_index,
				   curr_rd(c1, _previous_index) !=
				       curr_rd(c2, _previous_index)));

      // The following adds [16] predicates
      part_2.push_back(z3::implies(_previous_index == _new_index,
				   _previous_index == sig.ctx.int_val(0)));
    }
    // The following adds [17] predicates
    part_2.push_back(
	z3::implies(curr_rd(c1, _new_index) == curr_rd(c2, _new_index),
		    _new_index == sig.ctx.int_val(0)));
  }

  // [18] predicates are processed in
  // AXDInterpolant::SmtSolverSetup(z3::solver &);
}

void axdinterpolator::SeparatedPair::separateIntoPair(
    z3::expr_vector const &conjunction) {
  // Split input into part_1 and part_2
  // following the rules for "separated pairs".
  for (auto const &current_conj : conjunction) {
#if 0
    m_out << std::endl;
    m_out << "@SeparatedPair" << std::endl;
    m_out << current_conj << std::endl;
#endif
    switch (func_kind(current_conj)) {
    case Z3_OP_TRUE:
    case Z3_OP_FALSE:
      part_2.push_back(current_conj);
    case Z3_OP_UNINTERPRETED:
      if (current_conj.get_sort().is_bool()) {
	part_2.push_back(current_conj);
	continue;
      }
      break;

    case Z3_OP_EQ: // ==
    {
      auto const &_lhs = lhs(current_conj);
      auto const &_rhs = rhs(current_conj);

      // Covers equations of the form
      // a = wr(b, i, e) or a = b
      // when a is an array var
      if (isArraySort(_lhs.get_sort())) {
	// (17) predicates are added here,
	// i.e. equations of the form a = b
	// where a, b are array variables
	if (_lhs.num_args() == 0 && _rhs.num_args() == 0) {
	  auto const &curr_diff = sig.getDiffBySort(_lhs.get_sort());
	  auto const &curr_rd = sig.getRdBySort(_lhs.get_sort());
	  part_1.push_back(0 == curr_diff(_lhs, _rhs));
	  part_2.push_back(curr_rd(_lhs, 0) == curr_rd(_rhs, 0));
	  part_2.push_back(current_conj);
	}
	// Equations of the form
	// a = wr(b, i, e) will be processed
	// the processPart_1 method
	else {
	  assert(func_name(_rhs).find("wr") != std::string::npos);
	  part_1.push_back(current_conj);
	}
	continue;
      }

      // Covers equations of the
      // form i = diff(a, b)
      if (func_name(_rhs).find("diff") != std::string::npos) {
	// Equations of the form
	// i = diff(a, b) will be processed
	// the processPart_1 method
	part_1.push_back(current_conj);
	continue;
      }

      // Covers equations of the
      // form i = length(a)
      if (func_name(_rhs).find("length") != std::string::npos) {
	// Equations of the form
	// i = length(x) will be processed
	// the processPart_1 method
	part_1.push_back(current_conj);
	continue;
      }

      // If any of the previous cases were not
      // a match, `current_conj` belongs to
      // part_2
      part_2.push_back(current_conj);
      break;
    }

    case Z3_OP_DISTINCT: // !=
      ASSERT(lhs(current_conj).num_args() == 0 &&
		 rhs(current_conj).num_args() == 0,
	     "Invariant of constant_1 != constant_2 is violated");
    case Z3_OP_GE: // >=
    case Z3_OP_LE: // <=
    case Z3_OP_GT: // >
    case Z3_OP_LT: // <
      part_2.push_back(current_conj);
      break;
    default:
      m_out << current_conj << std::endl;
      throw "Problem @ "
	    "axdinterpolator::SeparatedPair "
	    "Invalid formula.";
    }
  }
}

void axdinterpolator::SeparatedPair::processPart_1() {
  // Setting up internal data structures
  // WriteVector and DiffMap
  for (auto const &equation : part_1) {
#if 0
    m_out << std::endl;
    m_out << "Processing equation: " << equation << std::endl;
#endif
    auto f_name = func_name(rhs(equation));

    // Processing equations of the form a = wr(b, i, e)
    // The following adds (18) predicates
    if (f_name.find("wr") != std::string::npos) {
      auto const &a = lhs(equation);
      auto const &b = rhs(equation).arg(0);
      auto const &i = rhs(equation).arg(1);
      auto const &e = rhs(equation).arg(2);
      auto const &curr_rd = sig.getRdBySort(a.get_sort());
      auto const &curr_undefined = sig.getUndefinedBySort(e.get_sort());
      auto const &length_b = preprocessor.getLengthIndexVar(b);

      auto const &first_predicate = z3::implies(
	  e != curr_undefined && 0 <= i && i <= length_b, curr_rd(a, i) == e);
      auto const &second_predicate =
	  z3::implies(i < 0 || i > length_b || e == curr_undefined,
		      curr_rd(a, i) == curr_rd(b, i));
      auto const &third_predicate = z3::implies(
	  index_var != i, curr_rd(a, index_var) == curr_rd(b, index_var));

#if 0
      m_out << std::endl;
      m_out << "First predicate Formula (18)" << std::endl;
      m_out << first_predicate << std::endl;
      m_out << "Second predicate Formula (18)" << std::endl;
      m_out << second_predicate << std::endl;
      m_out << "Third predicate Formula (18)" << std::endl;
      m_out << third_predicate << std::endl;
#endif
      part_2.push_back(first_predicate);
      part_2.push_back(second_predicate);
      part_2.push_back(third_predicate);
    }

    // Processing equations of the form i = diff(a, b)
    // The following adds instances when n = 1
    // of (22) predicates
    if (f_name.find("diff") != std::string::npos) {
      diff_map.add(lhs(rhs(equation)), rhs(rhs(equation)), lhs(equation));
      auto const &i = lhs(equation);
      auto const &a = rhs(equation).arg(0);
      auto const &b = rhs(equation).arg(1);
      auto const &curr_rd = sig.getRdBySort(a.get_sort());
      auto const &length_a = preprocessor.getLengthIndexVar(a);
      auto const &length_b = preprocessor.getLengthIndexVar(b);
      auto const &first_predicate = i >= 0;
      auto const &second_predicate =
	  z3::implies(curr_rd(a, i) == curr_rd(b, i), i == 0);
      auto const &third_predicate = z3::implies(
	  index_var > i, curr_rd(a, index_var) == curr_rd(b, index_var));
      auto const &forth_predicate =
	  z3::implies(length_a > length_b, i == length_a);
      auto const &fifth_predicate =
	  z3::implies(length_b > length_a, i == length_b);
#if 0
      m_out << std::endl;
      m_out << "First predicate" << std::endl;
      m_out << first_predicate << std::endl;
      m_out << "Second predicate" << std::endl;
      m_out << second_predicate << std::endl;
      m_out << "Third predicate" << std::endl;
      m_out << third_predicate << std::endl;
      m_out << "Forth predicate" << std::endl;
      m_out << forth_predicate << std::endl;
      m_out << "Fifth predicate" << std::endl;
      m_out << fifth_predicate << std::endl;
#endif
      part_2.push_back(first_predicate);
      part_2.push_back(second_predicate);
      part_2.push_back(third_predicate);
      part_2.push_back(forth_predicate);
      part_2.push_back(fifth_predicate);
    }

    // Processing equations of the form i = |a|
    // The following adds (19) predicates
    if (f_name.find("length") != std::string::npos) {
      auto const &i = lhs(equation);
      auto const &a = rhs(equation).arg(0);
      auto const &curr_rd = sig.getRdBySort(a.get_sort());
      auto const &read_a_at_h = curr_rd(a, index_var);
      auto const &curr_undefined =
	  sig.getUndefinedBySort(read_a_at_h.get_sort());
      auto const &first_predicate = i >= 0;
      auto const &second_predicate =
	  (read_a_at_h == curr_undefined) == (0 <= index_var && index_var <= i);
#if 0
      m_out << std::endl;
      m_out << "First predicate" << std::endl;
      m_out << first_predicate << std::endl;
      m_out << "Second predicate" << std::endl;
      m_out << second_predicate << std::endl;
#endif
      part_2.push_back(first_predicate);
      part_2.push_back(second_predicate);
    }
  }
}

namespace axdinterpolator {
std::ostream &operator<<(std::ostream &os,
			 axdinterpolator::SeparatedPair const &sp) {
  return (os << "Part 1: " << sp.part_1 << std::endl
	     << "Part 2: " << sp.part_2);
}
} // namespace axdinterpolator
