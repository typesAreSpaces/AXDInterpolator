#include "StandardInput.h"

StandardInput::StandardInput(
    AXDSignature const & sig,
    z3::expr_vector const & conjunction, 
    z3::expr_vector & initial_index_vars,
    Preprocessor::ArrayVars const & array_var_ids,
    unsigned _fresh_index) :
  sig(sig),
  s_fresh_index(_fresh_index),

  diff_map(array_var_ids, sig),
  write_vector(),
  instantiated_terms(sig, initial_index_vars),

  part_1(sig.ctx),
  part_2(sig.ctx), 

  index_var(sig.ctx.constant("index_var", sig.int_sort)),

  axiom_8(generateAxiom8(array_var_ids)), 
  axiom_9(generateAxiom9())
{
  // Split input into part_1 and part_2
  // following the rules for "separated pairs".
  for(auto const & current_arg : conjunction){
    if(func_kind(current_arg) == Z3_OP_UNINTERPRETED
        && current_arg.get_sort().is_bool()){
      part_2.push_back(current_arg);
      continue;
    }

    // Invariant from Preprocess.cpp
    assert(
        lhs(current_arg).num_args() <= 
        rhs(current_arg).num_args());
    switch(current_arg.decl().decl_kind()){
      case Z3_OP_EQ:       // ==
        {     
          auto const & _lhs = lhs(current_arg);
          auto const & _rhs = rhs(current_arg);
          auto const & lhs_sort = sort_name(_lhs);
          // [TODO]: rework the following conditional block
          if( // Covers equations of the form 
              // a = wr(b, i, e) or a = b 
              // when a is an array var
              lhs_sort == "ArraySort"  
              // Covers equations of the
              // form i = diff(a, b)
              || func_name(_lhs) == "diff" 
              || func_name(_rhs) == "diff"){

            // [10] predicates are added here
            if(_lhs.num_args() == 0 && _rhs.num_args() == 0){
              part_1.push_back(0 == sig.diff(_lhs, _rhs));
              part_2.push_back(
                  current_arg
                  && sig.rd(_lhs, 0) == sig.rd(_rhs, 0));
            }
            // Equations of the form i = diff(a, b), 
            // a = wr(b, i, e) will be processed 
            // in the for loop (*)
            else
              part_1.push_back(current_arg);
          }
          else
            part_2.push_back(current_arg);

          break;
        }
      case Z3_OP_DISTINCT: // !=
        // Invariant from Preprocess.cpp
        assert(lhs(current_arg).num_args() == 0
            && rhs(current_arg).num_args() == 0);
        //auto const & _lhs = lhs(current_arg);
        //auto const & _rhs = rhs(current_arg);
        //auto const & lhs_sort = sort_name(_lhs);
        if(sort_name(lhs(current_arg)) == "ArraySort"){
          auto const & f_index = fresh_index_constant();
          auto const & f_element1 = fresh_element_constant();
          auto const & f_element2 = fresh_element_constant();
          auto const & __lhs = lhs(current_arg);
          auto const & __rhs = rhs(current_arg);

          // [10] predicates are added here
          part_1.push_back(f_index 
              == sig.diff(__lhs, __rhs));
          part_1.push_back(f_element1 == sig.rd(__lhs, 0));
          part_1.push_back(f_element2 == sig.rd(__rhs, 0));
          part_2.push_back(f_index != 0 
              || f_element1 != f_element2);
        }
      case Z3_OP_GE:       // >=
      case Z3_OP_LE:       // <=
      case Z3_OP_GT:       // >
      case Z3_OP_LT:       // <
        part_2.push_back(current_arg);
        break;
      default:
        throw 
          "Problem @ "
          "StandardInput::StandardInput "
          "Invalid formula.";
    }
  }

#if _DEBUG_STDINPUT_
  m_out << "Part 1: " << part_1 << std::endl;
  m_out << "Part 2: " << part_2 << std::endl;
#endif

  // Setting up internal data structures
  // WriteVector and DiffMap
  for(auto const & equation : part_1){
#if _DEBUG_STDINPUT_
    m_out 
      << "Processing equation: " 
      << equation 
      << std::endl;
#endif
    auto f_name = func_name(rhs(equation));
    if(f_name == "wr"){
      write_vector.add(
          lhs(equation), 
          lhs(rhs(equation)), 
          rhs(rhs(equation)),
          rhs(equation).arg(2)
          );
    }
    if(f_name == "diff"){
      diff_map.add(
          lhs(rhs(equation)), 
          rhs(rhs(equation)),
          lhs(equation));
    }
  }

#if _DEBUG_STDINPUT_
  m_out << "Start - Printing current DiffMap" << std::endl;
  m_out << diff_map << std::endl;
  m_out << "End DiffMap" << std::endl;
  m_out << "Start - Printing current WriteVector" << std::endl;
  m_out << write_vector << std::endl;
  m_out << "End WriteVector" << std::endl;
  m_out << std::endl;
#endif
  
  initSaturation();
}

