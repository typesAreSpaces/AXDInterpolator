#include "AXDInterpolant.h"
#include "Preprocess.h"

axdinterpolator::AXDInterpolant::AXDInterpolant(AXDSignature &sig,
                                                z3::expr _input_part_a,
                                                z3::expr _input_part_b,
                                                char const *file_name)
    : Preprocessor(sig, _input_part_a, _input_part_b),

      part_a(sig, (Preprocessor const) * this, input_part_a, part_a_array_vars),
      part_b(sig, (Preprocessor const) * this, input_part_b, part_b_array_vars),
      instantiated_part_a(sig.ctx), instantiated_part_b(sig.ctx),

      m_file_name(std::string(file_name)),

      is_interpolant_computed(false), is_unsat(false), is_valid_result(false),
      state_output(undefined),

      current_interpolant(sig.ctx.bool_val(true)),

      solver(sig.ctx,
	     sig.is_QF_TO() || sig.is_QF_IDL() ? "QF_UFIDL" : "QF_UFLIA") {

#if _DEBUG_AXD_CONSTRUCTOR_
  m_out << "After Processing" << std::endl;
  m_out << "Input Part A" << std::endl;
  m_out << input_part_a << std::endl;
  m_out << "Input Part B" << std::endl;
  m_out << input_part_b << std::endl;

  m_out << "Before SeparatedPair" << std::endl;
  m_out << "Input Part A" << std::endl;
  m_out << part_a << std::endl;
  m_out << "Input Part B" << std::endl;
  m_out << part_b << std::endl;

  m_out << "Solving file " << m_file_name << std::endl;
  m_out << "Input formula" << std::endl;
  m_out << "Part A" << std::endl;
  m_out << part_a << std::endl;
  m_out << "Part B" << std::endl;
  m_out << part_b << std::endl;
#endif

  m_file_name = m_file_name.substr(0, m_file_name.find_last_of("."))
		    .substr(m_file_name.find_last_of("\\/") + 1);

  // TODOs:
  // @Abby
  // - Implement Step 1 from paper:
  //   The previous implementation was 'doing this
  //   step' in each iteration of the main loop
  //   Take a look to the code in
  //   SeparatedPair::updateSaturation
  // @Jose
  // - Remove loop
  // - Instantiate directly just once
  // - Check satisfiability using $EUF \cup T_I$

  step_1();
  step_2();
  step_3();
}

z3::expr axdinterpolator::AXDInterpolant::liftInterpolant(
    z3::expr_vector const &interpolant) {

  z3::expr_vector _interpolant(sig.ctx);

  if (sig.is_QF_TO())
    for (auto const &x : interpolant)
      _interpolant.push_back(x.qf_to_simplify());
  else
    for (auto const &x : interpolant)
      _interpolant.push_back(x);

  z3::expr_vector from(sig.ctx);
  z3::expr_vector to(sig.ctx);

  liftInterpolantDiffSubs(from, to, part_a);
  liftInterpolantDiffSubs(from, to, part_b);

  return z3::mk_and(_interpolant).substitute(from, to);
}

void axdinterpolator::AXDInterpolant::liftInterpolantDiffSubs(
    z3::expr_vector &from, z3::expr_vector &to, SeparatedPair const &input) {
  for (auto const &diff_entry : input.diff_map.m_map) {
    auto const &diff_a = diff_entry.first.first;
    auto const &diff_b = diff_entry.first.second;
    auto const &diff_seq = diff_entry.second;
    auto const &curr_diff_k = sig.getDiff_BySort(diff_a.get_sort());
    unsigned diff_iteration = 1;
    for (auto const &k_ : diff_seq) {
      if (func_name(k_).rfind(FRESH_COMMON_PREFIX, 0) == 0) {
	from.push_back(k_);
	to.push_back(
	    curr_diff_k(sig.ctx.int_val(diff_iteration++), diff_a, diff_b));
      }
    }
  }
}

namespace axdinterpolator {

std::ostream &operator<<(std::ostream &os,
			 axdinterpolator::AXDInterpolant const &axd) {

  if (!axd.is_unsat)
    return os << "Input formula is satisfiable.";
  if (!axd.is_valid_result)
    return os << "Input formula is unsatisfiable.\n"
		 "Solver used wasn't able to compute "
		 "an interpolant in the specified theory";
  if (axd.is_interpolant_computed) {
    os << "Input formula is unsatisfiable." << std::endl;
    switch (axd.state_output) {
    case axdinterpolator::AXDInterpolant::undefined:
      os << "State Output: not tested" << std::endl;
      break;
    case axdinterpolator::AXDInterpolant::fine:
      os
#if _TEST_OUTPUT_ORIGINAL_THY_
	  << "State Output: valid in the original theory"
#else
	  << "State Output: valid in the reduced theory"
#endif
	  << std::endl;
      break;
    case axdinterpolator::AXDInterpolant::notfine:
      os << "State Output: not valid" << std::endl;
      break;
    }
    return os
#if _INCLUDE_OUTPUT_
	   << "Interpolant:" << std::endl
	   << axd.current_interpolant
#endif
	;
  } else
    return os << "Interpolant hasn't been computed.\n"
		 "Use .z3OutputFile or .mathsatOutputFile\n"
		 "or .directComputation on a AXDInterpolant\n"
		 "object to obtain an interpolant.";
}

} // namespace axdinterpolator
