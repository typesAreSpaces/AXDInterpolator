#include "StandardInput.h"

StandardInput::DiffMap::DiffMapEntry::DiffMapEntry(z3::context & ctx) : 
  new_index_vars(ctx)
{
}

StandardInput::DiffMap::DiffMap(
    z3::context & ctx,
    z3_expr_set const & array_var_ids) : 
  m_map()
{
  for(auto const & x : array_var_ids)
    for(auto const & y : array_var_ids){
      if(x.id() > y.id())
        m_map.insert(std::make_pair(
              z3_expr_pair(x, y), 
              DiffMapEntry(ctx)));
      else if(y.id() > x.id())
        m_map.insert(std::make_pair(
              z3_expr_pair(y, x), 
              DiffMapEntry(ctx)));
    }
}

void StandardInput::DiffMap::add(
    z3::expr const & a, 
    z3::expr const & b, 
    z3::expr const & index){
  if(a.id() > b.id())
    add_aux(a, b, index);
  else if(b.id() > a.id())
    add_aux(b, a, index);
}

void StandardInput::DiffMap::add_aux(
    z3::expr const & a, 
    z3::expr const & b, 
    z3::expr const & index){
  auto table_entry = m_map.find(std::make_pair(a, b));
  if(table_entry == m_map.end()){
    throw "Problem @ StandardInput::DiffMap::add_aux."
      "Query a pair that should'nt be in the map";
    //std::cout << "The pair" << a << " " << b 
      //<< "was not found but still added" << std::endl;
    //table_entry->second = DiffMapEntry(a.ctx());
  }
  table_entry->second.new_index_vars.push_back(index);
}

StandardInput::WriteVector::WriteVector() : 
  m_vector({})
{
}

// Keeps track of the a, b, index in 
// wr-equations of the form a = wr(b, index, e)
void StandardInput::WriteVector::add(
    z3::expr const & a, 
    z3::expr const & b, 
    z3::expr const & index,
    z3::expr const & element){
  m_vector.push_back(std::make_tuple(a, b, index, element));
}

StandardInput::StandardInput(z3::expr const & e, 
    z3::expr_vector const & initial_index_vars,
    z3_expr_set const & array_var_ids) :
  AXDSignature(e.ctx()),
  diff_map(e.ctx(), array_var_ids),
  part_1(e.ctx()), part_2(e.ctx()), 
  initial_index_vars(initial_index_vars)
{
  assert(e.decl().decl_kind() == Z3_OP_AND);
  for(unsigned i = 0; i < e.num_args(); i++){
    auto current_arg = e.arg(i);
    switch(current_arg.decl().decl_kind()){
      case Z3_OP_EQ: // ==
        if(lhs(current_arg).decl().range().name().str() == "ArraySort" 
            || lhs(current_arg).decl().name().str() == "diff" 
            || rhs(current_arg).decl().name().str() == "diff")
          part_1.push_back(orientBinPredicate(current_arg));
        else
          part_2.push_back(orientBinPredicate(current_arg));
        continue;
      case Z3_OP_DISTINCT: // !=
      case Z3_OP_GE: // >=
      case Z3_OP_LE: // <=
      case Z3_OP_GT: // >
      case Z3_OP_LT: // <
        part_2.push_back(orientBinPredicate(current_arg));
        continue;
      default:
        throw "Problem @ StandardInput::StandardInput"
          "Invalid formula.";
    }
  }
#if _DEBUG_STDINPUT_
  std::cout << "Done" << std::endl;
  std::cout << "Part 1: " << part_1 << std::endl;
  std::cout << "Part 2: " << part_2 << std::endl;
#endif

  for(auto const & equation : part_1){
    //std::cout << "Processing equation: " << equation << std::endl;
    auto f_name = rhs(equation).decl().name().str();
    if(f_name == "wr"){
      write_vector.add(
          lhs(equation), 
          lhs(rhs(equation)), 
          rhs(rhs(equation)),
          rhs(equation).arg(2)
          );
    }
    else if(f_name == "diff"){
      //std::cout << "-****** equation in diff" << std::endl;
      //std::cout << equation << std::endl;
      diff_map.add(
          lhs(rhs(equation)), 
          rhs(rhs(equation)),
          lhs(equation));
    }
  }

#if _DEBUG_STDINPUT_
  std::cout << "Start DiffMap" << std::endl;
  for(auto const & x : diff_map.m_map){
    std::cout << x.first.first << " " << x.first.second << " -> ";
    std::cout << x.second.new_index_vars << std::endl;
  }
  std::cout << "End DiffMap" << std::endl;
  std::cout << "Start WriteVector" << std::endl;
  for(auto const & x : write_vector.m_vector){
    std::cout << std::get<0>(x) << ", " 
    << std::get<1>(x) << ", "
    << std::get<2>(x) << std::endl;
  }
  std::cout << "End WriteVector" << std::endl;
  std::cout << std::endl;
#endif

  initSaturation();
}

z3::expr StandardInput::orientBinPredicate(z3::expr const & eq){
  if(lhs(eq).num_args() > rhs(eq).num_args())
    return rhs(eq) == lhs(eq);
  return eq;
}

void StandardInput::initSaturation(){
  // Processing equations of the form a = wr(b, i, e)
  for(auto const & _4tuple : write_vector.m_vector){
    auto const & a = std::get<0>(_4tuple);
    auto const & b = std::get<1>(_4tuple);
    auto const & i = std::get<2>(_4tuple);
    auto const & e = std::get<3>(_4tuple);
    // The following adds rd(a, i) = e
    part_2.push_back(
        rd(a, i) == e
        );
    // The following instantiates the universally
    // quantified formula 
    // \forall h . h \neq i \rightarrow rd(a, b) = rd(b, h)
    for(auto const & h : initial_index_vars){
      part_2.push_back(
          z3::implies(h != i, rd(a, h) == rd(b, h))
          );
    }
  }
  
  // Processing equations of the form diff(a, b) = i
  // The following adds (25) predicates
  for(auto const & entry : diff_map.m_map){
    auto const & a   = entry.first.first;
    auto const & b   = entry.first.second;
    auto const & seq = entry.second;

    if(seq.new_index_vars.size() > 0){
      auto const & index = seq.new_index_vars[0];
      for(auto const & h : initial_index_vars)
        part_2.push_back(
            z3::implies(h > index,
              rd(a, h) == rd(b, h))
            );
      part_2.push_back(
          z3::implies(rd(a, index) == rd(b, index),
            index == ctx.int_val(0))
          );
    }
  }
}

void StandardInput::updateSaturation(){
  // The following adds (28) predicates
  // The following adds (29) predicates
  // The following adds (30) predicates
  // The following adds (31) predicates
}