z3::expr StandardInput::fresh_index_constant(){
  return sig.ctx.constant((FRESH_INDEX_PREFIX 
        + std::to_string(s_fresh_index++)).c_str(), sig.int_sort);
}

// [TODO] parametrize this function
z3::expr StandardInput::fresh_element_constant(){
  return sig.ctx.constant((FRESH_ELEMENT_PREFIX 
        + std::to_string(s_fresh_index++)).c_str(), sig.element_sort);
}

void StandardInput::initSaturation(){
  // ------------------------------------------------
  // Processing equations of the form a = wr(b, i, e)
  // The following adds [11] predicates
  for(auto const & _4tuple : write_vector.m_vector){
    auto const & a = std::get<0>(_4tuple);
    auto const & i = std::get<2>(_4tuple);
    auto const & e = std::get<3>(_4tuple);

    // The following adds i >=0 \rightarrow rd(a, i) = e
    part_2.push_back(
        z3::implies(
          i >= 0, 
          sig.rd(a, i) == e) 
        );

    // [11] predicates are processed in 
    // AXDInterpolant::SmtSolverSetup(z3::solver &);
  }
  // ------------------------------------------------

  // -----------------------------------------------
  // Processing equations of the form diff(a, b) = i
  // The following adds [12] predicates
  for(auto const & entry : diff_map.m_map){
    auto const & a   = entry.first.first;
    auto const & b   = entry.first.second;
    auto const & seq = entry.second;

    if(seq.size() > 0){
      auto const & i = seq[0];

      // The following adds
      // i >= 0
      part_2.push_back(i >= 0);

      // [18] predicates are processed in 
      // AXDInterpolant::SmtSolverSetup(z3::solver &);

      // The following adds
      // i > 0 \rightarrow rd(a, i) \neq rd(b, i) 
      part_2.push_back(
          z3::implies(
            i > sig.ctx.int_val(0), 
            sig.rd(a, i) != sig.rd(b, i))
          );
    }
  }
  // -----------------------------------------------
}

void StandardInput::updateSaturation(
    DiffMap::z3_expr_pair const & entry,
    z3::expr const & _new_index, 
    unsigned min_dim){

  auto const & a = entry.first;
  auto const & b = entry.second;
  auto const & map_element = diff_map.m_map.find(entry);
  auto const & current_indices = map_element->second;
  unsigned old_dim = current_indices.size();

  instantiated_terms.add_var(_new_index);
  ++instantiated_terms;

  // [11] predicates are processed in 
  // AXDInterpolant::SmtSolverSetup(z3::solver &);

  // Processing equations of the form diff_1(a, b) = k_1 
  // \land \dots \land diff_l(a, b) = k_l [13]
  if(min_dim < old_dim)
    part_2.push_back(
        _new_index == map_element->second[min_dim]
        );
  else{
    diff_map.add(entry.first, entry.second, _new_index);
    // old_dim > 0 guarantees having a previous index
    if(old_dim > 0){
      auto const & _previous_index = map_element->second[old_dim - 1];

      // The following adds [14] predicates
      part_2.push_back(
          _previous_index >= _new_index
          );
      part_2.push_back(
          _new_index >= sig.ctx.int_val(0)
          );

      // The following adds [15] predicates
      part_2.push_back(z3::implies(
            _previous_index > _new_index,
            sig.rd(a, _previous_index) != sig.rd(b, _previous_index)
            ));

      // The following adds [16] predicates
      part_2.push_back(z3::implies(
            _previous_index == _new_index,
            _previous_index == sig.ctx.int_val(0)
            ));
    }
    // The following adds [17] predicates
    part_2.push_back(z3::implies(
          sig.rd(a, _new_index) == sig.rd(b, _new_index),
          _new_index == sig.ctx.int_val(0)
          ));
  }

  // [18] predicates are processed in 
  // AXDInterpolant::SmtSolverSetup(z3::solver &);
}

unsigned StandardInput::get_fresh_index() const {
  return s_fresh_index;
}

std::ostream & operator << (std::ostream & os, StandardInput const & si){
  return (os 
      << "Part 1: " << si.part_1 
      << std::endl 
      << "Part 2: " << si.part_2);
}
