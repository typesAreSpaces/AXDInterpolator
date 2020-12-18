#include "StandardInput.h"

StandardInput::StandardInput(z3::expr const & conjunction, 
    z3::expr_vector & initial_index_vars,
    z3_expr_set const & array_var_ids,
    char const * theory) :
  AXDSignature(conjunction.ctx()),
  diff_map(conjunction.ctx(), array_var_ids),
  write_vector(),
  local_signature(ctx),
  part_1(conjunction.ctx()), part_2(conjunction.ctx()), 
  index_vars(initial_index_vars),
  // TODO: keep working with current_instantiated_index_terms
  current_instantiated_index_terms(ctx),
  index_var_h(ctx.constant("index_var_h", index_sort))
{
  assert(conjunction.decl().decl_kind() == Z3_OP_AND);

  std::string theory_signature(theory);
  if(theory_signature == "TO"){
  }
  if(theory_signature == "QF_IDL"){
    local_signature.push_back(pred);
    local_signature.push_back(succ);
  }
  if(theory_signature == "QF_UTVPI"){
    local_signature.push_back(pred);
    local_signature.push_back(succ);
    local_signature.push_back(neg);
  }
  if(theory_signature == "QF_LIA"){
    local_signature.push_back(pred);
    local_signature.push_back(succ);
    local_signature.push_back(neg);
    local_signature.push_back(add);
  }

  // Splitting input into part_1 and part_2
  // following the rules for "separated pairs".
  for(unsigned i = 0; i < conjunction.num_args(); i++){
    auto current_arg = conjunction.arg(i);
    switch(current_arg.decl().decl_kind()){
      case Z3_OP_EQ:       // ==
        if(sort_name(lhs(current_arg))     == "ArraySort" 
            || func_name(lhs(current_arg)) == "diff" 
            || func_name(rhs(current_arg)) == "diff")
          part_1.push_back(orientBinPredicate(current_arg));
        else
          part_2.push_back(orientBinPredicate(current_arg));
        continue;
      case Z3_OP_DISTINCT: // !=
      case Z3_OP_GE:       // >=
      case Z3_OP_LE:       // <=
      case Z3_OP_GT:       // >
      case Z3_OP_LT:       // <
        part_2.push_back(orientBinPredicate(current_arg));
        continue;
      default:
        throw "Problem @ StandardInput::StandardInput"
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

z3::expr StandardInput::orientBinPredicate(z3::expr const & eq){
  if(lhs(eq).num_args() > rhs(eq).num_args())
    return rhs(eq) == lhs(eq);
  return eq;
}

// TODO: rework this implementation
// using pseudo quantified formulas
void StandardInput::initSaturation(){
  // ------------------------------------------------
  // Processing equations of the form a = wr(b, i, e)
  // The following adds (24) predicates
  for(auto const & _4tuple : write_vector.m_vector){
    auto const & a = std::get<0>(_4tuple);
    auto const & b = std::get<1>(_4tuple);
    auto const & i = std::get<2>(_4tuple);
    auto const & e = std::get<3>(_4tuple);
    // The following adds rd(a, i) = e
    part_2.push_back(rd(a, i) == e);
    // The following instantiates 
    // the quantified formula 
    // \forall h . h \neq i \rightarrow rd(a, b) = rd(b, h)
    for(auto const & h : index_vars){
      part_2.push_back(
          z3::implies(h != i, rd(a, h) == rd(b, h))
          );
    }
  }
  // ------------------------------------------------

  // Processing equations of the form diff(a, b) = i
  // The following adds (25) predicates
  for(auto const & entry : diff_map.m_map){
    auto const & a   = entry.first.first;
    auto const & b   = entry.first.second;
    auto const & seq = entry.second;

    if(seq.size() > 0){
      auto const & index = seq[0];
      // The following instantiates
      // \forall h . h > i \rightarrow rd(a, h) = rd(b, h)
      for(auto const & h : index_vars)
        part_2.push_back(
            z3::implies(h > index, rd(a, h) == rd(b, h))
            );
      // The following adds
      // rd(a, i) = rd(b, i) \rightarrow i = 0
      part_2.push_back(
          z3::implies(rd(a, index) == rd(b, index), index == ctx.int_val(0))
          );
    }
  }
}

// TODO: rework this implementation
// using pseudo quantified formulas
void StandardInput::updateSaturation(
    DiffMap::z3_expr_pair const & entry,
    z3::expr const & _new_index, 
    unsigned min_dim){

  auto const & a = entry.first;
  auto const & b = entry.second;
  auto const & map_element = diff_map.m_map.find(entry);
  auto const & current_indices = map_element->second;
  unsigned old_dim = current_indices.size();

  index_vars.push_back(_new_index);

  // TODO: - Implement heuristic to upgrade
  // N for the N-instantiations.
  // - Implement N-instantations using
  // current_instantiated_index_terms.

  // ---------------------------------------------------
  // Processing equations of the form diff_1(a, b) = k_1 
  // \land \dots \land diff_l(a, b) = k_l
  // The following adds (26) predicates
  if(min_dim < old_dim)
    part_2.push_back(
        _new_index == map_element->second[min_dim]
        );
  else{
    diff_map.add(entry.first, entry.second, _new_index);
    // old_dim > 0 guarantees having a previous index
    if(old_dim > 0){
      auto const & _previous_index = map_element->second[old_dim - 1];

      // The following adds (27) predicates
      part_2.push_back(
          _previous_index >= _new_index
          );

      // The following adds (28) predicates
      part_2.push_back(z3::implies(
            _previous_index > _new_index,
            rd(a, _previous_index) != rd(b, _previous_index)
            ));

      // The following adds (29) predicates
      part_2.push_back(z3::implies(
            _previous_index == _new_index,
            _previous_index == ctx.int_val(0)
            ));
    }
    // The following adds (30) predicates
    part_2.push_back(z3::implies(
          rd(a, _new_index) == rd(b, _new_index),
          _new_index == ctx.int_val(0)
          ));
  }

  // The following adds (31) predicates
  for(auto const & h : index_vars){
    z3::expr_vector consequent_vector(ctx);
    consequent_vector.push_back(rd(a, h) == rd(b, h));
    for(auto const & k_ : current_indices)
      consequent_vector.push_back(h == k_);

    part_2.push_back(z3::implies(
          h > _new_index,
          z3::mk_or(consequent_vector)));
  }
  
  // The following adds (31) predicates for the 
  // previous quantifiers formulas effectively
  // updating them with _new_index
  for(auto const & diff_entry : diff_map.m_map){
    auto const & diff_a = diff_entry.first.first;
    auto const & diff_b = diff_entry.first.second;
    auto const & diff_seq = diff_entry.second;

    unsigned _i = 0, _size = diff_seq.size();
    z3::expr_vector accum_k_(ctx);
    for(; _i < _size; ++_i){
      auto const & k_ = diff_seq[_i];

      z3::expr_vector equalities_k(ctx);
      equalities_k.push_back(
          rd(diff_a, _new_index) == rd(diff_b, _new_index));
      for(unsigned _j = 0; _j < _i; ++_j)
        equalities_k.push_back(_new_index == accum_k_[_j]);

      part_2.push_back(z3::implies(
            _new_index > k_, 
            z3::mk_or(equalities_k)));

      accum_k_.push_back(k_);
    }
  }
  // ---------------------------------------------------

  // The following adds (24) predicates for the 
  // previous quantifiers formulas effectively
  // updating them with _new_index
  for(auto const & _4tuple : write_vector.m_vector){
    auto const & wr_a = std::get<0>(_4tuple);
    auto const & wr_b = std::get<1>(_4tuple);
    auto const & wr_i = std::get<2>(_4tuple);
    part_2.push_back(z3::implies(
          _new_index != wr_i,
          rd(wr_a, _new_index) == rd(wr_b, _new_index)
          ));
  }
}

std::ostream & operator << (std::ostream & os, StandardInput const & si){
  return (os 
      << "Part 1: " << si.part_1 
      << std::endl 
      << "Part 2: " << si.part_2);
}
