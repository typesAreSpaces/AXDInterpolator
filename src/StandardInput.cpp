#include "StandardInput.h"
#include <functional>
#include <ios>
#include <unordered_set>
#include <utility>

StandardInput::DiffTableEntry::DiffTableEntry(z3::context & ctx) : 
  index(ctx)
{
}

StandardInput::DiffTable::DiffTable(std::unordered_set<unsigned> const & array_var_ids, z3::context & ctx) : 
  m_table()
{
  for(auto const & x : array_var_ids)
    for(auto const & y : array_var_ids){
      if(x > y)
        m_table.insert(std::make_pair(
              std::pair<unsigned, unsigned>(x, y), 
              DiffTableEntry(ctx)));
      else if(y > x)
        m_table.insert(std::make_pair(
              std::pair<unsigned, unsigned>(y, x), 
              DiffTableEntry(ctx)));
    }
}

void StandardInput::DiffTable::add(
    z3::expr const & a, 
    z3::expr const & b, 
    z3::expr const & index){
  if(a.id() > b.id())
    add_aux(a, b, index);
  else if(b.id() > a.id())
    add_aux(b, a, index);
}

void StandardInput::DiffTable::add_aux(
    z3::expr const & a, 
    z3::expr const & b, 
    z3::expr const & index){
  auto table_entry = m_table.find(std::make_pair(a.id(), b.id()));
  if(table_entry == m_table.end()){
    std::cout << "Not found" << std::endl;
    return;
  }
  table_entry->second.index.push_back(index);
}

StandardInput::WriteVector::WriteVector() : 
  m_vector({})
{
}

void StandardInput::WriteVector::add(z3::expr const & a, z3::expr const & b, z3::expr const & i){
  m_vector.push_back(std::make_tuple(a, b, i));
}


StandardInput::StandardInput(z3::expr const & e, std::unordered_set<unsigned> const & array_var_ids) : 
  part_1(e.ctx()), part_2(e.ctx()),
  array_var_ids(array_var_ids),
  diff_table(array_var_ids, e.ctx())
{
  assert(e.decl().decl_kind() == Z3_OP_AND);
  for(unsigned i = 0; i < e.num_args(); i++){
    auto current_arg = e.arg(i);
    switch(current_arg.decl().decl_kind()){
      case Z3_OP_EQ: // ==
        if(current_arg.arg(0).decl().range().name().str() == "ArraySort" 
            || current_arg.arg(0).decl().name().str() == "diff" 
            || current_arg.arg(1).decl().name().str() == "diff" ){
          part_1.push_back(orientBinPredicate(current_arg));
        }
        else
          part_2.push_back(orientBinPredicate(current_arg));
        break;
      case Z3_OP_DISTINCT: // !=
      case Z3_OP_GE: // >=
      case Z3_OP_LE: // <=
      case Z3_OP_GT: // >
      case Z3_OP_LT: // <
        part_2.push_back(orientBinPredicate(current_arg));
        break;
      default:
        throw "Invalid formula.";
    }
  }
#if _DEBUG_STDINPUT_
  std::cout << "Done" << std::endl;
  std::cout << "Part 1: " << part_1 << std::endl;
  std::cout << "Part 2: " << part_2 << std::endl;
#endif

  for(auto const & equation : part_1){
    //std::cout << "Processing equation: " << equation << std::endl;

    auto f_name = equation.arg(1).decl().name().str();
    if(f_name == "wr"){
      write_vector.add(
          equation.arg(0), 
          equation.arg(1).arg(0), 
          equation.arg(1).arg(1));
    }
    else if(f_name == "diff"){
      //std::cout << "-****** equation in diff" << std::endl;
      //std::cout << equation << std::endl;
      diff_table.add(
          equation.arg(1).arg(0), 
          equation.arg(1).arg(1), 
          equation.arg(0));
    }
  }

  std::cout << "---------wot" << std::endl;
  for(auto const & x : diff_table.m_table){
    std::cout << x.first.first << " " << x.first.second << " -> ";
    std::cout << x.second.index << std::endl;
  }
  std::cout << "-hhhmmm" << std::endl;

  std::cout << "---------wot1" << std::endl;
  for(auto const & x : write_vector.m_vector){
    std::cout << std::get<0>(x) << ", " 
    << std::get<1>(x) << ", "
    << std::get<2>(x) << std::endl;
  }
  std::cout << "-hhhmmm" << std::endl;
  std::cout << std::endl << std::endl;
}

void StandardInput::update(){
}

z3::expr_vector const & StandardInput::getPart_1() const {
  return part_1;
}

z3::expr_vector const & StandardInput::getPart_2() const {
  return part_2;
}

z3::expr StandardInput::orientBinPredicate(z3::expr const & eq){
  if(eq.arg(0).num_args() > eq.arg(1).num_args())
    return eq.arg(1) == eq.arg(0);
  return eq;
}
