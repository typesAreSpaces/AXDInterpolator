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

#if _DEBUG_SEPARATED_PAIR_
  m_out << std::endl;
  m_out << "Part 1: " << part_1 << std::endl;
  m_out << "Part 2: " << part_2 << std::endl;
#endif

  processPart_1();

#if _DEBUG_SEPARATED_PAIR_
  m_out << std::endl;
  m_out << "Start - Printing current DiffMap" << std::endl;
  m_out << diff_map << std::endl;
  m_out << "End DiffMap" << std::endl;
#endif
}

void axdinterpolator::SeparatedPair::separateIntoPair(
    z3::expr_vector const &conjunction) {
  // Split input into part_1 and part_2
  // following the rules for "separated pairs".
  for (auto const &current_conj : conjunction) {
#if _DEBUG_SEPARATED_PAIR_
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
#if _DEBUG_PROCESS_PART_1_
    m_out << std::endl;
    m_out << ">> @processPart_1 Processing equation: " << equation << std::endl; 
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

#if _DEBUG_SEPARATED_PAIR_
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
#if _DEBUG_SEPARATED_PAIR_
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
	  (read_a_at_h != curr_undefined) == (0 <= index_var && index_var <= i);
#if _DEBUG_PROCESS_PART_1_
      m_out << std::endl;
      m_out << ">> Debugging translation lemmas for length" << std::endl;
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
