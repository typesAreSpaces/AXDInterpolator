#include "Preprocess.h"
#include "z3++.h"

// NORMALIZED_INPUT normalizes RAW_INPUT (using normalizedInput)
// and breaks the resulting z3::expr into a z3::vector_expr
// containing the conjuncts of the normalized expr
// Additionally, reorients equations into a 'prelimary canonical
// normal form', i.e. of the form (= a (f a_1 ... a_{n-1}))
// where a is a constant
#define NORMALIZE_INPUT(RAW_INPUT, NORMALIZED_INPUT, NORMALIZED_EXPR)\
  z3::expr const & NORMALIZED_EXPR =\
  normalizeInput(RAW_INPUT);\
  if(func_kind(NORMALIZED_EXPR) == Z3_OP_AND){\
    for(unsigned i = 0; i < NORMALIZED_EXPR.num_args(); ++i){\
      auto const & curr_arg = NORMALIZED_EXPR.arg(i);\
      if(func_kind(curr_arg) == Z3_OP_EQ\
          && rhs(curr_arg).num_args() == 0)\
      NORMALIZED_INPUT.push_back(rhs(curr_arg) == lhs(curr_arg));\
      else\
      NORMALIZED_INPUT.push_back(curr_arg);\
    }\
  }\
  else { \
    assert(NORMALIZED_EXPR.is_bool());\
    if(func_kind(NORMALIZED_EXPR) == Z3_OP_EQ\
        && rhs(NORMALIZED_EXPR).num_args() == 0)\
    NORMALIZED_INPUT.push_back(rhs(NORMALIZED_EXPR) == lhs(NORMALIZED_EXPR));\
    else\
    NORMALIZED_INPUT.push_back(NORMALIZED_EXPR);\
  }

#define FLATTEN_PREDICATE(NUM_CONJS_INPUT, CONJUNCTION, SIDE)\
  NUM_CONJS_INPUT = CONJUNCTION.size();\
  for(unsigned i = 0; i < NUM_CONJS_INPUT; i++)\
  flattenPredicate(CONJUNCTION[i], SIDE, NUM_CONJS_INPUT);

axdinterpolator::Preprocessor::Preprocessor(
    AXDSignature & sig, 
    z3::expr const & _input_part_a, 
    z3::expr const & _input_part_b):
  sig(sig),
  fresh_num(0), 
  n_IndexALocal(0),
  n_IndexBLocal(0),
  indexALocalIds({}),
  indexBLocalIds({}),
  length_index_vars({}),
  input_part_a(sig.ctx), 
  input_part_b(sig.ctx),
  part_a_index_vars(sig.ctx), 
  part_b_index_vars(sig.ctx),
  part_a_array_vars({}), 
  part_b_array_vars({}), 
  common_array_vars({})
{
#if _DEBUG_PREPROCESS_
  m_out << "From Preprocessor: Input a" << std::endl;
  m_out << _input_part_a << std::endl;
  m_out << "From Preprocessor: Input b" << std::endl;
  m_out << _input_part_b << std::endl;
#endif

  NORMALIZE_INPUT(_input_part_a, input_part_a, normalizedExpr_a);
  NORMALIZE_INPUT(_input_part_b, input_part_b, normalizedExpr_b);

#if _DEBUG_PREPROCESS_
  m_out << "Conjunction a" << std::endl;
  m_out << normalizedExpr_a << std::endl;
  m_out << "Conjunction b" << std::endl;
  m_out << normalizedExpr_b << std::endl;
  m_out << "Before Flattening Part a " << input_part_a << std::endl;
  m_out << "Before Flattening Part b " << input_part_b << std::endl;
#endif

  // empty_array elements are common to both
  // part A and part B
  for(auto const & _empty_array : sig.empty_array_es){
    part_a_array_vars.insert(_empty_array);
    part_b_array_vars.insert(_empty_array);
    updateLengthIndexVars(_empty_array, true);
  }

  unsigned current_conjs_in_input(0);
  FLATTEN_PREDICATE(current_conjs_in_input, input_part_a, PART_A);
  FLATTEN_PREDICATE(current_conjs_in_input, input_part_b, PART_B);

  // Initialize common_arrays_vars
  // to include common array variables
  // between part_a_arrays_vars and
  // part_b_arrays_vars
  for(auto const & a_array_var_entry : part_a_array_vars)
    for(auto const & b_array_var_entry  : part_b_array_vars)
      if(a_array_var_entry.first == b_array_var_entry.first){
        for(auto iterator_a = a_array_var_entry.second.begin();
            iterator_a != a_array_var_entry.second.end(); 
            ++iterator_a){
          if(inSet(*iterator_a, b_array_var_entry.second))
            common_array_vars.insert(*iterator_a);
        }
      }

#if _DEBUG_PREPROCESS_
  if (common_array_vars.areCommonPairsAvaible()) {
    m_out << ">> There are common symbols" << std::endl;

    CircularPairIterator search_common_pair(common_array_vars, false);

    while (!search_common_pair.end()) {
      auto const &common_pair = *search_common_pair;
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
      search_common_pair.next();
    }
    m_out << "Ok" << std::endl;
  }
#endif

  return;
}
