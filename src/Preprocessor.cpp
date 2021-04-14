#include "Preprocess.h"
#include "z3++.h"

#define NORMALIZE_INPUT(OLD, NEW, TEMP_VAR)\
  z3::expr const & TEMP_VAR =\
  normalizeInput(OLD);\
  for(unsigned i = 0; i < TEMP_VAR.num_args(); ++i){\
    auto const & curr_arg = TEMP_VAR.arg(i);\
    if(curr_arg.decl().decl_kind() == Z3_OP_EQ\
        && rhs(curr_arg).num_args() == 0)\
    NEW.push_back(rhs(curr_arg) == lhs(curr_arg));\
    else\
    NEW.push_back(curr_arg);\
  }

#define FLATTEN_PREDICATE(NUM_CONJS_INPUT, CONJUNCTION, SIDE)\
  NUM_CONJS_INPUT = CONJUNCTION.size();\
  for(unsigned i = 0; i < NUM_CONJS_INPUT; i++)\
  flattenPredicate(CONJUNCTION[i], SIDE, NUM_CONJS_INPUT);

Preprocessor::Preprocessor(
    AXDSignature & sig, 
    z3::expr const & _input_part_a, 
    z3::expr const & _input_part_b):
  sig(sig),
  fresh_num(0), 
  input_part_a(sig.ctx), 
  input_part_b(sig.ctx),
  part_a_index_vars(sig.ctx), 
  part_b_index_vars(sig.ctx),
  part_a_array_vars({}), 
  part_b_array_vars({}), 
  common_array_vars({})
{
  NORMALIZE_INPUT(_input_part_a, input_part_a, conjunction_a);
  NORMALIZE_INPUT(_input_part_b, input_part_b, conjunction_b);

#if _DEBUG_PREPROCESS_
  m_out << "Conjunction a" << std::endl;
  m_out << conjunction_a << std::endl;
  m_out << "Conjunction b" << std::endl;
  m_out << conjunction_b << std::endl;
#endif

  // [TODO]: parametrize empty_array elements
  // empty_array is a common symbol
  part_a_array_vars.insert(sig.empty_array);
  part_b_array_vars.insert(sig.empty_array);

  unsigned current_conjs_in_input(0);
  FLATTEN_PREDICATE(current_conjs_in_input, input_part_a, PART_A);
  FLATTEN_PREDICATE(current_conjs_in_input, input_part_b, PART_B);

#if _DEBUG_PREPROCESS_
  m_out << "Part a " << input_part_a << std::endl;
  m_out << "Part b " << input_part_b << std::endl;
  m_out << "Arrays A-local" << std::endl;
  m_out << part_a_array_vars << std::endl;
  m_out << "Arrays B-local" << std::endl;
  m_out << part_b_array_vars << std::endl;
#endif

  // Compute Common Array Var Ids
  for(auto iterator_a = part_a_array_vars.begin(); 
      iterator_a != part_a_array_vars.end(); ++iterator_a){
    if(inSet(*iterator_a, part_b_array_vars))
      common_array_vars.insert(*iterator_a);
  }
}
