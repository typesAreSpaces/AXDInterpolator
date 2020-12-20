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
  index_vars(ctx),
  N_instantiation(0),
  current_instantiated_index_terms(ctx),
  index_var(ctx.constant("index_var", index_sort)),
  axiom_8(ctx), axiom_9(rd(empty_array, index_var) == undefined)
{
  assert(conjunction.decl().decl_kind() == Z3_OP_AND);
  
  std::string theory_signature(theory);
  if(theory_signature == "QF_TO"){
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

  for(auto const & x : initial_index_vars)
    index_vars.push_back(x);
  N_instantiate();

  // Setup axiom 8
  // Instantiate will all the current array elements
  // since this won't change
  z3::expr_vector instances_axiom_8(ctx);
  for(auto const & array_element : array_var_ids)
    instances_axiom_8.push_back(
        z3::implies(
          index_var < 0, 
          rd(array_element, index_var) == undefined)
        );
  axiom_8 = z3::mk_and(instances_axiom_8);

  initSaturation();
}

z3::expr StandardInput::orientBinPredicate(
    z3::expr const & bin_predicate){
  z3::func_decl predicate = bin_predicate.decl();
  if(lhs(bin_predicate).num_args() > rhs(bin_predicate).num_args())
    return predicate(rhs(bin_predicate), lhs(bin_predicate));
  return bin_predicate;
}

void StandardInput::N_instantiate(){
  current_instantiated_index_terms.resize(0);
  for(unsigned i = 0; i < index_vars.size(); i++)
    current_instantiated_index_terms.push_back(index_vars[i]);

  for(unsigned i = 0; i < N_instantiation; i++){
    // TODO: implement this
    for(unsigned j = 0; j < local_signature.size(); j++)
      m_out << local_signature[j] << std::endl;
  }

  return;
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
          rd(a, i) == e) 
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
            i > ctx.int_val(0), 
            rd(a, i) != rd(b, i))
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

  index_vars.push_back(_new_index);

  // TODO: implement/compare more heuristics
  // to increase N_instantiation

  // Heuristic # 1
  if(index_vars.size() % 5 == 0){
    N_instantiation++;
    N_instantiate();
  }

  // [11] predicates are processed in 
  // AXDInterpolant::SmtSolverSetup(z3::solver &);

  // Processing equations of the form diff_1(a, b) = k_1 
  // \land \dots \land diff_l(a, b) = k_l [13]
  if(min_dim < old_dim)
    part_2.push_back(
        _new_index == map_element->second[min_dim]
        );
  else{
    current_instantiated_index_terms.push_back(_new_index);

    diff_map.add(entry.first, entry.second, _new_index);
    // old_dim > 0 guarantees having a previous index
    if(old_dim > 0){
      auto const & _previous_index = map_element->second[old_dim - 1];

      // The following adds [14] predicates
      part_2.push_back(
          _previous_index >= _new_index
          );
      part_2.push_back(
          _new_index >= ctx.int_val(0)
          );

      // The following adds [15] predicates
      part_2.push_back(z3::implies(
            _previous_index > _new_index,
            rd(a, _previous_index) != rd(b, _previous_index)
            ));

      // The following adds [16] predicates
      part_2.push_back(z3::implies(
            _previous_index == _new_index,
            _previous_index == ctx.int_val(0)
            ));
    }
    // The following adds [17] predicates
    part_2.push_back(z3::implies(
          rd(a, _new_index) == rd(b, _new_index),
          _new_index == ctx.int_val(0)
          ));
  }

  // [18] predicates are processed in 
  // AXDInterpolant::SmtSolverSetup(z3::solver &);
}

void StandardInput::instantiate(z3::solver & s, z3::expr & e) const {
  for(auto const & index_term : current_instantiated_index_terms){
    z3::expr_vector from(ctx), to(ctx);
    from.push_back(index_var);
    to.push_back(index_term);
    s.add(e.substitute(from, to));
  }
}

void StandardInput::instantiate(std::ostream & os, z3::expr & e) const {
  for(auto const & index_term : current_instantiated_index_terms){
    z3::expr_vector from(ctx), to(ctx);
    from.push_back(index_var);
    to.push_back(index_term);
    os << (e.substitute(from, to)) << std::endl;
  }
}

std::ostream & operator << (std::ostream & os, StandardInput const & si){
  return (os 
      << "Part 1: " << si.part_1 
      << std::endl 
      << "Part 2: " << si.part_2);
}
