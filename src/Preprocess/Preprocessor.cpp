#include "Preprocess.h"
#include "z3++.h"

// NORMALIZED_INPUT normalizes RAW_INPUT (using normalizedInput)
// and breaks the resulting z3::expr into a z3::vector_expr
// containing the conjuncts of the normalized expr
// Additionally, reorients equations into a 'prelimary canonical
// normal form', i.e. of the form (= a (f a_1 ... a_{n-1}))
// where a is a constant
#define NORMALIZE_INPUT(RAW_INPUT, NORMALIZED_INPUT, NORMALIZED_EXPR, SIDE)    \
  z3::expr const &NORMALIZED_EXPR = normalizeInput(RAW_INPUT, SIDE);           \
  switch (SIDE) {                                                              \
  case PART_A: {                                                               \
    for (unsigned i = 0; i < part_a_extra_length_eqs.size(); i++)              \
      NORMALIZED_INPUT.push_back(part_a_extra_length_eqs[i]);                  \
    break;                                                                     \
  }                                                                            \
  case PART_B: {                                                               \
    for (unsigned i = 0; i < part_b_extra_length_eqs.size(); i++)              \
      NORMALIZED_INPUT.push_back(part_b_extra_length_eqs[i]);                  \
    break;                                                                     \
  }                                                                            \
  }                                                                            \
  if (func_kind(NORMALIZED_EXPR) == Z3_OP_AND) {                               \
    for (unsigned i = 0; i < NORMALIZED_EXPR.num_args(); ++i) {                \
      auto const &curr_arg = NORMALIZED_EXPR.arg(i);                           \
      if (func_kind(curr_arg) == Z3_OP_EQ && rhs(curr_arg).num_args() == 0)    \
	NORMALIZED_INPUT.push_back(rhs(curr_arg) == lhs(curr_arg));            \
      else                                                                     \
	NORMALIZED_INPUT.push_back(curr_arg);                                  \
    }                                                                          \
  } else {                                                                     \
    assert(NORMALIZED_EXPR.is_bool());                                         \
    if (func_kind(NORMALIZED_EXPR) == Z3_OP_EQ &&                              \
	rhs(NORMALIZED_EXPR).num_args() == 0)                                  \
      NORMALIZED_INPUT.push_back(rhs(NORMALIZED_EXPR) ==                       \
				 lhs(NORMALIZED_EXPR));                        \
    else                                                                       \
      NORMALIZED_INPUT.push_back(NORMALIZED_EXPR);                             \
  }

#define FLATTEN_PREDICATE(NUM_CONJS_INPUT, CONJUNCTION, SIDE)                  \
  NUM_CONJS_INPUT = CONJUNCTION.size();                                        \
  for (unsigned i = 0; i < NUM_CONJS_INPUT; i++)                               \
    flattenPredicate(CONJUNCTION[i], SIDE, NUM_CONJS_INPUT);

axdinterpolator::Preprocessor::Preprocessor(AXDSignature &sig,
					    z3::expr const &_input_part_a,
					    z3::expr const &_input_part_b)
    : sig(sig), fresh_num(0), n_IndexALocal(0), n_IndexBLocal(0),
      indexALocalIds({}), indexBLocalIds({}), length_index_vars({}),
      input_part_a(sig.ctx), input_part_b(sig.ctx), part_a_index_vars(sig.ctx),
      part_b_index_vars(sig.ctx), part_a_extra_length_eqs(sig.ctx),
      part_b_extra_length_eqs(sig.ctx), part_a_array_vars({}),
      part_b_array_vars({}), common_array_vars({}) {
#if _DEBUG_PREPROCESS_
  m_out << std::endl;
  m_out << "Raw Input a" << std::endl;
  m_out << _input_part_a << std::endl;
  m_out << "Raw Input b" << std::endl;
  m_out << _input_part_b << std::endl;
#endif

  NORMALIZE_INPUT(_input_part_a, input_part_a, normalizedExpr_a, PART_A);
  NORMALIZE_INPUT(_input_part_b, input_part_b, normalizedExpr_b, PART_B);

#if _DEBUG_PREPROCESS_
  m_out << std::endl;
  m_out << "Normalized Expression A" << std::endl;
  m_out << normalizedExpr_a << std::endl;
  m_out << "Normalized Vector Input A" << std::endl;
  m_out << input_part_a << std::endl;
  m_out << "Extra Length Eqs A" << std::endl;
  m_out << part_a_extra_length_eqs << std::endl;
  m_out << "Normalized Expression B" << std::endl;
  m_out << normalizedExpr_b << std::endl;
  m_out << "Normalized Vector Input B" << std::endl;
  m_out << input_part_b << std::endl;
  m_out << "Extra Length Eqs B" << std::endl;
  m_out << part_b_extra_length_eqs << std::endl;
#endif

  // empty_array elements are common to both
  // part A and part B
  for (auto const &_empty_array : sig.empty_array_es) {
    part_a_array_vars.insert(_empty_array);
    part_b_array_vars.insert(_empty_array);
    updateLengthIndexVars(_empty_array, true, PART_A);
    updateLengthIndexVars(_empty_array, true, PART_B);
  }

  unsigned current_conjs_in_input(0);
  FLATTEN_PREDICATE(current_conjs_in_input, input_part_a, PART_A);
  FLATTEN_PREDICATE(current_conjs_in_input, input_part_b, PART_B);

  // Initialize common_arrays_vars
  // to include common array variables
  // between part_a_arrays_vars and
  // part_b_arrays_vars
  for (auto const &a_array_var_entry : part_a_array_vars)
    for (auto const &b_array_var_entry : part_b_array_vars)
      if (a_array_var_entry.first == b_array_var_entry.first) {
	for (auto iterator_a = a_array_var_entry.second.begin();
	     iterator_a != a_array_var_entry.second.end(); ++iterator_a) {
	  if (inSet(*iterator_a, b_array_var_entry.second)) {
	    common_array_vars.insert(*iterator_a);
#if _DEBUG_PREPROCESS_
	    m_out << std::endl;
	    m_out << "Common array variable" << std::endl;
	    m_out << *iterator_a << std::endl;
#endif
	  }
	}
      }

#if _DEBUG_PREPROCESS_
  m_out << std::endl;
  m_out << "Flatten Part A" << std::endl;
  m_out << input_part_a << std::endl;
  m_out << "Flatten Part B" << std::endl;
  m_out << input_part_b << std::endl;
  m_out << "Index variables Part A" << std::endl;
  m_out << part_a_index_vars << std::endl;
  m_out << "Index variables Part B" << std::endl;
  m_out << part_b_index_vars << std::endl;
#endif

  return;
}
